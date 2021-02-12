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
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot_error(invalid_resolve(year_quarter_day(2019, 1, 1)))
})
