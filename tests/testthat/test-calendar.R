# ------------------------------------------------------------------------------
# print() / obj_print_data() / obj_print_footer()

test_that("normal print method works", {
  x <- year_month_day(2019, 1:5)
  expect_snapshot(x)
})

test_that("can limit with `max`", {
  x <- year_month_day(2019, 1:5)

  expect_snapshot(print(x, max = 2))
  expect_snapshot(print(x, max = 4))

  # no footer if length >= max
  expect_snapshot(print(x, max = 5))
  expect_snapshot(print(x, max = 6))
})

test_that("`max` defaults to `getOption('max.print')` but can be overridden", {
  local_options(max.print = 3)

  x <- year_month_day(2019, 1:5)

  expect_snapshot(x)
  expect_snapshot(print(x, max = 4))
  expect_snapshot(print(x, max = 5))
})

test_that("`max` is validated", {
  x <- year_month_day(2019)

  expect_snapshot_error(print(x, max = -1))
  expect_snapshot_error(print(x, max = c(1, 2)))
  expect_snapshot_error(print(x, max = NA_integer_))
  expect_snapshot_error(print(x, max = "foo"))
})

# ------------------------------------------------------------------------------
# calendar_group()

test_that("group: `precision` is validated", {
  expect_snapshot_error(calendar_group(year_month_day(2019), "foo"))
})

test_that("group: `precision` must be calendar specific", {
  expect_snapshot_error(calendar_group(year_month_day(2019), "quarter"))
})

test_that("group: `precision` can't be wider than `x`", {
  expect_snapshot_error(calendar_group(year_month_day(2019, 1, 1), "second"))
})

test_that("group: can't group a subsecond precision `x` at another subsecond precision", {
  x <- calendar_widen(year_month_day(2019), "nanosecond")
  expect_snapshot_error(calendar_group(x, "microsecond"))
})

test_that("group: can group subsecond precision at the same subsecond precision", {
  x <- year_month_day(2019, 1, 1, 1, 1, 1, 0:1, subsecond_precision = "millisecond")
  expect_identical(calendar_group(x, "millisecond", n = 2L), x[c(1, 1)])
})

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

# ------------------------------------------------------------------------------
# calendar_start()

test_that("start: `x` is validated", {
  expect_snapshot_error(calendar_start(1))
})

test_that("start: `precision` is validated", {
  expect_snapshot_error(calendar_start(year_month_day(2019), "foo"))
  expect_snapshot_error(calendar_start(year_month_day(2019), 1))
})

test_that("start: errors on unsupported precision", {
  expect_snapshot_error(calendar_start(year_month_day(2019, 1), "quarter"))
})

test_that("start: `precision` can't be more precise than `x`", {
  expect_snapshot_error(calendar_start(year_month_day(2019), "month"))
})

test_that("start: can't mix different subsecond precisions", {
  x <- year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "microsecond")
  expect_snapshot_error(calendar_start(x, "millisecond"))
})

test_that("start: can use same subsecond precision", {
  x <- year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "microsecond")
  expect_identical(calendar_start(x, "microsecond"), x)
})

test_that("start: can compute day start", {
  x <- year_month_day(2019, 2, 2)
  expect_identical(calendar_start(x, "day"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "nanosecond")
  expect <- year_month_day(2019, 2, 2, 0, 0, 0, 0, subsecond_precision = "nanosecond")
  expect_identical(calendar_start(x, "day"), expect)
})

test_that("start: can compute hour start", {
  x <- year_month_day(2019, 2, 2, 2)
  expect_identical(calendar_start(x, "hour"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "nanosecond")
  expect <- year_month_day(2019, 2, 2, 2, 0, 0, 0, subsecond_precision = "nanosecond")
  expect_identical(calendar_start(x, "hour"), expect)
})

test_that("start: can compute minute start", {
  x <- year_month_day(2019, 2, 2, 2, 2)
  expect_identical(calendar_start(x, "minute"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "nanosecond")
  expect <- year_month_day(2019, 2, 2, 2, 2, 0, 0, subsecond_precision = "nanosecond")
  expect_identical(calendar_start(x, "minute"), expect)
})

test_that("start: can compute second start", {
  x <- year_month_day(2019, 2, 2, 2, 2, 2)
  expect_identical(calendar_start(x, "second"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "nanosecond")
  expect <- year_month_day(2019, 2, 2, 2, 2, 2, 0, subsecond_precision = "nanosecond")
  expect_identical(calendar_start(x, "second"), expect)
})

test_that("start: invalid dates are adjusted", {
  x <- year_month_day(2019, 2, 31, 3)
  expect_identical(calendar_start(x, "year"), year_month_day(2019, 1, 1, 0))
  expect_identical(calendar_start(x, "day"), year_month_day(2019, 2, 31, 0))
})

# ------------------------------------------------------------------------------
# calendar_end()

test_that("end: `x` is validated", {
  expect_snapshot_error(calendar_end(1))
})

test_that("end: `precision` is validated", {
  expect_snapshot_error(calendar_end(year_month_day(2019), "foo"))
  expect_snapshot_error(calendar_end(year_month_day(2019), 1))
})

test_that("end: errors on unsupported precision", {
  expect_snapshot_error(calendar_end(year_month_day(2019, 1), "quarter"))
})

test_that("end: `precision` can't be more precise than `x`", {
  expect_snapshot_error(calendar_end(year_month_day(2019), "month"))
})

test_that("end: can't mix different subsecond precisions", {
  x <- year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "microsecond")
  expect_snapshot_error(calendar_end(x, "millisecond"))
})

test_that("end: can use same subsecond precision", {
  x <- year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "microsecond")
  expect_identical(calendar_end(x, "microsecond"), x)
})

test_that("end: can compute day end", {
  x <- year_month_day(2019, 2, 2)
  expect_identical(calendar_end(x, "day"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "nanosecond")
  expect <- year_month_day(2019, 2, 2, 23, 59, 59, 999999999L, subsecond_precision = "nanosecond")
  expect_identical(calendar_end(x, "day"), expect)
})

test_that("end: can compute hour end", {
  x <- year_month_day(2019, 2, 2, 2)
  expect_identical(calendar_end(x, "hour"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "nanosecond")
  expect <- year_month_day(2019, 2, 2, 2, 59, 59, 999999999L, subsecond_precision = "nanosecond")
  expect_identical(calendar_end(x, "hour"), expect)
})

test_that("end: can compute minute end", {
  x <- year_month_day(2019, 2, 2, 2, 2)
  expect_identical(calendar_end(x, "minute"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "nanosecond")
  expect <- year_month_day(2019, 2, 2, 2, 2, 59, 999999999L, subsecond_precision = "nanosecond")
  expect_identical(calendar_end(x, "minute"), expect)
})

test_that("end: can compute second end", {
  x <- year_month_day(2019, 2, 2, 2, 2, 2)
  expect_identical(calendar_end(x, "second"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "nanosecond")
  expect <- year_month_day(2019, 2, 2, 2, 2, 2, 999999999L, subsecond_precision = "nanosecond")
  expect_identical(calendar_end(x, "second"), expect)
})

test_that("end: invalid dates are adjusted", {
  x <- year_month_day(2019, 2, 31, 3)
  expect_identical(calendar_end(x, "year"), year_month_day(2019, 12, 31, 23))
  expect_identical(calendar_end(x, "month"), year_month_day(2019, 2, 28, 23))
  expect_identical(calendar_end(x, "day"), year_month_day(2019, 2, 31, 23))
})

# ------------------------------------------------------------------------------
# calendar_count_between()

test_that("`n` gets used", {
  x <- year_month_day(2019, 1)
  y <- year_month_day(2019, 7)
  expect_identical(calendar_count_between(x, y, "month", n = 2), 3L)
})

test_that("`end` must be a calendar", {
  x <- year_month_day(2019)
  expect_snapshot((expect_error(calendar_count_between(x, 1, "year"))))
})

test_that("can't count with a precision finer than the calendar precision", {
  x <- year_month_day(2019)
  expect_snapshot((expect_error(calendar_count_between(x, x, "month"))))
})

test_that("`n` is validated", {
  x <- year_month_day(2019)

  expect_snapshot({
    (expect_error(calendar_count_between(x, x, "year", n = NA_integer_)))
    (expect_error(calendar_count_between(x, x, "year", n = -1)))
    (expect_error(calendar_count_between(x, x, "year", n = 1.5)))
    (expect_error(calendar_count_between(x, x, "year", n = "x")))
    (expect_error(calendar_count_between(x, x, "year", n = c(1L, 2L))))
  })
})

# ------------------------------------------------------------------------------
# calendar_precision()

test_that("precision: can get the precision", {
  expect_identical(calendar_precision(year_month_day(2019, 1)), "month")
  expect_identical(calendar_precision(year_day(2019, 100)), "day")
  expect_identical(calendar_precision(year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")), "nanosecond")
})

test_that("precision: can only be called on calendars", {
  expect_snapshot_error(calendar_precision(sys_days(0)))
})
