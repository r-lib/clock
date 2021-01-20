#' @export
iso_year_week_day <- function(year,
                              week = NULL,
                              day = NULL,
                              hour = NULL,
                              minute = NULL,
                              second = NULL,
                              subsecond = NULL,
                              ...,
                              subsecond_precision = NULL) {
  check_dots_empty()

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
    fields <- list(year = year, week = week, day = day, hour = hour, minute = minute)
  } else if (is_null(subsecond)) {
    precision <- PRECISION_SECOND
    fields <- list(year = year, week = week, day = day, hour = hour, minute = minute, second = second)
  } else {
    precision <- calendar_validate_subsecond_precision(subsecond_precision)
    fields <- list(year = year, week = week, day = day, hour = hour, minute = minute, second = second, subsecond = subsecond)
  }

  if (is_last(fields$week)) {
    fields$week <- 1L
    last <- TRUE
  } else {
    last <- FALSE
  }

  fields <- vec_recycle_common(!!!fields)
  fields <- vec_cast_common(!!!fields, .to = integer())

  fields <- collect_iso_year_week_day_fields(fields, precision)

  out <- new_iso_year_week_day_from_fields(fields, precision)

  if (last) {
    out <- set_week(out, "last")
  }

  out
}

# ------------------------------------------------------------------------------

#' @export
new_iso_year_week_day <- function(year = integer(),
                                  week = integer(),
                                  day = integer(),
                                  hour = integer(),
                                  minute = integer(),
                                  second = integer(),
                                  subsecond = integer(),
                                  precision = 0L,
                                  ...,
                                  names = NULL,
                                  class = NULL) {
  if (!is.integer(precision)) {
    abort("`precision` must be an integer.")
  }

  precision_string <- precision_to_string(precision)

  fields <- switch(
    precision_string,
    year = list(year = year),
    week = list(year = year, week = week),
    day = list(year = year, week = week, day = day),
    hour = list(year = year, week = week, day = day, hour = hour),
    minute = list(year = year, week = week, day = day, hour = hour, minute = minute),
    second = list(year = year, week = week, day = day, hour = hour, minute = minute, second = second),
    millisecond = list(year = year, week = week, day = day, hour = hour, minute = minute, second = second, subsecond = subsecond),
    microsecond = list(year = year, week = week, day = day, hour = hour, minute = minute, second = second, subsecond = subsecond),
    nanosecond = list(year = year, week = week, day = day, hour = hour, minute = minute, second = second, subsecond = subsecond)
  )

  field_names <- names(fields)
  for (i in seq_along(fields)) {
    int_assert(fields[[i]], field_names[[i]])
  }

  new_calendar(
    fields = fields,
    precision = precision,
    ...,
    names = names,
    class = c(class, "clock_iso_year_week_day")
  )
}

new_iso_year_week_day_from_fields <- function(fields, precision, names = NULL) {
  new_iso_year_week_day(
    year = fields$year,
    week = fields$week,
    day = fields$day,
    hour = fields$hour,
    minute = fields$minute,
    second = fields$second,
    subsecond = fields$subsecond,
    precision = precision,
    names = names
  )
}

# ------------------------------------------------------------------------------

#' @export
vec_proxy.clock_iso_year_week_day <- function(x, ...) {
  clock_rcrd_proxy(x, names(x))
}

#' @export
vec_restore.clock_iso_year_week_day <- function(x, to, ...) {
  fields <- clock_rcrd_restore_fields(x)
  names <- clock_rcrd_restore_names(x)
  precision <- calendar_precision(to)
  new_iso_year_week_day_from_fields(fields, precision, names)
}

#' @export
vec_proxy_equal.clock_iso_year_week_day <- function(x, ...) {
  clock_rcrd_proxy_equal(x)
}

# ------------------------------------------------------------------------------

#' @export
format.clock_iso_year_week_day <- function(x, ...) {
  out <- format_iso_year_week_day_cpp(x, calendar_precision(x))
  names(out) <- names(x)
  out
}

#' @export
vec_ptype_full.clock_iso_year_week_day <- function(x, ...) {
  calendar_ptype_full(x, "iso_year_week_day")
}

#' @export
vec_ptype_abbr.clock_iso_year_week_day <- function(x, ...) {
  calendar_ptype_abbr(x, "iso_ywd")
}

# ------------------------------------------------------------------------------

#' @export
is_iso_year_week_day <- function(x) {
  inherits(x, "clock_iso_year_week_day")
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype2.clock_iso_year_week_day.clock_iso_year_week_day <- function(x, y, ...) {
  ptype2_calendar_and_calendar(x, y, ...)
}

#' @export
vec_cast.clock_iso_year_week_day.clock_iso_year_week_day <- function(x, to, ...) {
  cast_calendar_to_calendar(x, to, ...)
}

# ------------------------------------------------------------------------------

#' @export
calendar_is_valid_precision.clock_iso_year_week_day <- function(x, precision) {
  iso_year_week_day_is_valid_precision(precision)
}

iso_year_week_day_is_valid_precision <- function(precision) {
  if (!is_valid_precision(precision)) {
    FALSE
  } else if (precision == PRECISION_YEAR || precision == PRECISION_WEEK) {
    TRUE
  } else if (precision >= PRECISION_DAY && precision <= PRECISION_NANOSECOND) {
    TRUE
  } else {
    FALSE
  }
}

# ------------------------------------------------------------------------------

#' @export
calendar_is_valid_component.clock_iso_year_week_day <- function(x, component) {
  iso_year_week_day_is_valid_component(component)
}
iso_year_week_day_is_valid_component <- function(component) {
  if (!is_string(component)) {
    return(FALSE)
  }
  component %in% c("year", "week", "day", calendar_standard_components())
}

# ------------------------------------------------------------------------------

#' @export
invalid_detect.clock_iso_year_week_day <- function(x) {
  invalid_detect_iso_year_week_day_cpp(x, calendar_precision(x))
}

#' @export
invalid_any.clock_iso_year_week_day <- function(x) {
  invalid_any_iso_year_week_day_cpp(x, calendar_precision(x))
}

#' @export
invalid_count.clock_iso_year_week_day <- function(x) {
  invalid_count_iso_year_week_day_cpp(x, calendar_precision(x))
}

#' @export
invalid_resolve.clock_iso_year_week_day <- function(x, ..., invalid = "error") {
  check_dots_empty()
  precision <- calendar_precision(x)
  fields <- invalid_resolve_iso_year_week_day_cpp(x, precision, invalid)
  new_iso_year_week_day_from_fields(fields, precision, names = names(x))
}

# ------------------------------------------------------------------------------

#' @export
get_year.clock_iso_year_week_day <- function(x) {
  field_year(x)
}

#' @export
get_week.clock_iso_year_week_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_WEEK, "get_week")
  field_week(x)
}

#' @export
get_day.clock_iso_year_week_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_DAY, "get_day")
  field_day(x)
}

#' @export
get_hour.clock_iso_year_week_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_HOUR, "get_hour")
  field_hour(x)
}

#' @export
get_minute.clock_iso_year_week_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_MINUTE, "get_minute")
  field_minute(x)
}

#' @export
get_second.clock_iso_year_week_day <- function(x) {
  calendar_require_minimum_precision(x, PRECISION_SECOND, "get_second")
  field_second(x)
}

#' @export
get_millisecond.clock_iso_year_week_day <- function(x) {
  calendar_require_precision(x, PRECISION_MILLISECOND, "get_millisecond")
  field_subsecond(x)
}

#' @export
get_microsecond.clock_iso_year_week_day <- function(x) {
  calendar_require_precision(x, PRECISION_MICROSECOND, "get_microsecond")
  field_subsecond(x)
}

#' @export
get_nanosecond.clock_iso_year_week_day <- function(x) {
  calendar_require_precision(x, PRECISION_NANOSECOND, "get_nanosecond")
  field_subsecond(x)
}

# ------------------------------------------------------------------------------

#' @export
calendar_get_component.clock_iso_year_week_day <- function(x, component) {
  switch(
    component,
    year = get_year(x),
    week = get_week(x),
    day = get_day(x),
    hour = get_hour(x),
    minute = get_minute(x),
    second = get_second(x),
    millisecond = get_millisecond(x),
    microsecond = get_microsecond(x),
    nanosecond = get_nanosecond(x),
    abort("Internal error: Unknown component name.")
  )
}

# ------------------------------------------------------------------------------

#' @export
set_year.clock_iso_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  set_field_iso_year_week_day(x, value, "year")
}

#' @export
set_week.clock_iso_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_YEAR, "set_week")
  set_field_iso_year_week_day(x, value, "week")
}

#' @export
set_day.clock_iso_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_WEEK, "set_day")
  set_field_iso_year_week_day(x, value, "day")
}

#' @export
set_hour.clock_iso_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_DAY, "set_hour")
  set_field_iso_year_week_day(x, value, "hour")
}

#' @export
set_minute.clock_iso_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_HOUR, "set_minute")
  set_field_iso_year_week_day(x, value, "minute")
}

#' @export
set_second.clock_iso_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, PRECISION_MINUTE, "set_second")
  set_field_iso_year_week_day(x, value, "second")
}

#' @export
set_millisecond.clock_iso_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c(PRECISION_SECOND, PRECISION_MILLISECOND), "set_millisecond")
  set_field_iso_year_week_day(x, value, "millisecond")
}

#' @export
set_microsecond.clock_iso_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c(PRECISION_SECOND, PRECISION_MICROSECOND), "set_microsecond")
  set_field_iso_year_week_day(x, value, "microsecond")
}

#' @export
set_nanosecond.clock_iso_year_week_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c(PRECISION_SECOND, PRECISION_NANOSECOND), "set_nanosecond")
  set_field_iso_year_week_day(x, value, "nanosecond")
}

set_field_iso_year_week_day <- function(x, value, component) {
  if (is_last(value) && identical(component, "week")) {
    return(set_field_iso_year_week_day_last(x))
  }

  precision_fields <- calendar_precision(x)
  precision_value <- iso_year_week_day_component_to_precision(component)
  precision_out <- precision_common2(precision_fields, precision_value)

  value <- vec_cast(value, integer(), x_arg = "value")
  args <- vec_recycle_common(x = x, value = value)
  x <- args$x
  value <- args$value

  result <- set_field_iso_year_week_day_cpp(x, value, precision_fields, precision_value)
  fields <- result$fields
  field <- iso_year_week_day_component_to_field(component)
  fields[[field]] <- result$value

  new_iso_year_week_day_from_fields(fields, precision_out, names = names(x))
}

set_field_iso_year_week_day_last <- function(x) {
  precision_fields <- calendar_precision(x)
  precision_out <- precision_common2(precision_fields, PRECISION_WEEK)

  result <- set_field_iso_year_week_day_last_cpp(x, precision_fields)
  fields <- result$fields
  fields[["week"]] <- result$value

  new_iso_year_week_day_from_fields(fields, precision_out, names = names(x))
}

# ------------------------------------------------------------------------------

#' @export
calendar_set_component.clock_iso_year_week_day <- function(x, value, component, ...) {
  switch(
    component,
    year = set_year(x, value, ...),
    week = set_week(x, value, ...),
    day = set_day(x, value, ...),
    hour = set_hour(x, value, ...),
    minute = set_minute(x, value, ...),
    second = set_second(x, value, ...),
    millisecond = set_millisecond(x, value, ...),
    microsecond = set_microsecond(x, value, ...),
    nanosecond = set_nanosecond(x, value, ...),
    abort("Internal error: Unknown component name.")
  )
}

# ------------------------------------------------------------------------------

#' @export
calendar_check_component_range.clock_iso_year_week_day <- function(x, value, component, value_arg) {
  iso_year_week_day_check_range_cpp(value, component, value_arg)
}

# ------------------------------------------------------------------------------

#' @export
calendar_name.clock_iso_year_week_day <- function(x) {
  "iso_year_week_day"
}

# ------------------------------------------------------------------------------

#' @export
calendar_component_to_precision.clock_iso_year_week_day <- function(x, component) {
  iso_year_week_day_component_to_precision(component)
}
iso_year_week_day_component_to_precision <- function(component) {
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

#' @export
calendar_component_to_field.clock_iso_year_week_day <- function(x, component) {
  iso_year_week_day_component_to_field(component)
}
iso_year_week_day_component_to_field <- function(component) {
  switch (
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

#' @export
calendar_precision_to_component.clock_iso_year_week_day <- function(x, precision) {
  iso_year_week_day_precision_to_component(precision)
}
iso_year_week_day_precision_to_component <- function(precision) {
  precision <- precision_to_string(precision)

  switch (
    precision,
    year = precision,
    week = precision,
    day = precision,
    hour = precision,
    minute = precision,
    second = precision,
    millisecond = precision,
    microsecond = precision,
    nanosecond = precision,
    abort("Internal error: Unknown precision.")
  )
}

#' @export
calendar_precision_to_field.clock_iso_year_week_day <- function(x, precision) {
  iso_year_week_day_precision_to_field(precision)
}
iso_year_week_day_precision_to_field <- function(precision) {
  precision <- precision_to_string(precision)

  switch (
    precision,
    year = precision,
    week = precision,
    day = precision,
    hour = precision,
    minute = precision,
    second = precision,
    millisecond = "subsecond",
    microsecond = "subsecond",
    nanosecond = "subsecond",
    abort("Internal error: Unknown precision.")
  )
}

# ------------------------------------------------------------------------------

#' @method vec_arith clock_iso_year_week_day
#' @export
vec_arith.clock_iso_year_week_day <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_iso_year_week_day", y)
}

#' @method vec_arith.clock_iso_year_week_day MISSING
#' @export
vec_arith.clock_iso_year_week_day.MISSING <- function(op, x, y, ...) {
  arith_calendar_and_missing(op, x, y, ...)
}

#' @method vec_arith.clock_iso_year_week_day clock_iso_year_week_day
#' @export
vec_arith.clock_iso_year_week_day.clock_iso_year_week_day <- function(op, x, y, ...) {
  arith_calendar_and_calendar(op, x, y, ..., calendar_minus_calendar_fn = iso_year_week_day_minus_iso_year_week_day)
}

#' @method vec_arith.clock_iso_year_week_day clock_duration
#' @export
vec_arith.clock_iso_year_week_day.clock_duration <- function(op, x, y, ...) {
  arith_calendar_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration clock_iso_year_week_day
#' @export
vec_arith.clock_duration.clock_iso_year_week_day <- function(op, x, y, ...) {
  arith_duration_and_calendar(op, x, y, ...)
}

#' @method vec_arith.clock_iso_year_week_day numeric
#' @export
vec_arith.clock_iso_year_week_day.numeric <- function(op, x, y, ...) {
  arith_calendar_and_numeric(op, x, y, ...)
}

#' @method vec_arith.numeric clock_iso_year_week_day
#' @export
vec_arith.numeric.clock_iso_year_week_day <- function(op, x, y, ...) {
  arith_numeric_and_calendar(op, x, y, ...)
}

iso_year_week_day_minus_iso_year_week_day <- function(op, x, y, ...) {
  args <- vec_recycle_common(x = x, y = y)
  args <- vec_cast_common(!!!args)
  x <- args$x
  y <- args$y

  names <- names_common(x, y)

  precision <- calendar_precision(x)

  if (precision > PRECISION_YEAR) {
    stop_incompatible_op(op, x, y, ...)
  }

  fields <- iso_year_week_day_minus_iso_year_week_day_cpp(x, y, precision)

  new_duration_from_fields(fields, precision, names = names)
}

# ------------------------------------------------------------------------------

#' @export
add_years.clock_iso_year_week_day <- function(x, n, ...) {
  iso_year_week_day_plus_duration(x, n, PRECISION_YEAR)
}

iso_year_week_day_plus_duration <- function(x, n, precision_n) {
  precision_fields <- calendar_precision(x)

  n <- duration_collect_n(n, precision_n)
  args <- vec_recycle_common(x = x, n = n)
  x <- args$x
  n <- args$n

  names <- names_common(x, n)

  fields <- iso_year_week_day_plus_duration_cpp(x, n, precision_fields, precision_n)

  new_iso_year_week_day_from_fields(fields, precision_fields, names = names)
}

# ------------------------------------------------------------------------------

#' @export
as_iso_year_week_day <- function(x)  {
  UseMethod("as_iso_year_week_day")
}

#' @export
as_iso_year_week_day.default <- function(x) {
  stop_clock_unsupported_conversion(x, "clock_iso_year_week_day")
}

#' @export
as_iso_year_week_day.clock_iso_year_week_day <- function(x) {
  x
}

#' @export
as_iso_year_week_day.clock_calendar <- function(x) {
  as_iso_year_week_day(as_sys_time(x))
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_iso_year_week_day <- function(x) {
  calendar_require_all_valid(x, "as_sys_time")
  precision <- calendar_precision(x)
  fields <- as_sys_time_iso_year_week_day_cpp(x, precision)
  duration <- new_duration_from_fields(fields, precision)
  new_sys_time(duration, names = names(x))
}

#' @export
as_naive_time.clock_iso_year_week_day <- function(x) {
  as_naive_time(as_sys_time(x))
}

# ------------------------------------------------------------------------------

#' @export
calendar_component_grouper.clock_iso_year_week_day <- function(x, component) {
  switch(
    component,
    year = group_component0,
    week = group_component1,
    day = group_component1,
    hour = group_component0,
    minute = group_component0,
    second = group_component0,
    millisecond = group_component0,
    microsecond = group_component0,
    nanosecond = group_component0
  )
}

# ------------------------------------------------------------------------------

#' @export
calendar_narrow.clock_iso_year_week_day <- function(x, precision) {
  x_precision <- calendar_precision(x)
  precision <- validate_precision(precision)

  if (x_precision == precision) {
    return(x)
  }

  out_fields <- list()
  x_fields <- calendar_fields(x)

  if (precision >= PRECISION_YEAR) {
    out_fields[["year"]] <- x_fields[["year"]]
  }
  if (precision >= PRECISION_WEEK) {
    out_fields[["week"]] <- x_fields[["week"]]
  }
  if (precision >= PRECISION_DAY) {
    out_fields[["day"]] <- x_fields[["day"]]
  }
  if (precision >= PRECISION_HOUR) {
    out_fields <- calendar_narrow_time(out_fields, precision, x_fields, x_precision)
  }

  new_iso_year_week_day_from_fields(out_fields, precision = precision, names = names(x))
}
