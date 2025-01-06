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
  expect_snapshot(error = TRUE, year_day(50000))
  expect_snapshot(error = TRUE, year_day(2020, 367))
  expect_snapshot(error = TRUE, year_day(2020, 1, 24))
  expect_snapshot(error = TRUE, year_day(2020, 1, 1, 60))
  expect_snapshot(error = TRUE, year_day(2020, 1, 1, 1, 60))
  expect_snapshot(error = TRUE, {
    year_day(2020, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond")
  })
  expect_snapshot(error = TRUE, {
    year_day(2020, 1, 1, 1, 1, 1000000, subsecond_precision = "microsecond")
  })
  expect_snapshot(error = TRUE, {
    year_day(2020, 1, 1, 1, 1, 1000000000, subsecond_precision = "nanosecond")
  })
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

test_that("`NA` propagates through 'last'", {
  x <- year_day(2019, c(1, NA))
  x <- set_day(x, "last")
  expect_identical(get_day(x), c(365L, NA))
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
  expect_snapshot_output(
    vec_ptype_full(year_day(2019, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond"))
  )
  expect_snapshot_output(vec_ptype_full(year_day(2019, 366)))
})

# ------------------------------------------------------------------------------
# vec_ptype_abbr()

test_that("abbreviated ptype is correct", {
  expect_snapshot_output(vec_ptype_abbr(year_day(2019)))
  expect_snapshot_output(vec_ptype_abbr(year_day(2019, 1, 1)))
  expect_snapshot_output(
    vec_ptype_abbr(year_day(2019, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond"))
  )
  expect_snapshot_output(vec_ptype_abbr(year_day(2019, 366)))
})

# ------------------------------------------------------------------------------
# set_*()

test_that("setters work", {
  x <- year_day(1L)

  x <- set_year(x, 2L)
  expect_identical(get_year(x), 2L)

  x <- set_day(x, 2L)
  expect_identical(get_day(x), 2L)

  x <- set_hour(x, 3L)
  expect_identical(get_hour(x), 3L)

  x <- set_minute(x, 4L)
  expect_identical(get_minute(x), 4L)

  x <- set_second(x, 5L)
  expect_identical(get_second(x), 5L)

  ms <- set_millisecond(x, 6L)
  expect_identical(get_millisecond(ms), 6L)

  us <- set_microsecond(x, 7L)
  expect_identical(get_microsecond(us), 7L)

  ns <- set_nanosecond(x, 8L)
  expect_identical(get_nanosecond(ns), 8L)
})

test_that("setters propagate all missings", {
  x <- year_day(2019, c(1, NA, 3))
  x <- set_hour(x, c(NA, 2, 4))
  expect_identical(vec_detect_missing(x), c(TRUE, TRUE, FALSE))
})

test_that("setters recycling works both ways", {
  x <- year_day(2019)

  x <- set_day(x, 1:2)
  expect_identical(x, year_day(2019, 1:2))

  x <- set_hour(x, 1)
  expect_identical(x, year_day(2019, 1:2, 1))

  expect_snapshot(error = TRUE, {
    x <- year_day(1:2)
    y <- 1:3
    set_day(x, y)
  })
})

test_that("setters require integer `value`", {
  x <- year_day(2019, 1, 2, 3, 4)

  expect_snapshot(error = TRUE, {
    set_year(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_day(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_hour(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_minute(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_second(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_millisecond(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_microsecond(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_nanosecond(x, 1.5)
  })
})

test_that("setters check `value` range", {
  x <- year_day(2019, 1, 2, 3, 4)

  expect_snapshot(error = TRUE, {
    set_year(x, 100000)
  })
  expect_snapshot(error = TRUE, {
    set_day(x, 367)
  })
  expect_snapshot(error = TRUE, {
    set_hour(x, 24)
  })
  expect_snapshot(error = TRUE, {
    set_minute(x, 60)
  })
  expect_snapshot(error = TRUE, {
    set_second(x, 60)
  })
  expect_snapshot(error = TRUE, {
    set_millisecond(x, -1)
  })
  expect_snapshot(error = TRUE, {
    set_microsecond(x, -1)
  })
  expect_snapshot(error = TRUE, {
    set_nanosecond(x, -1)
  })
})

test_that("setters require minimum precision", {
  expect_snapshot(error = TRUE, {
    set_hour(year_day(year = 1), 1)
  })
  expect_snapshot(error = TRUE, {
    set_minute(year_day(year = 1, day = 1), 1)
  })
  expect_snapshot(error = TRUE, {
    set_second(year_day(year = 1, day = 1, hour = 1), 1)
  })
  expect_snapshot(error = TRUE, {
    set_millisecond(year_day(year = 1, day = 1, hour = 1, minute = 1), 1)
  })
  expect_snapshot(error = TRUE, {
    set_microsecond(year_day(year = 1, day = 1, hour = 1, minute = 1), 1)
  })
  expect_snapshot(error = TRUE, {
    set_nanosecond(year_day(year = 1, day = 1, hour = 1, minute = 1), 1)
  })
})

test_that("setters require correct subsecond precision", {
  expect_snapshot(error = TRUE, {
    set_millisecond(
      year_day(
        year = 1,
        day = 3,
        hour = 4,
        minute = 5,
        second = 6,
        subsecond = 7,
        subsecond_precision = "microsecond"
      ),
      1
    )
  })
  expect_snapshot(error = TRUE, {
    set_millisecond(
      year_day(
        year = 1,
        day = 3,
        hour = 4,
        minute = 5,
        second = 6,
        subsecond = 7,
        subsecond_precision = "nanosecond"
      ),
      1
    )
  })

  expect_snapshot(error = TRUE, {
    set_microsecond(
      year_day(
        year = 1,
        day = 3,
        hour = 4,
        minute = 5,
        second = 6,
        subsecond = 7,
        subsecond_precision = "millisecond"
      ),
      1
    )
  })
  expect_snapshot(error = TRUE, {
    set_microsecond(
      year_day(
        year = 1,
        day = 3,
        hour = 4,
        minute = 5,
        second = 6,
        subsecond = 7,
        subsecond_precision = "nanosecond"
      ),
      1
    )
  })

  expect_snapshot(error = TRUE, {
    set_nanosecond(
      year_day(
        year = 1,
        day = 3,
        hour = 4,
        minute = 5,
        second = 6,
        subsecond = 7,
        subsecond_precision = "millisecond"
      ),
      1
    )
  })
  expect_snapshot(error = TRUE, {
    set_nanosecond(
      year_day(
        year = 1,
        day = 3,
        hour = 4,
        minute = 5,
        second = 6,
        subsecond = 7,
        subsecond_precision = "microsecond"
      ),
      1
    )
  })
})

test_that("setters retain names", {
  x <- year_day(2019)
  x <- set_names(x, "foo")
  expect_named(set_day(x, 2), "foo")
})

test_that("setting with named `value` strips its names", {
  x <- year_day(2019)
  x <- set_day(x, set_names(1L, "x"))
  expect_named(field(x, "day"), NULL)
})

# ------------------------------------------------------------------------------
# as_year_quarter_day()

test_that("invalid dates must be resolved when converting to another calendar", {
  expect_snapshot(error = TRUE, as_year_quarter_day(year_day(2019, 366)))
})

# ------------------------------------------------------------------------------
# as_sys_time()

test_that("invalid dates must be resolved when converting to a sys-time", {
  expect_snapshot(error = TRUE, as_sys_time(year_day(2019, 366)))
})

# ------------------------------------------------------------------------------
# as_naive_time()

test_that("invalid dates must be resolved when converting to a naive-time", {
  expect_snapshot(error = TRUE, as_naive_time(year_day(2019, 366)))
})

# ------------------------------------------------------------------------------
# format()

test_that("default formats are correct", {
  expect_snapshot(format(year_day(2019)))
  expect_snapshot(format(year_day(2019, 1)))
  expect_snapshot(format(year_day(2019, 1, 1)))
  expect_snapshot(
    format(year_day(2019, 1, 1, 2, 3, 50, subsecond_precision = "microsecond"))
  )
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
  expect <- year_day(
    2019:2020,
    365:366,
    23,
    59,
    59,
    999L,
    subsecond_precision = "millisecond"
  )
  expect_identical(calendar_end(x, "year"), expect)
})

# ------------------------------------------------------------------------------
# calendar_leap_year()

test_that("can detect leap years", {
  x <- year_day(c(2019, 2020, NA), 1)
  expect_identical(calendar_leap_year(x), c(FALSE, TRUE, NA))
})

# ------------------------------------------------------------------------------
# calendar_count_between()

test_that("can compute year counts", {
  x <- year_day(2019, 1)
  y <- year_day(2020, 3)

  expect_identical(calendar_count_between(x, y, "year"), 1L)
})

test_that("can't compute a unsupported difference precision", {
  x <- year_day(2019, 1)
  expect_snapshot((expect_error(calendar_count_between(x, x, "day"))))
})

test_that("positive / negative differences are correct", {
  start <- year_day(1972, 04)

  end <- year_day(1973, 03)
  expect_identical(calendar_count_between(start, end, "year"), 0L)

  end <- year_day(1973, 04)
  expect_identical(calendar_count_between(start, end, "year"), 1L)

  end <- year_day(1973, 05)
  expect_identical(calendar_count_between(start, end, "year"), 1L)

  end <- year_day(1971, 03)
  expect_identical(calendar_count_between(start, end, "year"), -1L)

  end <- year_day(1971, 04)
  expect_identical(calendar_count_between(start, end, "year"), -1L)

  end <- year_day(1971, 05)
  expect_identical(calendar_count_between(start, end, "year"), 0L)
})

# ------------------------------------------------------------------------------
# seq()

test_that("only granular precisions are allowed", {
  expect_snapshot(error = TRUE, seq(year_day(2019, 1), by = 1, length.out = 2))
})

test_that("seq(to, by) works", {
  expect_identical(
    seq(year_day(2019), to = year_day(2024), by = 2),
    year_day(c(2019, 2021, 2023))
  )
  expect_identical(
    seq(year_day(2019), to = year_day(2023), by = 2),
    year_day(c(2019, 2021, 2023))
  )
})

test_that("seq(to, length.out) works", {
  expect_identical(
    seq(year_day(2019), to = year_day(2024), length.out = 2),
    year_day(c(2019, 2024))
  )
  expect_identical(
    seq(year_day(2019), to = year_day(2024), length.out = 6),
    year_day(2019:2024)
  )

  expect_identical(
    seq(year_day(2019), to = year_day(2024), along.with = 1:2),
    year_day(c(2019, 2024))
  )
})

test_that("seq(by, length.out) works", {
  expect_identical(
    seq(year_day(2019), by = 2, length.out = 3),
    year_day(c(2019, 2021, 2023))
  )

  expect_identical(
    seq(year_day(2019), by = 2, along.with = 1:3),
    year_day(c(2019, 2021, 2023))
  )
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
# invalid_detect()

test_that("`invalid_detect()` works", {
  # Not possible to be invalid
  x <- year_day(2019:2020)
  expect_identical(invalid_detect(x), c(FALSE, FALSE))

  # Now possible
  x <- year_day(2019, c(1, 365, 366, NA))
  expect_identical(invalid_detect(x), c(FALSE, FALSE, TRUE, FALSE))

  # Possible after that too
  x <- year_day(2019, c(1, 365, 366, NA), 3)
  expect_identical(invalid_detect(x), c(FALSE, FALSE, TRUE, FALSE))
})

# ------------------------------------------------------------------------------
# invalid_any()

test_that("`invalid_any()` works", {
  # Not possible to be invalid
  x <- year_day(2019:2020)
  expect_false(invalid_any(x))

  # Now possible
  x <- year_day(2019, c(1, 365, 366, NA))
  expect_true(invalid_any(x))

  # Possible after that too
  x <- year_day(2019, c(1, 365, 366, NA), 3)
  expect_true(invalid_any(x))
})

# ------------------------------------------------------------------------------
# invalid_count()

test_that("`invalid_count()` works", {
  # Not possible to be invalid
  x <- year_day(2019:2020)
  expect_identical(invalid_count(x), 0L)

  # Now possible
  x <- year_day(2019, c(1, 365, 366, NA))
  expect_identical(invalid_count(x), 1L)

  # Possible after that too
  x <- year_day(2019, c(1, 365, 366, NA), 3)
  expect_identical(invalid_count(x), 1L)
})

# ------------------------------------------------------------------------------
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot(error = TRUE, invalid_resolve(year_day(2019, 1)))
})

test_that("can resolve correctly", {
  x <- year_day(2019, 366, 2, 3, 4, 5, subsecond_precision = "millisecond")

  expect_identical(
    invalid_resolve(x, invalid = "previous"),
    year_day(2019, 365, 23, 59, 59, 999, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "previous-day"),
    year_day(2019, 365, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "next"),
    year_day(2020, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "next-day"),
    year_day(2020, 1, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "overflow"),
    year_day(2020, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "overflow-day"),
    year_day(2020, 1, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "NA"),
    year_day(NA, NA, NA, NA, NA, NA, subsecond_precision = "millisecond")
  )
})

test_that("throws known classed error", {
  expect_snapshot(error = TRUE, invalid_resolve(year_day(2019, 366)))
  expect_error(invalid_resolve(year_day(2019, 366)), class = "clock_error_invalid_date")
})

test_that("works with always valid precisions", {
  x <- year_day(2019:2020)
  expect_identical(invalid_resolve(x), x)
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

# ------------------------------------------------------------------------------
# diff()

test_that("works with `lag` and `differences`", {
  x <- year_day(2019:2026)
  expect_identical(diff(x), rep(duration_years(1), 7))
  expect_identical(diff(x, lag = 2), rep(duration_years(2), 6))
  expect_identical(diff(x, differences = 2), rep(duration_years(0), 6))
  expect_identical(
    diff(x, lag = 3, differences = 2),
    rep(duration_years(0), 2)
  )
})

test_that("works with `lag` and `differences` that force an empty result (#364)", {
  expect_identical(diff(year_day(integer())), duration_years())
  expect_identical(diff(year_day(1)), duration_years())
  expect_identical(
    diff(year_day(1:8), lag = 4, differences = 3),
    duration_years()
  )
})

test_that("errors on invalid precisions", {
  expect_snapshot(error = TRUE, {
    diff(year_day(2019, 1))
  })
})

test_that("errors on invalid lag/differences", {
  # These are base R errors we don't control
  expect_error(diff(year_day(2019), lag = 1:2))
  expect_error(diff(year_day(2019), differences = 1:2))
})

# ------------------------------------------------------------------------------
# clock_minimum() / clock_maximum()

test_that("minimums are right", {
  expect_snapshot({
    clock_minimum(clock_empty_year_day_year)
    clock_minimum(clock_empty_year_day_day)
    clock_minimum(clock_empty_year_day_hour)
    clock_minimum(clock_empty_year_day_minute)
    clock_minimum(clock_empty_year_day_second)
    clock_minimum(clock_empty_year_day_millisecond)
    clock_minimum(clock_empty_year_day_microsecond)
    clock_minimum(clock_empty_year_day_nanosecond)
  })
})

test_that("maximums are right", {
  expect_snapshot({
    clock_maximum(clock_empty_year_day_year)
    clock_maximum(clock_empty_year_day_day)
    clock_maximum(clock_empty_year_day_hour)
    clock_maximum(clock_empty_year_day_minute)
    clock_maximum(clock_empty_year_day_second)
    clock_maximum(clock_empty_year_day_millisecond)
    clock_maximum(clock_empty_year_day_microsecond)
    clock_maximum(clock_empty_year_day_nanosecond)
  })
})

# ------------------------------------------------------------------------------
# min() / max() / range()

test_that("min() / max() / range() works", {
  x <- year_day(c(1, 3, 2, 1, -1))

  expect_identical(min(x), year_day(-1))
  expect_identical(max(x), year_day(3))
  expect_identical(range(x), year_day(c(-1, 3)))
})

test_that("min() / max() / range() works with `NA`", {
  x <- year_day(c(1, NA, 2, 0))

  expect_identical(min(x), year_day(NA))
  expect_identical(max(x), year_day(NA))
  expect_identical(range(x), year_day(c(NA, NA)))

  expect_identical(min(x, na.rm = TRUE), year_day(0))
  expect_identical(max(x, na.rm = TRUE), year_day(2))
  expect_identical(range(x, na.rm = TRUE), year_day(c(0, 2)))
})

test_that("min() / max() / range() works when empty", {
  x <- year_day(integer())

  expect_identical(min(x), clock_maximum(x))
  expect_identical(max(x), clock_minimum(x))
  expect_identical(range(x), c(clock_maximum(x), clock_minimum(x)))

  x <- year_day(c(NA, NA))

  expect_identical(min(x, na.rm = TRUE), clock_maximum(x))
  expect_identical(max(x, na.rm = TRUE), clock_minimum(x))
  expect_identical(range(x, na.rm = TRUE), c(clock_maximum(x), clock_minimum(x)))
})

# ------------------------------------------------------------------------------
# add_*()

test_that("add_years() works", {
  x <- year_day(2019, 1, 2, 3:4)

  expect_identical(
    add_years(x, 1:2),
    year_day(c(2020, 2021), 1, 2, 3:4)
  )
  expect_identical(
    add_years(x, NA),
    vec_init(x, 2L)
  )
})

test_that("add_*() respect recycling rules", {
  expect_length(add_years(year_day(1), 1:2), 2L)
  expect_length(add_years(year_day(1:2), 1), 2L)

  expect_length(add_years(year_day(1), integer()), 0L)
  expect_length(add_years(year_day(integer()), 1), 0L)

  expect_snapshot(error = TRUE, {
    add_years(year_day(1:2), 1:3)
  })
})

test_that("add_*() retains names", {
  x <- set_names(year_day(1), "x")
  y <- year_day(1)

  n <- set_names(1, "n")

  expect_named(add_years(x, n), "x")
  expect_named(add_years(y, n), "n")
})
