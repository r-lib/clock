# ------------------------------------------------------------------------------
# as.Date()

test_that("invalid dates must be resolved when converting to a Date", {
  expect_snapshot_error(as.Date(year_month_day(2019, 2, 31)))
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

test_that("can get the zone of a Date", {
  x <- as.Date("2019-01-01")

  expect_identical(date_zone(x), "UTC")
})

# ------------------------------------------------------------------------------
# date_set_zone()

test_that("can't set the zone of a Date", {
  x <- as.Date("2019-01-01")

  expect_snapshot_error(date_set_zone(x, "America/New_York"))
})

# ------------------------------------------------------------------------------
# date_parse()

test_that("parsing with `%z` can shift the returned Date", {
  expect_identical(
    date_parse("2019-12-31 23:59:59-0500", format = "%Y-%m-%d %H:%M:%S%z"),
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
