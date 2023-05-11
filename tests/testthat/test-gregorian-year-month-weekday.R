# ------------------------------------------------------------------------------
# year_month_weekday()

test_that("helper can create different precisions", {
  x <- year_month_weekday(2019, 1:2)
  expect_identical(get_year(x), c(2019L, 2019L))
  expect_identical(get_month(x), 1:2)

  x <- year_month_weekday(2019, 1:2, clock_weekdays$monday, 2)
  expect_identical(get_day(x), c(clock_weekdays$monday, clock_weekdays$monday))
  expect_identical(get_index(x), c(2L, 2L))
})

test_that("can create subsecond precision calendars", {
  x <- year_month_weekday(2019, 1, 1, 1, 0, 0, 0, 1, subsecond_precision = "millisecond")
  expect_identical(get_millisecond(x), 1L)

  x <- year_month_weekday(2019, 1, 1, 1, 0, 0, 0, 1, subsecond_precision = "microsecond")
  expect_identical(get_microsecond(x), 1L)

  x <- year_month_weekday(2019, 1, 1, 1, 0, 0, 0, 1, subsecond_precision = "nanosecond")
  expect_identical(get_nanosecond(x), 1L)
})

test_that("both day and index must be specified", {
  expect_snapshot(error = TRUE, year_month_weekday(2020, 1, 1))
})

test_that("validates value ranges", {
  expect_snapshot(error = TRUE, year_month_weekday(50000))
  expect_snapshot(error = TRUE, year_month_weekday(2020, 13))
  expect_snapshot(error = TRUE, year_month_weekday(2020, 1, 32, 1))
  expect_snapshot(error = TRUE, year_month_weekday(2020, 1, 1, 6))
  expect_snapshot(error = TRUE, year_month_weekday(2020, 1, 1, 1, 24))
  expect_snapshot(error = TRUE, year_month_weekday(2020, 1, 1, 1, 1, 60))
  expect_snapshot(error = TRUE, year_month_weekday(2020, 1, 1, 1, 1, 1, 60))
  expect_snapshot(error = TRUE, year_month_weekday(2020, 1, 1, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond"))
  expect_snapshot(error = TRUE, year_month_weekday(2020, 1, 1, 1, 1, 1, 1, 1000000, subsecond_precision = "microsecond"))
  expect_snapshot(error = TRUE, year_month_weekday(2020, 1, 1, 1, 1, 1, 1, 1000000000, subsecond_precision = "nanosecond"))
})

test_that("can get the last indexed weekday of the month", {
  x <- year_month_weekday(2019, 1:4, clock_weekdays$monday, "last")
  expect_identical(get_index(x), c(4L, 4L, 4L, 5L))
})

test_that("`NA` propagates through 'last'", {
  x <- year_month_weekday(2019, c(1, NA), clock_weekdays$monday, 1)
  x <- set_index(x, "last")
  expect_identical(get_index(x), c(4L, NA))
})

test_that("ignores values past first `NULL`", {
  expect_identical(year_month_weekday(2019, minute = 2), year_month_weekday(2019))
})

test_that("NA values propagate", {
  x <- year_month_weekday(2019, 1:3, c(NA, 2, 3), c(3, 4, NA))
  expect_identical(is.na(x), c(TRUE, FALSE, TRUE))
})

# ------------------------------------------------------------------------------
# vec_ptype()

test_that("ptype is correct", {
  base <- year_month_weekday(1)
  ptype <- year_month_weekday(integer())

  for (precision in precision_names()) {
    if (precision == "quarter" || precision == "week") {
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
  expect_identical(vec_proxy(year_month_weekday(2019)), data_frame(year = 2019L))
  expect_identical(vec_proxy(year_month_weekday(2019, 1)), data_frame(year = 2019L, month = 1L))
})

test_that("proxy has names on `year`", {
  x <- set_names(year_month_weekday(2019, 1), "nm")
  year <- vec_proxy(x)$year
  expect_named(year, "nm")
})

test_that("restore works", {
  to <- year_month_weekday(2019, 1:5)
  proxy <- vec_slice(vec_proxy(to), 1:2)
  expect_identical(vec_restore(proxy, to), year_month_weekday(2019, 1:2))
})

# ------------------------------------------------------------------------------
# vec_proxy_compare()

test_that("can compare with year / month precision", {
  expect_identical(
    year_month_weekday(2019) > year_month_weekday(2018:2020),
    c(TRUE, FALSE, FALSE)
  )
  expect_identical(
    year_month_weekday(2019, 2) > year_month_weekday(2019, 1:3),
    c(TRUE, FALSE, FALSE)
  )
})

test_that("cannot compare / sort with day precision or finer", {
  x <- year_month_weekday(2019, 1, 1, 1)

  expect_snapshot(error = TRUE, x > x)
  expect_snapshot(error = TRUE, vec_order(x))
})

# ------------------------------------------------------------------------------
# vec_ptype_full()

test_that("full ptype is correct", {
  expect_snapshot_output(vec_ptype_full(year_month_weekday(2019)))
  expect_snapshot_output(vec_ptype_full(year_month_weekday(2019, 1, 1, 2)))
  expect_snapshot_output(vec_ptype_full(year_month_weekday(2019, 1, 1, 2, 1, 1, 1, 1, subsecond_precision = "nanosecond")))
  expect_snapshot_output(vec_ptype_full(year_month_weekday(2019, 1, 1, 5)))
})

# ------------------------------------------------------------------------------
# vec_ptype_abbr()

test_that("abbreviated ptype is correct", {
  expect_snapshot_output(vec_ptype_abbr(year_month_weekday(2019)))
  expect_snapshot_output(vec_ptype_abbr(year_month_weekday(2019, 1, 1, 2)))
  expect_snapshot_output(vec_ptype_abbr(year_month_weekday(2019, 1, 1, 2, 1, 1, 1, 1, subsecond_precision = "nanosecond")))
  expect_snapshot_output(vec_ptype_abbr(year_month_weekday(2019, 1, 1, 5)))
})

# ------------------------------------------------------------------------------
# set_*()

test_that("setters work", {
  x <- year_month_weekday(1L)

  x <- set_year(x, 2L)
  expect_identical(get_year(x), 2L)

  x <- set_month(x, 1L)
  expect_identical(get_month(x), 1L)

  x <- set_day(x, 2L, index = 1L)
  expect_identical(get_day(x), 2L)
  expect_identical(get_index(x), 1L)

  x <- set_index(x, 3L)
  expect_identical(get_index(x), 3L)

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
  x <- year_month_weekday(2019, c(1, NA, 3, 4))
  x <- set_day(x, c(NA, 2, 4, 5), index = c(1, 2, 3, NA))
  expect_identical(vec_detect_missing(x), c(TRUE, TRUE, FALSE, TRUE))
})

test_that("setters recycling works both ways", {
  x <- year_month_weekday(2019)

  x <- set_month(x, 1:2)
  expect_identical(x, year_month_weekday(2019, 1:2))

  x <- set_day(x, 1, index = 1)
  expect_identical(x, year_month_weekday(2019, 1:2, 1, 1))

  expect_snapshot(error = TRUE, {
    x <- year_month_weekday(1:2)
    y <- 1:3
    set_month(x, y)
  })
})

test_that("setters require integer `value`", {
  x <- year_month_weekday(2019, 1, 2, 3, 4, 5, 6)

  expect_snapshot(error = TRUE, {
    set_year(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_month(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_day(x, 1.5)
  })
  expect_snapshot(error = TRUE, {
    set_index(x, 1.5)
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
  x <- year_month_weekday(2019, 1, 2, 3, 4, 5, 6)

  expect_snapshot(error = TRUE, {
    set_year(x, 100000)
  })
  expect_snapshot(error = TRUE, {
    set_month(x, 13)
  })
  expect_snapshot(error = TRUE, {
    set_day(x, 8)
  })
  expect_snapshot(error = TRUE, {
    set_index(x, 6)
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
    set_day(year_month_weekday(year = 1), 1, index = 1)
  })
  expect_snapshot(error = TRUE, {
    set_hour(year_month_weekday(year = 1, month = 2), 1)
  })
  expect_snapshot(error = TRUE, {
    set_minute(year_month_weekday(year = 1, month = 2, day = 3, index = 1), 1)
  })
  expect_snapshot(error = TRUE, {
    set_second(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4), 1)
  })
  expect_snapshot(error = TRUE, {
    set_millisecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4, minute = 5), 1)
  })
  expect_snapshot(error = TRUE, {
    set_microsecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4, minute = 5), 1)
  })
  expect_snapshot(error = TRUE, {
    set_nanosecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4, minute = 5), 1)
  })
})

test_that("setters require correct subsecond precision", {
  expect_snapshot(error = TRUE, {
    set_millisecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "microsecond"), 1)
  })
  expect_snapshot(error = TRUE, {
    set_millisecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "nanosecond"), 1)
  })

  expect_snapshot(error = TRUE, {
    set_microsecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "millisecond"), 1)
  })
  expect_snapshot(error = TRUE, {
    set_microsecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "nanosecond"), 1)
  })

  expect_snapshot(error = TRUE, {
    set_nanosecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "millisecond"), 1)
  })
  expect_snapshot(error = TRUE, {
    set_nanosecond(year_month_weekday(year = 1, month = 2, day = 3, index = 1, hour = 4, minute = 5, second = 6, subsecond = 7, subsecond_precision = "microsecond"), 1)
  })
})

test_that("setters retain names", {
  x <- year_month_weekday(2019)
  x <- set_names(x, "foo")
  expect_named(set_month(x, 2), "foo")
})

test_that("setting with named `value` strips its names", {
  x <- year_month_weekday(2019)
  x <- set_month(x, set_names(1L, "x"))
  expect_named(field(x, "month"), NULL)
})

# ------------------------------------------------------------------------------
# format()

test_that("default formats are correct", {
  expect_snapshot(format(year_month_weekday(2019)))
  expect_snapshot(format(year_month_weekday(2019, 1)))
  expect_snapshot(format(year_month_weekday(2019, 1, 1, 2, 1)))
  expect_snapshot(format(year_month_weekday(2019, 1, 1, 2, 1, 2, 3, 50, subsecond_precision = "microsecond")))
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- year_month_weekday(2019, 1)
  y <- year_month_weekday(2019, 1, 2, 1)

  expect_identical(as.character(x), format(x))
  expect_identical(as.character(y), format(y))
})

# ------------------------------------------------------------------------------
# calendar_narrow()

test_that("can narrow to month", {
  x_expect <- year_month_weekday(2019, 2)
  x <- set_day(x_expect, 1, index = 2)
  expect_identical(calendar_narrow(x, "month"), x_expect)
  expect_identical(calendar_narrow(x_expect, "month"), x_expect)
})

test_that("can narrow to day", {
  x_expect <- year_month_weekday(2019, 2, 3, 2)
  x <- set_hour(x_expect, 5)
  expect_identical(calendar_narrow(x, "day"), x_expect)
  expect_identical(calendar_narrow(x_expect, "day"), x_expect)
})

# ------------------------------------------------------------------------------
# calendar_widen()

test_that("can widen to month", {
  x <- year_month_weekday(2019)
  expect_identical(calendar_widen(x, "month"), set_month(x, 1))
})

test_that("can widen to day", {
  x <- year_month_weekday(2019)
  y <- year_month_weekday(2019, 02)
  expect_identical(calendar_widen(x, "day"), set_day(set_month(x, 1), 1, index = 1))
  expect_identical(calendar_widen(y, "day"), set_day(y, 1, index = 1))
})

# ------------------------------------------------------------------------------
# calendar_start()

test_that("can compute year start", {
  x <- year_month_weekday(2019)
  expect_identical(calendar_start(x, "year"), x)

  x <- year_month_weekday(2019, 2)
  expect_identical(calendar_start(x, "year"), year_month_weekday(2019, 1))
})

test_that("can compute month start", {
  x <- year_month_weekday(2019, 2)
  expect_identical(calendar_start(x, "month"), x)
})

test_that("can't compute start with a year_month_weekday at day precision or greater", {
  expect_snapshot(error = TRUE, calendar_start(year_month_weekday(2019, 2, 2, 2), "day"))
  expect_snapshot(error = TRUE, calendar_start(year_month_weekday(2019, 2, 2, 2), "month"))
})

# ------------------------------------------------------------------------------
# calendar_end()

test_that("can compute year end", {
  x <- year_month_weekday(2019)
  expect_identical(calendar_end(x, "year"), x)

  x <- year_month_weekday(2019, 2)
  expect_identical(calendar_end(x, "year"), year_month_weekday(2019, 12))
})

test_that("can compute month end", {
  x <- year_month_weekday(2019, 2)
  expect_identical(calendar_end(x, "month"), x)
})

test_that("can't compute end with a year_month_weekday at day precision or greater", {
  expect_snapshot(error = TRUE, calendar_end(year_month_weekday(2019, 2, 2, 2), "day"))
  expect_snapshot(error = TRUE, calendar_end(year_month_weekday(2019, 2, 2, 2), "month"))
})

# ------------------------------------------------------------------------------
# calendar_month_factor()

test_that("can get a month factor", {
  expect_identical(
    calendar_month_factor(year_month_weekday(2019, 1:12)),
    factor(month.name, levels = month.name, ordered = TRUE)
  )
})

# ------------------------------------------------------------------------------
# calendar_count_between()

test_that("can compute year and month counts", {
  x <- year_month_weekday(2019, 1)
  y <- year_month_weekday(2020, 3)

  expect_identical(calendar_count_between(x, y, "year"), 1L)
  expect_identical(calendar_count_between(x, y, "month"), 14L)
  expect_identical(calendar_count_between(x, y, "month", n = 2), 7L)
})

test_that("can compute a quarter count", {
  x <- year_month_weekday(2019, 1)

  y <- year_month_weekday(2019, c(3, 5))
  expect_identical(calendar_count_between(x, y, "quarter"), c(0L, 1L))
  expect_identical(
    calendar_count_between(x, y, "quarter"),
    calendar_count_between(x, y, "month", n = 3L)
  )

  y <- year_month_weekday(2020, c(3, 5))
  expect_identical(calendar_count_between(x, y, "quarter", n = 2L), c(2L, 2L))
  expect_identical(
    calendar_count_between(x, y, "quarter", n = 2L),
    calendar_count_between(x, y, "month", n = 6L)
  )
})

test_that("positive / negative counts are correct", {
  start <- year_month_weekday(1972, 04)


  end <- year_month_weekday(1973, 03)
  expect_identical(calendar_count_between(start, end, "year"), 0L)
  expect_identical(calendar_count_between(start, end, "month"), 11L)

  end <- year_month_weekday(1973, 04)
  expect_identical(calendar_count_between(start, end, "year"), 1L)
  expect_identical(calendar_count_between(start, end, "month"), 12L)

  end <- year_month_weekday(1973, 05)
  expect_identical(calendar_count_between(start, end, "year"), 1L)
  expect_identical(calendar_count_between(start, end, "month"), 13L)


  end <- year_month_weekday(1971, 03)
  expect_identical(calendar_count_between(start, end, "year"), -1L)
  expect_identical(calendar_count_between(start, end, "month"), -13L)

  end <- year_month_weekday(1971, 04)
  expect_identical(calendar_count_between(start, end, "year"), -1L)
  expect_identical(calendar_count_between(start, end, "month"), -12L)

  end <- year_month_weekday(1971, 05)
  expect_identical(calendar_count_between(start, end, "year"), 0L)
  expect_identical(calendar_count_between(start, end, "month"), -11L)
})

test_that("can't compare a 'year_month_weekday' with day precision!", {
  x <- year_month_weekday(2019, 1, 1, 1)
  expect_snapshot((expect_error(calendar_count_between(x, x, "month"))))
})

# ------------------------------------------------------------------------------
# seq()

test_that("only granular precisions are allowed", {
  expect_snapshot(error = TRUE, seq(year_month_weekday(2019, 1, 1, 1), by = 1, length.out = 2))
})

# NOTE: Most tests are done by `year_month_day()` since they share an implementation
test_that("can generate a sequence", {
  expect_identical(seq(year_month_day(2019, 1), to = year_month_day(2019, 6), by = 2), year_month_day(2019, c(1, 3, 5)))
})

# ------------------------------------------------------------------------------
# invalid_detect()

test_that("`invalid_detect()` works", {
  # Not possible to be invalid
  x <- year_month_weekday(2019:2020, 1:2)
  expect_identical(invalid_detect(x), c(FALSE, FALSE))

  # Now possible
  x <- year_month_weekday(2019, 1, 1, c(1, 2, 5, NA))
  expect_identical(invalid_detect(x), c(FALSE, FALSE, TRUE, FALSE))

  # Possible after that too
  x <- year_month_weekday(2019, 1, 1, c(1, 2, 5, NA), 3)
  expect_identical(invalid_detect(x), c(FALSE, FALSE, TRUE, FALSE))
})

# ------------------------------------------------------------------------------
# invalid_any()

test_that("`invalid_any()` works", {
  # Not possible to be invalid
  x <- year_month_weekday(2019:2020, 1:2)
  expect_false(invalid_any(x))

  # Now possible
  x <- year_month_weekday(2019, 1, 1, c(1, 2, 5, NA))
  expect_true(invalid_any(x))

  # Possible after that too
  x <- year_month_weekday(2019, 1, 1, c(1, 2, 5, NA), 3)
  expect_true(invalid_any(x))
})

# ------------------------------------------------------------------------------
# invalid_count()

test_that("`invalid_count()` works", {
  # Not possible to be invalid
  x <- year_month_weekday(2019:2020, 1:2)
  expect_identical(invalid_count(x), 0L)

  # Now possible
  x <- year_month_weekday(2019, 1, 1, c(1, 2, 5, NA))
  expect_identical(invalid_count(x), 1L)

  # Possible after that too
  x <- year_month_weekday(2019, 1, 1, c(1, 2, 5, NA), 3)
  expect_identical(invalid_count(x), 1L)
})

# ------------------------------------------------------------------------------
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot(error = TRUE, invalid_resolve(year_month_weekday(2019, 1, 1, 1)))
})

test_that("can resolve correctly", {
  x <- year_month_weekday(2019, 1, clock_weekdays$friday, 5, 2, 3, 4, 5, subsecond_precision = "millisecond")

  expect_identical(
    invalid_resolve(x, invalid = "previous"),
    year_month_weekday(2019, 1, clock_weekdays$thursday, 5, 23, 59, 59, 999, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "previous-day"),
    year_month_weekday(2019, 1, clock_weekdays$thursday, 5, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "next"),
    year_month_weekday(2019, 2, clock_weekdays$friday, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "next-day"),
    year_month_weekday(2019, 2, clock_weekdays$friday, 1, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "overflow"),
    year_month_weekday(2019, 2, clock_weekdays$friday, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "overflow-day"),
    year_month_weekday(2019, 2, clock_weekdays$friday, 1, 2, 3, 4, 5, subsecond_precision = "millisecond")
  )
  expect_identical(
    invalid_resolve(x, invalid = "NA"),
    year_month_weekday(NA, NA, NA, NA, NA, NA, NA, NA, subsecond_precision = "millisecond")
  )
})

test_that("throws known classed error", {
  expect_snapshot(error = TRUE, invalid_resolve(year_month_weekday(2019, 1, 1, 5)))
  expect_error(invalid_resolve(year_month_weekday(2019, 1, 1, 5)), class = "clock_error_invalid_date")
})

test_that("works with always valid precisions", {
  x <- year_month_weekday(2019:2020, 1:2)
  expect_identical(invalid_resolve(x), x)
})

# ------------------------------------------------------------------------------
# vec_math()

test_that("is.nan() works", {
  x <- year_month_weekday(c(2019, NA))
  expect_identical(is.nan(x), c(FALSE, FALSE))
})

test_that("is.finite() works", {
  x <- year_month_weekday(c(2019, NA))
  expect_identical(is.finite(x), c(TRUE, FALSE))
})

test_that("is.infinite() works", {
  x <- year_month_weekday(c(2019, NA))
  expect_identical(is.infinite(x), c(FALSE, FALSE))
})

# ------------------------------------------------------------------------------
# clock_minimum() / clock_maximum()

test_that("minimums are right", {
  expect_snapshot({
    clock_minimum(clock_empty_year_month_weekday_year)
    clock_minimum(clock_empty_year_month_weekday_month)
  })

  # Not defined at or past day precision
  expect_snapshot(error = TRUE, {
    clock_minimum(year_month_weekday(1, 1, 1, 1))
  })
})

test_that("maximums are right", {
  expect_snapshot({
    clock_maximum(clock_empty_year_month_weekday_year)
    clock_maximum(clock_empty_year_month_weekday_month)
  })

  # Not defined at or past day precision
  expect_snapshot(error = TRUE, {
    clock_maximum(year_month_weekday(1, 1, 1, 1))
  })
})

# ------------------------------------------------------------------------------
# min() / max() / range()

test_that("min() / max() / range() works", {
  x <- year_month_weekday(c(1, 3, 2, 1, -1))

  expect_identical(min(x), year_month_weekday(-1))
  expect_identical(max(x), year_month_weekday(3))
  expect_identical(range(x), year_month_weekday(c(-1, 3)))
})

test_that("min() / max() / range() works with `NA`", {
  x <- year_month_weekday(c(1, NA, 2, 0))

  expect_identical(min(x), year_month_weekday(NA))
  expect_identical(max(x), year_month_weekday(NA))
  expect_identical(range(x), year_month_weekday(c(NA, NA)))

  expect_identical(min(x, na.rm = TRUE), year_month_weekday(0))
  expect_identical(max(x, na.rm = TRUE), year_month_weekday(2))
  expect_identical(range(x, na.rm = TRUE), year_month_weekday(c(0, 2)))
})

test_that("min() / max() / range() works when empty", {
  x <- year_month_weekday(integer())

  expect_identical(min(x), clock_maximum(x))
  expect_identical(max(x), clock_minimum(x))
  expect_identical(range(x), c(clock_maximum(x), clock_minimum(x)))

  x <- year_month_weekday(c(NA, NA))

  expect_identical(min(x, na.rm = TRUE), clock_maximum(x))
  expect_identical(max(x, na.rm = TRUE), clock_minimum(x))
  expect_identical(range(x, na.rm = TRUE), c(clock_maximum(x), clock_minimum(x)))
})

test_that("min() / max() / range() aren't defined at or past day precision", {
  x <- year_month_weekday(1, 1, 1, 1)

  expect_snapshot(error = TRUE, min(x))
  expect_snapshot(error = TRUE, max(x))
  expect_snapshot(error = TRUE, range(x))

  x <- year_month_weekday(integer(), integer(), integer(), integer())

  expect_snapshot(error = TRUE, min(x))
  expect_snapshot(error = TRUE, max(x))
  expect_snapshot(error = TRUE, range(x))
})

# ------------------------------------------------------------------------------
# add_*()

test_that("add_years() works", {
  x <- year_month_weekday(2019, 1, 2, 3:4, 5)

  expect_identical(
    add_years(x, 1:2),
    year_month_weekday(c(2020, 2021), 1, 2, 3:4, 5)
  )
  expect_identical(
    add_years(x, NA),
    vec_init(x, 2L)
  )
})

test_that("add_months() works", {
  x <- year_month_weekday(2019, 1, 2, 3:4, 5)

  expect_identical(
    add_months(x, 1:2),
    year_month_weekday(2019, 2:3, 2, 3:4, 5)
  )
  expect_identical(
    add_months(x, NA),
    vec_init(x, 2L)
  )
})

test_that("add_quarters() works (special)", {
  x <- year_month_weekday(2019, 1, 2, 3:4, 5)

  expect_identical(
    add_quarters(x, 1:2),
    add_months(x, (1:2) * 3)
  )
  expect_identical(
    add_quarters(x, NA),
    vec_init(x, 2L)
  )
})

test_that("add_*() respect recycling rules", {
  expect_length(add_years(year_month_weekday(1), 1:2), 2L)
  expect_length(add_years(year_month_weekday(1:2), 1), 2L)

  expect_length(add_years(year_month_weekday(1), integer()), 0L)
  expect_length(add_years(year_month_weekday(integer()), 1), 0L)

  expect_snapshot(error = TRUE, {
    add_years(year_month_weekday(1:2), 1:3)
  })
})

test_that("add_*() retains names", {
  x <- set_names(year_month_weekday(1), "x")
  y <- year_month_weekday(1)

  n <- set_names(1, "n")

  expect_named(add_years(x, n), "x")
  expect_named(add_years(y, n), "n")
})
