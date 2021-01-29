#' @export
as_sys_time.POSIXt <- function(x) {
  # The sys_time that would give the equivalent zoned time when a tz is attached
  as_sys_time(as_zoned_time(x))
}

#' @export
as_naive_time.POSIXt <- function(x) {
  as_naive_time(as_zoned_time(x))
}

#' Convert to a zoned-time from a date-time
#'
#' Converting from one of R's native date-time classes (POSIXct or POSIXlt)
#' will retain the time zone of that object. There is no `zone` argument.
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param x `[POSIXct / POSIXlt]`
#'
#'   A date-time.
#'
#' @return A zoned-time.
#'
#' @name as-zoned-time-posixt
#' @export
#' @examples
#' x <- as.POSIXct("2019-01-01", tz = "America/New_York")
#' as_zoned_time(x)
as_zoned_time.POSIXt <- function(x, ...) {
  check_dots_empty()

  x <- to_posixct(x)

  names <- names(x)
  zone <- posixt_tzone(x)

  fields <- to_sys_duration_fields_from_sys_seconds_cpp(x)

  new_zoned_time_from_fields(fields, PRECISION_SECOND, zone, names)
}

#' @export
as_year_month_day.POSIXt <- function(x) {
  # Assumes zoned -> naive -> calendar is what the user expects
  x <- as_naive_time(x)
  as_year_month_day(x)
}

#' @export
as_year_month_weekday.POSIXt <- function(x) {
  # Assumes zoned -> naive -> calendar is what the user expects
  x <- as_naive_time(x)
  as_year_month_weekday(x)
}

#' @export
as_year_quarter_day.POSIXt <- function(x, ..., start = NULL) {
  # Assumes zoned -> naive -> calendar is what the user expects
  x <- as_naive_time(x)
  as_year_quarter_day(x, ..., start = start)
}

#' @export
as_iso_year_week_day.POSIXt <- function(x) {
  # Assumes zoned -> naive -> calendar is what the user expects
  x <- as_naive_time(x)
  as_iso_year_week_day(x)
}

# ------------------------------------------------------------------------------

# Not using `check_dots_empty()` because that might
# be too aggressive with base generics. Also not passing `...` on to methods
# that do check empty dots.

# Using `tz = ""` to be compatible with the generic of `as.POSIXct()`

#' @export
as.POSIXct.clock_calendar <- function(x,
                                      tz = "",
                                      ...,
                                      nonexistent = "error",
                                      ambiguous = "error") {
  x <- as_naive_time(x)
  as.POSIXct(x, tz = tz, nonexistent = nonexistent, ambiguous = ambiguous)
}

#' @export
as.POSIXct.clock_sys_time <- function(x, tz = "", ...) {
  zone <- zone_standardize(tz)
  x <- time_point_cast(x, "second")
  seconds <- to_sys_seconds_from_sys_duration_fields_cpp(x)
  names(seconds) <- clock_rcrd_names(x)
  new_datetime(seconds, zone)
}

#' @export
as.POSIXct.clock_naive_time <- function(x,
                                        tz = "",
                                        ...,
                                        nonexistent = "error",
                                        ambiguous = "error") {
  x <- as_zoned_time(x, zone = tz, nonexistent = nonexistent, ambiguous = ambiguous)
  as.POSIXct(x)
}

#' @export
as.POSIXct.clock_zoned_time <- function(x, ...) {
  zone <- zoned_time_zone(x)
  x <- as_sys_time(x)
  as.POSIXct(x, tz = zone)
}

# ------------------------------------------------------------------------------

#' @export
as.POSIXlt.clock_calendar <- function(x,
                                      tz = "",
                                      ...,
                                      nonexistent = "error",
                                      ambiguous = "error") {
  x <- as.POSIXct(x, tz = tz, ..., nonexistent = nonexistent, ambiguous = ambiguous)
  as.POSIXlt(x)
}

#' @export
as.POSIXlt.clock_sys_time <- function(x, tz = "", ...) {
  x <- as.POSIXct(x, tz = tz, ...)
  as.POSIXlt(x)
}

#' @export
as.POSIXlt.clock_naive_time <- as.POSIXlt.clock_calendar

#' @export
as.POSIXlt.clock_zoned_time <- function(x, ...) {
  x <- as.POSIXct(x, ...)
  as.POSIXlt(x)
}

# ------------------------------------------------------------------------------

#' Getters: date-time
#'
#' @description
#' These functions are supported getters for date-time vectors, like POSIXct
#' and POSIXlt.
#'
#' - `get_year()` returns the Gregorian year.
#'
#' - `get_month()` returns the month of the year.
#'
#' - `get_day()` returns the day of the month.
#'
#' - There are sub-daily getters for extracting more precise components, up to
#'   a precision of seconds.
#'
#' For more advanced component extraction, convert to the calendar type
#' that you are interested in.
#'
#' @param x `[POSIXct / POSIXlt]`
#'
#'   A date-time type to get the component from.
#'
#' @return The component.
#'
#' @name posixt-getters
#' @examples
#' x <- as.POSIXct("2019-01-01", tz = "America/New_York")
#'
#' x <- add_days(x, 0:5)
#' x <- set_second(x, 10:15)
#'
#' get_day(x)
#' get_second(x)
NULL

#' @rdname posixt-getters
#' @export
get_year.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_year)
}
#' @rdname posixt-getters
#' @export
get_month.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_month)
}
#' @rdname posixt-getters
#' @export
get_day.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_day)
}
#' @rdname posixt-getters
#' @export
get_hour.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_hour)
}
#' @rdname posixt-getters
#' @export
get_minute.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_minute)
}
#' @rdname posixt-getters
#' @export
get_second.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_second)
}
get_posixt_field_year_month_day <- function(x, get_fn) {
  x <- as_year_month_day(x)
  get_fn(x)
}

# ------------------------------------------------------------------------------

#' @export
zoned_zone.POSIXt <- function(x) {
  zone_standardize(posixt_tzone(x))
}

#' @export
zoned_set_zone.POSIXt <- function(x, zone) {
  x <- to_posixct(x)
  zone <- zone_validate(zone)
  posixt_set_tzone(x, zone)
}

#' @export
zoned_info.POSIXt <- function(x) {
  x <- as_zoned_time(x)
  zoned_info(x)
}

# ------------------------------------------------------------------------------

#' Setters: date-time
#'
#' @description
#' These functions are supported setters for date-time vectors, like POSIXct
#' and POSIXlt.
#'
#' - `set_year()` sets the year.
#'
#' - `set_month()` sets the month of the year. Valid values are in the range
#'   of `[1, 12]`.
#'
#' - `set_day()` sets the day of the month. Valid values are in the range
#'   of `[1, 31]`.
#'
#' - There are sub-daily setters for setting more precise components, up to
#'   a precision of seconds.
#'
#' @inheritParams ellipsis::dots_empty
#' @inheritParams invalid_resolve
#' @inheritParams as-zoned-time-naive-time
#'
#' @param x `[POSIXct / POSIXlt]`
#'
#'   A date-time vector.
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
#' @name posixt-setters
#' @examples
#' x <- as.POSIXct("2019-02-01", tz = "America/New_York")
#'
#' # Set the day
#' set_day(x, 12:14)
#'
#' # Set to the "last" day of the month
#' set_day(x, "last")
#'
#' # You cannot set a date-time to an invalid date like you can with
#' # a year-month-day. Instead, the default strategy is to error.
#' try(set_day(x, 31))
#' set_day(as_year_month_day(x), 31)
#'
#' # You can resolve these issues while setting the day by specifying
#' # an invalid date resolution strategy with `invalid`
#' set_day(x, 31, invalid = "previous")
#'
#' y <- as.POSIXct("2020-03-08 01:30:00", tz = "America/New_York")
#'
#' # Nonexistent and ambiguous times must be resolved immediately when
#' # working with R's native date-time types. An error is thrown by default.
#' try(set_hour(y, 2))
#' set_hour(y, 2, nonexistent = "roll-forward")
#' set_hour(y, 2, nonexistent = "roll-backward")
NULL

#' @rdname posixt-setters
#' @export
set_year.POSIXt <- function(x, value, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  set_posixt_field_year_month_day(x, value, invalid, nonexistent, ambiguous, set_year, ...)
}
#' @rdname posixt-setters
#' @export
set_month.POSIXt <- function(x, value, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  set_posixt_field_year_month_day(x, value, invalid, nonexistent, ambiguous, set_month, ...)
}
#' @rdname posixt-setters
#' @export
set_day.POSIXt <- function(x, value, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  set_posixt_field_year_month_day(x, value, invalid, nonexistent, ambiguous, set_day, ...)
}
#' @rdname posixt-setters
#' @export
set_hour.POSIXt <- function(x, value, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  set_posixt_field_year_month_day(x, value, invalid, nonexistent, ambiguous, set_hour, ...)
}
#' @rdname posixt-setters
#' @export
set_minute.POSIXt <- function(x, value, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  set_posixt_field_year_month_day(x, value, invalid, nonexistent, ambiguous, set_minute, ...)
}
#' @rdname posixt-setters
#' @export
set_second.POSIXt <- function(x, value, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  set_posixt_field_year_month_day(x, value, invalid, nonexistent, ambiguous, set_second, ...)
}
set_posixt_field_year_month_day <- function(x, value, invalid, nonexistent, ambiguous, set_fn, ...) {
  check_dots_empty()
  zone <- posixt_tzone(x)
  x <- as_year_month_day(x)
  x <- set_fn(x, value)
  x <- invalid_resolve(x, invalid = invalid)
  as.POSIXct(x, tz = zone, nonexistent = nonexistent, ambiguous = ambiguous)
}

# ------------------------------------------------------------------------------

#' Arithmetic: date-time
#'
#' @description
#' The following arithmetic operations are available for use on R's native
#' date-time types, POSIXct and POSIXlt.
#'
#' Calendrical based arithmetic:
#'
#' These functions convert to a naive-time, then to a year-month-day, perform
#' the arithmetic, then convert back to a date-time.
#'
#' - `add_years()`
#'
#' - `add_quarters()`
#'
#' - `add_months()`
#'
#' Naive-time based arithmetic:
#'
#' These functions convert to a naive-time, perform the arithmetic, then
#' convert back to a date-time.
#'
#' - `add_weeks()`
#'
#' - `add_days()`
#'
#' Sys-time based arithmetic:
#'
#' These functions convert to a sys-time, perform the arithmetic, then
#' convert back to a date-time.
#'
#' - `add_hours()`
#'
#' - `add_minutes()`
#'
#' - `add_seconds()`
#'
#' @details
#' Adding a single quarter with `add_quarters()` is equivalent to adding
#' 3 months.
#'
#' `x` and `n` are recycled against each other.
#'
#' Calendrical based arithmetic has the potential to generate invalid dates
#' (like the 31st of February), nonexistent times (due to daylight saving
#' time gaps), and ambiguous times (due to daylight saving time fallbacks).
#'
#' Naive-time based arithmetic will never generate an invalid date, but
#' may generate a nonexistent or ambiguous time (i.e. you added 1 day and
#' landed in a daylight saving time gap).
#'
#' Sys-time based arithmetic operates in the UTC time zone, which means
#' that it will never generate any invalid dates or nonexistent / ambiguous
#' times.
#'
#' The conversion from POSIXct/POSIXlt to the corresponding clock type uses
#' a "best guess" about whether you want to do the arithmetic using a naive-time
#' or a sys-time. For example, when adding months, you probably want to
#' retain the printed time when converting to a year-month-day to perform
#' the arithmetic, so the conversion goes through naive-time. However,
#' when adding smaller units like seconds, you probably want
#' `"2020-03-08 01:59:59" + 1 second` in the America/New_York time zone to
#' return `"2020-03-08 03:00:00"`, taking into account the fact that there
#' was a daylight saving time gap. This requires doing the arithmetic in
#' sys-time, so that is what clock converts to. If you disagree with this
#' heuristic for any reason, you can take control and perform the conversions
#' yourself. For example, you could convert the previous example to a
#' naive-time instead of a sys-time manually with [as_naive_time()], add
#' 1 second giving `"2020-03-08 02:00:00"`, then convert back to a
#' POSIXct/POSIXlt, dealing with the nonexistent time that get's created by
#' using the `nonexistent` argument of `as.POSIXct()`.
#'
#' @inheritParams add_years
#' @inheritParams invalid_resolve
#' @inheritParams as-zoned-time-naive-time
#'
#' @param x `[POSIXct / POSIXlt]`
#'
#'   A date-time vector.
#'
#' @return `x` after performing the arithmetic.
#'
#' @name posixt-arithmetic
#'
#' @examples
#' x <- as.POSIXct("2019-01-01", tz = "America/New_York")
#'
#' add_years(x, 1:5)
#'
#' y <- as.POSIXct("2019-01-31 00:30:00", tz = "America/New_York")
#'
#' # Adding 1 month to `y` generates an invalid date. Unlike year-month-day
#' # types, R's native date-time types cannot handle invalid dates, so you must
#' # resolve them immediately. If you don't you get an error:
#' try(add_months(y, 1:2))
#' add_months(as_year_month_day(y), 1:2)
#'
#' # Resolve invalid dates by specifying an invalid date resolution strategy
#' # with the `invalid` argument. Using `"previous"` here sets the date-time to
#' # the previous valid moment in time - i.e. the end of the month. The
#' # time is set to the last moment in the day to retain the relative ordering
#' # within your input. If you are okay with potentially losing this, and
#' # want to retain your time of day, you can use `"previous-day"` to set the
#' # date-time to the previous valid day, while keeping the time of day.
#' add_months(y, 1:2, invalid = "previous")
#' add_months(y, 1:2, invalid = "previous-day")
NULL

#' @rdname posixt-arithmetic
#' @export
add_years.POSIXt <- function(x, n, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  add_posixt_duration_year_month_day(x, n, invalid, nonexistent, ambiguous, add_years, ...)
}
#' @rdname posixt-arithmetic
#' @export
add_quarters.POSIXt <- function(x, n, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  add_posixt_duration_year_month_day(x, n, invalid, nonexistent, ambiguous, add_quarters, ...)
}
#' @rdname posixt-arithmetic
#' @export
add_months.POSIXt <- function(x, n, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  add_posixt_duration_year_month_day(x, n, invalid, nonexistent, ambiguous, add_months, ...)
}
add_posixt_duration_year_month_day <- function(x, n, invalid, nonexistent, ambiguous, add_fn, ...) {
  check_dots_empty()
  zone <- posixt_tzone(x)
  x <- as_year_month_day(x)
  x <- add_fn(x, n)
  x <- invalid_resolve(x, invalid = invalid)
  as.POSIXct(x, tz = zone, nonexistent = nonexistent, ambiguous = ambiguous)
}

#' @rdname posixt-arithmetic
#' @export
add_weeks.POSIXt <- function(x, n, ..., nonexistent = "error", ambiguous = "error") {
  add_posixt_duration_naive_time_point(x, n, nonexistent, ambiguous, add_weeks, ...)
}
#' @rdname posixt-arithmetic
#' @export
add_days.POSIXt <- function(x, n, ..., nonexistent = "error", ambiguous = "error") {
  add_posixt_duration_naive_time_point(x, n, nonexistent, ambiguous, add_days, ...)
}
add_posixt_duration_naive_time_point <- function(x, n, nonexistent, ambiguous, add_fn, ...) {
  check_dots_empty()
  zone <- posixt_tzone(x)
  x <- as_naive_time(x)
  x <- add_fn(x, n)
  as.POSIXct(x, tz = zone, nonexistent = nonexistent, ambiguous = ambiguous)
}

#' @rdname posixt-arithmetic
#' @export
add_hours.POSIXt <- function(x, n, ...) {
  add_posixt_duration_sys_time_point(x, n, add_hours, ...)
}
#' @rdname posixt-arithmetic
#' @export
add_minutes.POSIXt <- function(x, n, ...) {
  add_posixt_duration_sys_time_point(x, n, add_minutes, ...)
}
#' @rdname posixt-arithmetic
#' @export
add_seconds.POSIXt <- function(x, n, ...) {
  add_posixt_duration_sys_time_point(x, n, add_seconds, ...)
}
add_posixt_duration_sys_time_point <- function(x, n, add_fn, ...) {
  check_dots_empty()
  zone <- posixt_tzone(x)
  x <- as_sys_time(x)
  x <- add_fn(x, n)
  as.POSIXct(x, tz = zone)
}
