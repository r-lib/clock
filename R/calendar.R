# ------------------------------------------------------------------------------

#' @export
print.clock_calendar <- function(x, ..., max = NULL) {
  clock_print(x, max)
}

# - Each subclass implements a `format()` method
# - Unlike vctrs, don't use `print(quote = FALSE)` since we want to match base R
#' @export
obj_print_data.clock_calendar <- function(x, ..., max) {
  if (vec_is_empty(x)) {
    return(invisible(x))
  }

  x <- max_slice(x, max)

  out <- format(x)

  # Pass `max` to avoid base R's default footer
  print(out, max = max)

  invisible(x)
}

#' @export
obj_print_footer.clock_calendar <- function(x, ..., max) {
  clock_print_footer(x, max)
}

# Align left to match pillar_shaft.Date
# @export - lazy in .onLoad()
pillar_shaft.clock_calendar <- function(x, ...) {
  out <- format(x)
  pillar::new_pillar_shaft_simple(out, align = "left")
}

# ------------------------------------------------------------------------------

# Note: Cannot cast between calendar precisions. Casting to a more precise
# precision is undefined because we consider things like year-month to be
# a range of days over the whole month, and it would be impossible to map
# that to just one day.

ptype2_calendar_and_calendar <- function(x, y, ...) {
  if (calendar_precision_attribute(x) == calendar_precision_attribute(y)) {
    x
  } else {
    stop_incompatible_type(
      x,
      y,
      ...,
      details = "Can't combine calendars with different precisions."
    )
  }
}

cast_calendar_to_calendar <- function(x, to, ...) {
  if (calendar_precision_attribute(x) == calendar_precision_attribute(to)) {
    x
  } else {
    stop_incompatible_cast(
      x,
      to,
      ...,
      details = "Can't cast between calendars with different precisions."
    )
  }
}

# ------------------------------------------------------------------------------

#' Is the calendar year a leap year?
#'
#' @description
#' `calendar_leap_year()` detects if the calendar year is a leap year - i.e.
#' does it contain one or more extra components than other years?
#'
#' A particular year is a leap year if:
#'
#' - [year_month_day()]: February has 29 days.
#'
#' - [year_month_weekday()]: February has a weekday that occurs 5 times.
#'
#' - [year_week_day()]: There are 53 weeks in the year, resulting in 371
#'   days in the year.
#'
#' - [iso_year_week_day()]: There are 53 weeks in the year, resulting in 371
#'   days in the year.
#'
#' - [year_quarter_day()]: One of the quarters has 1 more day than normal (the
#'   quarter with an extra day depends on the `start` used, but will always be
#'   the same for a particular `start`). This aligns with Gregorian leap years
#'   for all `start`s except February, in which case the leap year is always 1
#'   year after the Gregorian leap year.
#'
#' - [year_day()]: There are 366 days in the year.
#'
#' @param x `[calendar]`
#'
#'   A calendar type to detect leap years in.
#'
#' @return A logical vector the same size as `x`. Returns `TRUE` if in a leap
#'   year, `FALSE` if not in a leap year, and `NA` if `x` is `NA`.
#'
#' @export
#' @examples
#' x <- year_month_day(c(2019:2024, NA))
#' calendar_leap_year(x)
#'
#' # For year-quarter-day, the leap year typically aligns with the Gregorian
#' # leap year, unless the `start` is February, in which case the leap year is
#' # always 1 year after the Gregorian leap year
#' x <- year_quarter_day(2020:2021, start = clock_months$january)
#' calendar_leap_year(x)
#'
#' x <- year_quarter_day(2020:2021, start = clock_months$february)
#' calendar_leap_year(x)
#'
#' # With a January start, 2020 has the extra day
#' get_day(year_quarter_day(2020, 1:4, "last", start = clock_months$january))
#' get_day(year_quarter_day(2021, 1:4, "last", start = clock_months$january))
#' get_day(year_quarter_day(2022, 1:4, "last", start = clock_months$january))
#'
#' # With a February start, 2021 has the extra day
#' get_day(year_quarter_day(2020, 1:4, "last", start = clock_months$february))
#' get_day(year_quarter_day(2021, 1:4, "last", start = clock_months$february))
#' get_day(year_quarter_day(2022, 1:4, "last", start = clock_months$february))
calendar_leap_year <- function(x) {
  check_calendar(x)
  UseMethod("calendar_leap_year")
}

#' @export
calendar_leap_year.clock_calendar <- function(x) {
  stop_clock_unsupported(x)
}

# ------------------------------------------------------------------------------

#' Convert a calendar to an ordered factor of month names
#'
#' @description
#' `calendar_month_factor()` extracts the month values from a calendar and
#' converts them to an ordered factor of month names. This can be useful in
#' combination with ggplot2, or for modeling.
#'
#' This function is only relevant for calendar types that use a month field,
#' i.e. [year_month_day()] and [year_month_weekday()]. The calendar type must
#' have at least month precision.
#'
#' @inheritParams rlang::args_dots_empty
#' @inheritParams clock_locale
#'
#' @param x `[calendar]`
#'
#'   A calendar vector.
#'
#' @param abbreviate `[logical(1)]`
#'
#'   If `TRUE`, the abbreviated month names from `labels` will be used.
#'
#'   If `FALSE`, the full month names from `labels` will be used.
#'
#' @return An ordered factor representing the months.
#'
#' @export
#' @examples
#' x <- year_month_day(2019, 1:12)
#'
#' calendar_month_factor(x)
#' calendar_month_factor(x, abbreviate = TRUE)
#' calendar_month_factor(x, labels = "fr")
calendar_month_factor <- function(x, ..., labels = "en", abbreviate = FALSE) {
  check_calendar(x)
  UseMethod("calendar_month_factor")
}

#' @export
calendar_month_factor.clock_calendar <- function(
  x,
  ...,
  labels = "en",
  abbreviate = FALSE
) {
  stop_clock_unsupported(x)
}

calendar_month_factor_impl <- function(
  x,
  labels,
  abbreviate,
  ...,
  error_call = caller_env()
) {
  check_dots_empty0(...)

  if (calendar_precision_attribute(x) < PRECISION_MONTH) {
    cli::cli_abort(
      "{.arg x} must have at least {.str month} precision.",
      call = error_call
    )
  }

  if (is_character(labels)) {
    labels <- clock_labels_lookup(labels)
  }
  check_clock_labels(labels, call = error_call)

  check_bool(abbreviate, call = error_call)

  if (abbreviate) {
    labels <- labels$month_abbrev
  } else {
    labels <- labels$month
  }

  x <- get_month(x)
  x <- labels[x]

  factor(x, levels = labels, ordered = TRUE)
}

# ------------------------------------------------------------------------------

#' Group calendar components
#'
#' @description
#' `calendar_group()` groups at a multiple of the specified precision. Grouping
#' alters the value of a single component (i.e. the month component
#' if grouping by month). Components that are more precise than the precision
#' being grouped at are dropped altogether (i.e. the day component is dropped
#' if grouping by month).
#'
#' Each calendar has its own help page describing the grouping process in more
#' detail:
#'
#' - [year-month-day][year-month-day-group]
#'
#' - [year-month-weekday][year-month-weekday-group]
#'
#' - [year-week-day][year-week-day-group]
#'
#' - [iso-year-week-day][iso-year-week-day-group]
#'
#' - [year-quarter-day][year-quarter-day-group]
#'
#' - [year-day][year-day-group]
#'
#' @inheritParams rlang::args_dots_empty
#'
#' @param x `[calendar]`
#'
#'   A calendar vector.
#'
#' @param precision `[character(1)]`
#'
#'   A precision. Allowed precisions are dependent on the calendar used.
#'
#' @param n `[positive integer(1)]`
#'
#'   A single positive integer specifying a multiple of `precision` to use.
#'
#' @return `x` grouped at the specified `precision`.
#'
#' @export
#' @examples
#' # See the calendar specific help pages for more examples
#' x <- year_month_day(2019, c(1, 1, 2, 2, 3, 3, 4, 4), 1:8)
#' x
#'
#' # Group by two months
#' calendar_group(x, "month", n = 2)
#'
#' # Group by two days of the month
#' calendar_group(x, "day", n = 2)
calendar_group <- function(x, precision, ..., n = 1L) {
  check_dots_empty0(...)

  check_calendar(x)

  check_precision(precision)
  precision <- precision_to_integer(precision)
  calendar_check_precision(x, precision)

  x_precision <- calendar_precision_attribute(x)

  if (precision > x_precision) {
    precision <- precision_to_string(precision)
    x_precision <- precision_to_string(x_precision)

    message <- paste0(
      "Can't group at a precision ({.str {precision}}) ",
      "that is more precise than `x` ({.str {x_precision}})."
    )
    cli::cli_abort(message)
  }

  if (precision > PRECISION_SECOND && x_precision != precision) {
    # Grouping nanosecond precision by millisecond would be inconsistent
    # with our general philosophy that you "lock in" the subsecond precision.
    precision <- precision_to_string(precision)
    x_precision <- precision_to_string(x_precision)

    message <- paste0(
      "Can't group a subsecond precision `x` ({.str {x_precision}}) ",
      "by another subsecond precision ({.str {precision}})."
    )
    cli::cli_abort(message)
  }

  UseMethod("calendar_group")
}

#' @export
calendar_group.clock_calendar <- function(x, precision, ..., n = 1L) {
  stop_clock_unsupported(x)
}

calendar_group_time <- function(x, n, precision) {
  if (precision == PRECISION_HOUR) {
    value <- get_hour(x)
    value <- group_component0(value, n)
    x <- set_hour(x, value)
    return(x)
  }
  if (precision == PRECISION_MINUTE) {
    value <- get_minute(x)
    value <- group_component0(value, n)
    x <- set_minute(x, value)
    return(x)
  }
  if (precision == PRECISION_SECOND) {
    value <- get_second(x)
    value <- group_component0(value, n)
    x <- set_second(x, value)
    return(x)
  }

  # Generic ensures that if `x_precision > PRECISION_SECOND`,
  # then `precision == x_precision`, making this safe.
  if (precision == PRECISION_MILLISECOND) {
    value <- get_millisecond(x)
    value <- group_component0(value, n)
    x <- set_millisecond(x, value)
    return(x)
  }
  if (precision == PRECISION_MICROSECOND) {
    value <- get_microsecond(x)
    value <- group_component0(value, n)
    x <- set_microsecond(x, value)
    return(x)
  }
  if (precision == PRECISION_NANOSECOND) {
    value <- get_nanosecond(x)
    value <- group_component0(value, n)
    x <- set_nanosecond(x, value)
    return(x)
  }

  abort("Unknown precision.", .internal = TRUE)
}

group_component0 <- function(x, n) {
  (x %/% n) * n
}
group_component1 <- function(x, n) {
  ((x - 1L) %/% n) * n + 1L
}

validate_calendar_group_n <- function(n, ..., error_call = caller_env()) {
  check_number_whole(n, min = 0, call = error_call)
  n <- vec_cast(n, integer(), call = error_call)
  n
}

# ------------------------------------------------------------------------------

#' Narrow a calendar to a less precise precision
#'
#' @description
#' `calendar_narrow()` narrows `x` to the specified `precision`. It does so
#' by dropping components that represent a precision that is finer than
#' `precision`.
#'
#' Each calendar has its own help page describing the precisions that you
#' can narrow to:
#'
#' - [year-month-day][year-month-day-narrow]
#'
#' - [year-month-weekday][year-month-weekday-narrow]
#'
#' - [year-week-day][year-week-day-narrow]
#'
#' - [iso-year-week-day][iso-year-week-day-narrow]
#'
#' - [year-quarter-day][year-quarter-day-narrow]
#'
#' - [year-day][year-day-narrow]
#'
#' @details
#' A subsecond precision `x` cannot be narrowed to another subsecond precision.
#' You cannot narrow from, say, `"nanosecond"` to `"millisecond"` precision.
#' clock operates under the philosophy that once you have set the subsecond
#' precision of a calendar, it is "locked in" at that precision. If you
#' expected this to use integer division to divide the nanoseconds by 1e6 to
#' get to millisecond precision, you probably want to convert to a time point
#' first, and use [time_point_floor()].
#'
#' @inheritParams calendar_group
#'
#' @return `x` narrowed to the supplied `precision`.
#'
#' @export
#' @examples
#' # Hour precision
#' x <- year_month_day(2019, 1, 3, 4)
#' x
#'
#' # Narrowed to day precision
#' calendar_narrow(x, "day")
#'
#' # Or month precision
#' calendar_narrow(x, "month")
calendar_narrow <- function(x, precision) {
  check_calendar(x)

  check_precision(precision)
  precision <- precision_to_integer(precision)
  calendar_check_precision(x, precision)

  x_precision <- calendar_precision_attribute(x)

  if (precision > x_precision) {
    precision <- precision_to_string(precision)
    x_precision <- precision_to_string(x_precision)

    message <- paste0(
      "Can't narrow to a precision ({.str {precision}}) ",
      "that is wider than `x` ({.str {x_precision}})."
    )
    cli::cli_abort(message)
  }

  if (precision > PRECISION_SECOND && x_precision != precision) {
    # Allowing Nanosecond -> Millisecond wouldn't be consistent with us
    # disallowing `set_millisecond(<calendar<nanosecond>>)`, and is ambiguous.
    precision <- precision_to_string(precision)
    x_precision <- precision_to_string(x_precision)

    message <- paste0(
      "Can't narrow a subsecond precision `x` ({.str {x_precision}}) ",
      "to another subsecond precision ({.str {precision}})."
    )
    cli::cli_abort(message)
  }

  UseMethod("calendar_narrow")
}

#' @export
calendar_narrow.clock_calendar <- function(x, precision) {
  stop_clock_unsupported(x)
}

calendar_narrow_time <- function(out_fields, out_precision, x_fields) {
  if (out_precision >= PRECISION_HOUR) {
    out_fields[["hour"]] <- x_fields[["hour"]]
  }
  if (out_precision >= PRECISION_MINUTE) {
    out_fields[["minute"]] <- x_fields[["minute"]]
  }
  if (out_precision >= PRECISION_SECOND) {
    out_fields[["second"]] <- x_fields[["second"]]
  }
  if (out_precision > PRECISION_SECOND) {
    out_fields[["subsecond"]] <- x_fields[["subsecond"]]
  }

  out_fields
}

# ------------------------------------------------------------------------------

#' Widen a calendar to a more precise precision
#'
#' @description
#' `calendar_widen()` widens `x` to the specified `precision`. It does so
#' by setting new components to their smallest value.
#'
#' Each calendar has its own help page describing the precisions that you
#' can widen to:
#'
#' - [year-month-day][year-month-day-widen]
#'
#' - [year-month-weekday][year-month-weekday-widen]
#'
#' - [year-week-day][year-week-day-widen]
#'
#' - [iso-year-week-day][iso-year-week-day-widen]
#'
#' - [year-quarter-day][year-quarter-day-widen]
#'
#' - [year-day][year-day-widen]
#'
#' @details
#' A subsecond precision `x` cannot be widened. You cannot widen from, say,
#' `"millisecond"` to `"nanosecond"` precision. clock operates under the
#' philosophy that once you have set the subsecond precision of a calendar,
#' it is "locked in" at that precision. If you expected this to multiply
#' the milliseconds by 1e6 to get to nanosecond precision, you probably
#' want to convert to a time point first, and use [time_point_cast()].
#'
#' Generally, clock treats calendars at a specific precision as a _range_ of
#' values. For example, a month precision year-month-day is treated as a range
#' over `[yyyy-mm-01, yyyy-mm-last]`, with no assumption about the day of the
#' month. However, occasionally it is useful to quickly widen a calendar,
#' assuming that you want the beginning of this range to be used for each
#' component. This is where `calendar_widen()` can come in handy.
#'
#' @inheritParams calendar_group
#'
#' @return `x` widened to the supplied `precision`.
#'
#' @export
#' @examples
#' # Month precision
#' x <- year_month_day(2019, 1)
#' x
#'
#' # Widen to day precision
#' calendar_widen(x, "day")
#'
#' # Or second precision
#' calendar_widen(x, "second")
calendar_widen <- function(x, precision) {
  check_calendar(x)

  check_precision(precision)
  precision <- precision_to_integer(precision)
  calendar_check_precision(x, precision)

  x_precision <- calendar_precision_attribute(x)

  if (x_precision > precision) {
    precision <- precision_to_string(precision)
    x_precision <- precision_to_string(x_precision)

    message <- paste0(
      "Can't widen to a precision ({.str {precision}}) ",
      "that is narrower than `x` ({.str {x_precision}})."
    )
    cli::cli_abort(message)
  }

  if (x_precision > PRECISION_SECOND && x_precision != precision) {
    # Allowing Millisecond -> Nanosecond wouldn't be consistent with us
    # disallowing `set_nanosecond(<calendar<millisecond>>)`, and is ambiguous.
    precision <- precision_to_string(precision)
    x_precision <- precision_to_string(x_precision)

    message <- paste0(
      "Can't widen a subsecond precision `x` ({.str {x_precision}}) ",
      "to another subsecond precision ({.str {precision}})."
    )
    cli::cli_abort(message)
  }

  UseMethod("calendar_widen")
}

#' @export
calendar_widen.clock_calendar <- function(x, precision) {
  stop_clock_unsupported(x)
}

calendar_widen_time <- function(x, x_precision, precision) {
  if (precision >= PRECISION_HOUR && x_precision < PRECISION_HOUR) {
    x <- set_hour(x, 0L)
  }
  if (precision >= PRECISION_MINUTE && x_precision < PRECISION_MINUTE) {
    x <- set_minute(x, 0L)
  }
  if (precision >= PRECISION_SECOND && x_precision < PRECISION_SECOND) {
    x <- set_second(x, 0L)
  }

  # `x` is required to fulfill:
  # `x_precision < PRECISION_SECOND` or `x_precision == precision`
  if (precision == PRECISION_MILLISECOND && x_precision != precision) {
    x <- set_millisecond(x, 0L)
  }
  if (precision == PRECISION_MICROSECOND && x_precision != precision) {
    x <- set_microsecond(x, 0L)
  }
  if (precision == PRECISION_NANOSECOND && x_precision != precision) {
    x <- set_nanosecond(x, 0L)
  }

  x
}

# ------------------------------------------------------------------------------

#' Boundaries: calendars
#'
#' @description
#' - `calendar_start()` computes the start of a calendar at a particular
#'   `precision`, such as the "start of the quarter".
#'
#' - `calendar_end()` computes the end of a calendar at a particular
#'   `precision`, such as the "end of the month".
#'
#' For both `calendar_start()` and `calendar_end()`, the precision of `x` is
#' always retained.
#'
#' Each calendar has its own help page describing the precisions that you
#' can compute a boundary at:
#'
#' - [year-month-day][year-month-day-boundary]
#'
#' - [year-month-weekday][year-month-weekday-boundary]
#'
#' - [year-week-day][year-week-day-boundary]
#'
#' - [iso-year-week-day][iso-year-week-day-boundary]
#'
#' - [year-quarter-day][year-quarter-day-boundary]
#'
#' - [year-day][year-day-boundary]
#'
#' @inheritParams calendar_group
#'
#' @return `x` at the same precision, but with some components altered to be
#'   at the boundary value.
#'
#' @name calendar-boundary
#' @examples
#' # Hour precision
#' x <- year_month_day(2019, 2:4, 5, 6)
#' x
#'
#' # Compute the start of the month
#' calendar_start(x, "month")
#'
#' # Or the end of the month, notice that the hour value is adjusted as well
#' calendar_end(x, "month")
NULL

#' @rdname calendar-boundary
#' @export
calendar_start <- function(x, precision) {
  check_calendar(x)
  UseMethod("calendar_start")
}

#' @export
calendar_start.clock_calendar <- function(x, precision) {
  stop_clock_unsupported(x)
}

#' @rdname calendar-boundary
#' @export
calendar_end <- function(x, precision) {
  check_calendar(x)
  UseMethod("calendar_end")
}

#' @export
calendar_end.clock_calendar <- function(x, precision) {
  stop_clock_unsupported(x)
}

calendar_start_end_checks <- function(x, x_precision, precision, which) {
  calendar_check_precision(x, precision)

  if (x_precision < precision) {
    precision <- precision_to_string(precision)
    x_precision <- precision_to_string(x_precision)

    message <- paste0(
      "Can't compute the {which} of `x` ({.str {x_precision}}) ",
      "at a more precise precision ({.str {precision}})."
    )
    cli::cli_abort(message)
  }

  if (precision > PRECISION_SECOND && x_precision != precision) {
    # Computing the start/end of nanosecond precision at millisecond precision
    # would be inconsistent with our general philosophy that you "lock in"
    # the subsecond precision.
    precision <- precision_to_string(precision)
    x_precision <- precision_to_string(x_precision)

    message <- paste0(
      "Can't compute the {which} of a subsecond precision `x` ({.str {x_precision}}) ",
      "at another subsecond precision ({.str {precision}})."
    )
    cli::cli_abort(message)
  }

  invisible(x)
}

calendar_start_time <- function(x, x_precision, precision) {
  values <- list(
    hour = 0L,
    minute = 0L,
    second = 0L,
    millisecond = 0L,
    microsecond = 0L,
    nanosecond = 0L
  )

  calendar_start_end_time(x, x_precision, precision, values)
}

calendar_end_time <- function(x, x_precision, precision) {
  values <- list(
    hour = 23L,
    minute = 59L,
    second = 59L,
    millisecond = 999L,
    microsecond = 999999L,
    nanosecond = 999999999L
  )

  calendar_start_end_time(x, x_precision, precision, values)
}

calendar_start_end_time <- function(x, x_precision, precision, values) {
  if (precision <= PRECISION_DAY && x_precision > PRECISION_DAY) {
    x <- set_hour(x, values$hour)
  }
  if (precision <= PRECISION_HOUR && x_precision > PRECISION_HOUR) {
    x <- set_minute(x, values$minute)
  }
  if (precision <= PRECISION_MINUTE && x_precision > PRECISION_MINUTE) {
    x <- set_second(x, values$second)
  }
  if (precision <= PRECISION_SECOND && x_precision > PRECISION_SECOND) {
    if (x_precision == PRECISION_MILLISECOND) {
      x <- set_millisecond(x, values$millisecond)
    } else if (x_precision == PRECISION_MICROSECOND) {
      x <- set_microsecond(x, values$microsecond)
    } else if (x_precision == PRECISION_NANOSECOND) {
      x <- set_nanosecond(x, values$nanosecond)
    }
  }

  x
}

# ------------------------------------------------------------------------------

#' Counting: calendars
#'
#' @description
#' `calendar_count_between()` counts the number of `precision` units between
#' `start` and `end` (i.e., the number of years or months). This count
#' corresponds to the _whole number_ of units, and will never return a
#' fractional value.
#'
#' This is suitable for, say, computing the whole number of years or months
#' between two calendar dates, accounting for the day and time of day.
#'
#' Each calendar has its own help page describing the precisions that you can
#' count at:
#'
#' - [year-month-day][year-month-day-count-between]
#'
#' - [year-month-weekday][year-month-weekday-count-between]
#'
#' - [year-week-day][year-week-day-count-between]
#'
#' - [iso-year-week-day][iso-year-week-day-count-between]
#'
#' - [year-quarter-day][year-quarter-day-count-between]
#'
#' - [year-day][year-day-count-between]
#'
#' @section Comparison Direction:
#' The computed count has the property that if `start <= end`, then
#' `start + <count> <= end`. Similarly, if `start >= end`, then
#' `start + <count> >= end`. In other words, the comparison direction between
#' `start` and `end` will never change after adding the count to `start`. This
#' makes this function useful for repeated count computations at
#' increasingly fine precisions.
#'
#' @inheritParams calendar_group
#'
#' @param start,end `[clock_calendar]`
#'
#'   A pair of calendar vectors. These will be recycled to their common size.
#'
#' @return An integer representing the number of `precision` units between
#' `start` and `end`.
#'
#' @name calendar-count-between
#' @examples
#' # Number of whole years between these dates
#' x <- year_month_day(2000, 01, 05)
#' y <- year_month_day(2005, 01, 04:06)
#'
#' # Note that `2000-01-05 -> 2005-01-04` is only 4 full years
#' calendar_count_between(x, y, "year")
NULL

#' @rdname calendar-count-between
#' @export
calendar_count_between <- function(start, end, precision, ..., n = 1L) {
  check_calendar(start)
  UseMethod("calendar_count_between")
}

#' @export
calendar_count_between.clock_calendar <- function(
  start,
  end,
  precision,
  ...,
  n = 1L
) {
  check_dots_empty0(...)
  check_calendar(end)

  size <- vec_size_common(start = start, end = end)

  args <- vec_cast_common(start = start, end = end)
  args <- vec_recycle_common(!!!args, .size = size)
  start <- args[[1]]
  end <- args[[2]]

  check_number_whole(n, min = 0)
  n <- vec_cast(n, integer())

  check_precision(precision)
  precision_int <- precision_to_integer(precision)

  if (calendar_precision_attribute(start) < precision_int) {
    start_precision <- precision_to_string(calendar_precision_attribute(start))

    message <- paste0(
      "Precision of inputs ({.str {start_precision}}) must be at least as precise ",
      "as {.arg precision} ({.str {precision}})."
    )

    cli::cli_abort(message)
  }

  args <- calendar_count_between_standardize_precision_n(start, precision, n)
  precision <- args$precision
  n <- args$n

  # Core computation to get the difference (pre-adjustment).
  # Result is an integer because it represents a count of duration units.
  out <- calendar_count_between_compute(start, end, precision)

  # Comparison proxy, truncated to avoid fields already when computing `out`
  args <- calendar_count_between_proxy_compare(start, end, precision)
  start_proxy <- args$start
  end_proxy <- args$end

  if (ncol(start_proxy) == 0L) {
    # vctrs bug with vec_compare()?
    # https://github.com/r-lib/vctrs/issues/1500
    comparison <- vec_rep(0L, size)
  } else {
    comparison <- vec_compare(end_proxy, start_proxy)
  }

  # - When `start > end` and the non-year portion of `start < end`, add 1
  # - When `start < end` and the non-year portion of `start > end`, subtract 1
  adjustment <- vec_rep(-1L, size)
  adjustment[start > end] <- 1L
  adjustment[comparison != adjustment] <- 0L

  out <- out + adjustment

  if (n != 1L) {
    out <- out %/% n
  }

  out
}

# Internal generic
calendar_count_between_standardize_precision_n <- function(x, precision, n) {
  UseMethod("calendar_count_between_standardize_precision_n")
}

# Internal generic
calendar_count_between_compute <- function(start, end, precision) {
  UseMethod("calendar_count_between_compute")
}

# Internal generic
calendar_count_between_proxy_compare <- function(start, end, precision) {
  UseMethod("calendar_count_between_proxy_compare")
}

# ------------------------------------------------------------------------------

#' Spanning sequence: calendars
#'
#' @description
#' `calendar_spanning_seq()` generates a regular sequence along the span of
#' `x`, i.e. along `[min(x), max(x)]`. The sequence is generated at the
#' precision of `x`.
#'
#' Importantly, sequences can only be generated if the underlying [seq()] method
#' for the calendar in question supports a `from` and `to` value at the same
#' precision as `x`. For example, you can't compute a day precision spanning
#' sequence for a [year_month_day()] calendar (you can only compute a year
#' and month one). To create a day precision sequence, you'd have to convert to
#' a time-point first. See the individual [seq()] method documentation to learn
#' what precisions are allowed.
#'
#' @details
#' Missing values are automatically removed before the sequence is generated.
#'
#' If you need more precise sequence generation, call [range()] and [seq()]
#' directly.
#'
#' @param x `[clock_calendar]`
#'
#'   A calendar vector.
#'
#' @return A sequence along `[min(x), max(x)]`.
#'
#' @export
#' @examples
#' x <- year_month_day(c(2019, 2022, 2020), c(2, 5, 3))
#' x
#'
#' # Month precision spanning sequence
#' calendar_spanning_seq(x)
#'
#' # Quarter precision:
#' x <- year_quarter_day(c(2005, 2006, 2003), c(4, 2, 3))
#' calendar_spanning_seq(x)
#'
#' # Can't generate sequences if `seq()` doesn't allow the precision
#' x <- year_month_day(2019, c(1, 2, 1), c(20, 3, 25))
#' try(calendar_spanning_seq(x))
#'
#' # Generally this means you need to convert to a time point and use
#' # `time_point_spanning_seq()` instead
#' time_point_spanning_seq(as_sys_time(x))
calendar_spanning_seq <- function(x) {
  check_calendar(x)
  spanning_seq_impl(x)
}

# ------------------------------------------------------------------------------

#' Precision: calendar
#'
#' `calendar_precision()` extracts the precision from a calendar object. It
#' returns the precision as a single string.
#'
#' @param x `[clock_calendar]`
#'
#'   A calendar.
#'
#' @return A single string holding the precision of the calendar.
#'
#' @export
#' @examples
#' calendar_precision(year_month_day(2019))
#' calendar_precision(year_month_day(2019, 1, 1))
#' calendar_precision(year_quarter_day(2019, 3))
calendar_precision <- function(x) {
  check_calendar(x)
  UseMethod("calendar_precision")
}

#' @export
calendar_precision.clock_calendar <- function(x) {
  precision <- calendar_precision_attribute(x)
  precision <- precision_to_string(precision)
  precision
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_name <- function(x) {
  UseMethod("calendar_name")
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_is_precision <- function(x, precision) {
  UseMethod("calendar_is_precision")
}

calendar_check_precision <- function(
  x,
  precision,
  ...,
  error_call = caller_env()
) {
  if (calendar_is_precision(x, precision)) {
    return(invisible(NULL))
  }

  message <- paste0(
    "{.arg precision} must be a valid precision for a {.cls {calendar_name(x)}}, ",
    "not {.str {precision_to_string(precision)}}."
  )

  cli::cli_abort(message, call = error_call)
}

# ------------------------------------------------------------------------------

calendar_precision_attribute <- function(x) {
  attr(x, "precision", exact = TRUE)
}

# ------------------------------------------------------------------------------

calendar_check_minimum_precision <- function(
  x,
  precision,
  ...,
  arg = caller_arg(x),
  call = caller_env()
) {
  x_precision <- calendar_precision_attribute(x)

  if (x_precision >= precision) {
    return(invisible(NULL))
  }

  message <- c(
    "Can't perform this operation because of the precision of {.arg {arg}}.",
    i = "The precision of {.arg {arg}} must be at least {.str {precision_to_string(precision)}}.",
    i = "{.arg {arg}} has a precision of {.str {precision_to_string(x_precision)}}."
  )

  cli::cli_abort(message, call = call)
}

calendar_check_maximum_precision <- function(
  x,
  precision,
  ...,
  arg = caller_arg(x),
  call = caller_env()
) {
  x_precision <- calendar_precision_attribute(x)

  if (x_precision <= precision) {
    return(invisible(NULL))
  }

  message <- c(
    "Can't perform this operation because of the precision of {.arg {arg}}.",
    i = "The precision of {.arg {arg}} must be at most {.str {precision_to_string(precision)}}.",
    i = "{.arg {arg}} has a precision of {.str {precision_to_string(x_precision)}}."
  )

  cli::cli_abort(message, call = call)
}

calendar_check_exact_precision <- function(
  x,
  precision,
  ...,
  arg = caller_arg(x),
  call = caller_env()
) {
  x_precision <- calendar_precision_attribute(x)

  if (x_precision %in% precision) {
    return(invisible(NULL))
  }

  precision <- vapply(precision, precision_to_string, character(1))

  message <- c(
    "Can't perform this operation because of the precision of {.arg {arg}}.",
    i = "The precision of {.arg {arg}} must be {.or {.str {precision}}}.",
    i = "{.arg {arg}} has a precision of {.str {precision_to_string(x_precision)}}."
  )

  cli::cli_abort(message, call = call)
}

# For use in calendar constructor helpers
calendar_check_subsecond_precision <- function(
  subsecond_precision,
  ...,
  call = caller_env()
) {
  if (is_null(subsecond_precision)) {
    cli::cli_abort(
      "When {.arg subsecond} is provided, {.arg subsecond_precision} must also be specified.",
      call = call
    )
  }

  check_precision_subsecond(subsecond_precision, call = call)
}

# ------------------------------------------------------------------------------

calendar_check_no_invalid <- function(
  x,
  ...,
  arg = caller_arg(x),
  call = caller_env()
) {
  if (!invalid_any(x)) {
    return(invisible(NULL))
  }

  loc <- invalid_detect(x)
  loc <- which(loc)

  message <- c(
    "Can't convert {.arg {arg}} to another type because some dates are invalid.",
    i = "The following locations are invalid: {loc}.",
    i = "Resolve invalid dates with {.fn invalid_resolve}."
  )

  cli::cli_abort(message, call = call)
}

# ------------------------------------------------------------------------------

calendar_ptype_full <- function(x, class) {
  precision <- calendar_precision_attribute(x)
  precision <- precision_to_string(precision)
  paste0(class, "<", precision, ">")
}

calendar_ptype_abbr <- function(x, abbr) {
  precision <- calendar_precision_attribute(x)
  precision <- precision_to_string(precision)
  precision <- precision_abbr(precision)
  paste0(abbr, "<", precision, ">")
}

# ------------------------------------------------------------------------------

arith_calendar_and_missing <- function(op, x, y, ...) {
  switch(
    op,
    "+" = x,
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_calendar_and_calendar <- function(
  op,
  x,
  y,
  ...,
  calendar_minus_calendar_fn
) {
  switch(
    op,
    "-" = calendar_minus_calendar_fn(op, x, y, ...),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_calendar_and_duration <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(x, y),
    "-" = add_duration(x, -y),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_duration_and_calendar <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(y, x, swapped = TRUE),
    "-" = stop_incompatible_op(
      op,
      x,
      y,
      details = "Can't subtract a calendar from a duration.",
      ...
    ),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_calendar_and_numeric <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(
      x,
      duration_helper(y, calendar_precision_attribute(x), retain_names = TRUE)
    ),
    "-" = add_duration(
      x,
      duration_helper(-y, calendar_precision_attribute(x), retain_names = TRUE)
    ),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_numeric_and_calendar <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(
      y,
      duration_helper(x, calendar_precision_attribute(y), retain_names = TRUE),
      swapped = TRUE
    ),
    "-" = stop_incompatible_op(
      op,
      x,
      y,
      details = "Can't subtract a calendar from a duration.",
      ...
    ),
    stop_incompatible_op(op, x, y, ...)
  )
}

# ------------------------------------------------------------------------------

# Special support for when `lag * differences >= n`
#
# In vctrs, this forces a return value of `vec_slice(x, 0L)`, but this is
# not correct for calendar types. See also, `diff.Date()` which exists for a
# similar reason.
calendar_diff_is_empty <- function(x, lag, differences) {
  # Same errors as vctrs
  stopifnot(length(lag) == 1L, lag >= 1L)
  stopifnot(length(differences) == 1L, differences >= 1L)
  n <- vec_size(x)
  lag * differences >= n
}

# ------------------------------------------------------------------------------

#' @export
as_year_month_day.clock_calendar <- function(x, ...) {
  check_dots_empty0(...)
  as_year_month_day(as_sys_time(x))
}

#' @export
as_year_month_weekday.clock_calendar <- function(x, ...) {
  check_dots_empty0(...)
  as_year_month_weekday(as_sys_time(x))
}

#' @export
as_year_week_day.clock_calendar <- function(x, ..., start = NULL) {
  check_dots_empty0(...)
  as_year_week_day(as_sys_time(x), start = start)
}

#' @export
as_iso_year_week_day.clock_calendar <- function(x, ...) {
  check_dots_empty0(...)
  as_iso_year_week_day(as_sys_time(x))
}

#' @export
as_year_day.clock_calendar <- function(x, ...) {
  check_dots_empty0(...)
  as_year_day(as_sys_time(x))
}

#' @export
as_year_quarter_day.clock_calendar <- function(x, ..., start = NULL) {
  check_dots_empty0(...)
  as_year_quarter_day(as_sys_time(x), start = start)
}

# ------------------------------------------------------------------------------

#' @export
add_weeks.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported(x, details = advice_convert_to_time_point())
}

#' @export
add_days.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported(x, details = advice_convert_to_time_point())
}

#' @export
add_hours.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported(x, details = advice_convert_to_time_point())
}

#' @export
add_minutes.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported(x, details = advice_convert_to_time_point())
}

#' @export
add_seconds.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported(x, details = advice_convert_to_time_point())
}

#' @export
add_milliseconds.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported(x, details = advice_convert_to_time_point())
}

#' @export
add_microseconds.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported(x, details = advice_convert_to_time_point())
}

#' @export
add_nanoseconds.clock_calendar <- function(x, n, ...) {
  stop_clock_unsupported(x, details = advice_convert_to_time_point())
}

advice_convert_to_time_point <- function() {
  c(
    i = "Do you need to convert to a time point first?",
    i = cli::format_inline(
      "Use {.fn as_naive_time} or {.fn as_sys_time} to convert to a time point."
    )
  )
}

# ------------------------------------------------------------------------------

calendar_minimum <- function(precision, year) {
  out <- calendar_widen(year, precision)
  out <- calendar_start(out, "year")
  out
}

calendar_maximum <- function(precision, year) {
  out <- calendar_widen(year, precision)
  out <- calendar_end(out, "year")
  out
}

# ------------------------------------------------------------------------------

field_year <- function(x) {
  # The `year` field is the first field of every calendar type, which makes
  # it the field that names are on. When extracting the field, we don't ever
  # want the names to come with it.
  out <- field(x, "year")
  names(out) <- NULL
  out
}
field_quarter <- function(x) {
  field(x, "quarter")
}
field_month <- function(x) {
  field(x, "month")
}
field_week <- function(x) {
  field(x, "week")
}
field_day <- function(x) {
  field(x, "day")
}
field_hour <- function(x) {
  field(x, "hour")
}
field_minute <- function(x) {
  field(x, "minute")
}
field_second <- function(x) {
  field(x, "second")
}
field_subsecond <- function(x) {
  field(x, "subsecond")
}
field_index <- function(x) {
  field(x, "index")
}

is_calendar <- function(x) {
  inherits(x, "clock_calendar")
}

check_calendar <- function(x, ..., arg = caller_arg(x), call = caller_env()) {
  check_inherits(x, what = "clock_calendar", arg = arg, call = call)
}
