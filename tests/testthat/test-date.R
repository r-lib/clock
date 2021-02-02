# ------------------------------------------------------------------------------
# as_weekday()

test_that("can convert to weekday", {
  x <- as.Date("2019-01-01")
  expect_identical(as_weekday(x), weekday(2L))
})

test_that("converting to weekday retains names", {
  x <- c(x = as.Date("2019-01-01"))
  expect_named(as_weekday(x), names(x))
})
