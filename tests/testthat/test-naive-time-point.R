test_that("can test for naive objects", {
  x <- new_naive_time_point()
  expect_true(is_naive_time_point(x))
})

test_that("printing naive_date_time()", {
  expect_snapshot_output(naive_date_time(2019, second = 5))
  expect_snapshot_output(naive_date_time(c(2019, 2020, NA), second = 5))
})

test_that("printing in tibble columns is nice - date time", {
  skip_if_not_installed("pillar")

  x <- naive_date_time(c(2019, NA))
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})

test_that("printing naive_date_nanotime()", {
  expect_snapshot_output(naive_date_nanotime(2019, second = 5, nanosecond = 6))
  expect_snapshot_output(naive_date_nanotime(c(2019, 2020, NA), second = 5, nanosecond = 6))
})

test_that("printing in tibble columns is nice - date nanotime", {
  skip_if_not_installed("pillar")

  x <- naive_date_nanotime(c(2019, NA))
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})
