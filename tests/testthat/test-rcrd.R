test_that("can create a civil rcrd", {
  x <- new_civil_rcrd(list(x = 1))
  expect_s3_class(x, "civil_rcrd")
})

test_that("can test for civil objects", {
  x <- new_civil_rcrd(list(x = 1))
  expect_true(is_civil_rcrd(x))
})

test_that("can add extra attributes", {
  x <- new_civil_rcrd(list(x = 1), foo = "bar")
  expect_identical(attr(x, "foo"), "bar")
})

test_that("can create, get, and set names", {
  x <- new_civil_rcrd(list(x = 1:2), names = c("a", "b"))

  expect_named(x, c("a", "b"))
  expect_named(`names<-`(x, c("c", "d")), c("c", "d"))
  expect_named(`names<-`(x, NULL), NULL)
})

test_that("Names can be coerced to character when setting", {
  x <- new_civil_rcrd(list(x = 1:2))
  expect_named(`names<-`(x, c(1, 2)), c("1", "2"))
})

test_that("names have all attributes stripped", {
  x <- new_civil_rcrd(list(x = 1:2))
  nms <- structure(c(x1 = "a", x1 = "b"), foo = "bar")
  names(x) <- nms
  expect_identical(names(x), c("a", "b"))
})

test_that("must have at least 1 field", {
  expect_snapshot_error(new_civil_rcrd(list()))
})

test_that("validates names", {
  expect_snapshot_error(new_civil_rcrd(list(x = 1:2), names = "x"))
  expect_snapshot_error(new_civil_rcrd(list(x = 1:2), names = 1))
  expect_snapshot_error(new_civil_rcrd(list(x = 1:2), names = c(NA, "x")))
})

test_that("`[` keeps names", {
  x <- new_civil_rcrd(list(x = 1L), names = "foo")
  expect_named(x[1], "foo")
  expect_named(x["foo"], "foo")
})

test_that("`[` allows missing `i`", {
  x <- new_civil_rcrd(list(x = 1:2))
  expect_identical(x[], x)
})

test_that("`[` doesn't allow 2nd dimension subsetting", {
  x <- new_civil_rcrd(list(x = 1:3, y = 1:3))
  expect_error(x[1, 1])
})

test_that("`[[` drops names", {
  x <- new_civil_rcrd(list(x = 1L), names = "foo")
  expect_named(x[[1]], NULL)
  expect_named(x[["foo"]], NULL)
})

test_that("`[[` only selects 1 element", {
  x <- new_civil_rcrd(list(x = 1:2))
  expect_snapshot_error(x[[c(1, 2)]])
  expect_snapshot_error(x[[-1]])
  expect_snapshot_error(x[["a"]])
})
