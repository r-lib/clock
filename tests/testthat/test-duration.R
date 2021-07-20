# ------------------------------------------------------------------------------
# duration_precision_common_cpp()

test_that("correctly computes common duration precision", {
  granular <- c(
    PRECISION_YEAR,
    PRECISION_QUARTER,
    PRECISION_MONTH
  )

  precise <- c(
    PRECISION_WEEK,
    PRECISION_DAY,
    PRECISION_HOUR,
    PRECISION_MINUTE,
    PRECISION_SECOND,
    PRECISION_MILLISECOND,
    PRECISION_MICROSECOND,
    PRECISION_NANOSECOND
  )

  for (p1 in granular) {
    for (p2 in granular) {
      expect_identical(duration_precision_common_cpp(p1, p2), max(p1, p2))
    }
  }

  for (p1 in precise) {
    for (p2 in precise) {
      expect_identical(duration_precision_common_cpp(p1, p2), max(p1, p2))
    }
  }

  for (p1 in granular) {
    for (p2 in precise) {
      expect_identical(duration_precision_common_cpp(p1, p2), NA_integer_)
    }
  }
})

# ------------------------------------------------------------------------------
# duration_floor() / _ceiling() / _round()

test_that("floor rounds down", {
  x <- duration_days(2) + duration_seconds(-1:1)
  x <- c(-x, x)

  expect2 <- duration_seconds(c(-172800, -172800, -172802, 172798, 172800, 172800))
  expect3 <- duration_days(c(-2, -2, -3, 1, 2, 2))
  expect4 <- duration_days(c(-2, -2, -4, 0, 2, 2))

  expect_identical(duration_floor(x, "second"), x)
  expect_identical(duration_floor(x, "second", n = 2), expect2)
  expect_identical(duration_floor(x, "day"), expect3)
  expect_identical(duration_floor(x, "day", n = 2), expect4)
})

test_that("ceiling rounds up", {
  x <- duration_days(2) + duration_seconds(-1:1)
  x <- c(-x, x)

  expect2 <- duration_seconds(c(-172798, -172800, -172800, 172800, 172800, 172802))
  expect3 <- duration_days(c(-1, -2, -2, 2, 2, 3))
  expect4 <- duration_days(c(0, -2, -2, 2, 2, 4))

  expect_identical(duration_ceiling(x, "second"), x)
  expect_identical(duration_ceiling(x, "second", n = 2), expect2)
  expect_identical(duration_ceiling(x, "day"), expect3)
  expect_identical(duration_ceiling(x, "day", n = 2), expect4)
})

test_that("round rounds to nearest, ties round up", {
  x <- duration_days(2) + duration_seconds(-1:3)
  x <- c(-x, x)

  expect2 <- duration_seconds(c(-172800, -172800, -172800, -172800, -172804, 172800, 172800, 172800, 172804, 172804))
  expect3 <- duration_days(c(-2, -2, -2, -2, -2, 2, 2, 2, 2, 2))
  expect4 <- duration_days(c(0, 0, -4, -4, -4, 0, 4, 4, 4, 4))

  expect_identical(duration_round(x, "second"), x)
  expect_identical(duration_round(x, "second", n = 4), expect2)
  expect_identical(duration_round(x, "day"), expect3)
  expect_identical(duration_round(x, "day", n = 4), expect4)
})

test_that("can't round to more precise precision", {
  expect_error(duration_floor(duration_seconds(1), "millisecond"), "more precise")
})

test_that("can't round across common precision boundary", {
  expect_snapshot_error(duration_ceiling(duration_weeks(), "month"))
  expect_snapshot_error(duration_floor(duration_seconds(), "year"))
})

test_that("input is validated", {
  expect_error(duration_floor(1, "year"), "must be a duration object")
  expect_error(duration_floor(duration_seconds(1), "foo"), "not recognized")
  expect_error(duration_floor(duration_seconds(1), "day", n = -1), "positive number")
})

# ------------------------------------------------------------------------------
# seq()

test_that("seq() validates from", {
  expect_snapshot_error(seq(duration_years(1:2)), class = "vctrs_error_assert_size")
  expect_snapshot_error(seq(duration_years(NA_integer_)))
})

test_that("seq() validates length.out / along.with exclusiveness", {
  expect_snapshot_error(seq(duration_years(1L), length.out = 1, along.with = 2))
})

test_that("seq() only takes two optional args", {
  x <- duration_years(1L)
  expect_snapshot_error(seq(x, to = duration_years(1), by = 1, length.out = 1))
  expect_snapshot_error(seq(x, to = duration_years(1), by = 1, along.with = 1))
})

test_that("seq() requires two optional args", {
  x <- duration_years(1L)
  expect_snapshot_error(seq(x, to = duration_years(1)))
  expect_snapshot_error(seq(x, by = 1))
  expect_snapshot_error(seq(x, length.out = 1))
  expect_snapshot_error(seq(x, along.with = 1))
})

test_that("seq() validates `to`", {
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(1:2), by = 1), class = "vctrs_error_assert_size")
  expect_snapshot_error(seq(duration_years(1L), to = 1, by = 1))
  expect_snapshot_error(seq(duration_years(1L), to = duration_days(1), by = 1))
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(NA_integer_), by = 1))
})

test_that("seq() validates `by`", {
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(1L), by = 1:2), class = "vctrs_error_assert_size")
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(1L), by = NA_integer_))
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(1L), by = 0))
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(1L), by = duration_years(0)))
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(1L), by = "x"), class = "vctrs_error_incompatible_type")
})

test_that("`by` must be castable to the type of `from`", {
  expect_snapshot_error(seq(duration_years(0), to = duration_years(1), by = duration_months(1)))
  expect_snapshot_error(seq(duration_years(0), to = duration_years(1), by = duration_days(1)))
  expect_snapshot_error(seq(duration_days(0), to = duration_days(1), by = duration_years(1)))
})

test_that("seq() validates `length.out`", {
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(1L), length.out = 1:2), class = "vctrs_error_assert_size")
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(1L), length.out = NA_integer_))
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(1L), length.out = -1))
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(1L), length.out = "x"), class = "vctrs_error_incompatible_type")
})

test_that("seq() validates dots", {
  expect_snapshot_error(seq(duration_years(1), duration_years(1), 1, 1, 1, 1))
})

test_that("seq() validates from/to/by signs", {
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(2L), by = -1))
  expect_snapshot_error(seq(duration_years(2L), to = duration_years(1L), by = 1))
})

test_that("seq() enforces non-fractional results", {
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(2L), length.out = 3))
  expect_snapshot_error(seq(duration_years(1L), to = duration_years(2L), along.with = 1:3))
})

test_that("seq(to, by) works", {
  expect_identical(seq(duration_years(0L), to = duration_years(4L), by = 2), duration_years(c(0L, 2L, 4L)))
  expect_identical(seq(duration_years(0L), to = duration_years(5L), by = 2), duration_years(c(0L, 2L, 4L)))

  expect_identical(seq(duration_years(0L), to = duration_years(-4L), by = -2), duration_years(c(0L, -2L, -4L)))
  expect_identical(seq(duration_years(0L), to = duration_years(-5L), by = -2), duration_years(c(0L, -2L, -4L)))

  expect_identical(seq(duration_years(4L), to = duration_years(0L), by = -2), duration_years(c(4L, 2L, 0L)))
  expect_identical(seq(duration_years(4L), to = duration_years(-1L), by = -2), duration_years(c(4L, 2L, 0L)))
})

test_that("seq(to, by = <duration>) works", {
  expect_identical(
    seq(duration_years(0), to = duration_years(4), by = duration_years(1)),
    seq(duration_years(0), to = duration_years(4), by = 1)
  )
  expect_identical(
    seq(duration_months(0), to = duration_months(20), by = duration_years(1)),
    seq(duration_months(0), to = duration_months(20), by = 12)
  )
  expect_identical(
    seq(duration_seconds(0), to = duration_seconds(1000), by = duration_minutes(2)),
    seq(duration_seconds(0), to = duration_seconds(1000), by = 120)
  )
  expect_identical(
    seq(duration_nanoseconds(0), by = duration_days(2), length.out = 5),
    duration_nanoseconds(0) + duration_days(c(0, 2, 4, 6, 8))
  )
  expect_identical(
    seq(duration_nanoseconds(0), to = duration_days(100000), by = duration_days(10000)),
    duration_nanoseconds(0) + duration_days(seq(0L, 100000L, by = 10000L))
  )
  expect_identical(
    seq(duration_nanoseconds(0), to = -duration_days(100000), by = -duration_days(10000)),
    duration_nanoseconds(0) - duration_days(seq(0L, 100000L, by = 10000L))
  )
})

test_that("seq(to, length.out) works", {
  expect_identical(seq(duration_years(0L), to = duration_years(4L), length.out = 2), duration_years(c(0L, 4L)))
  expect_identical(seq(duration_years(0L), to = duration_years(4L), length.out = 1), duration_years(c(0L)))
  expect_identical(seq(duration_years(0L), to = duration_years(4L), length.out = 5), duration_years(c(0:4)))

  expect_identical(seq(duration_years(0L), to = duration_years(-4L), length.out = 2), duration_years(c(0L, -4L)))
  expect_identical(seq(duration_years(0L), to = duration_years(-6L), length.out = 3), duration_years(c(0L, -3L, -6L)))

  expect_identical(seq(duration_years(0L), to = duration_years(4L), along.with = 1:2), duration_years(c(0L, 4L)))
})

test_that("seq(to, length.out = 1) is special cased to return `from`", {
  expect_identical(
    seq(duration_years(1), duration_years(5), length.out = 1),
    duration_years(1)
  )
})

test_that("seq(by, length.out) works", {
  expect_identical(seq(duration_years(0L), by = 2, length.out = 3), duration_years(c(0L, 2L, 4L)))
  expect_identical(seq(duration_years(0L), by = -2, length.out = 3), duration_years(c(0L, -2L, -4L)))

  expect_identical(seq(duration_years(0L), by = 2, along.with = 1:3), duration_years(c(0L, 2L, 4L)))
})

test_that("`to` is always cast to `from`", {
  expect_identical(
    seq(duration_months(0), to = duration_years(1), by = 2),
    seq(duration_months(0), to = duration_months(12), by = 2)
  )

  expect_snapshot_error(seq(duration_days(0), to = duration_years(5), by = 2))
  expect_snapshot_error(seq(duration_years(0), to = duration_months(5), by = 2))
})

test_that("special test to ensure we never lose precision (i.e. by trying to convert to double)", {
  expect_identical(
    seq(duration_nanoseconds(0), duration_cast(duration_years(10), "nanosecond"), length.out = 3),
    duration_nanoseconds(0) + duration_cast(duration_years(c(0, 5, 10)), "nanosecond")
  )
})

# ------------------------------------------------------------------------------
# add_*()

test_that("can't add chronological and calendrical durations", {
  expect_snapshot_error(add_seconds(duration_years(1), 1))
  expect_snapshot_error(add_years(duration_seconds(1), 1))
})

# ------------------------------------------------------------------------------
# as_sys_time() / as_naive_time()

test_that("can convert week precision duration to time point", {
  expect_identical(as_sys_time(duration_weeks(c(0, 1))), sys_days(c(0, 7)))
  expect_identical(as_naive_time(duration_weeks(c(0, 1))), naive_days(c(0, 7)))
})

test_that("can't convert calendrical duration to time point", {
  expect_snapshot_error(as_sys_time(duration_years(0)))
  expect_snapshot_error(as_naive_time(duration_years(0)))
})

# ------------------------------------------------------------------------------
# duration_precision()

test_that("precision: can get the precision", {
  expect_identical(duration_precision(duration_months(2:5)), "month")
  expect_identical(duration_precision(duration_days(1)), "day")
  expect_identical(duration_precision(duration_nanoseconds(5:6)), "nanosecond")
})

test_that("precision: can only be called on durations", {
  expect_snapshot_error(duration_precision(sys_days(0)))
})

# ------------------------------------------------------------------------------
# vec_math()

test_that("is.nan() works", {
  x <- duration_years(c(1, NA))
  expect_identical(is.nan(x), c(FALSE, FALSE))
})

test_that("is.finite() works", {
  x <- duration_years(c(1, NA))
  expect_identical(is.finite(x), c(TRUE, FALSE))
})

test_that("is.infinite() works", {
  x <- duration_years(c(1, NA))
  expect_identical(is.infinite(x), c(FALSE, FALSE))
})

test_that("abs() works", {
  x <- duration_hours(c(-2, -1, 0, 1, 2, NA))
  expect <- duration_hours(c(2, 1, 0, 1, 2, NA))
  expect_identical(abs(x), expect)
})

test_that("abs() propagates names", {
  x <- set_names(duration_years(1:2), c("a", "b"))
  expect_named(abs(x), c("a", "b"))
})

test_that("sign() works", {
  x <- duration_hours(c(-2, -1, 0, 1, 2, NA))
  expect <- c(-1L, -1L, 0L, 1L, 1L, NA)
  expect_identical(sign(x), expect)
})

test_that("sign() propagates names", {
  x <- set_names(duration_years(1:2), c("a", "b"))
  expect_named(sign(x), c("a", "b"))
})
