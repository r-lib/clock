test_that("printing year-month", {
  expect_snapshot_output(year_month(2019, month = 5))
  expect_snapshot_output(year_month(c(2019, NA), month = 5))
})

test_that("printing in tibble columns is nice", {
  skip_if_not_installed("pillar")

  x <- year_month(c(2019, NA))
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})
