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

duration_helper <- function(n, precision) {
  n <- vec_cast(n, integer(), x_arg = "n")
  fields <- duration_helper_cpp(n, precision)

  new_duration(
    ticks = fields$ticks,
    ticks_of_day = fields$ticks_of_day,
    ticks_of_second = fields$ticks_of_second,
    precision = precision
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

  mapply(int_assert, fields, names(fields))

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

# Don't allow automatic promotion of duration precisions.
# Promoting year -> day can actually lose precision because a year is a fractional
# number of days and our storage is always integer based.

#' @export
vec_ptype2.clock_duration.clock_duration <- function(x, y, ...) {
  x_precision <- duration_precision(x)
  y_precision <- duration_precision(y)

  if (x_precision == y_precision) {
    x
  } else {
    stop_incompatible_type(x, y, ...)
  }
}

#' @export
vec_cast.clock_duration.clock_duration <- function(x, to, ...) {
  x_precision <- duration_precision(x)
  to_precision <- duration_precision(to)

  if (x_precision == to_precision) {
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
duration_floor <- function(x, precision) {
  duration_rounder(x, precision, duration_floor_cpp, "floor")
}

#' @export
duration_ceil <- function(x, precision) {
  duration_rounder(x, precision, duration_ceil_cpp, "ceil")
}

#' @export
duration_round <- function(x, precision) {
  duration_rounder(x, precision, duration_round_cpp, "round")
}

duration_rounder <- function(x, precision, rounder, verb) {
  if (!is_valid_precision(precision)) {
    abort("`precision` must be a valid precision string.")
  }

  x_precision <- duration_precision(x)

  if (precision_value(x_precision) < precision_value(precision)) {
    abort(paste0("Can't ", verb, " to a more precise precision."))
  }

  fields <- rounder(x, x_precision, precision)

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
is_duration <- function(x) {
  inherits(x, "clock_duration")
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
