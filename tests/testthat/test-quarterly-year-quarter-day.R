# ------------------------------------------------------------------------------
# calendar_widen()

test_that("can widen to quarter", {
  x <- year_quarter_day(2019)
  expect_identical(calendar_widen(x, "quarter"), set_quarter(x, 1))
})

test_that("can widen to day", {
  x <- year_quarter_day(2019)
  y <- year_quarter_day(2019, 02)
  expect_identical(calendar_widen(x, "day"), set_day(set_quarter(x, 1), 1))
  expect_identical(calendar_widen(y, "day"), set_day(y, 1))
})
