# ------------------------------------------------------------------------------
# calendar_widen()

test_that("`precision` is validated", {
  expect_snapshot_error(calendar_widen(year_month_day(2019), "foo"))
})

test_that("`precision` must be calendar specific", {
  expect_snapshot_error(calendar_widen(year_month_day(2019), "quarter"))
})

test_that("`precision` can't be narrower than `x`", {
  expect_snapshot_error(calendar_widen(year_month_day(2019, 1, 1), "month"))
})

test_that("can't widen a subsecond precision `x`", {
  x <- calendar_widen(year_month_day(2019), "millisecond")

  expect_snapshot_error(calendar_widen(x, "millisecond"))
  expect_snapshot_error(calendar_widen(x, "microsecond"))
})
