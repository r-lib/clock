test_that("can format with locale specific weekday", {
  expect_snapshot_output(format(year_month_weekday(2019), format = "%Y-%A", locale = date_locale(date_names = "fr")))
  expect_snapshot_output(format(year_month_weekday(2019), format = "%Y-%a", locale = date_locale(date_names = "fr")))
})

test_that("can format with locale specific month name", {
  expect_snapshot_output(format(year_month_day(2019), format = "%Y-%B", locale = date_locale(date_names = "fr")))
  expect_snapshot_output(format(year_month_day(2019), format = "%Y-%b", locale = date_locale(date_names = "fr")))
})

test_that("can format with locale specific AM/PM name", {
  expect_snapshot_output(format(naive_date_time(2019, hour = c(1, 13)), format = "%p", locale = date_locale("af")))
})

format(naive_date_time(2019, hour = c(1, 13)), format = "%p", locale = date_locale("af"))

test_that("can format with different decimal mark", {
  expect_snapshot_output(
    format(naive_date_nanotime(2019), locale = date_locale(decimal_mark = ","))
  )
})

test_that("decimal mark customization passes through with %T and %X", {
  expect_snapshot_output(format(naive_date_nanotime(2019), format = "%T", locale = date_locale(decimal_mark = ",")))
  expect_snapshot_output(format(naive_date_nanotime(2019), format = "%X", locale = date_locale(decimal_mark = ",")))
})

test_that("locale passes through to default calendar print method", {
  expect_snapshot_output(
    format(naive_second_point(year_month_weekday(2019)), locale = date_locale(date_names = "fr"))
  )
})
