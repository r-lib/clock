# ------------------------------------------------------------------------------
# clock_labels()

test_that("can make custom labels", {
  months <- month.name
  weekdays <- c("Su", "Mo", "Tu", "We", "Th", "Fr", "Sa")
  am_pm <- c("A", "P")

  labels <- clock_labels(month = month.name, weekday = weekdays, am_pm = am_pm)

  expect_s3_class(labels, "clock_labels")
  expect_snapshot(labels)
})

test_that("input is validated", {
  months <- month.name
  weekdays <- c("Su", "Mo", "Tu", "We", "Th", "Fr", "Sa")
  am_pm <- c("A", "P")

  expect_snapshot_error(clock_labels(1))
  expect_snapshot_error(clock_labels("x"))
  expect_snapshot_error(clock_labels(months, 1))
  expect_snapshot_error(clock_labels(months, "x"))
  expect_snapshot_error(clock_labels(months, months, 1))
  expect_snapshot_error(clock_labels(months, months, "x"))
  expect_snapshot_error(clock_labels(months, months, weekdays, 1))
  expect_snapshot_error(clock_labels(months, months, weekdays, "x"))
  expect_snapshot_error(clock_labels(months, months, weekdays, weekdays, 1))
  expect_snapshot_error(clock_labels(months, months, weekdays, weekdays, "x"))
})

# ------------------------------------------------------------------------------
# clock_labels_lookup()

test_that("can lookup a language", {
  labels <- clock_labels_lookup("fr")

  expect_s3_class(labels, "clock_labels")
  expect_snapshot(labels)
})

test_that("must be a valid language code", {
  expect_snapshot_error(clock_labels_lookup(1))
  expect_snapshot_error(clock_labels_lookup("foo"))
})

# ------------------------------------------------------------------------------
# clock_labels_languages()

test_that("can list all the languages", {
  langs <- clock_labels_languages()

  expect_length(langs, 196)
  expect_snapshot(langs)
})
