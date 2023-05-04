# ------------------------------------------------------------------------------
# as_date()

test_that("can convert Date -> Date", {
  expect_identical(as_date(new_date(0)), new_date(0))
})

test_that("integer Dates are normalized to double", {
  # seq.Date() can make these
  x <- 0L
  class(x) <- "Date"

  expect_identical(as_date(x), new_date(0))
})

test_that("can convert POSIXct / POSIXlt -> Date (Date assumed to be naive)", {
  ct <- date_time_parse("2019-01-01 23:00:00", "America/New_York")
  lt <- as.POSIXlt(ct)

  expect <- date_parse("2019-01-01")

  expect_identical(as_date(ct), expect)
  expect_identical(as_date(lt), expect)
})

test_that("can convert calendar -> Date", {
  expect_identical(as_date(year_month_day(1970, 1, 2)), new_date(1))
  expect_identical(as_date(year_quarter_day(1970, 1, 2)), new_date(1))
})

test_that("can convert sys-time -> Date", {
  expect_identical(as_date(sys_seconds(0)), new_date(0))
})

test_that("can convert naive-time -> Date", {
  expect_identical(as_date(naive_seconds(0)), new_date(0))
})

test_that("can convert zoned-time -> Date", {
  x <- as_zoned_time(naive_time_parse("2019-01-01T23:02:03"), "America/New_York")
  expect <- date_parse("2019-01-01")
  expect_identical(as_date(x), expect)
})

# ------------------------------------------------------------------------------
# as.Date()

test_that("invalid dates must be resolved when converting to a Date", {
  expect_snapshot(error = TRUE, as.Date(year_month_day(2019, 2, 31)))
})

test_that("conversion from zoned-time uses naive-time as an intermediate", {
  x <- as_naive_time(year_month_day(2019, 12, 31, 23, 30, 00))
  x <- as_zoned_time(x, "America/New_York")

  expect_identical(as.Date(x), as.Date("2019-12-31"))
})

test_that("casting to Date floors components more precise than days (#205)", {
  x <- naive_time_parse("1969-01-01T13", precision = "hour")

  expect_identical(
    as.Date(x),
    date_build(1969, 1, 1)
  )
})

# ------------------------------------------------------------------------------
# as_sys_time()

test_that("converting to sys-time floors fractional dates (#191)", {
  x <- new_date(c(-0.5, 1.5))
  y <- new_date(c(-1, 1))

  expect_identical(as_sys_time(x), as_sys_time(y))
  expect_identical(as.Date(as_sys_time(x)), y)
})

test_that("converting to sys-time works with integer storage dates", {
  # These can occur from `seq.Date(from, to, length.out)`
  x <- structure(1L, class = "Date")
  y <- new_date(1)

  expect_identical(as_sys_time(x), as_sys_time(y))
  expect_identical(as.Date(as_sys_time(x)), y)
})

# ------------------------------------------------------------------------------
# as_zoned_time()

test_that("Dates are assumed to be naive", {
  x <- new_date(0)
  nt <- as_naive_time(year_month_day(1970, 1, 1))

  expect_identical(as_zoned_time(x, "UTC"), as_zoned_time(nt, "UTC"))
  expect_identical(as_zoned_time(x, "America/New_York"), as_zoned_time(nt, "America/New_York"))
})

test_that("can resolve nonexistent midnight issues", {
  # In Asia/Beirut, DST gap from 2021-03-27 23:59:59 -> 2021-03-28 01:00:00
  zone <- "Asia/Beirut"
  x <- as.Date("2021-03-28")

  expect_snapshot({
    (expect_error(as_zoned_time(x, zone), class = "clock_error_nonexistent_time"))
  })

  expect_identical(
    as_zoned_time(x, zone, nonexistent = "roll-forward"),
    as_zoned_time(as_naive_time(year_month_day(2021, 03, 28, 1)), zone)
  )
})

test_that("can resolve ambiguous midnight issues", {
  # In Asia/Amman, DST fallback from 2021-10-29 00:59:59 -> 2021-10-29 00:00:00
  zone <- "Asia/Amman"
  x <- as.Date("2021-10-29")

  expect_snapshot({
    (expect_error(as_zoned_time(x, zone), class = "clock_error_ambiguous_time"))
  })

  expect_identical(
    as_zoned_time(x, zone, ambiguous = "earliest"),
    zoned_time_parse_complete("2021-10-29T00:00:00+03:00[Asia/Amman]")
  )
  expect_identical(
    as_zoned_time(x, zone, ambiguous = "latest"),
    zoned_time_parse_complete("2021-10-29T00:00:00+02:00[Asia/Amman]")
  )
})

# ------------------------------------------------------------------------------
# as_weekday()

test_that("can convert to weekday", {
  x <- as.Date("2019-01-01")
  expect_identical(as_weekday(x), weekday(3L))
})

test_that("converting to weekday retains names", {
  x <- c(x = as.Date("2019-01-01"))
  expect_named(as_weekday(x), names(x))
})

# ------------------------------------------------------------------------------
# date_group()

test_that("can group by year/month/day", {
  x <- as.Date("2019-01-01") + c(0, 1, 2, 3)

  expect_identical(date_group(x, "day"), x)
  expect_identical(date_group(x, "day", n = 2), x[c(1, 1, 3, 3)])

  y <- as.Date(c("2019-01-01", "2019-02-05", "2019-03-10"))
  expect <- set_day(y, 1)

  expect_identical(date_group(y, "month"), expect)
  expect_identical(date_group(y, "month", n = 2), expect[c(1, 1, 3)])

  # Using year 0 as the starting point
  z <- as.Date(c("0000-01-05", "0001-02-10", "0002-03-12", "0003-01-15", "0004-12-10"))
  expect <- set_month(set_day(z, 1), 1)

  expect_identical(date_group(z, "year"), expect)
  expect_identical(date_group(z, "year", n = 3), expect[c(1, 1, 1, 4, 4)])
})

test_that("can't group by finer precisions", {
  x <- as.Date("2019-01-01")
  expect_snapshot(error = TRUE, date_group(x, "hour"))
  expect_snapshot(error = TRUE, date_group(x, "nanosecond"))
})

test_that("can't group by non-year-month-day precisions", {
  x <- as.Date("2019-01-01")
  expect_snapshot(error = TRUE, date_group(x, "quarter"))
})

# ------------------------------------------------------------------------------
# date_leap_year()

test_that("can detect leap years", {
  x <- c("2019-01-01", "2020-01-01", NA)
  x <- as.Date(x)
  expect_identical(date_leap_year(x), c(FALSE, TRUE, NA))
})

# ------------------------------------------------------------------------------
# date_floor()

test_that("can floor by weeks", {
  x <- c("1970-01-01", "1970-01-07", "1970-01-08")
  x <- as.Date(x)
  expect_identical(date_floor(x, "week"), x[c(1, 1, 3)])
  expect_identical(date_floor(x, "week", n = 2), x[c(1, 1, 1)])
})

test_that("can only floor by week/day", {
  expect_snapshot(error = TRUE, date_floor(as.Date("2019-01-01"), "hour"))
  expect_snapshot(error = TRUE, date_floor(as.Date("2019-01-01"), "month"))
})

test_that("`origin` can be used", {
  origin <- as.Date("1970-01-02")
  x <- as.Date(c("1970-01-01", "1970-01-02", "1970-01-03"))
  expect <- as.Date(c("1969-12-31", "1970-01-02", "1970-01-02"))
  expect_identical(date_floor(x, "day", n = 2, origin = origin), expect)
})

test_that("`origin` is validated", {
  x <- as.Date("2019-01-01")
  expect_snapshot(error = TRUE, date_floor(x, "day", origin = 1))
  expect_snapshot(error = TRUE, date_floor(x, "day", origin = new_date(NA_real_)))
  expect_snapshot(error = TRUE, date_floor(x, "day", origin = new_date(Inf)))
  expect_snapshot(error = TRUE, date_floor(x, "day", origin = new_date(c(0, 1))))
})

# ------------------------------------------------------------------------------
# date_ceiling()

test_that("can ceiling", {
  x <- c("1970-01-01", "1970-01-07", "1970-01-08")
  x <- as.Date(x)
  expect_identical(date_ceiling(x, "week"), x[c(1, 3, 3)])
  expect_identical(date_ceiling(x, "week", n = 2), as.Date(c("1970-01-01", "1970-01-15", "1970-01-15")))
})

# ------------------------------------------------------------------------------
# date_round()

test_that("can round", {
  x <- c("1970-01-01", "1970-01-03", "1970-01-05", "1970-01-08")
  x <- as.Date(x)
  expect_identical(date_round(x, "week"), x[c(1, 1, 4, 4)])
  expect_identical(date_round(x, "week", n = 2), as.Date(c("1970-01-01", "1970-01-01", "1970-01-01", "1970-01-15")))
})

# ------------------------------------------------------------------------------
# date_weekday_factor()

test_that("can convert to a weekday factor", {
  x <- as.Date("2019-01-01") + 0:6

  expect_identical(
    date_weekday_factor(x),
    weekday_factor(as_weekday(x)),
  )
  expect_identical(
    date_weekday_factor(x, labels = "fr", abbreviate = FALSE, encoding = "iso"),
    weekday_factor(as_weekday(x), labels = "fr", abbreviate = FALSE, encoding = "iso"),
  )
})

# ------------------------------------------------------------------------------
# date_month_factor()

test_that("can convert to a month factor", {
  x <- add_months(as.Date("2019-01-01"), 0:11)

  expect_identical(
    date_month_factor(x, labels = "fr", abbreviate = TRUE),
    calendar_month_factor(as_year_month_day(x), labels = "fr", abbreviate = TRUE),
  )
})

# ------------------------------------------------------------------------------
# date_format()

test_that("can format dates", {
  x <- as.Date("2018-12-31")
  formats <- test_all_formats(zone = FALSE)

  expect_snapshot(
    vapply(
      X = formats,
      FUN = function(format) date_format(x, format = format),
      FUN.VALUE = character(1)
    )
  )
  expect_snapshot(
    vapply(
      X = formats,
      FUN = function(format) date_format(x, format = format, locale = clock_locale("fr")),
      FUN.VALUE = character(1)
    )
  )
})

test_that("formatting Dates with `%z` or `%Z` returns NA with a warning", {
  x <- as.Date("2018-01-01")

  expect_warning(date_format(x, format = "%z"), class = "clock_warning_format_failures")
  expect_snapshot(date_format(x, format = "%z"))

  expect_warning(date_format(x, format = "%Z"), class = "clock_warning_format_failures")
  expect_snapshot(date_format(x, format = "%Z"))
})

# ------------------------------------------------------------------------------
# date_parse()

test_that("`%z` and `%Z` commands are ignored", {
  expect_identical(
    date_parse("2019-12-31 -0500", format = "%Y-%m-%d %z"),
    as.Date("2019-12-31")
  )
  expect_identical(
    date_parse("2019-12-31 America/New_York", format = "%Y-%m-%d %Z"),
    as.Date("2019-12-31")
  )
})

test_that("parsing into a date if you requested to parse time components rounds the time (#207) (#230) (undocumented)", {
  expect_identical(
    date_parse("2019-12-31 11:59:59", format = "%Y-%m-%d %H:%M:%S"),
    as.Date("2019-12-31")
  )
  expect_identical(
    date_parse("2019-12-31 12:00:00", format = "%Y-%m-%d %H:%M:%S"),
    as.Date("2020-01-01")
  )
})

test_that("parsing fails when undocumented rounding behavior would result in invalid 60 second component (#230) (undocumented)", {
  expect_warning(
    expect_identical(
      date_parse("2019-01-01 01:01:59.550", format = "%Y-%m-%d %H:%M:%6S"),
      new_date(NA_real_)
    ),
    class = "clock_warning_parse_failures"
  )
})

test_that("failure to parse throws a warning", {
  expect_warning(date_parse("foo"), class = "clock_warning_parse_failures")
  expect_snapshot(date_parse("foo"))
})

# ------------------------------------------------------------------------------
# date_shift()

test_that("can shift dates", {
  x <- date_parse("2019-01-01")
  x <- x + c(0, 1)

  monday <- weekday(clock_weekdays$monday)
  tuesday <- weekday(clock_weekdays$tuesday)

  expect_identical(
    date_shift(x, monday),
    add_days(x, c(6, 5))
  )
  expect_identical(
    date_shift(x, monday, which = "previous"),
    add_days(x, c(-1, -2))
  )
  expect_identical(
    date_shift(x, tuesday, boundary = "advance"),
    add_days(x, c(7, 6))
  )
})

# ------------------------------------------------------------------------------
# date_build()

test_that("can build a Date", {
  expect_identical(date_build(2019), as.Date("2019-01-01"))
  expect_identical(date_build(2020, 2, 3), as.Date("2020-02-03"))
})

test_that("can handle invalid dates", {
  expect_snapshot(error = TRUE, date_build(2019, 1:12, 31))

  expect_identical(
    date_build(2019, 1:12, 31, invalid = "previous"),
    date_build(2019, 1:12, "last")
  )
})

# ------------------------------------------------------------------------------
# date_today()

test_that("can get the current date", {
  x <- date_today("America/New_York")
  expect_length(x, 1)
  expect_s3_class(x, "Date")
})

# ------------------------------------------------------------------------------
# date_start()

test_that("can get the start", {
  x <- date_build(2019, 2, 2)
  expect_identical(date_start(x, "day"), x)
  expect_identical(date_start(x, "month"), date_build(2019, 2, 1))
  expect_identical(date_start(x, "year"), date_build(2019, 1, 1))
})

test_that("start: can't use invalid precisions", {
  expect_snapshot(error = TRUE, date_start(date_build(2019), "quarter"))
})

# ------------------------------------------------------------------------------
# date_end()

test_that("can get the end", {
  x <- date_build(2019:2020, 2, 2)
  expect_identical(date_end(x, "day"), x)
  expect_identical(date_end(x, "month"), date_build(2019:2020, 2, 28:29))
  expect_identical(date_end(x, "year"), date_build(2019:2020, 12, 31))
})

test_that("end: can't use invalid precisions", {
  expect_snapshot(error = TRUE, date_end(date_build(2019), "quarter"))
})

# ------------------------------------------------------------------------------
# date_seq()

test_that("integer `by` means day precision", {
  expect_identical(
    date_seq(new_date(0), to = new_date(5), by = 1),
    date_seq(new_date(0), to = new_date(5), by = duration_days(1))
  )
})

test_that("day precision seq works", {
  expect_identical(
    date_seq(new_date(0), to = new_date(5), by = duration_days(2)),
    new_date(c(0, 2, 4))
  )
})

test_that("granular `by` are allowed", {
  expect_identical(
    date_seq(date_build(2019, 1, 1), to = date_build(2019, 6, 1), by = duration_months(2)),
    date_build(2019, c(1, 3, 5), 1)
  )
  expect_identical(
    date_seq(date_build(2019, 1, 1), to = date_build(2025, 1, 1), by = duration_years(2)),
    date_build(c(2019, 2021, 2023, 2025), 1, 1)
  )
})

test_that("combining `by` with `total_size` works", {
  expect_identical(
    date_seq(date_build(2019, 1, 1), by = 2, total_size = 3),
    date_build(2019, 1, c(1, 3, 5))
  )
})

test_that("combining `to` with `total_size` works", {
  expect_identical(
    date_seq(date_build(2019, 1, 1), to = date_build(2019, 1, 5), total_size = 3),
    date_build(2019, 1, c(1, 3, 5))
  )
})

test_that("can resolve invalid dates", {
  from <- date_build(2019, 1, 31)
  to <- date_build(2019, 5, 31)

  expect_snapshot(error = TRUE, date_seq(from, to = to, by = duration_months(1)))

  expect_identical(
    date_seq(from, to = to, by = duration_months(1), invalid = "previous"),
    date_build(2019, 1:5, "last")
  )
})

test_that("quarterly `by` works", {
  expect_identical(
    date_seq(date_build(2019, 1, 2), by = duration_quarters(1), total_size = 3),
    date_seq(date_build(2019, 1, 2), by = duration_months(3), total_size = 3)
  )
})

test_that("weekly `by` works", {
  expect_identical(
    date_seq(date_build(2019, 1, 2), by = duration_weeks(1), total_size = 3),
    date_seq(date_build(2019, 1, 2), by = duration_days(7), total_size = 3)
  )
})

test_that("components of `from` more precise than `by` are restored", {
  expect_identical(
    date_seq(date_build(2019, 2, 3), by = duration_months(1), total_size = 2),
    date_build(2019, 2:3, 3)
  )
  expect_identical(
    date_seq(date_build(2019, 2, 3), by = duration_years(1), total_size = 2),
    date_build(2019:2020, 2, 3)
  )
})

test_that("seq() with `from > to && by > 0` or `from < to && by > 0` results in length 0 output (#282)", {
  expect_identical(
    date_seq(date_build(2019, 1, 2), to = date_build(2019, 1, 1), by = 1),
    date_build(integer())
  )

  expect_identical(
    date_seq(date_build(2019), to = date_build(2020), by = -1),
    date_build(integer())
  )
})

test_that("components of `to` more precise than `by` must match `from`", {
  expect_snapshot(error = TRUE, date_seq(date_build(2019, 1, 1), to = date_build(2019, 2, 2), by = duration_months(1)))
  expect_snapshot(error = TRUE, date_seq(date_build(2019, 1, 1), to = date_build(2019, 3, 1), by = duration_years(1)))
})

test_that("validates integerish `by`", {
  expect_snapshot(error = TRUE, date_seq(new_date(1), by = 1.5, total_size = 1))
})

test_that("validates `total_size` early", {
  expect_snapshot(error = TRUE, date_seq(new_date(1), by = 1, total_size = 1.5))
  expect_snapshot(error = TRUE, date_seq(new_date(1), by = 1, total_size = NA))
  expect_snapshot(error = TRUE, date_seq(new_date(1), by = 1, total_size = -1))
})

test_that("`to` and `total_size` must not generate a non-fractional sequence", {
  expect_snapshot(error = TRUE, date_seq(new_date(0), to = new_date(3), total_size = 3))
})

test_that("requires exactly two optional arguments", {
  expect_snapshot(error = TRUE, date_seq(new_date(1), by = 1))
  expect_snapshot(error = TRUE, date_seq(new_date(1), total_size = 1))
  expect_snapshot(error = TRUE, date_seq(new_date(1), to = new_date(1)))
})

test_that("requires `to` to be Date", {
  expect_snapshot(error = TRUE, date_seq(new_date(1), to = 1, by = 1))
})

test_that("requires year, month, or day precision", {
  expect_snapshot(error = TRUE, date_seq(new_date(1), to = new_date(2), by = duration_nanoseconds(1)))
})

test_that("checks empty dots", {
  expect_snapshot(error = TRUE, date_seq(new_date(1), new_date(2)))
})

test_that("golden test: ensure that we never allow components of `to` to differ with `from` (#224)", {
  from <- date_build(1970, 01, 31)
  to <- date_build(1970, 03, 01)

  expect_error(date_seq(from, to = to, by = duration_months(1), invalid = "overflow"))

  # Could theoretically generate this, where the second element is past `to`
  #> "1970-01-31" "1970-03-03"
})

# ------------------------------------------------------------------------------
# date_spanning_seq()

test_that("generates the regular sequence along the full span", {
  x <- date_build(c(2019, 2022, 2020), c(2, 1, 3), c(3, 5, 2))
  expect_identical(
    date_spanning_seq(x),
    seq(date_build(2019, 2, 3), date_build(2022, 1, 5), by = 1)
  )
})

test_that("missing values are removed", {
  x <- date_build(2020, 1, c(1, NA, 5, 2))
  expect_identical(date_spanning_seq(x), date_build(2020, 1, 1:5))

  x <- date_build(c(NA, NA))
  expect_identical(date_spanning_seq(x), new_date())
})

test_that("infinite dates are removed", {
  x <- new_date(c(0, Inf, 3, 6, -Inf))
  expect_identical(date_spanning_seq(x), new_date(as.double(0:6)))

  x <- new_date(c(Inf, -Inf))
  expect_identical(date_spanning_seq(x), new_date())
})

test_that("works with empty vectors", {
  x <- date_build(integer())
  expect_identical(date_spanning_seq(x), x)
})

# ------------------------------------------------------------------------------
# date_count_between()

test_that("can compute precisions at year / quarter / month / week / day", {
  x <- date_build(2019, 1, 5)

  y <- date_build(2025, 1, c(4, 6))
  expect_identical(date_count_between(x, y, "year"), c(5L, 6L))
  expect_identical(date_count_between(x, y, "quarter"), c(23L, 24L))
  expect_identical(date_count_between(x, y, "month"), c(71L, 72L))

  y <- date_build(2019, 1, c(25, 26))
  expect_identical(date_count_between(x, y, "week"), c(2L, 3L))
  expect_identical(date_count_between(x, y, "day"), c(20L, 21L))
})

test_that("must use a valid Date precision", {
  x <- date_build(2019)
  expect_snapshot((expect_error(date_count_between(x, x, "hour"))))
})

test_that("can't count between a Date and a POSIXt", {
  x <- date_build(2019)
  y <- date_time_build(2019, zone = "UTC")
  expect_snapshot((expect_error(date_count_between(x, y, "year"))))
})

# ------------------------------------------------------------------------------
# vec_arith()

test_that("<date> op <duration>", {
  expect_identical(vec_arith("+", new_date(0), duration_years(1)), new_date(365))
  expect_identical(vec_arith("-", new_date(0), duration_years(1)), new_date(-365))

  expect_snapshot(error = TRUE, vec_arith("+", new_date(0), duration_hours(1)))
  expect_snapshot(error = TRUE, vec_arith("*", new_date(0), duration_years(1)))
})

test_that("<duration> op <date>", {
  expect_identical(vec_arith("+", duration_years(1), new_date(0)), new_date(365))

  expect_snapshot(error = TRUE, vec_arith("-", duration_years(1), new_date(0)))
  expect_snapshot(error = TRUE, vec_arith("+", duration_hours(1), new_date(0)))
  expect_snapshot(error = TRUE, vec_arith("*", duration_years(1), new_date(0)))
})

# ------------------------------------------------------------------------------
# slider_plus() / slider_minus()

test_that("`slider_plus()` method is registered", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  x <- date_build(2019, 1, 1:2)

  y <- duration_days(2)
  expect_identical(
    slider::slider_plus(x, y),
    date_build(2019, 1, 3:4)
  )

  y <- duration_years(1)
  expect_identical(
    slider::slider_plus(x, y),
    date_build(2020, 1, 1:2)
  )
})

test_that("`slider_minus()` method is registered", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  x <- date_build(2019, 1, 1:2)

  y <- duration_days(2)
  expect_identical(
    slider::slider_minus(x, y),
    date_build(2018, 12, 30:31)
  )

  y <- duration_years(1)
  expect_identical(
    slider::slider_minus(x, y),
    date_build(2018, 1, 1:2)
  )
})

test_that("`slide_index()` works with dates and durations", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  i <- date_build(2019, 1, 1:4)
  x <- seq_along(i)

  before <- duration_days(1)
  after <- duration_days(2)

  expect_identical(
    slider::slide_index(x, i, identity, .before = before, .after = after),
    list(
      1:3,
      1:4,
      2:4,
      3:4
    )
  )
})

test_that("`slide_index()` will error on calendrical arithmetic and invalid dates", {
  skip_if_not_installed("slider", minimum_version = "0.3.0")

  i <- date_build(2019, 1, 28:31)
  x <- seq_along(i)

  after <- duration_months(1)

  expect_snapshot(error = TRUE, {
    slider::slide_index(x, i, identity, .after = after)
  })
})
