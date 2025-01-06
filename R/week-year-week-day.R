#' Calendar: year-week-day
#'
#' @description
#' `year_week_day()` constructs a calendar from the year, week number,
#' week day, and the `start` of the week.
#'
#' Using `start = clock_weekdays$monday` represents the ISO week calendar and
#' is equivalent to using [iso_year_week_day()].
#'
#' Using `start = clock_weekdays$sunday` is how Epidemiologists encode their
#' week-based data.
#'
#' @details
#' Fields are recycled against each other using
#' [tidyverse recycling rules][vctrs::vector_recycling_rules].
#'
#' Fields are collected in order until the first `NULL` field is located. No
#' fields after the first `NULL` field are used.
#'
#' @inheritParams rlang::args_dots_empty
#' @inheritParams year_month_day
#'
#' @param year `[integer]`
#'
#'   The year. Values `[-32767, 32767]` are generally allowed.
#'
#' @param week `[integer / "last" / NULL]`
#'
#'   The week. Values `[1, 53]` are allowed.
#'
#'   If `"last"`, then the last week of the year is returned.
#'
#' @param day `[integer / NULL]`
#'
#'   The day of the week. Values `[1, 7]` are allowed, with `1 = start of week`
#'   and `7 = end of week`, in accordance with `start`.
#'
#' @param start `[integer(1) / NULL]`
#'
#'   The day to consider the start of the week. 1 = Sunday and 7 = Saturday.
#'
#'   Use [clock_weekdays] for a readable way to specify the start.
#'
#'   If `NULL`, a `start` of Sunday will be used.
#'
#' @return A year-week-day calendar vector.
#'
#' @export
#' @examples
#' # Year-week
#' x <- year_week_day(2019:2025, "last")
#' x
#'
#' # Start the week on Monday
#' y <- year_week_day(2019:2025, "last", start = clock_weekdays$monday)
#' y
#'
#' # Last days of the year
#' as_year_month_day(set_day(x, 7))
#' as_year_month_day(set_day(y, 7))
year_week_day <- function(
  year,
  week = NULL,
  day = NULL,
  hour = NULL,
  minute = NULL,
  second = NULL,
  subsecond = NULL,
  ...,
  start = NULL,
  subsecond_precision = NULL
) {
  check_dots_empty()

  start <- week_validate_start(start)

  # Stop on the first `NULL` argument
  if (is_null(week)) {
    precision <- PRECISION_YEAR
    fields <- list(year = year)
  } else if (is_null(day)) {
    precision <- PRECISION_WEEK
    fields <- list(year = year, week = week)
  } else if (is_null(hour)) {
    precision <- PRECISION_DAY
    fields <- list(year = year, week = week, day = day)
  } else if (is_null(minute)) {
    precision <- PRECISION_HOUR
    fields <- list(year = year, week = week, day = day, hour = hour)
  } else if (is_null(second)) {
    precision <- PRECISION_MINUTE
    fields <- list(
      year = year,
      week = week,
      day = day,
      hour = hour,
      minute = minute
    )
  } else if (is_null(subsecond)) {
    precision <- PRECISION_SECOND
    fields <- list(
      year = year,
      week = week,
      day = day,
      hour = hour,
      minute = minute,
      second = second
    )
  } else {
    calendar_check_subsecond_precision(subsecond_precision)
    precision <- precision_to_integer(subsecond_precision)
    fields <- list(
      year = year,
      week = week,
      day = day,
      hour = hour,
      minute = minute,
      second = second,
      subsecond = subsecond
    )
  }

  if (is_last(fields$week)) {
    fields$week <- 1L
    last <- TRUE
  } else {
    last <- FALSE
  }

  fields <- vec_cast_common(!!!fields, .to = integer())

  if (precision >= PRECISION_YEAR) {
    check_between_year(fields$year, arg = "year")
  }
  if (precision >= PRECISION_WEEK) {
    check_between_week(fields$week, arg = "week")
  }
  if (precision >= PRECISION_DAY) {
    check_between_day_of_week(fields$day, arg = "day")
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
  out <- new_year_week_day_from_fields(fields, precision, start, names)

  if (last) {
    out <- set_week(out, "last")
  }

  out
}

# ------------------------------------------------------------------------------

#' @export
vec_proxy.clock_year_week_day <- function(x, ...) {
  .Call(`_clock_clock_rcrd_proxy`, x)
}

#' @export
vec_restore.clock_year_week_day <- function(x, to, ...) {
  .Call(`_clock_year_week_day_restore`, x, to)
}

# ------------------------------------------------------------------------------

#' @export
format.clock_year_week_day <- function(x, ...) {
  out <- format_year_week_day_cpp(
    x,
    calendar_precision_attribute(x),
    week_start(x)
  )
  names(out) <- names(x)
  out
}

#' @export
vec_ptype_full.clock_year_week_day <- function(x, ...) {
  start <- week_start(x)
  start <- week_start_prettify(start, abbreviate = FALSE)
  class <- paste0("year_week_day<", start, ">")
  calendar_ptype_full(x, class)
}

#' @export
vec_ptype_abbr.clock_year_week_day <- function(x, ...) {
  start <- week_start(x)
  start <- week_start_prettify(start, abbreviate = TRUE)
  class <- paste0("ywd<", start, ">")
  calendar_ptype_abbr(x, class)
}

# ------------------------------------------------------------------------------

#' Is `x` a year-week-day?
#'
#' Check if `x` is a year-week-day.
#'
#' @param x `[object]`
#'
#'   An object.
#'
#' @return Returns `TRUE` if `x` inherits from `"clock_year_week_day"`,
#'   otherwise returns `FALSE`.
#'
#' @export
#' @examples
#' is_year_week_day(year_week_day(2019))
is_year_week_day <- function(x) {
  inherits(x, "clock_year_week_day")
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype.clock_year_week_day <- function(x, ...) {
  names <- NULL
  precision <- calendar_precision_attribute(x)
  start <- week_start(x)

  f <- integer()

  fields <- switch(
    precision + 1L,
    list(year = f),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    list(
      year = f,
      week = f
    ),
    list(
      year = f,
      week = f,
      day = f
    ),
    list(
      year = f,
      week = f,
      day = f,
      hour = f
    ),
    list(
      year = f,
      week = f,
      day = f,
      hour = f,
      minute = f
    ),
    list(
      year = f,
      week = f,
      day = f,
      hour = f,
      minute = f,
      second = f
    ),
    list(
      year = f,
      week = f,
      day = f,
      hour = f,
      minute = f,
      second = f,
      subsecond = f
    ),
    list(
      year = f,
      week = f,
      day = f,
      hour = f,
      minute = f,
      second = f,
      subsecond = f
    ),
    list(
      year = f,
      week = f,
      day = f,
      hour = f,
      minute = f,
      second = f,
      subsecond = f
    ),
    abort("Internal error: Invalid precision.")
  )

  new_year_week_day_from_fields(fields, precision, start, names)
}

#' @export
vec_ptype2.clock_year_week_day.clock_year_week_day <- function(x, y, ...) {
  if (week_start(x) != week_start(y)) {
    stop_incompatible_type(
      x,
      y,
      ...,
      details = "Can't combine week types with different `start`s."
    )
  }

  ptype2_calendar_and_calendar(x, y, ...)
}

#' @export
vec_cast.clock_year_week_day.clock_year_week_day <- function(x, to, ...) {
  if (week_start(x) != week_start(to)) {
    stop_incompatible_cast(
      x,
      to,
      ...,
      details = "Can't cast between week types with different `start`s."
    )
  }

  cast_calendar_to_calendar(x, to, ...)
}

# ------------------------------------------------------------------------------

#' @export
calendar_is_precision.clock_year_week_day <- function(x, precision) {
  year_week_day_is_precision(precision)
}

year_week_day_is_precision <- function(precision) {
  if (precision == PRECISION_YEAR || precision == PRECISION_WEEK) {
    TRUE
  } else if (precision >= PRECISION_DAY && precision <= PRECISION_NANOSECOND) {
    TRUE
  } else {
    FALSE
  }
}

# ------------------------------------------------------------------------------

#' @export
invalid_detect.clock_year_week_day <- function(x) {
  precision <- calendar_precision_attribute(x)
  start <- week_start(x)

  if (precision < PRECISION_WEEK) {
    rep_along(x, FALSE)
  } else {
    year <- field_year(x)
    week <- field_week(x)
    invalid_detect_year_week_day_cpp(year, week, start)
  }
}

#' @export
invalid_any.clock_year_week_day <- function(x) {
  precision <- calendar_precision_attribute(x)
  start <- week_start(x)

  if (precision < PRECISION_WEEK) {
    FALSE
  } else {
    year <- field_year(x)
    week <- field_week(x)
    invalid_any_year_week_day_cpp(year, week, start)
  }
}

#' @export
invalid_count.clock_year_week_day <- function(x) {
  precision <- calendar_precision_attribute(x)
  start <- week_start(x)

  if (precision < PRECISION_WEEK) {
    0L
  } else {
    year <- field_year(x)
    week <- field_week(x)
    invalid_count_year_week_day_cpp(year, week, start)
  }
}

#' @export
invalid_resolve.clock_year_week_day <- function(x, ..., invalid = NULL) {
  check_dots_empty()
  precision <- calendar_precision_attribute(x)
  start <- week_start(x)
  invalid <- validate_invalid(invalid)

  if (precision < PRECISION_WEEK) {
    x
  } else {
    fields <- invalid_resolve_year_week_day_cpp(
      x,
      precision,
      start,
      invalid,
      current_env()
    )
    new_year_week_day_from_fields(fields, precision, start, names(x))
  }
}

# ------------------------------------------------------------------------------

#' Getters: year-week-day
#'
#' @description
#' These are year-week-day methods for the
#' [getter generics][clock-getters].
#'
#' - `get_year()` returns the year. Note that this can differ from the
#'   Gregorian year.
#'
#' - `get_week()` returns the week of the current year.
#'
#' - `get_day()` returns a value between 1-7 indicating the weekday of the
#'   current week, where `1 = start of week` and `7 = end of week`, in line with
#'   the chosen `start`.
#'
#' - There are sub-daily getters for extracting more precise components.
#'
#' @param x `[clock_year_week_day]`
#'
#'   A year-week-day to get the component from.
#'
#' @return The component.
#'
#' @name year-week-day-getters
#' @examples
#' x <- year_week_day(2019, 50:52, 1:3)
#' x
#'
#' # Get the week
#' get_week(x)
#'
#' # Gets the weekday
#' get_day(x)
#'
#' # Note that the year can differ from the Gregorian year
#' iso <- year_week_day(2019, 1, 1, start = clock_weekdays$monday)
#' ymd <- as_year_month_day(iso)
#'
#' get_year(iso)
#' get_year(ymd)
NULL

#' @rdname year-week-day-getters
#' @export
get_year.clock_year_week_day <- function(x) {
  field_year(x)
}

#' @rdname year-week-day-getters
#' @export
get_week.clock_year_week_day <- function(x) {
  calendar_check_minimum_precision(x, PRECISION_WEEK)
  field_week(x)
}

#' @rdname year-week-day-getters
#' @export
get_day.clock_year_week_day <- function(x) {
  calendar_check_minimum_precision(x, PRECISION_DAY)
  field_day(x)
}

#' @rdname year-week-day-getters
#' @export
get_hour.clock_year_week_day <- function(x) {
  calendar_check_minimum_precision(x, PRECISION_HOUR)
  field_hour(x)
}

#' @rdname year-week-day-getters
#' @export
get_minute.clock_year_week_day <- function(x) {
  calendar_check_minimum_precision(x, PRECISION_MINUTE)
  field_minute(x)
}

#' @rdname year-week-day-getters
#' @export
get_second.clock_year_week_day <- function(x) {
  calendar_check_minimum_precision(x, PRECISION_SECOND)
  field_second(x)
}

#' @rdname year-week-day-getters
#' @export
get_millisecond.clock_year_week_day <- function(x) {
  calendar_check_exact_precision(x, PRECISION_MILLISECOND)
  field_subsecond(x)
}

#' @rdname year-week-day-getters
#' @export
get_microsecond.clock_year_week_day <- function(x) {
  calendar_check_exact_precision(x, PRECISION_MICROSECOND)
  field_subsecond(x)
}

#' @rdname year-week-day-getters
#' @export
get_nanosecond.clock_year_week_day <- function(x) {
  calendar_check_exact_precision(x, PRECISION_NANOSECOND)
  field_subsecond(x)
}

# ------------------------------------------------------------------------------

#' Setters: year-week-day
#'
#' @description
#' These are year-week-day methods for the [setter generics][clock-setters].
#'
#' - `set_year()` sets the year.
#'
#' - `set_week()` sets the week of the year. Valid values are in the range
#'   of `[1, 53]`.
#'
#' - `set_day()` sets the day of the week. Valid values are in the range of
#'   `[1, 7]`.
#'
#' - There are sub-daily setters for setting more precise components.
#'
#' @inheritParams rlang::args_dots_empty
#'
#' @param x `[clock_year_week_day]`
#'
#'   A year-week-day vector.
#'
#' @param value `[integer / "last"]`
#'
#'   The value to set the component to.
#'
#'   For `set_week()`, this can also be `"last"` to adjust to the last
#'   week of the current year.
#'
#' @return `x` with the component set.
#'
#' @name year-week-day-setters
#' @examples
#' # Year precision vector
#' x <- year_week_day(2019:2023)
#'
#' # Promote to week precision by setting the week
#' # (Note that some weeks have 52 weeks, and others have 53)
#' x <- set_week(x, "last")
#' x
#'
#' # Set to an invalid week
#' invalid <- set_week(x, 53)
#' invalid
#'
#' # Here are the invalid ones (they only have 52 weeks)
#' invalid[invalid_detect(invalid)]
#'
#' # Resolve the invalid dates by choosing the previous/next valid moment
#' invalid_resolve(invalid, invalid = "previous")
#' invalid_resolve(invalid, invalid = "next")
NULL

#' @rdname year-week-day-setters
#' @export
set_year.clock_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  set_field_year_week_day(x, value, "year")
}

#' @rdname year-week-day-setters
#' @export
set_week.clock_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_YEAR)
  set_field_year_week_day(x, value, "week")
}

#' @rdname year-week-day-setters
#' @export
set_day.clock_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_WEEK)
  set_field_year_week_day(x, value, "day")
}

#' @rdname year-week-day-setters
#' @export
set_hour.clock_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_DAY)
  set_field_year_week_day(x, value, "hour")
}

#' @rdname year-week-day-setters
#' @export
set_minute.clock_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_HOUR)
  set_field_year_week_day(x, value, "minute")
}

#' @rdname year-week-day-setters
#' @export
set_second.clock_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_minimum_precision(x, PRECISION_MINUTE)
  set_field_year_week_day(x, value, "second")
}

#' @rdname year-week-day-setters
#' @export
set_millisecond.clock_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_exact_precision(x, c(PRECISION_SECOND, PRECISION_MILLISECOND))
  set_field_year_week_day(x, value, "millisecond")
}

#' @rdname year-week-day-setters
#' @export
set_microsecond.clock_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_exact_precision(x, c(PRECISION_SECOND, PRECISION_MICROSECOND))
  set_field_year_week_day(x, value, "microsecond")
}

#' @rdname year-week-day-setters
#' @export
set_nanosecond.clock_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_check_exact_precision(x, c(PRECISION_SECOND, PRECISION_NANOSECOND))
  set_field_year_week_day(x, value, "nanosecond")
}

set_field_year_week_day <- function(x, value, component) {
  if (is_last(value) && identical(component, "week")) {
    return(set_field_year_week_day_last(x))
  }

  precision_fields <- calendar_precision_attribute(x)
  precision_value <- year_week_day_component_to_precision(component)
  precision_out <- precision_common2(precision_fields, precision_value)

  start_out <- week_start(x)
  names_out <- names(x)

  value <- vec_cast(value, integer())
  value <- unname(value)

  switch(
    component,
    year = check_between_year(value),
    week = check_between_week(value),
    day = check_between_day_of_week(value),
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

  field <- year_week_day_component_to_field(component)

  out <- vec_unstructure(x)
  out[[field]] <- value

  new_year_week_day_from_fields(
    out,
    precision_out,
    start_out,
    names = names_out
  )
}

set_field_year_week_day_last <- function(x) {
  precision_fields <- calendar_precision_attribute(x)
  precision_out <- precision_common2(precision_fields, PRECISION_WEEK)

  start_out <- week_start(x)
  names_out <- names(x)

  year <- field_year(x)
  value <- get_year_week_day_last_cpp(year, start_out)

  out <- vec_unstructure(x)
  out[["week"]] <- value

  new_year_week_day_from_fields(
    out,
    precision_out,
    start_out,
    names = names_out
  )
}

# ------------------------------------------------------------------------------

#' @export
calendar_name.clock_year_week_day <- function(x) {
  "year_week_day"
}

# ------------------------------------------------------------------------------

year_week_day_component_to_precision <- function(component) {
  switch(
    component,
    year = PRECISION_YEAR,
    week = PRECISION_WEEK,
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

year_week_day_component_to_field <- function(component) {
  switch(
    component,
    year = component,
    week = component,
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
#' @method vec_arith clock_year_week_day
#' @export
vec_arith.clock_year_week_day <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_year_week_day", y)
}

#' @method vec_arith.clock_year_week_day MISSING
#' @export
vec_arith.clock_year_week_day.MISSING <- function(op, x, y, ...) {
  arith_calendar_and_missing(op, x, y, ...)
}

#' @method vec_arith.clock_year_week_day clock_year_week_day
#' @export
vec_arith.clock_year_week_day.clock_year_week_day <- function(op, x, y, ...) {
  arith_calendar_and_calendar(
    op,
    x,
    y,
    ...,
    calendar_minus_calendar_fn = year_week_day_minus_year_week_day
  )
}

#' @method vec_arith.clock_year_week_day clock_duration
#' @export
vec_arith.clock_year_week_day.clock_duration <- function(op, x, y, ...) {
  arith_calendar_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration clock_year_week_day
#' @export
vec_arith.clock_duration.clock_year_week_day <- function(op, x, y, ...) {
  arith_duration_and_calendar(op, x, y, ...)
}

#' @method vec_arith.clock_year_week_day numeric
#' @export
vec_arith.clock_year_week_day.numeric <- function(op, x, y, ...) {
  arith_calendar_and_numeric(op, x, y, ...)
}

#' @method vec_arith.numeric clock_year_week_day
#' @export
vec_arith.numeric.clock_year_week_day <- function(op, x, y, ...) {
  arith_numeric_and_calendar(op, x, y, ...)
}

year_week_day_minus_year_week_day <- function(op, x, y, ...) {
  args <- vec_recycle_common(x = x, y = y)
  args <- vec_cast_common(!!!args)
  x <- args$x
  y <- args$y

  names <- names_common(x, y)

  start <- week_start(x)
  precision <- calendar_precision_attribute(x)

  if (precision > PRECISION_YEAR) {
    stop_incompatible_op(op, x, y, ...)
  }

  fields <- year_week_day_minus_year_week_day_cpp(x, y, precision, start)

  new_duration_from_fields(fields, precision, names = names)
}

# ------------------------------------------------------------------------------

#' Arithmetic: year-week-day
#'
#' @description
#' These are year-week-day methods for the
#' [arithmetic generics][clock-arithmetic].
#'
#' - `add_years()`
#'
#' You cannot add weeks or days to a year-week-day calendar. Adding
#' days is much more efficiently done by converting to a time point first
#' by using [as_naive_time()] or [as_sys_time()]. Adding weeks is equally
#' as efficient as adding 7 days. Additionally, adding weeks to an invalid
#' year-week object (i.e. one set to the 53rd week, when that doesn't exist)
#' would be undefined.
#'
#' @details
#' `x` and `n` are recycled against each other using
#' [tidyverse recycling rules][vctrs::vector_recycling_rules].
#'
#' @inheritParams clock-arithmetic
#'
#' @param x `[clock_year_week_day]`
#'
#'   A year-week-day vector.
#'
#' @return `x` after performing the arithmetic.
#'
#' @name year-week-day-arithmetic
#'
#' @examples
#' x <- year_week_day(2019, 1, 1)
#' add_years(x, 1:2)
NULL

#' @rdname year-week-day-arithmetic
#' @export
add_years.clock_year_week_day <- function(x, n, ...) {
  year_week_day_plus_duration(x, n, PRECISION_YEAR)
}

year_week_day_plus_duration <- function(
  x,
  n,
  n_precision,
  ...,
  error_call = caller_env()
) {
  check_dots_empty0(...)

  start <- week_start(x)
  x_precision <- calendar_precision_attribute(x)

  n <- duration_collect_n(n, n_precision, error_call = error_call)

  size <- vec_size_common(x = x, n = n, .call = error_call)
  args <- vec_recycle_common(x = x, n = n, .size = size)
  x <- args$x
  n <- args$n

  names <- names_common(x, n)

  x <- vec_unstructure(x)

  if (n_precision == PRECISION_YEAR) {
    fields <- year_week_day_plus_years_cpp(x$year, start, n)
    x$year <- fields$year
  } else {
    abort("Unknown precision.", .internal = TRUE)
  }

  if (x_precision != n_precision) {
    x <- df_list_propagate_missing(x, size = size)
  }

  new_year_week_day_from_fields(x, x_precision, start, names = names)
}

# ------------------------------------------------------------------------------

#' @export
diff.clock_year_week_day <- function(x, lag = 1L, differences = 1L, ...) {
  calendar_check_maximum_precision(x, PRECISION_YEAR)
  if (calendar_diff_is_empty(x, lag, differences)) {
    duration_helper(integer(), calendar_precision_attribute(x))
  } else {
    NextMethod()
  }
}

# ------------------------------------------------------------------------------

#' Convert to year-week-day
#'
#' `as_year_week_day()` converts a vector to the year-week-day
#' calendar. Time points, Dates, POSIXct, and other calendars can all be
#' converted to year-week-day.
#'
#' @inheritParams rlang::args_dots_empty
#'
#' @param x `[vector]`
#'
#'   A vector to convert to year-week-day.
#'
#' @param start `[integer(1) / NULL]`
#'
#'   The day to consider the start of the week. 1 = Sunday and 7 = Saturday.
#'
#'   If `NULL`:
#'
#'   - If `x` is a year-week-day, it will be returned as is.
#'
#'   - Otherwise, a `start` of Sunday will be used.
#'
#' @return A year-week-day vector.
#' @export
#' @examples
#' # From Date
#' as_year_week_day(as.Date("2019-01-01"))
#' as_year_week_day(as.Date("2019-01-01"), start = clock_weekdays$monday)
#'
#' # From POSIXct, which assumes that the naive time is what should be converted
#' as_year_week_day(as.POSIXct("2019-01-01 02:30:30", "America/New_York"))
#'
#' # From other calendars
#' as_year_week_day(year_quarter_day(2019, quarter = 2, day = 50))
as_year_week_day <- function(x, ..., start = NULL) {
  UseMethod("as_year_week_day")
}

#' @export
as_year_week_day.default <- function(x, ..., start = NULL) {
  stop_clock_unsupported_conversion(x, "clock_year_week_day")
}

#' @export
as_year_week_day.clock_year_week_day <- function(x, ..., start = NULL) {
  check_dots_empty0(...)

  if (is_null(start)) {
    return(x)
  }

  start <- week_validate_start(start)
  x_start <- week_start(x)

  if (x_start == start) {
    return(x)
  }

  as_year_week_day(as_sys_time(x), start = start)
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_year_week_day <- function(x, ...) {
  check_dots_empty0(...)
  calendar_check_no_invalid(x)
  start <- week_start(x)
  precision <- calendar_precision_attribute(x)
  fields <- as_sys_time_year_week_day_cpp(x, precision, start)
  new_sys_time_from_fields(fields, precision, clock_rcrd_names(x))
}

#' @export
as_naive_time.clock_year_week_day <- function(x, ...) {
  check_dots_empty0(...)
  as_naive_time(as_sys_time(x))
}

#' @export
as.character.clock_year_week_day <- function(x, ...) {
  format(x)
}

# ------------------------------------------------------------------------------

#' @export
calendar_leap_year.clock_year_week_day <- function(x) {
  year <- get_year(x)
  start <- week_start(x)
  year_week_day_leap_year_cpp(year, start)
}

# ------------------------------------------------------------------------------

#' Grouping: year-week-day
#'
#' @description
#' This is a year-week-day method for the [calendar_group()] generic.
#'
#' Grouping for a year-week-day object can be done at any precision, as
#' long as `x` is at least as precise as `precision`.
#'
#' @inheritParams calendar_group
#'
#' @param x `[clock_year_week_day]`
#'
#'   A year-week-day vector.
#'
#' @param precision `[character(1)]`
#'
#'   One of:
#'
#'   - `"year"`
#'   - `"week"`
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
#' @name year-week-day-group
#'
#' @export
#' @examples
#' x <- year_week_day(2019, 1:52)
#'
#' # Group by 3 weeks
#' calendar_group(x, "week", n = 3)
#'
#' y <- year_week_day(2000:2020, 1, 1)
#'
#' # Group by 2 years
#' calendar_group(y, "year", n = 2)
calendar_group.clock_year_week_day <- function(x, precision, ..., n = 1L) {
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
  if (precision == PRECISION_WEEK) {
    value <- get_week(x)
    value <- group_component1(value, n)
    x <- set_week(x, value)
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

#' Narrow: year-week-day
#'
#' This is a year-week-day method for the [calendar_narrow()] generic. It
#' narrows a year-week-day vector to the specified `precision`.
#'
#' @inheritParams year-week-day-group
#'
#' @return `x` narrowed to the supplied `precision`.
#'
#' @name year-week-day-narrow
#'
#' @export
#' @examples
#' # Day precision
#' x <- year_week_day(2019, 1, 5)
#' x
#'
#' # Narrowed to week precision
#' calendar_narrow(x, "week")
calendar_narrow.clock_year_week_day <- function(x, precision) {
  check_precision(precision)
  precision <- precision_to_integer(precision)

  start <- week_start(x)

  out_fields <- list()
  x_fields <- unclass(x)

  if (precision >= PRECISION_YEAR) {
    out_fields[["year"]] <- x_fields[["year"]]
  }
  if (precision >= PRECISION_WEEK) {
    out_fields[["week"]] <- x_fields[["week"]]
  }
  if (precision >= PRECISION_DAY) {
    out_fields[["day"]] <- x_fields[["day"]]
  }

  out_fields <- calendar_narrow_time(out_fields, precision, x_fields)

  new_year_week_day_from_fields(out_fields, precision, start, names = names(x))
}

# ------------------------------------------------------------------------------

#' Widen: year-week-day
#'
#' This is a year-week-day method for the [calendar_widen()] generic. It
#' widens a year-week-day vector to the specified `precision`.
#'
#' @inheritParams year-week-day-group
#'
#' @return `x` widened to the supplied `precision`.
#'
#' @name year-week-day-widen
#'
#' @export
#' @examples
#' # Week precision
#' x <- year_week_day(2019, 1, start = clock_weekdays$monday)
#' x
#'
#' # Widen to day precision
#' # In this calendar, the first day of the week is a Monday
#' calendar_widen(x, "day")
#'
#' # Or second precision
#' sec <- calendar_widen(x, "second")
#' sec
calendar_widen.clock_year_week_day <- function(x, precision) {
  x_precision <- calendar_precision_attribute(x)

  check_precision(precision)
  precision <- precision_to_integer(precision)

  if (precision >= PRECISION_WEEK && x_precision < PRECISION_WEEK) {
    x <- set_week(x, 1L)
  }
  if (precision >= PRECISION_DAY && x_precision < PRECISION_DAY) {
    x <- set_day(x, 1L)
  }

  x <- calendar_widen_time(x, x_precision, precision)

  x
}

# ------------------------------------------------------------------------------

#' Boundaries: year-week-day
#'
#' This is an year-week-day method for the [calendar_start()] and
#' [calendar_end()] generics. They adjust components of a calendar to the
#' start or end of a specified `precision`.
#'
#' @inheritParams year-week-day-group
#'
#' @return `x` at the same precision, but with some components altered to be
#'   at the boundary value.
#'
#' @name year-week-day-boundary
#'
#' @examples
#' x <- year_week_day(2019:2020, 5, 6, 10)
#' x
#'
#' # Compute the last moment of the last week of the year
#' calendar_end(x, "year")
#'
#' # Compare that to just setting the week to `"last"`,
#' # which doesn't affect the other components
#' set_week(x, "last")
NULL

#' @rdname year-week-day-boundary
#' @export
calendar_start.clock_year_week_day <- function(x, precision) {
  x_precision <- calendar_precision_attribute(x)

  check_precision(precision)
  precision <- precision_to_integer(precision)

  calendar_start_end_checks(x, x_precision, precision, "start")

  if (precision <= PRECISION_YEAR && x_precision > PRECISION_YEAR) {
    x <- set_week(x, 1L)
  }
  if (precision <= PRECISION_WEEK && x_precision > PRECISION_WEEK) {
    x <- set_day(x, 1L)
  }

  x <- calendar_start_time(x, x_precision, precision)

  x
}

#' @rdname year-week-day-boundary
#' @export
calendar_end.clock_year_week_day <- function(x, precision) {
  x_precision <- calendar_precision_attribute(x)

  check_precision(precision)
  precision <- precision_to_integer(precision)

  calendar_start_end_checks(x, x_precision, precision, "end")

  if (precision <= PRECISION_YEAR && x_precision > PRECISION_YEAR) {
    x <- set_week(x, "last")
  }
  if (precision <= PRECISION_WEEK && x_precision > PRECISION_WEEK) {
    x <- set_day(x, 7L)
  }

  x <- calendar_end_time(x, x_precision, precision)

  x
}

# ------------------------------------------------------------------------------

#' Counting: year-week-day
#'
#' This is an year-week-day method for the [calendar_count_between()]
#' generic. It counts the number of `precision` units between `start` and `end`
#' (i.e., the number of years).
#'
#' @inheritParams calendar-count-between
#'
#' @param start,end `[clock_year_week_day]`
#'
#'   A pair of year-week-day vectors. These will be recycled to their
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
#' @name year-week-day-count-between
#'
#' @export
#' @examples
#' # Compute the number of whole years between two dates
#' x <- year_week_day(2001, 1, 2)
#' y <- year_week_day(2021, 1, c(1, 3))
#' calendar_count_between(x, y, "year")
calendar_count_between.clock_year_week_day <- function(
  start,
  end,
  precision,
  ...,
  n = 1L
) {
  NextMethod()
}

#' @export
calendar_count_between_standardize_precision_n.clock_year_week_day <- function(
  x,
  precision,
  n
) {
  check_precision(precision)
  precision_int <- precision_to_integer(precision)

  allowed_precisions <- c(PRECISION_YEAR)

  if (!(precision_int %in% allowed_precisions)) {
    abort("`precision` must be one of: 'year'.")
  }

  list(precision = precision, n = n)
}

#' @export
calendar_count_between_compute.clock_year_week_day <- function(
  start,
  end,
  precision
) {
  check_precision(precision)
  precision <- precision_to_integer(precision)

  if (precision == PRECISION_YEAR) {
    out <- get_year(end) - get_year(start)
    return(out)
  }

  abort("Internal error: `precision` should be 'year' at this point.")
}

#' @export
calendar_count_between_proxy_compare.clock_year_week_day <- function(
  start,
  end,
  precision
) {
  check_precision(precision)
  precision <- precision_to_integer(precision)

  start <- vec_proxy_compare(start)
  end <- vec_proxy_compare(end)

  if (precision >= PRECISION_YEAR) {
    start$year <- NULL
    end$year <- NULL
  }

  list(start = start, end = end)
}

# ------------------------------------------------------------------------------

#' Sequences: year-week-day
#'
#' @description
#' This is a year-week-day method for the [seq()] generic.
#'
#' Sequences can only be generated for `"year"` precision
#' year-week-day vectors. If you need to generate week-based sequences,
#' you'll have to convert to a time point first.
#'
#' When calling `seq()`, exactly two of the following must be specified:
#' - `to`
#' - `by`
#' - Either `length.out` or `along.with`
#'
#' @inheritParams seq.clock_duration
#'
#' @param from `[clock_year_week_day(1)]`
#'
#'   A `"year"` precision year-week-day to start the sequence
#'   from.
#'
#'   `from` is always included in the result.
#'
#' @param to `[clock_year_week_day(1) / NULL]`
#'
#'   A `"year"` precision year-week-day to stop the sequence
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
#' # Yearly sequence
#' x <- seq(year_week_day(2020), year_week_day(2026), by = 2)
#' x
#'
#' # Which we can then set the week of.
#' # Some years have 53 weeks, some have 52.
#' set_week(x, "last")
seq.clock_year_week_day <- function(
  from,
  to = NULL,
  by = NULL,
  length.out = NULL,
  along.with = NULL,
  ...
) {
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

#' @export
clock_minimum.clock_year_week_day <- function(x) {
  names <- NULL
  start <- week_start(x)
  fields <- list(year = clock_calendar_year_minimum)
  year <- new_year_week_day_from_fields(fields, PRECISION_YEAR, start, names)

  precision <- calendar_precision_attribute(x)
  precision <- precision_to_string(precision)

  calendar_minimum(precision, year)
}

#' @export
clock_maximum.clock_year_week_day <- function(x) {
  names <- NULL
  start <- week_start(x)
  fields <- list(year = clock_calendar_year_maximum)
  year <- new_year_week_day_from_fields(fields, PRECISION_YEAR, start, names)

  precision <- calendar_precision_attribute(x)
  precision <- precision_to_string(precision)

  calendar_maximum(precision, year)
}

# ------------------------------------------------------------------------------

week_start <- function(x) {
  attr(x, "start", exact = TRUE)
}

week_start_prettify <- function(start, ..., abbreviate = FALSE) {
  check_dots_empty()
  if (abbreviate) {
    c(
      "Sun",
      "Mon",
      "Tue",
      "Wed",
      "Thu",
      "Fri",
      "Sat"
    )[start]
  } else {
    c(
      "Sunday",
      "Monday",
      "Tuesday",
      "Wednesday",
      "Thursday",
      "Friday",
      "Saturday"
    )[start]
  }
}

week_validate_start <- function(start, ..., error_call = caller_env()) {
  if (is_null(start)) {
    return(1L)
  }

  check_number_whole(start, min = 1, max = 7, call = error_call)
  start <- vec_cast(start, integer(), call = error_call)

  start
}
