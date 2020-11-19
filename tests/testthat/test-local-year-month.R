test_that("printing local-year-month", {
  expect_snapshot_output(local_year_month(2019, month = 5))
  expect_snapshot_output(local_year_month(c(2019, NA), month = 5))
})
