test_that("printing local-date", {
  expect_snapshot_output(local_date(2019))
  expect_snapshot_output(local_date(c(2019, 2020, NA)))
})

test_that("printing empty local-date doesn't print `character()`", {
  expect_snapshot_output(local_date(integer()))
})

test_that("printing in tibble columns is nice", {
  skip_if_not_installed("pillar")

  x <- local_date(c(2019, NA))
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})
