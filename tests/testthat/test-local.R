test_that("can test for local objects", {
  x <- new_local(list(x = 1))
  expect_true(is_local(x))
})
