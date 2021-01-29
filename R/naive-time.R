new_naive_time_from_fields <- function(fields, precision, names) {
  new_time_point_from_fields(fields, precision, CLOCK_NAIVE, names)
}

#' @export
naive_days <- function(n = integer()) {
  names <- NULL
  duration <- duration_days(n)
  new_naive_time_from_fields(duration, PRECISION_DAY, names)
}

#' @export
naive_seconds <- function(n = integer()) {
  names <- NULL
  duration <- duration_seconds(n)
  new_naive_time_from_fields(duration, PRECISION_SECOND, names)
}

# ------------------------------------------------------------------------------

#' @export
is_naive_time <- function(x) {
  inherits(x, "clock_naive_time")
}

# ------------------------------------------------------------------------------

#' Convert to a naive-time
#'
#' @description
#' `as_naive_time()` converts `x` to a naive-time.
#'
#' You can convert to a naive-time from any calendar type, as long as it has
#' at least day precision. There also must not be any invalid dates. If invalid
#' dates exist, they must first be resolved with [invalid_resolve()].
#'
#' Converting to a naive-time from a sys-time or zoned-time retains the printed
#' time, but drops the assumption that the time should be interpreted with any
#' specific time zone.
#'
#' Converting to a naive-time from a duration just wraps the duration in a
#' naive-time object, there is no assumption about the time zone. The duration
#' must have at least day precision.
#'
#' There are convenience methods for converting to a naive-time from R's
#' native date and date-time types. Like converting from a zoned-time, these
#' retain the printed time.
#'
#' @param x `[object]`
#'
#'   An object to convert to a naive-time.
#'
#' @return A naive-time vector.
#'
#' @export
#' @examples
#' x <- as.Date("2019-01-01")
#' as_naive_time(x)
#'
#' ym <- year_month_day(2019, 02)
#'
#' # A minimum of day precision is required
#' try(as_naive_time(ym))
#'
#' ymd <- set_day(ym, 10)
#' as_naive_time(ymd)
as_naive_time <- function(x) {
  UseMethod("as_naive_time")
}

#' @export
as_naive_time.clock_naive_time <- function(x) {
  x
}

#' @export
as_naive_time.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("as_naive_time")
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_naive_time <- function(x) {
  new_sys_time_from_fields(x, time_point_precision(x), clock_rcrd_names(x))
}

#' Convert to a zoned-time from a naive-time
#'
#' @description
#' Converting to a zoned-time from a naive-time retains the printed time,
#' but changes the underlying duration, depending on the `zone` that you choose.
#'
#' Naive-times are time points with a yet-to-be-determined time zone. By
#' converting them to a zoned-time, all you are doing is specifying that
#' time zone while attempting to keep all other printed information the
#' same (if possible).
#'
#' If you want to retain the underlying duration, try converting to a zoned-time
#' [from a sys-time][as-zoned-time-sys-time], which is a time point
#' interpreted as having a UTC time zone.
#'
#' @section Daylight Saving Time:
#'
#' Converting from a naive-time to a zoned-time is not always possible due to
#' daylight saving time issues. There are two types of these issues:
#'
#' _Nonexistent_ times are the result of daylight saving time "gaps".
#' For example, in the America/New_York time zone, there was a daylight
#' saving time gap 1 second after `"2020-03-08 01:59:59"`, where the clocks
#' changed from `01:59:59 -> 03:00:00`, completely skipping the 2 o'clock hour.
#' This means that if you had a naive time of `"2020-03-08 02:30:00"`, you
#' couldn't convert that straight into a zoned-time with this time zone. To
#' resolve these issues, the `nonexistent` argument can be used to specify
#' one of many nonexistent time resolution strategies.
#'
#' _Ambiguous_ times are the result of daylight saving time "fallbacks".
#' For example, in the America/New_York time zone, there was a daylight
#' saving time fallback 1 second after `"2020-11-01 01:59:59 EDT"`, at which
#' point the clocks "fell backwards" by 1 hour, resulting in a printed time of
#' `"2020-11-01 01:00:00 EST"` (note the EDT->EST shift). This resulted in two
#' 1 o'clock hours for this day, so if you had a naive time of
#' `"2020-11-01 01:30:00"`, you wouldn't be able to convert that directly
#' into a zoned-time with this time zone, as there is no way for clock to know
#' which of the two ambiguous times you wanted. To resolve these issues,
#' the `ambiguous` argument can be used to specify one of many ambiguous
#' time resolution strategies.
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param x `[clock_naive_time]`
#'
#'   A naive-time to convert to a zoned-time.
#'
#' @param zone `[character(1)]`
#'
#'   The zone to convert to.
#'
#' @param nonexistent `[character]`
#'
#'   One of the following nonexistent time resolution strategies:
#'
#'   - `"roll-forward"`: The next valid instant in time.
#'
#'   - `"roll-backward"`: The previous valid instant in time.
#'
#'   - `"shift-forward"`: Shift the nonexistent time forward by the size of
#'     the daylight saving time gap.
#'
#'   - `"shift-backward`: Shift the nonexistent time backward by the size of
#'     the daylight saving time gap.
#'
#'   - `"NA"`: Replace nonexistent times with `NA`.
#'
#'   - `"error"`: Error on nonexistent times.
#'
#'   Allowed to be either length 1, or the same length as the input.
#'
#'   Using either `"roll-forward"` or `"roll-backward"` is generally
#'   recommended over shifting, as these two strategies maintain the
#'   _relative ordering_ between elements of the input.
#'
#' @param ambiguous `[character]`
#'
#'   One of the following ambiguous time resolution strategies:
#'
#'   - `"earliest"`: Of the two possible times, choose the earliest one.
#'
#'   - `"latest"`: Of the two possible times, choose the latest one.
#'
#'   - `"NA"`: Replace ambiguous times with `NA`.
#'
#'   - `"error"`: Error on ambiguous times.
#'
#'   Allowed to be either length 1, or the same length as the input.
#'
#' @return A zoned-time vector.
#'
#' @name as-zoned-time-naive-time
#' @export
#' @examples
#' x <- as_naive_time(year_month_day(2019, 1, 1))
#'
#' # Converting a naive-time to a zoned-time generally retains the
#' # printed time, while changing the underlying duration.
#' as_zoned_time(x, "America/New_York")
#' as_zoned_time(x, "America/Los_Angeles")
#'
#' # ---------------------------------------------------------------------------
#' # Nonexistent time:
#'
#' new_york <- "America/New_York"
#'
#' # There was a daylight saving gap in the America/New_York time zone on
#' # 2020-03-08 01:59:59 -> 03:00:00, which means that one of these
#' # naive-times don't exist in that time zone. By default, attempting to
#' # convert it to a zoned time will result in an error.
#' nonexistent_time <- year_month_day(2020, 03, 08, c(02, 03), c(45, 30), 00)
#' nonexistent_time <- as_naive_time(nonexistent_time)
#' try(as_zoned_time(nonexistent_time, new_york))
#'
#' # Resolve this by specifying a nonexistent time resolution strategy
#' as_zoned_time(nonexistent_time, new_york, nonexistent = "roll-forward")
#' as_zoned_time(nonexistent_time, new_york, nonexistent = "roll-backward")
#'
#' # Note that rolling backwards will choose the last possible moment in
#' # time at the current precision of the input
#' nonexistent_nanotime <- time_point_cast(nonexistent_time, "nanosecond")
#' nonexistent_nanotime
#' as_zoned_time(nonexistent_nanotime, new_york, nonexistent = "roll-backward")
#'
#' # A word of caution - Shifting does not guarantee that the relative ordering
#' # of the input is maintained
#' shifted <- as_zoned_time(nonexistent_time, new_york, nonexistent = "shift-forward")
#' shifted
#' # 02:45:00 < 03:30:00
#' nonexistent_time[1] < nonexistent_time[2]
#' # 03:45:00 > 03:30:00 (relative ordering is lost)
#' shifted[1] < shifted[2]
#'
#' # ---------------------------------------------------------------------------
#' # Ambiguous time:
#'
#' new_york <- "America/New_York"
#'
#' # There was a daylight saving time fallback in the America/New_York time
#' # zone on 2020-11-01 01:59:59 EDT -> 2020-11-01 01:00:00 EST, resulting
#' # in two 1 o'clock hours. This means that the following naive time is
#' # ambiguous since we don't know which of the two 1 o'clocks it belongs to.
#' # By default, attempting to convert it to a zoned time will result in an
#' # error.
#' ambiguous_time <- year_month_day(2020, 11, 01, 01, 30, 00)
#' ambiguous_time <- as_naive_time(ambiguous_time)
#' try(as_zoned_time(ambiguous_time, new_york))
#'
#' # Resolve this by specifying an ambiguous time resolution strategy
#' as_zoned_time(ambiguous_time, new_york, ambiguous = "earliest")
#' as_zoned_time(ambiguous_time, new_york, ambiguous = "latest")
#' as_zoned_time(ambiguous_time, new_york, ambiguous = "NA")
as_zoned_time.clock_naive_time <- function(x,
                                           zone,
                                           ...,
                                           nonexistent = "error",
                                           ambiguous = "error") {
  # `nonexistent` and `ambiguous` are allowed to be
  # size 1 or the same size as `x`
  size <- vec_size(x)
  validate_nonexistent(nonexistent, size)
  validate_ambiguous(nonexistent, size)

  # Promote to at least seconds precision for `zoned_time`
  x <- vec_cast(x, vec_ptype2(x, naive_seconds()))

  zone <- zone_validate(zone)
  precision <- time_point_precision(x)
  names <- clock_rcrd_names(x)

  fields <- as_zoned_sys_time_from_naive_time_cpp(x, precision, zone, nonexistent, ambiguous)

  new_zoned_time_from_fields(fields, precision, zone, names)
}

validate_nonexistent <- function(nonexistent, size) {
  nonexistent_size <- vec_size(nonexistent)

  if (nonexistent_size != 1L && nonexistent_size != size) {
    abort(paste0("`nonexistent` must have length 1, or ", size, "."))
  }

  invisible(nonexistent)
}

validate_ambiguous <- function(ambiguous, size) {
  ambiguous_size <- vec_size(ambiguous)

  if (ambiguous_size != 1L && ambiguous_size != size) {
    abort(paste0("`ambiguous` must have length 1, or ", size, "."))
  }

  invisible(ambiguous)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype2.clock_naive_time.clock_naive_time <- function(x, y, ...) {
  ptype2_time_point_and_time_point(x, y, ...)
}

#' @export
vec_cast.clock_naive_time.clock_naive_time <- function(x, to, ...) {
  cast_time_point_to_time_point(x, to, ...)
}

# ------------------------------------------------------------------------------

#' @method vec_arith clock_naive_time
#' @export
vec_arith.clock_naive_time <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_naive_time", y)
}

#' @method vec_arith.clock_naive_time MISSING
#' @export
vec_arith.clock_naive_time.MISSING <- function(op, x, y, ...) {
  arith_time_point_and_missing(op, x, y, ...)
}

#' @method vec_arith.clock_naive_time clock_naive_time
#' @export
vec_arith.clock_naive_time.clock_naive_time <- function(op, x, y, ...) {
  arith_time_point_and_time_point(op, x, y, ...)
}

#' @method vec_arith.clock_naive_time clock_duration
#' @export
vec_arith.clock_naive_time.clock_duration <- function(op, x, y, ...) {
  arith_time_point_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration clock_naive_time
#' @export
vec_arith.clock_duration.clock_naive_time <- function(op, x, y, ...) {
  arith_duration_and_time_point(op, x, y, ...)
}

#' @method vec_arith.clock_naive_time numeric
#' @export
vec_arith.clock_naive_time.numeric <- function(op, x, y, ...) {
  arith_time_point_and_numeric(op, x, y, ...)
}

#' @method vec_arith.numeric clock_naive_time
#' @export
vec_arith.numeric.clock_naive_time <- function(op, x, y, ...) {
  arith_numeric_and_time_point(op, x, y, ...)
}
