test_that("floor rounds down", {
  x <- duration_days(2) + duration_seconds(-1:1)
  x <- c(-x, x)

  expect2 <- duration_seconds(c(-172800, -172800, -172802, 172798, 172800, 172800))
  expect3 <- duration_days(c(-2, -2, -3, 1, 2, 2))
  expect4 <- duration_days(c(-2, -2, -4, 0, 2, 2))

  expect_identical(duration_floor(x, "second"), x)
  expect_identical(duration_floor(x, "second", n = 2), expect2)
  expect_identical(duration_floor(x, "day"), expect3)
  expect_identical(duration_floor(x, "day", n = 2), expect4)
})

test_that("ceiling rounds up", {
  x <- duration_days(2) + duration_seconds(-1:1)
  x <- c(-x, x)

  expect2 <- duration_seconds(c(-172798, -172800, -172800, 172800, 172800, 172802))
  expect3 <- duration_days(c(-1, -2, -2, 2, 2, 3))
  expect4 <- duration_days(c(0, -2, -2, 2, 2, 4))

  expect_identical(duration_ceiling(x, "second"), x)
  expect_identical(duration_ceiling(x, "second", n = 2), expect2)
  expect_identical(duration_ceiling(x, "day"), expect3)
  expect_identical(duration_ceiling(x, "day", n = 2), expect4)
})

test_that("round rounds to nearest, ties round up", {
  x <- duration_days(2) + duration_seconds(-1:3)
  x <- c(-x, x)

  expect2 <- duration_seconds(c(-172800, -172800, -172800, -172800, -172804, 172800, 172800, 172800, 172804, 172804))
  expect3 <- duration_days(c(-2, -2, -2, -2, -2, 2, 2, 2, 2, 2))
  expect4 <- duration_days(c(0, 0, -4, -4, -4, 0, 4, 4, 4, 4))

  expect_identical(duration_round(x, "second"), x)
  expect_identical(duration_round(x, "second", n = 4), expect2)
  expect_identical(duration_round(x, "day"), expect3)
  expect_identical(duration_round(x, "day", n = 4), expect4)
})

test_that("can't round to more precise precision", {
  expect_error(duration_floor(duration_seconds(1), "millisecond"), "more precise")
})

test_that("input is validated", {
  expect_error(duration_floor(1, "year"), "must be a duration object")
  expect_error(duration_floor(duration_seconds(1), "foo"), "valid precision")
  expect_error(duration_floor(duration_seconds(1), "day", n = -1), "positive number")
})
