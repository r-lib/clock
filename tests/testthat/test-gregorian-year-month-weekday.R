# ------------------------------------------------------------------------------
# calendar_narrow()

test_that("can narrow to month", {
  x_expect <- year_month_weekday(2019, 2)
  x <- set_day(x_expect, 1, index = 2)
  expect_identical(calendar_narrow(x, "month"), x_expect)
  expect_identical(calendar_narrow(x_expect, "month"), x_expect)
})

test_that("can narrow to day", {
  x_expect <- year_month_weekday(2019, 2, 3, 2)
  x <- set_hour(x_expect, 5)
  expect_identical(calendar_narrow(x, "day"), x_expect)
  expect_identical(calendar_narrow(x_expect, "day"), x_expect)
})

# ------------------------------------------------------------------------------
# calendar_widen()

test_that("can widen to month", {
  x <- year_month_weekday(2019)
  expect_identical(calendar_widen(x, "month"), set_month(x, 1))
})

test_that("can widen to day", {
  x <- year_month_weekday(2019)
  y <- year_month_weekday(2019, 02)
  expect_identical(calendar_widen(x, "day"), set_day(set_month(x, 1), 1, index = 1))
  expect_identical(calendar_widen(y, "day"), set_day(y, 1, index = 1))
})
