test_that("catch nonexistent days with varying quarterly starts", {
  # particularly, start = 3,6,9,12 which can have a quarter of
  # [12, 1, 2] where the leap date can fall in the next year
  expect_identical(quarterly_year_quarternum_quarterday(2021, 4, 91, start = 3),  quarterly_year_quarternum_quarterday(2021, 4, 90, start = 3))
  expect_identical(quarterly_year_quarternum_quarterday(2021, 3, 91, start = 6),  quarterly_year_quarternum_quarterday(2021, 3, 90, start = 6))
  expect_identical(quarterly_year_quarternum_quarterday(2021, 2, 91, start = 9),  quarterly_year_quarternum_quarterday(2021, 2, 90, start = 9))
  expect_identical(quarterly_year_quarternum_quarterday(2021, 1, 91, start = 12), quarterly_year_quarternum_quarterday(2021, 1, 90, start = 12))
})
