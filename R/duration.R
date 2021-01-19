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
  duration_helper(n, "year")
}
#' @rdname duration-helper
#' @export
duration_quarters <- function(n = integer()) {
  duration_helper(n, "quarter")
}
#' @rdname duration-helper
#' @export
duration_months <- function(n = integer()) {
  duration_helper(n, "month")
}
#' @rdname duration-helper
#' @export
duration_weeks <- function(n = integer()) {
  duration_helper(n, "week")
}
#' @rdname duration-helper
#' @export
duration_days <- function(n = integer()) {
  duration_helper(n, "day")
}
#' @rdname duration-helper
#' @export
duration_hours <- function(n = integer()) {
  duration_helper(n, "hour")
}
#' @rdname duration-helper
#' @export
duration_minutes <- function(n = integer()) {
  duration_helper(n, "minute")
}
#' @rdname duration-helper
#' @export
duration_seconds <- function(n = integer()) {
  duration_helper(n, "second")
}
#' @rdname duration-helper
#' @export
duration_milliseconds <- function(n = integer()) {
  duration_helper(n, "millisecond")
}
#' @rdname duration-helper
#' @export
duration_microseconds <- function(n = integer()) {
  duration_helper(n, "microsecond")
}
#' @rdname duration-helper
#' @export
duration_nanoseconds <- function(n = integer()) {
  duration_helper(n, "nanosecond")
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

  new_duration(
    ticks = fields$ticks,
    ticks_of_day = fields$ticks_of_day,
    ticks_of_second = fields$ticks_of_second,
    precision = precision,
    names = names
  )
}

# ------------------------------------------------------------------------------

new_duration <- function(ticks = integer(),
                         ticks_of_day = integer(),
                         ticks_of_second = integer(),
                         precision = "year",
                         ...,
                         names = NULL,
                         class = NULL) {
  if (!is_valid_precision(precision)) {
    abort("`precision` must be a valid precision string.")
  }
  precision_value <- precision_value(precision)

  if (precision_value <= PRECISION_DAY) {
    fields <- list(ticks = ticks)
  } else if (precision_value <= PRECISION_SECOND) {
    fields <- list(ticks = ticks, ticks_of_day = ticks_of_day)
  } else {
    fields <- list(ticks = ticks, ticks_of_day = ticks_of_day, ticks_of_second = ticks_of_second)
  }

  field_names <- names(fields)
  for (i in seq_along(fields)) {
    int_assert(fields[[i]], field_names[[i]])
  }

  new_clock_rcrd(
    fields = fields,
    precision = precision,
    ...,
    names = names,
    class = c(class, "clock_duration")
  )
}

new_duration_from_fields <- function(fields, precision, names = NULL) {
  new_duration(
    ticks = fields$ticks,
    ticks_of_day = fields$ticks_of_day,
    ticks_of_second = fields$ticks_of_second,
    precision = precision,
    names = names
  )
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
  paste0("duration<", precision, ">")
}

#' @export
vec_ptype_abbr.clock_duration <- function(x, ...) {
  precision <- duration_precision(x)
  precision <- precision_abbr(precision)
  paste0("dur<", precision, ">")
}

# ------------------------------------------------------------------------------

#' @export
vec_proxy.clock_duration <- function(x, ...) {
  names <- names(x)
  duration_proxy(x, names)
}

duration_proxy <- function(x, names = NULL) {
  out <- unclass(x)
  out[["clock_rcrd:::names"]] <- names
  out <- new_data_frame(out)
  out
}

#' @export
vec_restore.clock_duration <- function(x, to, ...) {
  names <- restore_rcrd_names(x)
  duration_restore(x, to, names)
}

duration_restore <- function(x, to, names = NULL) {
  fields <- restore_rcrd_fields(x)
  precision <- duration_precision(to)
  new_duration_from_fields(fields, precision, names)
}

#' @export
vec_proxy_equal.clock_duration <- function(x, ...) {
  duration_proxy_equal(x)
}

duration_proxy_equal <- function(x) {
  proxy_equal_rcrd(x)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype2.clock_duration.clock_duration <- function(x, y, ...) {
  x_precision <- duration_precision(x)
  y_precision <- duration_precision(y)

  if (x_precision == y_precision) {
    return(x)
  }

  precision <- duration_precision_common(x_precision, y_precision)

  if (is_na(precision)) {
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

  if (precision_value(x_precision) > precision_value(to_precision)) {
    stop_incompatible_cast(
      x = x,
      to = to,
      ...,
      details = "Can't cast to a less precise precision."
    )
  }

  precision <- duration_precision_common(x_precision, to_precision)

  if (is_na(precision)) {
    stop_incompatible_cast(
      x = x,
      to = to,
      ...,
      details = "Duration cast cannot be done exactly."
    )
  }

  duration_cast(x, precision)
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
  if (!is_valid_precision(precision)) {
    abort("`precision` must be a valid precision string.")
  }

  fields <- duration_cast_cpp(x, duration_precision(x), precision)

  new_duration(
    ticks = fields$ticks,
    ticks_of_day = fields$ticks_of_day,
    ticks_of_second = fields$ticks_of_second,
    precision = precision,
    names = names(x)
  )
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

  if (!is_valid_precision(precision)) {
    abort("`precision` must be a valid precision string.")
  }

  x_precision <- duration_precision(x)

  if (precision_value(x_precision) < precision_value(precision)) {
    abort(paste0("Can't ", verb, " to a more precise precision."))
  }

  fields <- rounder(x, x_precision, precision, n)

  new_duration(
    ticks = fields$ticks,
    ticks_of_day = fields$ticks_of_day,
    ticks_of_second = fields$ticks_of_second,
    precision = precision,
    names = names(x)
  )
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
  n <- duration_collect_n(n, "year")
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_quarters.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, "quarter")
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_months.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, "month")
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_weeks.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, "week")
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_days.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, "day")
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_hours.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, "hour")
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_minutes.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, "minute")
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_seconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, "second")
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_milliseconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, "millisecond")
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_microseconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, "microsecond")
  duration_plus(x, n, names_common(x, n))
}
#' @export
add_nanoseconds.clock_duration <- function(x, n, ...) {
  n <- duration_collect_n(n, "nanosecond")
  duration_plus(x, n, names_common(x, n))
}

duration_collect_n <- function(n, precision) {
  if (is_duration_with_precision(n, precision)) {
    return(n)
  }

  if (is_duration(n)) {
    abort(paste0("`n` must have '", precision, "' precision."))
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

  new_duration_from_fields(
    fields = fields,
    precision = precision,
    names = names
  )
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

  new_duration_from_fields(
    fields = fields,
    precision = precision,
    names = names
  )
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

duration_precision_common <- function(x_precision, y_precision) {
  duration_precision_common_cpp(x_precision, y_precision)
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
