#' Construct a duration
#'
#' These helpers construct durations of the specified precision.
#'
#' @param n `[integer]`
#'
#'   The number of ticks to use when creating the duration.
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
  clock_rcrd_proxy(x)
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

#' @export
as_duration <- function(x) {
  UseMethod("as_duration")
}

#' @export
as_duration.clock_duration <- function(x) {
  x
}

# ------------------------------------------------------------------------------

# Note:
# Will cast upward or downward.
# Casting year -> day or month -> day can lose precision because those durations
# are based on fractions of a day and durations always use integer storage

#' @export
duration_cast <- function(x, precision) {
  x_precision <- duration_precision(x)
  precision <- validate_precision(precision)
  fields <- duration_cast_cpp(x, x_precision, precision)
  new_duration_from_fields(fields, precision, clock_rcrd_names(x))
}

#' @export
duration_floor <- function(x, precision, ..., n = 1L) {
  duration_rounder(x, precision, n, duration_floor_cpp, "floor", ...)
}

#' @export
duration_ceiling <- function(x, precision, ..., n = 1L) {
  duration_rounder(x, precision, n, duration_ceiling_cpp, "ceiling", ...)
}

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

#' @export
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
  field(x, "ticks")
}
field_ticks_of_day <- function(x) {
  field(x, "ticks_of_day")
}
field_ticks_of_second <- function(x) {
  field(x, "ticks_of_second")
}
