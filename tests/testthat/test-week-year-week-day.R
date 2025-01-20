# ------------------------------------------------------------------------------
# year_week_day()

test_that("helper can create different precisions", {
  x <- year_week_day(2019, 1:2)
  expect_identical(get_year(x), c(2019L, 2019L))
  expect_identical(get_week(x), 1:2)

  x <- year_week_day(2019, 1:2, 3)
  expect_identical(get_day(x), c(3L, 3L))
})

test_that("can create subsecond precision calendars", {
  x <- year_week_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "millisecond")
  expect_identical(get_millisecond(x), 1L)

  x <- year_week_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "microsecond")
  expect_identical(get_microsecond(x), 1L)

  x <- year_week_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "nanosecond")
  expect_identical(get_nanosecond(x), 1L)
})

test_that("validates value ranges", {
  expect_snapshot(error = TRUE, year_week_day(50000))
  expect_snapshot(error = TRUE, year_week_day(2020, 54))
  expect_snapshot(error = TRUE, year_week_day(2020, 1, 8))
  expect_snapshot(error = TRUE, year_week_day(2020, 1, 1, 24))
  expect_snapshot(error = TRUE, year_week_day(2020, 1, 1, 1, 60))
  expect_snapshot(error = TRUE, year_week_day(2020, 1, 1, 1, 1, 60))
  expect_snapshot(
    error = TRUE,
    year_week_day(2020, 1, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond")
  )
  expect_snapshot(
    error = TRUE,
    year_week_day(2020, 1, 1, 1, 1, 1, 1000000, subsecond_precision = "microsecond")
  )
  expect_snapshot(
    error = TRUE,
    year_week_day(2020, 1, 1, 1, 1, 1, 1000000000, subsecond_precision = "nanosecond")
  )
})

test_that("can get the last week of the year", {
  x <- year_week_day(2024:2026, "last")
  expect_identical(get_week(x), c(52L, 53L, 52L))

  x <- year_week_day(2024:2026, "last", start = clock_weekdays$monday)
  expect_identical(get_week(x), c(52L, 52L, 53L))
})

test_that("`NA` propagates through 'last'", {
  x <- year_week_day(c(2019, NA))
  x <- set_week(x, "last")
  expect_identical(get_week(x), c(52L, NA))
})

test_that("ignores values past first `NULL`", {
  expect_identical(year_week_day(2019, day = 2), year_week_day(2019))
})

test_that("NA values propagate", {
  x <- year_week_day(2019, 1:3, c(NA, 2, 3), c(3, 4, NA))
  expect_identical(is.na(x), c(TRUE, FALSE, TRUE))
})

# ------------------------------------------------------------------------------
# vec_ptype()

test_that("ptype is correct", {
  base <- year_week_day(1)
  ptype <- year_week_day(integer())

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
  expect_identical(vec_proxy(year_week_day(2019)), data_frame(year = 2019L))
  expect_identical(
    vec_proxy(year_week_day(2019, 1)),
    data_frame(year = 2019L, week = 1L)
  )
})

test_that("proxy has names on `year`", {
  x <- set_names(year_week_day(2019, 1), "nm")
  year <- vec_proxy(x)$year
  expect_named(year, "nm")
})

test_that("restore works", {
  to <- year_week_day(2019, 1:5)
  proxy <- vec_slice(vec_proxy(to), 1:2)
  expect_identical(vec_restore(proxy, to), year_week_day(2019, 1:2))
})

# ------------------------------------------------------------------------------
# vec_ptype_full()

test_that("full ptype is correct", {
  expect_snapshot_output(vec_ptype_full(year_week_day(2019)))
  expect_snapshot_output(vec_ptype_full(year_week_day(2019, start = 2)))
  expect_snapshot_output(vec_ptype_full(year_week_day(2019, 1, 1)))
  expect_snapshot_output(
    vec_ptype_full(
      year_week_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")
    )
  )
  expect_snapshot_output(vec_ptype_full(year_week_day(2019, 53)))
})

# ------------------------------------------------------------------------------
# vec_ptype_abbr()

test_that("abbreviated ptype is correct", {
  expect_snapshot_output(vec_ptype_abbr(year_week_day(2019)))
  expect_snapshot_output(vec_ptype_abbr(year_week_day(2019, start = 2)))
  expect_snapshot_output(vec_ptype_abbr(year_week_day(2019, 1, 1)))
  expect_snapshot_output(
    vec_ptype_abbr(
      year_week_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")
    )
  )
  expect_snapshot_output(vec_ptype_abbr(year_week_day(2019, 53)))
})

# ------------------------------------------------------------------------------
# set_*()

test_that("setters work", {
  x <- year_week_day(1L)

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
  x <- year_week_day(2019, c(1, NA, 3))
  x <- set_day(x, c(NA, 2, 4))
  expect_identical(vec_detect_missing(x), c(TRUE, TRUE, FALSE))
})

test_that("setters recycling works both ways", {
  x <- year_week_day(2019)

  x <- set_week(x, 1:2)
  expect_identical(x, year_week_day(2019, 1:2))

  x <- set_day(x, 1)
  expect_identical(x, year_week_day(2019, 1:2, 1))

  expect_snapshot(error = TRUE, {
    x <- year_week_day(1:2)
    y <- 1:3
    set_week(x, y)
  })
})

test_that("setters require integer `value`", {
  x <- year_week_day(2019, 1, 2, 3, 4, 5)

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
  x <- year_week_day(2019, 1, 2, 3, 4, 5)

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
    set_day(year_week_day(year = 1), 1)
  })
  expect_snapshot(error = TRUE, {
    set_hour(year_week_day(year = 1, week = 2), 1)
  })
  expect_snapshot(error = TRUE, {
    set_minute(year_week_day(year = 1, week = 2, day = 3), 1)
  })
  expect_snapshot(error = TRUE, {
    set_second(year_week_day(year = 1, week = 2, day = 3, hour = 4), 1)
  })
  expect_snapshot(error = TRUE, {
    set_millisecond(year_week_day(year = 1, week = 2, day = 3, hour = 4, minute = 5), 1)
  })
  expect_snapshot(error = TRUE, {
    set_microsecond(year_week_day(year = 1, week = 2, day = 3, hour = 4, minute = 5), 1)
  })
  expect_snapshot(error = TRUE, {
    set_nanosecond(year_week_day(year = 1, week = 2, day = 3, hour = 4, minute = 5), 1)
  })
})

test_that("setters require correct subsecond precision", {
  expect_snapshot(error = TRUE, {
    set_millisecond(
      year_week_day(
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
      year_week_day(
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
      year_week_day(
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
      year_week_day(
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
      year_week_day(
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
      year_week_day(
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
  x <- year_week_day(2019)
  x <- set_names(x, "foo")
  expect_named(set_week(x, 2), "foo")
})

test_that("setting with named `value` strips its names", {
  x <- year_week_day(2019)
  x <- set_week(x, set_names(1L, "x"))
  expect_named(field(x, "week"), NULL)
})

# ------------------------------------------------------------------------------
# format()

test_that("default formats are correct", {
  expect_snapshot(format(year_week_day(2019)))
  expect_snapshot(format(year_week_day(2019, 1)))
  expect_snapshot(format(year_week_day(2019, 1, 1, 1)))
  expect_snapshot(
    format(year_week_day(2019, 1, 1, 1, 2, 3, 50, subsecond_precision = "microsecond"))
  )
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- year_week_day(2019, 1)
  y <- year_week_day(2019, 1, 2)

  expect_identical(as.character(x), format(x))
  expect_identical(as.character(y), format(y))
})

# ------------------------------------------------------------------------------
# calendar_narrow()

test_that("can narrow to week", {
  x_expect <- year_week_day(2019, 2)
  x <- set_day(x_expect, 1)
  expect_identical(calendar_narrow(x, "week"), x_expect)
  expect_identical(calendar_narrow(x_expect, "week"), x_expect)
})

test_that("can narrow to day", {
  x_expect <- year_week_day(2019, 2, 3)
  x <- set_hour(x_expect, 5)
  expect_identical(calendar_narrow(x, "day"), x_expect)
  expect_identical(calendar_narrow(x_expect, "day"), x_expect)
})

# ------------------------------------------------------------------------------
# calendar_widen()

test_that("can widen to week", {
  x <- year_week_day(2019)
  expect_identical(calendar_widen(x, "week"), set_week(x, 1))
})

test_that("can widen to day", {
  x <- year_week_day(2019)
  y <- year_week_day(2019, 02)
  expect_identical(calendar_widen(x, "day"), set_day(set_week(x, 1), 1))
  expect_identical(calendar_widen(y, "day"), set_day(y, 1))
})

# ------------------------------------------------------------------------------
# calendar_start()

test_that("can compute year start", {
  x <- year_week_day(2019)
  expect_identical(calendar_start(x, "year"), x)

  x <- year_week_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_week_day(2019, 1, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  expect_identical(calendar_start(x, "year"), expect)
})

test_that("can compute week start", {
  x <- year_week_day(2019, 2)
  expect_identical(calendar_start(x, "week"), x)

  x <- year_week_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_week_day(2019, 2, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  expect_identical(calendar_start(x, "week"), expect)
})

# ------------------------------------------------------------------------------
# calendar_end()

test_that("can compute year end", {
  x <- year_week_day(2019)
  expect_identical(calendar_end(x, "year"), x)

  x <- year_week_day(2019:2020, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_week_day(
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
  x <- year_week_day(2019, 2)
  expect_identical(calendar_end(x, "week"), x)

  x <- year_week_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_week_day(
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
# calendar_count_between()

test_that("can compute year counts", {
  x <- year_week_day(2019, 1, 1)
  y <- year_week_day(2020, 3, 4)

  expect_identical(calendar_count_between(x, y, "year"), 1L)
})

test_that("can't compute a unsupported count precision", {
  x <- year_week_day(2019, 1, 1)
  expect_snapshot((expect_error(calendar_count_between(x, x, "week"))))
})

test_that("positive / negative counts are correct", {
  start <- year_week_day(1972, 03, 04)

  end <- year_week_day(1973, 03, 03)
  expect_identical(calendar_count_between(start, end, "year"), 0L)

  end <- year_week_day(1973, 03, 04)
  expect_identical(calendar_count_between(start, end, "year"), 1L)

  end <- year_week_day(1973, 03, 05)
  expect_identical(calendar_count_between(start, end, "year"), 1L)

  end <- year_week_day(1971, 03, 03)
  expect_identical(calendar_count_between(start, end, "year"), -1L)

  end <- year_week_day(1971, 03, 04)
  expect_identical(calendar_count_between(start, end, "year"), -1L)

  end <- year_week_day(1971, 03, 05)
  expect_identical(calendar_count_between(start, end, "year"), 0L)
})

# ------------------------------------------------------------------------------
# seq()

test_that("only year precision is allowed", {
  expect_snapshot(error = TRUE, seq(year_week_day(2019, 1), by = 1, length.out = 2))
})

test_that("seq(to, by) works", {
  expect_identical(
    seq(year_week_day(2019), to = year_week_day(2024), by = 2),
    year_week_day(c(2019, 2021, 2023))
  )
  expect_identical(
    seq(year_week_day(2019), to = year_week_day(2023), by = 2),
    year_week_day(c(2019, 2021, 2023))
  )
})

test_that("seq(to, length.out) works", {
  expect_identical(
    seq(year_week_day(2019), to = year_week_day(2024), length.out = 2),
    year_week_day(c(2019, 2024))
  )
  expect_identical(
    seq(year_week_day(2019), to = year_week_day(2024), length.out = 6),
    year_week_day(2019:2024)
  )

  expect_identical(
    seq(year_week_day(2019), to = year_week_day(2024), along.with = 1:2),
    year_week_day(c(2019, 2024))
  )
})

test_that("seq(by, length.out) works", {
  expect_identical(
    seq(year_week_day(2019), by = 2, length.out = 3),
    year_week_day(c(2019, 2021, 2023))
  )

  expect_identical(
    seq(year_week_day(2019), by = 2, along.with = 1:3),
    year_week_day(c(2019, 2021, 2023))
  )
})

# ------------------------------------------------------------------------------
# miscellaneous

test_that("can roundtrip to naive-time with any `start`", {
  x <- seq(
    as_naive_time(year_month_day(-9999, 1, 1)),
    as_naive_time(year_month_day(9999, 12, 31)),
    by = 1
  )

  for (start in seq_len(7)) {
    expect_identical(x, as_naive_time(as_year_week_day(x, start = start)))
  }
})

test_that("can generate correct last week of the year with any `start`", {
  start <- 1L

  expect_identical(
    as.Date(year_week_day(2019:2023, "last", 7, start = 1)),
    as.Date(
      year_month_day(
        c(2019, 2021, 2022, 2022, 2023),
        c(12, 1, 1, 12, 12),
        c(28, 2, 1, 31, 30)
      )
    )
  )
  expect_identical(
    as.Date(year_week_day(2019:2023, "last", 7, start = 2)),
    as.Date(
      year_month_day(
        c(2019, 2021, 2022, 2023, 2023),
        c(12, 1, 1, 1, 12),
        c(29, 3, 2, 1, 31)
      )
    )
  )
  expect_identical(
    as.Date(year_week_day(2019:2023, "last", 7, start = 3)),
    as.Date(
      year_month_day(
        c(2019, 2020, 2022, 2023, 2024),
        c(12, 12, 1, 1, 1),
        c(30, 28, 3, 2, 1)
      )
    )
  )
  # ..., 4, 5, ...
  expect_identical(
    as.Date(year_week_day(2019:2023, "last", 7, start = 6)),
    as.Date(
      year_month_day(
        c(2020, 2020, 2021, 2022, 2023),
        c(1, 12, 12, 12, 12),
        c(2, 31, 30, 29, 28)
      )
    )
  )
  expect_identical(
    as.Date(year_week_day(2019:2023, "last", 7, start = 7)),
    as.Date(
      year_month_day(
        c(2020, 2021, 2021, 2022, 2023),
        c(1, 1, 12, 12, 12),
        c(3, 1, 31, 30, 29)
      )
    )
  )
})

# ------------------------------------------------------------------------------
# invalid_detect()

test_that("`invalid_detect()` works", {
  # Not possible to be invalid
  x <- year_week_day(2019:2020)
  expect_identical(invalid_detect(x), c(FALSE, FALSE))

  # Now possible
  x <- year_week_day(2019, c(1, 52, 53, NA))
  expect_identical(invalid_detect(x), c(FALSE, FALSE, TRUE, FALSE))

  # Possible after that too
  x <- year_week_day(2019, c(1, 52, 53, NA), 1)
  expect_identical(invalid_detect(x), c(FALSE, FALSE, TRUE, FALSE))
})

test_that("`invalid_detect()` works with different `start`", {
  x <- year_week_day(2024:2025, 53, start = clock_weekdays$sunday)
  expect_identical(invalid_detect(x), c(TRUE, FALSE))

  x <- year_week_day(2024:2025, 53, start = clock_weekdays$monday)
  expect_identical(invalid_detect(x), c(TRUE, TRUE))
})

# ------------------------------------------------------------------------------
# invalid_any()

test_that("`invalid_any()` works", {
  # Not possible to be invalid
  x <- year_week_day(2019:2020)
  expect_false(invalid_any(x))

  # Now possible
  x <- year_week_day(2019, c(1, 52, 53, NA))
  expect_true(invalid_any(x))

  # Possible after that too
  x <- year_week_day(2019, c(1, 52, 53, NA), 1)
  expect_true(invalid_any(x))
})

test_that("`invalid_any()` works with different `start`", {
  x <- year_week_day(2024:2025, 53, start = clock_weekdays$sunday)
  expect_identical(invalid_any(x), TRUE)

  x <- year_week_day(2024:2025, 53, start = clock_weekdays$monday)
  expect_identical(invalid_any(x), TRUE)
})

# ------------------------------------------------------------------------------
# invalid_count()

test_that("`invalid_count()` works", {
  # Not possible to be invalid
  x <- year_week_day(2019:2020)
  expect_identical(invalid_count(x), 0L)

  # Now possible
  x <- year_week_day(2019, c(1, 52, 53, NA))
  expect_identical(invalid_count(x), 1L)

  # Possible after that too
  x <- year_week_day(2019, c(1, 52, 53, NA), 1)
  expect_identical(invalid_count(x), 1L)
})

test_that("`invalid_count()` works with different `start`", {
  x <- year_week_day(2024:2025, 53, start = clock_weekdays$sunday)
  expect_identical(invalid_count(x), 1L)

  x <- year_week_day(2024:2025, 53, start = clock_weekdays$monday)
  expect_identical(invalid_count(x), 2L)
})

# ------------------------------------------------------------------------------
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot(error = TRUE, invalid_resolve(year_week_day(2019, 1)))
})

test_that("can resolve correctly", {
  x <- year_week_day(2019, 53, 2, 2, 3, 4, 5, subsecond_precision = "millisecond")

  expect_identical(
    invalid_resolve(x, invalid = "previous"),
    year_week_day(2019, 52, 7, 23, 59, 59, 999, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "previous-day"),
    year_week_day(2019, 52, 7, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "next"),
    year_week_day(2020, 01, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "next-day"),
    year_week_day(2020, 01, 1, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "overflow"),
    year_week_day(2020, 01, 02, 0, 0, 0, 0, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "overflow-day"),
    year_week_day(2020, 01, 02, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "NA"),
    year_week_day(NA, NA, NA, NA, NA, NA, NA, subsecond_precision = "millisecond")
  )
})

test_that("throws known classed error", {
  expect_snapshot(error = TRUE, invalid_resolve(year_week_day(2019, 53)))
  expect_error(
    invalid_resolve(year_week_day(2019, 53)),
    class = "clock_error_invalid_date"
  )
})

test_that("works with always valid precisions", {
  x <- year_week_day(2019:2020)
  expect_identical(invalid_resolve(x), x)
})

# ------------------------------------------------------------------------------
# vec_math()

test_that("is.nan() works", {
  x <- year_week_day(c(2019, NA))
  expect_identical(is.nan(x), c(FALSE, FALSE))
})

test_that("is.finite() works", {
  x <- year_week_day(c(2019, NA))
  expect_identical(is.finite(x), c(TRUE, FALSE))
})

test_that("is.infinite() works", {
  x <- year_week_day(c(2019, NA))
  expect_identical(is.infinite(x), c(FALSE, FALSE))
})

# ------------------------------------------------------------------------------
# clock_minimum() / clock_maximum()

test_that("minimums are right", {
  expect_snapshot({
    clock_minimum(year_week_day(1))
    clock_minimum(year_week_day(1, 1))
    clock_minimum(year_week_day(1, 1, 1))
    clock_minimum(year_week_day(1, 1, 1, 1))
    clock_minimum(year_week_day(1, 1, 1, 1, 1))
    clock_minimum(year_week_day(1, 1, 1, 1, 1, 1))
    clock_minimum(
      year_week_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "millisecond")
    )
    clock_minimum(
      year_week_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "microsecond")
    )
    clock_minimum(
      year_week_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")
    )
  })
})

test_that("maximums are right", {
  expect_snapshot({
    clock_maximum(year_week_day(1))
    clock_maximum(year_week_day(1, 1))
    clock_maximum(year_week_day(1, 1, 1))
    clock_maximum(year_week_day(1, 1, 1, 1))
    clock_maximum(year_week_day(1, 1, 1, 1, 1))
    clock_maximum(year_week_day(1, 1, 1, 1, 1, 1))
    clock_maximum(
      year_week_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "millisecond")
    )
    clock_maximum(
      year_week_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "microsecond")
    )
    clock_maximum(
      year_week_day(1, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")
    )
  })
})

test_that("minimums and maximums respect `start`", {
  expect_snapshot({
    clock_minimum(year_week_day(1, start = clock_weekdays$friday))
    clock_maximum(year_week_day(1, start = clock_weekdays$friday))
  })
})

# ------------------------------------------------------------------------------
# min() / max() / range()

test_that("min() / max() / range() works", {
  x <- year_week_day(c(1, 3, 2, 1, -1))

  expect_identical(min(x), year_week_day(-1))
  expect_identical(max(x), year_week_day(3))
  expect_identical(range(x), year_week_day(c(-1, 3)))
})

test_that("min() / max() / range() works with `NA`", {
  x <- year_week_day(c(1, NA, 2, 0))

  expect_identical(min(x), year_week_day(NA))
  expect_identical(max(x), year_week_day(NA))
  expect_identical(range(x), year_week_day(c(NA, NA)))

  expect_identical(min(x, na.rm = TRUE), year_week_day(0))
  expect_identical(max(x, na.rm = TRUE), year_week_day(2))
  expect_identical(range(x, na.rm = TRUE), year_week_day(c(0, 2)))
})

test_that("min() / max() / range() works when empty", {
  x <- year_week_day(integer())

  expect_identical(min(x), clock_maximum(x))
  expect_identical(max(x), clock_minimum(x))
  expect_identical(range(x), c(clock_maximum(x), clock_minimum(x)))

  x <- year_week_day(c(NA, NA))

  expect_identical(min(x, na.rm = TRUE), clock_maximum(x))
  expect_identical(max(x, na.rm = TRUE), clock_minimum(x))
  expect_identical(range(x, na.rm = TRUE), c(clock_maximum(x), clock_minimum(x)))
})

# ------------------------------------------------------------------------------
# add_*()

test_that("add_years() works", {
  x <- year_week_day(2019, 1, 2, 3:4)

  expect_identical(
    add_years(x, 1:2),
    year_week_day(c(2020, 2021), 1, 2, 3:4)
  )
  expect_identical(
    add_years(x, NA),
    vec_init(x, 2L)
  )
})

test_that("add_*() respect recycling rules", {
  expect_length(add_years(year_week_day(1), 1:2), 2L)
  expect_length(add_years(year_week_day(1:2), 1), 2L)

  expect_length(add_years(year_week_day(1), integer()), 0L)
  expect_length(add_years(year_week_day(integer()), 1), 0L)

  expect_snapshot(error = TRUE, {
    add_years(year_week_day(1:2), 1:3)
  })
})

test_that("add_*() retains names", {
  x <- set_names(year_week_day(1), "x")
  y <- year_week_day(1)

  n <- set_names(1, "n")

  expect_named(add_years(x, n), "x")
  expect_named(add_years(y, n), "n")
})

test_that("`start` value is retained", {
  expect_identical(
    year_week_day(2019, 1, 1) + duration_years(1),
    year_week_day(2020, 1, 1)
  )
  expect_identical(
    year_week_day(2019, 1, 1) + duration_years(5),
    year_week_day(2024, 1, 1)
  )

  # Ensure that the `start` is retained
  expect_identical(
    year_week_day(2019, 1, 1, start = 2) + duration_years(5),
    year_week_day(2024, 1, 1, start = 2)
  )
})

# ------------------------------------------------------------------------------
# diff()

test_that("works with `lag` and `differences`", {
  x <- year_week_day(2019:2026)
  expect_identical(diff(x), rep(duration_years(1), 7))
  expect_identical(diff(x, lag = 2), rep(duration_years(2), 6))
  expect_identical(diff(x, differences = 2), rep(duration_years(0), 6))
})

test_that("works with `lag` and `differences` that force an empty result (#364)", {
  expect_identical(diff(year_week_day(integer())), duration_years())
  expect_identical(diff(year_week_day(1)), duration_years())
  expect_identical(
    diff(year_week_day(1:8), lag = 4, differences = 3),
    duration_years()
  )
})

test_that("errors on invalid precisions", {
  expect_snapshot(error = TRUE, {
    diff(year_week_day(2019, 1))
  })
})

test_that("errors on invalid lag/differences", {
  # These are base R errors we don't control
  expect_error(diff(year_week_day(2019), lag = 1:2))
  expect_error(diff(year_week_day(2019), differences = 1:2))
})
