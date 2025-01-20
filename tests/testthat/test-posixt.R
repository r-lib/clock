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
  expect_snapshot(
    error = TRUE,
    as_date_time(new_datetime(0), zone = "America/New_York")
  )
})

test_that("can convert Date -> POSIXct (Date assumed to be naive)", {
  x <- date_parse("2019-01-01")

  expect_identical(
    as_date_time(x, "America/New_York"),
    date_time_parse("2019-01-01 00:00:00", "America/New_York")
  )
  expect_identical(
    as_date_time(x, "Europe/London"),
    date_time_parse("2019-01-01 00:00:00", "Europe/London")
  )
})

test_that("can convert calendar -> POSIXct", {
  zone <- "America/New_York"
  expect_identical(
    as_date_time(year_month_day(1970, 1, 2), zone),
    new_datetime(104400, zone)
  )
  expect_identical(
    as_date_time(year_quarter_day(1970, 1, 2), zone),
    new_datetime(104400, zone)
  )
})

test_that("can convert sys-time -> POSIXct", {
  expect_identical(as_date_time(sys_seconds(0), "UTC"), new_datetime(0, "UTC"))
  expect_identical(
    as_date_time(sys_seconds(0), "America/New_York"),
    new_datetime(0, "America/New_York")
  )
})

test_that("can convert naive-time -> POSIXct", {
  expect_identical(as_date_time(naive_seconds(0), "UTC"), new_datetime(0, "UTC"))
  expect_identical(
    as_date_time(naive_seconds(0), "America/New_York"),
    new_datetime(18000, "America/New_York")
  )
})

test_that("can convert zoned-time -> POSIXct", {
  x <- as_zoned_time(naive_time_parse("2019-01-01T23:02:03"), "America/New_York")
  expect <- date_time_parse("2019-01-01 23:02:03", "America/New_York")
  expect_identical(as_date_time(x), expect)
})

test_that("can resolve nonexistent midnight issues for Date -> POSIXct", {
  # In Asia/Beirut, DST gap from 2021-03-27 23:59:59 -> 2021-03-28 01:00:00
  zone <- "Asia/Beirut"
  x <- as.Date("2021-03-28")

  expect_snapshot({
    (expect_error(as_date_time(x, zone), class = "clock_error_nonexistent_time"))
  })

  expect_identical(
    as_date_time(x, zone, nonexistent = "roll-forward"),
    as_date_time(as_naive_time(year_month_day(2021, 03, 28, 1)), zone)
  )
})

test_that("can resolve ambiguous midnight issues for Date -> POSIXct", {
  # In Asia/Amman, DST fallback from 2021-10-29 00:59:59 -> 2021-10-29 00:00:00
  zone <- "Asia/Amman"
  x <- as.Date("2021-10-29")

  expect_snapshot({
    (expect_error(as_date_time(x, zone), class = "clock_error_ambiguous_time"))
  })

  expect_identical(
    as_date_time(x, zone, ambiguous = "earliest"),
    date_time_parse_complete("2021-10-29T00:00:00+03:00[Asia/Amman]")
  )
  expect_identical(
    as_date_time(x, zone, ambiguous = "latest"),
    date_time_parse_complete("2021-10-29T00:00:00+02:00[Asia/Amman]")
  )
})

# ------------------------------------------------------------------------------
# as.POSIXct()

test_that("casting to POSIXct floors components more precise than seconds (#205)", {
  x <- naive_time_parse("1969-01-01T01:01:01.678", precision = "millisecond")

  expect_identical(
    as.POSIXct(x, "America/New_York"),
    date_time_parse("1969-01-01 01:01:01", "America/New_York")
  )
})

test_that("casting to POSIXct with time point less precise than seconds works (#278)", {
  x <- as_naive_time(year_month_day(2019, 1, 1, 1))

  expect_identical(
    as.POSIXct(x, "America/New_York"),
    date_time_parse("2019-01-01 01:00:00", "America/New_York")
  )

  x <- as_sys_time(year_month_day(2019, 1, 1, 1))

  expect_identical(
    as.POSIXct(x, "America/New_York"),
    date_time_parse("2018-12-31 20:00:00", "America/New_York")
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

test_that("can group by year (#312)", {
  # Skip on 32-bit machines
  # https://github.com/r-lib/clock/issues/312#issuecomment-1507528450
  skip_if(
    condition = .Machine$sizeof.pointer < 8L,
    message = "Likely an integer overflow/underflow bug in base R here"
  )

  # Using year 0 as the starting point
  x <- as.POSIXct(
    c("0000-01-05", "0001-02-10", "0002-03-12", "0003-01-15", "0004-12-10"),
    "America/New_York"
  )
  expect <- set_month(set_day(x, 1), 1)

  expect_identical(date_group(x, "year"), expect)
  expect_identical(date_group(x, "year", n = 3), expect[c(1, 1, 1, 4, 4)])
})

test_that("can group by month/day/hour/minute/second", {
  x <- as.POSIXct("2019-01-01", "America/New_York")
  x <- add_days(x, 0:3)

  expect_identical(date_group(x, "day"), x)
  expect_identical(date_group(x, "day", n = 2), x[c(1, 1, 3, 3)])

  y <- as.POSIXct(c("2019-01-01", "2019-02-05", "2019-03-10"), "America/New_York")
  expect <- set_day(y, 1)

  expect_identical(date_group(y, "month"), expect)
  expect_identical(date_group(y, "month", n = 2), expect[c(1, 1, 3)])

  z <- as.POSIXct("2019-01-01", "America/New_York")
  z <- add_hours(z, c(0, 1, 2, 4, 5))

  expect_identical(date_group(z, "hour", n = 2), z[c(1, 1, 3, 4, 4)])
})

test_that("can handle nonexistent times resulting from grouping", {
  x <- as.POSIXct("1970-04-26 03:30:00", "America/New_York")
  expect <- set_minute(x, 0)

  expect_snapshot(error = TRUE, date_group(x, "hour", n = 2))
  expect_identical(date_group(x, "hour", n = 2, nonexistent = "roll-forward"), expect)
})

test_that("can't group by finer precisions", {
  x <- as.POSIXct("2019-01-01", "America/New_York")
  expect_snapshot(error = TRUE, date_group(x, "nanosecond"))
})

test_that("can't group by non-year-month-day precisions", {
  x <- as.POSIXct("2019-01-01", "America/New_York")
  expect_snapshot(error = TRUE, date_group(x, "quarter"))
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
  expect <- as.POSIXct(
    c("1970-04-26 00:00:00", "1970-04-26 03:00:00"),
    "America/New_York"
  )

  expect_snapshot(error = TRUE, date_floor(x, "hour", n = 2))
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
  expect_warning(
    date_floor(x, "day", origin = origin),
    class = "clock_warning_invalid_rounding_origin"
  )
})

test_that("`origin` is validated", {
  zone <- "America/New_York"
  x <- as.POSIXct("2019-01-01", zone)
  expect_snapshot(error = TRUE, date_floor(x, "day", origin = 1))
  expect_snapshot(
    error = TRUE,
    date_floor(x, "day", origin = new_datetime(NA_real_, zone))
  )
  expect_snapshot(error = TRUE, date_floor(x, "day", origin = new_datetime(Inf, zone)))
  expect_snapshot(
    error = TRUE,
    date_floor(x, "day", origin = new_datetime(c(0, 1), zone))
  )
  expect_snapshot(error = TRUE, date_floor(x, "day", origin = new_datetime(0, "")))
  expect_snapshot(
    error = TRUE,
    date_floor(x, "day", origin = new_datetime(0, "America/Los_Angeles"))
  )
})

# ------------------------------------------------------------------------------
# date_ceiling()

test_that("can ceiling", {
  x <- c("1970-01-01", "1970-01-07", "1970-01-08")
  x <- as.POSIXct(x, "America/New_York")
  expect_identical(date_ceiling(x, "week"), x[c(1, 3, 3)])
  expect_identical(
    date_ceiling(x, "week", n = 2),
    as.POSIXct(c("1970-01-01", "1970-01-15", "1970-01-15"), "America/New_York")
  )
})

# ------------------------------------------------------------------------------
# date_round()

test_that("can round", {
  x <- c("1970-01-01", "1970-01-03", "1970-01-05", "1970-01-08")
  x <- as.POSIXct(x, "America/New_York")
  expect_identical(date_round(x, "week"), x[c(1, 1, 4, 4)])
  expect_identical(
    date_round(x, "week", n = 2),
    as.POSIXct(
      c("1970-01-01", "1970-01-01", "1970-01-01", "1970-01-15"),
      "America/New_York"
    )
  )
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

test_that("default format is correct", {
  x <- date_time_parse("2019-01-01 00:00:00", "America/New_York")
  expect_snapshot(date_format(x))
})

test_that("can format date-times", {
  x <- as.POSIXct("2018-12-31 23:59:59", "America/New_York")
  formats <- test_all_formats()

  expect_snapshot(
    vapply(
      X = formats,
      FUN = function(format) date_format(x, format = format),
      FUN.VALUE = character(1)
    )
  )
  expect_snapshot(
    vapply(
      X = formats,
      FUN = function(format)
        date_format(x, format = format, locale = clock_locale("fr")),
      FUN.VALUE = character(1)
    )
  )
})

# ------------------------------------------------------------------------------
# date_time_zone()

test_that("can get the zone of a POSIXt", {
  ct <- as.POSIXct("2019-01-01", "America/New_York")
  lt <- as.POSIXlt(ct)

  expect_identical(date_time_zone(ct), "America/New_York")
  expect_identical(date_time_zone(lt), "America/New_York")
})

test_that("`date_time_zone()` has a special error on Dates", {
  expect_snapshot(error = TRUE, date_time_zone(new_date(0)))
})

test_that("`date_time_zone()` validates `x`", {
  expect_snapshot(error = TRUE, date_time_zone(1))
})

# ------------------------------------------------------------------------------
# date_time_set_zone()

test_that("can set the zone of a POSIXt", {
  ct <- as.POSIXct("2019-01-01", "America/New_York")
  lt <- as.POSIXlt(ct)

  expect <- as.POSIXct("2018-12-31 21:00:00", "America/Los_Angeles")

  expect_identical(date_time_set_zone(ct, "America/Los_Angeles"), expect)
  expect_identical(date_time_set_zone(lt, "America/Los_Angeles"), expect)
})

test_that("`date_time_set_zone()` has a special error on Dates", {
  expect_snapshot(error = TRUE, date_time_set_zone(new_date(), "UTC"))
})

test_that("`date_time_set_zone()` validates `x`", {
  expect_snapshot(error = TRUE, date_time_set_zone(1, "UTC"))
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
  expect_snapshot(
    error = TRUE,
    date_time_parse("1970-04-26 02:30:00", "America/New_York")
  )

  expect_identical(
    date_time_parse(
      "1970-04-26 02:30:00",
      "America/New_York",
      nonexistent = "roll-forward"
    ),
    date_time_parse("1970-04-26 03:00:00", "America/New_York")
  )

  expect_snapshot(
    error = TRUE,
    date_time_parse("1970-10-25 01:30:00", "America/New_York")
  )

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
  expect_warning(
    date_time_parse("foo", "America/New_York"),
    class = "clock_warning_parse_failures"
  )
  expect_snapshot(date_time_parse("foo", "America/New_York"))
})

# ------------------------------------------------------------------------------
# date_time_parse_complete()

test_that("can parse into a POSIXct", {
  expect_identical(
    date_time_parse_complete("2019-12-31T23:59:59-05:00[America/New_York]"),
    as.POSIXct("2019-12-31 23:59:59", tz = "America/New_York")
  )
})

test_that("ambiguity is resolved through the string", {
  expect_identical(
    date_time_parse_complete("1970-10-25T01:30:00-04:00[America/New_York]"),
    add_seconds(date_time_parse("1970-10-25 00:30:00", "America/New_York"), 3600)
  )
  expect_identical(
    date_time_parse_complete("1970-10-25T01:30:00-05:00[America/New_York]"),
    add_seconds(date_time_parse("1970-10-25 00:30:00", "America/New_York"), 7200)
  )
})

test_that("throws warning on failed parses", {
  expect_warning(
    date_time_parse_complete("foo"),
    class = "clock_warning_parse_failures"
  )
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
  expect_warning(
    date_time_parse_abbrev("foo", "America/New_York"),
    class = "clock_warning_parse_failures"
  )
  expect_snapshot(date_time_parse_abbrev("foo", "America/New_York"))
})

# ------------------------------------------------------------------------------
# date_time_parse_RFC_3339()

test_that("can parse default RFC 3339 format", {
  x <- "2019-01-01T00:00:01Z"
  expect_identical(
    date_time_parse_RFC_3339(x),
    date_time_build(2019, 1, 1, 0, 0, 1, zone = "UTC")
  )
})

test_that("`offset` is passed through", {
  x <- "2019-01-01T00:00:01-03:30"
  expect_identical(
    date_time_parse_RFC_3339(x, offset = "%Ez"),
    as_date_time(sys_time_parse_RFC_3339(x, offset = "%Ez"), zone = "UTC")
  )
})

test_that("`separator` is passed through", {
  x <- "2019-01-01 00:00:01Z"
  expect_identical(
    date_time_parse_RFC_3339(x, separator = " "),
    as_date_time(sys_time_parse_RFC_3339(x, separator = " "), zone = "UTC")
  )
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
  x <- date_time_parse(
    "1970-10-25 01:30:00",
    "America/New_York",
    ambiguous = "earliest"
  )
  x <- x + c(0, 3600)

  expect_snapshot(error = TRUE, date_shift(x, as_weekday(x), ambiguous = "error"))
  expect_identical(date_shift(x, as_weekday(x)), x)
})

# ------------------------------------------------------------------------------
# date_time_build()

test_that("can build a POSIXct", {
  zone <- "America/New_York"

  expect_identical(
    date_time_build(2019, zone = zone),
    as.POSIXct("2019-01-01", tz = zone)
  )
  expect_identical(
    date_time_build(2020, 2, 3, 4, 5, 6, zone = zone),
    as.POSIXct("2020-02-03 04:05:06", tz = zone)
  )
})

test_that("`zone` is required", {
  expect_snapshot(error = TRUE, date_time_build(2019))
})

test_that("can handle invalid dates", {
  zone <- "America/New_York"

  expect_snapshot(error = TRUE, date_time_build(2019, 1:12, 31, zone = zone))

  expect_identical(
    date_time_build(2019, 1:12, 31, zone = zone, invalid = "previous-day"),
    date_time_build(2019, 1:12, "last", zone = zone)
  )
})

test_that("can handle nonexistent times", {
  zone <- "America/New_York"

  expect_snapshot(error = TRUE, date_time_build(1970, 4, 26, 2, 30, zone = zone))

  expect_identical(
    date_time_build(1970, 4, 26, 2, 30, zone = zone, nonexistent = "roll-forward"),
    date_time_build(1970, 4, 26, 3, zone = zone)
  )
})

test_that("can handle ambiguous times", {
  zone <- "America/New_York"

  expect_snapshot(error = TRUE, date_time_build(1970, 10, 25, 1, 30, zone = zone))

  expect_identical(
    date_time_build(1970, 10, 25, 1, 30, zone = zone, ambiguous = "earliest"),
    date_time_parse_complete("1970-10-25T01:30:00-04:00[America/New_York]")
  )
  expect_identical(
    date_time_build(1970, 10, 25, 1, 30, zone = zone, ambiguous = "latest"),
    date_time_parse_complete("1970-10-25T01:30:00-05:00[America/New_York]")
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
# date_time_info()

test_that("can get info of a date time (#295)", {
  zone <- "America/New_York"

  x <- date_time_build(2019, 1, 1, zone = zone)
  x <- date_time_info(x)

  begin <- date_time_build(2018, 11, 4, 1, zone = zone, ambiguous = "latest")
  end <- date_time_build(2019, 03, 10, 3, zone = zone)

  expect_identical(x$begin, begin)
  expect_identical(x$end, end)
  expect_identical(x$offset, -18000L)
  expect_identical(x$dst, FALSE)
  expect_identical(x$abbreviation, "EST")
})

test_that("`NA` propagates", {
  x <- date_time_build(NA, zone = "UTC")
  info <- date_time_info(x)

  expect_identical(info$begin, x)
  expect_identical(info$end, x)
  expect_identical(info$offset, NA_integer_)
  expect_identical(info$dst, NA)
  expect_identical(info$abbreviation, NA_character_)
})

test_that("boundaries are handled right", {
  x <- date_time_build(2019, 1, 1, zone = "UTC")
  x <- date_time_info(x)

  # Only snapshotting in case boundaries are different on CRAN
  expect_snapshot(x$begin)
  expect_snapshot(x$end)

  expect_identical(x$offset, 0L)
  expect_identical(x$dst, FALSE)
  expect_identical(x$abbreviation, "UTC")
})

test_that("works with POSIXlt", {
  x <- date_time_build(2019, 1, 1, zone = "America/New_York")

  expect_identical(
    date_time_info(as.POSIXlt(x)),
    date_time_info(x)
  )
})

test_that("input must be a date-time", {
  expect_snapshot(error = TRUE, {
    date_time_info(1)
  })
})

# ------------------------------------------------------------------------------
# date_start()

test_that("can get the start", {
  zone <- "America/New_York"
  x <- date_time_build(2019, 2, 2, 2, 2, 2, zone = zone)
  expect_identical(date_start(x, "second"), x)
  expect_identical(date_start(x, "month"), date_time_build(2019, 2, 1, zone = zone))
  expect_identical(date_start(x, "year"), date_time_build(2019, 1, 1, zone = zone))
})

test_that("start: can't use invalid precisions", {
  expect_snapshot(
    error = TRUE,
    date_start(date_time_build(2019, zone = "America/New_York"), "quarter")
  )
})

test_that("can resolve nonexistent start issues", {
  # In Asia/Beirut, DST gap from 2021-03-27 23:59:59 -> 2021-03-28 01:00:00
  zone <- "Asia/Beirut"
  x <- date_time_build(2021, 3, 28, 2, zone = zone)

  expect_snapshot({
    (expect_error(date_start(x, "day"), class = "clock_error_nonexistent_time"))
  })

  expect_identical(
    date_start(x, "day", nonexistent = "roll-forward"),
    date_time_build(2021, 3, 28, 1, zone = zone)
  )
})

test_that("can resolve ambiguous start issues", {
  # In Asia/Amman, DST fallback from 2021-10-29 00:59:59 -> 2021-10-29 00:00:00
  zone <- "Asia/Amman"
  x <- date_time_build(2021, 10, 29, 2, zone = zone)

  expect_snapshot({
    (expect_error(date_start(x, "day"), class = "clock_error_ambiguous_time"))
  })

  expect_identical(
    date_start(x, "day", ambiguous = "earliest"),
    date_time_parse_complete("2021-10-29T00:00:00+03:00[Asia/Amman]")
  )
  expect_identical(
    date_start(x, "day", ambiguous = "latest"),
    date_time_parse_complete("2021-10-29T00:00:00+02:00[Asia/Amman]")
  )
})

test_that("can automatically resolve ambiguous issues", {
  # In Asia/Amman, DST fallback from 2021-10-29 00:59:59 -> 2021-10-29 00:00:00
  zone <- "Asia/Amman"

  # Starts and ends the manipulation in the "earliest" hour
  x <- date_time_build(2021, 10, 29, 0, 20, zone = zone, ambiguous = "earliest")
  expect_identical(
    date_start(x, "day"),
    date_time_parse_complete("2021-10-29T00:00:00+03:00[Asia/Amman]")
  )

  # Starts and ends the manipulation in the "latest" hour
  x <- date_time_build(2021, 10, 29, 0, 20, zone = zone, ambiguous = "latest")
  expect_identical(
    date_start(x, "day"),
    date_time_parse_complete("2021-10-29T00:00:00+02:00[Asia/Amman]")
  )
})

# ------------------------------------------------------------------------------
# date_end()

test_that("can get the end", {
  zone <- "America/New_York"
  x <- date_time_build(2019:2020, 2, 2, 2, 2, 2, zone = zone)
  expect_identical(
    date_end(x, "day"),
    date_time_build(2019:2020, 2, 2, 23, 59, 59, zone = zone)
  )
  expect_identical(
    date_end(x, "month"),
    date_time_build(2019:2020, 2, 28:29, 23, 59, 59, zone = zone)
  )
})

test_that("end: can't use invalid precisions", {
  expect_snapshot(
    error = TRUE,
    date_end(date_time_build(2019, zone = "America/New_York"), "quarter")
  )
})

# ------------------------------------------------------------------------------
# date_seq()

test_that("integer `by` means second precision", {
  expect_identical(
    date_seq(new_datetime(0), to = new_datetime(5), by = 1),
    date_seq(new_datetime(0), to = new_datetime(5), by = duration_seconds(1))
  )
})

test_that("calendar backed `by` works", {
  zone <- "America/New_York"

  expect_identical(
    date_seq(
      date_time_build(2019, 1, 1, zone = zone),
      to = date_time_build(2019, 6, 1, zone = zone),
      by = duration_months(2)
    ),
    date_time_build(2019, c(1, 3, 5), 1, zone = zone)
  )
  expect_identical(
    date_seq(
      date_time_build(2019, 1, 1, zone = zone),
      to = date_time_build(2025, 1, 1, zone = zone),
      by = duration_years(2)
    ),
    date_time_build(c(2019, 2021, 2023, 2025), 1, 1, zone = zone)
  )
})

test_that("naive-time backed `by` works", {
  zone <- "America/New_York"

  expect_identical(
    date_seq(
      date_time_build(2019, 1, 1, zone = zone),
      to = date_time_build(2019, 1, 6, zone = zone),
      by = duration_days(2)
    ),
    date_time_build(2019, 1, c(1, 3, 5), zone = zone)
  )
})

test_that("sys-time backed `by` works", {
  zone <- "America/New_York"

  expect_identical(
    date_seq(
      date_time_build(2019, 1, 1, zone = zone),
      to = date_time_build(2019, 1, 1, 6, zone = zone),
      by = duration_hours(3)
    ),
    date_time_build(2019, 1, 1, c(0, 3, 6), zone = zone)
  )
  expect_identical(
    date_seq(
      date_time_build(2019, 1, 1, zone = zone),
      to = date_time_build(2019, 1, 1, 0, 6, zone = zone),
      by = duration_minutes(3)
    ),
    date_time_build(2019, 1, 1, 0, c(0, 3, 6), zone = zone)
  )
})

test_that("sub daily `by` uses sys-time around DST", {
  zone <- "America/New_York"
  from <- date_time_build(1970, 4, 26, 1, 30, zone = zone)

  expect_identical(
    date_seq(from, by = duration_hours(1), total_size = 3),
    date_time_build(1970, 4, 26, c(1, 3, 4), 30, zone = zone)
  )
})

test_that("daily `by` uses naive-time around DST gaps", {
  zone <- "America/New_York"
  from <- date_time_build(1970, 4, 25, 2, 30, zone = zone)

  expect_snapshot(error = TRUE, date_seq(from, by = duration_days(1), total_size = 3))

  expect_identical(
    date_seq(from, by = duration_days(1), total_size = 3, nonexistent = "roll-forward"),
    date_time_build(1970, 4, c(25, 26, 27), c(2, 3, 2), c(30, 0, 30), zone = zone)
  )
})

test_that("daily `by` uses naive-time around DST fallbacks", {
  zone <- "America/New_York"
  from <- date_time_build(1970, 10, 24, 1, 30, zone = zone)

  expect_snapshot(error = TRUE, date_seq(from, by = duration_days(1), total_size = 3))

  expect_identical(
    date_seq(from, by = duration_days(1), total_size = 3, ambiguous = "earliest"),
    date_time_build(1970, 10, c(24, 25, 26), 1, 30, zone = zone, ambiguous = "earliest")
  )
})

test_that("monthly / yearly `by` uses calendar -> naive-time around DST gaps", {
  zone <- "America/New_York"
  from <- date_time_build(1970, 3, 26, 2, 30, zone = zone)

  expect_snapshot(error = TRUE, date_seq(from, by = duration_months(1), total_size = 3))

  expect_identical(
    date_seq(
      from,
      by = duration_months(1),
      total_size = 3,
      nonexistent = "roll-forward"
    ),
    date_time_build(1970, c(3, 4, 5), 26, c(2, 3, 2), c(30, 0, 30), zone = zone)
  )
})

test_that("monthly / yearly `by` uses calendar -> naive-time around DST fallbacks", {
  zone <- "America/New_York"
  from <- date_time_build(1969, 10, 25, 1, 30, zone = zone)

  expect_snapshot(error = TRUE, date_seq(from, by = duration_years(1), total_size = 3))

  expect_identical(
    date_seq(from, by = duration_years(1), total_size = 3, ambiguous = "earliest"),
    date_time_build(
      c(1969, 1970, 1971),
      10,
      25,
      1,
      30,
      zone = zone,
      ambiguous = "earliest"
    )
  )
})

test_that("combining `by` with `total_size` works", {
  zone <- "America/New_York"

  expect_identical(
    date_seq(date_time_build(2019, 1, 1, zone = zone), by = 2, total_size = 3),
    date_time_build(2019, 1, 1, 0, 0, c(0, 2, 4), zone = zone)
  )
})

test_that("combining `to` with `total_size` works", {
  zone <- "America/New_York"

  expect_identical(
    date_seq(
      date_time_build(2019, 1, 1, zone = zone),
      to = date_time_build(2019, 1, 5, zone = zone),
      total_size = 3
    ),
    date_time_build(2019, 1, c(1, 3, 5), zone = zone)
  )
})

test_that("can resolve invalid dates", {
  zone <- "America/New_York"

  from <- date_time_build(2019, 1, 31, zone = zone)
  to <- date_time_build(2019, 5, 31, zone = zone)

  expect_snapshot(error = TRUE, date_seq(from, to = to, by = duration_months(1)))

  expect_identical(
    date_seq(from, to = to, by = duration_months(1), invalid = "previous-day"),
    date_time_build(2019, 1:5, "last", zone = zone)
  )
})

test_that("quarterly `by` works", {
  zone <- "America/New_York"

  expect_identical(
    date_seq(
      date_time_build(2019, 1, 2, zone = zone),
      by = duration_quarters(1),
      total_size = 3
    ),
    date_seq(
      date_time_build(2019, 1, 2, zone = zone),
      by = duration_months(3),
      total_size = 3
    )
  )
})

test_that("weekly `by` works", {
  zone <- "America/New_York"

  expect_identical(
    date_seq(
      date_time_build(2019, 1, 2, zone = zone),
      by = duration_weeks(1),
      total_size = 3
    ),
    date_seq(
      date_time_build(2019, 1, 2, zone = zone),
      by = duration_days(7),
      total_size = 3
    )
  )
})

test_that("components of `from` more precise than `by` are restored", {
  zone <- "America/New_York"

  expect_identical(
    date_seq(
      date_time_build(2019, 2, 3, 2, 2, 2, zone = zone),
      by = duration_minutes(1),
      total_size = 2
    ),
    date_time_build(2019, 2, 3, 2, 2:3, 2, zone = zone)
  )
  expect_identical(
    date_seq(
      date_time_build(2019, 2, 3, 2, 2, 2, zone = zone),
      by = duration_hours(1),
      total_size = 2
    ),
    date_time_build(2019, 2, 3, 2:3, 2, 2, zone = zone)
  )
  expect_identical(
    date_seq(
      date_time_build(2019, 2, 3, 2, 2, 2, zone = zone),
      by = duration_days(1),
      total_size = 2
    ),
    date_time_build(2019, 2, 3:4, 2, 2, 2, zone = zone)
  )
  expect_identical(
    date_seq(
      date_time_build(2019, 2, 3, 2, 2, 2, zone = zone),
      by = duration_months(1),
      total_size = 2
    ),
    date_time_build(2019, 2:3, 3, 2, 2, 2, zone = zone)
  )
  expect_identical(
    date_seq(
      date_time_build(2019, 2, 3, 2, 2, 2, zone = zone),
      by = duration_years(1),
      total_size = 2
    ),
    date_time_build(2019:2020, 2, 3, 2, 2, 2, zone = zone)
  )
})

test_that("components of `to` more precise than `by` must match `from`", {
  zone <- "America/New_York"

  expect_snapshot(
    error = TRUE,
    date_seq(
      date_time_build(2019, 1, 1, 2, 3, 20, zone = zone),
      to = date_time_build(2019, 2, 2, 1, 3, 5, zone = zone),
      by = duration_minutes(1)
    )
  )

  expect_snapshot(
    error = TRUE,
    date_seq(
      date_time_build(2019, 1, 1, zone = zone),
      to = date_time_build(2019, 2, 2, 2, zone = zone),
      by = duration_days(1)
    )
  )

  expect_snapshot(
    error = TRUE,
    date_seq(
      date_time_build(2019, 1, 1, zone = zone),
      to = date_time_build(2019, 2, 2, zone = zone),
      by = duration_months(1)
    )
  )
  expect_snapshot(
    error = TRUE,
    date_seq(
      date_time_build(2019, 1, 1, zone = zone),
      to = date_time_build(2019, 2, 1, 1, zone = zone),
      by = duration_months(1)
    )
  )

  expect_snapshot(
    error = TRUE,
    date_seq(
      date_time_build(2019, 1, 1, zone = zone),
      to = date_time_build(2019, 1, 2, zone = zone),
      by = duration_years(1)
    )
  )
})

test_that("seq() with `from > to && by > 0` or `from < to && by > 0` results in length 0 output (#282)", {
  zone <- "America/New_York"

  expect_identical(
    date_seq(
      date_time_build(2019, 1, 2, second = 1, zone = zone),
      to = date_time_build(2019, 1, 2, zone = zone),
      by = 1
    ),
    date_time_build(integer(), zone = zone)
  )

  expect_identical(
    date_seq(
      date_time_build(2019, zone = zone),
      to = date_time_build(2020, zone = zone),
      by = -1
    ),
    date_time_build(integer(), zone = zone)
  )
})

test_that("`to` must have same time zone as `by`", {
  expect_snapshot(
    error = TRUE,
    date_seq(
      date_time_build(1970, zone = "UTC"),
      to = date_time_build(1970, zone = "America/New_York"),
      by = 1
    )
  )
})

test_that("validates integerish `by`", {
  expect_snapshot(error = TRUE, date_seq(new_datetime(1), by = 1.5, total_size = 1))
})

test_that("validates `total_size` early", {
  expect_snapshot(error = TRUE, date_seq(new_datetime(1), by = 1, total_size = 1.5))
  expect_snapshot(error = TRUE, date_seq(new_datetime(1), by = 1, total_size = NA))
  expect_snapshot(error = TRUE, date_seq(new_datetime(1), by = 1, total_size = -1))
})

test_that("`to` and `total_size` must not generate a non-fractional sequence", {
  expect_snapshot(
    error = TRUE,
    date_seq(new_datetime(0), to = new_datetime(3), total_size = 3)
  )
})

test_that("requires exactly two optional arguments", {
  expect_snapshot(error = TRUE, date_seq(new_datetime(1), by = 1))
  expect_snapshot(error = TRUE, date_seq(new_datetime(1), total_size = 1))
  expect_snapshot(error = TRUE, date_seq(new_datetime(1), to = new_datetime(1)))
})

test_that("requires `to` to be POSIXt", {
  expect_snapshot(error = TRUE, date_seq(new_datetime(1), to = 1, by = 1))
})

test_that("requires year, month, day, hour, minute, or second precision", {
  expect_snapshot(
    error = TRUE,
    date_seq(new_datetime(1), to = new_datetime(2), by = duration_nanoseconds(1))
  )
})

test_that("checks empty dots", {
  expect_snapshot(error = TRUE, date_seq(new_datetime(1), new_datetime(2)))
})

test_that("golden test: ensure that we never allow components of `to` to differ with `from` (#224)", {
  # DST gap from 01:59:59 -> 03:00:00 on the 26th
  from <- date_time_build(1970, 04, 25, 02, 30, 00, zone = "America/New_York")
  to <- date_time_build(1970, 04, 26, 03, 00, 00, zone = "America/New_York")

  expect_error(
    date_seq(from, to = to, by = duration_days(1), nonexistent = "shift-forward")
  )

  # Could theoretically generate this, where the second element is past `to`
  #> "1970-04-25 02:30:00 EST" "1970-04-26 03:30:00 EDT"
})

# ------------------------------------------------------------------------------
# date_spanning_seq()

test_that("generates the regular sequence along the full span", {
  zone <- "UTC"

  x <- date_time_build(2020, minute = c(2, 1, 5), zone = zone)

  expect_equal(
    date_spanning_seq(x),
    seq(
      date_time_build(2020, minute = 1, zone = zone),
      date_time_build(2020, minute = 5, zone = zone),
      by = 1
    )
  )
})

test_that("missing values are removed", {
  zone <- "America/New_York"

  x <- date_time_build(2020, second = c(1, NA, 5, 2), zone = zone)
  expect_identical(
    date_spanning_seq(x),
    date_time_build(2020, second = 1:5, zone = zone)
  )

  x <- date_time_build(c(NA, NA), zone = zone)
  expect_identical(date_spanning_seq(x), new_datetime(tzone = zone))
})

test_that("infinite dates are removed", {
  zone <- "America/New_York"

  x <- new_datetime(c(0, Inf, 3, 6, -Inf), tzone = zone)
  expect_identical(date_spanning_seq(x), new_datetime(as.double(0:6), tzone = zone))

  x <- new_datetime(c(Inf, -Inf), tzone = zone)
  expect_identical(date_spanning_seq(x), new_datetime(tzone = zone))
})

test_that("works with empty vectors", {
  zone <- "America/New_York"
  x <- date_time_build(integer(), zone = zone)
  expect_identical(date_spanning_seq(x), x)
})

test_that("uses sys-time when generating the sequence (nonexistent)", {
  # Because this is happening at second precision and `date_seq()` uses sys-time
  # when generating second precision sequences
  zone <- "America/New_York"

  x <- date_time_build(1970, 4, 26, c(1, 3), c(59, 00), c(58, 02), zone = zone)

  expect_identical(
    date_spanning_seq(x),
    vec_c(
      date_time_build(1970, 4, 26, 1, 59, 58:59, zone = zone),
      date_time_build(1970, 4, 26, 3, 00, 00:02, zone = zone)
    )
  )
})

test_that("uses sys-time when generating the sequence (ambiguous)", {
  # Because this is happening at second precision and `date_seq()` uses sys-time
  # when generating second precision sequences
  zone <- "America/New_York"

  x <- date_time_build(
    1970,
    10,
    25,
    c(1, 1),
    c(59, 00),
    c(58, 02),
    ambiguous = c("earliest", "latest"),
    zone = zone
  )

  expect_identical(
    date_spanning_seq(x),
    vec_c(
      date_time_build(1970, 10, 25, 1, 59, 58:59, zone = zone, ambiguous = "earliest"),
      date_time_build(1970, 10, 25, 1, 00, 00:02, zone = zone, ambiguous = "latest")
    )
  )
})

# ------------------------------------------------------------------------------
# date_count_between()

test_that("can compute precisions at year / quarter / month / week / day / hour / minute / second", {
  x <- date_time_build(2019, 1, 5, 5, zone = "UTC")

  y <- date_time_build(2025, 1, c(4, 6), zone = "UTC")
  expect_identical(date_count_between(x, y, "year"), c(5L, 6L))
  expect_identical(date_count_between(x, y, "quarter"), c(23L, 24L))
  expect_identical(date_count_between(x, y, "month"), c(71L, 72L))

  y <- date_time_build(2019, 1, c(25, 27), zone = "UTC")
  expect_identical(date_count_between(x, y, "week"), c(2L, 3L))
  expect_identical(date_count_between(x, y, "day"), c(19L, 21L))

  y <- date_time_build(2019, 1, 6, c(5, 6), c(59, 0), zone = "UTC")
  expect_identical(date_count_between(x, y, "hour"), c(24L, 25L))
  expect_identical(date_count_between(x, y, "minute"), c(1499L, 1500L))
  expect_identical(date_count_between(x, y, "second"), c(89940L, 90000L))
})

test_that("can use posixlt", {
  x <- as.POSIXlt(date_time_build(2019, 1, 5, 5, zone = "UTC"))
  y <- as.POSIXlt(date_time_build(2020, 1, 5, c(4, 5), zone = "UTC"))
  expect_identical(date_count_between(x, y, "year"), c(0L, 1L))
})

test_that("nonexistent times are handled correctly", {
  x <- date_time_build(1970, 4, 26, 1, 59, 59, zone = "America/New_York")
  y <- date_time_build(1970, 4, 26, 3, 00, 00, zone = "America/New_York")

  # sys-time for hour, minute, second
  expect_identical(date_count_between(x, y, "second"), 1L)
  expect_identical(date_count_between(x, y, "hour"), 0L)

  # naive-time for week and day
  z <- date_time_build(1970, 5, 3, 2, 00, 00, zone = "America/New_York")
  expect_identical(date_count_between(x, z, "day"), 7L)
  expect_identical(date_count_between(y, z, "day"), 6L)

  # calendar (naive) for year and month
  z <- date_time_build(1970, 5, 26, 2, 30, 00, zone = "America/New_York")
  expect_identical(date_count_between(x, z, "month"), 1L)
  expect_identical(date_count_between(y, z, "month"), 0L)
})

test_that("ambiguous times are handled correctly", {
  x <- date_time_build(
    1970,
    10,
    25,
    1,
    00,
    01,
    zone = "America/New_York",
    ambiguous = "earliest"
  )
  y <- date_time_build(
    1970,
    10,
    25,
    1,
    00,
    01,
    zone = "America/New_York",
    ambiguous = "latest"
  )

  # sys-time for hour, minute, second
  expect_identical(date_count_between(x, y, "second"), 3600L)
  expect_identical(date_count_between(x, y, "hour"), 1L)

  # naive-time for week and day
  z <- date_time_build(1970, 11, 01, 1, 00, c(00, 02), zone = "America/New_York")
  expect_identical(date_count_between(x, z, "week"), c(0L, 1L))
  expect_identical(date_count_between(y, z, "week"), c(0L, 1L))
  expect_identical(date_count_between(x, z, "day"), c(6L, 7L))
  expect_identical(date_count_between(y, z, "day"), c(6L, 7L))

  # calendar (naive) for year and month
  z <- date_time_build(1970, 11, 25, 1, 00, c(00, 02), zone = "America/New_York")
  expect_identical(date_count_between(x, z, "month"), c(0L, 1L))
  expect_identical(date_count_between(y, z, "month"), c(0L, 1L))
})

test_that("must use a valid POSIXt precision", {
  x <- date_time_build(2019, zone = "UTC")
  expect_snapshot((expect_error(date_count_between(x, x, "millisecond"))))
})

test_that("can't count between a POSIXt and a Date", {
  x <- date_time_build(2019, zone = "UTC")
  y <- date_build(2019)
  expect_snapshot((expect_error(date_count_between(x, y, "year"))))
})

# ------------------------------------------------------------------------------
# vec_arith()

test_that("<posixt> op <duration>", {
  zone <- "America/New_York"

  new_posixlt <- function(x, zone) {
    as.POSIXlt(new_datetime(x, zone))
  }

  expect_identical(
    vec_arith("+", new_datetime(0, zone), duration_years(1)),
    new_datetime(31536000, zone)
  )
  expect_identical(
    vec_arith("+", new_posixlt(0, zone), duration_years(1)),
    new_datetime(31536000, zone)
  )

  expect_identical(
    vec_arith("-", new_datetime(0, zone), duration_years(1)),
    new_datetime(-31536000, zone)
  )
  expect_identical(
    vec_arith("-", new_posixlt(0, zone), duration_years(1)),
    new_datetime(-31536000, zone)
  )

  expect_identical(
    vec_arith("+", new_datetime(0, zone), duration_seconds(1)),
    new_datetime(1, zone)
  )
  expect_identical(
    vec_arith("+", new_posixlt(0, zone), duration_seconds(1)),
    new_datetime(1, zone)
  )

  expect_snapshot(
    error = TRUE,
    vec_arith("+", new_datetime(0, zone), duration_milliseconds(1))
  )
  expect_snapshot(
    error = TRUE,
    vec_arith("+", new_posixlt(0, zone), duration_milliseconds(1))
  )

  expect_snapshot(
    error = TRUE,
    vec_arith("*", new_datetime(0, zone), duration_years(1))
  )
  expect_snapshot(error = TRUE, vec_arith("*", new_posixlt(0, zone), duration_years(1)))
})

test_that("<duration> op <posixt>", {
  zone <- "America/New_York"

  new_posixlt <- function(x, zone) {
    as.POSIXlt(new_datetime(x, zone))
  }

  expect_identical(
    vec_arith("+", duration_years(1), new_datetime(0, zone)),
    new_datetime(31536000, zone)
  )
  expect_identical(
    vec_arith("+", duration_years(1), new_posixlt(0, zone)),
    new_datetime(31536000, zone)
  )

  expect_identical(
    vec_arith("+", duration_seconds(1), new_datetime(0, zone)),
    new_datetime(1, zone)
  )
  expect_identical(
    vec_arith("+", duration_seconds(1), new_posixlt(0, zone)),
    new_datetime(1, zone)
  )

  expect_snapshot(
    error = TRUE,
    vec_arith("-", duration_years(1), new_datetime(0, zone))
  )
  expect_snapshot(error = TRUE, vec_arith("-", duration_years(1), new_posixlt(0, zone)))

  expect_snapshot(
    error = TRUE,
    vec_arith("+", duration_milliseconds(1), new_datetime(0, zone))
  )
  expect_snapshot(
    error = TRUE,
    vec_arith("+", duration_milliseconds(1), new_posixlt(0, zone))
  )

  expect_snapshot(
    error = TRUE,
    vec_arith("*", duration_years(1), new_datetime(0, zone))
  )
  expect_snapshot(error = TRUE, vec_arith("*", duration_years(1), new_posixlt(0, zone)))
})

# ------------------------------------------------------------------------------
# slider_plus() / slider_minus()

test_that("`slider_plus()` method is registered", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  zone <- "America/New_York"

  x <- date_time_build(2019, 1, 1, 3:4, 30, zone = zone)

  y <- duration_hours(3)
  expect_identical(
    slider::slider_plus(x, y),
    date_time_build(2019, 1, 1, 6:7, 30, zone = zone)
  )
  expect_identical(
    slider::slider_plus(as.POSIXlt(x), y),
    date_time_build(2019, 1, 1, 6:7, 30, zone = zone)
  )

  y <- duration_days(2)
  expect_identical(
    slider::slider_plus(x, y),
    date_time_build(2019, 1, 3, 3:4, 30, zone = zone)
  )
  expect_identical(
    slider::slider_plus(as.POSIXlt(x), y),
    date_time_build(2019, 1, 3, 3:4, 30, zone = zone)
  )
})

test_that("`slider_minus()` method is registered", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  zone <- "America/New_York"

  x <- date_time_build(2019, 1, 1, 3:4, 30, zone = zone)

  y <- duration_hours(3)
  expect_identical(
    slider::slider_minus(x, y),
    date_time_build(2019, 1, 1, 0:1, 30, zone = zone)
  )
  expect_identical(
    slider::slider_minus(as.POSIXlt(x), y),
    date_time_build(2019, 1, 1, 0:1, 30, zone = zone)
  )

  y <- duration_days(2)
  expect_identical(
    slider::slider_minus(x, y),
    date_time_build(2018, 12, 30, 3:4, 30, zone = zone)
  )
  expect_identical(
    slider::slider_minus(as.POSIXlt(x), y),
    date_time_build(2018, 12, 30, 3:4, 30, zone = zone)
  )
})

test_that("`slide_index()` works with date-times and durations", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  zone <- "America/New_York"

  i <- date_time_build(2019, 1, 1, 1:6, zone = zone)
  x <- seq_along(i)

  before <- duration_hours(2)
  after <- duration_hours(1)

  expect <- list(
    1:2,
    1:3,
    1:4,
    2:5,
    3:6,
    4:6
  )

  expect_identical(
    slider::slide_index(x, i, identity, .before = before, .after = after),
    expect
  )
  expect_identical(
    slider::slide_index(x, as.POSIXlt(i), identity, .before = before, .after = after),
    expect
  )
})

test_that("`slide_index()` with date-times and sys-time based arithmetic is sensible around ambiguous times", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  zone <- "America/New_York"

  hour <- c(0, 1, 1, 2, 3)
  ambiguous <- c("error", "earliest", "latest", "error", "error")

  i <- date_time_build(1970, 10, 25, hour, zone = zone, ambiguous = ambiguous)
  x <- seq_along(i)

  # Sys-time based arithmetic
  before <- duration_hours(2)

  expect_identical(
    slider::slide_index(x, i, identity, .before = before),
    list(
      1L,
      1:2,
      1:3,
      2:4,
      3:5
    )
  )
})

test_that("`slide_index()` with date-times and sys-time based arithmetic is sensible around nonexistent times", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  zone <- "America/New_York"

  i <- date_time_build(1970, 4, 26, 1, 59, 59, zone = zone)
  i <- add_seconds(i, 0:4)
  x <- seq_along(i)

  # Sys-time based arithmetic
  before <- duration_seconds(2)

  expect_identical(
    slider::slide_index(x, i, identity, .before = before),
    list(
      1L,
      1:2,
      1:3,
      2:4,
      3:5
    )
  )
})

test_that("`slide_index()` will error on naive-time based arithmetic and ambiguous times", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  zone <- "America/New_York"

  i <- date_time_build(1970, 10, 24, 1, zone = zone)
  x <- seq_along(i)

  # Naive-time based arithmetic
  after <- duration_days(1)

  expect_snapshot(error = TRUE, {
    slider::slide_index(x, i, identity, .after = after)
  })
})

test_that("`slide_index()` will error on naive-time based arithmetic and nonexistent times", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  zone <- "America/New_York"

  i <- date_time_build(1970, 4, 25, 2, 30, zone = zone)
  x <- seq_along(i)

  # Naive-time based arithmetic
  after <- duration_days(1)

  expect_snapshot(error = TRUE, {
    slider::slide_index(x, i, identity, .after = after)
  })
})

test_that("`slide_index()` will error on calendrical arithmetic and ambiguous times", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  zone <- "America/New_York"

  i <- date_time_build(1970, 9, 25, 1, zone = zone)
  x <- seq_along(i)

  # Calendrical based arithmetic
  after <- duration_months(1)

  expect_snapshot(error = TRUE, {
    slider::slide_index(x, i, identity, .after = after)
  })
})

test_that("`slide_index()` will error on calendrical arithmetic and nonexistent times", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  zone <- "America/New_York"

  i <- date_time_build(1970, 3, 26, 2, 30, zone = zone)
  x <- seq_along(i)

  # Calendrical based arithmetic
  after <- duration_months(1)

  expect_snapshot(error = TRUE, {
    slider::slide_index(x, i, identity, .after = after)
  })
})
