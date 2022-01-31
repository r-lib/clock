#' Calendar: year-day
#'
#' `year_day()` constructs a calendar vector from the Gregorian
#' year and day of the year.
#'
#' @details
#' Fields are recycled against each other.
#'
#' Fields are collected in order until the first `NULL` field is located. No
#' fields after the first `NULL` field are used.
#'
#' @inheritParams year_month_day
#'
#' @param day `[integer / NULL]`
#'
#'   The day of the year. Values `[1, 366]` are allowed.
#'
#' @return A year-day calendar vector.
#'
#' @export
#' @examples
#' # Just the year
#' x <- year_day(2019:2025)
#' x
#'
#' year_day(2020, 1:10)
#'
#' # Last day of the year, accounting for leap years
#' year_day(2019:2021, "last")
#'
#' # Precision can go all the way out to nanosecond
#' year_day(2019, 100, 2, 40, 45, 200, subsecond_precision = "nanosecond")
year_day <- function(year,
                     day = NULL,
                     hour = NULL,
                     minute = NULL,
                     second = NULL,
                     subsecond = NULL,
                     ...,
                     subsecond_precision = NULL) {
  check_dots_empty()

  # Stop on the first `NULL` argument
  if (is_null(day)) {
    precision <- PRECISION_YEAR
    fields <- list(year = year)
  } else if (is_null(hour)) {
    precision <- PRECISION_DAY
    fields <- list(year = year, day = day)
  } else if (is_null(minute)) {
    precision <- PRECISION_HOUR
    fields <- list(year = year, day = day, hour = hour)
  } else if (is_null(second)) {
    precision <- PRECISION_MINUTE
    fields <- list(year = year, day = day, hour = hour, minute = minute)
  } else if (is_null(subsecond)) {
    precision <- PRECISION_SECOND
    fields <- list(year = year, day = day, hour = hour, minute = minute, second = second)
  } else {
    precision <- calendar_validate_subsecond_precision(subsecond_precision)
    fields <- list(year = year, day = day, hour = hour, minute = minute, second = second, subsecond = subsecond)
  }

  if (is_last(fields$day)) {
    fields$day <- 1L
    last <- TRUE
  } else {
    last <- FALSE
  }

  fields <- vec_recycle_common(!!!fields)
  fields <- vec_cast_common(!!!fields, .to = integer())

  fields <- collect_year_day_fields(fields, precision)

  names <- NULL

  out <- new_year_day_from_fields(fields, precision, names)

  if (last) {
    out <- set_day(out, "last")
  }

  out
}

# ------------------------------------------------------------------------------

#' @export
vec_proxy.clock_year_day <- function(x, ...) {
  .Call(`_clock_clock_rcrd_proxy`, x)
}

#' @export
vec_restore.clock_year_day <- function(x, to, ...) {
  .Call(`_clock_year_day_restore`, x, to)
}

# ------------------------------------------------------------------------------

#' @export
format.clock_year_day <- function(x, ...) {
  out <- format_year_day_cpp(x, calendar_precision_attribute(x))
  names(out) <- names(x)
  out
}

#' @export
vec_ptype_full.clock_year_day <- function(x, ...) {
  calendar_ptype_full(x, "year_day")
}

#' @export
vec_ptype_abbr.clock_year_day <- function(x, ...) {
  calendar_ptype_abbr(x, "yd")
}

# ------------------------------------------------------------------------------

#' Is `x` a year-day?
#'
#' Check if `x` is a year-day.
#'
#' @param x `[object]`
#'
#'   An object.
#'
#' @return Returns `TRUE` if `x` inherits from `"clock_year_day"`,
#'   otherwise returns `FALSE`.
#'
#' @export
#' @examples
#' is_year_day(year_day(2019))
is_year_day <- function(x) {
  inherits(x, "clock_year_day")
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype.clock_year_day <- function(x, ...) {
  switch(
    calendar_precision_attribute(x) + 1L,
    clock_empty_year_day_year,
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    clock_empty_year_day_day,
    clock_empty_year_day_hour,
    clock_empty_year_day_minute,
    clock_empty_year_day_second,
    clock_empty_year_day_millisecond,
    clock_empty_year_day_microsecond,
    clock_empty_year_day_nanosecond,
    abort("Internal error: Invalid precision.")
  )
}

#' @export
vec_ptype2.clock_year_day.clock_year_day <- function(x, y, ...) {
  ptype2_calendar_and_calendar(x, y, ...)
}

#' @export
vec_cast.clock_year_day.clock_year_day <- function(x, to, ...) {
  cast_calendar_to_calendar(x, to, ...)
}

# ------------------------------------------------------------------------------

#' @export
calendar_is_valid_precision.clock_year_day <- function(x, precision) {
  year_day_is_valid_precision(precision)
}

year_day_is_valid_precision <- function(precision) {
  if (!is_valid_precision(precision)) {
    FALSE
  } else if (precision == PRECISION_YEAR) {
    TRUE
  } else if (precision >= PRECISION_DAY && precision <= PRECISION_NANOSECOND) {
    TRUE
  } else {
    FALSE
  }
}

# ------------------------------------------------------------------------------

#' @export
invalid_detect.clock_year_day <- function(x) {
  invalid_detect_year_day_cpp(x, calendar_precision_attribute(x))
}

#' @export
invalid_any.clock_year_day <- function(x) {
  invalid_any_year_day_cpp(x, calendar_precision_attribute(x))
}

#' @export
invalid_count.clock_year_day <- function(x) {
  invalid_count_year_day_cpp(x, calendar_precision_attribute(x))
}

#' @export
invalid_resolve.clock_year_day <- function(x, ..., invalid = NULL) {
  check_dots_empty()
  precision <- calendar_precision_attribute(x)
  invalid <- validate_invalid(invalid)
  fields <- invalid_resolve_year_day_cpp(x, precision, invalid)
  new_year_day_from_fields(fields, precision, names(x))
}

# ------------------------------------------------------------------------------

#' Getters: year-day
#'
#' @description
#' These are year-day methods for the [getter generics][clock-getters].
#'
#' - `get_year()` returns the Gregorian year.
#'
#' - `get_day()` returns the day of the year.
#'
#' - There are sub-daily getters for extracting more precise components.
#'
#' @param x `[clock_year_day]`
#'
#'   A year-day to get the component from.
#'
#' @return The component.
#'
#' @name year-day-getters
#' @examples
#' x <- year_day(2019, 101:105, 1, 20, 30)
#'
#' get_day(x)
#' get_second(x)
#'
#' # Cannot extract more precise components
#' y <- year_day(2019, 1)
#' try(get_hour(y))
#'
#' # Cannot extract components that don't exist for this calendar
#' try(get_quarter(x))
NULL

#' @rdname year-day-getters
#' @export
get_year.clock_year_day <- function(x) {
  field_year(x)
}

#' @rdname year-day-getters
#' @export
get_day.clock_year_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_DAY, "get_day")
  field_day(x)
}

#' @rdname year-day-getters
#' @export
get_hour.clock_year_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_HOUR, "get_hour")
  field_hour(x)
}

#' @rdname year-day-getters
#' @export
get_minute.clock_year_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_MINUTE, "get_minute")
  field_minute(x)
}

#' @rdname year-day-getters
#' @export
get_second.clock_year_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_SECOND, "get_second")
  field_second(x)
}

#' @rdname year-day-getters
#' @export
get_millisecond.clock_year_day <- function(x) {
  calendar_require_precision(x, PRECISION_MILLISECOND, "get_millisecond")
  field_subsecond(x)
}

#' @rdname year-day-getters
#' @export
get_microsecond.clock_year_day <- function(x) {
  calendar_require_precision(x, PRECISION_MICROSECOND, "get_microsecond")
  field_subsecond(x)
}

#' @rdname year-day-getters
#' @export
get_nanosecond.clock_year_day <- function(x) {
  calendar_require_precision(x, PRECISION_NANOSECOND, "get_nanosecond")
  field_subsecond(x)
}

# ------------------------------------------------------------------------------

#' Setters: year-day
#'
#' @description
#' These are year-day methods for the
#' [setter generics][clock-setters].
#'
#' - `set_year()` sets the Gregorian year.
#'
#' - `set_day()` sets the day of the year. Valid values are in the range
#'   of `[1, 366]`.
#'
#' - There are sub-daily setters for setting more precise components.
#'
#' @inheritParams rlang::args_dots_empty
#'
#' @param x `[clock_year_day]`
#'
#'   A year-day vector.
#'
#' @param value `[integer / "last"]`
#'
#'   The value to set the component to.
#'
#'   For `set_day()`, this can also be `"last"` to set the day to the
#'   last day of the year.
#'
#' @return `x` with the component set.
#'
#' @name year-day-setters
#' @examples
#' x <- year_day(2019)
#'
#' # Set the day
#' set_day(x, 12:14)
#'
#' # Set to the "last" day of the year
#' set_day(x, "last")
#'
#' # Set to an invalid day of the year
#' invalid <- set_day(x, 366)
#' invalid
#'
#' # Then resolve the invalid day by choosing the next valid day
#' invalid_resolve(invalid, invalid = "next")
#'
#' # Cannot set a component two levels more precise than where you currently are
#' try(set_hour(x, 5))
NULL

#' @rdname year-day-setters
#' @export
set_year.clock_year_day <- function(x, value, ...) {
  check_dots_empty()
  set_field_year_day(x, value, "year")
}

#' @rdname year-day-setters
#' @export
set_day.clock_year_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_YEAR, "set_day")
  set_field_year_day(x, value, "day")
}

#' @rdname year-day-setters
#' @export
set_hour.clock_year_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_DAY, "set_hour")
  set_field_year_day(x, value, "hour")
}

#' @rdname year-day-setters
#' @export
set_minute.clock_year_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_HOUR, "set_minute")
  set_field_year_day(x, value, "minute")
}

#' @rdname year-day-setters
#' @export
set_second.clock_year_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_MINUTE, "set_second")
  set_field_year_day(x, value, "second")
}

#' @rdname year-day-setters
#' @export
set_millisecond.clock_year_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c(PRECISION_SECOND, PRECISION_MILLISECOND), "set_millisecond")
  set_field_year_day(x, value, "millisecond")
}

#' @rdname year-day-setters
#' @export
set_microsecond.clock_year_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c(PRECISION_SECOND, PRECISION_MICROSECOND), "set_microsecond")
  set_field_year_day(x, value, "microsecond")
}

#' @rdname year-day-setters
#' @export
set_nanosecond.clock_year_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c(PRECISION_SECOND, PRECISION_NANOSECOND), "set_nanosecond")
  set_field_year_day(x, value, "nanosecond")
}

set_field_year_day <- function(x, value, component) {
  if (is_last(value) && identical(component, "day")) {
    return(set_field_year_day_last(x))
  }

  precision_fields <- calendar_precision_attribute(x)
  precision_value <- year_day_component_to_precision(component)
  precision_out <- precision_common2(precision_fields, precision_value)

  value <- vec_cast(value, integer(), x_arg = "value")
  args <- vec_recycle_common(x = x, value = value)
  x <- args$x
  value <- args$value

  result <- set_field_year_day_cpp(x, value, precision_fields, precision_value)
  fields <- result$fields
  field <- year_day_component_to_field(component)
  fields[[field]] <- result$value

  new_year_day_from_fields(fields, precision_out, names = names(x))
}

set_field_year_day_last <- function(x) {
  precision_fields <- calendar_precision_attribute(x)
  precision_out <- precision_common2(precision_fields, PRECISION_DAY)

  result <- set_field_year_day_last_cpp(x, precision_fields)
  fields <- result$fields
  fields[["day"]] <- result$value

  new_year_day_from_fields(fields, precision_out, names = names(x))
}

# ------------------------------------------------------------------------------

#' @export
calendar_name.clock_year_day <- function(x) {
  "year_day"
}

# ------------------------------------------------------------------------------

year_day_component_to_precision <- function(component) {
  switch(
    component,
    year = PRECISION_YEAR,
    day = PRECISION_DAY,
    hour = PRECISION_HOUR,
    minute = PRECISION_MINUTE,
    second = PRECISION_SECOND,
    millisecond = PRECISION_MILLISECOND,
    microsecond = PRECISION_MICROSECOND,
    nanosecond = PRECISION_NANOSECOND,
    abort("Internal error: Unknown component name.")
  )
}

year_day_component_to_field <- function(component) {
  switch (
    component,
    year = component,
    day = component,
    hour = component,
    minute = component,
    second = component,
    millisecond = "subsecond",
    microsecond = "subsecond",
    nanosecond = "subsecond",
    abort("Internal error: Unknown component name.")
  )
}

# ------------------------------------------------------------------------------

#' @rdname clock-arith
#' @method vec_arith clock_year_day
#' @export
vec_arith.clock_year_day <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_year_day", y)
}

#' @method vec_arith.clock_year_day MISSING
#' @export
vec_arith.clock_year_day.MISSING <- function(op, x, y, ...) {
  arith_calendar_and_missing(op, x, y, ...)
}

#' @method vec_arith.clock_year_day clock_year_day
#' @export
vec_arith.clock_year_day.clock_year_day <- function(op, x, y, ...) {
  arith_calendar_and_calendar(op, x, y, ..., calendar_minus_calendar_fn = year_day_minus_year_day)
}

#' @method vec_arith.clock_year_day clock_duration
#' @export
vec_arith.clock_year_day.clock_duration <- function(op, x, y, ...) {
  arith_calendar_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration clock_year_day
#' @export
vec_arith.clock_duration.clock_year_day <- function(op, x, y, ...) {
  arith_duration_and_calendar(op, x, y, ...)
}

#' @method vec_arith.clock_year_day numeric
#' @export
vec_arith.clock_year_day.numeric <- function(op, x, y, ...) {
  arith_calendar_and_numeric(op, x, y, ...)
}

#' @method vec_arith.numeric clock_year_day
#' @export
vec_arith.numeric.clock_year_day <- function(op, x, y, ...) {
  arith_numeric_and_calendar(op, x, y, ...)
}

year_day_minus_year_day <- function(op, x, y, ...) {
  args <- vec_recycle_common(x = x, y = y)
  args <- vec_cast_common(!!!args)
  x <- args$x
  y <- args$y

  names <- names_common(x, y)

  precision <- calendar_precision_attribute(x)

  if (precision > PRECISION_YEAR) {
    stop_incompatible_op(op, x, y, ...)
  }

  fields <- year_day_minus_year_day_cpp(x, y, precision)

  new_duration_from_fields(fields, precision, names)
}

# ------------------------------------------------------------------------------

#' Arithmetic: year-day
#'
#' @description
#' These are year-day methods for the
#' [arithmetic generics][clock-arithmetic].
#'
#' - `add_years()`
#'
#' Notably, _you cannot add days to a year-day_. For day-based arithmetic,
#' first convert to a time point with [as_naive_time()] or [as_sys_time()].
#'
#' @details
#' `x` and `n` are recycled against each other.
#'
#' @inheritParams add_years
#'
#' @param x `[clock_year_day]`
#'
#'   A year-day vector.
#'
#' @return `x` after performing the arithmetic.
#'
#' @name year-day-arithmetic
#'
#' @examples
#' x <- year_day(2019, 10)
#'
#' add_years(x, 1:5)
#'
#' # A valid day in a leap year
#' y <- year_day(2020, 366)
#' y
#'
#' # Adding 1 year to `y` generates an invalid date
#' y_plus <- add_years(y, 1)
#' y_plus
#'
#' # Invalid dates are fine, as long as they are eventually resolved
#' # by either manually resolving, or by calling `invalid_resolve()`
#'
#' # Resolve by returning the previous / next valid moment in time
#' invalid_resolve(y_plus, invalid = "previous")
#' invalid_resolve(y_plus, invalid = "next")
#'
#' # Manually resolve by setting to the last day of the year
#' invalid <- invalid_detect(y_plus)
#' y_plus[invalid] <- set_day(y_plus[invalid], "last")
#' y_plus
NULL

#' @rdname year-day-arithmetic
#' @export
add_years.clock_year_day <- function(x, n, ...) {
  year_day_plus_duration(x, n, PRECISION_YEAR)
}

year_day_plus_duration <- function(x, n, precision_n) {
  precision_fields <- calendar_precision_attribute(x)

  n <- duration_collect_n(n, precision_n)
  args <- vec_recycle_common(x = x, n = n)
  x <- args$x
  n <- args$n

  names <- names_common(x, n)

  fields <- year_day_plus_duration_cpp(x, n, precision_fields, precision_n)

  new_year_day_from_fields(fields, precision_fields, names = names)
}

# ------------------------------------------------------------------------------

#' Convert to year-day
#'
#' `as_year_day()` converts a vector to the year-day calendar.
#' Time points, Dates, POSIXct, and other calendars can all be converted to
#' year-day.
#'
#' @param x `[vector]`
#'
#'   A vector to convert to year-day.
#'
#' @return A year-day vector.
#' @export
#' @examples
#' # From Date
#' as_year_day(as.Date("2019-05-01"))
#'
#' # From POSIXct, which assumes that the naive time is what should be converted
#' as_year_day(as.POSIXct("2019-05-01 02:30:30", "America/New_York"))
#'
#' # From other calendars
#' as_year_day(year_quarter_day(2019, quarter = 2, day = 50))
as_year_day <- function(x)  {
  UseMethod("as_year_day")
}

#' @export
as_year_day.default <- function(x) {
  stop_clock_unsupported_conversion(x, "clock_year_day")
}

#' @export
as_year_day.clock_year_day <- function(x) {
  x
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_year_day <- function(x) {
  calendar_require_all_valid(x)
  precision <- calendar_precision_attribute(x)
  fields <- as_sys_time_year_day_cpp(x, precision)
  new_sys_time_from_fields(fields, precision, clock_rcrd_names(x))
}

#' @export
as_naive_time.clock_year_day <- function(x) {
  as_naive_time(as_sys_time(x))
}

#' @export
as.character.clock_year_day <- function(x, ...) {
  format(x)
}

# ------------------------------------------------------------------------------

#' @export
calendar_leap_year.clock_year_day <- function(x) {
  x <- get_year(x)
  gregorian_leap_year_cpp(x)
}

# ------------------------------------------------------------------------------

#' Grouping: year-day
#'
#' @description
#' This is a year-day method for the [calendar_group()] generic.
#'
#' Grouping for a year-day object can be done at any precision, as
#' long as `x` is at least as precise as `precision`.
#'
#' @inheritParams calendar_group
#'
#' @param x `[clock_year_day]`
#'
#'   A year-day vector.
#'
#' @param precision `[character(1)]`
#'
#'   One of:
#'
#'   - `"year"`
#'   - `"day"`
#'   - `"hour"`
#'   - `"minute"`
#'   - `"second"`
#'   - `"millisecond"`
#'   - `"microsecond"`
#'   - `"nanosecond"`
#'
#' @return `x` grouped at the specified `precision`.
#'
#' @name year-day-group
#'
#' @export
#' @examples
#' x <- seq(as_naive_time(year_month_day(2019, 1, 1)), by = 5, length.out = 20)
#' x <- as_year_day(x)
#' x
#'
#' # Group by day of the current year
#' calendar_group(x, "day", n = 20)
calendar_group.clock_year_day <- function(x, precision, ..., n = 1L) {
  n <- validate_calendar_group_n(n)
  x <- calendar_narrow(x, precision)

  precision <- validate_precision_string(precision)

  if (precision == PRECISION_YEAR) {
    value <- get_year(x)
    value <- group_component0(value, n)
    x <- set_year(x, value)
    return(x)
  }
  if (precision == PRECISION_DAY) {
    value <- get_day(x)
    value <- group_component1(value, n)
    x <- set_day(x, value)
    return(x)
  }

  x <- calendar_group_time(x, n, precision)
  x
}

# ------------------------------------------------------------------------------

#' Narrow: year-day
#'
#' This is a year-day method for the [calendar_narrow()] generic. It
#' narrows a year-day vector to the specified `precision`.
#'
#' @inheritParams year-day-group
#'
#' @return `x` narrowed to the supplied `precision`.
#'
#' @name year-day-narrow
#'
#' @export
#' @examples
#' # Hour precision
#' x <- year_day(2019, 3, 4)
#' x
#'
#' # Narrowed to day precision
#' calendar_narrow(x, "day")
#'
#' # Or year precision
#' calendar_narrow(x, "year")
#'
#' # Subsecond precision can be narrowed to second precision
#' milli <- calendar_widen(x, "millisecond")
#' micro <- calendar_widen(x, "microsecond")
#' milli
#' micro
#'
#' calendar_narrow(milli, "second")
#' calendar_narrow(micro, "second")
#'
#' # But once you have "locked in" a subsecond precision, it can't be
#' # narrowed to another subsecond precision
#' try(calendar_narrow(micro, "millisecond"))
calendar_narrow.clock_year_day <- function(x, precision) {
  precision <- validate_precision_string(precision)

  out_fields <- list()
  x_fields <- unclass(x)

  if (precision >= PRECISION_YEAR) {
    out_fields[["year"]] <- x_fields[["year"]]
  }
  if (precision >= PRECISION_DAY) {
    out_fields[["day"]] <- x_fields[["day"]]
  }

  out_fields <- calendar_narrow_time(out_fields, precision, x_fields)

  new_year_day_from_fields(out_fields, precision, names = names(x))
}

# ------------------------------------------------------------------------------

#' Widen: year-day
#'
#' This is a year-day method for the [calendar_widen()] generic. It
#' widens a year-day vector to the specified `precision`.
#'
#' @inheritParams year-day-group
#'
#' @return `x` widened to the supplied `precision`.
#'
#' @name year-day-widen
#'
#' @export
#' @examples
#' # Year precision
#' x <- year_day(2019)
#' x
#'
#' # Widen to day precision
#' calendar_widen(x, "day")
#'
#' # Or second precision
#' sec <- calendar_widen(x, "second")
#' sec
#'
#' # Second precision can be widened to subsecond precision
#' milli <- calendar_widen(sec, "millisecond")
#' micro <- calendar_widen(sec, "microsecond")
#' milli
#' micro
#'
#' # But once you have "locked in" a subsecond precision, it can't
#' # be widened again
#' try(calendar_widen(milli, "microsecond"))
calendar_widen.clock_year_day <- function(x, precision) {
  x_precision <- calendar_precision_attribute(x)
  precision <- validate_precision_string(precision)

  if (precision >= PRECISION_DAY && x_precision < PRECISION_DAY) {
    x <- set_day(x, 1L)
  }

  x <- calendar_widen_time(x, x_precision, precision)

  x
}

# ------------------------------------------------------------------------------

#' Boundaries: year-day
#'
#' This is a year-day method for the [calendar_start()] and
#' [calendar_end()] generics. They adjust components of a calendar to the
#' start or end of a specified `precision`.
#'
#' @inheritParams year-day-group
#'
#' @return `x` at the same precision, but with some components altered to be
#'   at the boundary value.
#'
#' @name year-day-boundary
#'
#' @examples
#' # Day precision
#' x <- year_day(2019:2020, 5)
#' x
#'
#' # Compute the last day of the year
#' calendar_end(x, "year")
NULL

#' @rdname year-day-boundary
#' @export
calendar_start.clock_year_day <- function(x, precision) {
  x_precision <- calendar_precision_attribute(x)
  precision <- validate_precision_string(precision)

  calendar_start_end_checks(x, x_precision, precision, "start")

  if (precision <= PRECISION_YEAR && x_precision > PRECISION_YEAR) {
    x <- set_day(x, 1L)
  }

  x <- calendar_start_time(x, x_precision, precision)

  x
}

#' @rdname year-day-boundary
#' @export
calendar_end.clock_year_day <- function(x, precision) {
  x_precision <- calendar_precision_attribute(x)
  precision <- validate_precision_string(precision)

  calendar_start_end_checks(x, x_precision, precision, "end")

  if (precision <= PRECISION_YEAR && x_precision > PRECISION_YEAR) {
    x <- set_day(x, "last")
  }

  x <- calendar_end_time(x, x_precision, precision)

  x
}

# ------------------------------------------------------------------------------

#' Counting: year-day
#'
#' This is a year-day method for the [calendar_count_between()] generic.
#' It counts the number of `precision` units between `start` and `end`
#' (i.e., the number of years).
#'
#' @inheritParams calendar-count-between
#'
#' @param start,end `[clock_year_day]`
#'
#'   A pair of year-day vectors. These will be recycled to their
#'   common size.
#'
#' @param precision `[character(1)]`
#'
#'   One of:
#'
#'   - `"year"`
#'
#' @inherit calendar-count-between return
#'
#' @name year-day-count-between
#'
#' @export
#' @examples
#' # Compute an individual's age in years
#' x <- year_day(2001, 100)
#' y <- year_day(2021, c(99, 101))
#'
#' calendar_count_between(x, y, "year")
#'
#' # Or in a whole number multiple of years
#' calendar_count_between(x, y, "year", n = 3)
calendar_count_between.clock_year_day <- function(start,
                                                  end,
                                                  precision,
                                                  ...,
                                                  n = 1L) {
  NextMethod()
}

calendar_count_between_standardize_precision_n.clock_year_day <- function(x,
                                                                          precision,
                                                                          n) {
  precision_int <- validate_precision_string(precision)

  allowed_precisions <- c(PRECISION_YEAR)

  if (!(precision_int %in% allowed_precisions)) {
    abort("`precision` must be one of: 'year'.")
  }

  list(precision = precision, n = n)
}

calendar_count_between_compute.clock_year_day <- function(start,
                                                          end,
                                                          precision) {
  precision <- validate_precision_string(precision)

  if (precision == PRECISION_YEAR) {
    out <- get_year(end) - get_year(start)
    return(out)
  }

  abort("Internal error: `precision` should be 'year' at this point.")
}

calendar_count_between_proxy_compare.clock_year_day <- function(start,
                                                                end,
                                                                precision) {
  precision <- validate_precision_string(precision)

  start <- vec_proxy_compare(start)
  end <- vec_proxy_compare(end)

  if (precision >= PRECISION_YEAR) {
    start$year <- NULL
    end$year <- NULL
  }

  list(start = start, end = end)
}

# ------------------------------------------------------------------------------

#' Sequences: year-day
#'
#' @description
#' This is a year-day method for the [seq()] generic.
#'
#' Sequences can only be generated for `"year"` precision year-day vectors.
#'
#' When calling `seq()`, exactly two of the following must be specified:
#' - `to`
#' - `by`
#' - Either `length.out` or `along.with`
#'
#' @inheritParams seq.clock_duration
#'
#' @param from `[clock_year_day(1)]`
#'
#'   A `"year"` precision year-day to start the sequence from.
#'
#'   `from` is always included in the result.
#'
#' @param to `[clock_year_day(1) / NULL]`
#'
#'   A `"year"` precision year-day to stop the sequence at.
#'
#'   `to` is cast to the type of `from`.
#'
#'   `to` is only included in the result if the resulting sequence divides
#'   the distance between `from` and `to` exactly.
#'
#' @return A sequence with the type of `from`.
#'
#' @export
#' @examples
#' # Yearly sequence
#' x <- seq(year_day(2020), year_day(2040), by = 2)
#' x
#'
#' # Which we can then set the day of to get a sequence of end-of-year values
#' set_day(x, "last")
#'
#' # Daily sequences are not allowed. Use a naive-time for this instead.
#' try(seq(year_day(2019, 1), by = 2, length.out = 2))
#' as_year_day(seq(as_naive_time(year_day(2019, 1)), by = 2, length.out = 2))
seq.clock_year_day <- function(from,
                               to = NULL,
                               by = NULL,
                               length.out = NULL,
                               along.with = NULL,
                               ...) {
  precision <- calendar_precision_attribute(from)

  if (precision > PRECISION_YEAR) {
    abort("`from` must be 'year' precision.")
  }

  seq_impl(
    from = from,
    to = to,
    by = by,
    length.out = length.out,
    along.with = along.with,
    precision = precision,
    ...
  )
}

# ------------------------------------------------------------------------------

clock_init_year_day_utils <- function(env) {
  year <- year_day(integer())

  assign("clock_empty_year_day_year", year, envir = env)
  assign("clock_empty_year_day_day", calendar_widen(year, "day"), envir = env)
  assign("clock_empty_year_day_hour", calendar_widen(year, "hour"), envir = env)
  assign("clock_empty_year_day_minute", calendar_widen(year, "minute"), envir = env)
  assign("clock_empty_year_day_second", calendar_widen(year, "second"), envir = env)
  assign("clock_empty_year_day_millisecond", calendar_widen(year, "millisecond"), envir = env)
  assign("clock_empty_year_day_microsecond", calendar_widen(year, "microsecond"), envir = env)
  assign("clock_empty_year_day_nanosecond", calendar_widen(year, "nanosecond"), envir = env)

  invisible(NULL)
}
