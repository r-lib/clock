test_that("printing local-year-month", {
  expect_snapshot_output(local_year_month(2019, month = 5))
  expect_snapshot_output(local_year_month(c(2019, NA), month = 5))
})

test_that("printing in tibble columns is nice", {
  skip_if_not_installed("pillar")

  x <- local_year_month(c(2019, NA))
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})
