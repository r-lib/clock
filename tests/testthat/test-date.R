# ------------------------------------------------------------------------------
# as_date()

test_that("can convert Date -> Date", {
  expect_identical(as_date(new_date(0)), new_date(0))
})

test_that("integer Dates are normalized to double", {
  # seq.Date() can make these
  x <- 0L
  class(x) <- "Date"

  expect_identical(as_date(x), new_date(0))
})

test_that("can convert POSIXct / POSIXlt -> Date (Date assumed to be naive)", {
  ct <- date_time_parse("2019-01-01 23:00:00", "America/New_York")
  lt <- as.POSIXlt(ct)

  expect <- date_parse("2019-01-01")

  expect_identical(as_date(ct), expect)
  expect_identical(as_date(lt), expect)
})

test_that("can convert calendar -> Date", {
  expect_identical(as_date(year_month_day(1970, 1, 2)), new_date(1))
  expect_identical(as_date(year_quarter_day(1970, 1, 2)), new_date(1))
})

test_that("can convert sys-time -> Date", {
  expect_identical(as_date(sys_seconds(0)), new_date(0))
})

test_that("can convert naive-time -> Date", {
  expect_identical(as_date(naive_seconds(0)), new_date(0))
})

test_that("can convert zoned-time -> Date", {
  x <- as_zoned_time(naive_time_parse("2019-01-01 23:02:03"), "America/New_York")
  expect <- date_parse("2019-01-01")
  expect_identical(as_date(x), expect)
})

# ------------------------------------------------------------------------------
# as.Date()

test_that("invalid dates must be resolved when converting to a Date", {
  expect_snapshot_error(as.Date(year_month_day(2019, 2, 31)))
})

test_that("conversion from zoned-time uses naive-time as an intermediate", {
  x <- as_naive_time(year_month_day(2019, 12, 31, 23, 30, 00))
  x <- as_zoned_time(x, "America/New_York")

  expect_identical(as.Date(x), as.Date("2019-12-31"))
})

test_that("casting to Date floors components more precise than days (#205)", {
  x <- naive_time_parse("1969-01-01 13", precision = "hour")

  expect_identical(
    as.Date(x),
    date_build(1969, 1, 1)
  )
})

# ------------------------------------------------------------------------------
# as_sys_time()

test_that("converting to sys-time floors fractional dates (#191)", {
  x <- new_date(c(-0.5, 1.5))
  y <- new_date(c(-1, 1))

  expect_identical(as_sys_time(x), as_sys_time(y))
  expect_identical(as.Date(as_sys_time(x)), y)
})

test_that("converting to sys-time works with integer storage dates", {
  # These can occur from `seq.Date(from, to, length.out)`
  x <- structure(1L, class = "Date")
  y <- new_date(1)

  expect_identical(as_sys_time(x), as_sys_time(y))
  expect_identical(as.Date(as_sys_time(x)), y)
})

# ------------------------------------------------------------------------------
# as_zoned_time()

test_that("Dates are assumed to be naive", {
  x <- new_date(0)
  nt <- as_naive_time(year_month_day(1970, 1, 1))

  expect_identical(as_zoned_time(x, "UTC"), as_zoned_time(nt, "UTC"))
  expect_identical(as_zoned_time(x, "America/New_York"), as_zoned_time(nt, "America/New_York"))
})

test_that("can resolve nonexistent midnight issues", {
  # In Asia/Beirut, DST gap from 2021-03-27 23:59:59 -> 2021-03-28 01:00:00
  zone <- "Asia/Beirut"
  x <- as.Date("2021-03-28")

  expect_snapshot_error(as_zoned_time(x, zone), class = "clock_error_nonexistent_time")

  expect_identical(
    as_zoned_time(x, zone, nonexistent = "roll-forward"),
    as_zoned_time(as_naive_time(year_month_day(2021, 03, 28, 1)), zone)
  )
})

test_that("can resolve ambiguous midnight issues", {
  # In Asia/Amman, DST fallback from 2021-10-29 00:59:59 -> 2021-10-29 00:00:00
  zone <- "Asia/Amman"
  x <- as.Date("2021-10-29")

  expect_snapshot_error(as_zoned_time(x, zone), class = "clock_error_ambiguous_time")

  expect_identical(
    as_zoned_time(x, zone, ambiguous = "earliest"),
    zoned_time_parse_complete("2021-10-29 00:00:00+03:00[Asia/Amman]")
  )
  expect_identical(
    as_zoned_time(x, zone, ambiguous = "latest"),
    zoned_time_parse_complete("2021-10-29 00:00:00+02:00[Asia/Amman]")
  )
})

# ------------------------------------------------------------------------------
# as_weekday()

test_that("can convert to weekday", {
  x <- as.Date("2019-01-01")
  expect_identical(as_weekday(x), weekday(3L))
})

test_that("converting to weekday retains names", {
  x <- c(x = as.Date("2019-01-01"))
  expect_named(as_weekday(x), names(x))
})

# ------------------------------------------------------------------------------
# date_group()

test_that("can group by year/month/day", {
  x <- as.Date("2019-01-01") + c(0, 1, 2, 3)

  expect_identical(date_group(x, "day"), x)
  expect_identical(date_group(x, "day", n = 2), x[c(1, 1, 3, 3)])

  y <- as.Date(c("2019-01-01", "2019-02-05", "2019-03-10"))
  expect <- set_day(y, 1)

  expect_identical(date_group(y, "month"), expect)
  expect_identical(date_group(y, "month", n = 2), expect[c(1, 1, 3)])

  # Using year 0 as the starting point
  z <- as.Date(c("0000-01-05", "0001-02-10", "0002-03-12", "0003-01-15", "0004-12-10"))
  expect <- set_month(set_day(z, 1), 1)

  expect_identical(date_group(z, "year"), expect)
  expect_identical(date_group(z, "year", n = 3), expect[c(1, 1, 1, 4, 4)])
})

test_that("can't group by finer precisions", {
  x <- as.Date("2019-01-01")
  expect_snapshot_error(date_group(x, "hour"))
  expect_snapshot_error(date_group(x, "nanosecond"))
})

test_that("can't group by non-year-month-day precisions", {
  x <- as.Date("2019-01-01")
  expect_snapshot_error(date_group(x, "quarter"))
})

# ------------------------------------------------------------------------------
# date_leap_year()

test_that("can detect leap years", {
  x <- c("2019-01-01", "2020-01-01", NA)
  x <- as.Date(x)
  expect_identical(date_leap_year(x), c(FALSE, TRUE, NA))
})

# ------------------------------------------------------------------------------
# date_floor()

test_that("can floor by weeks", {
  x <- c("1970-01-01", "1970-01-07", "1970-01-08")
  x <- as.Date(x)
  expect_identical(date_floor(x, "week"), x[c(1, 1, 3)])
  expect_identical(date_floor(x, "week", n = 2), x[c(1, 1, 1)])
})

test_that("can only floor by week/day", {
  expect_snapshot_error(date_floor(as.Date("2019-01-01"), "hour"))
  expect_snapshot_error(date_floor(as.Date("2019-01-01"), "month"))
})

test_that("`origin` can be used", {
  origin <- as.Date("1970-01-02")
  x <- as.Date(c("1970-01-01", "1970-01-02", "1970-01-03"))
  expect <- as.Date(c("1969-12-31", "1970-01-02", "1970-01-02"))
  expect_identical(date_floor(x, "day", n = 2, origin = origin), expect)
})

test_that("`origin` is validated", {
  x <- as.Date("2019-01-01")
  expect_snapshot_error(date_floor(x, "day", origin = 1))
  expect_snapshot_error(date_floor(x, "day", origin = new_date(NA_real_)))
  expect_snapshot_error(date_floor(x, "day", origin = new_date(Inf)))
  expect_snapshot_error(date_floor(x, "day", origin = new_date(c(0, 1))))
})

# ------------------------------------------------------------------------------
# date_ceiling()

test_that("can ceiling", {
  x <- c("1970-01-01", "1970-01-07", "1970-01-08")
  x <- as.Date(x)
  expect_identical(date_ceiling(x, "week"), x[c(1, 3, 3)])
  expect_identical(date_ceiling(x, "week", n = 2), as.Date(c("1970-01-01", "1970-01-15", "1970-01-15")))
})

# ------------------------------------------------------------------------------
# date_round()

test_that("can round", {
  x <- c("1970-01-01", "1970-01-03", "1970-01-05", "1970-01-08")
  x <- as.Date(x)
  expect_identical(date_round(x, "week"), x[c(1, 1, 4, 4)])
  expect_identical(date_round(x, "week", n = 2), as.Date(c("1970-01-01", "1970-01-01", "1970-01-01", "1970-01-15")))
})

# ------------------------------------------------------------------------------
# date_weekday_factor()

test_that("can convert to a weekday factor", {
  x <- as.Date("2019-01-01") + 0:6

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
  x <- add_months(as.Date("2019-01-01"), 0:11)

  expect_identical(
    date_month_factor(x, labels = "fr", abbreviate = TRUE),
    calendar_month_factor(as_year_month_day(x), labels = "fr", abbreviate = TRUE),
  )
})

# ------------------------------------------------------------------------------
# date_format()

test_that("can format dates", {
  x <- as.Date("2018-12-31")
  format <- test_all_formats(zone = FALSE)

  expect_snapshot_output(
    cat(date_format(x, format = format))
  )
  expect_snapshot_output(
    cat(date_format(x, format = format, locale = clock_locale("fr")))
  )
})

test_that("formatting Dates with `%z` or `%Z` returns NA with a warning", {
  x <- as.Date("2018-01-01")

  expect_warning(date_format(x, format = "%z"), class = "clock_warning_format_failures")
  expect_snapshot(date_format(x, format = "%z"))

  expect_warning(date_format(x, format = "%Z"), class = "clock_warning_format_failures")
  expect_snapshot(date_format(x, format = "%Z"))
})

# ------------------------------------------------------------------------------
# date_parse()

test_that("`%z` and `%Z` commands are ignored", {
  expect_identical(
    date_parse("2019-12-31 11:59:59-0500", format = "%Y-%m-%d %H:%M:%S%z"),
    as.Date("2019-12-31")
  )
  expect_identical(
    date_parse("2019-12-31 11:59:59[America/New_York]", format = "%Y-%m-%d %H:%M:%S[%Z]"),
    as.Date("2019-12-31")
  )
})

test_that("parsing into a date if you requested to parse time components rounds the time (#207)", {
  expect_identical(
    date_parse("2019-12-31 11:59:59", format = "%Y-%m-%d %H:%M:%S"),
    as.Date("2019-12-31")
  )
  expect_identical(
    date_parse("2019-12-31 12:00:00", format = "%Y-%m-%d %H:%M:%S"),
    as.Date("2020-01-01")
  )
})

test_that("failure to parse throws a warning", {
  expect_warning(date_parse("foo"), class = "clock_warning_parse_failures")
  expect_snapshot(date_parse("foo"))
})

# ------------------------------------------------------------------------------
# date_shift()

test_that("can shift dates", {
  x <- date_parse("2019-01-01")
  x <- x + c(0, 1)

  monday <- weekday(clock_weekdays$monday)
  tuesday <- weekday(clock_weekdays$tuesday)

  expect_identical(
    date_shift(x, monday),
    add_days(x, c(6, 5))
  )
  expect_identical(
    date_shift(x, monday, which = "previous"),
    add_days(x, c(-1, -2))
  )
  expect_identical(
    date_shift(x, tuesday, boundary = "advance"),
    add_days(x, c(7, 6))
  )
})

# ------------------------------------------------------------------------------
# date_build()

test_that("can build a Date", {
  expect_identical(date_build(2019), as.Date("2019-01-01"))
  expect_identical(date_build(2020, 2, 3), as.Date("2020-02-03"))
})

test_that("can handle invalid dates", {
  expect_snapshot_error(date_build(2019, 1:12, 31))

  expect_identical(
    date_build(2019, 1:12, 31, invalid = "previous"),
    date_build(2019, 1:12, "last")
  )
})

# ------------------------------------------------------------------------------
# date_today()

test_that("can get the current date", {
  x <- date_today("America/New_York")
  expect_length(x, 1)
  expect_s3_class(x, "Date")
})

# ------------------------------------------------------------------------------
# date_zone()

test_that("cannot get the zone of a Date", {
  expect_snapshot_error(date_zone(new_date(0)))
})

# ------------------------------------------------------------------------------
# date_set_zone()

test_that("cannot set the zone of a Date", {
  expect_snapshot_error(date_set_zone(new_date(0), "UTC"))
})

# ------------------------------------------------------------------------------
# vec_arith()

test_that("<date> op <duration>", {
  expect_identical(vec_arith("+", new_date(0), duration_years(1)), new_date(365))
  expect_identical(vec_arith("-", new_date(0), duration_years(1)), new_date(-365))

  expect_snapshot_error(vec_arith("+", new_date(0), duration_hours(1)))
  expect_snapshot_error(vec_arith("*", new_date(0), duration_years(1)))
})

test_that("<duration> op <date>", {
  expect_identical(vec_arith("+", duration_years(1), new_date(0)), new_date(365))

  expect_snapshot_error(vec_arith("-", duration_years(1), new_date(0)))
  expect_snapshot_error(vec_arith("+", duration_hours(1), new_date(0)))
  expect_snapshot_error(vec_arith("*", duration_years(1), new_date(0)))
})
