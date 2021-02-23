test_that("can get known database version", {
  expect_identical(zone_database_version(), "2021a")
})

test_that("can get database names with known length", {
  names <- zone_database_names()
  expect_length(names, 593)
})
