# ------------------------------------------------------------------------------
# invalid_remove()

test_that("can remove invalid dates from calendar types", {
  x <- year_month_day(2020, 2, 28:30)
  expect_identical(invalid_remove(x), x[1:2])

  x <- year_day(2019, 365:366)
  expect_identical(invalid_remove(x), x[1L])
})

test_that("errors on unsupported types", {
  expect_snapshot(error = TRUE, invalid_remove(1))
})
