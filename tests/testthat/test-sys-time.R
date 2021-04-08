# ------------------------------------------------------------------------------
# sys_time_now()

test_that("returns nanosecond precision", {
  x <- sys_time_now()
  expect_identical(time_point_precision_attribute(x), PRECISION_NANOSECOND)
})

test_that("returns a single sys-time", {
  x <- sys_time_now()
  expect_length(x, 1L)
  expect_s3_class(x, "clock_sys_time")
})

# ------------------------------------------------------------------------------
# sys_time_info()

test_that("can lookup sys-info", {
  # One in EST, one in EDT
  x <- year_month_day(2021, 03, 14, c(01, 03), c(59, 00), c(59, 00))
  x <- as_naive_time(x)
  x <- as_zoned_time(x, "America/New_York")

  info <- sys_time_info(as_sys_time(x), zoned_time_zone(x))

  beginend1 <- as_sys_time(c(
    year_month_day(2020, 11, 1, 6, 0, 0),
    year_month_day(2021, 03, 14, 7, 0, 0)
  ))
  beginend2 <- as_sys_time(c(
    year_month_day(2021, 03, 14, 7, 0, 0),
    year_month_day(2021, 11, 7, 6, 0, 0)
  ))

  expect_identical(info$begin, c(beginend1[1], beginend2[1]))
  expect_identical(info$end, c(beginend1[2], beginend2[2]))

  expect_identical(info$offset, duration_seconds(c(-18000, -14400)))
  expect_identical(info$dst, c(FALSE, TRUE))
  expect_identical(info$abbreviation, c("EST", "EDT"))
})

test_that("`zone` is vectorized and recycled against `x`", {
  zones <- c("America/New_York", "Australia/Lord_Howe")
  x <- as_sys_time(year_month_day(2019, 1, 1))

  info <- sys_time_info(x, zones)

  naive_times <- c(
    as_naive_time(as_zoned_time(x, zones[1])),
    as_naive_time(as_zoned_time(x, zones[2]))
  )

  expect_identical(as_naive_time(x + info$offset), naive_times)

  # DST is dependent on the time zone
  expect_identical(info$dst, c(FALSE, TRUE))

  expect_identical(info$abbreviation, c("EST", "+11"))
})

test_that("very old times are looked up correctly", {
  x <- year_month_day(1800, 01, 01)
  x <- as_sys_time(x)

  info <- sys_time_info(x, "America/New_York")

  end <- as_sys_time(year_month_day(1883, 11, 18, 17, 00, 00))
  offset <- duration_seconds(-17762)
  dst <- FALSE
  abbreviation <- "LMT"

  expect_identical(info$end, end)
  expect_identical(info$offset, offset)
  expect_identical(info$dst, dst)
  expect_identical(info$abbreviation, abbreviation)
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- as_sys_time(year_month_day(2019, 1, 1))
  expect_identical(as.character(x), "2019-01-01")

  x <- as_sys_time(year_month_day(2019, 1, 1, 1, 1))
  expect_identical(as.character(x), "2019-01-01 01:01")
})

# ------------------------------------------------------------------------------
# sys_time_parse()

# Note: Most tests are in `naive_time_parse()`. They share an implementation.

test_that("can parse day precision", {
  x <- c("2019-01-01", "2019-01-31")

  expect_identical(
    sys_time_parse(x, precision = "day"),
    as_sys_time(year_month_day(2019, 1, c(1, 31)))
  )
})

test_that("%z shifts the result by the offset", {
  x <- "2019-01-01 00:00:00+0100"
  y <- "2019-01-01 00:00:00-0100"

  expect_identical(
    sys_time_parse(x, format = "%Y-%m-%d %H:%M:%S%z"),
    as_sys_time(year_month_day(2018, 12, 31, 23, 0, 0))
  )
  expect_identical(
    sys_time_parse(y, format = "%Y-%m-%d %H:%M:%S%z"),
    as_sys_time(year_month_day(2019, 1, 1, 1, 0, 0))
  )
})

test_that("failure to parse throws a warning", {
  expect_warning(sys_time_parse("foo"), class = "clock_warning_parse_failures")
  expect_snapshot(sys_time_parse("foo"))
})

# ------------------------------------------------------------------------------
# format()

test_that("allows `%z` and `%Z`", {
  x <- sys_seconds(0)
  expect_identical(format(x, format = "%z"), "+0000")
  expect_identical(format(x, format = "%Ez"), "+00:00")
  expect_identical(format(x, format = "%Z"), "UTC")
})

# ------------------------------------------------------------------------------
# as_zoned_time()

test_that("empty dots are checked", {
  expect_snapshot_error(as_zoned_time(sys_seconds(), "UTC", 123))
})

# ------------------------------------------------------------------------------
# vec_ptype()

test_that("ptype is correct", {
  base <- sys_days(0)
  ptype <- sys_days(integer())

  for (precision in precision_names()) {
    if (validate_precision_string(precision) < PRECISION_DAY) {
      next
    }

    x <- time_point_cast(base, precision)
    expect <- time_point_cast(ptype, precision)

    expect_identical(vec_ptype(x), expect)
  }
})
