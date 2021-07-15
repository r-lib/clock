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
  expect_snapshot_error(iso_year_week_day(50000))
  expect_snapshot_error(iso_year_week_day(2020, 54))
  expect_snapshot_error(iso_year_week_day(2020, 1, 8))
  expect_snapshot_error(iso_year_week_day(2020, 1, 1, 24))
  expect_snapshot_error(iso_year_week_day(2020, 1, 1, 1, 60))
  expect_snapshot_error(iso_year_week_day(2020, 1, 1, 1, 1, 60))
  expect_snapshot_error(iso_year_week_day(2020, 1, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond"))
  expect_snapshot_error(iso_year_week_day(2020, 1, 1, 1, 1, 1, 1000000, subsecond_precision = "microsecond"))
  expect_snapshot_error(iso_year_week_day(2020, 1, 1, 1, 1, 1, 1000000000, subsecond_precision = "nanosecond"))
})

test_that("can get the last week of the iso year", {
  x <- iso_year_week_day(2019:2020, "last")
  expect_identical(get_week(x), c(52L, 53L))
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
  expect_identical(vec_proxy(iso_year_week_day(2019, 1)), data_frame(year = 2019L, week = 1L))
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
  expect_snapshot_output(vec_ptype_full(iso_year_week_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")))
  expect_snapshot_output(vec_ptype_full(iso_year_week_day(2019, 53)))
})

# ------------------------------------------------------------------------------
# vec_ptype_abbr()

test_that("abbreviated ptype is correct", {
  expect_snapshot_output(vec_ptype_abbr(iso_year_week_day(2019)))
  expect_snapshot_output(vec_ptype_abbr(iso_year_week_day(2019, 1, 1)))
  expect_snapshot_output(vec_ptype_abbr(iso_year_week_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")))
  expect_snapshot_output(vec_ptype_abbr(iso_year_week_day(2019, 53)))
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
  expect <- iso_year_week_day(2019, 1, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  expect_identical(calendar_start(x, "year"), expect)
})

test_that("can compute week start", {
  x <- iso_year_week_day(2019, 2)
  expect_identical(calendar_start(x, "week"), x)

  x <- iso_year_week_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- iso_year_week_day(2019, 2, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  expect_identical(calendar_start(x, "week"), expect)
})

# ------------------------------------------------------------------------------
# calendar_end()

test_that("can compute year end", {
  x <- iso_year_week_day(2019)
  expect_identical(calendar_end(x, "year"), x)

  x <- iso_year_week_day(2019:2020, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- iso_year_week_day(2019:2020, 52:53, 7, 23, 59, 59, 999L, subsecond_precision = "millisecond")
  expect_identical(calendar_end(x, "year"), expect)
})

test_that("can compute week end", {
  x <- iso_year_week_day(2019, 2)
  expect_identical(calendar_end(x, "week"), x)

  x <- iso_year_week_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- iso_year_week_day(2019, 2, 7, 23, 59, 59, 999L, subsecond_precision = "millisecond")
  expect_identical(calendar_end(x, "week"), expect)
})

# ------------------------------------------------------------------------------
# seq()

test_that("only year precision is allowed", {
  expect_snapshot_error(seq(iso_year_week_day(2019, 1), by = 1, length.out = 2))
})

test_that("seq(to, by) works", {
  expect_identical(seq(iso_year_week_day(2019), to = iso_year_week_day(2024), by = 2), iso_year_week_day(c(2019, 2021, 2023)))
  expect_identical(seq(iso_year_week_day(2019), to = iso_year_week_day(2023), by = 2), iso_year_week_day(c(2019, 2021, 2023)))
})

test_that("seq(to, length.out) works", {
  expect_identical(seq(iso_year_week_day(2019), to = iso_year_week_day(2024), length.out = 2), iso_year_week_day(c(2019, 2024)))
  expect_identical(seq(iso_year_week_day(2019), to = iso_year_week_day(2024), length.out = 6), iso_year_week_day(2019:2024))

  expect_identical(seq(iso_year_week_day(2019), to = iso_year_week_day(2024), along.with = 1:2), iso_year_week_day(c(2019, 2024)))
})

test_that("seq(by, length.out) works", {
  expect_identical(seq(iso_year_week_day(2019), by = 2, length.out = 3), iso_year_week_day(c(2019, 2021, 2023)))

  expect_identical(seq(iso_year_week_day(2019), by = 2, along.with = 1:3), iso_year_week_day(c(2019, 2021, 2023)))
})

# ------------------------------------------------------------------------------
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot_error(invalid_resolve(iso_year_week_day(2019, 1)))
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
