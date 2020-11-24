test_that("printing zoned-datetime", {
  local_c_locale()

  expect_snapshot_output(zoned_datetime(2019, month = 5))
  expect_snapshot_output(zoned_datetime(2019, month = 5, zone = "America/New_York"))
  expect_snapshot_output(zoned_datetime(c(2019, 2020, NA), month = 5, second = c(3, 4, 5)))
})

test_that("printing zoned-datetime - ambiguous time", {
  local_c_locale()

  expect_snapshot_output(zoned_datetime(1970, 10, 25, 01, 30, zone = "America/New_York", dst_ambiguous = c("earliest", "latest")))
})

test_that("format for zoned-datetime has the zone name by default", {
  local_c_locale()

  x <- zoned_datetime(1970, 10, 25, 01, 30, zone = "America/New_York")
  expect_snapshot_output(format(x))
  expect_snapshot_output(format(x, abbreviate_zone = TRUE))
  expect_snapshot_output(format(x, format = fmt_zoned_datetime(zone_name = FALSE)))
})

test_that("printing in data frames uses zone name", {
  local_c_locale()

  x <- zoned_datetime(1970, 10, 25, 01, 30, zone = "America/New_York")
  expect_snapshot_output(data.frame(x = x))
})

test_that("printing in tibble columns is nice and doesn't use zone name", {
  skip_if_not_installed("pillar")
  local_c_locale()

  x <- zoned_datetime(c(2019, NA), zone = "America/New_York")
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})
