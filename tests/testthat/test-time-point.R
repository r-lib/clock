# ------------------------------------------------------------------------------
# print() / obj_print_data() / obj_print_footer()

test_that("normal print method works", {
  x <- as_sys_time(year_month_day(2019, 1:5, 1))
  expect_snapshot(x)
})

test_that("can limit with `max`", {
  x <- as_sys_time(year_month_day(2019, 1:5, 1))

  expect_snapshot(print(x, max = 2))
  expect_snapshot(print(x, max = 4))

  # no footer if length >= max
  expect_snapshot(print(x, max = 5))
  expect_snapshot(print(x, max = 6))
})

test_that("`max` defaults to `getOption('max.print')` but can be overridden", {
  local_options(max.print = 3)

  x <- as_naive_time(year_month_day(2019, 1:5, 1))

  expect_snapshot(x)
  expect_snapshot(print(x, max = 4))
  expect_snapshot(print(x, max = 5))
})

# ------------------------------------------------------------------------------
# time_point_floor() / _ceiling() / _round()

test_that("can round to less precise precision", {
  x <- naive_seconds(c(-86401, -86400, -86399, 0, 86399, 86400, 86401))

  floor <- naive_days(c(-2, -1, -1, 0, 0, 1, 1))
  ceiling <- naive_days(c(-1, -1, 0, 0, 1, 1, 2))
  round <- naive_days(c(-1, -1, -1, 0, 1, 1, 1))

  expect_identical(time_point_floor(x, "day"), floor)
  expect_identical(time_point_ceiling(x, "day"), ceiling)
  expect_identical(time_point_round(x, "day"), round)

  floor <- naive_days(c(-2, -2, -2, 0, 0, 0, 0))
  ceiling <- naive_days(c(0, 0, 0, 0, 2, 2, 2))
  round <- naive_days(c(-2, 0, 0, 0, 0, 2, 2))

  expect_identical(time_point_floor(x, "day", n = 2), floor)
  expect_identical(time_point_ceiling(x, "day", n = 2), ceiling)
  expect_identical(time_point_round(x, "day", n = 2), round)
})

test_that("can round with `origin` altering starting point", {
  x <- sys_seconds(c(-86401, -86400, -86399, 0, 86399, 86400, 86401))

  origin <- sys_days(-1)

  floor <- sys_days(c(-3, -1, -1, -1, -1, 1, 1))
  ceiling <- sys_days(c(-1, -1, 1, 1, 1, 1, 3))
  round <- sys_days(c(-1, -1, -1, 1, 1, 1, 1))

  expect_identical(time_point_floor(x, "day", origin = origin, n = 2), floor)
  expect_identical(time_point_ceiling(x, "day", origin = origin, n = 2), ceiling)
  expect_identical(time_point_round(x, "day", origin = origin, n = 2), round)
})

test_that("cannot floor to more precise precision", {
  expect_snapshot_error(time_point_floor(naive_days(), "second"))
})

test_that("rounding with `origin` requires same clock", {
  origin <- sys_days(0)
  x <- naive_days(0)
  expect_snapshot_error(time_point_floor(x, "day", origin = origin))
})

test_that("`origin` can be cast to a more precise `precision`, but not to a less precise one", {
  origin1 <- as_naive_time(duration_days(1))
  origin2 <- as_naive_time(duration_milliseconds(0))
  x <- naive_seconds(0)

  expect_identical(
    time_point_floor(x, "hour", origin = origin1, n = 5),
    time_point_floor(x - as_duration(origin1), "hour", n = 5) + as_duration(origin1)
  )

  expect_snapshot_error(time_point_floor(x, "hour", origin = origin2))
})

test_that("`origin` must be size 1", {
  origin <- naive_days(0:1)
  x <- naive_days(0)
  expect_snapshot_error(time_point_floor(x, "day", origin = origin))
})

test_that("`origin` must not be `NA`", {
  origin <- naive_days(NA)
  x <- naive_days(0)
  expect_snapshot_error(time_point_floor(x, "day", origin = origin))
})

test_that("`origin` can't be Date or POSIXt", {
  origin1 <- new_date(0)
  origin2 <- new_datetime(0, "America/New_York")
  x <- naive_days(0)
  expect_snapshot_error(time_point_floor(x, "day", origin = origin1))
  expect_snapshot_error(time_point_floor(x, "day", origin = origin2))
})

# ------------------------------------------------------------------------------
# time_point_shift()

test_that("can shift to next weekday", {
  expect_identical(
    time_point_shift(
      naive_days(0:1),
      weekday(clock_weekdays$sunday)
    ),
    naive_days(c(3, 3))
  )
})

test_that("can shift to next if on the boundary", {
  naive_sunday <- naive_days(3)
  sunday <- weekday(clock_weekdays$sunday)

  expect_identical(
    time_point_shift(naive_sunday, sunday),
    naive_sunday
  )
  expect_identical(
    time_point_shift(naive_sunday, sunday, boundary = "advance"),
    naive_sunday + 7
  )
})

test_that("can shift to previous weekday", {
  expect_identical(
    time_point_shift(
      naive_days(0:1),
      weekday(clock_weekdays$sunday),
      which = "previous"
    ),
    naive_days(c(-4, -4))
  )
})

test_that("can shift to previous weekday if on boundary", {
  naive_sunday <- naive_days(3)
  sunday <- weekday(clock_weekdays$sunday)

  expect_identical(
    time_point_shift(naive_sunday, sunday, which = "previous"),
    naive_sunday
  )
  expect_identical(
    time_point_shift(naive_sunday, sunday, which = "previous", boundary = "advance"),
    naive_sunday - 7
  )
})

test_that("`target` is recycled to size of `x`", {
  expect_identical(
    time_point_shift(
      sys_days(0:1),
      weekday(1:2)
    ),
    sys_days(3:4)
  )

  expect_snapshot_error(time_point_shift(sys_days(0), weekday(1:2)))
})

test_that("`x` is validated", {
  expect_snapshot_error(time_point_shift(1))
})

test_that("`target` is validated", {
  expect_snapshot_error(time_point_shift(sys_days(0), 1))
})

test_that("`which` is validated", {
  expect_snapshot_error(time_point_shift(sys_days(), weekday(), which = 1))
  expect_snapshot_error(time_point_shift(sys_days(), weekday(), which = "foo"))
  expect_snapshot_error(time_point_shift(sys_days(), weekday(), which = c("next", "previous")))
})

test_that("`boundary` is validated", {
  expect_snapshot_error(time_point_shift(sys_days(), weekday(), boundary = 1))
  expect_snapshot_error(time_point_shift(sys_days(), weekday(), boundary = "foo"))
  expect_snapshot_error(time_point_shift(sys_days(), weekday(), boundary = c("keep", "advance")))
})

# ------------------------------------------------------------------------------
# time_point_count_between()

test_that("can count units between", {
  x <- as_naive_time(year_month_day(1990, 02, 03, 04))
  y <- as_naive_time(year_month_day(1995, 04, 05, 03))

  expect_identical(time_point_count_between(x, y, "day"), 1886L)
  expect_identical(time_point_count_between(x, y, "hour"), 45287L)
})

test_that("'week' is an allowed precision", {
  x <- sys_days(0)
  y <- sys_days(13:15)

  expect_identical(time_point_count_between(x, y, "week"), c(1L, 2L, 2L))
})

test_that("`n` affects the result", {
  x <- sys_days(0)
  y <- sys_days(10)

  expect_identical(time_point_count_between(x, y, "day", n = 2L), 5L)
  expect_identical(time_point_count_between(x, y, "day", n = 3L), 3L)
})

test_that("negative vs positive differences are handled correctly", {
  one_hour <- duration_hours(1)

  x <- sys_days(0)
  y <- sys_days(1)
  z <- sys_days(-1)

  expect_identical(time_point_count_between(x, y - one_hour, "day"), 0L)
  expect_identical(time_point_count_between(x, y, "day"), 1L)
  expect_identical(time_point_count_between(x, y + one_hour, "day"), 1L)

  expect_identical(time_point_count_between(x, z - one_hour, "day"), -1L)
  expect_identical(time_point_count_between(x, z, "day"), -1L)
  expect_identical(time_point_count_between(x, z + one_hour, "day"), 0L)
})

test_that("common precision of inputs and `precision` is taken", {
  expect_identical(
    time_point_count_between(sys_days(0), sys_days(2) + duration_hours(1), "second"),
    176400L
  )
  expect_identical(
    time_point_count_between(sys_seconds(0), sys_seconds(86401), "day"),
    1L
  )
})

test_that("OOB results return a warning and NA", {
  expect_snapshot({
    out <- time_point_count_between(sys_days(0), sys_days(1000), "nanosecond")
  })
  expect_identical(out, NA_integer_)
})

test_that("both inputs must be time points", {
  expect_snapshot({
    (expect_error(time_point_count_between(sys_days(1), 1)))
    (expect_error(time_point_count_between(1, sys_days(1))))
  })
})

test_that("both inputs must be compatible", {
  x <- sys_days(1)
  y <- naive_days(1)

  expect_snapshot((expect_error(
    time_point_count_between(x, y)
  )))
})

test_that("`n` is validated", {
  x <- sys_days(1)

  expect_snapshot({
    (expect_error(time_point_count_between(x, x, "day", n = NA_integer_)))
    (expect_error(time_point_count_between(x, x, "day", n = -1)))
    (expect_error(time_point_count_between(x, x, "day", n = 1.5)))
    (expect_error(time_point_count_between(x, x, "day", n = "x")))
    (expect_error(time_point_count_between(x, x, "day", n = c(1L, 2L))))
  })
})

test_that("`precision` must be a time point precision", {
  x <- sys_days(1)

  expect_snapshot((expect_error(
    time_point_count_between(x, x, "year")
  )))
})

# ------------------------------------------------------------------------------
# seq()

test_that("seq(to, by) works", {
  expect_identical(seq(sys_days(0L), to = sys_days(4L), by = 2), sys_days(c(0L, 2L, 4L)))
  expect_identical(seq(sys_days(0L), to = sys_days(5L), by = 2), sys_days(c(0L, 2L, 4L)))

  expect_identical(seq(sys_seconds(0L), to = sys_seconds(-4L), by = -2), sys_seconds(c(0L, -2L, -4L)))
  expect_identical(seq(sys_seconds(0L), to = sys_seconds(-5L), by = -2), sys_seconds(c(0L, -2L, -4L)))
})

test_that("seq(to, length.out) works", {
  expect_identical(seq(naive_days(0L), to = naive_days(4L), length.out = 2), naive_days(c(0L, 4L)))
  expect_identical(seq(naive_days(0L), to = naive_days(4L), length.out = 1), naive_days(c(0L)))
  expect_identical(seq(naive_days(0L), to = naive_days(4L), length.out = 5), naive_days(c(0:4)))

  expect_identical(seq(naive_seconds(0L), to = naive_seconds(4L), along.with = 1:2), naive_seconds(c(0L, 4L)))
})

test_that("seq(by, length.out) works", {
  expect_identical(seq(naive_seconds(0L), by = 2, length.out = 3), naive_seconds(c(0L, 2L, 4L)))
  expect_identical(seq(naive_seconds(0L), by = -2, length.out = 3), naive_seconds(c(0L, -2L, -4L)))

  expect_identical(seq(naive_seconds(0L), by = 2, along.with = 1:3), naive_seconds(c(0L, 2L, 4L)))
})

test_that("`by` can be a duration", {
  expect_identical(
    seq(naive_seconds(0), to = naive_seconds(1000), by = duration_minutes(1)),
    seq(naive_seconds(0), to = naive_seconds(1000), by = 60)
  )
  expect_identical(
    seq(as_naive_time(duration_nanoseconds(0)), to = as_naive_time(duration_nanoseconds(2e9)), by = duration_seconds(1)),
    seq(as_naive_time(duration_nanoseconds(0)), to = as_naive_time(duration_nanoseconds(2e9)), by = 1e9)
  )
})

test_that("can't mix chronological time points and calendrical durations", {
  expect_snapshot_error(seq(naive_seconds(0), by = duration_years(1), length.out = 2))
})

test_that("can't mix clocks in seq()", {
  expect_snapshot_error(seq(sys_seconds(0), to = naive_seconds(5), by = 1))
})

test_that("`to` is always cast to `from`", {
  expect_identical(
    seq(naive_seconds(0), to = naive_days(12), by = duration_days(2)),
    seq(naive_seconds(0), to = naive_seconds(12 * 86400), by = 86400 * 2)
  )

  expect_snapshot_error(seq(naive_days(0), to = naive_seconds(5), by = 2))
})

test_that("can make nanosecond precision seqs", {
  x <- as_naive_time(duration_nanoseconds(0))
  y <- as_naive_time(duration_nanoseconds(10))

  expect_identical(seq(x, by = 2, length.out = 5), x + c(0, 2, 4, 6, 8))
  expect_identical(seq(x, y, by = 3), x + c(0, 3, 6, 9))
})

# ------------------------------------------------------------------------------
# vec_arith()

test_that("duration to add to a time-point must have at least week precision (#120)", {
  expect_snapshot_error(naive_seconds(0) + duration_years(1))
})

# ------------------------------------------------------------------------------
# time_point_precision()

test_that("precision: can get the precision", {
  expect_identical(time_point_precision(as_naive_time(duration_days(2:5))), "day")
  expect_identical(time_point_precision(as_naive_time(duration_nanoseconds(2:5))), "nanosecond")
})

test_that("precision: can only be called on time points", {
  expect_snapshot_error(time_point_precision(duration_days()))
})
