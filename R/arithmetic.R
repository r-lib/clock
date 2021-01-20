# ------------------------------------------------------------------------------

#' @export
add_years <- function(x, n, ...) {
  UseMethod("add_years")
}

#' @export
add_years.clock_time_point <- function(x, n, ...) {
  stop_clock_unsupported_time_point_op("add_years")
}

#' @export
add_years.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_years")
}

# ------------------------------------------------------------------------------

#' @export
add_quarters <- function(x, n, ...) {
  UseMethod("add_quarters")
}

#' @export
add_quarters.clock_time_point <- function(x, n, ...) {
  stop_clock_unsupported_time_point_op("add_quarters")
}

#' @export
add_quarters.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_quarters")
}

# ------------------------------------------------------------------------------

#' @export
add_months <- function(x, n, ...) {
  UseMethod("add_months")
}

#' @export
add_months.clock_time_point <- function(x, n, ...) {
  stop_clock_unsupported_time_point_op("add_months")
}

#' @export
add_months.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_months")
}

# ------------------------------------------------------------------------------

#' @export
add_weeks <- function(x, n, ...) {
  UseMethod("add_weeks")
}

#' @export
add_weeks.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_weeks")
}

# ------------------------------------------------------------------------------

#' @export
add_days <- function(x, n, ...) {
  UseMethod("add_days")
}

#' @export
add_days.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_days")
}

# ------------------------------------------------------------------------------

#' @export
add_hours <- function(x, n, ...) {
  UseMethod("add_hours")
}

#' @export
add_hours.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_hours")
}

# ------------------------------------------------------------------------------

#' @export
add_minutes <- function(x, n, ...) {
  UseMethod("add_minutes")
}

#' @export
add_minutes.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_minutes")
}

# ------------------------------------------------------------------------------

#' @export
add_seconds <- function(x, n, ...) {
  UseMethod("add_seconds")
}

#' @export
add_seconds.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_seconds")
}

# ------------------------------------------------------------------------------

#' @export
add_milliseconds <- function(x, n, ...) {
  UseMethod("add_milliseconds")
}

#' @export
add_milliseconds.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_milliseconds")
}

# ------------------------------------------------------------------------------

#' @export
add_microseconds <- function(x, n, ...) {
  UseMethod("add_microseconds")
}

#' @export
add_microseconds.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_microseconds")
}

# ------------------------------------------------------------------------------

#' @export
add_nanoseconds <- function(x, n, ...) {
  UseMethod("add_nanoseconds")
}

#' @export
add_nanoseconds.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported_calendar_op("add_nanoseconds")
}

# ------------------------------------------------------------------------------

add_duration <- function(x, duration, ..., swapped = FALSE) {
  check_dots_empty()

  if (swapped) {
    # `duration` was LHS, so use names from it if applicable, making sure
    # that we only recycle everything once
    args <- vec_recycle_common(x = x, duration = duration)
    x <- args$x
    duration <- args$duration
    names(x) <- names_common(duration, x)
  }

  precision <- duration_precision(duration)
  precision <- precision_to_string(precision)

  switch (
    precision,
    year = add_years(x, duration),
    quarter = add_quarters(x, duration),
    month = add_months(x, duration),
    week = add_weeks(x, duration),
    day = add_days(x, duration),
    hour = add_hours(x, duration),
    minute = add_minutes(x, duration),
    second = add_seconds(x, duration),
    millisecond = add_milliseconds(x, duration),
    microsecond = add_microseconds(x, duration),
    nanosecond = add_nanoseconds(x, duration)
  )
}
