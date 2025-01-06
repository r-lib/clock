# ------------------------------------------------------------------------------
# vec_proxy_compare()

# Really testing `vec_proxy()` and `vec_ptype2()` / `vec_cast()`

test_that("can't compare zoned-times with different zones", {
  x <- as_zoned_time(sys_days(0), "America/New_York")
  y <- as_zoned_time(sys_days(0), "UTC")

  expect_snapshot(error = TRUE, x > y)
})

test_that("can compare zoned-times with same zone", {
  x <- as_zoned_time(sys_days(0:1), "America/New_York")

  expect_false(x[1] > x[2])
  expect_true(x[1] < x[2])
})

# ------------------------------------------------------------------------------
# print() / obj_print_data() / obj_print_footer()

test_that("normal print method works", {
  x <- as_zoned_time(as_sys_time(year_month_day(2019, 1:5, 1)), "America/New_York")
  expect_snapshot(x)
})

test_that("can limit with `max`", {
  x <- as_zoned_time(as_sys_time(year_month_day(2019, 1:5, 1)), "America/New_York")

  expect_snapshot(print(x, max = 2))
  expect_snapshot(print(x, max = 4))

  # no footer if length >= max
  expect_snapshot(print(x, max = 5))
  expect_snapshot(print(x, max = 6))
})

test_that("`max` defaults to `getOption('max.print')` but can be overridden", {
  local_options(max.print = 3)

  x <- as_zoned_time(as_sys_time(year_month_day(2019, 1:5, 1)), "America/New_York")

  expect_snapshot(x)
  expect_snapshot(print(x, max = 4))
  expect_snapshot(print(x, max = 5))
})

# ------------------------------------------------------------------------------
# as_zoned_time()

test_that("`as_zoned_time()` has good default error", {
  expect_snapshot(error = TRUE, {
    as_zoned_time(1)
  })
})

# ------------------------------------------------------------------------------
# as_year_month_day()

test_that("can't convert zoned-time directly to calendar", {
  x <- as_zoned_time(as_naive_time(year_month_day(2019, 1, 1)), "America/New_York")

  expect_snapshot({
    (expect_error(as_year_month_day(x), class = "clock_error_unsupported_conversion"))
  })
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  expect <- "2019-01-01T01:02:03-05:00[America/New_York]"
  x <- zoned_time_parse_complete(expect)
  expect_identical(as.character(x), expect)
})

# ------------------------------------------------------------------------------
# zoned_time_parse_complete()

test_that("can parse what we format with seconds precision zoned time", {
  zone <- "America/New_York"

  x <- as_zoned_time(as_naive_time(year_month_day(2019, 1, 1)), zone)

  expect_identical(
    zoned_time_parse_complete(format(x)),
    x
  )
})

test_that("can parse subsecond zoned time", {
  zone <- "America/New_York"

  x <- "2019-01-01T01:02:03.123-05:00[America/New_York]"
  y <- "2019-01-01T01:02:03.1234-05:00[America/New_York]"
  z <- "2019-01-01T01:02:03.123456789-05:00[America/New_York]"

  expect_identical(
    zoned_time_parse_complete(x, precision = "millisecond"),
    as_zoned_time(
      as_naive_time(
        year_month_day(2019, 1, 1, 1, 2, 3, 123, subsecond_precision = "millisecond")
      ),
      zone
    )
  )
  expect_identical(
    zoned_time_parse_complete(y, precision = "microsecond"),
    as_zoned_time(
      as_naive_time(
        year_month_day(2019, 1, 1, 1, 2, 3, 123400, subsecond_precision = "microsecond")
      ),
      zone
    )
  )
  expect_identical(
    zoned_time_parse_complete(z, precision = "nanosecond"),
    as_zoned_time(
      as_naive_time(
        year_month_day(
          2019,
          1,
          1,
          1,
          2,
          3,
          123456789,
          subsecond_precision = "nanosecond"
        )
      ),
      zone
    )
  )
})

test_that("multiple formats can be used", {
  zone <- "America/New_York"

  x <- c(
    "1970-10-25 05:30:00-05:00[America/New_York]",
    "1970/10/25 05:30:00-05:00[America/New_York]"
  )

  formats <- c(
    "%Y-%m-%d %H:%M:%S%Ez[%Z]",
    "%Y/%m/%d %H:%M:%S%Ez[%Z]"
  )

  expect_identical(
    zoned_time_parse_complete(x, format = formats),
    as_zoned_time(
      as_naive_time(year_month_day(1970, 10, 25, 05, 30, c(00, 00))),
      zone
    )
  )
})

test_that("cannot parse nonexistent time", {
  zone <- "America/New_York"

  x <- "1970-04-26T02:30:00-05:00[America/New_York]"

  expect_warning(
    expect_identical(
      zoned_time_parse_complete(x),
      as_zoned_time(naive_seconds(NA), zone)
    )
  )

  expect_snapshot(zoned_time_parse_complete(x))
})

test_that("ambiguous times are resolved by the offset", {
  zone <- "America/New_York"

  x <- c(
    "1970-10-25T01:30:00-04:00[America/New_York]",
    "1970-10-25T01:30:00-05:00[America/New_York]"
  )

  expect_identical(
    zoned_time_parse_complete(x),
    as_zoned_time(
      as_naive_time(year_month_day(1970, 10, 25, 01, 30, c(00, 00))),
      zone,
      ambiguous = c("earliest", "latest")
    )
  )
})

test_that("offset must align with unique offset", {
  zone <- "America/New_York"

  # Should be `-05:00`
  x <- "2019-01-01T01:02:03-03:00[America/New_York]"

  expect_warning(
    expect_identical(
      zoned_time_parse_complete(x),
      as_zoned_time(naive_seconds(NA), zone)
    )
  )

  expect_snapshot(zoned_time_parse_complete(x))
})

test_that("offset must align with one of two possible ambiguous offsets", {
  zone <- "America/New_York"

  # Should be `-04:00` or `-05:00`
  x <- c(
    "1970-10-25T01:30:00-03:00[America/New_York]",
    "1970-10-25T01:30:00-06:00[America/New_York]"
  )

  expect_warning(
    expect_identical(
      zoned_time_parse_complete(x),
      as_zoned_time(naive_seconds(c(NA, NA)), zone)
    )
  )

  expect_snapshot(zoned_time_parse_complete(x))
})

test_that("cannot have differing zone names", {
  x <- c(
    "2019-01-01T01:02:03-05:00[America/New_York]",
    "2019-01-01T01:02:03-08:00[America/Los_Angeles]"
  )

  expect_snapshot(error = TRUE, zoned_time_parse_complete(x))
})

test_that("zone name must be valid", {
  x <- "2019-01-01T01:02:03-05:00[America/New_Yor]"

  expect_snapshot(error = TRUE, zoned_time_parse_complete(x))
})

test_that("empty input uses UTC time zone (#162)", {
  expect_identical(
    zoned_time_parse_complete(character()),
    as_zoned_time(naive_seconds(), "UTC")
  )
  expect_identical(
    zoned_time_parse_complete(character(), precision = "nanosecond"),
    as_zoned_time(as_naive_time(duration_nanoseconds()), "UTC")
  )
})

test_that("all `NA`s uses UTC time zone (#162)", {
  expect_identical(
    zoned_time_parse_complete(c(NA_character_, NA_character_)),
    as_zoned_time(naive_seconds(c(NA, NA)), "UTC")
  )
})

test_that("all failures uses UTC time zone (#162)", {
  expect_warning(
    expect_identical(
      zoned_time_parse_complete(c("foo", "bar")),
      as_zoned_time(naive_seconds(c(NA, NA)), "UTC")
    )
  )
})

test_that("`x` is translated to UTF-8", {
  x <- "2019-f\u00E9vrier-01 01:02:03-05:00[America/New_York]"
  x <- iconv(x, from = "UTF-8", to = "latin1")

  locale <- clock_locale("fr")
  format <- "%Y-%B-%d %H:%M:%S%Ez[%Z]"

  expect_identical(Encoding(x), "latin1")
  expect_identical(Encoding(locale$labels$month[2]), "UTF-8")

  expect_identical(
    zoned_time_parse_complete(x, format = format, locale = locale),
    as_zoned_time(
      as_naive_time(year_month_day(2019, 2, 1, 1, 2, 3)),
      "America/New_York"
    )
  )
})

test_that("leftover subseconds result in a parse failure", {
  x <- "2019-01-01T01:01:01.1238-05:00[America/New_York]"

  # This is fine
  expect_identical(
    zoned_time_parse_complete(x, precision = "microsecond"),
    as_zoned_time(
      as_naive_time(
        year_month_day(2019, 1, 1, 1, 1, 1, 123800, subsecond_precision = "microsecond")
      ),
      "America/New_York"
    )
  )

  # This defaults to `%6S`, which parses `01.123` then stops,
  # leaving a `8` for `%z` to parse, resulting in a failure. Because everything
  # fails, we get a UTC time zone.
  expect_warning(
    expect_identical(
      zoned_time_parse_complete(x, precision = "millisecond"),
      as_zoned_time(naive_seconds(NA) + duration_milliseconds(NA), zone = "UTC")
    ),
    class = "clock_warning_parse_failures"
  )
})

test_that("parsing rounds parsed subsecond components more precise than the resulting container (#207) (#230) (undocumented)", {
  x <- "2019-01-01 01:01:01.1238-05:00[America/New_York]"

  # Requesting `%7S` parses the full `01.1238`, and the `1238` portion is rounded up
  expect_identical(
    zoned_time_parse_complete(
      x,
      precision = "millisecond",
      format = "%Y-%m-%d %H:%M:%7S%Ez[%Z]"
    ),
    as_zoned_time(
      as_naive_time(
        year_month_day(2019, 1, 1, 1, 1, 1, 124, subsecond_precision = "millisecond")
      ),
      "America/New_York"
    )
  )
})

test_that("parsing fails when undocumented rounding behavior would result in invalid 60 second component (#230) (undocumented)", {
  x <- "2019-01-01 01:01:59.550-05:00[America/New_York]"

  # Requesting `%6S` parses the full `59.550`, which is immediately rounded to `60` which looks invalid.
  # The correct way to do this is to parse the milliseconds, then round.
  expect_warning(
    expect_identical(
      zoned_time_parse_complete(
        x,
        precision = "second",
        format = "%Y-%m-%d %H:%M:%6S%Ez[%Z]"
      ),
      as_zoned_time(as_naive_time(year_month_day(NA, NA, NA, NA, NA, NA)), zone = "UTC")
    ),
    class = "clock_warning_parse_failures"
  )
})

# ------------------------------------------------------------------------------
# zoned_time_parse_abbrev()

test_that("can parse with abbreviation and zone name", {
  expect_identical(
    zoned_time_parse_abbrev("2019-01-01 01:02:03 EST", "America/New_York"),
    zoned_time_parse_complete("2019-01-01T01:02:03-05:00[America/New_York]")
  )
})

test_that("can parse when abbreviation is an offset", {
  expect_identical(
    zoned_time_parse_abbrev("2019-01-01 01:02:03 +11", "Australia/Lord_Howe"),
    zoned_time_parse_complete("2019-01-01T01:02:03+11:00[Australia/Lord_Howe]")
  )
  expect_identical(
    zoned_time_parse_abbrev("2019-10-01 01:02:03 +1030", "Australia/Lord_Howe"),
    zoned_time_parse_complete("2019-10-01T01:02:03+10:30[Australia/Lord_Howe]")
  )
})

test_that("can parse at more precise precisions", {
  expect_identical(
    zoned_time_parse_abbrev(
      "2019-01-01 01:02:03.123 EST",
      "America/New_York",
      precision = "millisecond"
    ),
    as_zoned_time(
      as_naive_time(
        year_month_day(2019, 1, 1, 1, 2, 3, 123, subsecond_precision = "millisecond")
      ),
      "America/New_York"
    )
  )
  expect_identical(
    zoned_time_parse_abbrev(
      "2019-01-01 01:02:03.123456 EST",
      "America/New_York",
      precision = "nanosecond"
    ),
    as_zoned_time(
      as_naive_time(
        year_month_day(
          2019,
          1,
          1,
          1,
          2,
          3,
          123456000,
          subsecond_precision = "nanosecond"
        )
      ),
      "America/New_York"
    )
  )
})

test_that("abbreviation is used to resolve ambiguity", {
  x <- c(
    "1970-10-25 01:30:00 EDT",
    "1970-10-25 01:30:00 EST"
  )

  expect <- c(
    "1970-10-25T01:30:00-04:00[America/New_York]",
    "1970-10-25T01:30:00-05:00[America/New_York]"
  )

  expect_identical(
    zoned_time_parse_abbrev(x, "America/New_York"),
    zoned_time_parse_complete(expect)
  )
})

test_that("nonexistent times are NAs", {
  expect_warning(
    expect_identical(
      zoned_time_parse_abbrev("1970-04-26 02:30:00 EST", "America/New_York"),
      as_zoned_time(sys_seconds(NA), "America/New_York")
    )
  )
})

test_that("abbreviation must match the one implied from naive + time zone name lookup", {
  x <- "1970-01-01 00:00:00 FOOBAR"

  expect_warning(
    expect_identical(
      zoned_time_parse_abbrev(x, "America/New_York"),
      as_zoned_time(sys_days(NA), "America/New_York")
    )
  )

  # Should be EST
  x <- "1970-01-01 00:00:00 EDT"

  expect_warning(
    expect_identical(
      zoned_time_parse_abbrev(x, "America/New_York"),
      as_zoned_time(sys_days(NA), "America/New_York")
    )
  )

  expect_snapshot(zoned_time_parse_abbrev(x, "America/New_York"))
})

test_that("%Z must be used", {
  x <- "1970-01-01"

  expect_snapshot(
    error = TRUE,
    zoned_time_parse_abbrev(x, "America/New_York", format = "%Y-%m-%d")
  )
})

test_that("%z can be parsed (but is ignored really)", {
  expect <- zoned_time_parse_complete("1970-01-01T00:00:00-05:00[America/New_York]")
  x <- "1970-01-01 00:00:00-05:00 EST"

  expect_identical(
    zoned_time_parse_abbrev(x, "America/New_York", format = "%Y-%m-%d %H:%M:%S%Ez %Z"),
    expect
  )
})

test_that("%z that is incorrect technically slips through unnoticed", {
  expect <- zoned_time_parse_complete("1970-01-01T00:00:00-05:00[America/New_York]")
  x <- "1970-01-01 00:00:00-02:00 EST"

  expect_identical(
    zoned_time_parse_abbrev(x, "America/New_York", format = "%Y-%m-%d %H:%M:%S%Ez %Z"),
    expect
  )
})

test_that("%z must parse correctly if included", {
  expect <- as_zoned_time(sys_days(NA), "America/New_York")
  x <- "1970-01-01 00:00:00-0a:00 EST"

  expect_warning(
    result <- zoned_time_parse_abbrev(
      x,
      "America/New_York",
      format = "%Y-%m-%d %H:%M:%S%Ez %Z"
    )
  )

  expect_identical(result, expect)
})

test_that("multiple formats can be attempted", {
  x <- c("1970-01-01 EST", "1970-01-01 05:06:07 EST")
  formats <- c("%Y-%m-%d %H:%M:%S %Z", "%Y-%m-%d %Z")

  expect <- as_zoned_time(
    as_naive_time(year_month_day(1970, 1, 1, c(0, 5), c(0, 6), c(0, 7))),
    "America/New_York"
  )

  expect_identical(
    zoned_time_parse_abbrev(x, "America/New_York", format = formats),
    expect
  )
})

test_that("NA parses correctly", {
  expect_identical(
    zoned_time_parse_abbrev(NA_character_, "America/New_York"),
    as_zoned_time(sys_seconds(NA), "America/New_York")
  )
  expect_identical(
    zoned_time_parse_abbrev(
      NA_character_,
      "America/New_York",
      precision = "nanosecond"
    ),
    as_zoned_time(as_sys_time(duration_nanoseconds(NA)), "America/New_York")
  )
})

test_that("`x` is translated to UTF-8", {
  x <- "2019-f\u00E9vrier-01 01:02:03-05:00[EST]"
  x <- iconv(x, from = "UTF-8", to = "latin1")

  locale <- clock_locale("fr")
  format <- "%Y-%B-%d %H:%M:%S%Ez[%Z]"

  expect_identical(Encoding(x), "latin1")
  expect_identical(Encoding(locale$labels$month[2]), "UTF-8")

  expect_identical(
    zoned_time_parse_abbrev(x, "America/New_York", format = format, locale = locale),
    as_zoned_time(
      as_naive_time(year_month_day(2019, 2, 1, 1, 2, 3)),
      "America/New_York"
    )
  )
})

# ------------------------------------------------------------------------------
# zoned_time_info()

test_that("can get info of a zoned time (#295)", {
  zone <- "America/New_York"

  x <- as_zoned_time(as_naive_time(year_month_day(2019, 1, 1)), zone)
  x <- zoned_time_info(x)

  begin <- as_zoned_time(
    as_naive_time(year_month_day(2018, 11, 4, 1)),
    zone,
    ambiguous = "latest"
  )
  end <- as_zoned_time(as_naive_time(year_month_day(2019, 03, 10, 3)), zone)

  expect_identical(x$begin, begin)
  expect_identical(x$end, end)
  expect_identical(x$offset, duration_seconds(-18000))
  expect_identical(x$dst, FALSE)
  expect_identical(x$abbreviation, "EST")
})

test_that("`NA` propagates", {
  x <- as_zoned_time(as_naive_time(year_month_day(NA, NA, NA)), "UTC")
  info <- zoned_time_info(x)

  expect_identical(info$begin, x)
  expect_identical(info$end, x)
  expect_identical(info$offset, duration_seconds(NA))
  expect_identical(info$dst, NA)
  expect_identical(info$abbreviation, NA_character_)
})

test_that("boundaries are handled right", {
  x <- as_zoned_time(as_naive_time(year_month_day(2019, 1, 1)), "UTC")
  x <- zoned_time_info(x)

  # Only snapshotting in case boundaries are different on CRAN
  expect_snapshot(x$begin)
  expect_snapshot(x$end)

  expect_identical(x$offset, duration_seconds(0))
  expect_identical(x$dst, FALSE)
  expect_identical(x$abbreviation, "UTC")
})

test_that("input must be a zoned-time", {
  expect_snapshot(error = TRUE, {
    zoned_time_info(1)
  })
})

# ------------------------------------------------------------------------------
# add_*()

test_that("zoned-times don't support arithmetic", {
  x <- as_zoned_time(as_naive_time(year_month_day(2019, 1, 1)), "America/New_York")

  expect_snapshot(error = TRUE, add_years(x, 1))
  expect_snapshot(error = TRUE, add_quarters(x, 1))
  expect_snapshot(error = TRUE, add_months(x, 1))
  expect_snapshot(error = TRUE, add_weeks(x, 1))
  expect_snapshot(error = TRUE, add_days(x, 1))
  expect_snapshot(error = TRUE, add_hours(x, 1))
  expect_snapshot(error = TRUE, add_minutes(x, 1))
  expect_snapshot(error = TRUE, add_seconds(x, 1))
  expect_snapshot(error = TRUE, add_milliseconds(x, 1))
  expect_snapshot(error = TRUE, add_microseconds(x, 1))
  expect_snapshot(error = TRUE, add_nanoseconds(x, 1))
})

# ------------------------------------------------------------------------------
# vec_ptype()

test_that("ptype is correct", {
  zones <- c("UTC", "America/New_York", "")

  for (zone in zones) {
    for (precision in precision_names()) {
      precision <- precision_to_integer(precision)

      if (precision < PRECISION_SECOND) {
        next
      }

      x <- duration_helper(0L, precision)
      x <- as_zoned_time(as_naive_time(x), zone)

      ptype <- duration_helper(integer(), precision)
      ptype <- as_zoned_time(as_naive_time(ptype), zone)

      expect_identical(vec_ptype(x), ptype)
    }
  }
})

# ------------------------------------------------------------------------------
# vec_math()

test_that("is.nan() works", {
  zone <- "America/New_York"
  x <- as_zoned_time(naive_days(c(2019, NA)), zone)
  expect_identical(is.nan(x), c(FALSE, FALSE))
})

test_that("is.finite() works", {
  zone <- "America/New_York"
  x <- as_zoned_time(naive_days(c(2019, NA)), zone)
  expect_identical(is.finite(x), c(TRUE, FALSE))
})

test_that("is.infinite() works", {
  zone <- "America/New_York"
  x <- as_zoned_time(naive_days(c(2019, NA)), zone)
  expect_identical(is.infinite(x), c(FALSE, FALSE))
})

# ------------------------------------------------------------------------------
# zoned_time_zone()

test_that("can get the time zone of a zoned-time", {
  zone <- "America/New_York"
  x <- as_zoned_time(naive_days(0), zone)
  expect_identical(zoned_time_zone(x), zone)
})

test_that("`zoned_time_zone()` validates `x`", {
  expect_snapshot(error = TRUE, {
    zoned_time_zone(1)
  })
})

# ------------------------------------------------------------------------------
# zoned_time_set_zone()

test_that("can set the time zone of a zoned-time", {
  zone1 <- "America/New_York"
  zone2 <- "America/Los_Angeles"

  sys <- sys_days(0)
  x <- as_zoned_time(sys, zone1)
  y <- as_zoned_time(sys, zone2)

  expect_identical(zoned_time_set_zone(x, zone2), y)
})

test_that("`zoned_time_set_zone()` validates `x`", {
  expect_snapshot(error = TRUE, {
    zoned_time_set_zone(1, "UTC")
  })
})

# ------------------------------------------------------------------------------
# zoned_time_precision()

test_that("precision: can get the precision", {
  zone <- "America/New_York"
  expect_identical(
    zoned_time_precision(as_zoned_time(as_naive_time(duration_seconds(2:5)), zone)),
    "second"
  )
  expect_identical(
    zoned_time_precision(as_zoned_time(as_naive_time(duration_nanoseconds(2:5)), zone)),
    "nanosecond"
  )
})

test_that("precision: can only be called on zoned-times", {
  expect_snapshot(error = TRUE, zoned_time_precision(duration_days()))
})
