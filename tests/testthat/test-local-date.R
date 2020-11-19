test_that("printing local-date", {
  expect_snapshot_output(local_date(2019))
  expect_snapshot_output(local_date(c(2019, 2020, NA)))
})
