# ------------------------------------------------------------------------------
# year-month

test_that("<year-month> - <year-month>", {
  x <- new_year_month()
  expect_identical(vec_ptype2(x, x), x)
})

test_that("<year-month> - <year-month-day>", {
  x <- new_year_month()
  y <- new_year_month_day()
  expect_identical(vec_ptype2(x, y), y)
  expect_identical(vec_ptype2(y, x), y)
})

test_that("<year-month> - <year-month-weekday>", {
  x <- new_year_month()
  y <- new_year_month_weekday()
  expect_identical(vec_ptype2(x, y), y)
  expect_identical(vec_ptype2(y, x), y)
})

test_that("<year-month> - <naive-time-point>", {
  x <- new_year_month()
  y <- new_naive_time_point(new_year_month_day())
  z <- new_naive_time_point(new_year_quarternum_quarterday())
  expect_identical(vec_ptype2(x, y), y)
  expect_identical(vec_ptype2(y, x), y)
  expect_error(vec_ptype2(x, z), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(z, x), class = "vctrs_error_incompatible_type")
})

test_that("<year-month> - <zoned-time-point>", {
  x <- new_year_month()
  y <- new_zoned_time_point(new_year_month_day())
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(y, x), class = "vctrs_error_incompatible_type")
})

# ------------------------------------------------------------------------------
# year-month-day

test_that("<year-month-day> - <year-month-day>", {
  x <- new_year_month_day()
  expect_identical(vec_ptype2(x, x), x)
})

test_that("<year-month-day> - <naive-time-point>", {
  x <- new_year_month_day()
  y <- new_naive_time_point(new_year_month_day())
  z <- new_naive_time_point(new_year_quarternum_quarterday())
  expect_identical(vec_ptype2(x, y), y)
  expect_identical(vec_ptype2(y, x), y)
  expect_error(vec_ptype2(x, z), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(z, x), class = "vctrs_error_incompatible_type")
})

test_that("<year-month-day> - <zoned-time-point>", {
  x <- new_year_month_day()
  y <- new_zoned_time_point(new_year_month_day())
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(y, x), class = "vctrs_error_incompatible_type")
})

# ------------------------------------------------------------------------------
# year-month-weekday

test_that("<year-month-weekday> - <year-month-weekday>", {
  x <- new_year_month_weekday()
  expect_identical(vec_ptype2(x, x), x)
})

test_that("<year-month-weekday> - <naive-time-point>", {
  x <- new_year_month_weekday()
  y <- new_naive_time_point(new_year_month_weekday())
  z <- new_naive_time_point(new_year_quarternum_quarterday())
  expect_identical(vec_ptype2(x, y), y)
  expect_identical(vec_ptype2(y, x), y)
  expect_error(vec_ptype2(x, z), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(z, x), class = "vctrs_error_incompatible_type")
})

test_that("<year-month-weekday> - <zoned-time-point>", {
  x <- new_year_month_weekday()
  y <- new_zoned_time_point(new_year_month_weekday())
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(y, x), class = "vctrs_error_incompatible_type")
})

# ------------------------------------------------------------------------------
# year-quarternum

test_that("<year-quarternum> - <year-quarternum>", {
  x <- new_year_quarternum(start = 1L)
  y <- new_year_quarternum(start = 2L)
  expect_identical(vec_ptype2(x, x), x)
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
})

test_that("<year-quarternum> - <year-quarternum-quarterday>", {
  x <- new_year_quarternum(start = 1L)
  y <- new_year_quarternum(start = 2L)
  z <- new_year_quarternum_quarterday(start = 1L)
  expect_identical(vec_ptype2(x, z), z)
  expect_identical(vec_ptype2(z, x), z)
  expect_error(vec_ptype2(y, z), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(z, y), class = "vctrs_error_incompatible_type")
})

test_that("<year-quarternum> - <naive-time-point>", {
  x <- new_year_quarternum(start = 1L)
  y <- new_year_quarternum(start = 2L)
  a <- new_naive_time_point(new_year_quarternum_quarterday(start = 1L))
  b <- new_naive_time_point(new_year_month_day())
  expect_identical(vec_ptype2(x, a), a)
  expect_identical(vec_ptype2(a, x), a)
  expect_error(vec_ptype2(y, a), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(a, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(x, b), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(b, x), class = "vctrs_error_incompatible_type")
})

test_that("<year-quarternum> - <zoned-time-point>", {
  x <- new_year_quarternum()
  y <- new_zoned_time_point(new_year_quarternum_quarterday())
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(y, x), class = "vctrs_error_incompatible_type")
})

# ------------------------------------------------------------------------------
# year-quarternum-quarterday

test_that("<year-quarternum-quarterday> - <year-quarternum-quarterday>", {
  x <- new_year_quarternum_quarterday(start = 1L)
  y <- new_year_quarternum_quarterday(start = 2L)
  expect_identical(vec_ptype2(x, x), x)
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
})

test_that("<year-quarternum-quarterday> - <naive-time-point>", {
  x <- new_year_quarternum_quarterday(start = 1L)
  y <- new_year_quarternum_quarterday(start = 2L)
  a <- new_naive_time_point(new_year_quarternum_quarterday(start = 1L))
  b <- new_naive_time_point(new_year_month_day())
  expect_identical(vec_ptype2(x, a), a)
  expect_identical(vec_ptype2(a, x), a)
  expect_error(vec_ptype2(y, a), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(a, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(x, b), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(b, x), class = "vctrs_error_incompatible_type")
})

test_that("<year-quarternum-quarterday> - <zoned-time-point>", {
  x <- new_year_quarternum_quarterday()
  y <- new_zoned_time_point(new_year_quarternum_quarterday())
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(y, x), class = "vctrs_error_incompatible_type")
})

# ------------------------------------------------------------------------------
# iso-year-weeknum

test_that("<iso-year-weeknum> - <iso-year-weeknum>", {
  x <- new_iso_year_weeknum()
  expect_identical(vec_ptype2(x, x), x)
})

test_that("<iso-year-weeknum> - <iso-year-weeknum-weekday>", {
  x <- new_iso_year_weeknum()
  y <- new_iso_year_weeknum_weekday()
  expect_identical(vec_ptype2(x, y), y)
  expect_identical(vec_ptype2(y, x), y)
})

test_that("<iso-year-weeknum> - <naive-time-point>", {
  x <- new_iso_year_weeknum()
  y <- new_naive_time_point(new_iso_year_weeknum_weekday())
  z <- new_naive_time_point(new_year_quarternum_quarterday())
  expect_identical(vec_ptype2(x, y), y)
  expect_identical(vec_ptype2(y, x), y)
  expect_error(vec_ptype2(x, z), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(z, x), class = "vctrs_error_incompatible_type")
})

test_that("<iso-year-weeknum> - <zoned-time-point>", {
  x <- new_iso_year_weeknum()
  y <- new_zoned_time_point(new_iso_year_weeknum_weekday())
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(y, x), class = "vctrs_error_incompatible_type")
})

# ------------------------------------------------------------------------------
# iso-year-weeknum-weekday

test_that("<iso-year-weeknum-weekday> - <iso-year-weeknum-weekday>", {
  x <- new_iso_year_weeknum_weekday()
  expect_identical(vec_ptype2(x, x), x)
})

test_that("<iso-year-weeknum-weekday> - <naive-time-point>", {
  x <- new_iso_year_weeknum_weekday()
  y <- new_naive_time_point(new_iso_year_weeknum_weekday())
  z <- new_naive_time_point(new_year_quarternum_quarterday())
  expect_identical(vec_ptype2(x, y), y)
  expect_identical(vec_ptype2(y, x), y)
  expect_error(vec_ptype2(x, z), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(z, x), class = "vctrs_error_incompatible_type")
})

test_that("<iso-year-weeknum-weekday> - <zoned-time-point>", {
  x <- new_iso_year_weeknum_weekday()
  y <- new_zoned_time_point(new_iso_year_weeknum_weekday())
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(y, x), class = "vctrs_error_incompatible_type")
})

# ------------------------------------------------------------------------------
# naive-time-point

test_that("<naive-time-point> - <naive-time-point>", {
  x <- new_naive_time_point(new_year_month_day(), precision = "second")
  y <- new_naive_time_point(new_year_month_weekday(), precision = "second")
  z <- new_naive_time_point(new_year_month_day(), nanoseconds_of_second = integer(), precision = "microsecond")
  expect_identical(vec_ptype2(x, x), x)
  expect_identical(vec_ptype2(x, z), z)
  expect_identical(vec_ptype2(z, x), z)
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(y, x), class = "vctrs_error_incompatible_type")
})

# ------------------------------------------------------------------------------
# zoned-time-point

test_that("<zoned-time-point> - <zoned-time-point>", {
  x <- new_zoned_time_point(new_year_month_day(), zone = "UTC", precision = "second")
  y <- new_zoned_time_point(new_year_month_weekday(), zone = "UTC", precision = "second")
  z <- new_zoned_time_point(new_year_month_day(), nanoseconds_of_second = integer(), zone = "UTC", precision = "microsecond")
  w <- new_zoned_time_point(new_year_month_day(), zone = "America/New_York", precision = "second")
  expect_identical(vec_ptype2(x, x), x)
  expect_identical(vec_ptype2(x, z), z)
  expect_identical(vec_ptype2(z, x), z)
  expect_error(vec_ptype2(x, y), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(y, x), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(x, w), class = "vctrs_error_incompatible_type")
  expect_error(vec_ptype2(w, x), class = "vctrs_error_incompatible_type")
})
