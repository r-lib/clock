# ------------------------------------------------------------------------------
# calendar_narrow()

test_that("can narrow to week", {
  x_expect <- iso_year_week_day(2019, 2)
  x <- set_day(x_expect, 1)
  expect_identical(calendar_narrow(x, "week"), x_expect)
  expect_identical(calendar_narrow(x_expect, "week"), x_expect)
})

test_that("can narrow to day", {
  x_expect <- iso_year_week_day(2019, 2, 3)
  x <- set_hour(x_expect, 5)
  expect_identical(calendar_narrow(x, "day"), x_expect)
  expect_identical(calendar_narrow(x_expect, "day"), x_expect)
})

# ------------------------------------------------------------------------------
# calendar_widen()

test_that("can widen to week", {
  x <- iso_year_week_day(2019)
  expect_identical(calendar_widen(x, "week"), set_week(x, 1))
})

test_that("can widen to day", {
  x <- iso_year_week_day(2019)
  y <- iso_year_week_day(2019, 02)
  expect_identical(calendar_widen(x, "day"), set_day(set_week(x, 1), 1))
  expect_identical(calendar_widen(y, "day"), set_day(y, 1))
})
