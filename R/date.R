#' @export
as_sys_time.Date <- function(x) {
  names <- names(x)
  x <- unstructure(x)
  x <- duration_days(x)
  new_sys_time_from_fields(x, PRECISION_DAY, names)
}

#' @export
as_naive_time.Date <- function(x) {
  as_naive_time(as_sys_time(x))
}

#' Convert to a zoned-time from a date
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
#' These functions are supported getters for a Date vector.
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

#' @export
zoned_zone.Date <- function(x) {
  "UTC"
}

#' @export
zoned_set_zone.Date <- function(x, zone) {
  abort("'Date' objects are required to be UTC.")
}

#' @export
zoned_info.Date <- function(x) {
  x <- as_zoned_time(x)
  zoned_info(x)
}

# ------------------------------------------------------------------------------

#' Setters: date
#'
#' @description
#' These functions are supported setters for a Date vector.
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
set_year.Date <- function(x, value, ..., invalid = "error") {
  set_date_field_year_month_day(x, value, invalid, set_year, ...)
}
#' @rdname Date-setters
#' @export
set_month.Date <- function(x, value, ..., invalid = "error") {
  set_date_field_year_month_day(x, value, invalid, set_month, ...)
}
#' @rdname Date-setters
#' @export
set_day.Date <- function(x, value, ..., invalid = "error") {
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

#' Arithmetic: date
#'
#' @description
#' The following arithmetic operations are available for use on R's native
#' Date type.
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
add_years.Date <- function(x, n, ..., invalid = "error") {
  add_date_duration_year_month_day(x, n, invalid, add_years, ...)
}
#' @rdname Date-arithmetic
#' @export
add_quarters.Date <- function(x, n, ..., invalid = "error") {
  add_date_duration_year_month_day(x, n, invalid, add_quarters, ...)
}
#' @rdname Date-arithmetic
#' @export
add_months.Date <- function(x, n, ..., invalid = "error") {
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
