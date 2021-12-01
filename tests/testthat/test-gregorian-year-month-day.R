# ------------------------------------------------------------------------------
# year_month_day()

test_that("helper can create different precisions", {
  x <- year_month_day(2019, 1:2)
  expect_identical(get_year(x), c(2019L, 2019L))
  expect_identical(get_month(x), 1:2)

  x <- year_month_day(2019, 1:2, 3)
  expect_identical(get_day(x), c(3L, 3L))
})

test_that("can create subsecond precision calendars", {
  x <- year_month_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "millisecond")
  expect_identical(get_millisecond(x), 1L)

  x <- year_month_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "microsecond")
  expect_identical(get_microsecond(x), 1L)

  x <- year_month_day(2019, 1, 1, 0, 0, 0, 1, subsecond_precision = "nanosecond")
  expect_identical(get_nanosecond(x), 1L)
})

test_that("validates value ranges", {
  expect_snapshot_error(year_month_day(50000))
  expect_snapshot_error(year_month_day(2020, 13))
  expect_snapshot_error(year_month_day(2020, 1, 32))
  expect_snapshot_error(year_month_day(2020, 1, 1, 24))
  expect_snapshot_error(year_month_day(2020, 1, 1, 1, 60))
  expect_snapshot_error(year_month_day(2020, 1, 1, 1, 1, 60))
  expect_snapshot_error(year_month_day(2020, 1, 1, 1, 1, 1, 1000, subsecond_precision = "millisecond"))
  expect_snapshot_error(year_month_day(2020, 1, 1, 1, 1, 1, 1000000, subsecond_precision = "microsecond"))
  expect_snapshot_error(year_month_day(2020, 1, 1, 1, 1, 1, 1000000000, subsecond_precision = "nanosecond"))
})

test_that("can create a date at the boundary", {
  x <- year_month_day(32767, 12, 31)
  expect_identical(get_year(x), 32767L)

  x <- year_month_day(-32767, 1, 1)
  expect_identical(get_year(x), -32767L)
})

test_that("can get the last day of the month", {
  x <- year_month_day(2019, 1:2, "last")
  expect_identical(get_day(x), c(31L, 28L))
})

test_that("ignores values past first `NULL`", {
  expect_identical(year_month_day(2019, day = 2), year_month_day(2019))
})

test_that("NA values propagate", {
  x <- year_month_day(2019, 1:3, c(NA, 2, 3), c(3, 4, NA))
  expect_identical(is.na(x), c(TRUE, FALSE, TRUE))
})

test_that("names of `year` are not retained", {
  expect_named(year_month_day(c(x = 1)), NULL)
})

# ------------------------------------------------------------------------------
# vec_ptype()

test_that("ptype is correct", {
  base <- year_month_day(1)
  ptype <- year_month_day(integer())

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
  expect_identical(vec_proxy(year_month_day(2019)), data_frame(year = 2019L))
  expect_identical(vec_proxy(year_month_day(2019, 1)), data_frame(year = 2019L, month = 1L))
})

test_that("proxy has names on `year`", {
  x <- set_names(year_month_day(2019, 1, 1), "nm")
  year <- vec_proxy(x)$year
  expect_named(year, "nm")
})

test_that("restore works", {
  to <- year_month_day(2019, 1:5)
  proxy <- vec_slice(vec_proxy(to), 1:2)
  expect_identical(vec_restore(proxy, to), year_month_day(2019, 1:2))
})

# ------------------------------------------------------------------------------
# vec_ptype_full()

test_that("full ptype is correct", {
  expect_snapshot_output(vec_ptype_full(year_month_day(2019)))
  expect_snapshot_output(vec_ptype_full(year_month_day(2019, 1, 1)))
  expect_snapshot_output(vec_ptype_full(year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")))
  expect_snapshot_output(vec_ptype_full(year_month_day(2019, 2, 31)))
})

# ------------------------------------------------------------------------------
# vec_ptype_abbr()

test_that("abbreviated ptype is correct", {
  expect_snapshot_output(vec_ptype_abbr(year_month_day(2019)))
  expect_snapshot_output(vec_ptype_abbr(year_month_day(2019, 1, 1)))
  expect_snapshot_output(vec_ptype_abbr(year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond")))
  expect_snapshot_output(vec_ptype_abbr(year_month_day(2019, 2, 31)))
})

# ------------------------------------------------------------------------------
# as_year_quarter_day()

test_that("invalid dates must be resolved when converting to another calendar", {
  expect_snapshot_error(as_year_quarter_day(year_month_day(2019, 2, 31)))
})

# ------------------------------------------------------------------------------
# as_sys_time()

test_that("invalid dates must be resolved when converting to a sys-time", {
  expect_snapshot_error(as_sys_time(year_month_day(2019, 2, 31)))
})

# ------------------------------------------------------------------------------
# as_naive_time()

test_that("invalid dates must be resolved when converting to a naive-time", {
  expect_snapshot_error(as_naive_time(year_month_day(2019, 2, 31)))
})

# ------------------------------------------------------------------------------
# format()

test_that("default formats are correct", {
  expect_snapshot(format(year_month_day(2019)))
  expect_snapshot(format(year_month_day(2019, 1)))
  expect_snapshot(format(year_month_day(2019, 1, 1, 1)))
  expect_snapshot(format(year_month_day(2019, 1, 1, 1, 2, 3, 50, subsecond_precision = "microsecond")))
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- year_month_day(2019, 1)
  y <- year_month_day(2019, 1, 2)

  expect_identical(as.character(x), format(x))
  expect_identical(as.character(y), format(y))
})

# ------------------------------------------------------------------------------
# year_month_day_parse()

test_that("default parses at day precision with ISO format", {
  x <- "2019-01-01"

  expect_identical(
    year_month_day_parse(x),
    year_month_day(2019, 1, 1)
  )
})

test_that("can parse years or year-months", {
  x <- "2019"
  y <- "2019-01"

  expect_identical(
    year_month_day_parse(x, precision = "year"),
    year_month_day(2019)
  )
  expect_identical(
    year_month_day_parse(y, precision = "month"),
    year_month_day(2019, 01)
  )
})

test_that("can parse second and subsecond precision", {
  x <- "2019-01-01T05:10:20"
  y <- "2019-01-01T05:10:20.1234"

  expect_identical(
    year_month_day_parse(x, precision = "second"),
    year_month_day(2019, 1, 1, 5, 10, 20)
  )
  expect_identical(
    year_month_day_parse(y, precision = "millisecond"),
    year_month_day(2019, 1, 1, 5, 10, 20, 123, subsecond_precision = "millisecond")
  )
  expect_identical(
    year_month_day_parse(y, precision = "microsecond"),
    year_month_day(2019, 1, 1, 5, 10, 20, 123400, subsecond_precision = "microsecond")
  )
})

test_that("can parse invalid dates", {
  x <- "2019-2-31"

  expect_identical(
    year_month_day_parse(x),
    year_month_day(2019, 2, 31)
  )
})

test_that("names are retained from the input", {
  x <- c(foo = "2019-01-01")

  expect_named(year_month_day_parse(x), "foo")
})

test_that("multiple formats can be provided", {
  x <- c("2019-01", "2020 Jan")
  formats <- c("%Y %B", "%Y-%m")

  expect_identical(
    year_month_day_parse(x, format = formats, precision = "month"),
    year_month_day(c(2019, 2020), 1)
  )
})

test_that("failure to parse results in `NA`", {
  x <- "2020-01-ohno"

  expect_warning(
    expect_identical(
      year_month_day_parse(x),
      year_month_day(NA, NA, NA)
    )
  )
})

test_that("failure to parse results in a warning", {
  expect_warning(year_month_day_parse("foo", precision = "year"), class = "clock_warning_parse_failures")
  expect_warning(year_month_day_parse("foo", precision = "month"), class = "clock_warning_parse_failures")
  expect_warning(year_month_day_parse("foo", precision = "day"), class = "clock_warning_parse_failures")
  expect_warning(year_month_day_parse("foo", precision = "hour"), class = "clock_warning_parse_failures")
  expect_warning(year_month_day_parse("foo", precision = "minute"), class = "clock_warning_parse_failures")
  expect_warning(year_month_day_parse("foo", precision = "second"), class = "clock_warning_parse_failures")
  expect_warning(year_month_day_parse("foo", precision = "millisecond"), class = "clock_warning_parse_failures")
  expect_warning(year_month_day_parse("foo", precision = "microsecond"), class = "clock_warning_parse_failures")
  expect_warning(year_month_day_parse("foo", precision = "nanosecond"), class = "clock_warning_parse_failures")

  expect_snapshot(year_month_day_parse("foo"))
})

test_that("can use a different locale", {
  x <- "janvier 05, 2020"
  y <- "2019-01-01T00:00:00,123456"

  expect_identical(
    year_month_day_parse(x, format = "%B %d, %Y", locale = clock_locale("fr")),
    year_month_day(2020, 1, 5)
  )
  expect_identical(
    year_month_day_parse(y, locale = clock_locale(decimal_mark = ","), precision = "microsecond"),
    year_month_day(2019, 1, 1, 0, 0, 0, 123456, subsecond_precision = "microsecond")
  )
})

test_that("can use a different locale with UTF-8 strings", {
  x <- c("1月 01 2019", "3月 05 2020")
  y <- "ለካቲት 01 2019"

  expect_identical(
    year_month_day_parse(x, format = "%B %d %Y", locale = clock_locale("ja")),
    year_month_day(c(2019, 2020), c(1, 3), c(1, 5))
  )
  expect_identical(
    year_month_day_parse(y, format = "%B %d %Y", locale = clock_locale("ti")),
    year_month_day(2019, 2, 1)
  )
})

test_that("`format` argument is translated to UTF-8", {
  x <- "f\u00E9v 2019-05-19"

  format <- "f\u00E9v %Y-%m-%d"
  format <- iconv(format, from = "UTF-8", to = "latin1")

  expect_identical(Encoding(x), "UTF-8")
  expect_identical(Encoding(format), "latin1")

  expect_identical(
    year_month_day_parse(x, format = format),
    year_month_day(2019, 5, 19)
  )
})

test_that("`x` is translated to UTF-8", {
  x <- "2019-f\u00E9vrier-01"
  x <- iconv(x, from = "UTF-8", to = "latin1")

  locale <- clock_locale("fr")
  format <- "%Y-%B-%d"

  expect_identical(Encoding(x), "latin1")
  expect_identical(Encoding(locale$labels$month[2]), "UTF-8")

  expect_identical(
    year_month_day_parse(x, format = format, locale = locale),
    year_month_day(2019, 2, 1)
  )
})

test_that("parsing NA returns NA", {
  expect_identical(
    year_month_day_parse(NA_character_),
    year_month_day(NA, NA, NA)
  )
  expect_identical(
    year_month_day_parse(NA_character_, precision = "month"),
    year_month_day(NA, NA)
  )
})

test_that("parsing doesn't round parsed components more precise than the resulting container (#207)", {
  # With year-month-day, only the year/month/day components are extracted at the end,
  # the hour component isn't touched
  expect_identical(
    year_month_day_parse("2019-12-31 12", format = "%Y-%m-%d %H", precision = "day"),
    year_month_day(2019, 12, 31)
  )
})

test_that("parsing rounds parsed subsecond components more precise than the resulting container (#207)", {
  # Requesting `%7S` parses the full `01.1238`, and the `1238` portion is rounded up immediately
  # after parsing the `%S` command, not at the very end
  expect_identical(
    year_month_day_parse("2019-01-01 01:01:01.1238", format = "%Y-%m-%d %H:%M:%7S", precision = "millisecond"),
    year_month_day(2019, 1, 1, 1, 1, 1, 124, subsecond_precision = "millisecond")
  )
})

# ------------------------------------------------------------------------------
# calendar_group()

test_that("works with negative years", {
  year <- c(-2, -1, 0, 1, 2)
  x <- year_month_day(year, 1, 1)

  expect_identical(calendar_group(x, "year"), year_month_day(year))
  expect_identical(calendar_group(x, "year", n = 2), year_month_day(c(-2, -2, 0, 0, 2)))
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
# calendar_start()

test_that("can compute year start", {
  x <- year_month_day(2019)
  expect_identical(calendar_start(x, "year"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_month_day(2019, 1, 1, 0, 0, 0, 0, subsecond_precision = "millisecond")
  expect_identical(calendar_start(x, "year"), expect)
})

test_that("can compute month start", {
  x <- year_month_day(2019, 2)
  expect_identical(calendar_start(x, "month"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "microsecond")
  expect <- year_month_day(2019, 2, 1, 0, 0, 0, 0, subsecond_precision = "microsecond")
  expect_identical(calendar_start(x, "month"), expect)
})

# ------------------------------------------------------------------------------
# calendar_end()

test_that("can compute year end", {
  x <- year_month_day(2019)
  expect_identical(calendar_end(x, "year"), x)

  x <- year_month_day(2019, 2, 2, 2, 2, 2, 2, subsecond_precision = "millisecond")
  expect <- year_month_day(2019, 12, 31, 23, 59, 59, 999L, subsecond_precision = "millisecond")
  expect_identical(calendar_end(x, "year"), expect)
})

test_that("can compute month end", {
  x <- year_month_day(2019, 2)
  expect_identical(calendar_end(x, "month"), x)

  x <- year_month_day(2019, 2:3, 2, 2, 2, 2, 2, subsecond_precision = "microsecond")
  expect <- year_month_day(2019, 2:3, c(28, 31), 23, 59, 59, 999999L, subsecond_precision = "microsecond")
  expect_identical(calendar_end(x, "month"), expect)
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
# calendar_count_between()

test_that("can compute year and month counts", {
  x <- year_month_day(2019, 1, 1)
  y <- year_month_day(2020, 3, 4)

  expect_identical(calendar_count_between(x, y, "year"), 1L)
  expect_identical(calendar_count_between(x, y, "month"), 14L)
  expect_identical(calendar_count_between(x, y, "month", n = 2), 7L)
})

test_that("can compute a quarter count", {
  x <- year_month_day(2019, 1, 2)

  y <- year_month_day(2019, 4, c(1, 3))
  expect_identical(calendar_count_between(x, y, "quarter"), c(0L, 1L))
  expect_identical(
    calendar_count_between(x, y, "quarter"),
    calendar_count_between(x, y, "month", n = 3L)
  )

  y <- year_month_day(2020, 4, c(1, 3))
  expect_identical(calendar_count_between(x, y, "quarter", n = 2L), c(2L, 2L))
  expect_identical(
    calendar_count_between(x, y, "quarter", n = 2L),
    calendar_count_between(x, y, "month", n = 6L)
  )
})

test_that("can't compute a unsupported count precision", {
  x <- year_month_day(2019, 1, 1)
  expect_snapshot((expect_error(calendar_count_between(x, x, "day"))))
})

test_that("positive / negative counts are correct", {
  start <- year_month_day(1972, 03, 04)


  end <- year_month_day(1973, 03, 03)
  expect_identical(calendar_count_between(start, end, "year"), 0L)
  expect_identical(calendar_count_between(start, end, "month"), 11L)

  end <- year_month_day(1973, 03, 04)
  expect_identical(calendar_count_between(start, end, "year"), 1L)
  expect_identical(calendar_count_between(start, end, "month"), 12L)

  end <- year_month_day(1973, 03, 05)
  expect_identical(calendar_count_between(start, end, "year"), 1L)
  expect_identical(calendar_count_between(start, end, "month"), 12L)


  end <- year_month_day(1971, 03, 03)
  expect_identical(calendar_count_between(start, end, "year"), -1L)
  expect_identical(calendar_count_between(start, end, "month"), -12L)

  end <- year_month_day(1971, 03, 04)
  expect_identical(calendar_count_between(start, end, "year"), -1L)
  expect_identical(calendar_count_between(start, end, "month"), -12L)

  end <- year_month_day(1971, 03, 05)
  expect_identical(calendar_count_between(start, end, "year"), 0L)
  expect_identical(calendar_count_between(start, end, "month"), -11L)
})

# ------------------------------------------------------------------------------
# seq()

test_that("only granular precisions are allowed", {
  expect_snapshot_error(seq(year_month_day(2019, 1, 1), by = 1, length.out = 2))
})

test_that("seq(to, by) works", {
  expect_identical(seq(year_month_day(2019, 1), to = year_month_day(2019, 6), by = 2), year_month_day(2019, c(1, 3, 5)))
  expect_identical(seq(year_month_day(2019, 1), to = year_month_day(2019, 5), by = 2), year_month_day(2019, c(1, 3, 5)))

  expect_identical(seq(year_month_day(2019, 1), to = year_month_day(2018, 9), by = -2), year_month_day(c(2019, 2018, 2018), c(1, 11, 9)))
  expect_identical(seq(year_month_day(2019, 1), to = year_month_day(2018, 8), by = -2), year_month_day(c(2019, 2018, 2018), c(1, 11, 9)))
})

test_that("seq(to, length.out) works", {
  expect_identical(seq(year_month_day(2019, 1), to = year_month_day(2019, 5), length.out = 2), year_month_day(2019, c(1, 5)))
  expect_identical(seq(year_month_day(2019, 1), to = year_month_day(2019, 5), length.out = 1), year_month_day(2019, 1))
  expect_identical(seq(year_month_day(2019, 1), to = year_month_day(2019, 5), length.out = 5), year_month_day(2019, 1:5))

  expect_identical(seq(year_month_day(2019, 1), to = year_month_day(2019, 5), along.with = 1:2), year_month_day(2019, c(1, 5)))
})

test_that("seq(by, length.out) works", {
  expect_identical(seq(year_month_day(2019, 1), by = 2, length.out = 3), year_month_day(2019, c(1, 3, 5)))
  expect_identical(seq(year_month_day(2019, 1), by = -2, length.out = 3), year_month_day(c(2019, 2018, 2018), c(1, 11, 9)))

  expect_identical(seq(year_month_day(2019, 1), by = 2, along.with = 1:3), year_month_day(2019, c(1, 3, 5)))
})

test_that("`by` can be a duration", {
  expect_identical(
    seq(year_month_day(2019, 1), to = year_month_day(2025, 5), by = duration_years(1)),
    seq(year_month_day(2019, 1), to = year_month_day(2025, 5), by = 12)
  )
  expect_identical(
    seq(year_month_day(2019, 3), by = duration_years(1), length.out = 5),
    seq(year_month_day(2019, 3), by = 12, length.out = 5)
  )
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

# ------------------------------------------------------------------------------
# vec_math()

test_that("is.nan() works", {
  x <- year_month_day(c(2019, NA))
  expect_identical(is.nan(x), c(FALSE, FALSE))
})

test_that("is.finite() works", {
  x <- year_month_day(c(2019, NA))
  expect_identical(is.finite(x), c(TRUE, FALSE))
})

test_that("is.infinite() works", {
  x <- year_month_day(c(2019, NA))
  expect_identical(is.infinite(x), c(FALSE, FALSE))
})
