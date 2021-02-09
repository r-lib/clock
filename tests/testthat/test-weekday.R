# ------------------------------------------------------------------------------
# format()

test_that("format - can format() a weekday", {
  expect_snapshot(format(weekday(0:6)))
})

test_that("format - can use full names", {
  expect_snapshot(format(weekday(0:6), abbreviate = FALSE))
})

test_that("format - can use a different locale", {
  expect_snapshot(format(weekday(0:6), labels = "fr", abbreviate = FALSE))
})

test_that("format - `labels` is validated", {
  expect_snapshot_error(format(weekday(1), labels = 1))
})

test_that("format - `abbreviate` is validated", {
  expect_snapshot_error(format(weekday(1), abbreviate = "foo"))
  expect_snapshot_error(format(weekday(1), abbreviate = 1))
  expect_snapshot_error(format(weekday(1), abbreviate = c(TRUE, FALSE)))
})

# ------------------------------------------------------------------------------
# as.character()

test_that("as.character() works", {
  x <- weekday(0:6)
  expect_identical(as.character(x), format(x))
})

# ------------------------------------------------------------------------------
# weekday_factor()

test_that("can make a weekday factor sunday->saturday", {
  order <- 6:0
  x <- weekday(order)
  x <- weekday_factor(x)

  levels <- clock_labels_lookup("en")$weekday
  data <- levels[order + 1L]

  expect_s3_class(x, "ordered")
  expect_identical(as.character(x), data)
  expect_identical(levels(x), levels)
})

test_that("can make a weekday factor monday->sunday", {
  order <- 6:0
  x <- weekday(order)
  x <- weekday_factor(x, encoding = "iso")

  levels <- clock_labels_lookup("en")$weekday
  data <- levels[order + 1L]
  levels <- levels[c(2:7, 1L)]

  expect_s3_class(x, "ordered")
  expect_identical(as.character(x), data)
  expect_identical(levels(x), levels)
})

test_that("can make a weekday factor with abbreviations", {
  x <- weekday(6:0)
  x <- weekday_factor(x, abbreviate = TRUE)

  levels <- clock_labels_lookup("en")$weekday_abbrev

  expect_identical(levels(x), levels)
})

test_that("can make a weekday factor with alternate labels", {
  x <- weekday(6:0)
  x <- weekday_factor(x, labels = "fr")

  levels <- clock_labels_lookup("fr")$weekday

  expect_identical(levels(x), levels)
})

test_that("`x` is validated", {
  expect_snapshot_error(weekday_factor(1))
})

test_that("`labels` is validated", {
  expect_snapshot_error(weekday_factor(weekday(1), labels = 1))
})

test_that("`encoding` is validated", {
  expect_snapshot_error(weekday_factor(weekday(1), encoding = "foo"))
  expect_snapshot_error(weekday_factor(weekday(1), encoding = 1))
  expect_snapshot_error(weekday_factor(weekday(1), encoding = c("c", "iso")))
})

test_that("`abbreviate` is validated", {
  expect_snapshot_error(weekday_factor(weekday(1), abbreviate = "foo"))
  expect_snapshot_error(weekday_factor(weekday(1), abbreviate = 1))
  expect_snapshot_error(weekday_factor(weekday(1), abbreviate = c(TRUE, FALSE)))
})
