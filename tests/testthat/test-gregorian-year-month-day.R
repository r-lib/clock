test_that("printing year-month-day", {
  expect_snapshot_output(year_month_day(2019))
  expect_snapshot_output(year_month_day(c(2019, 2020, NA)))
})

test_that("printing empty year-month-day doesn't print `character()`", {
  expect_snapshot_output(year_month_day(integer()))
})

test_that("printing in tibble columns is nice", {
  skip_if_not_installed("pillar")

  x <- year_month_day(c(2019, NA))
  x <- list(x = x)

  expect_snapshot_output(pillar::colonnade(x))
})
