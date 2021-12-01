# ------------------------------------------------------------------------------
# year_quarter_day()

test_that("helper can create different precisions", {
  x <- year_quarter_day(2019, 1:2)
  expect_identical(get_year(x), c(2019L, 2019L))
  expect_identical(get_quarter(x), 1:2)

  x <- year_quarter_day(2019, 1:2, 3)
  expect_identical(get_day(x), c(3L, 3L))
})

test_that("can create subsecond precision calendars", {
  x <- year_quarter_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "millisecond")
  expect_identical(get_millisecond(x), 1L)

  x <- year_quarter_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "microsecond")
  expect_identical(get_microsecond(x), 1L)

  x <- year_quarter_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "nanosecond")
  expect_identical(get_nanosecond(x), 1L)
})

test_that("validates value ranges", {
  expect_snapshot_error(year_quarter_day(50000))
  expect_snapshot_error(year_quarter_day(2020, 5))
  expect_snapshot_error(year_quarter_day(2020, 1, 93))
  expect_snapshot_error(year_quarter_day(2020, 1, 1, 24))
  expect_snapshot_error(year_quarter_day(2020, 1, 1, 1, 60))
  expect_snapshot_error(year_quarter_day(2020, 1, 1, 1, 1, 60))
  expect_snapshot_error(year_quarter_day(2020, 1, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond"))
  expect_snapshot_error(year_quarter_day(2020, 1, 1, 1, 1, 1, 1000000, subsecond_precision = "microsecond"))
  expect_snapshot_error(year_quarter_day(2020, 1, 1, 1, 1, 1, 1000000000, subsecond_precision = "nanosecond"))
})

test_that("can get the last day of the quarter", {
  x <- year_quarter_day(2019, 1:2, "last")
  expect_identical(get_day(x), c(90L, 91L))
})

test_that("ignores values past first `NULL`", {
  expect_identical(year_quarter_day(2019, day = 2), year_quarter_day(2019))
})

test_that("NA values propagate", {
  x <- year_quarter_day(2019, 1:3, c(NA, 2, 3), c(3, 4, NA))
  expect_identical(is.na(x), c(TRUE, FALSE, TRUE))
})

# ------------------------------------------------------------------------------
# vec_ptype()

test_that("ptype is correct", {
  base <- year_quarter_day(1)
  ptype <- year_quarter_day(integer())

  for (precision in precision_names()) {
    if (precision == "month" || precision == "week") {
      next
    }

    x <- calendar_widen(base, precision)
    expect <- calendar_widen(ptype, precision)

    expect_identical(vec_ptype(x), expect)
  }
})

# ------------------------------------------------------------------------------
# vec_proxy() / vec_restore()

test_that("proxy is a data frame", {
  expect_identical(vec_proxy(year_quarter_day(2019)), data_frame(year = 2019L))
  expect_identical(vec_proxy(year_quarter_day(2019, 1)), data_frame(year = 2019L, quarter = 1L))
})

test_that("proxy has names on `year`", {
  x <- set_names(year_quarter_day(2019, 1), "nm")
  year <- vec_proxy(x)$year
  expect_named(year, "nm")
})

test_that("restore works", {
  to <- year_quarter_day(2019, 1:4)
  proxy <- vec_slice(vec_proxy(to), 1:2)
  expect_identical(vec_restore(proxy, to), year_quarter_day(2019, 1:2))
})

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
# format()

test_that("default formats are correct", {
  expect_snapshot(format(year_quarter_day(2019)))
  expect_snapshot(format(year_quarter_day(2019, 1)))
  expect_snapshot(format(year_quarter_day(2019, 1, 1, 1)))
  expect_snapshot(format(year_quarter_day(2019, 1, 1, 1, 2, 3, 50, subsecond_precision = "microsecond")))
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
# calendar_start()

test_that("can compute year start", {
  x <- year_quarter_day(2019)
  expect_identical(calendar_start(x, "year"), x)

  x <- year_quarter_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_quarter_day(2019, 1, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  expect_identical(calendar_start(x, "year"), expect)
})

test_that("can compute quarter start", {
  x <- year_quarter_day(2019, 2)
  expect_identical(calendar_start(x, "quarter"), x)

  x <- year_quarter_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_quarter_day(2019, 2, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  expect_identical(calendar_start(x, "quarter"), expect)
})

# ------------------------------------------------------------------------------
# calendar_end()

test_that("can compute year end", {
  x <- year_quarter_day(2019)
  expect_identical(calendar_end(x, "year"), x)

  x <- year_quarter_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_quarter_day(2019, 4, 92, 23, 59, 59, 999L, subsecond_precision = "millisecond")
  expect_identical(calendar_end(x, "year"), expect)
})

test_that("can compute quarter end", {
  x <- year_quarter_day(2019, 2)
  expect_identical(calendar_end(x, "quarter"), x)

  x <- year_quarter_day(2019, 2:3, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_quarter_day(2019, 2:3, 91:92, 23, 59, 59, 999L, subsecond_precision = "millisecond")
  expect_identical(calendar_end(x, "quarter"), expect)
})

# ------------------------------------------------------------------------------
# calendar_count_between()

test_that("can compute year and month counts", {
  x <- year_quarter_day(2019, 1, 1)
  y <- year_quarter_day(2020, 3, 4)

  expect_identical(calendar_count_between(x, y, "year"), 1L)
  expect_identical(calendar_count_between(x, y, "quarter"), 6L)
  expect_identical(calendar_count_between(x, y, "quarter", n = 2), 3L)
})

test_that("works with different quarter start months", {
  x <- year_quarter_day(2019, 1, 1, start = clock_months$march)
  y <- year_quarter_day(2020, 3, 4, start = clock_months$march)

  expect_identical(calendar_count_between(x, y, "year"), 1L)
  expect_identical(calendar_count_between(x, y, "quarter"), 6L)
  expect_identical(calendar_count_between(x, y, "quarter", n = 2), 3L)
})

test_that("can't compute a unsupported count precision", {
  x <- year_quarter_day(2019, 1, 1)
  expect_snapshot((expect_error(calendar_count_between(x, x, "day"))))
})

test_that("positive / negative counts are correct", {
  start <- year_quarter_day(1972, 03, 04)


  end <- year_quarter_day(1973, 03, 03)
  expect_identical(calendar_count_between(start, end, "year"), 0L)
  expect_identical(calendar_count_between(start, end, "quarter"), 3L)

  end <- year_quarter_day(1973, 03, 04)
  expect_identical(calendar_count_between(start, end, "year"), 1L)
  expect_identical(calendar_count_between(start, end, "quarter"), 4L)

  end <- year_quarter_day(1973, 03, 05)
  expect_identical(calendar_count_between(start, end, "year"), 1L)
  expect_identical(calendar_count_between(start, end, "quarter"), 4L)


  end <- year_quarter_day(1971, 03, 03)
  expect_identical(calendar_count_between(start, end, "year"), -1L)
  expect_identical(calendar_count_between(start, end, "quarter"), -4L)

  end <- year_quarter_day(1971, 03, 04)
  expect_identical(calendar_count_between(start, end, "year"), -1L)
  expect_identical(calendar_count_between(start, end, "quarter"), -4L)

  end <- year_quarter_day(1971, 03, 05)
  expect_identical(calendar_count_between(start, end, "year"), 0L)
  expect_identical(calendar_count_between(start, end, "quarter"), -3L)
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
    as_naive_time(year_month_day(-9999, 1, 1)),
    as_naive_time(year_month_day(9999, 12, 31)),
    by = 1
  )

  for (start in seq_len(12)) {
    expect_identical(x, as_naive_time(as_year_quarter_day(x, start = start)))
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

# ------------------------------------------------------------------------------
# vec_math()

test_that("is.nan() works", {
  x <- year_quarter_day(c(2019, NA))
  expect_identical(is.nan(x), c(FALSE, FALSE))
})

test_that("is.finite() works", {
  x <- year_quarter_day(c(2019, NA))
  expect_identical(is.finite(x), c(TRUE, FALSE))
})

test_that("is.infinite() works", {
  x <- year_quarter_day(c(2019, NA))
  expect_identical(is.infinite(x), c(FALSE, FALSE))
})
