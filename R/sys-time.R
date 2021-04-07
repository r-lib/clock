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

#' Parsing: sys-time
#'
#' @description
#' `sys_time_parse()` is a parser into a sys-time.
#'
#' `sys_time_parse()` is useful when you have date-time strings like:
#' `"2020-01-01 01:04:30-0400"`. If there is an attached UTC offset, but no time
#' zone name, then parsing this string as a sys-time using the `%z` command to
#' capture the offset is probably your best option. If you know that this string
#' should be interpreted in a specific time zone, parse as a sys-time to get the
#' UTC equivalent, then use [as_zoned_time()].
#'
#' The default options assume that `x` should be parsed at second precision,
#' using a `format` string of `"%Y-%m-%d %H:%M:%S"`.
#'
#' `sys_time_parse()` is nearly equivalent to [naive_time_parse()], except for
#' the fact that the `%z` command is actually used. Using `%z` assumes that the
#' rest of the date-time string should be interpreted as a naive-time, which is
#' then shifted by the UTC offset found in `%z`. The returned time can then be
#' validly interpreted as UTC.
#'
#' _`sys_time_parse()` ignores the `%Z` command._
#'
#' If your date-time strings contain a full time zone name and a UTC offset, use
#' [zoned_time_parse_complete()]. If they contain a time zone abbreviation, use
#' [zoned_time_parse_abbrev()].
#'
#' If your date-time strings don't contain an offset from UTC, you might
#' consider using [naive_time_parse()], since the resulting naive-time doesn't
#' come with an assumption of a UTC time zone.
#'
#' @inheritParams zoned-parsing
#'
#' @param precision `[character(1)]`
#'
#'   A precision for the resulting time point. One of:
#'
#'   - `"day"`
#'
#'   - `"hour"`
#'
#'   - `"minute"`
#'
#'   - `"second"`
#'
#'   - `"millisecond"`
#'
#'   - `"microsecond"`
#'
#'   - `"nanosecond"`
#'
#'   Setting the `precision` determines how much information `%S` attempts
#'   to parse.
#'
#' @return A sys-time.
#'
#' @export
#' @examples
#' sys_time_parse("2020-01-01 05:06:07")
#'
#' # Day precision
#' sys_time_parse("2020-01-01", precision = "day")
#'
#' # Nanosecond precision, but using a day based format
#' sys_time_parse("2020-01-01", format = "%Y-%m-%d", precision = "nanosecond")
#'
#' # Multiple format strings are allowed for heterogeneous times
#' sys_time_parse(
#'   c("2019-01-01", "2019/1/1"),
#'   format = c("%Y/%m/%d", "%Y-%m-%d"),
#'   precision = "day"
#' )
#'
#' # The `%z` command shifts the date-time by subtracting the UTC offset so
#' # that the returned sys-time can be interpreted as UTC
#' sys_time_parse(
#'   "2020-01-01 02:00:00 -0400",
#'   format = "%Y-%m-%d %H:%M:%S %z"
#' )
#'
#' # Remember that the `%Z` command is ignored entirely!
#' sys_time_parse("2020-01-01 America/New_York", format = "%Y-%m-%d %Z")
sys_time_parse <- function(x,
                           ...,
                           format = NULL,
                           precision = "second",
                           locale = clock_locale()) {
  precision <- validate_time_point_precision_string(precision)

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
#' # Dates are assumed to be naive, so the printed time is the same whether
#' # we convert it to sys-time or naive-time
#' as_sys_time(x)
#' as_naive_time(x)
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
  new_naive_time_from_fields(x, time_point_precision_attribute(x), clock_rcrd_names(x))
}

#' Convert to a zoned-time from a sys-time
#'
#' @description
#' This is a sys-time method for the [as_zoned_time()] generic.
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
#' x_utc <- as_zoned_time(x, "UTC")
#' x_utc
#'
#' # Converting to a different zone results in a different printed time,
#' # which corresponds to the exact same point in time, just in a different
#' # part of the work
#' x_ny <- as_zoned_time(x, "America/New_York")
#' x_ny
as_zoned_time.clock_sys_time <- function(x, zone, ...) {
  check_dots_empty()

  zone <- zone_validate(zone)

  # Promote to at least seconds precision for `zoned_time`
  x <- vec_cast(x, vec_ptype2(x, sys_seconds()))

  precision <- time_point_precision_attribute(x)
  names <- clock_rcrd_names(x)

  new_zoned_time_from_fields(x, precision, zone, names)
}

#' @export
as.character.clock_sys_time <- function(x, ...) {
  format(x)
}

# ------------------------------------------------------------------------------

#' What is the current sys-time?
#'
#' @description
#' `sys_time_now()` returns the current time in UTC.
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
#' x <- sys_time_now()
sys_time_now <- function() {
  names <- NULL
  fields <- sys_time_now_cpp()
  new_sys_time_from_fields(fields, PRECISION_NANOSECOND, names)
}

# ------------------------------------------------------------------------------

#' Info: sys-time
#'
#' @description
#' `sys_time_info()` retrieves a set of low-level information generally not
#' required for most date-time manipulations. It returns a data frame with the
#' following columns:
#'
#' - `begin`, `end`: Second precision sys-times specifying the range of the
#' current daylight saving time rule. The range is a half-open interval of
#' `[begin, end)`.
#'
#' - `offset`: A second precision `duration` specifying the offset from UTC.
#'
#' - `dst`: A logical vector specifying if daylight saving time is currently
#' active.
#'
#' - `abbreviation`: The time zone abbreviation in use throughout this `begin`
#' to `end` range.
#'
#' @details
#' If there have never been any daylight saving time transitions, the minimum
#' supported year value is returned for `begin` (typically, a year value of
#' `-32767`).
#'
#' If daylight saving time is no longer used in a time zone, the maximum
#' supported year value is returned for `end` (typically, a year value of
#' `32767`).
#'
#' The `offset` is the bridge between sys-time and naive-time for the `zone`
#' being used. The relationship of the three values is:
#'
#' ```
#' offset = naive_time - sys_time
#' ```
#'
#' @param x `[clock_sys_time]`
#'
#'   A sys-time.
#'
#' @param zone `[character]`
#'
#'   A valid time zone name.
#'
#'   Unlike most functions in clock, in `sys_time_info()` `zone` is vectorized
#'   and is recycled against `x`.
#'
#' @return A data frame of low level information.
#'
#' @export
#' @examples
#' library(vctrs)
#'
#' x <- year_month_day(2021, 03, 14, c(01, 03), c(59, 00), c(59, 00))
#' x <- as_naive_time(x)
#' x <- as_zoned_time(x, "America/New_York")
#'
#' # x[1] is in EST, x[2] is in EDT
#' x
#'
#' x_sys <- as_sys_time(x)
#'
#' info <- sys_time_info(x_sys, zoned_time_zone(x))
#' info
#'
#' # Convert `begin` and `end` to zoned-times to see the previous and
#' # next daylight saving time transitions
#' data_frame(
#'   x = x,
#'   begin = as_zoned_time(info$begin, zoned_time_zone(x)),
#'   end = as_zoned_time(info$end, zoned_time_zone(x))
#' )
#'
#' # `end` can be used to iterate through daylight saving time transitions
#' # by repeatedly calling `sys_time_info()`
#' sys_time_info(info$end, zoned_time_zone(x))
#'
#' # Multiple `zone`s can be supplied to look up daylight saving time
#' # information in different time zones
#' zones <- c("America/New_York", "America/Los_Angeles")
#'
#' info2 <- sys_time_info(x_sys[1], zones)
#' info2
#'
#' # The offset can be used to display the naive-time (i.e. the printed time)
#' # in both of those time zones
#' data_frame(
#'   zone = zones,
#'   naive_time = x_sys[1] + info2$offset
#' )
sys_time_info <- function(x, zone) {
  if (!is_sys_time(x)) {
    abort("`x` must be a sys-time.")
  }

  precision <- time_point_precision_attribute(x)

  # Recycle `x` to the common size. `zone` is recycled internally as required,
  # which is more efficient than reloading the time zone repeatedly.
  size <- vec_size_common(x = x, zone = zone)
  x <- vec_recycle(x, size)

  fields <- sys_time_info_cpp(x, precision, zone)

  new_sys_time_info_from_fields(fields)
}

new_sys_time_info_from_fields <- function(fields) {
  names <- NULL

  fields[["begin"]] <- new_sys_time_from_fields(fields[["begin"]], PRECISION_SECOND, names)
  fields[["end"]] <- new_sys_time_from_fields(fields[["end"]], PRECISION_SECOND, names)
  fields[["offset"]] <- new_duration_from_fields(fields[["offset"]], PRECISION_SECOND, names)

  new_data_frame(fields)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype.clock_sys_time <- function(x, ...) {
  switch(
    time_point_precision_attribute(x) + 1L,
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    clock_empty_sys_time_day,
    clock_empty_sys_time_hour,
    clock_empty_sys_time_minute,
    clock_empty_sys_time_second,
    clock_empty_sys_time_millisecond,
    clock_empty_sys_time_microsecond,
    clock_empty_sys_time_nanosecond,
    abort("Internal error: Invalid precision.")
  )
}

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

# ------------------------------------------------------------------------------

clock_init_sys_time_utils <- function(env) {
  day <- as_sys_time(year_month_day(integer(), integer(), integer()))

  assign("clock_empty_sys_time_day", day, envir = env)
  assign("clock_empty_sys_time_hour", time_point_cast(day, "hour"), envir = env)
  assign("clock_empty_sys_time_minute", time_point_cast(day, "minute"), envir = env)
  assign("clock_empty_sys_time_second", time_point_cast(day, "second"), envir = env)
  assign("clock_empty_sys_time_millisecond", time_point_cast(day, "millisecond"), envir = env)
  assign("clock_empty_sys_time_microsecond", time_point_cast(day, "microsecond"), envir = env)
  assign("clock_empty_sys_time_nanosecond", time_point_cast(day, "nanosecond"), envir = env)

  invisible(NULL)
}
