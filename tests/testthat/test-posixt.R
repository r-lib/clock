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
