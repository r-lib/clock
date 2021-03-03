#' Calendar: year-month-day
#'
#' `year_month_day()` constructs the most common calendar type using the
#' Gregorian year, month, day, and time of day components.
#'
#' @details
#' Fields are recycled against each other.
#'
#' Fields are collected in order until the first `NULL` field is located. No
#' fields after the first `NULL` field are used.
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param year `[integer]`
#'
#'   The year. Values `[-32767, 32767]` are generally allowed.
#'
#' @param month `[integer / NULL]`
#'
#'   The month. Values `[1, 12]` are allowed.
#'
#' @param day `[integer / "last" / NULL]`
#'
#'   The day of the month. Values `[1, 31]` are allowed.
#'
#'   If `"last"`, then the last day of the month is returned.
#'
#' @param hour `[integer / NULL]`
#'
#'   The hour. Values `[0, 23]` are allowed.
#'
#' @param minute `[integer / NULL]`
#'
#'   The minute. Values `[0, 59]` are allowed.
#'
#' @param second `[integer / NULL]`
#'
#'   The second. Values `[0, 59]` are allowed.
#'
#' @param subsecond `[integer / NULL]`
#'
#'   The subsecond. If specified, `subsecond_precision` must also be specified
#'   to determine how to interpret the `subsecond`.
#'
#'   If using milliseconds, values `[0, 999]` are allowed.
#'
#'   If using microseconds, values `[0, 999999]` are allowed.
#'
#'   If using nanoseconds, values `[0, 999999999]` are allowed.
#'
#' @param subsecond_precision `[character(1) / NULL]`
#'
#'   The precision to interpret `subsecond` as. One of: `"millisecond"`,
#'   `"microsecond"`, or `"nanosecond"`.
#'
#' @return A year-month-day calendar vector.
#'
#' @export
#' @examples
#' # Just the year
#' x <- year_month_day(2019:2025)
#'
#' # Year-month type
#' year_month_day(2020, 1:12)
#'
#' # The most common use case involves year, month, and day fields
#' x <- year_month_day(2020, clock_months$january, 1:5)
#' x
#'
#' # Precision can go all the way out to nanosecond
#' year_month_day(2019, 1, 2, 2, 40, 45, 200, subsecond_precision = "nanosecond")
year_month_day <- function(year,
                           month = NULL,
                           day = NULL,
                           hour = NULL,
                           minute = NULL,
                           second = NULL,
                           subsecond = NULL,
                           ...,
                           subsecond_precision = NULL) {
  check_dots_empty()

  # Stop on the first `NULL` argument
  if (is_null(month)) {
    precision <- PRECISION_YEAR
    fields <- list(year = year)
  } else if (is_null(day)) {
    precision <- PRECISION_MONTH
    fields <- list(year = year, month = month)
  } else if (is_null(hour)) {
    precision <- PRECISION_DAY
    fields <- list(year = year, month = month, day = day)
  } else if (is_null(minute)) {
    precision <- PRECISION_HOUR
    fields <- list(year = year, month = month, day = day, hour = hour)
  } else if (is_null(second)) {
    precision <- PRECISION_MINUTE
    fields <- list(year = year, month = month, day = day, hour = hour, minute = minute)
  } else if (is_null(subsecond)) {
    precision <- PRECISION_SECOND
    fields <- list(year = year, month = month, day = day, hour = hour, minute = minute, second = second)
  } else {
    precision <- calendar_validate_subsecond_precision(subsecond_precision)
    fields <- list(year = year, month = month, day = day, hour = hour, minute = minute, second = second, subsecond = subsecond)
  }

  if (is_last(fields$day)) {
    fields$day <- 1L
    last <- TRUE
  } else {
    last <- FALSE
  }

  fields <- vec_recycle_common(!!!fields)
  fields <- vec_cast_common(!!!fields, .to = integer())

  fields <- collect_year_month_day_fields(fields, precision)

  names <- NULL

  out <- new_year_month_day_from_fields(fields, precision, names)

  if (last) {
    out <- set_day(out, "last")
  }

  out
}

# ------------------------------------------------------------------------------

#' @export
vec_proxy.clock_year_month_day <- function(x, ...) {
  .Call(`_clock_clock_rcrd_proxy`, x)
}

#' @export
vec_restore.clock_year_month_day <- function(x, to, ...) {
  .Call(`_clock_year_month_day_restore`, x, to)
}

# ------------------------------------------------------------------------------

#' @export
format.clock_year_month_day <- function(x, ...) {
  out <- format_year_month_day_cpp(x, calendar_precision_attribute(x))
  names(out) <- names(x)
  out
}

#' @export
vec_ptype_full.clock_year_month_day <- function(x, ...) {
  calendar_ptype_full(x, "year_month_day")
}

#' @export
vec_ptype_abbr.clock_year_month_day <- function(x, ...) {
  calendar_ptype_abbr(x, "ymd")
}

# ------------------------------------------------------------------------------

#' Parsing: year-month-day
#'
#' @description
#' `year_month_day_parse()` parses strings into a year-month-day.
#'
#' The default options assume `x` should be parsed at day precision, using a
#' `format` string of `"%Y-%m-%d"`.
#'
#' @details
#' `year_month_day_parse()` completely ignores the `%z` and `%Z` commands.
#'
#' @inheritParams zoned-parsing
#'
#' @param x `[character]`
#'
#'   A character vector to parse.
#'
#' @param precision `[character(1)]`
#'
#'   A precision for the resulting year-month-day. One of:
#'
#'   - `"year"`
#'   - `"month"`
#'   - `"day"`
#'   - `"hour"`
#'   - `"minute"`
#'   - `"second"`
#'   - `"millisecond"`
#'   - `"microsecond"`
#'   - `"nanosecond"`
#'
#'   Setting the `precision` determines how much information `%S` attempts
#'   to parse.
#'
#' @param locale `[clock_locale]`
#'
#'   A locale object created by [clock_locale()].
#'
#' @return A year-month-day calendar vector. If a parsing fails, `NA` is
#'   returned.
#'
#' @export
#' @examples
#' x <- "2019-01-01"
#'
#' # Default parses at day precision
#' year_month_day_parse(x)
#'
#' # Can parse at less precise precisions too
#' year_month_day_parse(x, precision = "month")
#' year_month_day_parse(x, precision = "year")
#'
#' # Even invalid dates can be round-tripped through format<->parse calls
#' invalid <- year_month_day(2019, 2, 30)
#' year_month_day_parse(format(invalid))
#'
#' # Can parse with time of day
#' year_month_day_parse(
#'   "2019-01-30 02:30:00.123456789",
#'   precision = "nanosecond"
#' )
#'
#' # Can parse using multiple format strings, which will be tried
#' # in the order they are provided
#' x <- c("2019-01-01", "2020-01-01", "2021/2/3")
#' formats <- c("%Y-%m-%d", "%Y/%m/%d")
#' year_month_day_parse(x, format = formats)
#'
#' # Can parse using other format tokens as well
#' year_month_day_parse(
#'   "January, 2019",
#'   format = "%B, %Y",
#'   precision = "month"
#' )
#'
#' # Parsing a French year-month-day
#' year_month_day_parse(
#'   "octobre 1, 2000",
#'   format = "%B %d, %Y",
#'   locale = clock_locale("fr")
#' )
year_month_day_parse <- function(x,
                                 ...,
                                 format = NULL,
                                 precision = "day",
                                 locale = clock_locale()) {
  check_dots_empty()

  if (!is_character(x)) {
    abort("`x` must be a character vector.")
  }

  precision <- validate_precision_string(precision)
  if (!year_month_day_is_valid_precision(precision)) {
    abort("`precision` must be a valid precision for 'year_month_day'.")
  }

  if (is_null(format)) {
    format <- year_month_day_format(precision)
  }

  if (!is_clock_locale(locale)) {
    abort("`locale` must be a 'clock_locale' object.")
  }

  labels <- locale$labels
  mark <- locale$decimal_mark

  fields <- year_month_day_parse_cpp(
    x,
    format,
    precision,
    labels$month,
    labels$month_abbrev,
    labels$weekday,
    labels$weekday_abbrev,
    labels$am_pm,
    mark
  )

  new_year_month_day_from_fields(fields, precision, names(x))
}

year_month_day_format <- function(precision) {
  precision <- precision_to_string(precision)

  switch(
    precision,
    year = "%Y",
    month = "%Y-%m",
    day = "%Y-%m-%d",
    hour = "%Y-%m-%d %H",
    minute = "%Y-%m-%d %H:%M",
    second = ,
    millisecond = ,
    microsecond = ,
    nanosecond = "%Y-%m-%d %H:%M:%S",
    abort("Internal error: Unknown precision.")
  )
}

# ------------------------------------------------------------------------------

#' Is `x` a year-month-day?
#'
#' Check if `x` is a year-month-day.
#'
#' @param x `[object]`
#'
#'   An object.
#'
#' @return Returns `TRUE` if `x` inherits from `"clock_year_month_day"`,
#'   otherwise returns `FALSE`.
#'
#' @export
#' @examples
#' is_year_month_day(year_month_day(2019))
is_year_month_day <- function(x) {
  inherits(x, "clock_year_month_day")
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype.clock_year_month_day <- function(x, ...) {
  switch(
    calendar_precision_attribute(x) + 1L,
    clock_empty_year_month_day_year,
    abort("Internal error: Invalid precision"),
    clock_empty_year_month_day_month,
    abort("Internal error: Invalid precision"),
    clock_empty_year_month_day_day,
    clock_empty_year_month_day_hour,
    clock_empty_year_month_day_minute,
    clock_empty_year_month_day_second,
    clock_empty_year_month_day_millisecond,
    clock_empty_year_month_day_microsecond,
    clock_empty_year_month_day_nanosecond,
    abort("Internal error: Invalid precision.")
  )
}

#' @export
vec_ptype2.clock_year_month_day.clock_year_month_day <- function(x, y, ...) {
  ptype2_calendar_and_calendar(x, y, ...)
}

#' @export
vec_cast.clock_year_month_day.clock_year_month_day <- function(x, to, ...) {
  cast_calendar_to_calendar(x, to, ...)
}

# ------------------------------------------------------------------------------

#' @export
calendar_is_valid_precision.clock_year_month_day <- function(x, precision) {
  year_month_day_is_valid_precision(precision)
}

year_month_day_is_valid_precision <- function(precision) {
  if (!is_valid_precision(precision)) {
    FALSE
  } else if (precision == PRECISION_YEAR || precision == PRECISION_MONTH) {
    TRUE
  } else if (precision >= PRECISION_DAY && precision <= PRECISION_NANOSECOND) {
    TRUE
  } else {
    FALSE
  }
}

# ------------------------------------------------------------------------------

#' @export
invalid_detect.clock_year_month_day <- function(x) {
  invalid_detect_year_month_day_cpp(x, calendar_precision_attribute(x))
}

#' @export
invalid_any.clock_year_month_day <- function(x) {
  invalid_any_year_month_day_cpp(x, calendar_precision_attribute(x))
}

#' @export
invalid_count.clock_year_month_day <- function(x) {
  invalid_count_year_month_day_cpp(x, calendar_precision_attribute(x))
}

#' @export
invalid_resolve.clock_year_month_day <- function(x, ..., invalid = NULL) {
  check_dots_empty()
  precision <- calendar_precision_attribute(x)
  invalid <- validate_invalid(invalid)
  fields <- invalid_resolve_year_month_day_cpp(x, precision, invalid)
  new_year_month_day_from_fields(fields, precision, names(x))
}

# ------------------------------------------------------------------------------

#' Getters: year-month-day
#'
#' @description
#' These are year-month-day methods for the [getter generics][clock-getters].
#'
#' - `get_year()` returns the Gregorian year.
#'
#' - `get_month()` returns the month of the year.
#'
#' - `get_day()` returns the day of the month.
#'
#' - There are sub-daily getters for extracting more precise components.
#'
#' @param x `[clock_year_month_day]`
#'
#'   A year-month-day to get the component from.
#'
#' @return The component.
#'
#' @name year-month-day-getters
#' @examples
#' x <- year_month_day(2019, 1:3, 5:7, 1, 20, 30)
#'
#' get_month(x)
#' get_day(x)
#' get_second(x)
#'
#' # Cannot extract more precise components
#' y <- year_month_day(2019, 1)
#' try(get_day(y))
#'
#' # Cannot extract components that don't exist for this calendar
#' try(get_quarter(x))
NULL

#' @rdname year-month-day-getters
#' @export
get_year.clock_year_month_day <- function(x) {
  field_year(x)
}

#' @rdname year-month-day-getters
#' @export
get_month.clock_year_month_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_MONTH, "get_month")
  field_month(x)
}

#' @rdname year-month-day-getters
#' @export
get_day.clock_year_month_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_DAY, "get_day")
  field_day(x)
}

#' @rdname year-month-day-getters
#' @export
get_hour.clock_year_month_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_HOUR, "get_hour")
  field_hour(x)
}

#' @rdname year-month-day-getters
#' @export
get_minute.clock_year_month_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_MINUTE, "get_minute")
  field_minute(x)
}

#' @rdname year-month-day-getters
#' @export
get_second.clock_year_month_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_SECOND, "get_second")
  field_second(x)
}

#' @rdname year-month-day-getters
#' @export
get_millisecond.clock_year_month_day <- function(x) {
  calendar_require_precision(x, PRECISION_MILLISECOND, "get_millisecond")
  field_subsecond(x)
}

#' @rdname year-month-day-getters
#' @export
get_microsecond.clock_year_month_day <- function(x) {
  calendar_require_precision(x, PRECISION_MICROSECOND, "get_microsecond")
  field_subsecond(x)
}

#' @rdname year-month-day-getters
#' @export
get_nanosecond.clock_year_month_day <- function(x) {
  calendar_require_precision(x, PRECISION_NANOSECOND, "get_nanosecond")
  field_subsecond(x)
}

# ------------------------------------------------------------------------------

#' Setters: year-month-day
#'
#' @description
#' These are year-month-day methods for the
#' [setter generics][clock-setters].
#'
#' - `set_year()` sets the Gregorian year.
#'
#' - `set_month()` sets the month of the year. Valid values are in the range
#'   of `[1, 12]`.
#'
#' - `set_day()` sets the day of the month. Valid values are in the range
#'   of `[1, 31]`.
#'
#' - There are sub-daily setters for setting more precise components.
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param x `[clock_year_month_day]`
#'
#'   A year-month-day vector.
#'
#' @param value `[integer / "last"]`
#'
#'   The value to set the component to.
#'
#'   For `set_day()`, this can also be `"last"` to set the day to the
#'   last day of the month.
#'
#' @return `x` with the component set.
#'
#' @name year-month-day-setters
#' @examples
#' x <- year_month_day(2019, 1:3)
#'
#' # Set the day
#' set_day(x, 12:14)
#'
#' # Set to the "last" day of the month
#' set_day(x, "last")
#'
#' # Set to an invalid day of the month
#' invalid <- set_day(x, 31)
#' invalid
#'
#' # Then resolve the invalid day by choosing the next valid day
#' invalid_resolve(invalid, invalid = "next")
#'
#' # Cannot set a component two levels more precise than where you currently are
#' try(set_hour(x, 5))
NULL

#' @rdname year-month-day-setters
#' @export
set_year.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  set_field_year_month_day(x, value, "year")
}

#' @rdname year-month-day-setters
#' @export
set_month.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_YEAR, "set_month")
  set_field_year_month_day(x, value, "month")
}

#' @rdname year-month-day-setters
#' @export
set_day.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_MONTH, "set_day")
  set_field_year_month_day(x, value, "day")
}

#' @rdname year-month-day-setters
#' @export
set_hour.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_DAY, "set_hour")
  set_field_year_month_day(x, value, "hour")
}

#' @rdname year-month-day-setters
#' @export
set_minute.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_HOUR, "set_minute")
  set_field_year_month_day(x, value, "minute")
}

#' @rdname year-month-day-setters
#' @export
set_second.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_MINUTE, "set_second")
  set_field_year_month_day(x, value, "second")
}

#' @rdname year-month-day-setters
#' @export
set_millisecond.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c(PRECISION_SECOND, PRECISION_MILLISECOND), "set_millisecond")
  set_field_year_month_day(x, value, "millisecond")
}

#' @rdname year-month-day-setters
#' @export
set_microsecond.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c(PRECISION_SECOND, PRECISION_MICROSECOND), "set_microsecond")
  set_field_year_month_day(x, value, "microsecond")
}

#' @rdname year-month-day-setters
#' @export
set_nanosecond.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c(PRECISION_SECOND, PRECISION_NANOSECOND), "set_nanosecond")
  set_field_year_month_day(x, value, "nanosecond")
}

set_field_year_month_day <- function(x, value, component) {
  if (is_last(value) && identical(component, "day")) {
    return(set_field_year_month_day_last(x))
  }

  precision_fields <- calendar_precision_attribute(x)
  precision_value <- year_month_day_component_to_precision(component)
  precision_out <- precision_common2(precision_fields, precision_value)

  value <- vec_cast(value, integer(), x_arg = "value")
  args <- vec_recycle_common(x = x, value = value)
  x <- args$x
  value <- args$value

  result <- set_field_year_month_day_cpp(x, value, precision_fields, precision_value)
  fields <- result$fields
  field <- year_month_day_component_to_field(component)
  fields[[field]] <- result$value

  new_year_month_day_from_fields(fields, precision_out, names = names(x))
}

set_field_year_month_day_last <- function(x) {
  precision_fields <- calendar_precision_attribute(x)
  precision_out <- precision_common2(precision_fields, PRECISION_DAY)

  result <- set_field_year_month_day_last_cpp(x, precision_fields)
  fields <- result$fields
  fields[["day"]] <- result$value

  new_year_month_day_from_fields(fields, precision_out, names = names(x))
}

# ------------------------------------------------------------------------------

#' @export
calendar_name.clock_year_month_day <- function(x) {
  "year_month_day"
}

# ------------------------------------------------------------------------------

year_month_day_component_to_precision <- function(component) {
  switch(
    component,
    year = PRECISION_YEAR,
    month = PRECISION_MONTH,
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

year_month_day_component_to_field <- function(component) {
  switch (
    component,
    year = component,
    month = component,
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

#' Support for vctrs arithmetic
#'
#' @inheritParams vctrs::vec_arith
#' @name clock-arith
#'
#' @return The result of the arithmetic operation.
#' @examples
#' vctrs::vec_arith("+", year_month_day(2019), 1)
NULL

#' @rdname clock-arith
#' @method vec_arith clock_year_month_day
#' @export
vec_arith.clock_year_month_day <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_year_month_day", y)
}

#' @method vec_arith.clock_year_month_day MISSING
#' @export
vec_arith.clock_year_month_day.MISSING <- function(op, x, y, ...) {
  arith_calendar_and_missing(op, x, y, ...)
}

#' @method vec_arith.clock_year_month_day clock_year_month_day
#' @export
vec_arith.clock_year_month_day.clock_year_month_day <- function(op, x, y, ...) {
  arith_calendar_and_calendar(op, x, y, ..., calendar_minus_calendar_fn = year_month_day_minus_year_month_day)
}

#' @method vec_arith.clock_year_month_day clock_duration
#' @export
vec_arith.clock_year_month_day.clock_duration <- function(op, x, y, ...) {
  arith_calendar_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration clock_year_month_day
#' @export
vec_arith.clock_duration.clock_year_month_day <- function(op, x, y, ...) {
  arith_duration_and_calendar(op, x, y, ...)
}

#' @method vec_arith.clock_year_month_day numeric
#' @export
vec_arith.clock_year_month_day.numeric <- function(op, x, y, ...) {
  arith_calendar_and_numeric(op, x, y, ...)
}

#' @method vec_arith.numeric clock_year_month_day
#' @export
vec_arith.numeric.clock_year_month_day <- function(op, x, y, ...) {
  arith_numeric_and_calendar(op, x, y, ...)
}

year_month_day_minus_year_month_day <- function(op, x, y, ...) {
  args <- vec_recycle_common(x = x, y = y)
  args <- vec_cast_common(!!!args)
  x <- args$x
  y <- args$y

  names <- names_common(x, y)

  precision <- calendar_precision_attribute(x)

  if (precision > PRECISION_MONTH) {
    stop_incompatible_op(op, x, y, ...)
  }

  fields <- year_month_day_minus_year_month_day_cpp(x, y, precision)

  new_duration_from_fields(fields, precision, names)
}

# ------------------------------------------------------------------------------

#' Arithmetic: year-month-day
#'
#' @description
#' These are year-month-day methods for the
#' [arithmetic generics][clock-arithmetic].
#'
#' - `add_years()`
#'
#' - `add_quarters()`
#'
#' - `add_months()`
#'
#' Notably, _you cannot add days to a year-month-day_. For day-based arithmetic,
#' first convert to a time point with [as_naive_time()] or [as_sys_time()].
#'
#' @details
#' Adding a single quarter with `add_quarters()` is equivalent to adding
#' 3 months.
#'
#' `x` and `n` are recycled against each other.
#'
#' @inheritParams add_years
#'
#' @param x `[clock_year_month_day]`
#'
#'   A year-month-day vector.
#'
#' @return `x` after performing the arithmetic.
#'
#' @name year-month-day-arithmetic
#'
#' @examples
#' x <- year_month_day(2019, 1, 1)
#'
#' add_years(x, 1:5)
#'
#' y <- year_month_day(2019, 1, 31)
#'
#' # Adding 1 month to `y` generates an invalid date
#' y_plus <- add_months(y, 1:2)
#' y_plus
#'
#' # Invalid dates are fine, as long as they are eventually resolved
#' # by either manually resolving, or by calling `invalid_resolve()`
#'
#' # Resolve by returning the previous / next valid moment in time
#' invalid_resolve(y_plus, invalid = "previous")
#' invalid_resolve(y_plus, invalid = "next")
#'
#' # Manually resolve by setting to the last day of the month
#' invalid <- invalid_detect(y_plus)
#' y_plus[invalid] <- set_day(y_plus[invalid], "last")
#' y_plus
NULL

#' @rdname year-month-day-arithmetic
#' @export
add_years.clock_year_month_day <- function(x, n, ...) {
  year_month_day_plus_duration(x, n, PRECISION_YEAR)
}

#' @rdname year-month-day-arithmetic
#' @export
add_quarters.clock_year_month_day <- function(x, n, ...) {
  calendar_require_minimum_precision(x, PRECISION_MONTH, "add_quarters")
  year_month_day_plus_duration(x, n, PRECISION_QUARTER)
}

#' @rdname year-month-day-arithmetic
#' @export
add_months.clock_year_month_day <- function(x, n, ...) {
  calendar_require_minimum_precision(x, PRECISION_MONTH, "add_months")
  year_month_day_plus_duration(x, n, PRECISION_MONTH)
}

year_month_day_plus_duration <- function(x, n, precision_n) {
  precision_fields <- calendar_precision_attribute(x)

  n <- duration_collect_n(n, precision_n)
  args <- vec_recycle_common(x = x, n = n)
  x <- args$x
  n <- args$n

  names <- names_common(x, n)

  fields <- year_month_day_plus_duration_cpp(x, n, precision_fields, precision_n)

  new_year_month_day_from_fields(fields, precision_fields, names = names)
}

# ------------------------------------------------------------------------------

#' Convert to year-month-day
#'
#' `as_year_month_day()` converts a vector to the year-month-day calendar.
#' Time points, Dates, POSIXct, and other calendars can all be converted to
#' year-month-day.
#'
#' @param x `[vector]`
#'
#'   A vector to convert to year-month-day.
#'
#' @return A year-month-day vector.
#' @export
#' @examples
#' # From Date
#' as_year_month_day(as.Date("2019-01-01"))
#'
#' # From POSIXct, which assumes that the naive time is what should be converted
#' as_year_month_day(as.POSIXct("2019-01-01 02:30:30", "America/New_York"))
#'
#' # From other calendars
#' as_year_month_day(year_quarter_day(2019, quarter = 2, day = 50))
as_year_month_day <- function(x)  {
  UseMethod("as_year_month_day")
}

#' @export
as_year_month_day.default <- function(x) {
  stop_clock_unsupported_conversion(x, "clock_year_month_day")
}

#' @export
as_year_month_day.clock_year_month_day <- function(x) {
  x
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_year_month_day <- function(x) {
  calendar_require_all_valid(x)
  precision <- calendar_precision_attribute(x)
  fields <- as_sys_time_year_month_day_cpp(x, precision)
  new_sys_time_from_fields(fields, precision, clock_rcrd_names(x))
}

#' @export
as_naive_time.clock_year_month_day <- function(x) {
  as_naive_time(as_sys_time(x))
}

#' @export
as.character.clock_year_month_day <- function(x, ...) {
  format(x)
}

# ------------------------------------------------------------------------------

#' @export
calendar_leap_year.clock_year_month_day <- function(x) {
  x <- get_year(x)
  gregorian_leap_year_cpp(x)
}

# ------------------------------------------------------------------------------

#' @export
calendar_month_factor.clock_year_month_day <- function(x,
                                                       ...,
                                                       labels = "en",
                                                       abbreviate = FALSE) {
  calendar_month_factor_impl(x, labels, abbreviate, ...)
}

# ------------------------------------------------------------------------------

#' Grouping: year-month-day
#'
#' @description
#' This is a year-month-day method for the [calendar_group()] generic.
#'
#' Grouping for a year-month-day object can be done at any precision, as
#' long as `x` is at least as precise as `precision`.
#'
#' @inheritParams calendar_group
#'
#' @param x `[clock_year_month_day]`
#'
#'   A year-month-day vector.
#'
#' @param precision `[character(1)]`
#'
#'   One of:
#'
#'   - `"year"`
#'   - `"month"`
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
#' @name year-month-day-group
#'
#' @export
#' @examples
#' steps <- duration_days(seq(0, 100, by = 5))
#' x <- year_month_day(2019, 1, 1)
#' x <- as_naive_time(x) + steps
#' x <- as_year_month_day(x)
#' x
#'
#' # Group by a single month
#' calendar_group(x, "month")
#'
#' # Or multiple months
#' calendar_group(x, "month", n = 2)
#'
#' # Group 3 days of the month together
#' y <- year_month_day(2019, 1, 1:12)
#' calendar_group(y, "day", n = 3)
#'
#' # Group by 5 nanosecond of the current second
#' z <- year_month_day(
#'   2019, 1, 2, 1, 5, 20, 1:20,
#'   subsecond_precision = "nanosecond"
#' )
#' calendar_group(z, "nanosecond", n = 5)
calendar_group.clock_year_month_day <- function(x, precision, ..., n = 1L) {
  n <- validate_calendar_group_n(n)
  x <- calendar_narrow(x, precision)

  precision <- validate_precision_string(precision)

  if (precision == PRECISION_YEAR) {
    value <- get_year(x)
    value <- group_component0(value, n)
    x <- set_year(x, value)
    return(x)
  }
  if (precision == PRECISION_MONTH) {
    value <- get_month(x)
    value <- group_component1(value, n)
    x <- set_month(x, value)
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

#' Narrow: year-month-day
#'
#' This is a year-month-day method for the [calendar_narrow()] generic. It
#' narrows a year-month-day vector to the specified `precision`.
#'
#' @inheritParams year-month-day-group
#'
#' @return `x` narrowed to the supplied `precision`.
#'
#' @name year-month-day-narrow
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
calendar_narrow.clock_year_month_day <- function(x, precision) {
  precision <- validate_precision_string(precision)

  out_fields <- list()
  x_fields <- unclass(x)

  if (precision >= PRECISION_YEAR) {
    out_fields[["year"]] <- x_fields[["year"]]
  }
  if (precision >= PRECISION_MONTH) {
    out_fields[["month"]] <- x_fields[["month"]]
  }
  if (precision >= PRECISION_DAY) {
    out_fields[["day"]] <- x_fields[["day"]]
  }

  out_fields <- calendar_narrow_time(out_fields, precision, x_fields)

  new_year_month_day_from_fields(out_fields, precision, names = names(x))
}

# ------------------------------------------------------------------------------

#' Widen: year-month-day
#'
#' This is a year-month-day method for the [calendar_widen()] generic. It
#' widens a year-month-day vector to the specified `precision`.
#'
#' @inheritParams year-month-day-group
#'
#' @return `x` widened to the supplied `precision`.
#'
#' @name year-month-day-widen
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
calendar_widen.clock_year_month_day <- function(x, precision) {
  x_precision <- calendar_precision_attribute(x)
  precision <- validate_precision_string(precision)

  if (precision >= PRECISION_MONTH && x_precision < PRECISION_MONTH) {
    x <- set_month(x, 1L)
  }
  if (precision >= PRECISION_DAY && x_precision < PRECISION_DAY) {
    x <- set_day(x, 1L)
  }

  x <- calendar_widen_time(x, x_precision, precision)

  x
}

# ------------------------------------------------------------------------------

#' Sequences: year-month-day
#'
#' @description
#' This is a year-month-day method for the [seq()] generic.
#'
#' Sequences can only be generated for `"year"` and `"month"` precision
#' year-month-day vectors.
#'
#' When calling `seq()`, exactly two of the following must be specified:
#' - `to`
#' - `by`
#' - Either `length.out` or `along.with`
#'
#' @inheritParams seq.clock_duration
#'
#' @param from `[clock_year_month_day(1)]`
#'
#'   A `"year"` or `"month"` precision year-month-day to start the sequence
#'   from.
#'
#'   `from` is always included in the result.
#'
#' @param to `[clock_year_month_day(1) / NULL]`
#'
#'   A `"year"` or `"month"` precision year-month-day to stop the sequence
#'   at.
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
#' # Monthly sequence
#' x <- seq(year_month_day(2019, 1), year_month_day(2020, 12), by = 1)
#' x
#'
#' # Which we can then set the day of to get a sequence of end-of-month values
#' set_day(x, "last")
#'
#' # Daily sequences are not allowed. Use a naive-time for this instead.
#' try(seq(year_month_day(2019, 1, 1), by = 2, length.out = 2))
#' seq(as_naive_time(year_month_day(2019, 1, 1)), by = 2, length.out = 2)
seq.clock_year_month_day <- function(from,
                                     to = NULL,
                                     by = NULL,
                                     length.out = NULL,
                                     along.with = NULL,
                                     ...) {
  precision <- calendar_precision_attribute(from)

  if (precision > PRECISION_MONTH) {
    abort("`from` must be 'year' or 'month' precision.")
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

clock_init_year_month_day_utils <- function(env) {
  year <- year_month_day(integer())

  assign("clock_empty_year_month_day_year", year, envir = env)
  assign("clock_empty_year_month_day_month", calendar_widen(year, "month"), envir = env)
  assign("clock_empty_year_month_day_day", calendar_widen(year, "day"), envir = env)
  assign("clock_empty_year_month_day_hour", calendar_widen(year, "hour"), envir = env)
  assign("clock_empty_year_month_day_minute", calendar_widen(year, "minute"), envir = env)
  assign("clock_empty_year_month_day_second", calendar_widen(year, "second"), envir = env)
  assign("clock_empty_year_month_day_millisecond", calendar_widen(year, "millisecond"), envir = env)
  assign("clock_empty_year_month_day_microsecond", calendar_widen(year, "microsecond"), envir = env)
  assign("clock_empty_year_month_day_nanosecond", calendar_widen(year, "nanosecond"), envir = env)

  invisible(NULL)
}
