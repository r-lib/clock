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
  .Call(`_clock_clock_rcrd_proxy`, x)
}

#' @export
vec_restore.clock_duration <- function(x, to, ...) {
  .Call(`_clock_duration_restore`, x, to)
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
#' x <- as_sys(year_month_day(2019, 01, 01))
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
as_sys.clock_duration <- function(x) {
  names <- clock_rcrd_names(x)
  precision <- duration_precision(x)

  if (precision < PRECISION_DAY) {
    abort("`x` must have at least 'day' precision to convert to a time point.")
  }

  new_sys_time_from_fields(x, precision, names)
}

#' @export
as_naive.clock_duration <- function(x) {
  as_naive(as_sys(x))
}

#' @export
as.integer.clock_duration <- function(x, ...) {
  out <- duration_as_integer_cpp(x, duration_precision(x))
  names(out) <- names(x)
  out
}

#' @export
as.double.clock_duration <- function(x, ...) {
  out <- duration_as_double_cpp(x, duration_precision(x))
  names(out) <- names(x)
  out
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
#' as_sys(x)
#' as_sys(cast)
#' as_sys(floor)
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
seq.clock_duration <- function(from,
                               to = NULL,
                               by = NULL,
                               length.out = NULL,
                               along.with = NULL,
                               ...) {
  check_dots_empty()

  vec_assert(from, size = 1L)
  if (is.na(from)) {
    abort("`from` can't be `NA`.")
  }

  has_to <- !is_null(to)
  has_by <- !is_null(by)
  has_lo <- !is_null(length.out)
  has_aw <- !is_null(along.with)

  if (has_aw) {
    if (has_lo) {
      abort("Can only specify one of `length.out` and `along.with`.")
    } else {
      has_lo <- TRUE
      length.out <- vec_size(along.with)
    }
  }

  n_has <- sum(has_to, has_by, has_lo)

  if (n_has != 2L) {
    message <- paste0(
      "Must specify exactly two of:\n",
      "- `to`\n",
      "- `by`\n",
      "- Either `length.out` or `along.with`"
    )
    abort(message)
  }

  if (has_to) {
    args <- vec_cast_common(from = from, to = to)
    from <- args$from
    to <- args$to

    vec_assert(to, size = 1L, arg = "to")
    if (is.na(to)) {
      abort("`to` can't be `NA`.")
    }
  }

  if (has_by) {
    precision <- duration_precision(from)
    by <- duration_collect_by(by, precision)

    vec_assert(by, size = 1L, arg = "by")
    if (is.na(by)) {
      abort("`by` can't be `NA`.")
    }
    if (by == duration_helper(0L, precision)) {
      abort("`by` can't be `0`.")
    }
  }

  if (has_lo) {
    length.out <- vec_cast(length.out, integer(), x_arg = "length.out")

    vec_assert(length.out, size = 1L, arg = "length.out")
    if (is.na(length.out)) {
      abort("`length.out` can't be `NA`.")
    }
    if (length.out < 0) {
      abort("`length.out` can't be negative.")
    }
  }

  if (has_to) {
    if (has_by) {
      duration_seq_to_by(from, to, by)
    } else {
      duration_seq_to_lo(from, to, length.out)
    }
  } else {
    duration_seq_by_lo(from, by, length.out)
  }
}

duration_seq_to_by <- function(from, to, by) {
  # TODO: Can generate `NA`
  by <- as.integer(by)

  # Base seq() requires negative `by` when creating a decreasing seq, so this
  # helps be compatible with that.
  if (from > to && by > 0L) {
    abort("When `from` is greater than `to`, `by` must be negative.")
  }
  if (from < to && by < 0L) {
    abort("When `from` is less than `to`, `by` must be positive.")
  }

  start <- from

  to <- to - from
  # TODO: Will often generate `NA`
  to <- as.integer(to)

  from <- 0L

  steps <- seq.int(from, to, by = by)
  steps <- duration_helper(steps, duration_precision(start))

  start + steps
}

duration_seq_to_lo <- function(from, to, length.out) {
  start <- from

  to <- to - from
  # TODO: Will often generate `NA`
  to <- as.integer(to)

  from <- 0L

  steps <- seq.int(from, to, length.out = length.out)

  tryCatch(
    expr = {
      steps <- vec_cast(steps, integer())
    },
    vctrs_error_cast_lossy = function(cnd) {
      message <- paste0(
        "Usage of `length.out` or `along.with` must generate a non-fractional ",
        "sequence between `from` and `to`."
      )
      abort(message)
    }
  )

  steps <- duration_helper(steps, duration_precision(start))

  start + steps
}

duration_seq_by_lo <- function(from, by, length.out) {
  names <- NULL
  precision <- duration_precision(from)
  fields <- duration_seq_by_lo_cpp(from, precision, by, length.out)
  new_duration_from_fields(fields, precision, names)
}

duration_collect_by <- function(by, precision) {
  if (is_duration(by)) {
    to <- duration_helper(integer(), precision)
    vec_cast(by, to, x_arg = "by")
  } else {
    duration_helper(by, precision)
  }
}

# Used by other methods that eventually call the duration seq() method
seq_impl <- function(from, to, by, length.out, along.with, precision, ...) {
  if (!is_null(to)) {
    to <- to - from
  }

  start <- from
  from <- duration_helper(0L, precision)

  steps <- seq(
    from = from,
    to = to,
    by = by,
    length.out = length.out,
    along.with = along.with,
    ...
  )

  start + steps
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

#' Arithmetic: duration
#'
#' @description
#' These are duration methods for the
#' [arithmetic generics][clock-arithmetic].
#'
#' - `add_years()`
#'
#' - `add_quarters()`
#'
#' - `add_months()`
#'
#' - `add_weeks()`
#'
#' - `add_days()`
#'
#' - `add_hours()`
#'
#' - `add_minutes()`
#'
#' - `add_seconds()`
#'
#' - `add_milliseconds()`
#'
#' - `add_microseconds()`
#'
#' - `add_nanoseconds()`
#'
#' When adding to a duration using one of these functions, a second duration
#' is created based on the function name and `n`. The two durations are then
#' added together, and the precision of the result is determined as the
#' _more precise precision_ of the two durations.
#'
#' @details
#' Duration arithmetic is most useful when adding sub-daily or daily
#' precisions together, or when adding monthly/quarterly/yearly precisions to
#' other durations of similar precisions.
#'
#' Be _extremely_ careful when adding sub-daily or daily precisions to less
#' precise precisions, such as monthly or yearly. Durations are defined in terms
#' of a number of seconds, and calendrical months and years cannot be broken
#' down like that, since they are irregular periods of time (there aren't always
#' 30 days in a month, or 365 days in a year). Read the Internal Representation
#' section of the documentation for [duration helpers][duration-helper] to
#' learn more about how durations are defined.
#'
#' `x` and `n` are recycled against each other.
#'
#' @inheritParams add_years
#'
#' @param x `[clock_duration]`
#'
#'   A duration vector.
#'
#' @return `x` after performing the arithmetic, possibly with a more precise
#'   precision.
#'
#' @name duration-arithmetic
#'
#' @examples
#' x <- duration_seconds(5)
#'
#' # Addition in the same precision
#' add_seconds(x, 1:10)
#'
#' # Addition with days, defined as 86400 seconds
#' add_days(x, 1)
#'
#' # Similarly, if you start with days and add seconds, you get the common
#' # precision of the two back, which is seconds
#' y <- duration_days(1)
#' add_seconds(y, 5)
#'
#' # You can add years to a duration of months, which adds
#' # an additional 12 months / year
#' z <- duration_months(5)
#' add_years(z, 1)
NULL

#' @rdname duration-arithmetic
#' @export
add_years.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_YEAR)
  duration_plus(x, n, names_common(x, n))
}
#' @rdname duration-arithmetic
#' @export
add_quarters.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_QUARTER)
  duration_plus(x, n, names_common(x, n))
}
#' @rdname duration-arithmetic
#' @export
add_months.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_MONTH)
  duration_plus(x, n, names_common(x, n))
}
#' @rdname duration-arithmetic
#' @export
add_weeks.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_WEEK)
  duration_plus(x, n, names_common(x, n))
}
#' @rdname duration-arithmetic
#' @export
add_days.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_DAY)
  duration_plus(x, n, names_common(x, n))
}
#' @rdname duration-arithmetic
#' @export
add_hours.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_HOUR)
  duration_plus(x, n, names_common(x, n))
}
#' @rdname duration-arithmetic
#' @export
add_minutes.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_MINUTE)
  duration_plus(x, n, names_common(x, n))
}
#' @rdname duration-arithmetic
#' @export
add_seconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_SECOND)
  duration_plus(x, n, names_common(x, n))
}
#' @rdname duration-arithmetic
#' @export
add_milliseconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_MILLISECOND)
  duration_plus(x, n, names_common(x, n))
}
#' @rdname duration-arithmetic
#' @export
add_microseconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_MICROSECOND)
  duration_plus(x, n, names_common(x, n))
}
#' @rdname duration-arithmetic
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
