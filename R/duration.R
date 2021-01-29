#' Construct a duration
#'
#' @description
#' These helpers construct durations of the specified precision. Durations
#' represent units of time in a specified precision.
#'
#' @section Internal Representation:
#'
#' Durations are internally represented as an integer number of "ticks" along
#' with a ratio describing how it converts to a number of seconds. The
#' following duration ratios are used in clock:
#'
#' - `1 year == 31556952 seconds`
#'
#' - `1 quarter == 7889238 seconds`
#'
#' - `1 month == 2629746 seconds`
#'
#' - `1 week == 604800 seconds`
#'
#' - `1 day == 86400 seconds`
#'
#' - `1 hour == 3600 seconds`
#'
#' - `1 minute == 60 seconds`
#'
#' - `1 second == 1 second`
#'
#' - `1 millisecond == 1 / 1000 seconds`
#'
#' - `1 microsecond == 1 / 1000000 seconds`
#'
#' - `1 nanosecond == 1 / 1000000000 seconds`
#'
#' A duration of 1 year is defined to correspond to the
#' average length of a proleptic Gregorian year, i.e. 365.2425 days.
#'
#' A duration of 1 month is defined as exactly 1/12 of a year.
#'
#' A duration of 1 quarter is defined as exactly 1/4 of a year.
#'
#' A duration of 1 week is defined as exactly 7 days.
#'
#' @param n `[integer]`
#'
#'   The number of units of time to use when creating the duration.
#'
#' @return A duration of the specified precision.
#'
#' @name duration-helper
#'
#' @examples
#' duration_years(1:5)
#' duration_nanoseconds(1:5)
NULL

#' @rdname duration-helper
#' @export
duration_years <- function(n = integer()) {
  duration_helper(n, PRECISION_YEAR)
}
#' @rdname duration-helper
#' @export
duration_quarters <- function(n = integer()) {
  duration_helper(n, PRECISION_QUARTER)
}
#' @rdname duration-helper
#' @export
duration_months <- function(n = integer()) {
  duration_helper(n, PRECISION_MONTH)
}
#' @rdname duration-helper
#' @export
duration_weeks <- function(n = integer()) {
  duration_helper(n, PRECISION_WEEK)
}
#' @rdname duration-helper
#' @export
duration_days <- function(n = integer()) {
  duration_helper(n, PRECISION_DAY)
}
#' @rdname duration-helper
#' @export
duration_hours <- function(n = integer()) {
  duration_helper(n, PRECISION_HOUR)
}
#' @rdname duration-helper
#' @export
duration_minutes <- function(n = integer()) {
  duration_helper(n, PRECISION_MINUTE)
}
#' @rdname duration-helper
#' @export
duration_seconds <- function(n = integer()) {
  duration_helper(n, PRECISION_SECOND)
}
#' @rdname duration-helper
#' @export
duration_milliseconds <- function(n = integer()) {
  duration_helper(n, PRECISION_MILLISECOND)
}
#' @rdname duration-helper
#' @export
duration_microseconds <- function(n = integer()) {
  duration_helper(n, PRECISION_MICROSECOND)
}
#' @rdname duration-helper
#' @export
duration_nanoseconds <- function(n = integer()) {
  duration_helper(n, PRECISION_NANOSECOND)
}

duration_helper <- function(n, precision, ..., retain_names = FALSE) {
  check_dots_empty()

  # Generally don't retain names for helpers like `duration_years()`,
  # but might need to during arithmetic
  if (retain_names) {
    names <- names(n)
  } else {
    names <- NULL
  }

  n <- vec_cast(n, integer(), x_arg = "n")
  fields <- duration_helper_cpp(n, precision)

  new_duration_from_fields(fields, precision, names)
}

# ------------------------------------------------------------------------------

#' @export
format.clock_duration <- function(x, ...) {
  out <- format_duration_cpp(x, duration_precision(x))
  names(out) <- names(x)
  out
}

#' @export
obj_print_data.clock_duration <- function(x, ...) {
  if (vec_size(x) == 0L) {
    return(invisible(x))
  }

  out <- format(x)
  print(out, quote = FALSE, na.print = "NA")

  invisible(x)
}

#' @export
vec_ptype_full.clock_duration <- function(x, ...) {
  precision <- duration_precision(x)
  precision <- precision_to_string(precision)
  paste0("duration<", precision, ">")
}

#' @export
vec_ptype_abbr.clock_duration <- function(x, ...) {
  precision <- duration_precision(x)
  precision <- precision_to_string(precision)
  precision <- precision_abbr(precision)
  paste0("dur<", precision, ">")
}

# ------------------------------------------------------------------------------

#' @export
vec_proxy.clock_duration <- function(x, ...) {
  .Call("_clock_clock_rcrd_proxy", x, PACKAGE = "clock")
}

#' @export
vec_restore.clock_duration <- function(x, to, ...) {
  .Call("_clock_duration_restore", x, to, PACKAGE = "clock")
}

#' @export
vec_proxy_equal.clock_duration <- function(x, ...) {
  clock_rcrd_proxy_equal(x)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype2.clock_duration.clock_duration <- function(x, y, ...) {
  x_precision <- duration_precision(x)
  y_precision <- duration_precision(y)

  if (x_precision == y_precision) {
    return(x)
  }

  precision <- duration_precision_common_cpp(x_precision, y_precision)

  if (is.na(precision)) {
    stop_incompatible_type(
      x = x,
      y = y,
      ...,
      details = "Duration common type would not generate a named duration type."
    )
  }

  if (precision == x_precision) {
    x
  } else {
    y
  }
}

#' @export
vec_cast.clock_duration.clock_duration <- function(x, to, ...) {
  x_precision <- duration_precision(x)
  to_precision <- duration_precision(to)

  if (x_precision == to_precision) {
    return(x)
  }

  if (x_precision > to_precision) {
    stop_incompatible_cast(
      x = x,
      to = to,
      ...,
      details = "Can't cast to a less precise precision."
    )
  }

  precision <- duration_precision_common_cpp(x_precision, to_precision)

  if (is.na(precision)) {
    stop_incompatible_cast(
      x = x,
      to = to,
      ...,
      details = "Duration cast cannot be done exactly."
    )
  }

  fields <- duration_cast_cpp(x, x_precision, to_precision)

  new_duration_from_fields(fields, to_precision, clock_rcrd_names(x))
}

# ------------------------------------------------------------------------------

#' Convert to a duration
#'
#' You generally convert to a duration from either a sys-time or a naive-time.
#' The precision of the input is retained in the returned duration.
#'
#' @param x `[object]`
#'
#'   An object to convert to a duration.
#'
#' @return A duration with the same precision as `x`.
#'
#' @export
#' @examples
#' x <- as_sys_time(year_month_day(2019, 01, 01))
#'
#' # The number of days since 1970-01-01 UTC
#' as_duration(x)
#'
#' x <- x + duration_seconds(1)
#' x
#'
#' # The number of seconds since 1970-01-01 00:00:00 UTC
#' as_duration(x)
as_duration <- function(x) {
  UseMethod("as_duration")
}

#' @export
as_duration.clock_duration <- function(x) {
  x
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_duration <- function(x) {
  names <- clock_rcrd_names(x)
  precision <- duration_precision(x)

  if (precision < PRECISION_DAY) {
    abort("`x` must have at least 'day' precision to convert to a time point.")
  }

  new_sys_time_from_fields(x, precision, names)
}

#' @export
as_naive_time.clock_duration <- function(x) {
  as_naive_time(as_sys_time(x))
}

# ------------------------------------------------------------------------------

#' Cast a duration between precisions
#'
#' @description
#' Casting is one way to change a duration's precision.
#'
#' Casting to a less precise precision will completely drop information that
#' is more precise than the precision that you are casting to. It does so
#' in a way that makes it round towards zero.
#'
#' Casting to a more precise precision is done through a multiplication by
#' a conversion factor between the current precision and the new precision.
#'
#' @details
#' When you want to change to a less precise precision, you often want
#' [duration_floor()] instead of `duration_cast()`, as that rounds towards
#' negative infinity, which is generally the desired behavior when working with
#' time points (especially ones pre-1970, which are stored as negative
#' durations).
#'
#' @param x `[clock_duration]`
#'
#'   A duration.
#'
#' @param precision `[character(1)]`
#'
#'   A precision. One of:
#'
#'   - `"year"`
#'
#'   - `"quarter"`
#'
#'   - `"month"`
#'
#'   - `"week"`
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
#' @export
#' @examples
#' x <- duration_seconds(c(86401, -86401))
#'
#' # Casting rounds towards 0
#' cast <- duration_cast(x, "day")
#' cast
#'
#' # Flooring rounds towards negative infinity
#' floor <- duration_floor(x, "day")
#' floor
#'
#' # Flooring is generally more useful when working with time points,
#' # note that the cast ends up rounding the pre-1970 date up to the next
#' # day, while the post-1970 date is rounded down.
#' as_sys_time(x)
#' as_sys_time(cast)
#' as_sys_time(floor)
#'
#' # Casting to a more precise precision
#' duration_cast(x, "millisecond")
duration_cast <- function(x, precision) {
  if (!is_duration(x)) {
    abort("`x` must be a duration.")
  }
  x_precision <- duration_precision(x)
  precision <- validate_precision(precision)
  fields <- duration_cast_cpp(x, x_precision, precision)
  new_duration_from_fields(fields, precision, clock_rcrd_names(x))
}

# ------------------------------------------------------------------------------

#' Duration rounding
#'
#' @description
#' - `duration_floor()` rounds a duration down to a multiple of the specified
#'   `precision`.
#'
#' - `duration_ceiling()` rounds a duration up to a multiple of the specified
#'   `precision`.
#'
#' - `duration_round()` rounds up or down depending on what is closer,
#'   rounding up on ties.
#'
#' @details
#' Duration rounding is most useful when rounding from sub-daily precisions up
#' to daily precision, or when rounding monthly/quarterly precisions up to
#' yearly precision. These durations are defined intuitively relative to
#' each other.
#'
#' Be _extremely_ careful when rounding sub-daily or daily precisions up to more
#' granular precisions such as monthly or yearly. Durations are defined in terms
#' of a number of seconds, and calendrical months and years cannot be broken
#' down like that, since they are irregular periods of time (there aren't always
#' 30 days in a month, or 365 days in a year). Read the Internal Representation
#' section of the documentation for [duration helpers][duration-helper] to
#' learn more about how durations are defined.
#'
#' @inheritParams ellipsis::dots_empty
#' @inheritParams duration_cast
#'
#' @param n `[positive integer(1)]`
#'
#'   A positive integer specifying the multiple of `precision` to use.
#'
#' @name duration-rounding
#'
#' @examples
#' x <- duration_seconds(c(86399, 86401))
#'
#' duration_floor(x, "day")
#' duration_ceiling(x, "day")
#'
#' # Every 2 days, using an origin of day 0
#' y <- duration_seconds(c(0, 86400, 86400 * 2, 86400 * 3))
#' duration_floor(y, "day", n = 2)
#'
#' # Shifting the origin to be day 1
#' origin <- duration_days(1)
#' duration_floor(y - origin, "day", n = 2) + origin
#'
#' # Rounding will round ties up
#' half_day <- 86400 / 2
#' half_day_durations <- duration_seconds(c(half_day - 1, half_day, half_day + 1))
#' duration_round(half_day_durations, "day")
NULL

#' @rdname duration-rounding
#' @export
duration_floor <- function(x, precision, ..., n = 1L) {
  duration_rounder(x, precision, n, duration_floor_cpp, "floor", ...)
}

#' @rdname duration-rounding
#' @export
duration_ceiling <- function(x, precision, ..., n = 1L) {
  duration_rounder(x, precision, n, duration_ceiling_cpp, "ceiling", ...)
}

#' @rdname duration-rounding
#' @export
duration_round <- function(x, precision, ..., n = 1L) {
  duration_rounder(x, precision, n, duration_round_cpp, "round", ...)
}

duration_rounder <- function(x, precision, n, rounder, verb, ...) {
  check_dots_empty()

  if (!is_duration(x)) {
    abort("`x` must be a duration object.")
  }

  n <- vec_cast(n, integer(), x_arg = "n")
  if (!is_number(n)) {
    abort("`n` must be a single number.")
  }
  if (n <= 0L) {
    abort("`n` must be a positive number.")
  }

  precision <- validate_precision(precision)
  x_precision <- duration_precision(x)

  if (x_precision < precision) {
    abort(paste0("Can't ", verb, " to a more precise precision."))
  }

  fields <- rounder(x, x_precision, precision, n)

  new_duration_from_fields(fields, precision, clock_rcrd_names(x))
}

# ------------------------------------------------------------------------------

#' @export
#' @method vec_arith clock_duration
vec_arith.clock_duration <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_duration", y)
}

#' @export
#' @method vec_arith.clock_duration MISSING
vec_arith.clock_duration.MISSING <- function(op, x, y, ...) {
  switch(
    op,
    "+" = x,
    "-" = duration_unary_minus(x),
  )
}

#' @export
#' @method vec_arith.clock_duration clock_duration
vec_arith.clock_duration.clock_duration <- function(op, x, y, ...) {
  switch(
    op,
    "+" = duration_plus(x, y, names_common(x, y)),
    "-" = duration_minus(x, y, names_common(x, y)),
    "%%" = duration_modulus(x, y, names_common(x, y)),

    # TODO: Operation would return an integer vector,
    # but precision often won't be good enough
    "%/%" = stop_incompatible_op(
      op,
      x,
      y,
      details = paste0(
        "<duration> %/% <duration> is not yet supported, ",
        "as the resulting type often won't fit into an R integer."
      )
    ),

    stop_incompatible_op(op, x, y)
  )
}

#' @export
#' @method vec_arith.clock_duration numeric
vec_arith.clock_duration.numeric <- function(op, x, y, ...) {
  switch(
    op,
    "*" = duration_scalar_multiply(x, y, names_common(x, y)),
    "/" = stop_incompatible_op(op, x, y, details = "Durations only support integer division. Did you want `%/%`?"),
    "%/%" = duration_scalar_divide(x, y, names_common(x, y)),
    stop_incompatible_op(op, x, y)
  )
}

#' @export
#' @method vec_arith.numeric clock_duration
vec_arith.numeric.clock_duration <- function(op, x, y, ...) {
  switch(
    op,
    "*" = duration_scalar_multiply(y, x, names_common(x, y)),
    "/" = stop_incompatible_op(op, x, y, details = "Durations only support integer division. Did you want `%/%`?"),
    "%/%" = stop_incompatible_op(op, x, y, details = "Cannot divide a numeric by a duration."),
    stop_incompatible_op(op, x, y)
  )
}

# ------------------------------------------------------------------------------

#' @export
add_years.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_YEAR)
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_quarters.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_QUARTER)
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_months.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_MONTH)
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_weeks.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_WEEK)
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_days.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_DAY)
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_hours.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_HOUR)
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_minutes.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_MINUTE)
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_seconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_SECOND)
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_milliseconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_MILLISECOND)
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_microseconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_MICROSECOND)
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_nanoseconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_NANOSECOND)
  duration_plus(x, n, names_common(x, n))
}

duration_collect_n <- function(n, precision) {
  if (is_duration_with_precision(n, precision)) {
    return(n)
  }

  if (is_duration(n)) {
    precision_string <- precision_to_string(precision)
    abort(paste0("`n` must have '", precision_string, "' precision."))
  }

  duration_helper(n, precision, retain_names = TRUE)
}

# ------------------------------------------------------------------------------

duration_plus <- function(x, y, names) {
  duration_arith(x, y, names, duration_plus_cpp)
}
duration_minus <- function(x, y, names) {
  duration_arith(x, y, names, duration_minus_cpp)
}
duration_modulus <- function(x, y, names) {
  duration_arith(x, y, names, duration_modulus_cpp)
}

duration_arith <- function(x, y, names, fn) {
  if (!is_duration(x) || !is_duration(y)) {
    abort("`x` and `y` must both be 'duration' objects.")
  }

  args <- vec_cast_common(x = x, y = y)
  args <- vec_recycle_common(!!!args, names = names)
  x <- args$x
  y <- args$y
  names <- args$names

  precision <- duration_precision(x)

  fields <- fn(x, y, precision)

  new_duration_from_fields(fields, precision, names)
}

duration_scalar_multiply <- function(x, y, names) {
  duration_scalar_arith(x, y, names, duration_scalar_multiply_cpp)
}
duration_scalar_divide <- function(x, y, names) {
  duration_scalar_arith(x, y, names, duration_scalar_divide_cpp)
}

duration_scalar_arith <- function(x, y, names, fn) {
  if (!is_duration(x)) {
    abort("`x` must be a 'duration' object.")
  }

  precision <- duration_precision(x)

  y <- vec_cast(y, integer(), x_arg = "y")

  args <- vec_recycle_common(x = x, y = y, names = names)
  x <- args$x
  y <- args$y
  names <- args$names

  fields <- fn(x, y, precision)

  new_duration_from_fields(fields, precision, names)
}

names_common <- function(x, y) {
  names <- names(x)
  if (is_null(names)) {
    names <- names(y)
  }
  names
}

duration_unary_minus <- function(x) {
  precision <- duration_precision(x)
  fields <- duration_unary_minus_cpp(x, precision)
  new_duration_from_fields(fields, precision, names(x))
}

# ------------------------------------------------------------------------------

#' Is `x` a duration?
#'
#' This function determines if the input is a duration object.
#'
#' @param x `[object]`
#'
#'   An object.
#'
#' @return `TRUE` if `x` inherits from `"clock_duration"`, otherwise `FALSE`.
#'
#' @export
#' @examples
#' is_duration(1)
#' is_duration(duration_days(1))
is_duration <- function(x) {
  inherits(x, "clock_duration")
}

is_duration_with_precision <- function(x, precision) {
  is_duration(x) && duration_precision(x) == precision
}

# ------------------------------------------------------------------------------

duration_precision <- function(x) {
  attr(x, "precision", exact = TRUE)
}

# ------------------------------------------------------------------------------

field_ticks <- function(x) {
  # The `ticks` field is the first field of every
  # duration / time point / zoned time type, which makes it the field that
  # names are on. When extracting the field, we don't ever
  # want the names to come with it.
  out <- field(x, "ticks")
  names(out) <- NULL
  out
}
field_ticks_of_day <- function(x) {
  field(x, "ticks_of_day")
}
field_ticks_of_second <- function(x) {
  field(x, "ticks_of_second")
}
