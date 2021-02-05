# ------------------------------------------------------------------------------

test_that("can parse what we format with seconds precision zoned time", {
  zone <- "America/New_York"

  x <- as_zoned_time(as_naive_time(year_month_day(2019, 1, 1)), zone)

  expect_identical(
    parse_zoned_time(format(x)),
    x
  )
})

test_that("can parse subsecond zoned time", {
  zone <- "America/New_York"

  x <- "2019-01-01 01:02:03.123-05:00[America/New_York]"
  y <- "2019-01-01 01:02:03.1234-05:00[America/New_York]"
  z <- "2019-01-01 01:02:03.123456789-05:00[America/New_York]"

  expect_identical(
    parse_zoned_time(x, precision = "millisecond"),
    as_zoned_time(as_naive_time(year_month_day(2019, 1, 1, 1, 2, 3, 123, subsecond_precision = "millisecond")), zone)
  )
  expect_identical(
    parse_zoned_time(y, precision = "microsecond"),
    as_zoned_time(as_naive_time(year_month_day(2019, 1, 1, 1, 2, 3, 123400, subsecond_precision = "microsecond")), zone)
  )
  expect_identical(
    parse_zoned_time(z, precision = "nanosecond"),
    as_zoned_time(as_naive_time(year_month_day(2019, 1, 1, 1, 2, 3, 123456789, subsecond_precision = "nanosecond")), zone)
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
    parse_zoned_time(x, format = formats),
    as_zoned_time(
      as_naive_time(year_month_day(1970, 10, 25, 05, 30, c(00, 00))),
      zone
    )
  )
})

test_that("cannot parse nonexistent time", {
  zone <- "America/New_York"

  x <- "1970-04-26 02:30:00-05:00[America/New_York]"

  expect_identical(
    parse_zoned_time(x),
    as_zoned_time(naive_seconds(NA), zone)
  )
})

test_that("ambiguous times are resolved by the offset", {
  zone <- "America/New_York"

  x <- c(
    "1970-10-25 01:30:00-04:00[America/New_York]",
    "1970-10-25 01:30:00-05:00[America/New_York]"
  )

  expect_identical(
    parse_zoned_time(x),
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
  x <- "2019-01-01 01:02:03-03:00[America/New_York]"

  expect_identical(
    parse_zoned_time(x),
    as_zoned_time(naive_seconds(NA), zone)
  )
})

test_that("offset must align with one of two possible ambiguous offsets", {
  zone <- "America/New_York"

  # Should be `-04:00` or `-05:00`
  x <- "1970-10-25 01:30:00-03:00[America/New_York]"

  expect_identical(
    parse_zoned_time(x),
    as_zoned_time(naive_seconds(NA), zone)
  )
})

test_that("cannot have differing zone names", {
  x <- c(
    "2019-01-01 01:02:03-05:00[America/New_York]",
    "2019-01-01 01:02:03-08:00[America/Los_Angeles]"
  )

  expect_snapshot_error(parse_zoned_time(x))
})

test_that("zone name must be valid", {
  x <- "2019-01-01 01:02:03-05:00[America/New_Yor]"

  expect_snapshot_error(parse_zoned_time(x))
})
