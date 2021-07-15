# ------------------------------------------------------------------------------
# year_day()

test_that("helper can create different precisions", {
  x <- year_day(2019, 1:2)
  expect_identical(get_year(x), c(2019L, 2019L))
  expect_identical(get_day(x), 1:2)

  x <- year_day(2019)
  expect_identical(get_year(x), 2019L)
})

test_that("can create subsecond precision calendars", {
  x <- year_day(2019, 1, 0, 0, 0, 1, subsecond_precision = "millisecond")
  expect_identical(get_millisecond(x), 1L)

  x <- year_day(2019, 1, 0, 0, 0, 1, subsecond_precision = "microsecond")
  expect_identical(get_microsecond(x), 1L)

  x <- year_day(2019, 1, 0, 0, 0, 1, subsecond_precision = "nanosecond")
  expect_identical(get_nanosecond(x), 1L)
})

test_that("validates value ranges", {
  expect_snapshot_error(year_day(50000))
  expect_snapshot_error(year_day(2020, 367))
  expect_snapshot_error(year_day(2020, 1, 24))
  expect_snapshot_error(year_day(2020, 1, 1, 60))
  expect_snapshot_error(year_day(2020, 1, 1, 1, 60))
  expect_snapshot_error(year_day(2020, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond"))
  expect_snapshot_error(year_day(2020, 1, 1, 1, 1, 1000000, subsecond_precision = "microsecond"))
  expect_snapshot_error(year_day(2020, 1, 1, 1, 1, 1000000000, subsecond_precision = "nanosecond"))
})

test_that("can create a date at the boundary", {
  x <- year_day(32767, 365)
  expect_identical(get_year(x), 32767L)

  x <- year_day(-32767, 1)
  expect_identical(get_year(x), -32767L)
})

test_that("can get the last day of the year", {
  x <- year_day(2019:2020, "last")
  expect_identical(get_day(x), c(365L, 366L))
})

test_that("ignores values past first `NULL`", {
  expect_identical(year_day(2019, hour = 2), year_day(2019))
})

test_that("NA values propagate", {
  x <- year_day(2019, 1:3, c(NA, 2, 3), c(3, 4, NA))
  expect_identical(is.na(x), c(TRUE, FALSE, TRUE))
})

test_that("names of `year` are not retained", {
  expect_named(year_day(c(x = 1)), NULL)
})

# ------------------------------------------------------------------------------
# vec_ptype()

test_that("ptype is correct", {
  base <- year_day(1)
  ptype <- year_day(integer())

  for (precision in precision_names()) {
    if (precision == "quarter" || precision == "month" || precision == "week") {
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
  expect_identical(vec_proxy(year_day(2019)), data_frame(year = 2019L))
  expect_identical(vec_proxy(year_day(2019, 1)), data_frame(year = 2019L, day = 1L))
})

test_that("proxy has names on `year`", {
  x <- set_names(year_day(2019, 1), "nm")
  year <- vec_proxy(x)$year
  expect_named(year, "nm")
})

test_that("restore works", {
  to <- year_day(2019, 1:5)
  proxy <- vec_slice(vec_proxy(to), 1:2)
  expect_identical(vec_restore(proxy, to), year_day(2019, 1:2))
})

# ------------------------------------------------------------------------------
# vec_ptype_full()

test_that("full ptype is correct", {
  expect_snapshot_output(vec_ptype_full(year_day(2019)))
  expect_snapshot_output(vec_ptype_full(year_day(2019, 1, 1)))
  expect_snapshot_output(vec_ptype_full(year_day(2019, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")))
  expect_snapshot_output(vec_ptype_full(year_day(2019, 366)))
})

# ------------------------------------------------------------------------------
# vec_ptype_abbr()

test_that("abbreviated ptype is correct", {
  expect_snapshot_output(vec_ptype_abbr(year_day(2019)))
  expect_snapshot_output(vec_ptype_abbr(year_day(2019, 1, 1)))
  expect_snapshot_output(vec_ptype_abbr(year_day(2019, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")))
  expect_snapshot_output(vec_ptype_abbr(year_day(2019, 366)))
})

# ------------------------------------------------------------------------------
# as_year_quarter_day()

test_that("invalid dates must be resolved when converting to another calendar", {
  expect_snapshot_error(as_year_quarter_day(year_day(2019, 366)))
})

# ------------------------------------------------------------------------------
# as_sys_time()

test_that("invalid dates must be resolved when converting to a sys-time", {
  expect_snapshot_error(as_sys_time(year_day(2019, 366)))
})

# ------------------------------------------------------------------------------
# as_naive_time()

test_that("invalid dates must be resolved when converting to a naive-time", {
  expect_snapshot_error(as_naive_time(year_day(2019, 366)))
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- year_day(2019, 1)
  y <- year_day(2019, 1, 2)

  expect_identical(as.character(x), format(x))
  expect_identical(as.character(y), format(y))
})

# ------------------------------------------------------------------------------
# calendar_group()

test_that("can group by day of the year", {
  x <- year_day(2019, 1, c(3, 4))
  expect_identical(calendar_group(x, "day"), calendar_narrow(x, "day"))
})

test_that("can group by multiple days, resetting at the new year", {
  x <- c(year_day(2019, 364:365), year_day(2020, 1:3))

  expect_identical(
    calendar_group(x, "day", n = 2),
    c(year_day(2019, c(363, 365)), year_day(2020, c(1, 1, 3)))
  )
})

# ------------------------------------------------------------------------------
# calendar_narrow()

test_that("can narrow to year", {
  x_expect <- year_day(2019)
  x <- set_day(x_expect, 1)
  expect_identical(calendar_narrow(x, "year"), x_expect)
  expect_identical(calendar_narrow(x_expect, "year"), x_expect)
})

test_that("can narrow to day", {
  x_expect <- year_day(2019, 2)
  x <- set_hour(x_expect, 5)
  expect_identical(calendar_narrow(x, "day"), x_expect)
  expect_identical(calendar_narrow(x_expect, "day"), x_expect)
})

test_that("can narrow to hour", {
  x_expect <- year_day(2019, 3, 4)
  x <- set_minute(x_expect, 5)
  expect_identical(calendar_narrow(x, "hour"), x_expect)
  expect_identical(calendar_narrow(x_expect, "hour"), x_expect)
})

test_that("can narrow to minute", {
  x_expect <- year_day(2019, 3, 4, 5)
  x <- set_second(x_expect, 6)
  expect_identical(calendar_narrow(x, "minute"), x_expect)
  expect_identical(calendar_narrow(x_expect, "minute"), x_expect)
})

test_that("can narrow to second", {
  expect <- year_day(2019, 3, 4, 5, 6)
  x <- set_millisecond(expect, 7)
  y <- set_nanosecond(expect, 7)
  expect_identical(calendar_narrow(x, "second"), expect)
  expect_identical(calendar_narrow(y, "second"), expect)
  expect_identical(calendar_narrow(expect, "second"), expect)
})

# ------------------------------------------------------------------------------
# calendar_widen()

test_that("can widen to year", {
  x <- year_day(2019)
  expect_identical(calendar_widen(x, "year"), x)
})

test_that("can widen to day", {
  x <- year_day(2019)
  expect_identical(calendar_widen(x, "day"), set_day(x, 1))
})

test_that("can widen to hour", {
  x <- year_day(2019)
  y <- year_day(2019, 02)
  expect_identical(calendar_widen(x, "hour"), set_hour(set_day(x, 1), 0))
  expect_identical(calendar_widen(y, "hour"), set_hour(y, 0))
})

test_that("can widen to minute", {
  x <- year_day(2019)
  y <- year_day(2019, 02, 02)
  x_expect <- year_day(2019, 1, 0, 0)
  y_expect <- set_minute(y, 0)
  expect_identical(calendar_widen(x, "minute"), x_expect)
  expect_identical(calendar_widen(y, "minute"), y_expect)
})

test_that("can widen to second", {
  x <- year_day(2019)
  y <- year_day(2019, 02, 02, 02)
  x_expect <- year_day(2019, 1, 0, 0, 0)
  y_expect <- set_second(y, 0)
  expect_identical(calendar_widen(x, "second"), x_expect)
  expect_identical(calendar_widen(y, "second"), y_expect)
})

test_that("can widen to subsecond precision", {
  x <- year_day(2019)
  y <- year_day(2019, 02, 02, 02, 02)
  x_expect <- year_day(2019, 1, 0, 0, 0, 0, subsecond_precision = "microsecond")
  y_expect <- set_nanosecond(y, 0)
  expect_identical(calendar_widen(x, "microsecond"), x_expect)
  expect_identical(calendar_widen(y, "nanosecond"), y_expect)
})

# ------------------------------------------------------------------------------
# calendar_start()

test_that("can compute year start", {
  x <- year_day(2019)
  expect_identical(calendar_start(x, "year"), x)

  x <- year_day(2019, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_day(2019, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  expect_identical(calendar_start(x, "year"), expect)
})

# ------------------------------------------------------------------------------
# calendar_end()

test_that("can compute year end", {
  x <- year_day(2019)
  expect_identical(calendar_end(x, "year"), x)

  x <- year_day(2019:2020, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_day(2019:2020, 365:366, 23, 59, 59, 999L, subsecond_precision = "millisecond")
  expect_identical(calendar_end(x, "year"), expect)
})

# ------------------------------------------------------------------------------
# calendar_leap_year()

test_that("can detect leap years", {
  x <- year_day(c(2019, 2020, NA), 1)
  expect_identical(calendar_leap_year(x), c(FALSE, TRUE, NA))
})

# ------------------------------------------------------------------------------
# seq()

test_that("only granular precisions are allowed", {
  expect_snapshot_error(seq(year_day(2019, 1), by = 1, length.out = 2))
})

test_that("seq(to, by) works", {
  expect_identical(seq(year_day(2019), to = year_day(2024), by = 2), year_day(c(2019, 2021, 2023)))
  expect_identical(seq(year_day(2019), to = year_day(2023), by = 2), year_day(c(2019, 2021, 2023)))
})

test_that("seq(to, length.out) works", {
  expect_identical(seq(year_day(2019), to = year_day(2024), length.out = 2), year_day(c(2019, 2024)))
  expect_identical(seq(year_day(2019), to = year_day(2024), length.out = 6), year_day(2019:2024))

  expect_identical(seq(year_day(2019), to = year_day(2024), along.with = 1:2), year_day(c(2019, 2024)))
})

test_that("seq(by, length.out) works", {
  expect_identical(seq(year_day(2019), by = 2, length.out = 3), year_day(c(2019, 2021, 2023)))

  expect_identical(seq(year_day(2019), by = 2, along.with = 1:3), year_day(c(2019, 2021, 2023)))
})

# ------------------------------------------------------------------------------
# miscellaneous

test_that("can roundtrip to naive-time", {
  x <- seq(
    as_naive_time(year_month_day(-9999, 1, 1)),
    as_naive_time(year_month_day(9999, 12, 31)),
    by = 1
  )

  expect_identical(x, as_naive_time(as_year_day(x)))
})

# ------------------------------------------------------------------------------
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot_error(invalid_resolve(year_day(2019, 1)))
})

test_that("throws known classed error", {
  expect_snapshot_error(invalid_resolve(year_day(2019, 366)))
  expect_error(invalid_resolve(year_day(2019, 366)), class = "clock_error_invalid_date")
})

# ------------------------------------------------------------------------------
# vec_math()

test_that("is.nan() works", {
  x <- year_day(c(2019, NA))
  expect_identical(is.nan(x), c(FALSE, FALSE))
})

test_that("is.finite() works", {
  x <- year_day(c(2019, NA))
  expect_identical(is.finite(x), c(TRUE, FALSE))
})

test_that("is.infinite() works", {
  x <- year_day(c(2019, NA))
  expect_identical(is.infinite(x), c(FALSE, FALSE))
})
