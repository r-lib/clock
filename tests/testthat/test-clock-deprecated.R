# ------------------------------------------------------------------------------
# date_zone()

test_that("`date_zone()` is deprecated but works for POSIXt (#326)", {
  x <- date_time_build(2020, zone = "America/New_York")

  expect_snapshot({
    zone <- date_zone(x)
  })

  expect_identical(zone, "America/New_York")
})

test_that("`date_zone()` is deprecated and errors for Date (#326)", {
  x <- date_build(2020)

  expect_snapshot(error = TRUE, {
    date_zone(x)
  })
})

# ------------------------------------------------------------------------------
# date_set_zone()

test_that("`date_set_zone()` is deprecated but works for POSIXt (#326)", {
  x <- date_time_build(2020, zone = "America/New_York")
  y <- date_time_set_zone(x, "America/Los_Angeles")

  expect_snapshot({
    x <- date_set_zone(x, "America/Los_Angeles")
  })

  expect_identical(x, y)
})

test_that("`date_set_zone()` is deprecated and errors for Date (#326)", {
  x <- date_build(2020)

  expect_snapshot(error = TRUE, {
    date_set_zone(x, "America/Los_Angeles")
  })
})
