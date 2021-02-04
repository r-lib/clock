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
  expect_identical(zoned_time_precision(as_zoned_time(x, zone)), PRECISION_NANOSECOND)
})

test_that("day precision time point is promoted", {
  zone <- "America/New_York"
  x <- as_naive_time(year_month_day(2019, 1, 1))
  expect_identical(zoned_time_precision(as_zoned_time(x, zone)), PRECISION_SECOND)
})

test_that("can resolve ambiguous issues - character", {
  zone <- "America/New_York"
  x <- as_naive_time(year_month_day(1970, 10, 25, 01, 30, 00, 01, subsecond_precision = "millisecond"))
  earliest <- as_sys_time(year_month_day(1970, 10, 25, 05, 30, 00, 01, subsecond_precision = "millisecond"))
  latest <- as_sys_time(year_month_day(1970, 10, 25, 06, 30, 00, 01, subsecond_precision = "millisecond"))

  expect_snapshot_error(as_zoned_time(x, zone))

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
