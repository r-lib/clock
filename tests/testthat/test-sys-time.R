# ------------------------------------------------------------------------------
# sys_now()

test_that("returns nanosecond precision", {
  x <- sys_now()
  expect_identical(time_point_precision(x), PRECISION_NANOSECOND)
})

test_that("returns a single sys-time", {
  x <- sys_now()
  expect_length(x, 1L)
  expect_s3_class(x, "clock_sys_time")
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- as_sys(year_month_day(2019, 1, 1))
  expect_identical(as.character(x), "2019-01-01")

  x <- as_sys(year_month_day(2019, 1, 1, 1, 1))
  expect_identical(as.character(x), "2019-01-01 01:01")
})

# ------------------------------------------------------------------------------
# sys_parse()

# Note: Most tests are in `naive_parse()`. They share an implementation.

test_that("can parse day precision", {
  x <- c("2019-01-01", "2019-01-31")

  expect_identical(
    sys_parse(x, precision = "day"),
    as_sys(year_month_day(2019, 1, c(1, 31)))
  )
})

test_that("%z shifts the result by the offset", {
  x <- "2019-01-01 00:00:00+0100"
  y <- "2019-01-01 00:00:00-0100"

  expect_identical(
    sys_parse(x, format = "%Y-%m-%d %H:%M:%S%z"),
    as_sys(year_month_day(2018, 12, 31, 23, 0, 0))
  )
  expect_identical(
    sys_parse(y, format = "%Y-%m-%d %H:%M:%S%z"),
    as_sys(year_month_day(2019, 1, 1, 1, 0, 0))
  )
})

test_that("failure to parse throws a warning", {
  expect_warning(sys_parse("foo"), class = "clock_warning_parse_failures")
  expect_snapshot(sys_parse("foo"))
})
