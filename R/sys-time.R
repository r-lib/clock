new_sys_time_from_fields <- function(fields, precision, names) {
  new_time_point_from_fields(fields, precision, CLOCK_SYS, names)
}

# ------------------------------------------------------------------------------

sys_days <- function(n = integer()) {
  names <- NULL
  duration <- duration_days(n)
  new_sys_time_from_fields(duration, PRECISION_DAY, names)
}

sys_seconds <- function(n = integer()) {
  names <- NULL
  duration <- duration_seconds(n)
  new_sys_time_from_fields(duration, PRECISION_SECOND, names)
}

# ------------------------------------------------------------------------------

#' Is `x` a sys-time?
#'
#' This function determines if the input is a sys-time object.
#'
#' @param x `[object]`
#'
#'   An object.
#'
#' @return `TRUE` if `x` inherits from `"clock_sys_time"`, otherwise `FALSE`.
#'
#' @export
#' @examples
#' is_sys_time(1)
#' is_sys_time(as_sys_time(duration_days(1)))
is_sys_time <- function(x) {
  inherits(x, "clock_sys_time")
}

# ------------------------------------------------------------------------------

sys_parse <- function(x,
                      ...,
                      format = NULL,
                      precision = "second",
                      locale = clock_locale()) {
  precision <- validate_time_point_precision(precision)

  fields <- time_point_parse(
    x = x,
    ...,
    format = format,
    precision = precision,
    locale = locale,
    clock = CLOCK_SYS
  )

  new_sys_time_from_fields(fields, precision, names(x))
}

# ------------------------------------------------------------------------------

#' Convert to a sys-time
#'
#' @description
#' `as_sys_time()` converts `x` to a sys-time.
#'
#' You can convert to a sys-time from any calendar type, as long as it has
#' at least day precision. There also must not be any invalid dates. If invalid
#' dates exist, they must first be resolved with [invalid_resolve()].
#'
#' Converting to a sys-time from a naive-time retains the printed time,
#' but adds an assumption that the time should be interpreted in the UTC time
#' zone.
#'
#' Converting to a sys-time from a zoned-time retains the underlying duration,
#' but the printed time is the equivalent UTC time to whatever the zoned-time's
#' zone happened to be.
#'
#' Converting to a sys-time from a duration just wraps the duration in a
#' sys-time object, adding the assumption that the time should be interpreted
#' in the UTC time zone. The duration must have at least day precision.
#'
#' There are convenience methods for converting to a sys-time from R's
#' native date and date-time types. Like converting from a zoned-time, these
#' retain the underlying duration, but will change the printed time if the
#' zone was not already UTC.
#'
#' @param x `[object]`
#'
#'   An object to convert to a sys-time.
#'
#' @return A sys-time vector.
#'
#' @export
#' @examples
#' x <- as.Date("2019-01-01")
#'
#' # Dates are assumed to be UTC, so the printed time is the same
#' as_sys_time(x)
#'
#' y <- as.POSIXct("2019-01-01 01:00:00", tz = "America/New_York")
#'
#' # The sys time displays the equivalent time in UTC (5 hours ahead of
#' # America/New_York at this point in the year)
#' as_sys_time(y)
#'
#' ym <- year_month_day(2019, 02)
#'
#' # A minimum of day precision is required
#' try(as_sys_time(ym))
#'
#' ymd <- set_day(ym, 10)
#' as_sys_time(ymd)
as_sys_time <- function(x) {
  UseMethod("as_sys_time")
}

#' @export
as_sys_time.clock_sys_time <- function(x) {
  x
}

#' @export
as_sys_time.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("as_sys_time")
}

# ------------------------------------------------------------------------------

#' @export
as_naive_time.clock_sys_time <- function(x) {
  new_naive_time_from_fields(x, time_point_precision(x), clock_rcrd_names(x))
}

#' Convert to a zoned-time from a sys-time
#'
#' @description
#' This is a sys-time method for the [as_zoned()] generic.
#'
#' Converting to a zoned-time from a sys-time retains the underlying duration,
#' but changes the printed time, depending on the `zone` that you choose.
#' Remember that sys-times are interpreted as UTC.
#'
#' If you want to retain the printed time, try converting to a zoned-time
#' [from a naive-time][as-zoned-time-naive-time], which is a time point
#' with a yet-to-be-determined time zone.
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param x `[clock_sys_time]`
#'
#'   A sys-time to convert to a zoned-time.
#'
#' @param zone `[character(1)]`
#'
#'   The zone to convert to.
#'
#' @return A zoned-time vector.
#'
#' @name as-zoned-time-sys-time
#' @export
#' @examples
#' x <- as_sys_time(year_month_day(2019, 02, 01, 02, 30, 00))
#' x
#'
#' # Since sys-time is interpreted as UTC, converting to a zoned-time with
#' # a zone of UTC retains the printed time
#' x_utc <- as_zoned(x, "UTC")
#' x_utc
#'
#' # Converting to a different zone results in a different printed time,
#' # which corresponds to the exact same point in time, just in a different
#' # part of the work
#' x_ny <- as_zoned(x, "America/New_York")
#' x_ny
as_zoned.clock_sys_time <- function(x, zone, ...) {
  zone <- zone_validate(zone)

  # Promote to at least seconds precision for `zoned_time`
  x <- vec_cast(x, vec_ptype2(x, sys_seconds()))

  precision <- time_point_precision(x)
  names <- clock_rcrd_names(x)

  new_zoned_time_from_fields(x, precision, zone, names)
}

# ------------------------------------------------------------------------------

#' What is the current sys-time?
#'
#' @description
#' `sys_now()` returns the current time in UTC.
#'
#' @details
#' The time is returned with a nanosecond precision, but the actual amount
#' of data returned is OS dependent. Usually, information at at least the
#' microsecond level is returned, with some platforms returning nanosecond
#' information.
#'
#' @return A sys-time of the current time in UTC.
#'
#' @export
#' @examples
#' x <- sys_now()
sys_now <- function() {
  names <- NULL
  fields <- sys_now_cpp()
  new_sys_time_from_fields(fields, PRECISION_NANOSECOND, names)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype2.clock_sys_time.clock_sys_time <- function(x, y, ...) {
  ptype2_time_point_and_time_point(x, y, ...)
}

#' @export
vec_cast.clock_sys_time.clock_sys_time <- function(x, to, ...) {
  cast_time_point_to_time_point(x, to, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arith
#' @method vec_arith clock_sys_time
#' @export
vec_arith.clock_sys_time <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_sys_time", y)
}

#' @method vec_arith.clock_sys_time MISSING
#' @export
vec_arith.clock_sys_time.MISSING <- function(op, x, y, ...) {
  arith_time_point_and_missing(op, x, y, ...)
}

#' @method vec_arith.clock_sys_time clock_sys_time
#' @export
vec_arith.clock_sys_time.clock_sys_time <- function(op, x, y, ...) {
  arith_time_point_and_time_point(op, x, y, ...)
}

#' @method vec_arith.clock_sys_time clock_duration
#' @export
vec_arith.clock_sys_time.clock_duration <- function(op, x, y, ...) {
  arith_time_point_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration clock_sys_time
#' @export
vec_arith.clock_duration.clock_sys_time <- function(op, x, y, ...) {
  arith_duration_and_time_point(op, x, y, ...)
}

#' @method vec_arith.clock_sys_time numeric
#' @export
vec_arith.clock_sys_time.numeric <- function(op, x, y, ...) {
  arith_time_point_and_numeric(op, x, y, ...)
}

#' @method vec_arith.numeric clock_sys_time
#' @export
vec_arith.numeric.clock_sys_time <- function(op, x, y, ...) {
  arith_numeric_and_time_point(op, x, y, ...)
}

