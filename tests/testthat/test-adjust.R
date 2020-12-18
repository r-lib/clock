
# ------------------------------------------------------------------------------
# adjust_calendar()

test_that("can adjust the calendar - naive", {
  x <- new_naive_time_point(calendar = new_year_month_day())
  to <- new_year_quarternum_quarterday(start = 2L)
  expect_identical(field_calendar(adjust_calendar(x, to)), to)
})

test_that("can adjust the calendar - zoned", {
  x <- new_zoned_time_point(calendar = new_year_month_day())
  to <- new_year_quarternum_quarterday(start = 2L)
  expect_identical(field_calendar(adjust_calendar(x, to)), to)
})

test_that("cannot adjust with a non-daily calendar", {
  x <- new_naive_time_point(calendar = new_year_month_day())
  to <- new_year_quarternum(start = 2L)
  expect_snapshot_error(adjust_calendar(x, to))
})
