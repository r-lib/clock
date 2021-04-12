# ------------------------------------------------------------------------------
# as_date_time()

test_that("can convert POSIXct -> POSIXct", {
  x <- new_datetime(0, tzone = "America/New_York")
  expect_identical(as_date_time(x), x)
})

test_that("can convert POSIXlt -> POSIXct", {
  expect <- new_datetime(0, tzone = "America/New_York")
  x <- as.POSIXlt(expect)
  expect_identical(as_date_time(x), expect)
})

test_that("integer POSIXct are normalized to double", {
  x <- 0L
  class(x) <- c("POSIXct", "POSIXt")
  attr(x, "tzone") <- "America/New_York"

  expect <- new_datetime(0, tzone = "America/New_York")

  expect_identical(as_date_time(x), expect)
})

test_that("can't accidentally supply `zone` to reinterpret date-time in new zone", {
  expect_snapshot_error(as_date_time(new_datetime(0), zone = "America/New_York"))
})

test_that("can convert Date -> POSIXct (Date assumed to be naive)", {
  x <- date_parse("2019-01-01")

  expect_identical(as_date_time(x, "America/New_York"), date_time_parse("2019-01-01 00:00:00", "America/New_York"))
  expect_identical(as_date_time(x, "Europe/London"), date_time_parse("2019-01-01 00:00:00", "Europe/London"))
})

test_that("can convert calendar -> POSIXct", {
  zone <- "America/New_York"
  expect_identical(as_date_time(year_month_day(1970, 1, 2), zone), new_datetime(104400, zone))
  expect_identical(as_date_time(year_quarter_day(1970, 1, 2), zone), new_datetime(104400, zone))
})

test_that("can convert sys-time -> POSIXct", {
  expect_identical(as_date_time(sys_seconds(0), "UTC"), new_datetime(0, "UTC"))
  expect_identical(as_date_time(sys_seconds(0), "America/New_York"), new_datetime(0, "America/New_York"))
})

test_that("can convert naive-time -> POSIXct", {
  expect_identical(as_date_time(naive_seconds(0), "UTC"), new_datetime(0, "UTC"))
  expect_identical(as_date_time(naive_seconds(0), "America/New_York"), new_datetime(18000, "America/New_York"))
})

test_that("can convert zoned-time -> POSIXct", {
  x <- as_zoned_time(naive_time_parse("2019-01-01 23:02:03"), "America/New_York")
  expect <- date_time_parse("2019-01-01 23:02:03", "America/New_York")
  expect_identical(as_date_time(x), expect)
})

test_that("can resolve nonexistent midnight issues for Date -> POSIXct", {
  # In Asia/Beirut, DST gap from 2021-03-27 23:59:59 -> 2021-03-28 01:00:00
  zone <- "Asia/Beirut"
  x <- as.Date("2021-03-28")

  expect_snapshot_error(as_date_time(x, zone), class = "clock_error_nonexistent_time")

  expect_identical(
    as_date_time(x, zone, nonexistent = "roll-forward"),
    as_date_time(as_naive_time(year_month_day(2021, 03, 28, 1)), zone)
  )
})

test_that("can resolve ambiguous midnight issues for Date -> POSIXct", {
  # In Asia/Amman, DST fallback from 2021-10-29 00:59:59 -> 2021-10-29 00:00:00
  zone <- "Asia/Amman"
  x <- as.Date("2021-10-29")

  expect_snapshot_error(as_date_time(x, zone), class = "clock_error_ambiguous_time")

  expect_identical(
    as_date_time(x, zone, ambiguous = "earliest"),
    date_time_parse_complete("2021-10-29 00:00:00+03:00[Asia/Amman]")
  )
  expect_identical(
    as_date_time(x, zone, ambiguous = "latest"),
    date_time_parse_complete("2021-10-29 00:00:00+02:00[Asia/Amman]")
  )
})

# ------------------------------------------------------------------------------
# as.POSIXct()

test_that("casting to POSIXct floors components more precise than seconds (#205)", {
  x <- naive_time_parse("1969-01-01 01:01:01.678", precision = "millisecond")

  expect_identical(
    as.POSIXct(x, "America/New_York"),
    date_time_parse("1969-01-01 01:01:01", "America/New_York")
  )
})

# ------------------------------------------------------------------------------
# as_weekday()

test_that("can convert to weekday", {
  x <- as.POSIXct("2019-01-01", tz = "America/New_York")
  expect_identical(as_weekday(x), weekday(3L))
  expect_identical(as_weekday(as.POSIXlt(x)), weekday(3L))
})

test_that("converting to weekday retains names", {
  x <- c(x = as.POSIXct("2019-01-01", tz = "America/New_York"))
  expect_named(as_weekday(x), names(x))
  expect_named(as_weekday(as.POSIXlt(x)), names(x))
})

# ------------------------------------------------------------------------------
# date_group()

test_that("can group by year/month/day/hour/minute/second", {
  x <- as.POSIXct("2019-01-01", "America/New_York")
  x <- add_days(x, 0:3)

  expect_identical(date_group(x, "day"), x)
  expect_identical(date_group(x, "day", n = 2), x[c(1, 1, 3, 3)])

  y <- as.POSIXct(c("2019-01-01", "2019-02-05", "2019-03-10"), "America/New_York")
  expect <- set_day(y, 1)

  expect_identical(date_group(y, "month"), expect)
  expect_identical(date_group(y, "month", n = 2), expect[c(1, 1, 3)])

  # Using year 0 as the starting point
  z <- as.POSIXct(c("0000-01-05", "0001-02-10", "0002-03-12", "0003-01-15", "0004-12-10"), "America/New_York")
  expect <- set_month(set_day(z, 1), 1)

  expect_identical(date_group(z, "year"), expect)
  expect_identical(date_group(z, "year", n = 3), expect[c(1, 1, 1, 4, 4)])

  w <- as.POSIXct("2019-01-01", "America/New_York")
  w <- add_hours(w, c(0, 1, 2, 4, 5))

  expect_identical(date_group(w, "hour", n = 2), w[c(1, 1, 3, 4, 4)])
})

test_that("can handle nonexistent times resulting from grouping", {
  x <- as.POSIXct("1970-04-26 03:30:00", "America/New_York")
  expect <- set_minute(x, 0)

  expect_snapshot_error(date_group(x, "hour", n = 2))
  expect_identical(date_group(x, "hour", n = 2, nonexistent = "roll-forward"), expect)
})

test_that("can't group by finer precisions", {
  x <- as.POSIXct("2019-01-01", "America/New_York")
  expect_snapshot_error(date_group(x, "nanosecond"))
})

test_that("can't group by non-year-month-day precisions", {
  x <- as.POSIXct("2019-01-01", "America/New_York")
  expect_snapshot_error(date_group(x, "quarter"))
})

# ------------------------------------------------------------------------------
# date_leap_year()

test_that("can detect leap years", {
  x <- c("2019-01-01", "2020-01-01", NA)
  x <- as.POSIXct(x, tz = "America/New_York")
  expect_identical(date_leap_year(x), c(FALSE, TRUE, NA))
})

# ------------------------------------------------------------------------------
# date_floor()

test_that("can floor by weeks", {
  x <- c("1970-01-01", "1970-01-07", "1970-01-08")
  x <- as.POSIXct(x, "America/New_York")
  expect_identical(date_floor(x, "week"), x[c(1, 1, 3)])
  expect_identical(date_floor(x, "week", n = 2), x[c(1, 1, 1)])
})

test_that("flooring can handle nonexistent times", {
  x <- as.POSIXct("1970-04-26 01:59:59", "America/New_York") + c(0, 1)
  expect <- as.POSIXct(c("1970-04-26 00:00:00", "1970-04-26 03:00:00"), "America/New_York")

  expect_snapshot_error(date_floor(x, "hour", n = 2))
  expect_identical(date_floor(x, "hour", n = 2, nonexistent = "roll-forward"), expect)
})

test_that("flooring can handle ambiguous times", {
  x <- as.POSIXct("1970-10-25 00:30:00", "America/New_York") + c(3600, 7200)
  expect <- as.POSIXct("1970-10-25 00:00:00", "America/New_York") + c(3600, 3600)

  expect_identical(date_floor(x, "hour", ambiguous = "earliest"), expect)
})

test_that("`origin` can be used", {
  origin <- as.POSIXct("1970-01-02", "America/New_York")
  x <- as.POSIXct(c("1970-01-01", "1970-01-02", "1970-01-03"), "America/New_York")
  expect <- as.POSIXct(c("1969-12-31", "1970-01-02", "1970-01-02"), "America/New_York")
  expect_identical(date_floor(x, "day", n = 2, origin = origin), expect)
})

test_that("`origin` is floored to the precision of `precision` with a potential warning before rounding", {
  origin <- as.POSIXct("1970-01-01 01:00:00", "America/New_York")
  x <- as.POSIXct(c("1970-01-01 00:00:00", "1970-01-02 00:00:00"), "America/New_York")

  expect_snapshot(date_floor(x, "day", origin = origin))
  expect_warning(date_floor(x, "day", origin = origin), class = "clock_warning_invalid_rounding_origin")
})

test_that("`origin` is validated", {
  zone <- "America/New_York"
  x <- as.POSIXct("2019-01-01", zone)
  expect_snapshot_error(date_floor(x, "day", origin = 1))
  expect_snapshot_error(date_floor(x, "day", origin = new_datetime(NA_real_, zone)))
  expect_snapshot_error(date_floor(x, "day", origin = new_datetime(Inf, zone)))
  expect_snapshot_error(date_floor(x, "day", origin = new_datetime(c(0, 1), zone)))
  expect_snapshot_error(date_floor(x, "day", origin = new_datetime(0, "")))
  expect_snapshot_error(date_floor(x, "day", origin = new_datetime(0, "America/Los_Angeles")))
})

# ------------------------------------------------------------------------------
# date_ceiling()

test_that("can ceiling", {
  x <- c("1970-01-01", "1970-01-07", "1970-01-08")
  x <- as.POSIXct(x, "America/New_York")
  expect_identical(date_ceiling(x, "week"), x[c(1, 3, 3)])
  expect_identical(date_ceiling(x, "week", n = 2), as.POSIXct(c("1970-01-01", "1970-01-15", "1970-01-15"), "America/New_York"))
})

# ------------------------------------------------------------------------------
# date_round()

test_that("can round", {
  x <- c("1970-01-01", "1970-01-03", "1970-01-05", "1970-01-08")
  x <- as.POSIXct(x, "America/New_York")
  expect_identical(date_round(x, "week"), x[c(1, 1, 4, 4)])
  expect_identical(date_round(x, "week", n = 2), as.POSIXct(c("1970-01-01", "1970-01-01", "1970-01-01", "1970-01-15"), "America/New_York"))
})

# ------------------------------------------------------------------------------
# date_weekday_factor()

test_that("can convert to a weekday factor", {
  x <- as.POSIXct("2019-01-01", tz = "America/New_York") + 0:6

  expect_identical(
    date_weekday_factor(x),
    weekday_factor(as_weekday(x)),
  )
  expect_identical(
    date_weekday_factor(x, labels = "fr", abbreviate = FALSE, encoding = "iso"),
    weekday_factor(as_weekday(x), labels = "fr", abbreviate = FALSE, encoding = "iso"),
  )
})

# ------------------------------------------------------------------------------
# date_month_factor()

test_that("can convert to a month factor", {
  x <- add_months(as.POSIXct("2019-01-01", tz = "America/New_York"), 0:11)

  expect_identical(
    date_month_factor(x, labels = "fr", abbreviate = TRUE),
    calendar_month_factor(as_year_month_day(x), labels = "fr", abbreviate = TRUE),
  )
})

# ------------------------------------------------------------------------------
# date_format()

test_that("can format date-times", {
  x <- as.POSIXct("2018-12-31 23:59:59", "America/New_York")
  format <- test_all_formats()

  expect_snapshot_output(
    cat(date_format(x, format = format))
  )
  expect_snapshot_output(
    cat(date_format(x, format = format, locale = clock_locale("fr")))
  )
})

# ------------------------------------------------------------------------------
# date_zone()

test_that("can get the zone of a POSIXt", {
  ct <- as.POSIXct("2019-01-01", "America/New_York")
  lt <- as.POSIXlt(ct)

  expect_identical(date_zone(ct), "America/New_York")
  expect_identical(date_zone(lt), "America/New_York")
})

# ------------------------------------------------------------------------------
# date_set_zone()

test_that("can set the zone of a POSIXt", {
  ct <- as.POSIXct("2019-01-01", "America/New_York")
  lt <- as.POSIXlt(ct)

  expect <- as.POSIXct("2018-12-31 21:00:00", "America/Los_Angeles")

  expect_identical(date_set_zone(ct, "America/Los_Angeles"), expect)
  expect_identical(date_set_zone(lt, "America/Los_Angeles"), expect)
})

# ------------------------------------------------------------------------------
# date_time_parse()

test_that("can parse into a POSIXct", {
  expect_identical(
    date_time_parse("2019-12-31 23:59:59", "America/New_York"),
    as.POSIXct("2019-12-31 23:59:59", tz = "America/New_York")
  )
})

test_that("can resolve ambiguity and nonexistent times", {
  expect_snapshot_error(date_time_parse("1970-04-26 02:30:00", "America/New_York"))

  expect_identical(
    date_time_parse("1970-04-26 02:30:00", "America/New_York", nonexistent = "roll-forward"),
    date_time_parse("1970-04-26 03:00:00", "America/New_York")
  )

  expect_snapshot_error(date_time_parse("1970-10-25 01:30:00", "America/New_York"))

  expect_identical(
    date_time_parse("1970-10-25 01:30:00", "America/New_York", ambiguous = "earliest"),
    add_seconds(date_time_parse("1970-10-25 00:30:00", "America/New_York"), 3600)
  )
  expect_identical(
    date_time_parse("1970-10-25 01:30:00", "America/New_York", ambiguous = "latest"),
    add_seconds(date_time_parse("1970-10-25 00:30:00", "America/New_York"), 7200)
  )
})

test_that("failure to parse throws a warning", {
  expect_warning(date_time_parse("foo", "America/New_York"), class = "clock_warning_parse_failures")
  expect_snapshot(date_time_parse("foo", "America/New_York"))
})

# ------------------------------------------------------------------------------
# date_time_parse_complete()

test_that("can parse into a POSIXct", {
  expect_identical(
    date_time_parse_complete("2019-12-31 23:59:59-05:00[America/New_York]"),
    as.POSIXct("2019-12-31 23:59:59", tz = "America/New_York")
  )
})

test_that("ambiguity is resolved through the string", {
  expect_identical(
    date_time_parse_complete("1970-10-25 01:30:00-04:00[America/New_York]"),
    add_seconds(date_time_parse("1970-10-25 00:30:00", "America/New_York"), 3600)
  )
  expect_identical(
    date_time_parse_complete("1970-10-25 01:30:00-05:00[America/New_York]"),
    add_seconds(date_time_parse("1970-10-25 00:30:00", "America/New_York"), 7200)
  )
})

test_that("throws warning on failed parses", {
  expect_warning(date_time_parse_complete("foo"), class = "clock_warning_parse_failures")
  expect_snapshot(date_time_parse_complete("foo"))
})

# ------------------------------------------------------------------------------
# date_time_parse_abbrev()

test_that("can parse into a POSIXct using the abbrev", {
  expect_identical(
    date_time_parse_abbrev("2019-01-01 01:02:03 EST", "America/New_York"),
    as.POSIXct(as_naive_time(year_month_day(2019, 1, 1, 1, 2, 3)), "America/New_York")
  )
})

test_that("ambiguity is resolved through the string", {
  expect_identical(
    date_time_parse_abbrev("1970-10-25 01:30:00 EDT", "America/New_York"),
    add_seconds(date_time_parse("1970-10-25 00:30:00", "America/New_York"), 3600)
  )
  expect_identical(
    date_time_parse_abbrev("1970-10-25 01:30:00 EST", "America/New_York"),
    add_seconds(date_time_parse("1970-10-25 00:30:00", "America/New_York"), 7200)
  )
})

test_that("abbrev - throws warning on failed parses", {
  expect_warning(date_time_parse_abbrev("foo", "America/New_York"), class = "clock_warning_parse_failures")
  expect_snapshot(date_time_parse_abbrev("foo", "America/New_York"))
})

# ------------------------------------------------------------------------------
# date_shift()

test_that("can shift date times", {
  x <- date_time_parse("2019-01-01 01:30:35", "America/New_York")
  x <- x + c(0, 1)

  monday <- weekday(clock_weekdays$monday)
  tuesday <- weekday(clock_weekdays$tuesday)

  expect_identical(
    date_shift(x, monday),
    add_days(x, 6)
  )
  expect_identical(
    date_shift(x, monday, which = "previous"),
    add_days(x, -1)
  )
  expect_identical(
    date_shift(x, tuesday, boundary = "advance"),
    add_days(x, 7)
  )
})

test_that("`ambiguous = x` retains the offset of `x` if applicable", {
  x <- date_time_parse("1970-10-25 01:30:00", "America/New_York", ambiguous = "earliest")
  x <- x + c(0, 3600)

  expect_snapshot_error(date_shift(x, as_weekday(x), ambiguous = "error"))
  expect_identical(date_shift(x, as_weekday(x)), x)
})

# ------------------------------------------------------------------------------
# date_time_build()

test_that("can build a POSIXct", {
  zone <- "America/New_York"

  expect_identical(date_time_build(2019, zone = zone), as.POSIXct("2019-01-01", tz = zone))
  expect_identical(date_time_build(2020, 2, 3, 4, 5, 6, zone = zone), as.POSIXct("2020-02-03 04:05:06", tz = zone))
})

test_that("`zone` is required", {
  expect_snapshot_error(date_time_build(2019))
})

test_that("can handle invalid dates", {
  zone <- "America/New_York"

  expect_snapshot_error(date_time_build(2019, 1:12, 31, zone = zone))

  expect_identical(
    date_time_build(2019, 1:12, 31, zone = zone, invalid = "previous-day"),
    date_time_build(2019, 1:12, "last", zone = zone)
  )
})

test_that("can handle nonexistent times", {
  zone <- "America/New_York"

  expect_snapshot_error(date_time_build(1970, 4, 26, 2, 30, zone = zone))

  expect_identical(
    date_time_build(1970, 4, 26, 2, 30, zone = zone, nonexistent = "roll-forward"),
    date_time_build(1970, 4, 26, 3, zone = zone)
  )
})

test_that("can handle ambiguous times", {
  zone <- "America/New_York"

  expect_snapshot_error(date_time_build(1970, 10, 25, 1, 30, zone = zone))

  expect_identical(
    date_time_build(1970, 10, 25, 1, 30, zone = zone, ambiguous = "earliest"),
    date_time_parse_complete("1970-10-25 01:30:00-04:00[America/New_York]")
  )
  expect_identical(
    date_time_build(1970, 10, 25, 1, 30, zone = zone, ambiguous = "latest"),
    date_time_parse_complete("1970-10-25 01:30:00-05:00[America/New_York]")
  )
})

# ------------------------------------------------------------------------------
# date_now()

test_that("can get the current date-time", {
  x <- date_now("America/New_York")
  expect_length(x, 1)
  expect_s3_class(x, "POSIXct")
})

# ------------------------------------------------------------------------------
# vec_arith()

test_that("<posixt> op <duration>", {
  zone <- "America/New_York"

  new_posixlt <- function(x, zone) {
    as.POSIXlt(new_datetime(x, zone))
  }

  expect_identical(vec_arith("+", new_datetime(0, zone), duration_years(1)), new_datetime(31536000, zone))
  expect_identical(vec_arith("+", new_posixlt(0, zone), duration_years(1)), new_datetime(31536000, zone))

  expect_identical(vec_arith("-", new_datetime(0, zone), duration_years(1)), new_datetime(-31536000, zone))
  expect_identical(vec_arith("-", new_posixlt(0, zone), duration_years(1)), new_datetime(-31536000, zone))

  expect_identical(vec_arith("+", new_datetime(0, zone), duration_seconds(1)), new_datetime(1, zone))
  expect_identical(vec_arith("+", new_posixlt(0, zone), duration_seconds(1)), new_datetime(1, zone))

  expect_snapshot_error(vec_arith("+", new_datetime(0, zone), duration_milliseconds(1)))
  expect_snapshot_error(vec_arith("+", new_posixlt(0, zone), duration_milliseconds(1)))

  expect_snapshot_error(vec_arith("*", new_datetime(0, zone), duration_years(1)))
  expect_snapshot_error(vec_arith("*", new_posixlt(0, zone), duration_years(1)))
})

test_that("<duration> op <posixt>", {
  zone <- "America/New_York"

  new_posixlt <- function(x, zone) {
    as.POSIXlt(new_datetime(x, zone))
  }

  expect_identical(vec_arith("+", duration_years(1), new_datetime(0, zone)), new_datetime(31536000, zone))
  expect_identical(vec_arith("+", duration_years(1), new_posixlt(0, zone)), new_datetime(31536000, zone))

  expect_identical(vec_arith("+", duration_seconds(1), new_datetime(0, zone)), new_datetime(1, zone))
  expect_identical(vec_arith("+", duration_seconds(1), new_posixlt(0, zone)), new_datetime(1, zone))

  expect_snapshot_error(vec_arith("-", duration_years(1), new_datetime(0, zone)))
  expect_snapshot_error(vec_arith("-", duration_years(1), new_posixlt(0, zone)))

  expect_snapshot_error(vec_arith("+", duration_milliseconds(1), new_datetime(0, zone)))
  expect_snapshot_error(vec_arith("+", duration_milliseconds(1), new_posixlt(0, zone)))

  expect_snapshot_error(vec_arith("*", duration_years(1), new_datetime(0, zone)))
  expect_snapshot_error(vec_arith("*", duration_years(1), new_posixlt(0, zone)))
})
