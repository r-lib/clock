# ------------------------------------------------------------------------------
# vec_ptype_full()

test_that("full ptype is correct", {
  expect_snapshot_output(vec_ptype_full(year_quarter_day(2019)))
  expect_snapshot_output(vec_ptype_full(year_quarter_day(2019, start = 2)))
  expect_snapshot_output(vec_ptype_full(year_quarter_day(2019, 1, 1)))
  expect_snapshot_output(vec_ptype_full(year_quarter_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")))
  expect_snapshot_output(vec_ptype_full(year_quarter_day(2019, 1, 92)))
})

# ------------------------------------------------------------------------------
# vec_ptype_abbr()

test_that("abbreviated ptype is correct", {
  expect_snapshot_output(vec_ptype_abbr(year_quarter_day(2019)))
  expect_snapshot_output(vec_ptype_abbr(year_quarter_day(2019, start = 2)))
  expect_snapshot_output(vec_ptype_abbr(year_quarter_day(2019, 1, 1)))
  expect_snapshot_output(vec_ptype_abbr(year_quarter_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")))
  expect_snapshot_output(vec_ptype_abbr(year_quarter_day(2019, 1, 92)))
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- year_quarter_day(2019, 1)
  y <- year_quarter_day(2019, 1, 2)

  expect_identical(as.character(x), format(x))
  expect_identical(as.character(y), format(y))
})

# ------------------------------------------------------------------------------
# calendar_narrow()

test_that("can narrow to week", {
  x_expect <- year_quarter_day(2019, 2)
  x <- set_day(x_expect, 1)
  expect_identical(calendar_narrow(x, "quarter"), x_expect)
  expect_identical(calendar_narrow(x_expect, "quarter"), x_expect)
})

test_that("can narrow to day", {
  x_expect <- year_quarter_day(2019, 2, 3)
  x <- set_hour(x_expect, 5)
  expect_identical(calendar_narrow(x, "day"), x_expect)
  expect_identical(calendar_narrow(x_expect, "day"), x_expect)
})

# ------------------------------------------------------------------------------
# calendar_widen()

test_that("can widen to quarter", {
  x <- year_quarter_day(2019)
  expect_identical(calendar_widen(x, "quarter"), set_quarter(x, 1))
})

test_that("can widen to day", {
  x <- year_quarter_day(2019)
  y <- year_quarter_day(2019, 02)
  expect_identical(calendar_widen(x, "day"), set_day(set_quarter(x, 1), 1))
  expect_identical(calendar_widen(y, "day"), set_day(y, 1))
})

# ------------------------------------------------------------------------------
# seq()

test_that("only granular precisions are allowed", {
  expect_snapshot_error(seq(year_quarter_day(2019, 1, 1), by = 1, length.out = 2))
})

test_that("seq(to, by) works", {
  expect_identical(seq(year_quarter_day(2019, 1), to = year_quarter_day(2020, 2), by = 2), year_quarter_day(c(2019, 2019, 2020), c(1, 3, 1)))
  expect_identical(seq(year_quarter_day(2019, 1), to = year_quarter_day(2020, 1), by = 2), year_quarter_day(c(2019, 2019, 2020), c(1, 3, 1)))
})

test_that("seq(to, length.out) works", {
  expect_identical(seq(year_quarter_day(2019, 1), to = year_quarter_day(2020, 2), length.out = 2), year_quarter_day(c(2019, 2020), c(1, 2)))
  expect_identical(seq(year_quarter_day(2019, 1), to = year_quarter_day(2020, 2), length.out = 6), year_quarter_day(2019, 1) + 0:5)

  expect_identical(seq(year_quarter_day(2019, 1), to = year_quarter_day(2020, 2), along.with = 1:2), year_quarter_day(c(2019, 2020), c(1, 2)))
})

test_that("seq(by, length.out) works", {
  expect_identical(seq(year_quarter_day(2019, 1), by = 2, length.out = 3), year_quarter_day(2019, 1) + c(0, 2, 4))
  expect_identical(seq(year_quarter_day(2019, 1), by = -2, length.out = 3), year_quarter_day(2019, 1) + c(0, -2, -4))

  expect_identical(seq(year_quarter_day(2019, 1), by = 2, along.with = 1:3), year_quarter_day(2019, 1) + c(0, 2, 4))
})

# ------------------------------------------------------------------------------
# add_*()

test_that("can add quarters", {
  expect_identical(year_quarter_day(2019, 1, 1) + duration_quarters(1), year_quarter_day(2019, 2, 1))
  expect_identical(year_quarter_day(2019, 1, 1) + duration_quarters(5), year_quarter_day(2020, 2, 1))

  expect_identical(year_quarter_day(2019, 1, 1, start = 2) + duration_quarters(5), year_quarter_day(2020, 2, 1, start = 2))
})

# ------------------------------------------------------------------------------
# miscellaneous

test_that("can roundtrip to naive-time with any `start`", {
  x <- seq(
    as_naive(year_month_day(-9999, 1, 1)),
    as_naive(year_month_day(9999, 12, 31)),
    by = 1
  )

  for (start in seq_len(12)) {
    expect_identical(x, as_naive(as_year_quarter_day(x, start = start)))
  }
})

test_that("can generate correct last days of the quarter with any `start`", {
  start <- 1L

  expect_identical(
    as.Date(year_quarter_day(2019, 1:4, "last", start = 1)),
    as.Date(year_month_day(2019, c(3, 6, 9, 12), "last"))
  )
  expect_identical(
    as.Date(year_quarter_day(2019, 1:4, "last", start = 2)),
    as.Date(year_month_day(c(2018, 2018, 2018, 2019), c(4, 7, 10, 1), "last"))
  )
  expect_identical(
    as.Date(year_quarter_day(2019, 1:4, "last", start = 3)),
    as.Date(year_month_day(c(2018, 2018, 2018, 2019), c(5, 8, 11, 2), "last"))
  )
  # ..., 4, 5, 6, 7, 8, 9, ...
  expect_identical(
    as.Date(year_quarter_day(2019, 1:4, "last", start = 10)),
    as.Date(year_month_day(c(2018, 2019, 2019, 2019), c(12, 3, 6, 9), "last"))
  )
  expect_identical(
    as.Date(year_quarter_day(2019, 1:4, "last", start = 11)),
    as.Date(year_month_day(2019, c(1, 4, 7, 10), "last"))
  )
  expect_identical(
    as.Date(year_quarter_day(2019, 1:4, "last", start = 12)),
    as.Date(year_month_day(2019, c(2, 5, 8, 11), "last"))
  )

  # Leap year!
  expect_identical(
    as.Date(year_quarter_day(2020, 1:4, "last", start = 1)),
    as.Date(year_month_day(2020, c(3, 6, 9, 12), "last"))
  )
  expect_identical(
    as.Date(year_quarter_day(2020, 1:4, "last", start = 2)),
    as.Date(year_month_day(c(2019, 2019, 2019, 2020), c(4, 7, 10, 1), "last"))
  )
  expect_identical(
    as.Date(year_quarter_day(2020, 1:4, "last", start = 3)),
    as.Date(year_month_day(c(2019, 2019, 2019, 2020), c(5, 8, 11, 2), "last"))
  )
})

# ------------------------------------------------------------------------------
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot_error(invalid_resolve(year_quarter_day(2019, 1, 1)))
})
