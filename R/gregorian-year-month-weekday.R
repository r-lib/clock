#' Calendar: year-month-weekday
#'
#' `year_month_weekday()` constructs a calendar vector from the Gregorian
#' year, month, weekday, and index specifying that this is the n-th weekday
#' of the month.
#'
#' @details
#' Fields are recycled against each other using
#' [tidyverse recycling rules][vctrs::vector_recycling_rules].
#'
#' Fields are collected in order until the first `NULL` field is located. No
#' fields after the first `NULL` field are used.
#'
#' @inheritParams year_month_day
#'
#' @param day `[integer / NULL]`
#'
#'   The weekday of the month. Values `[1, 7]` are allowed, where `1` is
#'   Sunday and `7` is Saturday.
#'
#' @param index `[integer / "last" / NULL]`
#'
#'   The index specifying that `day` is the n-th weekday of the month.
#'   Values `[1, 5]` are allowed.
#'
#'   If `"last"`, then the last instance of `day` in the current month
#'   is returned.
#'
#' @return A year-month-weekday calendar vector.
#'
#' @export
#' @examples
#' # All Fridays in January, 2019
#' # Note that there was no 5th Friday in January
#' x <- year_month_weekday(
#'   2019,
#'   clock_months$january,
#'   clock_weekdays$friday,
#'   1:5
#' )
#' x
#'
#' invalid_detect(x)
#'
#' # Resolve this invalid date by using the previous valid date
#' invalid_resolve(x, invalid = "previous")
year_month_weekday <- function(year,
                               month = NULL,
                               day = NULL,
                               index = NULL,
                               hour = NULL,
                               minute = NULL,
                               second = NULL,
                               subsecond = NULL,
                               ...,
                               subsecond_precision = NULL) {
  if (xor(is_null(day), is_null(index))) {
    abort("If either `day` or `index` is specified, both must be specified.")
  }

  # Stop on the first `NULL` argument
  if (is_null(month)) {
    precision <- PRECISION_YEAR
    fields <- list(year = year)
  } else if (is_null(index)) {
    precision <- PRECISION_MONTH
    fields <- list(year = year, month = month)
  } else if (is_null(hour)) {
    precision <- PRECISION_DAY
    fields <- list(year = year, month = month, day = day, index = index)
  } else if (is_null(minute)) {
    precision <- PRECISION_HOUR
    fields <- list(year = year, month = month, day = day, index = index, hour = hour)
  } else if (is_null(second)) {
    precision <- PRECISION_MINUTE
    fields <- list(year = year, month = month, day = day, index = index, hour = hour, minute = minute)
  } else if (is_null(subsecond)) {
    precision <- PRECISION_SECOND
    fields <- list(year = year, month = month, day = day, index = index, hour = hour, minute = minute, second = second)
  } else {
    calendar_check_subsecond_precision(subsecond_precision)
    precision <- precision_to_integer(subsecond_precision)
    fields <- list(year = year, month = month, day = day, index = index, hour = hour, minute = minute, second = second, subsecond = subsecond)
  }

  if (is_last(fields$index)) {
    fields$index <- 1L
    last <- TRUE
  } else {
    last <- FALSE
  }

  fields <- vec_cast_common(!!!fields, .to = integer())

  if (precision >= PRECISION_YEAR) {
    check_between_year(fields$year, arg = "year")
  }
  if (precision >= PRECISION_MONTH) {
    check_between_month(fields$month, arg = "month")
  }
  if (precision >= PRECISION_DAY) {
    check_between_day_of_week(fields$day, arg = "day")
    check_between_index_of_week(fields$index, arg = "index")
  }
  if (precision >= PRECISION_HOUR) {
    check_between_hour(fields$hour, arg = "hour")
  }
  if (precision >= PRECISION_MINUTE) {
    check_between_minute(fields$minute, arg = "minute")
  }
  if (precision >= PRECISION_SECOND) {
    check_between_second(fields$second, arg = "second")
  }
  if (precision > PRECISION_SECOND) {
    check_between_subsecond(fields$subsecond, precision, arg = "subsecond")
  }

  fields <- vec_recycle_common(!!!fields)
  fields <- df_list_propagate_missing(fields)

  names <- NULL
  out <- new_year_month_weekday_from_fields(fields, precision, names)

  if (last) {
    out <- set_index(out, "last")
  }

  out
}

# ------------------------------------------------------------------------------

#' @export
vec_proxy.clock_year_month_weekday <- function(x, ...) {
  .Call(`_clock_clock_rcrd_proxy`, x)
}

#' @export
vec_restore.clock_year_month_weekday <- function(x, to, ...) {
  .Call(`_clock_year_month_weekday_restore`, x, to)
}

#' @export
vec_proxy_compare.clock_year_month_weekday <- function(x, ...) {
  precision <- calendar_precision_attribute(x)

  if (precision >= PRECISION_DAY) {
    # See issue #32
    message <- paste0(
      "'year_month_weekday' types with a precision of >= 'day' cannot be ",
      "trivially compared or ordered. ",
      "Convert to 'year_month_day' to compare using day-of-month values."
    )
    abort(message)
  }

  # Year / month year-month-weekday precision can be compared without ambiguity
  vec_proxy(x)
}

# ------------------------------------------------------------------------------

#' @export
format.clock_year_month_weekday <- function(x, ...) {
  out <- format_year_month_weekday_cpp(x, calendar_precision_attribute(x))
  names(out) <- names(x)
  out
}

#' @export
vec_ptype_full.clock_year_month_weekday <- function(x, ...) {
  calendar_ptype_full(x, "year_month_weekday")
}

#' @export
vec_ptype_abbr.clock_year_month_weekday <- function(x, ...) {
  calendar_ptype_abbr(x, "ymw")
}

# ------------------------------------------------------------------------------

#' Is `x` a year-month-weekday?
#'
#' Check if `x` is a year-month-weekday.
#'
#' @param x `[object]`
#'
#'   An object.
#'
#' @return Returns `TRUE` if `x` inherits from `"clock_year_month_weekday"`,
#'   otherwise returns `FALSE`.
#'
#' @export
#' @examples
#' is_year_month_weekday(year_month_weekday(2019))
is_year_month_weekday <- function(x) {
  inherits(x, "clock_year_month_weekday")
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype.clock_year_month_weekday <- function(x, ...) {
  switch(
    calendar_precision_attribute(x) + 1L,
    clock_empty_year_month_weekday_year,
    abort("Internal error: Invalid precision"),
    clock_empty_year_month_weekday_month,
    abort("Internal error: Invalid precision"),
    clock_empty_year_month_weekday_day,
    clock_empty_year_month_weekday_hour,
    clock_empty_year_month_weekday_minute,
    clock_empty_year_month_weekday_second,
    clock_empty_year_month_weekday_millisecond,
    clock_empty_year_month_weekday_microsecond,
    clock_empty_year_month_weekday_nanosecond,
    abort("Internal error: Invalid precision.")
  )
}

#' @export
vec_ptype2.clock_year_month_weekday.clock_year_month_weekday <- function(x, y, ...) {
  ptype2_calendar_and_calendar(x, y, ...)
}

#' @export
vec_cast.clock_year_month_weekday.clock_year_month_weekday <- function(x, to, ...) {
  cast_calendar_to_calendar(x, to, ...)
}

# ------------------------------------------------------------------------------

#' @export
calendar_is_precision.clock_year_month_weekday <- function(x, precision) {
  year_month_weekday_is_precision(precision)
}

year_month_weekday_is_precision <- function(precision) {
  if (precision == PRECISION_YEAR || precision == PRECISION_MONTH) {
    TRUE
  } else if (precision >= PRECISION_DAY && precision <= PRECISION_NANOSECOND) {
    TRUE
  } else {
    FALSE
  }
}

# ------------------------------------------------------------------------------

#' @export
invalid_detect.clock_year_month_weekday <- function(x) {
  precision <- calendar_precision_attribute(x)

  if (precision < PRECISION_DAY) {
    rep_along(x, FALSE)
  } else {
    year <- field_year(x)
    month <- field_month(x)
    day <- field_day(x)
    index <- field_index(x)
    invalid_detect_year_month_weekday_cpp(year, month, day, index)
  }
}

#' @export
invalid_any.clock_year_month_weekday <- function(x) {
  precision <- calendar_precision_attribute(x)

  if (precision < PRECISION_DAY) {
    FALSE
  } else {
    year <- field_year(x)
    month <- field_month(x)
    day <- field_day(x)
    index <- field_index(x)
    invalid_any_year_month_weekday_cpp(year, month, day, index)
  }
}

#' @export
invalid_count.clock_year_month_weekday <- function(x) {
  precision <- calendar_precision_attribute(x)

  if (precision < PRECISION_DAY) {
    0L
  } else {
    year <- field_year(x)
    month <- field_month(x)
    day <- field_day(x)
    index <- field_index(x)
    invalid_count_year_month_weekday_cpp(year, month, day, index)
  }
}

#' @export
invalid_resolve.clock_year_month_weekday <- function(x, ..., invalid = NULL) {
  check_dots_empty()
  precision <- calendar_precision_attribute(x)
  invalid <- validate_invalid(invalid)

  if (precision < PRECISION_DAY) {
    x
  } else {
    fields <- invalid_resolve_year_month_weekday_cpp(x, precision, invalid, current_env())
    new_year_month_weekday_from_fields(fields, precision, names(x))
  }
}

# ------------------------------------------------------------------------------

#' Getters: year-month-weekday
#'
#' @description
#' These are year-month-weekday methods for the
#' [getter generics][clock-getters].
#'
#' - `get_year()` returns the Gregorian year.
#'
#' - `get_month()` returns the month of the year.
#'
#' - `get_day()` returns the day of the week encoded from 1-7, where 1 = Sunday
#'   and 7 = Saturday.
#'
#' - `get_index()` returns a value from 1-5 indicating that the corresponding
#'   weekday is the n-th instance of that weekday in the current month.
#'
#' - There are sub-daily getters for extracting more precise components.
#'
#' @param x `[clock_year_month_weekday]`
#'
#'   A year-month-weekday to get the component from.
#'
#' @return The component.
#'
#' @name year-month-weekday-getters
#' @examples
#' monday <- clock_weekdays$monday
#' thursday <- clock_weekdays$thursday
#'
#' x <- year_month_weekday(2019, 1, monday:thursday, 1:4)
#' x
#'
#' # Gets the weekday, 1 = Sunday, 7 = Saturday
#' get_day(x)
#'
#' # Gets the index indicating which instance of that particular weekday
#' # it is in the current month (i.e. the "1st Monday of January, 2019")
#' get_index(x)
NULL

#' @rdname year-month-weekday-getters
#' @export
get_year.clock_year_month_weekday <- function(x) {
  field_year(x)
}

#' @rdname year-month-weekday-getters
#' @export
get_month.clock_year_month_weekday <- function(x) {
  calendar_check_minimum_precision(x, PRECISION_MONTH)
  field_month(x)
}

#' @rdname year-month-weekday-getters
#' @export
get_day.clock_year_month_weekday <- function(x) {
  # [Sunday, Saturday] -> [1, 7]
  calendar_check_minimum_precision(x, PRECISION_DAY)
  field_day(x)
}

#' @rdname year-month-weekday-getters
#' @export
get_index.clock_year_month_weekday <- function(x) {
  calendar_check_minimum_precision(x, PRECISION_DAY)
  field_index(x)
}

#' @rdname year-month-weekday-getters
#' @export
get_hour.clock_year_month_weekday <- function(x) {
  calendar_check_minimum_precision(x, PRECISION_HOUR)
  field_hour(x)
}

#' @rdname year-month-weekday-getters
#' @export
get_minute.clock_year_month_weekday <- function(x) {
  calendar_check_minimum_precision(x, PRECISION_MINUTE)
  field_minute(x)
}

#' @rdname year-month-weekday-getters
#' @export
get_second.clock_year_month_weekday <- function(x) {
  calendar_check_minimum_precision(x, PRECISION_SECOND)
  field_second(x)
}

#' @rdname year-month-weekday-getters
#' @export
get_millisecond.clock_year_month_weekday <- function(x) {
  calendar_check_exact_precision(x, PRECISION_MILLISECOND)
  field_subsecond(x)
}

#' @rdname year-month-weekday-getters
#' @export
get_microsecond.clock_year_month_weekday <- function(x) {
  calendar_check_exact_precision(x, PRECISION_MICROSECOND)
  field_subsecond(x)
}

#' @rdname year-month-weekday-getters
#' @export
get_nanosecond.clock_year_month_weekday <- function(x) {
  calendar_check_exact_precision(x, PRECISION_NANOSECOND)
  field_subsecond(x)
}

# ------------------------------------------------------------------------------

#' Setters: year-month-weekday
#'
#' @description
#' These are year-month-weekday methods for the
#' [setter generics][clock-setters].
#'
#' - `set_year()` sets the Gregorian year.
#'
#' - `set_month()` sets the month of the year. Valid values are in the range
#'   of `[1, 12]`.
#'
#' - `set_day()` sets the day of the week. Valid values are in the range of
#'   `[1, 7]`, with 1 = Sunday, and 7 = Saturday.
#'
#' - `set_index()` sets the index indicating that the corresponding
#'   weekday is the n-th instance of that weekday in the current month. Valid
#'   values are in the range of `[1, 5]`.
#'
#' - There are sub-daily setters for setting more precise components.
#'
#' @inheritParams rlang::args_dots_empty
#'
#' @param x `[clock_year_month_weekday]`
#'
#'   A year-month-weekday vector.
#'
#' @param value `[integer / "last"]`
#'
#'   The value to set the component to.
#'
#'   For `set_index()`, this can also be `"last"` to adjust to the last
#'   instance of the corresponding weekday in that month.
#'
#' @param index `[NULL / integer / "last"]`
#'
#'   This argument is only used with `set_day()`, and allows you to set the
#'   index while also setting the weekday.
#'
#'   If `x` is a month precision year-month-weekday, `index` is required to
#'   be set, as you must specify the weekday and the index simultaneously to
#'   promote from month to day precision.
#'
#' @return `x` with the component set.
#'
#' @name year-month-weekday-setters
#' @examples
#' x <- year_month_weekday(2019, 1:3)
#'
#' set_year(x, 2020:2022)
#'
#' # Setting the weekday on a month precision year-month-weekday requires
#' # also setting the `index` to fully specify the day information
#' x <- set_day(x, clock_weekdays$sunday, index = 1)
#' x
#'
#' # Once you have at least day precision, you can set the weekday and
#' # the index separately
#' set_day(x, clock_weekdays$monday)
#' set_index(x, 3)
#'
#' # Set to the "last" instance of the corresponding weekday in this month
#' # (Note that some months have 4 Sundays, and others have 5)
#' set_index(x, "last")
#'
#' # Set to an invalid index
#' # January and February of 2019 don't have 5 Sundays!
#' invalid <- set_index(x, 5)
#' invalid
#'
#' # Resolve the invalid dates by choosing the previous/next valid moment
#' invalid_resolve(invalid, invalid = "previous")
#' invalid_resolve(invalid, invalid = "next")
#'
#' # You can also "overflow" the index. This keeps the weekday, but resets
#' # the index to 1 and increments the month value by 1.
#' invalid_resolve(invalid, invalid = "overflow")
NULL

#' @rdname year-month-weekday-setters
#' @export
set_year.clock_year_month_weekday <- function(x, value, ...) {
  check_dots_empty()
  set_field_year_month_weekday(x, value, "year")
}

#' @rdname year-month-weekday-setters
#' @export
set_month.clock_year_month_weekday <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_YEAR)
  set_field_year_month_weekday(x, value, "month")
}

#' @rdname year-month-weekday-setters
#' @export
set_day.clock_year_month_weekday <- function(x, value, ..., index = NULL) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_MONTH)

  has_index <- !is_null(index)
  precision <- calendar_precision_attribute(x)

  if (precision == PRECISION_MONTH) {
    if (!has_index) {
      abort("For 'month' precision 'year_month_weekday', both the day and index must be set simultaneously.")
    }

    # Promote up to day precision so we can assign to fields individually
    ones <- ones_along(x, na_propagate = TRUE)
    fields <- list(year = get_year(x), month = get_month(x), day = ones, index = ones)
    x <- new_year_month_weekday_from_fields(fields, PRECISION_DAY, names(x))
  }

  out <- set_field_year_month_weekday(x, value, "day")

  if (has_index) {
    out <- set_field_year_month_weekday(out, index, "index")
  }

  out
}

#' @rdname year-month-weekday-setters
#' @export
set_index.clock_year_month_weekday <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_DAY)
  set_field_year_month_weekday(x, value, "index")
}

#' @rdname year-month-weekday-setters
#' @export
set_hour.clock_year_month_weekday <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_DAY)
  set_field_year_month_weekday(x, value, "hour")
}

#' @rdname year-month-weekday-setters
#' @export
set_minute.clock_year_month_weekday <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_HOUR)
  set_field_year_month_weekday(x, value, "minute")
}

#' @rdname year-month-weekday-setters
#' @export
set_second.clock_year_month_weekday <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_MINUTE)
  set_field_year_month_weekday(x, value, "second")
}

#' @rdname year-month-weekday-setters
#' @export
set_millisecond.clock_year_month_weekday <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_exact_precision(x, c(PRECISION_SECOND, PRECISION_MILLISECOND))
  set_field_year_month_weekday(x, value, "millisecond")
}

#' @rdname year-month-weekday-setters
#' @export
set_microsecond.clock_year_month_weekday <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_exact_precision(x, c(PRECISION_SECOND, PRECISION_MICROSECOND))
  set_field_year_month_weekday(x, value, "microsecond")
}

#' @rdname year-month-weekday-setters
#' @export
set_nanosecond.clock_year_month_weekday <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_exact_precision(x, c(PRECISION_SECOND, PRECISION_NANOSECOND))
  set_field_year_month_weekday(x, value, "nanosecond")
}

set_field_year_month_weekday <- function(x, value, component) {
  if (is_last(value) && identical(component, "index")) {
    return(set_field_year_month_weekday_last(x))
  }

  precision_fields <- calendar_precision_attribute(x)
  precision_value <- year_month_weekday_component_to_precision(component)
  precision_out <- precision_common2(precision_fields, precision_value)

  names_out <- names(x)

  value <- vec_cast(value, integer())
  value <- unname(value)

  switch(
    component,
    year = check_between_year(value),
    month = check_between_month(value),
    day = check_between_day_of_week(value),
    index = check_between_index_of_week(value),
    hour = check_between_hour(value),
    minute = check_between_minute(value),
    second = check_between_second(value),
    millisecond = check_between_subsecond(value, PRECISION_MILLISECOND),
    microsecond = check_between_subsecond(value, PRECISION_MICROSECOND),
    nanosecond = check_between_subsecond(value, PRECISION_NANOSECOND),
    abort("Unknown `component`", .internal = TRUE)
  )

  args <- vec_recycle_common(x = x, value = value)
  args <- df_list_propagate_missing(args)
  x <- args$x
  value <- args$value

  field <- year_month_weekday_component_to_field(component)

  out <- vec_unstructure(x)
  out[[field]] <- value

  new_year_month_weekday_from_fields(out, precision_out, names = names_out)
}

set_field_year_month_weekday_last <- function(x) {
  # We require 'day' precision to set the `index` at all, so no
  # need to find a common precision here
  precision_out <- calendar_precision_attribute(x)

  names_out <- names(x)

  year <- field_year(x)
  month <- field_month(x)
  day <- field_day(x)
  index <- field_index(x)
  value <- get_year_month_weekday_last_cpp(year, month, day, index)

  out <- vec_unstructure(x)
  out[["index"]] <- value

  new_year_month_weekday_from_fields(out, precision_out, names = names_out)
}

# ------------------------------------------------------------------------------

#' @export
calendar_name.clock_year_month_weekday <- function(x) {
  "year_month_weekday"
}

# ------------------------------------------------------------------------------

year_month_weekday_component_to_precision <- function(component) {
  switch(
    component,
    year = PRECISION_YEAR,
    month = PRECISION_MONTH,
    day = PRECISION_DAY,
    index = PRECISION_DAY,
    hour = PRECISION_HOUR,
    minute = PRECISION_MINUTE,
    second = PRECISION_SECOND,
    millisecond = PRECISION_MILLISECOND,
    microsecond = PRECISION_MICROSECOND,
    nanosecond = PRECISION_NANOSECOND,
    abort("Internal error: Unknown component name.")
  )
}

year_month_weekday_component_to_field <- function(component) {
  switch (
    component,
    year = component,
    month = component,
    day = component,
    index = component,
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
#' @method vec_arith clock_year_month_weekday
#' @export
vec_arith.clock_year_month_weekday <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_year_month_weekday", y)
}

#' @method vec_arith.clock_year_month_weekday MISSING
#' @export
vec_arith.clock_year_month_weekday.MISSING <- function(op, x, y, ...) {
  arith_calendar_and_missing(op, x, y, ...)
}

#' @method vec_arith.clock_year_month_weekday clock_year_month_weekday
#' @export
vec_arith.clock_year_month_weekday.clock_year_month_weekday <- function(op, x, y, ...) {
  arith_calendar_and_calendar(op, x, y, ..., calendar_minus_calendar_fn = year_month_weekday_minus_year_month_weekday)
}

#' @method vec_arith.clock_year_month_weekday clock_duration
#' @export
vec_arith.clock_year_month_weekday.clock_duration <- function(op, x, y, ...) {
  arith_calendar_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration clock_year_month_weekday
#' @export
vec_arith.clock_duration.clock_year_month_weekday <- function(op, x, y, ...) {
  arith_duration_and_calendar(op, x, y, ...)
}

#' @method vec_arith.clock_year_month_weekday numeric
#' @export
vec_arith.clock_year_month_weekday.numeric <- function(op, x, y, ...) {
  arith_calendar_and_numeric(op, x, y, ...)
}

#' @method vec_arith.numeric clock_year_month_weekday
#' @export
vec_arith.numeric.clock_year_month_weekday <- function(op, x, y, ...) {
  arith_numeric_and_calendar(op, x, y, ...)
}

year_month_weekday_minus_year_month_weekday <- function(op, x, y, ...) {
  args <- vec_recycle_common(x = x, y = y)
  args <- vec_cast_common(!!!args)
  x <- args$x
  y <- args$y

  names <- names_common(x, y)

  precision <- calendar_precision_attribute(x)

  if (precision > PRECISION_MONTH) {
    stop_incompatible_op(op, x, y, ...)
  }

  fields <- year_month_weekday_minus_year_month_weekday_cpp(x, y, precision)

  new_duration_from_fields(fields, precision, names)
}

# ------------------------------------------------------------------------------

#' Arithmetic: year-month-weekday
#'
#' @description
#' These are year-month-weekday methods for the
#' [arithmetic generics][clock-arithmetic].
#'
#' - `add_years()`
#'
#' - `add_quarters()`
#'
#' - `add_months()`
#'
#' Notably, _you cannot add days to a year-month-weekday_. For day-based
#' arithmetic, first convert to a time point with [as_naive_time()] or
#' [as_sys_time()].
#'
#' @details
#' Adding a single quarter with `add_quarters()` is equivalent to adding
#' 3 months.
#'
#' `x` and `n` are recycled against each other using
#' [tidyverse recycling rules][vctrs::vector_recycling_rules].
#'
#' @inheritParams clock-arithmetic
#'
#' @param x `[clock_year_month_weekday]`
#'
#'   A year-month-weekday vector.
#'
#' @return `x` after performing the arithmetic.
#'
#' @name year-month-weekday-arithmetic
#'
#' @examples
#' # 2nd Friday in January, 2019
#' x <- year_month_weekday(2019, 1, clock_weekdays$friday, 2)
#' x
#'
#' add_months(x, 1:5)
#'
#' # These don't necessarily correspond to the same day of the month
#' as_year_month_day(add_months(x, 1:5))
NULL

#' @rdname year-month-weekday-arithmetic
#' @export
add_years.clock_year_month_weekday <- function(x, n, ...) {
  year_month_weekday_plus_duration(x, n, PRECISION_YEAR)
}

#' @rdname year-month-weekday-arithmetic
#' @export
add_quarters.clock_year_month_weekday <- function(x, n, ...) {
  calendar_check_minimum_precision(x, PRECISION_MONTH)
  year_month_weekday_plus_duration(x, n, PRECISION_QUARTER)
}

#' @rdname year-month-weekday-arithmetic
#' @export
add_months.clock_year_month_weekday <- function(x, n, ...) {
  calendar_check_minimum_precision(x, PRECISION_MONTH)
  year_month_weekday_plus_duration(x, n, PRECISION_MONTH)
}

year_month_weekday_plus_duration <- function(x,
                                             n,
                                             n_precision,
                                             ...,
                                             error_call = caller_env()) {
  check_dots_empty0(...)

  x_precision <- calendar_precision_attribute(x)

  n <- duration_collect_n(n, n_precision, error_call = error_call)

  if (n_precision == PRECISION_QUARTER) {
    n <- duration_cast(n, "month")
    n_precision <- PRECISION_MONTH
  }

  size <- vec_size_common(x = x, n = n, .call = error_call)
  args <- vec_recycle_common(x = x, n = n, .size = size)
  x <- args$x
  n <- args$n

  names <- names_common(x, n)

  x <- vec_unstructure(x)

  if (n_precision == PRECISION_YEAR) {
    fields <- year_month_weekday_plus_years_cpp(x$year, n)
    x$year <- fields$year
  } else if (n_precision == PRECISION_MONTH) {
    fields <- year_month_weekday_plus_months_cpp(x$year, x$month, n)
    x$year <- fields$year
    x$month <- fields$month
  } else {
    abort("Unknown precision.", .internal = TRUE)
  }

  if (x_precision != n_precision) {
    x <- df_list_propagate_missing(x, size = size)
  }

  new_year_month_weekday_from_fields(x, x_precision, names = names)
}

# ------------------------------------------------------------------------------

#' Convert to year-month-weekday
#'
#' `as_year_month_weekday()` converts a vector to the year-month-weekday
#' calendar. Time points, Dates, POSIXct, and other calendars can all be
#' converted to year-month-weekday.
#'
#' @inheritParams rlang::args_dots_empty
#'
#' @param x `[vector]`
#'
#'   A vector to convert to year-month-weekday.
#'
#' @return A year-month-weekday vector.
#' @export
#' @examples
#' # From Date
#' as_year_month_weekday(as.Date("2019-01-01"))
#'
#' # From POSIXct, which assumes that the naive time is what should be converted
#' as_year_month_weekday(as.POSIXct("2019-01-01 02:30:30", "America/New_York"))
#'
#' # From other calendars
#' as_year_month_weekday(year_quarter_day(2019, quarter = 2, day = 50))
as_year_month_weekday <- function(x, ...)  {
  UseMethod("as_year_month_weekday")
}

#' @export
as_year_month_weekday.default <- function(x, ...) {
  stop_clock_unsupported_conversion(x, "clock_year_month_weekday")
}

#' @export
as_year_month_weekday.clock_year_month_weekday <- function(x, ...) {
  check_dots_empty0(...)
  x
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_year_month_weekday <- function(x, ...) {
  check_dots_empty0(...)
  calendar_check_no_invalid(x)
  precision <- calendar_precision_attribute(x)
  fields <- as_sys_time_year_month_weekday_cpp(x, precision)
  new_sys_time_from_fields(fields, precision, clock_rcrd_names(x))
}

#' @export
as_naive_time.clock_year_month_weekday <- function(x, ...) {
  check_dots_empty0(...)
  as_naive_time(as_sys_time(x))
}

#' @export
as.character.clock_year_month_weekday <- function(x, ...) {
  format(x)
}

# ------------------------------------------------------------------------------

#' @export
calendar_leap_year.clock_year_month_weekday <- function(x) {
  x <- get_year(x)
  gregorian_leap_year_cpp(x)
}

# ------------------------------------------------------------------------------

#' @export
calendar_month_factor.clock_year_month_weekday <- function(x,
                                                           ...,
                                                           labels = "en",
                                                           abbreviate = FALSE) {
  check_dots_empty0(...)
  calendar_month_factor_impl(x, labels, abbreviate)
}

# ------------------------------------------------------------------------------

#' Grouping: year-month-weekday
#'
#' @description
#' This is a year-month-weekday method for the [calendar_group()] generic.
#'
#' Grouping for a year-month-weekday object can be done at any precision except
#' for `"day"`, as long as `x` is at least as precise as `precision`.
#'
#' @details
#' Grouping by `"day"` is undefined for a year-month-weekday because there are
#' two day fields, the weekday and the index, and there is no clear way to
#' define how to group by that.
#'
#' @inheritParams calendar_group
#'
#' @param x `[clock_year_month_weekday]`
#'
#'   A year-month-weekday vector.
#'
#' @param precision `[character(1)]`
#'
#'   One of:
#'
#'   - `"year"`
#'   - `"month"`
#'   - `"hour"`
#'   - `"minute"`
#'   - `"second"`
#'   - `"millisecond"`
#'   - `"microsecond"`
#'   - `"nanosecond"`
#'
#' @return `x` grouped at the specified `precision`.
#'
#' @name year-month-weekday-group
#'
#' @export
#' @examples
#' x <- year_month_weekday(2019, 1:12, clock_weekdays$sunday, 1, 00, 05, 05)
#' x
#'
#' # Group by 3 months - drops more precise components!
#' calendar_group(x, "month", n = 3)
calendar_group.clock_year_month_weekday <- function(x, precision, ..., n = 1L) {
  n <- validate_calendar_group_n(n)
  x <- calendar_narrow(x, precision)

  check_precision(precision)
  precision <- precision_to_integer(precision)

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
    message <- paste0(
      "Grouping 'year_month_weekday' by 'day' precision is undefined. ",
      "Convert to 'year_month_day' to group by day of month."
    )
    abort(message)
  }

  x <- calendar_group_time(x, n, precision)
  x
}

# ------------------------------------------------------------------------------

#' Narrow: year-month-weekday
#'
#' This is a year-month-weekday method for the [calendar_narrow()] generic. It
#' narrows a year-month-weekday vector to the specified `precision`.
#'
#' @inheritParams year-month-weekday-widen
#'
#' @return `x` narrowed to the supplied `precision`.
#'
#' @name year-month-weekday-narrow
#'
#' @export
#' @examples
#' # Day precision
#' x <- year_month_weekday(2019, 1, 1, 2)
#' x
#'
#' # Narrowed to month precision
#' calendar_narrow(x, "month")
calendar_narrow.clock_year_month_weekday <- function(x, precision) {
  check_precision(precision)
  precision <- precision_to_integer(precision)

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
    out_fields[["index"]] <- x_fields[["index"]]
  }

  out_fields <- calendar_narrow_time(out_fields, precision, x_fields)

  new_year_month_weekday_from_fields(out_fields, precision, names = names(x))
}

# ------------------------------------------------------------------------------

#' Widen: year-month-weekday
#'
#' This is a year-month-weekday method for the [calendar_widen()] generic. It
#' widens a year-month-weekday vector to the specified `precision`.
#'
#' @details
#' Widening a month precision year-month-weekday to day precision will set
#' the day and the index to `1`. This sets the weekday components to the
#' first Sunday of the month.
#'
#' @inheritParams year-month-weekday-group
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
#' @return `x` widened to the supplied `precision`.
#'
#' @name year-month-weekday-widen
#'
#' @export
#' @examples
#' # Month precision
#' x <- year_month_weekday(2019, 1)
#' x
#'
#' # Widen to day precision
#' # Note that this sets both the day and index to 1,
#' # i.e. the first Sunday of the month.
#' calendar_widen(x, "day")
#'
#' # Or second precision
#' sec <- calendar_widen(x, "second")
#' sec
calendar_widen.clock_year_month_weekday <- function(x, precision) {
  x_precision <- calendar_precision_attribute(x)

  check_precision(precision)
  precision <- precision_to_integer(precision)

  if (precision >= PRECISION_MONTH && x_precision < PRECISION_MONTH) {
    x <- set_month(x, 1L)
  }
  if (precision >= PRECISION_DAY && x_precision < PRECISION_DAY) {
    x <- set_day(x, 1L, index = 1L)
  }

  x <- calendar_widen_time(x, x_precision, precision)

  x
}

# ------------------------------------------------------------------------------

#' Boundaries: year-month-weekday
#'
#' @description
#' This is a year-month-weekday method for the [calendar_start()] and
#' [calendar_end()] generics. They adjust components of a calendar to the
#' start or end of a specified `precision`.
#'
#' This method is restricted to only `"year"` and `"month"` `precision`s, and
#' `x` can't be more precise than month precision. Computing the "start" of
#' a day precision year-month-weekday object isn't defined because
#' a year-month-weekday with `day = 1, index = 1` doesn't necessarily occur
#' earlier (chronologically) than `day = 2, index = 1`. Because of these
#' restrictions, this method isn't particularly useful, but is included for
#' completeness.
#'
#' @inheritParams year-month-weekday-group
#'
#' @param precision `[character(1)]`
#'
#'   One of:
#'
#'   - `"year"`
#'   - `"month"`
#'
#' @return `x` at the same precision, but with some components altered to be
#'   at the boundary value.
#'
#' @name year-month-weekday-boundary
#'
#' @examples
#' # Month precision
#' x <- year_month_weekday(2019, 1:5)
#' x
#'
#' # Compute the last month of the year
#' calendar_end(x, "year")
NULL

#' @rdname year-month-weekday-boundary
#' @export
calendar_start.clock_year_month_weekday <- function(x, precision) {
  x_precision <- calendar_precision_attribute(x)

  check_precision(precision)
  precision <- precision_to_integer(precision)

  calendar_start_end_checks(x, x_precision, precision, "start")

  if (x_precision >= PRECISION_DAY) {
    message <- paste0(
      "Computing the start of a 'year_month_weekday' with a precision equal to ",
      "or more precise than 'day' is undefined."
    )
    abort(message)
  }

  if (precision <= PRECISION_YEAR && x_precision > PRECISION_YEAR) {
    x <- set_month(x, 1L)
  }

  x
}

#' @rdname year-month-weekday-boundary
#' @export
calendar_end.clock_year_month_weekday <- function(x, precision) {
  x_precision <- calendar_precision_attribute(x)

  check_precision(precision)
  precision <- precision_to_integer(precision)

  calendar_start_end_checks(x, x_precision, precision, "end")

  if (x_precision >= PRECISION_DAY) {
    message <- paste0(
      "Computing the end of a 'year_month_weekday' with a precision equal to ",
      "or more precise than 'day' is undefined."
    )
    abort(message)
  }

  if (precision <= PRECISION_YEAR && x_precision > PRECISION_YEAR) {
    x <- set_month(x, 12L)
  }

  x
}

# ------------------------------------------------------------------------------

#' Counting: year-month-weekday
#'
#' This is a year-month-weekday method for the [calendar_count_between()]
#' generic. It counts the number of `precision` units between `start` and `end`
#' (i.e., the number of years or months).
#'
#' @details
#' Remember that year-month-weekday is not comparable when it is `"day"`
#' precision or finer, so this method is only defined for `"year"` and
#' `"month"` precision year-month-weekday objects.
#'
#' `"quarter"` is equivalent to `"month"` precision with `n` set to `n * 3L`.
#'
#' @inheritParams calendar-count-between
#'
#' @param start,end `[clock_year_month_weekday]`
#'
#'   A pair of year-month-weekday vectors. These will be recycled to their
#'   common size.
#'
#' @param precision `[character(1)]`
#'
#'   One of:
#'
#'   - `"year"`
#'   - `"quarter"`
#'   - `"month"`
#'
#' @inherit calendar-count-between return
#'
#' @name year-month-weekday-count-between
#'
#' @export
#' @examples
#' # Compute the number of months between two dates
#' x <- year_month_weekday(2001, 2)
#' y <- year_month_weekday(2021, c(1, 3))
#'
#' calendar_count_between(x, y, "month")
#'
#' # Remember that day precision or finer year-month-weekday objects
#' # are not comparable, so this won't work
#' x <- year_month_weekday(2001, 2, 1, 1)
#' try(calendar_count_between(x, x, "month"))
calendar_count_between.clock_year_month_weekday <- function(start,
                                                            end,
                                                            precision,
                                                            ...,
                                                            n = 1L) {
  NextMethod()
}

calendar_count_between_standardize_precision_n.clock_year_month_weekday <- function(x,
                                                                                    precision,
                                                                                    n) {
  calendar_count_between_standardize_precision_n.clock_year_month_day(x, precision, n)
}

calendar_count_between_compute.clock_year_month_weekday <- function(start,
                                                                    end,
                                                                    precision) {
  calendar_count_between_compute.clock_year_month_day(start, end, precision)
}

calendar_count_between_proxy_compare.clock_year_month_weekday <- function(start,
                                                                          end,
                                                                          precision) {
  calendar_count_between_proxy_compare.clock_year_month_day(start, end, precision)
}

# ------------------------------------------------------------------------------

#' Sequences: year-month-weekday
#'
#' @description
#' This is a year-month-weekday method for the [seq()] generic.
#'
#' Sequences can only be generated for `"year"` and `"month"` precision
#' year-month-weekday vectors.
#'
#' When calling `seq()`, exactly two of the following must be specified:
#' - `to`
#' - `by`
#' - Either `length.out` or `along.with`
#'
#' @inheritParams seq.clock_duration
#'
#' @param from `[clock_year_month_weekday(1)]`
#'
#'   A `"year"` or `"month"` precision year-month-weekday to start the sequence
#'   from.
#'
#'   `from` is always included in the result.
#'
#' @param to `[clock_year_month_weekday(1) / NULL]`
#'
#'   A `"year"` or `"month"` precision year-month-weekday to stop the sequence
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
#' x <- seq(year_month_weekday(2019, 1), year_month_weekday(2020, 12), by = 1)
#' x
#'
#' # Which we can then set the indexed weekday of
#' set_day(x, clock_weekdays$sunday, index = "last")
seq.clock_year_month_weekday <- function(from,
                                         to = NULL,
                                         by = NULL,
                                         length.out = NULL,
                                         along.with = NULL,
                                         ...) {
  seq.clock_year_month_day(from, to, by, length.out, along.with, ...)
}

# ------------------------------------------------------------------------------

#' @export
clock_minimum.clock_year_month_weekday <- function(x) {
  check_year_month_weekday_precision_limit(x, "minimum")

  switch(
    calendar_precision_attribute(x) + 1L,
    clock_minimum_year_month_weekday_year,
    abort("Invalid precision", .internal = TRUE),
    clock_minimum_year_month_weekday_month,
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE)
  )
}

#' @export
clock_maximum.clock_year_month_weekday <- function(x) {
  check_year_month_weekday_precision_limit(x, "maximum")

  switch(
    calendar_precision_attribute(x) + 1L,
    clock_maximum_year_month_weekday_year,
    abort("Invalid precision", .internal = TRUE),
    clock_maximum_year_month_weekday_month,
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
    abort("Invalid precision", .internal = TRUE),
  )
}

year_month_weekday_minimum <- function(precision) {
  calendar_minimum(precision, year_month_weekday(clock_calendar_year_minimum))
}
year_month_weekday_maximum <- function(precision) {
  calendar_maximum(precision, year_month_weekday(clock_calendar_year_maximum))
}

check_year_month_weekday_precision_limit <- function(x,
                                                     which,
                                                     ...,
                                                     arg = caller_arg(x),
                                                     call = caller_env()) {
  x_precision <- calendar_precision_attribute(x)
  precision <- PRECISION_MONTH

  if (x_precision <= precision) {
    return(invisible(NULL))
  }

  x_precision <- precision_to_string(x_precision)
  precision <- precision_to_string(precision)

  message <- c(
    "Can't compute the {which} of this {.cls year_month_weekday}, as it is undefined.",
    i = "The most precise allowed precision is {.str {precision}}.",
    i = "{.arg {arg}} has precision {.str {x_precision}}."
  )

  cli::cli_abort(message, call = call)
}

# ------------------------------------------------------------------------------

clock_init_year_month_weekday_utils <- function(env) {
  year <- year_month_weekday(integer())

  assign("clock_empty_year_month_weekday_year", year, envir = env)
  assign("clock_empty_year_month_weekday_month", calendar_widen(year, "month"), envir = env)
  assign("clock_empty_year_month_weekday_day", calendar_widen(year, "day"), envir = env)
  assign("clock_empty_year_month_weekday_hour", calendar_widen(year, "hour"), envir = env)
  assign("clock_empty_year_month_weekday_minute", calendar_widen(year, "minute"), envir = env)
  assign("clock_empty_year_month_weekday_second", calendar_widen(year, "second"), envir = env)
  assign("clock_empty_year_month_weekday_millisecond", calendar_widen(year, "millisecond"), envir = env)
  assign("clock_empty_year_month_weekday_microsecond", calendar_widen(year, "microsecond"), envir = env)
  assign("clock_empty_year_month_weekday_nanosecond", calendar_widen(year, "nanosecond"), envir = env)

  assign("clock_minimum_year_month_weekday_year", year_month_weekday_minimum("year"), envir = env)
  assign("clock_minimum_year_month_weekday_month", year_month_weekday_minimum("month"), envir = env)

  assign("clock_maximum_year_month_weekday_year", year_month_weekday_maximum("year"), envir = env)
  assign("clock_maximum_year_month_weekday_month", year_month_weekday_maximum("month"), envir = env)

  invisible(NULL)
}
