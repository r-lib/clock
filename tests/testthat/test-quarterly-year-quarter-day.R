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
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot_error(invalid_resolve(year_quarter_day(2019, 1, 1)))
})
