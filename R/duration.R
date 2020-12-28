new_duration <- function(ticks = integer(),
                         ticks_of_day = integer(),
                         ticks_of_second = integer(),
                         precision = "year",
                         ...,
                         names = NULL,
                         class = NULL) {
  if (!is_integer(ticks)) {
    abort("`ticks` must be an integer vector.")
  }
  if (!is_integer(ticks_of_day)) {
    abort("`ticks_of_day` must be an integer vector.")
  }
  if (!is_integer(ticks_of_second)) {
    abort("`ticks_of_second` must be an integer vector.")
  }

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

  new_clock_rcrd(
    fields = fields,
    precision = precision,
    ...,
    names = names,
    class = c(class, "clock_duration")
  )
}

#' @export
is_duration <- function(x) {
  inherits(x, "clock_duration")
}

#' @export
format.clock_duration <- function(x, ...) {
  ticks <- field_ticks(x)
  ticks_of_day <- field_ticks_of_day(x, strict = FALSE)
  ticks_of_second <- field_ticks_of_second(x, strict = FALSE)
  precision <- duration_precision(x)
  out <- format_duration_cpp(ticks, ticks_of_day, ticks_of_second, precision)
  names(out) <- names(x)
  out
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

duration_precision <- function(x) {
  attr(x, "precision", exact = TRUE)
}

# ------------------------------------------------------------------------------

duration_years <- function(n = integer()) {
  duration_helper(n, "year")
}
duration_quarters <- function(n = integer()) {
  duration_helper(n, "quarter")
}
duration_months <- function(n = integer()) {
  duration_helper(n, "month")
}
duration_weeks <- function(n = integer()) {
  duration_helper(n, "week")
}
duration_days <- function(n = integer()) {
  duration_helper(n, "day")
}
duration_hours <- function(n = integer()) {
  duration_helper(n, "hour")
}
duration_minutes <- function(n = integer()) {
  duration_helper(n, "minute")
}
duration_seconds <- function(n = integer()) {
  duration_helper(n, "second")
}
duration_milliseconds <- function(n = integer()) {
  duration_helper(n, "millisecond")
}
duration_microseconds <- function(n = integer()) {
  duration_helper(n, "microsecond")
}
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
duration_cast <- function(x, precision) {
  if (!is_valid_precision(precision)) {
    abort("`precision` must be a valid precision string.")
  }

  x_precision <- duration_precision(x)

  if (x_precision == precision) {
    return(x)
  }

  ticks <- field_ticks(x)
  ticks_of_day <- field_ticks_of_day(x, strict = FALSE)
  ticks_of_second <- field_ticks_of_second(x, strict = FALSE)

  fields <- duration_cast_cpp(ticks, ticks_of_day, ticks_of_second, x_precision, precision)

  new_duration(
    ticks = fields$ticks,
    ticks_of_day = fields$ticks_of_day,
    ticks_of_second = fields$ticks_of_second,
    precision = precision,
    names = names(x)
  )
}

# ------------------------------------------------------------------------------

field_ticks <- function(x) {
  field(x, "ticks")
}
field_ticks_of_day <- function(x, strict = TRUE) {
  if (strict || has_ticks_of_day(x)) {
    field(x, "ticks_of_day")
  } else {
    integer()
  }
}
field_ticks_of_second <- function(x, strict = TRUE) {
  if (strict || has_ticks_of_second(x)) {
    field(x, "ticks_of_second")
  } else {
    integer()
  }
}

has_ticks_of_day <- function(x) {
  "ticks_of_day" %in% fields(x)
}
has_ticks_of_second <- function(x) {
  "ticks_of_second" %in% fields(x)
}
