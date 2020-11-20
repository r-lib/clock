test_that("printing zoned-nano-datetime", {
  expect_snapshot_output(zoned_nano_datetime(2019, month = 5, nanos = 5))
  expect_snapshot_output(zoned_nano_datetime(2019, month = 5, nanos = 5, zone = "America/New_York"))
  expect_snapshot_output(zoned_nano_datetime(c(2019, 2020, NA), month = 5, second = c(3, 4, 5), nanos = c(5, 6, 7)))
})

test_that("printing zoned-nano-datetime - ambiguous time", {
  expect_snapshot_output(zoned_nano_datetime(1970, 10, 25, 01, 30, nanos = 5, zone = "America/New_York", dst_ambiguous = c("earliest", "latest")))
})

test_that("printing zoned-nano-datetime - extended format", {
  x <- zoned_nano_datetime(1970, 10, 25, 01, 30, nanos = 5000, zone = "America/New_York")
  expect_snapshot_output(format(x))
  expect_snapshot_output(format(x, format = fmt_zoned_nano_datetime(extended = TRUE)))
  expect_snapshot_output(format(x, format = fmt_zoned_nano_datetime(extended = TRUE), abbreviate_zone = TRUE))
})
