# ------------------------------------------------------------------------------
# parse_sys_time()

# Note: Most tests are in `parse_naive_time()`. They share an implementation.

test_that("can parse day precision", {
  x <- c("2019-01-01", "2019-01-31")

  expect_identical(
    parse_sys_time(x, precision = "day"),
    as_sys_time(year_month_day(2019, 1, c(1, 31)))
  )
})

test_that("%z shifts the result by the offset", {
  x <- "2019-01-01 00:00:00+0100"
  y <- "2019-01-01 00:00:00-0100"

  expect_identical(
    parse_sys_time(x, format = "%Y-%m-%d %H:%M:%S%z"),
    as_sys_time(year_month_day(2018, 12, 31, 23, 0, 0))
  )
  expect_identical(
    parse_sys_time(y, format = "%Y-%m-%d %H:%M:%S%z"),
    as_sys_time(year_month_day(2019, 1, 1, 1, 0, 0))
  )
})
