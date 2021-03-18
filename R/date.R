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
#' Since R assumes that Dates are UTC, converting to a zoned-time returns
#' a zoned-time with a UTC time zone. There is no `zone` argument.
#'
#' @inheritParams ellipsis::dots_empty
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
#' as_zoned_time(x)
as_zoned_time.Date <- function(x, ...) {
  check_dots_empty()
  x <- as_sys_time(x)
  as_zoned_time(x, zone = "UTC")
}

#' @export
as_year_month_day.Date <- function(x) {
  as_year_month_day(as_sys_time(x))
}

#' @export
as_year_month_weekday.Date <- function(x) {
  as_year_month_weekday(as_sys_time(x))
}

#' @export
as_year_quarter_day.Date <- function(x, ..., start = NULL) {
  as_year_quarter_day(as_sys_time(x), ..., start = start)
}

#' @export
as_iso_year_week_day.Date <- function(x) {
  as_iso_year_week_day(as_sys_time(x))
}

#' @export
as_year_day.Date <- function(x) {
  as_year_day(as_sys_time(x))
}

#' @export
as_weekday.Date <- function(x) {
  as_weekday(as_sys_time(x))
}

# ------------------------------------------------------------------------------

# Not using `check_dots_empty()` because that might
# be too aggressive with base generics

#' @export
as.Date.clock_calendar <- function(x, ...) {
  as.Date(as_sys_time(x))
}

#' @export
as.Date.clock_time_point <- function(x, ...) {
  names <- clock_rcrd_names(x)
  x <- time_point_cast(x, "day")
  x <- field_ticks(x)
  x <- as.double(x)
  names(x) <- names
  new_date(x)
}

#' @export
as.Date.clock_zoned_time <- function(x, ...) {
  as.Date(as_sys_time(x))
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
  x <- as_sys_time(x)
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
#' `date_format()` formats a date (Date) or date-time (POSIXct/POSIXlt) using
#' a `format` string.
#'
#' @inheritParams format.clock_zoned_time
#'
#' @param x `[Date / POSIXct / POSIXlt]`
#'
#'   A date or date-time vector.
#'
#' @return A character vector of the formatted input.
#'
#' @export
#' @examples
#' x <- as.Date("2019-01-01")
#'
#' # Date objects are assumed to be UTC
#' date_format(x, format = "%Y-%m-%d %z %Z")
#'
#' x <- as.POSIXct(
#'   c("1970-04-26 01:30:00", "1970-04-26 03:30:00"),
#'   tz = "America/New_York"
#' )
#'
#' date_format(x, format = "%B %d, %Y %H:%M:%S")
#'
#' # By default, `%Z` uses the full zone name, but you can switch to the
#' # abbreviated name
#' date_format(x, format = "%z %Z")
#' date_format(x, format = "%z %Z", abbreviate_zone = TRUE)
date_format <- function(x,
                        ...,
                        format = NULL,
                        locale = clock_locale(),
                        abbreviate_zone = FALSE) {
  UseMethod("date_format")
}

#' @export
date_format.Date <- function(x,
                             ...,
                             format = NULL,
                             locale = clock_locale(),
                             abbreviate_zone = FALSE) {
  check_dots_empty()
  x <- as_zoned_time(x)
  format(x, format = format, locale = locale, abbreviate_zone = abbreviate_zone)
}

# ------------------------------------------------------------------------------

#' Get or set the time zone
#'
#' @description
#' - `date_zone()` gets the time zone.
#'
#' - `date_set_zone()` sets the time zone. This retains the _underlying
#' duration_, but changes the _printed time_ depending on the zone that is
#' chosen.
#'
#' Note that attempting to call `date_set_zone()` on a Date is an error, as R
#' assumes that Date objects are always UTC.
#'
#' @param x `[Date / POSIXct / POSIXlt]`
#'
#'   A date or date-time vector.
#'
#' @param zone `[character(1)]`
#'
#'   A valid time zone to switch to.
#'
#' @return
#' - `date_zone()` returns a string containing the time zone.
#'
#' - `date_set_zone()` returns `x` with an altered printed time. The
#' underlying duration is not changed.
#'
#' @name date-zone
#'
#' @examples
#' library(magrittr)
#'
#' x <- as.Date("2019-01-01")
#'
#' # Dates are always UTC
#' date_zone(x)
#'
#' # You can't change this!
#' try(date_set_zone(x, "America/New_York"))
#'
#' x <- as.POSIXct("2019-01-02 01:30:00", tz = "America/New_York")
#' x
#'
#' # If it is 1:30am in New York, what time is it in Los Angeles?
#' # Same underlying duration, new printed time
#' date_set_zone(x, "America/Los_Angeles")
#'
#' # If you want to retain the printed time, but change the underlying duration,
#' # convert to a naive-time to drop the time zone, then convert back to a
#' # date-time. Be aware that this requires that you handle daylight saving time
#' # irregularities with the `nonexistent` and `ambiguous` arguments to
#' # `as.POSIXct()`!
#' x %>%
#'   as_naive_time() %>%
#'   as.POSIXct("America/Los_Angeles")
#'
#' y <- as.POSIXct("2021-03-28 03:30:00", "America/New_York")
#' y
#'
#' y_nt <- as_naive_time(y)
#' y_nt
#'
#' # Helsinki had a daylight saving time gap where they jumped from
#' # 02:59:59 -> 04:00:00
#' try(as.POSIXct(y_nt, "Europe/Helsinki"))
#'
#' as.POSIXct(y_nt, "Europe/Helsinki", nonexistent = "roll-forward")
#' as.POSIXct(y_nt, "Europe/Helsinki", nonexistent = "roll-backward")
NULL

#' @rdname date-zone
#' @export
date_zone <- function(x) {
  UseMethod("date_zone")
}

#' @rdname date-zone
#' @export
date_set_zone <- function(x, zone) {
  UseMethod("date_set_zone")
}


#' @export
date_zone.Date <- function(x) {
  "UTC"
}

#' @export
date_set_zone.Date <- function(x, zone) {
  abort("'Date' objects are required to be UTC.")
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
#' If the `%z` command is used, then the date-time string is interpreted as
#' a naive-time, which is then shifted by the UTC offset found in `%z`. The
#' returned Date can then validly be interpreted as UTC. Remember that in R,
#' Date objects are assumed to be UTC, similar to a sys-time.
#'
#' _`date_parse()` ignores the `%Z` command._
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
date_parse <- function(x, ..., format = NULL, locale = clock_locale()) {
  x <- sys_time_parse(x, ..., format = format, precision = "day", locale = locale)
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
