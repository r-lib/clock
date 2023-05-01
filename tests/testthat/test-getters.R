# ------------------------------------------------------------------------------
# get_*()

test_that("getters throw default error", {
  x <- 1

  expect_snapshot(error = TRUE, {
    get_year(x)
  })
  expect_snapshot(error = TRUE, {
    get_quarter(x)
  })
  expect_snapshot(error = TRUE, {
    get_month(x)
  })
  expect_snapshot(error = TRUE, {
    get_week(x)
  })
  expect_snapshot(error = TRUE, {
    get_day(x)
  })
  expect_snapshot(error = TRUE, {
    get_hour(x)
  })
  expect_snapshot(error = TRUE, {
    get_minute(x)
  })
  expect_snapshot(error = TRUE, {
    get_second(x)
  })
  expect_snapshot(error = TRUE, {
    get_millisecond(x)
  })
  expect_snapshot(error = TRUE, {
    get_microsecond(x)
  })
  expect_snapshot(error = TRUE, {
    get_nanosecond(x)
  })
  expect_snapshot(error = TRUE, {
    get_index(x)
  })
})
