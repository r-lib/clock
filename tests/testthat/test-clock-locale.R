test_that("can create a default locale object", {
  locale <- clock_locale()
  expect_s3_class(locale, "clock_locale")
  expect_snapshot(locale)
})

test_that("can change labels with either the language name or labels object", {
  labels1 <- clock_locale("fr")
  labels2 <- clock_locale(clock_labels_lookup("fr"))
  expect_identical(labels1, labels2)
})

test_that("must use a valid clock-labels object", {
  expect_snapshot_error(clock_locale(1))
})

test_that("must use a valid decimal-mark", {
  expect_snapshot_error(clock_locale(decimal_mark = "f"))
})

test_that("can change the decimal-mark", {
  x <- clock_locale(decimal_mark = ",")
  expect_identical(x$decimal_mark, ",")
  expect_snapshot(x)
})
