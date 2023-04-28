# ------------------------------------------------------------------------------
# format()

test_that("format - can format() a weekday", {
  expect_snapshot(format(weekday(1:7)))
})

test_that("format - can use full names", {
  expect_snapshot(format(weekday(1:7), abbreviate = FALSE))
})

test_that("format - can use a different locale", {
  expect_snapshot(format(weekday(1:7), labels = "fr", abbreviate = FALSE))
})

test_that("format - `labels` is validated", {
  expect_snapshot(error = TRUE, format(weekday(1), labels = 1))
})

test_that("format - `abbreviate` is validated", {
  expect_snapshot(error = TRUE, format(weekday(1), abbreviate = "foo"))
  expect_snapshot(error = TRUE, format(weekday(1), abbreviate = 1))
  expect_snapshot(error = TRUE, format(weekday(1), abbreviate = c(TRUE, FALSE)))
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- weekday(1:7)
  expect_identical(as.character(x), format(x))
})

test_that("as.character() works with NA", {
  expect_identical(as.character(weekday(NA)), NA_character_)
})

# ------------------------------------------------------------------------------
# weekday_code()

test_that("can get the western code", {
  expect_identical(weekday_code(weekday(1:7)), 1:7)
})

test_that("can get the ISO code", {
  expect_identical(weekday_code(weekday(1:7), encoding = "iso"), c(7L, 1:6))
})

test_that("names are not retained", {
  expect_named(weekday_code(c(foo = weekday(1))), NULL)
})

test_that("NA passes through", {
  expect_identical(weekday_code(weekday(NA)), NA_integer_)
})

test_that("validates `x`", {
  expect_snapshot(error = TRUE, weekday_code(1))
})

test_that("weekday_code - `encoding` is validated", {
  expect_snapshot(error = TRUE, weekday_code(weekday(1), encoding = "foo"))
  expect_snapshot(error = TRUE, weekday_code(weekday(1), encoding = 1))
})

# ------------------------------------------------------------------------------
# weekday_factor()

test_that("can make a weekday factor sunday->saturday", {
  order <- 7:1
  x <- weekday(order)
  x <- weekday_factor(x)

  levels <- clock_labels_lookup("en")$weekday_abbrev
  data <- levels[order]

  expect_s3_class(x, "ordered")
  expect_identical(as.character(x), data)
  expect_identical(levels(x), levels)
})

test_that("can make a weekday factor monday->sunday", {
  order <- 7:1
  x <- weekday(order)
  x <- weekday_factor(x, encoding = "iso")

  levels <- clock_labels_lookup("en")$weekday_abbrev
  data <- levels[order]
  levels <- levels[c(2:7, 1L)]

  expect_s3_class(x, "ordered")
  expect_identical(as.character(x), data)
  expect_identical(levels(x), levels)
})

test_that("can make a weekday factor with full names", {
  x <- weekday(7:1)
  x <- weekday_factor(x, abbreviate = FALSE)

  levels <- clock_labels_lookup("en")$weekday

  expect_identical(levels(x), levels)
})

test_that("can make a weekday factor with alternate labels", {
  x <- weekday(7:1)
  x <- weekday_factor(x, labels = "fr")

  levels <- clock_labels_lookup("fr")$weekday_abbrev

  expect_identical(levels(x), levels)
})

test_that("`x` is validated", {
  expect_snapshot(error = TRUE, weekday_factor(1))
})

test_that("`labels` is validated", {
  expect_snapshot(error = TRUE, weekday_factor(weekday(1), labels = 1))
})

test_that("`encoding` is validated", {
  expect_snapshot(error = TRUE, weekday_factor(weekday(1), encoding = "foo"))
  expect_snapshot(error = TRUE, weekday_factor(weekday(1), encoding = 1))
})

test_that("`abbreviate` is validated", {
  expect_snapshot(error = TRUE, weekday_factor(weekday(1), abbreviate = "foo"))
  expect_snapshot(error = TRUE, weekday_factor(weekday(1), abbreviate = 1))
  expect_snapshot(error = TRUE, weekday_factor(weekday(1), abbreviate = c(TRUE, FALSE)))
})

# ------------------------------------------------------------------------------
# add_days()

test_that("can add days with circular arithmetic", {
  expect_identical(add_days(weekday(6:7), 1), weekday(c(7,1)))
})

test_that("can add days with NA", {
  expect_identical(
    add_days(weekday(c(1, 2, NA)), c(NA, 1, 2)),
    weekday(c(NA, 3, NA))
  )
})

# ------------------------------------------------------------------------------
# vec_arith()

test_that("can subtract two weekdays with circular arithmetic", {
  expect_identical(
    weekday(3) - weekday(c(1, 7)),
    duration_days(c(2, 3))
  )
})

test_that("subtraction respects NA", {
  expect_identical(
    weekday(c(1, NA)) - weekday(c(NA, 2)),
    duration_days(c(NA, NA))
  )
})

# ------------------------------------------------------------------------------
# vec_proxy_compare()

test_that("can't compare or order weekdays (#153)", {
  expect_snapshot(error = TRUE, weekday(1) < weekday(2))
  expect_snapshot(error = TRUE, min(weekday(1)))

  expect_snapshot(error = TRUE, xtfrm(weekday(1:2)))
  expect_snapshot(error = TRUE, vec_order(weekday(1:2)))
})

# ------------------------------------------------------------------------------
# vec_ptype()

test_that("ptype is correct", {
  expect_identical(vec_ptype(weekday(1:7)), weekday(integer()))
})

# ------------------------------------------------------------------------------
# vec_math()

test_that("is.nan() works", {
  x <- weekday(c(1, NA))
  expect_identical(is.nan(x), c(FALSE, FALSE))
})

test_that("is.finite() works", {
  x <- weekday(c(1, NA))
  expect_identical(is.finite(x), c(TRUE, FALSE))
})

test_that("is.infinite() works", {
  x <- weekday(c(1, NA))
  expect_identical(is.infinite(x), c(FALSE, FALSE))
})
