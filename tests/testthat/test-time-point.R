test_that("can create a time point", {
  x <- new_time_point()
  expect_s3_class(x, "clock_time_point")
})

test_that("can test for time point objects", {
  x <- new_time_point()
  expect_true(is_time_point(x))
})

test_that("can add extra attributes", {
  x <- new_time_point(foo = "bar")
  expect_identical(attr(x, "foo"), "bar")
})

test_that("can create, get, and set names", {
  x <- new_time_point(year_month_day(2019:2020), c(0L, 0L), names = c("a", "b"))

  expect_named(x, c("a", "b"))
  expect_named(`names<-`(x, c("c", "d")), c("c", "d"))
  expect_named(`names<-`(x, NULL), NULL)
})

test_that("Names can be coerced to character when setting", {
  x <- new_time_point(year_month_day(2019:2020), c(0L, 0L))
  expect_named(`names<-`(x, c(1, 2)), c("1", "2"))
})

test_that("names have all attributes stripped", {
  x <- new_time_point(year_month_day(2019:2020), c(0L, 0L))
  nms <- structure(c(x1 = "a", x1 = "b"), foo = "bar")
  names(x) <- nms
  expect_identical(names(x), c("a", "b"))
})

test_that("validates names", {
  expect_snapshot_error(new_time_point(year_month_day(2019:2020), names = "x"))
  expect_snapshot_error(new_time_point(year_month_day(2019:2020), names = 1))
  expect_snapshot_error(new_time_point(year_month_day(2019:2020), names = c(NA, "x")))
})

test_that("`[` keeps names", {
  x <- new_time_point(year_month_day(2019), 0L, names = "foo")
  expect_named(x[1], "foo")
  expect_named(x["foo"], "foo")
})

test_that("`[` allows missing `i`", {
  x <- new_time_point(year_month_day(2019:2020), 0:1)
  expect_identical(x[], x)
})

test_that("`[` doesn't allow 2nd dimension subsetting", {
  x <- new_time_point(year_month_day(2019:2020), 0:1)
  expect_error(x[1, 1])
})

test_that("`[[` drops names", {
  x <- new_time_point(year_month_day(2019), 0L, names = "foo")
  expect_named(x[[1]], NULL)
  expect_named(x[["foo"]], NULL)
})

test_that("`[[` only selects 1 element", {
  x <- new_time_point(year_month_day(2019:2020), 0:1)
  expect_snapshot_error(x[[c(1, 2)]])
  expect_snapshot_error(x[[-1]])
  expect_snapshot_error(x[["a"]])
})
