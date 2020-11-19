test_that("printing local-datetime", {
  expect_snapshot_output(local_datetime(2019, second = 5))
  expect_snapshot_output(local_datetime(c(2019, 2020, NA), second = 5))
})
