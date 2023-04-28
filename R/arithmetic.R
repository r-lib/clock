#' Clock arithmetic
#'
#' @description
#' This is the landing page for all clock arithmetic functions. There are
#' specific sub-pages describing how arithmetic works for different calendars
#' and time points, which is where you should look for more information.
#'
#' Calendars are efficient at arithmetic with irregular units of time, such as
#' month, quarters, or years.
#'
#' - [year-month-day][year-month-day-arithmetic]
#'
#' - [year-month-weekday][year-month-weekday-arithmetic]
#'
#' - [year-quarter-day][year-quarter-day-arithmetic]
#'
#' - [year-week-day][year-week-day-arithmetic]
#'
#' - [iso-year-week-day][iso-year-week-day-arithmetic]
#'
#' - [year-day][year-day-arithmetic]
#'
#' Time points, such as naive-times and sys-times, are efficient at arithmetic
#' with regular, well-defined units of time, such as days, hours, seconds,
#' or nanoseconds.
#'
#' - [time-point][time-point-arithmetic]
#'
#' Durations can use any of these arithmetic functions, and return a new
#' duration with a precision corresponding to the common type of the
#' input and the function used.
#'
#' - [duration][duration-arithmetic]
#'
#' Weekdays can perform day-based circular arithmetic.
#'
#' - [weekday][weekday-arithmetic]
#'
#' There are also convenience methods for doing arithmetic directly on a
#' native R date or date-time type:
#'
#' - [dates (Date)][Date-arithmetic]
#'
#' - [date-times (POSIXct / POSIXlt)][posixt-arithmetic]
#'
#' @details
#' `x` and `n` are recycled against each other using
#' [tidyverse recycling rules][vctrs::vector_recycling_rules].
#'
#' Months and years are considered "irregular" because some months have more
#' days then others (28, 29, 30, or 31), and some years have more days than
#' others (365 or 366).
#'
#' Days are considered "regular" because they are defined as 86,400 seconds.
#'
#' @inheritParams rlang::args_dots_empty
#'
#' @param x `[object]`
#'
#'   An object.
#'
#' @param n `[integer / clock_duration]`
#'
#'   An integer vector to be converted to a duration, or a duration
#'   corresponding to the arithmetic function being used. This corresponds
#'   to the number of duration units to add. `n` may be negative to subtract
#'   units of duration.
#'
#' @return `x` after performing the arithmetic.
#'
#' @name clock-arithmetic
#'
#' @examples
#' # See each sub-page for more specific examples
#' x <- year_month_day(2019, 2, 1)
#' add_months(x, 1)
NULL

#' @rdname clock-arithmetic
#' @export
add_years <- function(x, n, ...) {
  UseMethod("add_years")
}

#' @rdname clock-arithmetic
#' @export
add_quarters <- function(x, n, ...) {
  UseMethod("add_quarters")
}

#' @rdname clock-arithmetic
#' @export
add_months <- function(x, n, ...) {
  UseMethod("add_months")
}

#' @rdname clock-arithmetic
#' @export
add_weeks <- function(x, n, ...) {
  UseMethod("add_weeks")
}

#' @rdname clock-arithmetic
#' @export
add_days <- function(x, n, ...) {
  UseMethod("add_days")
}

#' @rdname clock-arithmetic
#' @export
add_hours <- function(x, n, ...) {
  UseMethod("add_hours")
}

#' @rdname clock-arithmetic
#' @export
add_minutes <- function(x, n, ...) {
  UseMethod("add_minutes")
}

#' @rdname clock-arithmetic
#' @export
add_seconds <- function(x, n, ...) {
  UseMethod("add_seconds")
}

#' @rdname clock-arithmetic
#' @export
add_milliseconds <- function(x, n, ...) {
  UseMethod("add_milliseconds")
}

#' @rdname clock-arithmetic
#' @export
add_microseconds <- function(x, n, ...) {
  UseMethod("add_microseconds")
}

#' @rdname clock-arithmetic
#' @export
add_nanoseconds <- function(x, n, ...) {
  UseMethod("add_nanoseconds")
}

# ------------------------------------------------------------------------------

#' @export
add_years.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
}

#' @export
add_quarters.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
}

#' @export
add_months.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
}

#' @export
add_weeks.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
}

#' @export
add_days.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
}

#' @export
add_hours.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
}

#' @export
add_minutes.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
}

#' @export
add_seconds.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
}

#' @export
add_milliseconds.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
}

#' @export
add_microseconds.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
}

#' @export
add_nanoseconds.default <- function(x, n, ...) {
  stop_clock_unsupported(x)
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

  precision <- duration_precision_attribute(duration)
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
