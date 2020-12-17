test_that("printing zoned-date-time", {
  expect_snapshot_output(zoned_date_time(2019, month = 5))
  expect_snapshot_output(zoned_date_time(2019, month = 5, zone = "America/New_York"))
  expect_snapshot_output(zoned_date_time(c(2019, 2020, NA), month = 5, second = c(3, 4, 5)))
})

test_that("printing zoned-date-nano-time", {
  expect_snapshot_output(zoned_date_nanotime(2019, month = 5, nanosecond = 5))
  expect_snapshot_output(zoned_date_nanotime(2019, month = 5, nanosecond = 5, zone = "America/New_York"))
  expect_snapshot_output(zoned_date_nanotime(c(2019, 2020, NA), month = 5, second = c(3, 4, 5), nanosecond = c(5, 6, 7)))
})

test_that("printing zoned-date-time - ambiguous time", {
  expect_snapshot_output(zoned_date_time(1970, 10, 25, 01, 30, zone = "America/New_York", dst_ambiguous = c("earliest", "latest")))
})

test_that("printing zoned-date-nano-time - ambiguous time", {
  expect_snapshot_output(zoned_date_nanotime(1970, 10, 25, 01, 30, nanosecond = 5, zone = "America/New_York", dst_ambiguous = c("earliest", "latest")))
})

test_that("format for zoned-date-time has the zone name by default", {
  x <- zoned_date_time(1970, 10, 25, 01, 30, zone = "America/New_York")
  expect_snapshot_output(format(x))
  expect_snapshot_output(format(x, abbreviate_zone = TRUE))
  expect_snapshot_output(format(x, format = "%Y-%m-%d %H:%M:%S%Ez"))
})

test_that("format for zoned-date-nano-time has the zone name by default", {
  x <- zoned_date_nanotime(1970, 10, 25, 01, 30, nanosecond = 5000, zone = "America/New_York")
  expect_snapshot_output(format(x))
  expect_snapshot_output(format(x, abbreviate_zone = TRUE))
  expect_snapshot_output(format(x, format = "%Y-%m-%d %H:%M:%S%Ez"))
})

test_that("printing in data frames uses zone name - zoned-date-time", {
  x <- zoned_date_time(1970, 10, 25, 01, 30, zone = "America/New_York")
  expect_snapshot_output(data.frame(x = x))
})
test_that("printing in data frames uses zone name - zoned-date-nano-time", {
  x <- zoned_date_nanotime(1970, 10, 25, 01, 30, nanosecond = 5000, zone = "America/New_York")
  expect_snapshot_output(data.frame(x = x))
})

test_that("printing in tibble columns is nice and doesn't use zone name - zoned-date-time", {
  skip_if_not_installed("pillar")

  x <- zoned_date_time(c(2019, NA), zone = "America/New_York")
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})

test_that("printing in tibble columns is nice and doesn't use zone name - zoned-date-nano-time", {
  skip_if_not_installed("pillar")

  x <- zoned_date_nanotime(c(2019, NA), zone = "America/New_York")
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})
