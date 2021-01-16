#' @export
duration_years <- function(n = integer()) {
  duration_helper(n, "year")
}
#' @export
duration_quarters <- function(n = integer()) {
  duration_helper(n, "quarter")
}
#' @export
duration_months <- function(n = integer()) {
  duration_helper(n, "month")
}
#' @export
duration_weeks <- function(n = integer()) {
  duration_helper(n, "week")
}
#' @export
duration_days <- function(n = integer()) {
  duration_helper(n, "day")
}
#' @export
duration_hours <- function(n = integer()) {
  duration_helper(n, "hour")
}
#' @export
duration_minutes <- function(n = integer()) {
  duration_helper(n, "minute")
}
#' @export
duration_seconds <- function(n = integer()) {
  duration_helper(n, "second")
}
#' @export
duration_milliseconds <- function(n = integer()) {
  duration_helper(n, "millisecond")
}
#' @export
duration_microseconds <- function(n = integer()) {
  duration_helper(n, "microsecond")
}
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

#' @export
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
  proxy_rcrd(x)
}

#' @export
vec_restore.clock_duration <- function(x, to, ...) {
  fields <- restore_rcrd_fields(x)
  names <- restore_rcrd_names(x)
  precision <- duration_precision(to)
  new_duration_from_fields(fields, precision, names)
}

#' @export
vec_proxy_equal.clock_duration <- function(x, ...) {
  proxy_equal_rcrd(x)
}

# ------------------------------------------------------------------------------

# Don't allow automatic promotion of duration precisions. There is no common
# duration type between, say, <year> and <day> because the common
# ratio is <216> and doesn't result in a named duration class.

#' @export
vec_ptype2.clock_duration.clock_duration <- function(x, y, ...) {
  if (duration_precision(x) == duration_precision(y)) {
    x
  } else {
    stop_incompatible_type(x, y, ...)
  }
}

#' @export
vec_cast.clock_duration.clock_duration <- function(x, to, ...) {
  if (duration_precision(x) == duration_precision(to)) {
    x
  } else {
    stop_incompatible_cast(x, to, ...)
  }
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
    stop_incompatible_op(op, x, y)
  )
}

#' @export
#' @method vec_arith.clock_duration numeric
vec_arith.clock_duration.numeric <- function(op, x, y, ...) {
  switch(
    op,
    "+" = duration_plus(x, duration_helper(y, duration_precision(x)), names_common(x, y)),
    "-" = duration_minus(x, duration_helper(y, duration_precision(x)), names_common(x, y)),
    stop_incompatible_op(op, x, y)
  )
}

#' @export
#' @method vec_arith.numeric clock_duration
vec_arith.numeric.clock_duration <- function(op, x, y, ...) {
  switch(
    op,
    "+" = duration_plus(duration_helper(x, duration_precision(y)), y, names_common(x, y)),
    "-" = stop_incompatible_op(op, x, y, details = "Cannot subtract a duration from a numeric."),
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

duration_arith <- function(x, y, names, fn) {
  if (!is_duration(x) || !is_duration(y)) {
    abort("`x` and `y` must both be 'duration' objects.")
  }

  x_precision <- duration_precision(x)
  y_precision <- duration_precision(y)

  args <- vec_recycle_common(x = x, y = y, names = names)
  x <- args$x
  y <- args$y
  names <- args$names

  result <- fn(x, y, x_precision, y_precision)

  new_duration_from_fields(
    fields = result$fields,
    precision = result$precision,
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

is_years <- function(x) {
  is_duration_with_precision("year")
}
is_quarters <- function(x) {
  is_duration_with_precision("quarter")
}
is_months <- function(x) {
  is_duration_with_precision("month")
}
is_weeks <- function(x) {
  is_duration_with_precision("week")
}
is_days <- function(x) {
  is_duration_with_precision("day")
}
is_hours <- function(x) {
  is_duration_with_precision("hour")
}
is_minutes <- function(x) {
  is_duration_with_precision("minute")
}
is_seconds <- function(x) {
  is_duration_with_precision("second")
}
is_milliseconds <- function(x) {
  is_duration_with_precision("millisecond")
}
is_microseconds <- function(x) {
  is_duration_with_precision("microsecond")
}
is_nanoseconds <- function(x) {
  is_duration_with_precision("nanoseconds")
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
