test_that("can test for naive objects", {
  x <- new_naive(list(x = 1))
  expect_true(is_naive(x))
})
