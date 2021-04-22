#' @export
as_sys_time.Date <- function(x) {
  names <- names(x)
  x <- unstructure(x)
  if (is.double(x)) {
    x <- floor(x)
  }
  x <- duration_days(x)
  new_sys_time_from_fields(x, PRECISION_DAY, names)
}

#' @export
as_naive_time.Date <- function(x) {
  as_naive_time(as_sys_time(x))
}

#' Convert to a zoned-time from a date
#'
#' @description
#' This is a Date method for the [as_zoned_time()] generic.
#'
#' clock assumes that Dates are _naive_ date-time types. Like naive-times, they
#' have a yet-to-be-specified time zone. This method allows you to specify that
#' time zone, keeping the printed time. If possible, the time will be set to
#' midnight (see Details for the rare case in which this is not possible).
#'
#' @details
#' In the rare instance that the specified time zone does not contain a
#' date-time at midnight due to daylight saving time, `nonexistent` can be used
#' to resolve the issue. Similarly, if there are two possible midnight times due
#' to a daylight saving time fallback, `ambiguous` can be used.
#'
#' @inheritParams ellipsis::dots_empty
#' @inheritParams as-zoned-time-naive-time
#'
#' @param x `[Date]`
#'
#'   A Date.
#'
#' @return A zoned-time.
#'
#' @name as-zoned-time-Date
#' @export
#' @examples
#' x <- as.Date("2019-01-01")
#'
#' # The resulting zoned-times have the same printed time, but are in
#' # different time zones
#' as_zoned_time(x, "UTC")
#' as_zoned_time(x, "America/New_York")
#'
#' # Converting Date -> zoned-time is the same as naive-time -> zoned-time
#' x <- as_naive_time(year_month_day(2019, 1, 1))
#' as_zoned_time(x, "America/New_York")
#'
#' # In Asia/Beirut, there was a DST gap from
#' # 2021-03-27 23:59:59 -> 2021-03-28 01:00:00,
#' # skipping the 0th hour entirely. This means there is no midnight value.
#' x <- as.Date("2021-03-28")
#' try(as_zoned_time(x, "Asia/Beirut"))
#'
#' # To resolve this, set a `nonexistent` time resolution strategy
#' as_zoned_time(x, "Asia/Beirut", nonexistent = "roll-forward")
as_zoned_time.Date <- function(x,
                               zone,
                               ...,
                               nonexistent = NULL,
                               ambiguous = NULL) {
  x <- as_naive_time(x)
  as_zoned_time(x, zone = zone, ..., nonexistent = nonexistent, ambiguous = ambiguous)
}

#' @export
as_year_month_day.Date <- function(x) {
  as_year_month_day(as_naive_time(x))
}

#' @export
as_year_month_weekday.Date <- function(x) {
  as_year_month_weekday(as_naive_time(x))
}

#' @export
as_year_quarter_day.Date <- function(x, ..., start = NULL) {
  as_year_quarter_day(as_naive_time(x), ..., start = start)
}

#' @export
as_iso_year_week_day.Date <- function(x) {
  as_iso_year_week_day(as_naive_time(x))
}

#' @export
as_year_day.Date <- function(x) {
  as_year_day(as_naive_time(x))
}

#' @export
as_weekday.Date <- function(x) {
  as_weekday(as_naive_time(x))
}

# ------------------------------------------------------------------------------

# Not using `check_dots_empty()` because that might
# be too aggressive with base generics

#' @export
as.Date.clock_calendar <- function(x, ...) {
  as.Date(as_naive_time(x))
}

#' @export
as.Date.clock_time_point <- function(x, ...) {
  names <- clock_rcrd_names(x)
  x <- time_point_floor(x, "day")
  x <- field_ticks(x)
  x <- as.double(x)
  names(x) <- names
  new_date(x)
}

#' @export
as.Date.clock_zoned_time <- function(x, ...) {
  as.Date(as_naive_time(x))
}

# ------------------------------------------------------------------------------

#' Convert to a date
#'
#' @description
#' `as_date()` is a generic function that converts its input to a date (Date).
#'
#' There are methods for converting date-times (POSIXct), calendars,
#' time points, and zoned-times to dates.
#'
#' For converting to a date-time, see [as_date_time()].
#'
#' @details
#' Note that clock always assumes that R's Date class is naive, so converting
#' a POSIXct to a Date will always retain the printed year, month, and day
#' value.
#'
#' This is not a drop-in replacement for `as.Date()`, as it only converts a
#' limited set of types to Date. For parsing characters as dates, see
#' [date_parse()]. For converting numerics to dates, see [vctrs::new_date()] or
#' continue to use `as.Date()`.
#'
#' @param x `[vector]`
#'
#'   A vector.
#'
#' @return A date with the same length as `x`.
#'
#' @export
#' @examples
#' x <- date_time_parse("2019-01-01 23:02:03", "America/New_York")
#'
#' # R's `as.Date.POSIXct()` method defaults to changing the printed time
#' # to UTC before converting, which can result in odd conversions like this:
#' as.Date(x)
#'
#' # `as_date()` will never change the printed time before converting
#' as_date(x)
#'
#' # Can also convert from other clock types
#' as_date(year_month_day(2019, 2, 5))
as_date <- function(x) {
  UseMethod("as_date")
}

#' @rdname as_date
#' @export
as_date.Date <- function(x) {
  date_standardize(x)
}

#' @rdname as_date
#' @export
as_date.POSIXt <- function(x) {
  as.Date(as_naive_time(x))
}

#' @rdname as_date
#' @export
as_date.clock_calendar <- function(x) {
  as.Date(x)
}

#' @rdname as_date
#' @export
as_date.clock_time_point <- function(x) {
  as.Date(x)
}

#' @rdname as_date
#' @export
as_date.clock_zoned_time <- function(x) {
  as.Date(x)
}

# ------------------------------------------------------------------------------

#' Getters: date
#'
#' @description
#' These are Date methods for the [getter generics][clock-getters].
#'
#' - `get_year()` returns the Gregorian year.
#'
#' - `get_month()` returns the month of the year.
#'
#' - `get_day()` returns the day of the month.
#'
#' For more advanced component extraction, convert to the calendar type
#' that you are interested in.
#'
#' @param x `[Date]`
#'
#'   A Date to get the component from.
#'
#' @return The component.
#'
#' @name Date-getters
#' @examples
#' x <- as.Date("2019-01-01") + 0:5
#' get_day(x)
NULL

#' @rdname Date-getters
#' @export
get_year.Date <- function(x) {
  get_date_field_year_month_day(x, get_year)
}
#' @rdname Date-getters
#' @export
get_month.Date <- function(x) {
  get_date_field_year_month_day(x, get_month)
}
#' @rdname Date-getters
#' @export
get_day.Date <- function(x) {
  get_date_field_year_month_day(x, get_day)
}
get_date_field_year_month_day <- function(x, get_fn) {
  x <- as_year_month_day(x)
  get_fn(x)
}

# ------------------------------------------------------------------------------

#' Setters: date
#'
#' @description
#' These are Date methods for the [setter generics][clock-setters].
#'
#' - `set_year()` sets the year.
#'
#' - `set_month()` sets the month of the year. Valid values are in the range
#'   of `[1, 12]`.
#'
#' - `set_day()` sets the day of the month. Valid values are in the range
#'   of `[1, 31]`.
#'
#' @inheritParams ellipsis::dots_empty
#' @inheritParams invalid_resolve
#'
#' @param x `[Date]`
#'
#'   A Date vector.
#'
#' @param value `[integer / "last"]`
#'
#'   The value to set the component to.
#'
#'   For `set_day()`, this can also be `"last"` to set the day to the
#'   last day of the month.
#'
#' @return `x` with the component set.
#'
#' @name Date-setters
#' @examples
#' x <- as.Date("2019-02-01")
#'
#' # Set the day
#' set_day(x, 12:14)
#'
#' # Set to the "last" day of the month
#' set_day(x, "last")
#'
#' # You cannot set a Date to an invalid day like you can with
#' # a year-month-day. Instead, the default strategy is to error.
#' try(set_day(x, 31))
#' set_day(as_year_month_day(x), 31)
#'
#' # You can resolve these issues while setting the day by specifying
#' # an invalid date resolution strategy with `invalid`
#' set_day(x, 31, invalid = "previous")
NULL

#' @rdname Date-setters
#' @export
set_year.Date <- function(x, value, ..., invalid = NULL) {
  set_date_field_year_month_day(x, value, invalid, set_year, ...)
}
#' @rdname Date-setters
#' @export
set_month.Date <- function(x, value, ..., invalid = NULL) {
  set_date_field_year_month_day(x, value, invalid, set_month, ...)
}
#' @rdname Date-setters
#' @export
set_day.Date <- function(x, value, ..., invalid = NULL) {
  set_date_field_year_month_day(x, value, invalid, set_day, ...)
}
set_date_field_year_month_day <- function(x, value, invalid, set_fn, ...) {
  check_dots_empty()
  x <- as_year_month_day(x)
  x <- set_fn(x, value)
  x <- invalid_resolve(x, invalid = invalid)
  as.Date(x)
}

# ------------------------------------------------------------------------------

#' @method vec_arith.Date clock_duration
#' @export
vec_arith.Date.clock_duration <- function(op, x, y, ...) {
  arith_date_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration Date
#' @export
vec_arith.clock_duration.Date <- function(op, x, y, ...) {
  arith_duration_and_date(op, x, y, ...)
}

arith_date_and_duration <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(x, y),
    "-" = add_duration(x, -y),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_duration_and_date <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(y, x, swapped = TRUE),
    "-" = stop_incompatible_op(op, x, y, details = "Can't subtract a Date from a duration.", ...),
    stop_incompatible_op(op, x, y, ...)
  )
}

# ------------------------------------------------------------------------------

#' Arithmetic: date
#'
#' @description
#' These are Date methods for the
#' [arithmetic generics][clock-arithmetic].
#'
#' Calendrical based arithmetic:
#'
#' These functions convert to a year-month-day calendar, perform
#' the arithmetic, then convert back to a Date.
#'
#' - `add_years()`
#'
#' - `add_quarters()`
#'
#' - `add_months()`
#'
#' Time point based arithmetic:
#'
#' These functions convert to a time point, perform the arithmetic, then
#' convert back to a Date.
#'
#' - `add_weeks()`
#'
#' - `add_days()`
#'
#' @details
#' Adding a single quarter with `add_quarters()` is equivalent to adding
#' 3 months.
#'
#' `x` and `n` are recycled against each other.
#'
#' Only calendrical based arithmetic has the potential to generate invalid
#' dates. Time point based arithmetic, like adding days, will always generate
#' a valid date.
#'
#' @inheritParams add_years
#' @inheritParams invalid_resolve
#'
#' @param x `[Date]`
#'
#'   A Date vector.
#'
#' @return `x` after performing the arithmetic.
#'
#' @name Date-arithmetic
#'
#' @examples
#' x <- as.Date("2019-01-01")
#'
#' add_years(x, 1:5)
#'
#' y <- as.Date("2019-01-31")
#'
#' # Adding 1 month to `y` generates an invalid date. Unlike year-month-day
#' # types, R's native Date type cannot handle invalid dates, so you must
#' # resolve them immediately. If you don't you get an error:
#' try(add_months(y, 1:2))
#' add_months(as_year_month_day(y), 1:2)
#'
#' # Resolve invalid dates by specifying an invalid date resolution strategy
#' # with the `invalid` argument. Using `"previous"` here sets the date to
#' # the previous valid date - i.e. the end of the month.
#' add_months(y, 1:2, invalid = "previous")
NULL

#' @rdname Date-arithmetic
#' @export
add_years.Date <- function(x, n, ..., invalid = NULL) {
  add_date_duration_year_month_day(x, n, invalid, add_years, ...)
}
#' @rdname Date-arithmetic
#' @export
add_quarters.Date <- function(x, n, ..., invalid = NULL) {
  add_date_duration_year_month_day(x, n, invalid, add_quarters, ...)
}
#' @rdname Date-arithmetic
#' @export
add_months.Date <- function(x, n, ..., invalid = NULL) {
  add_date_duration_year_month_day(x, n, invalid, add_months, ...)
}
add_date_duration_year_month_day <- function(x, n, invalid, add_fn, ...) {
  check_dots_empty()
  x <- as_year_month_day(x)
  x <- add_fn(x, n)
  x <- invalid_resolve(x, invalid = invalid)
  as.Date(x)
}

#' @rdname Date-arithmetic
#' @export
add_weeks.Date <- function(x, n, ...) {
  add_date_duration_time_point(x, n, add_weeks, ...)
}
#' @rdname Date-arithmetic
#' @export
add_days.Date <- function(x, n, ...) {
  add_date_duration_time_point(x, n, add_days, ...)
}
add_date_duration_time_point <- function(x, n, add_fn, ...) {
  check_dots_empty()
  x <- as_naive_time(x)
  x <- add_fn(x, n)
  as.Date(x)
}

# ------------------------------------------------------------------------------

#' Group date and date-time components
#'
#' @description
#' `date_group()` groups by a single component of a date-time, such as month
#' of the year, or day of the month.
#'
#' There are separate help pages for grouping dates and date-times:
#'
#' - [dates (Date)][date-group]
#'
#' - [date-times (POSIXct/POSIXlt)][posixt-group]
#'
#' @inheritParams calendar_group
#'
#' @param x `[Date / POSIXct / POSIXlt]`
#'
#'   A date or date-time vector.
#'
#' @param precision `[character(1)]`
#'
#'   A precision. Allowed precisions are dependent on the input used.
#'
#' @return `x`, grouped at `precision`.
#'
#' @export
#' @examples
#' # See type specific documentation for more examples
#' date_group(as.Date("2019-01-01") + 0:5, "day", n = 2)
date_group <- function(x, precision, ..., n = 1L) {
  UseMethod("date_group")
}

#' Group date components
#'
#' @description
#' This is a Date method for the [date_group()] generic.
#'
#' `date_group()` groups by a single component of a Date, such as month
#' of the year, or day of the month.
#'
#' If you need to group by more complex components, like ISO weeks, or quarters,
#' convert to a calendar type that contains the component you are interested
#' in grouping by.
#'
#' @inheritParams date_group
#' @inheritParams invalid_resolve
#'
#' @param x `[Date]`
#'
#'   A date vector.
#'
#' @param precision `[character(1)]`
#'
#'   One of:
#'
#'   - `"year"`
#'
#'   - `"month"`
#'
#'   - `"day"`
#'
#' @return `x`, grouped at `precision`.
#'
#' @name date-group
#'
#' @export
#' @examples
#' x <- as.Date("2019-01-01") + -3:5
#' x
#'
#' # Group by 2 days of the current month.
#' # Note that this resets at the beginning of the month, creating day groups
#' # of [29, 30] [31] [01, 02] [03, 04].
#' date_group(x, "day", n = 2)
#'
#' # Group by month
#' date_group(x, "month")
date_group.Date <- function(x, precision, ..., n = 1L, invalid = NULL) {
  x <- as_year_month_day(x)
  x <- calendar_group(x, precision, ..., n = n)
  x <- calendar_widen(x, "day")
  as.Date(x, invalid = invalid)
}

# ------------------------------------------------------------------------------

#' Is the year a leap year?
#'
#' `date_leap_year()` detects if the year is a leap year.
#'
#' @param x `[Date / POSIXct / POSIXlt]`
#'
#'   A date or date-time to detect leap years in.
#'
#' @return A logical vector the same size as `x`. Returns `TRUE` if in a leap
#'   year, `FALSE` if not in a leap year, and `NA` if `x` is `NA`.
#'
#' @examples
#' x <- as.Date("2019-01-01")
#' x <- add_years(x, 0:5)
#' date_leap_year(x)
#'
#' y <- as.POSIXct("2019-01-01", "America/New_York")
#' y <- add_years(y, 0:5)
#' date_leap_year(y)
#' @export
date_leap_year <- function(x) {
  UseMethod("date_leap_year")
}

#' @export
date_leap_year.Date <- function(x) {
  x <- as_year_month_day(x)
  calendar_leap_year(x)
}

# ------------------------------------------------------------------------------

#' Date and date-time rounding
#'
#' @description
#' - `date_floor()` rounds a date or date-time down to a multiple of
#'   the specified `precision`.
#'
#' - `date_ceiling()` rounds a date or date-time up to a multiple of
#'   the specified `precision`.
#'
#' - `date_round()` rounds up or down depending on what is closer,
#'   rounding up on ties.
#'
#' There are separate help pages for rounding dates and date-times:
#'
#' - [dates (Date)][date-rounding]
#'
#' - [date-times (POSIXct/POSIXlt)][posixt-rounding]
#'
#' These functions round the underlying duration itself, relative to an
#' `origin`. For example, rounding to 15 hours will construct groups of
#' 15 hours, starting from `origin`, which defaults to a naive time of
#' 1970-01-01 00:00:00.
#'
#' If you want to group by components, such as "day of the month", see
#' [date_group()].
#'
#' @inheritParams date_group
#'
#' @param origin `[Date(1) / POSIXct(1) / POSIXlt(1) / NULL]`
#'
#'   An origin to start counting from. The default `origin` is
#'   midnight on 1970-01-01 in the time zone of `x`.
#'
#' @return `x` rounded to the specified `precision`.
#'
#' @name date-and-date-time-rounding
#'
#' @examples
#' # See the type specific documentation for more examples
#'
#' x <- as.Date("2019-03-31") + 0:5
#' x
#'
#' # Flooring by 2 days, note that this is not tied to the current month,
#' # and instead counts from the specified `origin`.
#' date_floor(x, "day", n = 2)
NULL

#' @rdname date-and-date-time-rounding
#' @export
date_floor <- function(x, precision, ..., n = 1L, origin = NULL) {
  UseMethod("date_floor")
}

#' @rdname date-and-date-time-rounding
#' @export
date_ceiling <- function(x, precision, ..., n = 1L, origin = NULL) {
  UseMethod("date_ceiling")
}

#' @rdname date-and-date-time-rounding
#' @export
date_round <- function(x, precision, ..., n = 1L, origin = NULL) {
  UseMethod("date_round")
}

#' Rounding: date
#'
#' @description
#' These are Date methods for the
#' [rounding generics][date-and-date-time-rounding].
#'
#' - `date_floor()` rounds a date down to a multiple of
#'   the specified `precision`.
#'
#' - `date_ceiling()` rounds a date up to a multiple of
#'   the specified `precision`.
#'
#' - `date_round()` rounds up or down depending on what is closer,
#'   rounding up on ties.
#'
#' The only supported rounding `precision`s for Dates are `"day"` and `"week"`.
#' You can group by irregular periods such as `"month"` or `"year"` by using
#' [date_group()].
#'
#' @details
#' When rounding by `"week"`, remember that the `origin` determines the "week
#' start". By default, 1970-01-01 is the implicit origin, which is a
#' Thursday. If you would like to round by weeks with a different week start,
#' just supply an origin on the weekday you are interested in.
#'
#' @inheritParams date_floor
#'
#' @param x `[Date]`
#'
#'   A date vector.
#'
#' @param precision `[character(1)]`
#'
#'   One of:
#'
#'   - `"week"`
#'
#'   - `"day"`
#'
#'   `"week"` is an alias for `"day"` with `n * 7`.
#'
#' @param origin `[Date(1) / NULL]`
#'
#'   An origin to start counting from. The default `origin` is
#'   1970-01-01.
#'
#' @return `x` rounded to the specified `precision`.
#'
#' @name date-rounding
#'
#' @examples
#' x <- as.Date("2019-03-31") + 0:5
#' x
#'
#' # Flooring by 2 days, note that this is not tied to the current month,
#' # and instead counts from the specified `origin`, so groups can cross
#' # the month boundary
#' date_floor(x, "day", n = 2)
#'
#' # Compare to `date_group()`, which groups by the day of the month
#' date_group(x, "day", n = 2)
#'
#' y <- as.Date("2019-01-01") + 0:20
#' y
#'
#' # Flooring by week uses an implicit `origin` of 1970-01-01, which
#' # is a Thursday
#' date_floor(y, "week")
#' as_weekday(date_floor(y, "week"))
#'
#' # If you want to round by weeks with a different week start, supply an
#' # `origin` that falls on the weekday you care about. This uses a Monday.
#' origin <- as.Date("1970-01-05")
#' as_weekday(origin)
#'
#' date_floor(y, "week", origin = origin)
#' as_weekday(date_floor(y, "week", origin = origin))
NULL

#' @rdname date-rounding
#' @export
date_floor.Date <- function(x, precision, ..., n = 1L, origin = NULL) {
  date_rounder(x, precision, n, origin, time_point_floor, ...)
}

#' @rdname date-rounding
#' @export
date_ceiling.Date <- function(x, precision, ..., n = 1L, origin = NULL) {
  date_rounder(x, precision, n, origin, time_point_ceiling, ...)
}

#' @rdname date-rounding
#' @export
date_round.Date <- function(x, precision, ..., n = 1L, origin = NULL) {
  date_rounder(x, precision, n, origin, time_point_round, ...)
}

date_rounder <- function(x, precision, n, origin, time_point_rounder, ...) {
  result <- tweak_date_rounder_precision(precision, n)
  precision <- result$precision
  n <- result$n

  x <- as_naive_time(x)

  if (!is_null(origin)) {
    origin <- collect_date_rounder_origin(origin)
  }

  x <- time_point_rounder(x, precision, ..., n = n, origin = origin)

  as.Date(x)
}

# Note:
# For Date and POSIXct, which are always day and second precision, we can
# allow a special "week" precision for the rounding functions. This isn't
# normally allowed for time points, as there is no week precision time point,
# and instead you'd do `day`,` `n = n * 7`. This makes that a little easier.
tweak_date_rounder_precision <- function(precision, n) {
  if (identical(precision, "week")) {
    precision <- "day"
    n <- n * 7L
  }

  list(precision = precision, n = n)
}

collect_date_rounder_origin <- function(origin) {
  if (!inherits(origin, "Date")) {
    abort("`origin` must be a 'Date'.")
  }
  if (length(origin) != 1L) {
    abort("`origin` must have length 1.")
  }
  if (!is.finite(origin)) {
    abort("`origin` must not be `NA` or an infinite date.")
  }

  origin <- as_naive_time(origin)

  origin
}

# ------------------------------------------------------------------------------

#' Convert a date or date-time to a weekday factor
#'
#' `date_weekday_factor()` converts a date or date-time to an ordered factor
#' with levels representing the weekday. This can be useful in combination with
#' ggplot2, or for modeling.
#'
#' @inheritParams weekday_factor
#'
#' @param x `[Date / POSIXct / POSIXlt]`
#'
#'   A date or date-time vector.
#'
#' @return An ordered factor representing the weekdays.
#'
#' @export
#' @examples
#' x <- as.Date("2019-01-01") + 0:6
#'
#' # Default to Sunday -> Saturday
#' date_weekday_factor(x)
#'
#' # ISO encoding is Monday -> Sunday
#' date_weekday_factor(x, encoding = "iso")
#'
#' # With full names
#' date_weekday_factor(x, abbreviate = FALSE)
#'
#' # Or a different language
#' date_weekday_factor(x, labels = "fr")
date_weekday_factor <- function(x,
                                ...,
                                labels = "en",
                                abbreviate = TRUE,
                                encoding = "western") {
  UseMethod("date_weekday_factor")
}

#' @export
date_weekday_factor.Date <- function(x,
                                     ...,
                                     labels = "en",
                                     abbreviate = TRUE,
                                     encoding = "western") {
  x <- as_weekday(x)
  weekday_factor(x, ..., labels = labels, abbreviate = abbreviate, encoding = encoding)
}

# ------------------------------------------------------------------------------

#' Convert a date or date-time to an ordered factor of month names
#'
#' @description
#' `date_month_factor()` extracts the month values from a date or date-time and
#' converts them to an ordered factor of month names. This can be useful in
#' combination with ggplot2, or for modeling.
#'
#' @inheritParams calendar_month_factor
#'
#' @param x `[Date / POSIXct / POSIXlt]`
#'
#'   A date or date-time vector.
#'
#' @return An ordered factor representing the months.
#'
#' @export
#' @examples
#' x <- add_months(as.Date("2019-01-01"), 0:11)
#'
#' date_month_factor(x)
#' date_month_factor(x, abbreviate = TRUE)
#' date_month_factor(x, labels = "fr")
date_month_factor <- function(x,
                              ...,
                              labels = "en",
                              abbreviate = FALSE) {
  UseMethod("date_month_factor")
}

#' @export
date_month_factor.Date <- function(x,
                                   ...,
                                   labels = "en",
                                   abbreviate = FALSE) {
  x <- as_year_month_day(x)
  calendar_month_factor(x, ..., labels = labels, abbreviate = abbreviate)
}

# ------------------------------------------------------------------------------

#' Formatting: date and date-time
#'
#' @description
#' `date_format()` formats a date (Date) or date-time (POSIXct/POSIXlt) using
#' a `format` string.
#'
#' There are separate help pages for formatting dates and date-times:
#'
#' - [dates (Date)][date-formatting]
#'
#' - [date-times (POSIXct/POSIXlt)][posixt-formatting]
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param x `[Date / POSIXct / POSIXlt]`
#'
#'   A date or date-time vector.
#'
#' @return A character vector of the formatted input.
#'
#' @export
#' @examples
#' # See method specific documentation for more examples
#'
#' x <- as.Date("2019-01-01")
#' date_format(x, format = "year: %Y, month: %m, day: %d")
date_format <- function(x, ...) {
  UseMethod("date_format")
}

#' Formatting: date
#'
#' @description
#' This is a Date method for the [date_format()] generic.
#'
#' `date_format()` formats a date (Date) using a `format` string.
#'
#' If `format` is `NULL`, a default format of `"%Y-%m-%d"` is used.
#'
#' @details
#' Because a Date is considered to be a _naive_ type in clock, meaning that
#' it currently has no implied time zone, using the `%z` or `%Z` format commands
#' is not allowed and will result in `NA`.
#'
#' @inheritParams ellipsis::dots_empty
#' @inheritParams format.clock_zoned_time
#'
#' @param x `[Date]`
#'
#'   A date vector.
#'
#' @return A character vector of the formatted input.
#'
#' @name date-formatting
#'
#' @export
#' @examples
#' x <- as.Date("2019-01-01")
#'
#' # Default
#' date_format(x)
#'
#' date_format(x, format = "year: %Y, month: %m, day: %d")
#'
#' # With different locales
#' date_format(x, format = "%A, %B %d, %Y")
#' date_format(x, format = "%A, %B %d, %Y", locale = clock_locale("fr"))
date_format.Date <- function(x,
                             ...,
                             format = NULL,
                             locale = clock_locale()) {
  check_dots_empty()
  x <- as_naive_time(x)
  format(x, format = format, locale = locale)
}

# ------------------------------------------------------------------------------

#' @export
date_zone.Date <- function(x) {
  abort("Can't get the zone of a 'Date'.")
}

#' @export
date_set_zone.Date <- function(x, zone) {
  abort("Can't set the zone of a 'Date'.")
}

# ------------------------------------------------------------------------------

#' Parsing: date
#'
#' @description
#' `date_parse()` parses strings into a Date.
#'
#' The default `format` used is `"%Y-%m-%d"`.
#'
#' @details
#' _`date_parse()` ignores both the `%z` and `%Z` commands,_ as clock treats
#' Date as a _naive_ type, with a yet-to-be-specified time zone.
#'
#' If parsing a string with sub-daily components, such as hours, minutes or
#' seconds, note that the conversion to Date will round those components to
#' the nearest day. See the examples for a way to control this.
#'
#' @inheritParams zoned-parsing
#'
#' @return A Date.
#'
#' @export
#' @examples
#' date_parse("2020-01-01")
#'
#' date_parse(
#'   "January 5, 2020",
#'   format = "%B %d, %Y"
#' )
#'
#' # With a different locale
#' date_parse(
#'   "janvier 5, 2020",
#'   format = "%B %d, %Y",
#'   locale = clock_locale("fr")
#' )
#'
#' # A neat feature of `date_parse()` is the ability to parse
#' # the ISO year-week-day format
#' date_parse("2020-W01-2", format = "%G-W%V-%u")
#'
#' # ---------------------------------------------------------------------------
#' # Rounding of sub-daily components
#'
#' # Note that rounding a string with time components will round them to the
#' # nearest day if you try and parse them
#' x <- c("2019-01-01 11", "2019-01-01 12")
#'
#' # Hour 12 rounds up to the next day
#' date_parse(x, format = "%Y-%m-%d %H")
#'
#' # If you don't like this, one option is to just not parse the time component
#' date_parse(x, format = "%Y-%m-%d")
#'
#' # A more general option is to parse the full string as a naive-time,
#' # then round manually
#' nt <- naive_time_parse(x, format = "%Y-%m-%d %H", precision = "hour")
#' nt
#'
#' nt <- time_point_floor(nt, "day")
#' nt
#'
#' as.Date(nt)
date_parse <- function(x, ..., format = NULL, locale = clock_locale()) {
  x <- naive_time_parse(x, ..., format = format, precision = "day", locale = locale)
  as.Date(x)
}

# ------------------------------------------------------------------------------

#' Shifting: date and date-time
#'
#' @description
#' `date_shift()` shifts `x` to the `target` weekday. You can shift to the next
#' or previous weekday. If `x` is currently on the `target` weekday, you can
#' choose to leave it alone or advance it to the next instance of the `target`.
#'
#' There are separate help pages for shifting dates and date-times:
#'
#' - [dates (Date)][date-shifting]
#'
#' - [date-times (POSIXct/POSIXlt)][posixt-shifting]
#'
#' @inheritParams time_point_shift
#'
#' @param x `[Date / POSIXct / POSIXlt]`
#'
#'   A date or date-time vector.
#'
#' @return `x` shifted to the `target` weekday.
#'
#' @name date-and-date-time-shifting
#'
#' @export
#' @examples
#' # See the type specific documentation for more examples
#'
#' x <- as.Date("2019-01-01") + 0:1
#'
#' # A Tuesday and Wednesday
#' as_weekday(x)
#'
#' monday <- weekday(clock_weekdays$monday)
#'
#' # Shift to the next Monday
#' date_shift(x, monday)
date_shift <- function(x,
                       target,
                       ...,
                       which = "next",
                       boundary = "keep") {
  UseMethod("date_shift")
}

#' Shifting: date
#'
#' @description
#' `date_shift()` shifts `x` to the `target` weekday. You can shift to the next
#' or previous weekday. If `x` is currently on the `target` weekday, you can
#' choose to leave it alone or advance it to the next instance of the `target`.
#'
#' Weekday shifting is one of the easiest ways to floor by week while
#' controlling what is considered the first day of the week. You can also
#' accomplish this with the `origin` argument of [date_floor()], but this is
#' slightly easier.
#'
#' @inheritParams time_point_shift
#'
#' @param x `[Date]`
#'
#'   A date vector.
#'
#' @return `x` shifted to the `target` weekday.
#'
#' @name date-shifting
#'
#' @export
#' @examples
#' x <- as.Date("2019-01-01") + 0:1
#'
#' # A Tuesday and Wednesday
#' as_weekday(x)
#'
#' monday <- weekday(clock_weekdays$monday)
#'
#' # Shift to the next Monday
#' date_shift(x, monday)
#'
#' # Shift to the previous Monday
#' # This is an easy way to "floor by week" with a target weekday in mind
#' date_shift(x, monday, which = "previous")
#'
#' # What about Tuesday?
#' tuesday <- weekday(clock_weekdays$tuesday)
#'
#' # Notice that the day that was currently on a Tuesday was not shifted
#' date_shift(x, tuesday)
#'
#' # You can force it to `"advance"`
#' date_shift(x, tuesday, boundary = "advance")
date_shift.Date <- function(x,
                            target,
                            ...,
                            which = "next",
                            boundary = "keep") {
  x <- as_naive_time(x)
  x <- time_point_shift(x, target, ..., which = which, boundary = boundary)
  as.Date(x)
}

# ------------------------------------------------------------------------------

#' Building: date
#'
#' @description
#' `date_build()` builds a Date from it's individual components.
#'
#' @details
#' Components are recycled against each other.
#'
#' @inheritParams invalid_resolve
#'
#' @param year `[integer]`
#'
#'   The year. Values `[-32767, 32767]` are generally allowed.
#'
#' @param month `[integer]`
#'
#'   The month. Values `[1, 12]` are allowed.
#'
#' @param day `[integer / "last"]`
#'
#'   The day of the month. Values `[1, 31]` are allowed.
#'
#'   If `"last"`, then the last day of the month is returned.
#'
#' @return A Date.
#'
#' @export
#' @examples
#' date_build(2019)
#' date_build(2019, 1:3)
#'
#' # Generating invalid dates will trigger an error
#' try(date_build(2019, 1:12, 31))
#'
#' # You can resolve this with `invalid`
#' date_build(2019, 1:12, 31, invalid = "previous")
#'
#' # But this particular case (the last day of the month) is better
#' # specified as:
#' date_build(2019, 1:12, "last")
date_build <- function(year, month = 1L, day = 1L, ..., invalid = NULL) {
  check_dots_empty()
  x <- year_month_day(year, month, day)
  x <- invalid_resolve(x, invalid = invalid)
  as.Date(x)
}

# ------------------------------------------------------------------------------

#' Current date and date-time
#'
#' @description
#' - `date_today()` returns the current date in the specified `zone` as a Date.
#'
#' - `date_now()` returns the current date-time in the specified `zone` as a
#' POSIXct.
#'
#' @details
#' clock assumes that Date is a _naive_ type, like naive-time. This means that
#' `date_today()` first looks up the current date-time in the specified `zone`,
#' then converts that to a Date, retaining the printed time while dropping any
#' information about that time zone.
#'
#' @inheritParams zoned_time_now
#'
#' @return
#' - `date_today()` a single Date.
#'
#' - `date_now()` a single POSIXct.
#'
#' @name date-today
#'
#' @examples
#' # Current date in the local time zone
#' date_today("")
#'
#' # Current date in a specified time zone
#' date_today("Europe/London")
#'
#' # Current date-time in that same time zone
#' date_now("Europe/London")
NULL

#' @rdname date-today
#' @export
date_today <- function(zone) {
  as.Date(zoned_time_now(zone))
}

# ------------------------------------------------------------------------------

#' Sequences: date and date-time
#'
#' @description
#' `date_seq()` generates a date (Date) or date-time (POSIXct/POSIXlt) sequence.
#'
#' There are separate help pages for generating sequences for dates and
#' date-times:
#'
#' - [dates (Date)][date-sequence]
#'
#' - [date-times (POSIXct/POSIXlt)][posixt-sequence]
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param from `[Date(1) / POSIXct(1) / POSIXlt(1)]`
#'
#'   A date or date-time to start the sequence from.
#'
#'   `from` is always included in the result.
#'
#' @param to `[Date(1) / POSIXct(1) / POSIXlt(1) / NULL]`
#'
#'   A date or date-time to stop the sequence at.
#'
#'   `to` is only included in the result if the resulting sequence divides
#'   the distance between `from` and `to` exactly.
#'
#' @param by `[integer(1) / clock_duration(1) / NULL]`
#'
#'   The unit to increment the sequence by.
#'
#'   If `to < from`, then `by` must be positive.
#'
#'   If `to > from`, then `by` must be negative.
#'
#' @param total_size `[positive integer(1) / NULL]`
#'
#'   The size of the resulting sequence.
#'
#'   If specified alongside `to`, this must generate a non-fractional sequence
#'   between `from` and `to`.
#'
#' @return A date or date-time vector.
#'
#' @export
#' @examples
#' # See method specific documentation for more examples
#'
#' x <- as.Date("2019-01-01")
#' date_seq(x, by = duration_months(2), total_size = 20)
date_seq <- function(from,
                     ...,
                     to = NULL,
                     by = NULL,
                     total_size = NULL) {
  UseMethod("date_seq")
}

#' Sequences: date
#'
#' @description
#' This is a Date method for the [date_seq()] generic.
#'
#' `date_seq()` generates a date (Date) sequence.
#'
#' When calling `date_seq()`, exactly two of the following must be specified:
#' - `to`
#' - `by`
#' - `total_size`
#'
#' @inheritParams date_seq
#' @inheritParams invalid_resolve
#'
#' @param from `[Date(1)]`
#'
#'   A date to start the sequence from.
#'
#'   `from` is always included in the result.
#'
#' @param to `[Date(1) / NULL]`
#'
#'   A date to stop the sequence at.
#'
#'   `to` is only included in the result if the resulting sequence divides
#'   the distance between `from` and `to` exactly.
#'
#'   If `to` is supplied along with `by`, all components of `to` more precise
#'   than the precision of `by` must match `from` exactly. For example, if `by =
#'   duration_months(1)`, the day component of `to` must match the day component
#'   of `from`. This ensures that the generated sequence is, at a minimum, a
#'   weakly monotonic sequence of dates.
#'
#' @param by `[integer(1) / clock_duration(1) / NULL]`
#'
#'   The unit to increment the sequence by.
#'
#'   If `to < from`, then `by` must be positive.
#'
#'   If `to > from`, then `by` must be negative.
#'
#'   If `by` is an integer, it is equivalent to `duration_days(by)`.
#'
#'   If `by` is a duration, it is allowed to have a precision of:
#'   - year
#'   - quarter
#'   - month
#'   - week
#'   - day
#'
#' @return A date vector.
#'
#' @name date-sequence
#'
#' @export
#' @examples
#' from <- date_build(2019, 1)
#' to <- date_build(2019, 4)
#'
#' # Defaults to daily sequence
#' date_seq(from, to = to, by = 7)
#'
#' # Use durations to change to monthly or yearly sequences
#' date_seq(from, to = to, by = duration_months(1))
#' date_seq(from, by = duration_years(-2), total_size = 3)
#'
#' # Note that components of `to` more precise than the precision of `by`
#' # must match `from` exactly. For example, this is not well defined:
#' from <- date_build(2019, 5, 2)
#' to <- date_build(2025, 7, 5)
#' try(date_seq(from, to = to, by = duration_years(1)))
#'
#' # The month and day components of `to` must match `from`
#' to <- date_build(2025, 5, 2)
#' date_seq(from, to = to, by = duration_years(1))
#'
#' # ---------------------------------------------------------------------------
#'
#' # Invalid dates must be resolved with the `invalid` argument
#' from <- date_build(2019, 1, 31)
#' to <- date_build(2019, 12, 31)
#'
#' try(date_seq(from, to = to, by = duration_months(1)))
#' date_seq(from, to = to, by = duration_months(1), invalid = "previous")
#'
#' # Compare this to the base R result, which is often a source of confusion
#' seq(from, to = to, by = "1 month")
#'
#' # This is equivalent to the overflow invalid resolution strategy
#' date_seq(from, to = to, by = duration_months(1), invalid = "overflow")
#'
#' # ---------------------------------------------------------------------------
#'
#' # Usage of `to` and `total_size` must generate a non-fractional sequence
#' # between `from` and `to`
#' from <- date_build(2019, 1, 1)
#' to <- date_build(2019, 1, 4)
#'
#' # These are fine
#' date_seq(from, to = to, total_size = 2)
#' date_seq(from, to = to, total_size = 4)
#'
#' # But this is not!
#' try(date_seq(from, to = to, total_size = 3))
date_seq.Date <- function(from,
                          ...,
                          to = NULL,
                          by = NULL,
                          total_size = NULL,
                          invalid = NULL) {
  check_dots_empty()

  check_number_of_supplied_optional_arguments(to, by, total_size)

  if (!is_null(to) && !is_Date(to)) {
    abort("If supplied, `to` must be a <Date>.")
  }

  if (!is_null(total_size)) {
    total_size <- check_length_out(total_size, arg = "total_size")
  }

  if (is_null(by)) {
    precision <- "day"
  } else if (is_duration(by)) {
    precision <- duration_precision(by)
  } else {
    precision <- "day"
    by <- duration_helper(by, PRECISION_DAY, n_arg = "by")
  }

  precision_int <- validate_precision_string(precision)

  if (precision_int == PRECISION_QUARTER) {
    by <- duration_cast(by, "month")
    precision <- "month"
    precision_int <- PRECISION_MONTH
  }

  if (precision_int == PRECISION_WEEK) {
    by <- duration_cast(by, "day")
    precision <- "day"
    precision_int <- PRECISION_DAY
  }

  if (precision_int %in% c(PRECISION_YEAR, PRECISION_MONTH)) {
    out <- date_seq_year_month(from, to, by, total_size, precision)
    out <- invalid_resolve(out, invalid = invalid)
    out <- as.Date(out)
    return(out)
  }

  if (precision_int == PRECISION_DAY) {
    out <- date_seq_day(from, to, by, total_size, precision)
    out <- as.Date(out)
    return(out)
  }

  abort("`by` must have a precision of 'year', 'quarter', 'month', 'week', or 'day'.")
}

date_seq_year_month <- function(from, to, by, total_size, precision) {
  has_time <- is_POSIXt(from)

  from <- as_year_month_day(from)
  original_from <- from
  from <- calendar_narrow(from, precision)

  if (!is_null(to)) {
    to <- as_year_month_day(to)
    check_from_to_component_equivalence(original_from, to, precision, has_time)
    to <- calendar_narrow(to, precision)
  }

  out <- seq(from, to = to, by = by, length.out = total_size)
  out <- reset_original_components(out, original_from, precision, has_time)

  out
}

date_seq_day <- function(from, to, by, total_size, precision) {
  date_seq_day_hour_minute_second(from, to, by, total_size, precision, as_naive_time)
}
date_seq_hour_minute_second <- function(from, to, by, total_size, precision) {
  date_seq_day_hour_minute_second(from, to, by, total_size, precision, as_sys_time)
}
date_seq_day_hour_minute_second <- function(from, to, by, total_size, precision, as_time_point_fn) {
  has_time <- is_POSIXt(from)

  from <- as_time_point_fn(from)
  original_from <- from
  from <- time_point_floor(from, precision)

  if (!is_null(to)) {
    to <- as_time_point_fn(to)

    check_from_to_component_equivalence(
      from = as_year_month_day(original_from),
      to = as_year_month_day(to),
      precision = precision,
      has_time = has_time
    )

    to <- time_point_floor(to, precision)
  }

  out <- seq(from, to = to, by = by, length.out = total_size)

  original_time <- original_from - from
  out <- out + original_time

  out
}

check_from_to_component_equivalence <- function(from, to, precision, has_time) {
  ok <- TRUE
  precision_int <- validate_precision_string(precision)

  if (precision_int < PRECISION_MONTH) {
    ok <- ok && is_true(get_month(from) == get_month(to))
  }
  if (precision_int < PRECISION_DAY) {
    ok <- ok && is_true(get_day(from) == get_day(to))
  }

  if (has_time) {
    if (precision_int < PRECISION_HOUR) {
      ok <- ok && is_true(get_hour(from) == get_hour(to))
    }
    if (precision_int < PRECISION_MINUTE) {
      ok <- ok && is_true(get_minute(from) == get_minute(to))
    }
    if (precision_int < PRECISION_SECOND) {
      ok <- ok && is_true(get_second(from) == get_second(to))
    }
  }

  if (!ok) {
    message <- paste0(
      "All components of `from` and `to` more precise than ",
      "'", precision, "' ",
      "must match."
    )
    abort(message)
  }

  invisible()
}

reset_original_components <- function(out, from, precision, has_time) {
  precision_int <- validate_precision_string(precision)

  if (precision_int < PRECISION_MONTH) {
    out <- set_month(out, get_month(from))
  }
  if (precision_int < PRECISION_DAY) {
    out <- set_day(out, get_day(from))
  }

  if (has_time) {
    if (precision_int < PRECISION_HOUR) {
      out <- set_hour(out, get_hour(from))
    }
    if (precision_int < PRECISION_MINUTE) {
      out <- set_minute(out, get_minute(from))
    }
    if (precision_int < PRECISION_SECOND) {
      out <- set_second(out, get_second(from))
    }
  }

  out
}

check_number_of_supplied_optional_arguments <- function(to, by, total_size) {
  has_to <- !is_null(to)
  has_by <- !is_null(by)
  has_ts <- !is_null(total_size)

  n_has <- sum(has_to, has_by, has_ts)

  if (n_has != 2L) {
    message <- paste0(
      "Must specify exactly two of:\n",
      "- `to`\n",
      "- `by`\n",
      "- `total_size`"
    )
    abort(message)
  }

  invisible()
}
