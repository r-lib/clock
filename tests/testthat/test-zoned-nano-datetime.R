test_that("printing zoned-nano-datetime", {
  expect_snapshot_output(zoned_nano_datetime(2019, month = 5, nanos = 5))
  expect_snapshot_output(zoned_nano_datetime(2019, month = 5, nanos = 5, zone = "America/New_York"))
  expect_snapshot_output(zoned_nano_datetime(c(2019, 2020, NA), month = 5, second = c(3, 4, 5), nanos = c(5, 6, 7)))
})

test_that("printing zoned-nano-datetime - ambiguous time", {
  expect_snapshot_output(zoned_nano_datetime(1970, 10, 25, 01, 30, nanos = 5, zone = "America/New_York", dst_ambiguous = c("earliest", "latest")))
})

test_that("format for zoned-nano-datetime has the zone name by default", {
  x <- zoned_nano_datetime(1970, 10, 25, 01, 30, nanos = 5000, zone = "America/New_York")
  expect_snapshot_output(format(x))
  expect_snapshot_output(format(x, abbreviate_zone = TRUE))
  expect_snapshot_output(format(x, format = fmt_zoned_nano_datetime(zone_name = FALSE)))
})

test_that("printing in data frames uses zone name", {
  x <- zoned_nano_datetime(1970, 10, 25, 01, 30, nanos = 5000, zone = "America/New_York")
  expect_snapshot_output(data.frame(x = x))
})

test_that("printing in tibble columns is nice and doesn't use zone name", {
  skip_if_not_installed("pillar")

  x <- zoned_nano_datetime(c(2019, NA), zone = "America/New_York")
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})
