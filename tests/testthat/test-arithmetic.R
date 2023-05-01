# ------------------------------------------------------------------------------
# add_*()

test_that("arithmetic helpers throw default error", {
  x <- 1

  expect_snapshot(error = TRUE, {
    add_years(x)
  })
  expect_snapshot(error = TRUE, {
    add_quarters(x)
  })
  expect_snapshot(error = TRUE, {
    add_months(x)
  })
  expect_snapshot(error = TRUE, {
    add_weeks(x)
  })
  expect_snapshot(error = TRUE, {
    add_days(x)
  })
  expect_snapshot(error = TRUE, {
    add_hours(x)
  })
  expect_snapshot(error = TRUE, {
    add_minutes(x)
  })
  expect_snapshot(error = TRUE, {
    add_seconds(x)
  })
  expect_snapshot(error = TRUE, {
    add_milliseconds(x)
  })
  expect_snapshot(error = TRUE, {
    add_microseconds(x)
  })
  expect_snapshot(error = TRUE, {
    add_nanoseconds(x)
  })
})
