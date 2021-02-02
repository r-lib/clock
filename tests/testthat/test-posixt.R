# ------------------------------------------------------------------------------
# as_weekday()

test_that("can convert to weekday", {
  x <- as.POSIXct("2019-01-01", tz = "America/New_York")
  expect_identical(as_weekday(x), weekday(2L))
  expect_identical(as_weekday(as.POSIXlt(x)), weekday(2L))
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
