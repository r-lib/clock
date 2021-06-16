test_that("POSIXt time zones are standardized as expected", {
  expect_identical(posixt_tzone_standardize(NULL), "")
  expect_identical(posixt_tzone_standardize(""), "")
  expect_identical(posixt_tzone_standardize("America/New_York"), "America/New_York")

  # POSIXlt can return something like this
  expect_identical(posixt_tzone_standardize(c("", "EST", "EDT")), "")

  # Unknown if this can happen, but we handle it anyways
  expect_snapshot(posixt_tzone_standardize(character()))
  expect_warning(
    expect_identical(posixt_tzone_standardize(character()), "")
  )

  expect_snapshot_error(posixt_tzone_standardize(1))
})
