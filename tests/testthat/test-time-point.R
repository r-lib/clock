# ------------------------------------------------------------------------------
# format()

test_that("naive-time doesn't allow `%z` or `%Z`", {
  x <- naive_seconds(0)
  expect_identical(format(x, format = "%z"), NA_character_)
  expect_identical(format(x, format = "%Z"), NA_character_)
})

test_that("sys-time allows `%z` and `%Z`", {
  x <- sys_seconds(0)
  expect_identical(format(x, format = "%z"), "+0000")
  expect_identical(format(x, format = "%Ez"), "+00:00")
  expect_identical(format(x, format = "%Z"), "UTC")
})

# ------------------------------------------------------------------------------
# time_point_floor() / _ceiling() / _round()

test_that("can round to less precise precision", {
  x <- naive_seconds(c(-86401, -86400, -86399, 0, 86399, 86400, 86401))

  floor <- naive_days(c(-2, -1, -1, 0, 0, 1, 1))
  ceiling <- naive_days(c(-1, -1, 0, 0, 1, 1, 2))
  round <- naive_days(c(-1, -1, -1, 0, 1, 1, 1))

  expect_identical(time_point_floor(x, "day"), floor)
  expect_identical(time_point_ceiling(x, "day"), ceiling)
  expect_identical(time_point_round(x, "day"), round)

  floor <- naive_days(c(-2, -2, -2, 0, 0, 0, 0))
  ceiling <- naive_days(c(0, 0, 0, 0, 2, 2, 2))
  round <- naive_days(c(-2, 0, 0, 0, 0, 2, 2))

  expect_identical(time_point_floor(x, "day", n = 2), floor)
  expect_identical(time_point_ceiling(x, "day", n = 2), ceiling)
  expect_identical(time_point_round(x, "day", n = 2), round)
})

test_that("can round with `origin` altering starting point", {
  x <- sys_seconds(c(-86401, -86400, -86399, 0, 86399, 86400, 86401))

  origin <- sys_days(-1)

  floor <- sys_days(c(-3, -1, -1, -1, -1, 1, 1))
  ceiling <- sys_days(c(-1, -1, 1, 1, 1, 1, 3))
  round <- sys_days(c(-1, -1, -1, 1, 1, 1, 1))

  expect_identical(time_point_floor(x, "day", origin = origin, n = 2), floor)
  expect_identical(time_point_ceiling(x, "day", origin = origin, n = 2), ceiling)
  expect_identical(time_point_round(x, "day", origin = origin, n = 2), round)
})

test_that("cannot floor to more precise precision", {
  expect_snapshot_error(time_point_floor(naive_days(), "second"))
})

test_that("rounding with `origin` requires same clock", {
  origin <- sys_days(0)
  x <- naive_days(0)
  expect_snapshot_error(time_point_floor(x, "day", origin = origin))
})

test_that("`origin` can be cast to a more precise `precision`, but not to a less precise one", {
  origin1 <- as_naive(duration_days(1))
  origin2 <- as_naive(duration_milliseconds(0))
  x <- naive_seconds(0)

  expect_identical(
    time_point_floor(x, "hour", origin = origin1, n = 5),
    time_point_floor(x - as_duration(origin1), "hour", n = 5) + as_duration(origin1)
  )

  expect_snapshot_error(time_point_floor(x, "hour", origin = origin2))
})

test_that("`origin` must be size 1", {
  origin <- naive_days(0:1)
  x <- naive_days(0)
  expect_snapshot_error(time_point_floor(x, "day", origin = origin))
})

test_that("`origin` must not be `NA`", {
  origin <- naive_days(NA)
  x <- naive_days(0)
  expect_snapshot_error(time_point_floor(x, "day", origin = origin))
})

test_that("`origin` can't be Date or POSIXt", {
  origin1 <- new_date(0)
  origin2 <- new_datetime(0, "America/New_York")
  x <- naive_days(0)
  expect_snapshot_error(time_point_floor(x, "day", origin = origin1))
  expect_snapshot_error(time_point_floor(x, "day", origin = origin2))
})

