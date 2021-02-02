# ------------------------------------------------------------------------------
# calendar_narrow()

test_that("narrow: `precision` is validated", {
  expect_snapshot_error(calendar_narrow(year_month_day(2019), "foo"))
})

test_that("narrow: `precision` must be calendar specific", {
  expect_snapshot_error(calendar_narrow(year_month_day(2019), "quarter"))
})

test_that("narrow: `precision` can't be wider than `x`", {
  expect_snapshot_error(calendar_narrow(year_month_day(2019, 1, 1), "second"))
})

test_that("narrow: can't narrow a subsecond precision `x` to another subsecond precision", {
  x <- calendar_widen(year_month_day(2019), "nanosecond")
  expect_snapshot_error(calendar_narrow(x, "microsecond"))
})

test_that("narrow: can narrow subsecond precision to same subsecond precision", {
  x <- year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "millisecond")
  expect_identical(calendar_narrow(x, "millisecond"), x)
})

# ------------------------------------------------------------------------------
# calendar_widen()

test_that("widen: `precision` is validated", {
  expect_snapshot_error(calendar_widen(year_month_day(2019), "foo"))
})

test_that("widen: `precision` must be calendar specific", {
  expect_snapshot_error(calendar_widen(year_month_day(2019), "quarter"))
})

test_that("widen: `precision` can't be narrower than `x`", {
  expect_snapshot_error(calendar_widen(year_month_day(2019, 1, 1), "month"))
})

test_that("widen: can't widen a subsecond precision `x` to another subsecond precision", {
  x <- calendar_widen(year_month_day(2019), "millisecond")
  expect_snapshot_error(calendar_widen(x, "microsecond"))
})

test_that("widen: can widen subsecond precision to the same subsecond precision", {
  x <- year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "millisecond")
  expect_identical(calendar_widen(x, "millisecond"), x)
})
