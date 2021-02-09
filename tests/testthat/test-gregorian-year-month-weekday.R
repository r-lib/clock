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
# calendar_month_factor()

test_that("can get a month factor", {
  expect_identical(
    calendar_month_factor(year_month_weekday(2019, 1:12)),
    factor(month.name, levels = month.name, ordered = TRUE)
  )
})

# ------------------------------------------------------------------------------
# seq()

test_that("only granular precisions are allowed", {
  expect_snapshot_error(seq(year_month_weekday(2019, 1, 1, 1), by = 1, length.out = 2))
})

# NOTE: Most tests are done by `year_month_day()` since they share an implementation
test_that("can generate a sequence", {
  expect_identical(seq(year_month_day(2019, 1), to = year_month_day(2019, 6), by = 2), year_month_day(2019, c(1, 3, 5)))
})

# ------------------------------------------------------------------------------
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot_error(invalid_resolve(year_month_weekday(2019, 1, 1, 1)))
})
