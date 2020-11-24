test_that("printing local-datetime", {
  local_c_locale()

  expect_snapshot_output(local_datetime(2019, second = 5))
  expect_snapshot_output(local_datetime(c(2019, 2020, NA), second = 5))
})

test_that("printing in tibble columns is nice", {
  skip_if_not_installed("pillar")
  local_c_locale()

  x <- local_datetime(c(2019, NA))
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})
