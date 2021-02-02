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
