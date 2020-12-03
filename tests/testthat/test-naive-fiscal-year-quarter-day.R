test_that("catch nonexistent days with varying fiscal starts", {
  # particularly, fiscal_start = 3,6,9,12 which can have a quarter of
  # [12, 1, 2] where the leap date can fall in the next year
  expect_identical(fiscal_year_quarter_day(2021, 4, 91, fiscal_start = 3),  fiscal_year_quarter_day(2021, 4, 90, fiscal_start = 3))
  expect_identical(fiscal_year_quarter_day(2021, 3, 91, fiscal_start = 6),  fiscal_year_quarter_day(2021, 3, 90, fiscal_start = 6))
  expect_identical(fiscal_year_quarter_day(2021, 2, 91, fiscal_start = 9),  fiscal_year_quarter_day(2021, 2, 90, fiscal_start = 9))
  expect_identical(fiscal_year_quarter_day(2021, 1, 91, fiscal_start = 12), fiscal_year_quarter_day(2021, 1, 90, fiscal_start = 12))
})
