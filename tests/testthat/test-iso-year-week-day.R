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
