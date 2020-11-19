test_that("printing zoned-datetime", {
  expect_snapshot_output(zoned_datetime(2019, month = 5))
  expect_snapshot_output(zoned_datetime(2019, month = 5, zone = "America/New_York"))
  expect_snapshot_output(zoned_datetime(c(2019, 2020, NA), month = 5, second = c(3, 4, 5)))
})

test_that("printing zoned-datetime - ambiguous time", {
  expect_snapshot_output(zoned_datetime(1970, 10, 25, 01, 30, zone = "America/New_York", dst_ambiguous = c("earliest", "latest")))
})

test_that("printing zoned-datetime - unambiguous format", {
  x <- zoned_datetime(1970, 10, 25, 01, 30, zone = "America/New_York")
  expect_snapshot_output(format(x))
  expect_snapshot_output(format(x, zone = TRUE))
})
