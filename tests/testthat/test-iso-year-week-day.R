# ------------------------------------------------------------------------------
# iso_year_week_day()

test_that("helper can create different precisions", {
  x <- iso_year_week_day(2019, 1:2)
  expect_identical(get_year(x), c(2019L, 2019L))
  expect_identical(get_week(x), 1:2)

  x <- iso_year_week_day(2019, 1:2, 3)
  expect_identical(get_day(x), c(3L, 3L))
})

test_that("can create subsecond precision calendars", {
  x <- iso_year_week_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "millisecond")
  expect_identical(get_millisecond(x), 1L)

  x <- iso_year_week_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "microsecond")
  expect_identical(get_microsecond(x), 1L)

  x <- iso_year_week_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "nanosecond")
  expect_identical(get_nanosecond(x), 1L)
})

test_that("validates value ranges", {
  expect_snapshot(error = TRUE, iso_year_week_day(50000))
  expect_snapshot(error = TRUE, iso_year_week_day(2020, 54))
  expect_snapshot(error = TRUE, iso_year_week_day(2020, 1, 8))
  expect_snapshot(error = TRUE, iso_year_week_day(2020, 1, 1, 24))
  expect_snapshot(error = TRUE, iso_year_week_day(2020, 1, 1, 1, 60))
  expect_snapshot(error = TRUE, iso_year_week_day(2020, 1, 1, 1, 1, 60))
  expect_snapshot(
    error = TRUE,
    iso_year_week_day(2020, 1, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond")
  )
  expect_snapshot(
    error = TRUE,
    iso_year_week_day(2020, 1, 1, 1, 1, 1, 1000000, subsecond_precision = "microsecond")
  )
  expect_snapshot(
    error = TRUE,
    iso_year_week_day(
      2020,
      1,
      1,
      1,
      1,
      1,
      1000000000,
      subsecond_precision = "nanosecond"
    )
  )
})

test_that("can get the last week of the iso year", {
  x <- iso_year_week_day(2019:2020, "last")
  expect_identical(get_week(x), c(52L, 53L))
})

test_that("`NA` propagates through 'last'", {
  x <- iso_year_week_day(c(2019, NA))
  x <- set_week(x, "last")
  expect_identical(get_week(x), c(52L, NA))
})

test_that("ignores values past first `NULL`", {
  expect_identical(iso_year_week_day(2019, day = 2), iso_year_week_day(2019))
})

test_that("NA values propagate", {
  x <- iso_year_week_day(2019, 1:3, c(NA, 2, 3), c(3, 4, NA))
  expect_identical(is.na(x), c(TRUE, FALSE, TRUE))
})

# ------------------------------------------------------------------------------
# vec_ptype()

test_that("ptype is correct", {
  base <- iso_year_week_day(1)
  ptype <- iso_year_week_day(integer())

  for (precision in precision_names()) {
    if (precision == "quarter" || precision == "month") {
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
  expect_identical(vec_proxy(iso_year_week_day(2019)), data_frame(year = 2019L))
  expect_identical(
    vec_proxy(iso_year_week_day(2019, 1)),
    data_frame(year = 2019L, week = 1L)
  )
})

test_that("proxy has names on `year`", {
  x <- set_names(iso_year_week_day(2019, 1), "nm")
  year <- vec_proxy(x)$year
  expect_named(year, "nm")
})

test_that("restore works", {
  to <- iso_year_week_day(2019, 1:5)
  proxy <- vec_slice(vec_proxy(to), 1:2)
  expect_identical(vec_restore(proxy, to), iso_year_week_day(2019, 1:2))
})

# ------------------------------------------------------------------------------
# vec_ptype_full()

test_that("full ptype is correct", {
  expect_snapshot_output(vec_ptype_full(iso_year_week_day(2019)))
  expect_snapshot_output(vec_ptype_full(iso_year_week_day(2019, 1, 1)))
  expect_snapshot_output(
    vec_ptype_full(
      iso_year_week_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")
    )
  )
  expect_snapshot_output(vec_ptype_full(iso_year_week_day(2019, 53)))
})

# ------------------------------------------------------------------------------
# vec_ptype_abbr()

test_that("abbreviated ptype is correct", {
  expect_snapshot_output(vec_ptype_abbr(iso_year_week_day(2019)))
  expect_snapshot_output(vec_ptype_abbr(iso_year_week_day(2019, 1, 1)))
  expect_snapshot_output(
    vec_ptype_abbr(
      iso_year_week_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")
    )
  )
  expect_snapshot_output(vec_ptype_abbr(iso_year_week_day(2019, 53)))
})

# ------------------------------------------------------------------------------
# set_*()

test_that("setters work", {
  x <- iso_year_week_day(1L)

  x <- set_year(x, 2L)
  expect_identical(get_year(x), 2L)

  x <- set_week(x, 1L)
  expect_identical(get_week(x), 1L)

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
  x <- iso_year_week_day(2019, c(1, NA, 3))
  x <- set_day(x, c(NA, 2, 4))
  expect_identical(vec_detect_missing(x), c(TRUE, TRUE, FALSE))
})

test_that("setters recycling works both ways", {
  x <- iso_year_week_day(2019)

  x <- set_week(x, 1:2)
  expect_identical(x, iso_year_week_day(2019, 1:2))

  x <- set_day(x, 1)
  expect_identical(x, iso_year_week_day(2019, 1:2, 1))

  expect_snapshot(error = TRUE, {
    x <- iso_year_week_day(1:2)
    y <- 1:3
    set_week(x, y)
  })
})

test_that("setters require integer `value`", {
  x <- iso_year_week_day(2019, 1, 2, 3, 4, 5)

  expect_snapshot(error = TRUE, {
    set_year(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_week(x, 1.5)
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
  x <- iso_year_week_day(2019, 1, 2, 3, 4, 5)

  expect_snapshot(error = TRUE, {
    set_year(x, 100000)
  })
  expect_snapshot(error = TRUE, {
    set_week(x, 54)
  })
  expect_snapshot(error = TRUE, {
    set_day(x, 8)
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
    set_day(iso_year_week_day(year = 1), 1)
  })
  expect_snapshot(error = TRUE, {
    set_hour(iso_year_week_day(year = 1, week = 2), 1)
  })
  expect_snapshot(error = TRUE, {
    set_minute(iso_year_week_day(year = 1, week = 2, day = 3), 1)
  })
  expect_snapshot(error = TRUE, {
    set_second(iso_year_week_day(year = 1, week = 2, day = 3, hour = 4), 1)
  })
  expect_snapshot(error = TRUE, {
    set_millisecond(
      iso_year_week_day(year = 1, week = 2, day = 3, hour = 4, minute = 5),
      1
    )
  })
  expect_snapshot(error = TRUE, {
    set_microsecond(
      iso_year_week_day(year = 1, week = 2, day = 3, hour = 4, minute = 5),
      1
    )
  })
  expect_snapshot(error = TRUE, {
    set_nanosecond(
      iso_year_week_day(year = 1, week = 2, day = 3, hour = 4, minute = 5),
      1
    )
  })
})

test_that("setters require correct subsecond precision", {
  expect_snapshot(error = TRUE, {
    set_millisecond(
      iso_year_week_day(
        year = 1,
        week = 2,
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
      iso_year_week_day(
        year = 1,
        week = 2,
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
      iso_year_week_day(
        year = 1,
        week = 2,
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
      iso_year_week_day(
        year = 1,
        week = 2,
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
      iso_year_week_day(
        year = 1,
        week = 2,
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
      iso_year_week_day(
        year = 1,
        week = 2,
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
  x <- iso_year_week_day(2019)
  x <- set_names(x, "foo")
  expect_named(set_week(x, 2), "foo")
})

test_that("setting with named `value` strips its names", {
  x <- iso_year_week_day(2019)
  x <- set_week(x, set_names(1L, "x"))
  expect_named(field(x, "week"), NULL)
})

# ------------------------------------------------------------------------------
# format()

test_that("default formats are correct", {
  expect_snapshot(format(iso_year_week_day(2019)))
  expect_snapshot(format(iso_year_week_day(2019, 1)))
  expect_snapshot(format(iso_year_week_day(2019, 1, 1, 1)))
  expect_snapshot(
    format(
      iso_year_week_day(2019, 1, 1, 1, 2, 3, 50, subsecond_precision = "microsecond")
    )
  )
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- iso_year_week_day(2019, 1)
  y <- iso_year_week_day(2019, 1, 2)

  expect_identical(as.character(x), format(x))
  expect_identical(as.character(y), format(y))
})

# ------------------------------------------------------------------------------
# calendar_narrow()

test_that("can narrow to week", {
  x_expect <- iso_year_week_day(2019, 2)
  x <- set_day(x_expect, 1)
  expect_identical(calendar_narrow(x, "week"), x_expect)
  expect_identical(calendar_narrow(x_expect, "week"), x_expect)
})

test_that("can narrow to day", {
  x_expect <- iso_year_week_day(2019, 2, 3)
  x <- set_hour(x_expect, 5)
  expect_identical(calendar_narrow(x, "day"), x_expect)
  expect_identical(calendar_narrow(x_expect, "day"), x_expect)
})

# ------------------------------------------------------------------------------
# calendar_widen()

test_that("can widen to week", {
  x <- iso_year_week_day(2019)
  expect_identical(calendar_widen(x, "week"), set_week(x, 1))
})

test_that("can widen to day", {
  x <- iso_year_week_day(2019)
  y <- iso_year_week_day(2019, 02)
  expect_identical(calendar_widen(x, "day"), set_day(set_week(x, 1), 1))
  expect_identical(calendar_widen(y, "day"), set_day(y, 1))
})

# ------------------------------------------------------------------------------
# calendar_start()

test_that("can compute year start", {
  x <- iso_year_week_day(2019)
  expect_identical(calendar_start(x, "year"), x)

  x <- iso_year_week_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- iso_year_week_day(
    2019,
    1,
    1,
    0,
    0,
    0,
    0,
    subsecond_precision = "millisecond"
  )
  expect_identical(calendar_start(x, "year"), expect)
})

test_that("can compute week start", {
  x <- iso_year_week_day(2019, 2)
  expect_identical(calendar_start(x, "week"), x)

  x <- iso_year_week_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- iso_year_week_day(
    2019,
    2,
    1,
    0,
    0,
    0,
    0,
    subsecond_precision = "millisecond"
  )
  expect_identical(calendar_start(x, "week"), expect)
})

# ------------------------------------------------------------------------------
# calendar_end()

test_that("can compute year end", {
  x <- iso_year_week_day(2019)
  expect_identical(calendar_end(x, "year"), x)

  x <- iso_year_week_day(
    2019:2020,
    2,
    2,
    2,
    2,
    2,
    2,
    subsecond_precision = "millisecond"
  )
  expect <- iso_year_week_day(
    2019:2020,
    52:53,
    7,
    23,
    59,
    59,
    999L,
    subsecond_precision = "millisecond"
  )
  expect_identical(calendar_end(x, "year"), expect)
})

test_that("can compute week end", {
  x <- iso_year_week_day(2019, 2)
  expect_identical(calendar_end(x, "week"), x)

  x <- iso_year_week_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- iso_year_week_day(
    2019,
    2,
    7,
    23,
    59,
    59,
    999L,
    subsecond_precision = "millisecond"
  )
  expect_identical(calendar_end(x, "week"), expect)
})

# ------------------------------------------------------------------------------
# calendar_leap_year()

test_that("can detect leap years", {
  # Exactly 71 leap weeks for any 400 year cycle
  start <- 1900L

  while (start < 2000L) {
    # `- 1L` to have exactly 400 years considered in the range since both
    # `start` and `end` are included by `seq()`
    end <- start + 400L - 1L
    x <- iso_year_week_day(seq(start, end))
    expect_identical(sum(calendar_leap_year(x)), 71L)
    start <- start + 1L
  }
})

test_that("`NA` propagates", {
  expect_identical(calendar_leap_year(iso_year_week_day(NA)), NA)
})

# ------------------------------------------------------------------------------
# calendar_count_between()

test_that("can compute year and month counts", {
  x <- iso_year_week_day(2019, 1, 1)
  y <- iso_year_week_day(2020, 3, 4)

  expect_identical(calendar_count_between(x, y, "year"), 1L)
})

test_that("can't compute a unsupported count precision", {
  x <- iso_year_week_day(2019, 1, 1)
  expect_snapshot((expect_error(calendar_count_between(x, x, "week"))))
})

test_that("positive / negative counts are correct", {
  start <- iso_year_week_day(1972, 03, 04)

  end <- iso_year_week_day(1973, 03, 03)
  expect_identical(calendar_count_between(start, end, "year"), 0L)

  end <- iso_year_week_day(1973, 03, 04)
  expect_identical(calendar_count_between(start, end, "year"), 1L)

  end <- iso_year_week_day(1973, 03, 05)
  expect_identical(calendar_count_between(start, end, "year"), 1L)

  end <- iso_year_week_day(1971, 03, 03)
  expect_identical(calendar_count_between(start, end, "year"), -1L)

  end <- iso_year_week_day(1971, 03, 04)
  expect_identical(calendar_count_between(start, end, "year"), -1L)

  end <- iso_year_week_day(1971, 03, 05)
  expect_identical(calendar_count_between(start, end, "year"), 0L)
})

# ------------------------------------------------------------------------------
# seq()

test_that("only year precision is allowed", {
  expect_snapshot(error = TRUE, seq(iso_year_week_day(2019, 1), by = 1, length.out = 2))
})

test_that("seq(to, by) works", {
  expect_identical(
    seq(iso_year_week_day(2019), to = iso_year_week_day(2024), by = 2),
    iso_year_week_day(c(2019, 2021, 2023))
  )
  expect_identical(
    seq(iso_year_week_day(2019), to = iso_year_week_day(2023), by = 2),
    iso_year_week_day(c(2019, 2021, 2023))
  )
})

test_that("seq(to, length.out) works", {
  expect_identical(
    seq(iso_year_week_day(2019), to = iso_year_week_day(2024), length.out = 2),
    iso_year_week_day(c(2019, 2024))
  )
  expect_identical(
    seq(iso_year_week_day(2019), to = iso_year_week_day(2024), length.out = 6),
    iso_year_week_day(2019:2024)
  )

  expect_identical(
    seq(iso_year_week_day(2019), to = iso_year_week_day(2024), along.with = 1:2),
    iso_year_week_day(c(2019, 2024))
  )
})

test_that("seq(by, length.out) works", {
  expect_identical(
    seq(iso_year_week_day(2019), by = 2, length.out = 3),
    iso_year_week_day(c(2019, 2021, 2023))
  )

  expect_identical(
    seq(iso_year_week_day(2019), by = 2, along.with = 1:3),
    iso_year_week_day(c(2019, 2021, 2023))
  )
})

# ------------------------------------------------------------------------------
# invalid_detect()

test_that("`invalid_detect()` works", {
  # Not possible to be invalid
  x <- iso_year_week_day(2019:2020)
  expect_identical(invalid_detect(x), c(FALSE, FALSE))

  # Now possible
  x <- iso_year_week_day(2019, c(1, 52, 53, NA))
  expect_identical(invalid_detect(x), c(FALSE, FALSE, TRUE, FALSE))

  # Possible after that too
  x <- iso_year_week_day(2019, c(1, 52, 53, NA), 1)
  expect_identical(invalid_detect(x), c(FALSE, FALSE, TRUE, FALSE))
})

# ------------------------------------------------------------------------------
# invalid_any()

test_that("`invalid_any()` works", {
  # Not possible to be invalid
  x <- iso_year_week_day(2019:2020)
  expect_false(invalid_any(x))

  # Now possible
  x <- iso_year_week_day(2019, c(1, 52, 53, NA))
  expect_true(invalid_any(x))

  # Possible after that too
  x <- iso_year_week_day(2019, c(1, 52, 53, NA), 1)
  expect_true(invalid_any(x))
})

# ------------------------------------------------------------------------------
# invalid_count()

test_that("`invalid_count()` works", {
  # Not possible to be invalid
  x <- iso_year_week_day(2019:2020)
  expect_identical(invalid_count(x), 0L)

  # Now possible
  x <- iso_year_week_day(2019, c(1, 52, 53, NA))
  expect_identical(invalid_count(x), 1L)

  # Possible after that too
  x <- iso_year_week_day(2019, c(1, 52, 53, NA), 1)
  expect_identical(invalid_count(x), 1L)
})

# ------------------------------------------------------------------------------
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot(error = TRUE, invalid_resolve(iso_year_week_day(2019, 1)))
})

test_that("can resolve correctly", {
  x <- iso_year_week_day(2019, 53, 2, 2, 3, 4, 5, subsecond_precision = "millisecond")

  expect_identical(
    invalid_resolve(x, invalid = "previous"),
    iso_year_week_day(2019, 52, 7, 23, 59, 59, 999, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "previous-day"),
    iso_year_week_day(2019, 52, 7, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "next"),
    iso_year_week_day(2020, 01, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "next-day"),
    iso_year_week_day(2020, 01, 1, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "overflow"),
    iso_year_week_day(2020, 01, 02, 0, 0, 0, 0, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "overflow-day"),
    iso_year_week_day(2020, 01, 02, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "NA"),
    iso_year_week_day(NA, NA, NA, NA, NA, NA, NA, subsecond_precision = "millisecond")
  )
})

test_that("throws known classed error", {
  expect_snapshot(error = TRUE, invalid_resolve(iso_year_week_day(2019, 53)))
  expect_error(
    invalid_resolve(iso_year_week_day(2019, 53)),
    class = "clock_error_invalid_date"
  )
})

test_that("works with always valid precisions", {
  x <- iso_year_week_day(2019:2020)
  expect_identical(invalid_resolve(x), x)
})

# ------------------------------------------------------------------------------
# vec_math()

test_that("is.nan() works", {
  x <- iso_year_week_day(c(2019, NA))
  expect_identical(is.nan(x), c(FALSE, FALSE))
})

test_that("is.finite() works", {
  x <- iso_year_week_day(c(2019, NA))
  expect_identical(is.finite(x), c(TRUE, FALSE))
})

test_that("is.infinite() works", {
  x <- iso_year_week_day(c(2019, NA))
  expect_identical(is.infinite(x), c(FALSE, FALSE))
})

# ------------------------------------------------------------------------------
# diff()

test_that("works with `lag` and `differences`", {
  x <- iso_year_week_day(2019:2026)
  expect_identical(diff(x), rep(duration_years(1), 7))
  expect_identical(diff(x, lag = 2), rep(duration_years(2), 6))
  expect_identical(diff(x, differences = 2), rep(duration_years(0), 6))
  expect_identical(
    diff(x, lag = 3, differences = 2),
    rep(duration_years(0), 2)
  )
})

test_that("works with `lag` and `differences` that force an empty result (#364)", {
  expect_identical(diff(iso_year_week_day(integer())), duration_years())
  expect_identical(diff(iso_year_week_day(1)), duration_years())
  expect_identical(
    diff(iso_year_week_day(1:8), lag = 4, differences = 3),
    duration_years()
  )
})

test_that("errors on invalid precisions", {
  expect_snapshot(error = TRUE, {
    diff(iso_year_week_day(2019, 1))
  })
})

test_that("errors on invalid lag/differences", {
  # These are base R errors we don't control
  expect_error(diff(iso_year_week_day(2019), lag = 1:2))
  expect_error(diff(iso_year_week_day(2019), differences = 1:2))
})

# ------------------------------------------------------------------------------
# clock_minimum() / clock_maximum()

test_that("minimums are right", {
  expect_snapshot({
    clock_minimum(clock_empty_iso_year_week_day_year)
    clock_minimum(clock_empty_iso_year_week_day_week)
    clock_minimum(clock_empty_iso_year_week_day_day)
    clock_minimum(clock_empty_iso_year_week_day_hour)
    clock_minimum(clock_empty_iso_year_week_day_minute)
    clock_minimum(clock_empty_iso_year_week_day_second)
    clock_minimum(clock_empty_iso_year_week_day_millisecond)
    clock_minimum(clock_empty_iso_year_week_day_microsecond)
    clock_minimum(clock_empty_iso_year_week_day_nanosecond)
  })
})

test_that("maximums are right", {
  expect_snapshot({
    clock_maximum(clock_empty_iso_year_week_day_year)
    clock_maximum(clock_empty_iso_year_week_day_week)
    clock_maximum(clock_empty_iso_year_week_day_day)
    clock_maximum(clock_empty_iso_year_week_day_hour)
    clock_maximum(clock_empty_iso_year_week_day_minute)
    clock_maximum(clock_empty_iso_year_week_day_second)
    clock_maximum(clock_empty_iso_year_week_day_millisecond)
    clock_maximum(clock_empty_iso_year_week_day_microsecond)
    clock_maximum(clock_empty_iso_year_week_day_nanosecond)
  })
})

# ------------------------------------------------------------------------------
# min() / max() / range()

test_that("min() / max() / range() works", {
  x <- iso_year_week_day(c(1, 3, 2, 1, -1))

  expect_identical(min(x), iso_year_week_day(-1))
  expect_identical(max(x), iso_year_week_day(3))
  expect_identical(range(x), iso_year_week_day(c(-1, 3)))
})

test_that("min() / max() / range() works with `NA`", {
  x <- iso_year_week_day(c(1, NA, 2, 0))

  expect_identical(min(x), iso_year_week_day(NA))
  expect_identical(max(x), iso_year_week_day(NA))
  expect_identical(range(x), iso_year_week_day(c(NA, NA)))

  expect_identical(min(x, na.rm = TRUE), iso_year_week_day(0))
  expect_identical(max(x, na.rm = TRUE), iso_year_week_day(2))
  expect_identical(range(x, na.rm = TRUE), iso_year_week_day(c(0, 2)))
})

test_that("min() / max() / range() works when empty", {
  x <- iso_year_week_day(integer())

  expect_identical(min(x), clock_maximum(x))
  expect_identical(max(x), clock_minimum(x))
  expect_identical(range(x), c(clock_maximum(x), clock_minimum(x)))

  x <- iso_year_week_day(c(NA, NA))

  expect_identical(min(x, na.rm = TRUE), clock_maximum(x))
  expect_identical(max(x, na.rm = TRUE), clock_minimum(x))
  expect_identical(range(x, na.rm = TRUE), c(clock_maximum(x), clock_minimum(x)))
})

# ------------------------------------------------------------------------------
# add_*()

test_that("add_years() works", {
  x <- iso_year_week_day(2019, 1, 2, 3:4)

  expect_identical(
    add_years(x, 1:2),
    iso_year_week_day(c(2020, 2021), 1, 2, 3:4)
  )
  expect_identical(
    add_years(x, NA),
    vec_init(x, 2L)
  )
})

test_that("add_*() respect recycling rules", {
  expect_length(add_years(iso_year_week_day(1), 1:2), 2L)
  expect_length(add_years(iso_year_week_day(1:2), 1), 2L)

  expect_length(add_years(iso_year_week_day(1), integer()), 0L)
  expect_length(add_years(iso_year_week_day(integer()), 1), 0L)

  expect_snapshot(error = TRUE, {
    add_years(iso_year_week_day(1:2), 1:3)
  })
})

test_that("add_*() retains names", {
  x <- set_names(iso_year_week_day(1), "x")
  y <- iso_year_week_day(1)

  n <- set_names(1, "n")

  expect_named(add_years(x, n), "x")
  expect_named(add_years(y, n), "n")
})
