# ------------------------------------------------------------------------------
# naive_time_info()

test_that("naive-time-info returns the right structure", {
  info <- naive_time_info(naive_days(0), "America/New_York")

  expect_type(info$type, "character")
  expect_s3_class(info$first, "data.frame")
  expect_s3_class(info$second, "data.frame")
})

test_that("unique info works", {
  info <- naive_time_info(naive_days(0), "America/New_York")
  na_sys_info <- sys_time_info(sys_days(NA), "America/New_York")

  expect_identical(info$type, "unique")
  expect_identical(info$second, na_sys_info)

  begin <- as_sys_time(year_month_day(1969, 10, 26, 06, 00, 00))
  end <- as_sys_time(year_month_day(1970, 4, 26, 7, 0, 0))
  offset <- duration_seconds(-18000)

  expect_identical(info$first$begin, begin)
  expect_identical(info$first$end, end)
  expect_identical(info$first$offset, offset)
  expect_identical(info$first$dst, FALSE)
  expect_identical(info$first$abbreviation, "EST")
})

test_that("nonexistent info works", {
  x <- as_naive_time(year_month_day(1970, 4, 26, 2, 30, 00))
  info <- naive_time_info(x, "America/New_York")

  expect_identical(info$type, "nonexistent")

  begin <- as_sys_time(year_month_day(1969, 10, 26, 06, 00, 00))
  end <- as_sys_time(year_month_day(1970, 4, 26, 7, 0, 0))
  offset <- duration_seconds(-18000)

  expect_identical(info$first$begin, begin)
  expect_identical(info$first$end, end)
  expect_identical(info$first$offset, offset)
  expect_identical(info$first$dst, FALSE)
  expect_identical(info$first$abbreviation, "EST")

  begin <- as_sys_time(year_month_day(1970, 4, 26, 7, 0, 0))
  end <- as_sys_time(year_month_day(1970, 10, 25, 6, 0, 0))
  offset <- duration_seconds(-14400)

  expect_identical(info$second$begin, begin)
  expect_identical(info$second$end, end)
  expect_identical(info$second$offset, offset)
  expect_identical(info$second$dst, TRUE)
  expect_identical(info$second$abbreviation, "EDT")
})

test_that("ambiguous info works", {
  x <- as_naive_time(year_month_day(1970, 10, 25, 1, 30, 00))
  info <- naive_time_info(x, "America/New_York")

  expect_identical(info$type, "ambiguous")

  begin <- as_sys_time(year_month_day(1970, 4, 26, 7, 0, 0))
  end <- as_sys_time(year_month_day(1970, 10, 25, 6, 0, 0))
  offset <- duration_seconds(-14400)

  expect_identical(info$first$begin, begin)
  expect_identical(info$first$end, end)
  expect_identical(info$first$offset, offset)
  expect_identical(info$first$dst, TRUE)
  expect_identical(info$first$abbreviation, "EDT")

  begin <- as_sys_time(year_month_day(1970, 10, 25, 6, 0, 0))
  end <- as_sys_time(year_month_day(1971, 4, 25, 7, 0, 0))
  offset <- duration_seconds(-18000)

  expect_identical(info$second$begin, begin)
  expect_identical(info$second$end, end)
  expect_identical(info$second$offset, offset)
  expect_identical(info$second$dst, FALSE)
  expect_identical(info$second$abbreviation, "EST")
})

test_that("all rows are NA when x is NA", {
  info <- naive_time_info(naive_days(NA), "UTC")
  na_sys_info <- sys_time_info(sys_days(NA), "UTC")
  df <- data_frame(type = NA_character_, first = na_sys_info, second = na_sys_info)
  expect_identical(info, df)
})

test_that("x must be naive", {
  expect_snapshot_error(naive_time_info(sys_days(0), "UTC"))
})

test_that("zone is vectorized and recycled against x", {
  info <- naive_time_info(naive_days(0), c("UTC", "America/New_York"))
  expect_identical(nrow(info), 2L)

  expect_snapshot_error(naive_time_info(naive_days(0:3), c("UTC", "America/New_York")))
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- as_naive_time(year_month_day(2019, 1, 1))
  expect_identical(as.character(x), "2019-01-01")

  x <- as_naive_time(year_month_day(2019, 1, 1, 1, 1))
  expect_identical(as.character(x), "2019-01-01 01:01")
})

# ------------------------------------------------------------------------------
# naive_time_parse()

test_that("can parse day precision", {
  x <- c("2019-01-01", "2019-01-31")

  expect_identical(
    naive_time_parse(x, precision = "day"),
    as_naive_time(year_month_day(2019, 1, c(1, 31)))
  )
})

test_that("can parse second precision", {
  x <- c("2019-01-01 00:00:05", "2019-01-31 00:00:10")

  expect_identical(
    naive_time_parse(x, precision = "second"),
    as_naive_time(year_month_day(2019, 1, c(1, 31), 00, 00, c(05, 10)))
  )
})

test_that("can parse subsecond precision", {
  x <- c("2019-01-01 00:00:05.123", "2019-01-31 00:00:10.124")
  y <- c("2019-01-01 00:00:05.12345", "2019-01-31 00:00:10.124567")
  z <- c("2019-01-01 00:00:05.12345678", "2019-01-31 00:00:10.124567899")

  sec <- year_month_day(2019, 1, c(1, 31), 00, 00, c(05, 10))

  expect_identical(
    naive_time_parse(x, precision = "millisecond"),
    as_naive_time(set_millisecond(sec, c(123, 124)))
  )
  expect_identical(
    naive_time_parse(y, precision = "microsecond"),
    as_naive_time(set_microsecond(sec, c(123450, 124567)))
  )
  expect_identical(
    naive_time_parse(z, precision = "nanosecond"),
    as_naive_time(set_nanosecond(sec, c(123456780, 124567899)))
  )
})

test_that("parsing to a lower precision ignores higher precision info", {
  x <- "2019-01-01 01:00:00"
  y <- "2019-01-01 01:00:00.12345"

  expect_identical(
    naive_time_parse(x, precision = "day"),
    as_naive_time(year_month_day(2019, 1, 1))
  )
  expect_identical(
    naive_time_parse(y, precision = "second"),
    as_naive_time(year_month_day(2019, 1, 1, 1, 0, 0))
  )
  expect_identical(
    naive_time_parse(y, precision = "millisecond"),
    as_naive_time(year_month_day(2019, 1, 1, 1, 0, 0, 123, subsecond_precision = "millisecond"))
  )
})

test_that("parsing day components with second precision uses midnight as time", {
  x <- "2019/1/1"

  expect_identical(
    naive_time_parse(x, format = "%Y/%m/%d", precision = "second"),
    as_naive_time(year_month_day(2019, 1, 1, 0, 0, 0))
  )
})

test_that("cannot parse invalid dates", {
  x <- "2019-02-31"

  expect_identical(
    expect_warning(naive_time_parse(x, precision = "day")),
    naive_days(NA)
  )

  expect_snapshot(naive_time_parse(x, precision = "day"))
})

test_that("can parse with multiple formats", {
  x <- c("2019-01-01", "2020/1/2", "January 05, 2019")
  formats <- c("%Y-%m-%d", "%Y/%m/%d", "%B %d, %Y")

  expect_identical(
    naive_time_parse(x, format = formats, precision = "day"),
    as_naive_time(year_month_day(c(2019, 2020, 2019), 1, c(1, 2, 5)))
  )
})

test_that("failure to parse results in NA", {
  x <- "2019-01-oh"

  expect_identical(
    expect_warning(naive_time_parse(x, format = "%Y-%m-%d", precision = "day")),
    naive_days(NA)
  )
})

test_that("failure to parse throws a warning", {
  expect_warning(naive_time_parse("foo"), class = "clock_warning_parse_failures")
  expect_snapshot(naive_time_parse("foo"))
})

test_that("names of input are kept", {
  x <- c(foo = "2019-01-01")
  expect_named(naive_time_parse(x, precision = "day"), "foo")
})

test_that("can use a different locale", {
  x <- "janvier 01, 2019"
  y <- "2019-01-01 00:00:00,123456"

  expect_identical(
    naive_time_parse(x, format = "%B %d, %Y", precision = "day", locale = clock_locale("fr")),
    as_naive_time(year_month_day(2019, 1, 1))
  )
  expect_identical(
    naive_time_parse(y, precision = "microsecond", locale = clock_locale(decimal_mark = ",")),
    as_naive_time(year_month_day(2019, 1, 1, 0, 0, 0, 123456, subsecond_precision = "microsecond"))
  )
})

test_that("`x` is translated to UTF-8", {
  x <- "f\u00E9vrier 05, 2019"
  x <- iconv(x, from = "UTF-8", to = "latin1")

  locale <- clock_locale("fr")
  format <- "%B %d, %Y"

  expect_identical(Encoding(x[1]), "latin1")
  expect_identical(Encoding(locale$labels$month[2]), "UTF-8")

  expect_identical(
    naive_time_parse(x, format = format, precision = "day", locale = locale),
    as_naive_time(year_month_day(2019, 2, 5))
  )
})

test_that("%z is completely ignored, but is required to be parsed correctly if specified", {
  x <- "2019-01-01 00:00:00+0100"
  y <- "2019-01-01 00:00:00"

  expect_identical(
    naive_time_parse(x, format = "%Y-%m-%d %H:%M:%S%z"),
    as_naive_time(year_month_day(2019, 1, 1, 0, 0, 0))
  )
  expect_identical(
    expect_warning(naive_time_parse(y, format = "%Y-%m-%d %H:%M:%S%z")),
    naive_seconds(NA)
  )
})

test_that("%Z is completely ignored", {
  x <- "2019-01-01 00:00:00 America/New_York"

  expect_identical(
    naive_time_parse(x, format = "%Y-%m-%d %H:%M:%S %Z"),
    as_naive_time(year_month_day(2019, 1, 1, 0, 0, 0))
  )
})

# ------------------------------------------------------------------------------
# as_zoned_time()

test_that("can convert non-ambiguous/nonexistent times to zoned time", {
  zone <- "America/New_York"
  x <- as_naive_time(year_month_day(2019, 1, 1))
  expect <- as_sys_time(year_month_day(2019, 1, 1, 5))
  expect_identical(as_zoned_time(x, zone), as_zoned_time(expect, zone))
})

test_that("sub daily time point precision is retained", {
  zone <- "America/New_York"
  x <- as_naive_time(year_month_day(2019, 1, 1, 1, 1, 1, 1, subsecond_precision = "nanosecond"))
  expect_identical(zoned_time_precision_attribute(as_zoned_time(x, zone)), PRECISION_NANOSECOND)
})

test_that("day precision time point is promoted", {
  zone <- "America/New_York"
  x <- as_naive_time(year_month_day(2019, 1, 1))
  expect_identical(zoned_time_precision_attribute(as_zoned_time(x, zone)), PRECISION_SECOND)
})

test_that("can resolve ambiguous issues - character", {
  zone <- "America/New_York"
  x <- as_naive_time(year_month_day(1970, 10, 25, 01, 30, 00, 01, subsecond_precision = "millisecond"))
  earliest <- as_sys_time(year_month_day(1970, 10, 25, 05, 30, 00, 01, subsecond_precision = "millisecond"))
  latest <- as_sys_time(year_month_day(1970, 10, 25, 06, 30, 00, 01, subsecond_precision = "millisecond"))

  expect_snapshot_error(as_zoned_time(x, zone))
  expect_error(as_zoned_time(x, zone), class = "clock_error_ambiguous_time")

  expect_identical(
    as_zoned_time(x, zone, ambiguous = "earliest"),
    as_zoned_time(earliest, zone)
  )
  expect_identical(
    as_zoned_time(x, zone, ambiguous = "latest"),
    as_zoned_time(latest, zone)
  )
  expect_identical(
    as_zoned_time(x, zone, ambiguous = "NA"),
    as_zoned_time(as_naive_time(duration_milliseconds(NA)), zone)
  )
})

test_that("can resolve ambiguous issues - zoned-time", {
  zone <- "America/New_York"
  nt <- as_naive_time(year_month_day(1970, 10, 25, 01, 30, c(00, 00)))
  zt <- as_zoned_time(nt, zone, ambiguous = c("earliest", "latest"))

  expect_identical(
    as_zoned_time(nt, zone, ambiguous = zt),
    zt
  )

  nt <- as_naive_time(year_month_day(1970, 10, 25, c(01, 02), 30, 00))
  zt <- as_zoned_time(nt, zone, ambiguous = "earliest")

  expect_identical(
    as_zoned_time(nt, zone, ambiguous = zt),
    zt
  )

  # Issue at location 2 because zt[2] isn't ambiguous so we can't use offset
  # information from it
  nt_tweaked <- as_naive_time(set_hour(as_year_month_day(nt), 1))
  expect_snapshot_error(as_zoned_time(nt_tweaked, zone, ambiguous = zt))

  # Jump from one ambiguous time to another. Still can't use offset info,
  # because the ambiguous time transitions are different.
  ymd <- year_month_day(1969, 10, 26, 01, 30, 00)
  nt <- as_naive_time(ymd)
  zt <- as_zoned_time(nt, zone, ambiguous = "earliest")
  nt_tweaked <- nt + duration_days(364)
  expect_snapshot_error(as_zoned_time(nt_tweaked, zone, ambiguous = zt))
})

test_that("can resolve ambiguous issues - list", {
  zone <- "America/New_York"
  nt <- as_naive_time(year_month_day(1970, 10, 25, c(01, 01, 02), 30, 00))
  zt <- as_zoned_time(nt, zone, ambiguous = c("earliest", "latest", "error"))
  nt_tweaked <- as_naive_time(set_hour(as_year_month_day(nt), 1))

  # First two are resolved from consulting `zt`, otherwise resolved with "latest"
  expect_identical(
    as_zoned_time(nt_tweaked, zone, ambiguous = list(zt, "latest")),
    as_zoned_time(nt_tweaked, zone, ambiguous = c("earliest", "latest", "latest"))
  )
})

test_that("zoned-time ambiguous argument is recycled", {
  zone <- "America/New_York"
  nt <- as_naive_time(year_month_day(1970, 10, 25, 01, 30, 00))
  zt <- as_zoned_time(nt, zone, ambiguous = "earliest")
  nt_tweaked <- nt + duration_seconds(c(0, 1))

  expect_identical(
    as_zoned_time(nt_tweaked, zone, ambiguous = zt),
    as_zoned_time(nt_tweaked, zone, ambiguous = "earliest")
  )
})

test_that("can resolve nonexistent issues", {
  zone <- "America/New_York"
  x <- as_naive_time(year_month_day(1970, 04, 26, 02, 30, 00))

  expect_snapshot_error(as_zoned_time(x, zone))
  expect_error(as_zoned_time(x, zone), class = "clock_error_nonexistent_time")

  expect_identical(
    as_zoned_time(x, zone, nonexistent = "roll-forward"),
    as_zoned_time(add_minutes(x, 30), zone)
  )
  expect_identical(
    as_zoned_time(x, zone, nonexistent = "roll-backward"),
    as_zoned_time(add_seconds(add_minutes(x, -30), -1), zone)
  )
  expect_identical(
    as_zoned_time(x, zone, nonexistent = "shift-forward"),
    as_zoned_time(add_minutes(x, 60), zone)
  )
  expect_identical(
    as_zoned_time(x, zone, nonexistent = "shift-backward"),
    as_zoned_time(add_minutes(x, -60), zone)
  )
  expect_identical(
    as_zoned_time(x, zone, nonexistent = "NA"),
    as_zoned_time(naive_seconds(NA), zone)
  )
})

test_that("nonexistent can be vectorized", {
  zone <- "America/New_York"
  x <- as_naive_time(year_month_day(1970, 04, 26, 02, 00, c(00, 00)))

  expect_identical(
    as_zoned_time(x, zone, nonexistent = c("roll-forward", "roll-backward")),
    as_zoned_time(x + duration_seconds(c(3600, -1)), zone)
  )
})

test_that("roll-backward uses precision to determine last moment in time", {
  zone <- "America/New_York"
  w <- as_naive_time(year_month_day(1970, 04, 26, 02, 00, 00))
  x <- w + duration_milliseconds(0)
  y <- w + duration_microseconds(0)
  z <- w + duration_nanoseconds(0)

  expect_identical(
    as_zoned_time(w, zone, nonexistent = "roll-backward"),
    as_zoned_time(add_seconds(w, -1), zone)
  )
  expect_identical(
    as_zoned_time(x, zone, nonexistent = "roll-backward"),
    as_zoned_time(add_milliseconds(x, -1), zone)
  )
  expect_identical(
    as_zoned_time(y, zone, nonexistent = "roll-backward"),
    as_zoned_time(add_microseconds(y, -1), zone)
  )
  expect_identical(
    as_zoned_time(z, zone, nonexistent = "roll-backward"),
    as_zoned_time(add_nanoseconds(z, -1), zone)
  )
})

test_that("names are retained", {
  zone <- "America/New_York"
  x <- as_naive_time(year_month_day(2019, 1, 1))
  x <- c(foo = x)
  expect_named(as_zoned_time(x, zone), "foo")
})

test_that("NA pass through", {
  x <- as_zoned_time(naive_seconds(NA), "America/New_York")
  expect_true(is.na(x))
})

test_that("`ambiguous` is validated", {
  zone <- "America/New_York"
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, ambiguous = 1))
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, ambiguous = "foo"))
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, ambiguous = c("earliest", "latest")))

  ambiguous <- as_zoned_time(naive_seconds(), "America/Los_Angeles")
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, ambiguous = ambiguous))

  reference <- as_zoned_time(naive_seconds(), zone)
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, ambiguous = list()))
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, ambiguous = list(1, 1)))
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, ambiguous = list(reference, 1)))
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, ambiguous = list(reference, "foo")))
})

test_that("`nonexistent` is validated", {
  zone <- "America/New_York"
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, nonexistent = 1))
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, nonexistent = "foo"))
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, nonexistent = c("roll-forward", "roll-forward")))
})

test_that("zone is validated", {
  expect_snapshot_error(as_zoned_time(naive_seconds(), "foo"))
  expect_snapshot_error(as_zoned_time(naive_seconds(), 1))
  expect_snapshot_error(as_zoned_time(naive_seconds(), c("America/New_York", "EST", "EDT")))
})

test_that("strict mode can be activated - nonexistent", {
  local_options(clock.strict = TRUE)
  zone <- "America/New_York"
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, ambiguous = "earliest"))
})

test_that("strict mode can be activated - ambiguous", {
  zone <- "America/New_York"
  zt <- as_zoned_time(naive_seconds(), zone)

  local_options(clock.strict = TRUE)

  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, nonexistent = "roll-forward"))
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, nonexistent = "roll-forward", ambiguous = zt))
  expect_snapshot_error(as_zoned_time(naive_seconds(), zone, nonexistent = "roll-forward", ambiguous = list(zt, NULL)))
})

# ------------------------------------------------------------------------------
# vec_ptype()

test_that("ptype is correct", {
  base <- naive_days(0)
  ptype <- naive_days(integer())

  for (precision in precision_names()) {
    if (validate_precision_string(precision) < PRECISION_DAY) {
      next
    }

    x <- time_point_cast(base, precision)
    expect <- time_point_cast(ptype, precision)

    expect_identical(vec_ptype(x), expect)
  }
})
