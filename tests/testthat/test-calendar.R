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
# calendar_precision()

test_that("precision: can get the precision", {
  expect_identical(calendar_precision(year_month_day(2019, 1)), "month")
  expect_identical(calendar_precision(year_day(2019, 100)), "day")
  expect_identical(calendar_precision(year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")), "nanosecond")
})

test_that("precision: can only be called on calendars", {
  expect_snapshot_error(calendar_precision(sys_days(0)))
})
