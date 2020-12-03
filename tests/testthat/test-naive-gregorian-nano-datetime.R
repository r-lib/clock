test_that("printing naive-nano-datetime", {
  expect_snapshot_output(naive_nano_datetime(2019, second = 5, nanos = 6))
  expect_snapshot_output(naive_nano_datetime(c(2019, 2020, NA), second = 5, nanos = 6))
})

test_that("printing in tibble columns is nice", {
  skip_if_not_installed("pillar")

  x <- naive_nano_datetime(c(2019, NA))
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})
