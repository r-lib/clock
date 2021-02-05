# ------------------------------------------------------------------------------
# parse_year_month_day()

test_that("default parses at day precision with ISO format", {
  x <- "2019-01-01"

  expect_identical(
    parse_year_month_day(x),
    year_month_day(2019, 1, 1)
  )
})

test_that("can parse years or year-months", {
  x <- "2019"
  y <- "2019-01"

  expect_identical(
    parse_year_month_day(x, precision = "year"),
    year_month_day(2019)
  )
  expect_identical(
    parse_year_month_day(y, precision = "month"),
    year_month_day(2019, 01)
  )
})

test_that("can parse second and subsecond precision", {
  x <- "2019-01-01 05:10:20"
  y <- "2019-01-01 05:10:20.1234"

  expect_identical(
    parse_year_month_day(x, precision = "second"),
    year_month_day(2019, 1, 1, 5, 10, 20)
  )
  expect_identical(
    parse_year_month_day(y, precision = "millisecond"),
    year_month_day(2019, 1, 1, 5, 10, 20, 123, subsecond_precision = "millisecond")
  )
  expect_identical(
    parse_year_month_day(y, precision = "microsecond"),
    year_month_day(2019, 1, 1, 5, 10, 20, 123400, subsecond_precision = "microsecond")
  )
})

test_that("can parse invalid dates", {
  x <- "2019-2-31"

  expect_identical(
    parse_year_month_day(x),
    year_month_day(2019, 2, 31)
  )
})

test_that("names are retained from the input", {
  x <- c(foo = "2019-01-01")

  expect_named(parse_year_month_day(x), "foo")
})

test_that("multiple formats can be provided", {
  x <- c("2019-01", "2020 Jan")
  formats <- c("%Y %B", "%Y-%m")

  expect_identical(
    parse_year_month_day(x, format = formats, precision = "month"),
    year_month_day(c(2019, 2020), 1)
  )
})

test_that("failure to parse results in `NA`", {
  x <- "2020-01-ohno"

  expect_identical(
    parse_year_month_day(x),
    year_month_day(NA, NA, NA)
  )
})

test_that("can use a different locale", {
  x <- "janvier 05, 2020"
  y <- "2019-01-01 00:00:00,123456"

  expect_identical(
    parse_year_month_day(x, format = "%B %d, %Y", locale = clock_locale("fr")),
    year_month_day(2020, 1, 5)
  )
  expect_identical(
    parse_year_month_day(y, locale = clock_locale(decimal_mark = ","), precision = "microsecond"),
    year_month_day(2019, 1, 1, 0, 0, 0, 123456, subsecond_precision = "microsecond")
  )
})

test_that("parsing NA returns NA", {
  expect_identical(
    parse_year_month_day(NA_character_),
    year_month_day(NA, NA, NA)
  )
  expect_identical(
    parse_year_month_day(NA_character_, precision = "month"),
    year_month_day(NA, NA)
  )
})

# ------------------------------------------------------------------------------
# calendar_narrow()

test_that("can narrow to month", {
  x_expect <- year_month_day(2019, 2)
  x <- set_day(x_expect, 1)
  expect_identical(calendar_narrow(x, "month"), x_expect)
  expect_identical(calendar_narrow(x_expect, "month"), x_expect)
})

test_that("can narrow to day", {
  x_expect <- year_month_day(2019, 2, 3)
  x <- set_hour(x_expect, 5)
  expect_identical(calendar_narrow(x, "day"), x_expect)
  expect_identical(calendar_narrow(x_expect, "day"), x_expect)
})

test_that("can narrow to hour", {
  x_expect <- year_month_day(2019, 2, 3, 4)
  x <- set_minute(x_expect, 5)
  expect_identical(calendar_narrow(x, "hour"), x_expect)
  expect_identical(calendar_narrow(x_expect, "hour"), x_expect)
})

test_that("can narrow to minute", {
  x_expect <- year_month_day(2019, 2, 3, 4, 5)
  x <- set_second(x_expect, 6)
  expect_identical(calendar_narrow(x, "minute"), x_expect)
  expect_identical(calendar_narrow(x_expect, "minute"), x_expect)
})

test_that("can narrow to second", {
  expect <- year_month_day(2019, 2, 3, 4, 5, 6)
  x <- set_millisecond(expect, 7)
  y <- set_nanosecond(expect, 7)
  expect_identical(calendar_narrow(x, "second"), expect)
  expect_identical(calendar_narrow(y, "second"), expect)
  expect_identical(calendar_narrow(expect, "second"), expect)
})

# ------------------------------------------------------------------------------
# calendar_widen()

test_that("can widen to month", {
  x <- year_month_day(2019)
  expect_identical(calendar_widen(x, "month"), set_month(x, 1))
})

test_that("can widen to day", {
  x <- year_month_day(2019)
  y <- year_month_day(2019, 02)
  expect_identical(calendar_widen(x, "day"), set_day(set_month(x, 1), 1))
  expect_identical(calendar_widen(y, "day"), set_day(y, 1))
})

test_that("can widen to hour", {
  x <- year_month_day(2019)
  y <- year_month_day(2019, 02, 02)
  expect_identical(calendar_widen(x, "hour"), set_hour(set_day(set_month(x, 1), 1), 0))
  expect_identical(calendar_widen(y, "hour"), set_hour(y, 0))
})

test_that("can widen to minute", {
  x <- year_month_day(2019)
  y <- year_month_day(2019, 02, 02, 02)
  x_expect <- year_month_day(2019, 1, 1, 0, 0)
  y_expect <- set_minute(y, 0)
  expect_identical(calendar_widen(x, "minute"), x_expect)
  expect_identical(calendar_widen(y, "minute"), y_expect)
})

test_that("can widen to second", {
  x <- year_month_day(2019)
  y <- year_month_day(2019, 02, 02, 02, 02)
  x_expect <- year_month_day(2019, 1, 1, 0, 0, 0)
  y_expect <- set_second(y, 0)
  expect_identical(calendar_widen(x, "second"), x_expect)
  expect_identical(calendar_widen(y, "second"), y_expect)
})

test_that("can widen to subsecond precision", {
  x <- year_month_day(2019)
  y <- year_month_day(2019, 02, 02, 02, 02, 02)
  x_expect <- year_month_day(2019, 1, 1, 0, 0, 0, 0, subsecond_precision = "microsecond")
  y_expect <- set_nanosecond(y, 0)
  expect_identical(calendar_widen(x, "microsecond"), x_expect)
  expect_identical(calendar_widen(y, "nanosecond"), y_expect)
})

# ------------------------------------------------------------------------------
# calendar_month_factor()

test_that("can get a month factor", {
  expect_identical(
    calendar_month_factor(year_month_day(2019, 1:12)),
    factor(month.name, levels = month.name, ordered = TRUE)
  )
})

test_that("can abbreviate month names", {
  expect_identical(
    calendar_month_factor(year_month_day(2019, 1:12), abbreviate = TRUE),
    factor(month.abb, levels = month.abb, ordered = TRUE)
  )
})

test_that("can adjust labels language", {
  labels <- clock_labels_lookup("fr")$month
  expect_identical(
    calendar_month_factor(year_month_day(2019, 1:12), labels = "fr"),
    factor(labels, levels = labels, ordered = TRUE)
  )
})

test_that("requires month precision", {
  expect_snapshot_error(calendar_month_factor(year_month_day(2019)))
})

test_that("`labels` is validated", {
  expect_snapshot_error(calendar_month_factor(year_month_day(2019, 1), labels = 1))
})

test_that("`abbreviate` is validated", {
  expect_snapshot_error(calendar_month_factor(year_month_day(2019, 1), abbreviate = "foo"))
  expect_snapshot_error(calendar_month_factor(year_month_day(2019, 1), abbreviate = 1))
  expect_snapshot_error(calendar_month_factor(year_month_day(2019, 1), abbreviate = c(TRUE, FALSE)))
})

# ------------------------------------------------------------------------------
# invalid_resolve()

test_that("strict mode can be activated", {
  local_options(clock.strict = TRUE)
  expect_snapshot_error(invalid_resolve(year_month_day(2019, 1, 1)))
})

test_that("throws known classed error", {
  expect_snapshot_error(invalid_resolve(year_month_day(2019, 2, 31)))
  expect_error(invalid_resolve(year_month_day(2019, 2, 31)), class = "clock_error_invalid_date")
})
