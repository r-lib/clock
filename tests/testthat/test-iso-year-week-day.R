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
