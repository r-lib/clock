# ------------------------------------------------------------------------------
# seq_expanded_n()

test_that("subtracts 1 from `n`", {
  # Known test where if `1` isn't subtracted from `n`,
  # then we get more breaks than this
  from <- duration_years(2)
  to <- duration_years(18)

  expect_identical(
    seq_expanded_n(from, to, n = 3),
    duration_years(c(2, 10, 18))
  )
})

test_that("works with `n = 0`", {
  from <- duration_years(2)
  to <- duration_years(18)

  expect_identical(
    seq_expanded_n(from, to, n = 0),
    duration_years(c(2, 18))
  )
})

test_that("works with `n = 1`", {
  from <- duration_years(2)
  to <- duration_years(18)

  expect_identical(
    seq_expanded_n(from, to, n = 1),
    duration_years(c(2, 18))
  )
})

test_that("works with `to - from = 0`", {
  from <- duration_years(2)
  to <- duration_years(2)

  expect_identical(
    seq_expanded_n(from, to, n = 3),
    duration_years(2)
  )
})
