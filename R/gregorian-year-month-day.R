#' @export
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
    precision <- "year"
    fields <- list(year = year)
  } else if (is_null(day)) {
    precision <- "month"
    fields <- list(year = year, month = month)
  } else if (is_null(hour)) {
    precision <- "day"
    fields <- list(year = year, month = month, day = day)
  } else if (is_null(minute)) {
    precision <- "hour"
    fields <- list(year = year, month = month, day = day, hour = hour)
  } else if (is_null(second)) {
    precision <- "minute"
    fields <- list(year = year, month = month, day = day, hour = hour, minute = minute)
  } else if (is_null(subsecond)) {
    precision <- "second"
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

  out <- new_year_month_day_from_fields(fields, precision)

  if (last) {
    out <- set_day(out, "last")
  }

  out
}

# ------------------------------------------------------------------------------

#' @export
new_year_month_day <- function(year = integer(),
                               month = integer(),
                               day = integer(),
                               hour = integer(),
                               minute = integer(),
                               second = integer(),
                               subsecond = integer(),
                               precision = "year",
                               ...,
                               names = NULL,
                               class = NULL) {
  if (!year_month_day_is_valid_precision(precision)) {
    abort("`precision` must be a valid precision for 'year_month_day'.")
  }

  fields <- switch(
    precision,
    year = list(year = year),
    month = list(year = year, month = month),
    day = list(year = year, month = month, day = day),
    hour = list(year = year, month = month, day = day, hour = hour),
    minute = list(year = year, month = month, day = day, hour = hour, minute = minute),
    second = list(year = year, month = month, day = day, hour = hour, minute = minute, second = second),
    millisecond = list(year = year, month = month, day = day, hour = hour, minute = minute, second = second, subsecond = subsecond),
    microsecond = list(year = year, month = month, day = day, hour = hour, minute = minute, second = second, subsecond = subsecond),
    nanosecond = list(year = year, month = month, day = day, hour = hour, minute = minute, second = second, subsecond = subsecond)
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
    class = c(class, "clock_year_month_day")
  )
}

new_year_month_day_from_fields <- function(fields, precision, names = NULL) {
  new_year_month_day(
    year = fields$year,
    month = fields$month,
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
vec_proxy.clock_year_month_day <- function(x, ...) {
  clock_rcrd_proxy(x, names(x))
}

#' @export
vec_restore.clock_year_month_day <- function(x, to, ...) {
  fields <- clock_rcrd_restore_fields(x)
  names <- clock_rcrd_restore_names(x)
  precision <- calendar_precision(to)
  new_year_month_day_from_fields(fields, precision, names)
}

#' @export
vec_proxy_equal.clock_year_month_day <- function(x, ...) {
  clock_rcrd_proxy_equal(x)
}

# ------------------------------------------------------------------------------

#' @export
format.clock_year_month_day <- function(x, ...) {
  out <- format_year_month_day_cpp(x, calendar_precision(x))
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

#' @export
is_year_month_day <- function(x) {
  inherits(x, "clock_year_month_day")
}

# ------------------------------------------------------------------------------

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
  if (!is_string(precision)) {
    return(FALSE)
  }
  precision %in% c("year", "month", calendar_standard_precisions())
}

# ------------------------------------------------------------------------------

#' @export
calendar_is_valid_component.clock_year_month_day <- function(x, component) {
  year_month_day_is_valid_component(component)
}
year_month_day_is_valid_component <- function(component) {
  if (!is_string(component)) {
    return(FALSE)
  }
  component %in% c("year", "month", "day", calendar_standard_components())
}

# ------------------------------------------------------------------------------

#' @export
invalid_detect.clock_year_month_day <- function(x) {
  invalid_detect_year_month_day_cpp(x, calendar_precision(x))
}

#' @export
invalid_any.clock_year_month_day <- function(x) {
  invalid_any_year_month_day_cpp(x, calendar_precision(x))
}

#' @export
invalid_count.clock_year_month_day <- function(x) {
  invalid_count_year_month_day_cpp(x, calendar_precision(x))
}

#' @export
invalid_resolve.clock_year_month_day <- function(x, ..., invalid = "error") {
  check_dots_empty()
  precision <- calendar_precision(x)
  fields <- invalid_resolve_year_month_day_cpp(x, precision, invalid)
  new_year_month_day_from_fields(fields, precision, names = names(x))
}

# ------------------------------------------------------------------------------

#' @export
get_year.clock_year_month_day <- function(x) {
  field_year(x)
}

#' @export
get_month.clock_year_month_day <- function(x) {
  calendar_require_minimum_precision(x, "month", "get_month")
  field_month(x)
}

#' @export
get_day.clock_year_month_day <- function(x) {
  calendar_require_minimum_precision(x, "day", "get_day")
  field_day(x)
}

#' @export
get_hour.clock_year_month_day <- function(x) {
  calendar_require_minimum_precision(x, "hour", "get_hour")
  field_hour(x)
}

#' @export
get_minute.clock_year_month_day <- function(x) {
  calendar_require_minimum_precision(x, "minute", "get_minute")
  field_minute(x)
}

#' @export
get_second.clock_year_month_day <- function(x) {
  calendar_require_minimum_precision(x, "second", "get_second")
  field_second(x)
}

#' @export
get_millisecond.clock_year_month_day <- function(x) {
  calendar_require_precision(x, "millisecond", "get_millisecond")
  field_subsecond(x)
}

#' @export
get_microsecond.clock_year_month_day <- function(x) {
  calendar_require_precision(x, "microsecond", "get_microsecond")
  field_subsecond(x)
}

#' @export
get_nanosecond.clock_year_month_day <- function(x) {
  calendar_require_precision(x, "nanosecond", "get_nanosecond")
  field_subsecond(x)
}

# ------------------------------------------------------------------------------

#' @export
calendar_get_component.clock_year_month_day <- function(x, component) {
  switch(
    component,
    year = get_year(x),
    month = get_month(x),
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
set_year.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  set_field_year_month_day(x, value, "year")
}

#' @export
set_month.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, "year", "set_month")
  set_field_year_month_day(x, value, "month")
}

#' @export
set_day.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, "month", "set_day")
  set_field_year_month_day(x, value, "day")
}

#' @export
set_hour.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, "day", "set_hour")
  set_field_year_month_day(x, value, "hour")
}

#' @export
set_minute.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, "hour", "set_minute")
  set_field_year_month_day(x, value, "minute")
}

#' @export
set_second.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_minimum_precision(x, "minute", "set_second")
  set_field_year_month_day(x, value, "second")
}

#' @export
set_millisecond.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c("second", "millisecond"), "set_millisecond")
  set_field_year_month_day(x, value, "millisecond")
}

#' @export
set_microsecond.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c("second", "microsecond"), "set_microsecond")
  set_field_year_month_day(x, value, "microsecond")
}

#' @export
set_nanosecond.clock_year_month_day <- function(x, value, ...) {
  check_dots_empty()
  calendar_require_any_of_precisions(x, c("second", "nanosecond"), "set_nanosecond")
  set_field_year_month_day(x, value, "nanosecond")
}

set_field_year_month_day <- function(x, value, component) {
  if (is_last(value) && identical(component, "day")) {
    return(set_field_year_month_day_last(x))
  }

  precision_fields <- calendar_precision(x)
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
  precision_fields <- calendar_precision(x)
  precision_out <- precision_common2(precision_fields, "day")

  result <- set_field_year_month_day_last_cpp(x, precision_fields)
  fields <- result$fields
  fields[["day"]] <- result$value

  new_year_month_day_from_fields(fields, precision_out, names = names(x))
}

# ------------------------------------------------------------------------------

#' @export
calendar_set_component.clock_year_month_day <- function(x, value, component, ...) {
  switch(
    component,
    year = set_year(x, value, ...),
    month = set_month(x, value, ...),
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
calendar_check_component_range.clock_year_month_day <- function(x, value, component, value_arg) {
  year_month_day_check_range_cpp(value, component, value_arg)
}

# ------------------------------------------------------------------------------

#' @export
calendar_name.clock_year_month_day <- function(x) {
  "year_month_day"
}

# ------------------------------------------------------------------------------

#' @export
calendar_component_to_precision.clock_year_month_day <- function(x, component) {
  year_month_day_component_to_precision(component)
}
year_month_day_component_to_precision <- function(component) {
  switch (
    component,
    year = component,
    month = component,
    day = component,
    hour = component,
    minute = component,
    second = component,
    millisecond = component,
    microsecond = component,
    nanosecond = component,
    abort("Internal error: Unknown component name.")
  )
}

#' @export
calendar_component_to_field.clock_year_month_day <- function(x, component) {
  year_month_day_component_to_field(component)
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

#' @export
calendar_precision_to_component.clock_year_month_day <- function(x, precision) {
  year_month_day_precision_to_component(precision)
}
year_month_day_precision_to_component <- function(precision) {
  switch (
    precision,
    year = precision,
    month = precision,
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
calendar_precision_to_field.clock_year_month_day <- function(x, precision) {
  year_month_day_precision_to_field(precision)
}
year_month_day_precision_to_field <- function(precision) {
  switch (
    precision,
    year = precision,
    month = precision,
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

  precision <- calendar_precision(x)

  if (precision_value(precision) > PRECISION_MONTH) {
    stop_incompatible_op(op, x, y, ...)
  }

  fields <- year_month_day_minus_year_month_day_cpp(x, y, precision)

  new_duration_from_fields(fields, precision, names = names)
}

# ------------------------------------------------------------------------------

#' @export
add_years.clock_year_month_day <- function(x, n, ...) {
  year_month_day_plus_duration(x, n, "year")
}

#' @export
add_quarters.clock_year_month_day <- function(x, n, ...) {
  calendar_require_minimum_precision(x, "month", "add_quarters")
  year_month_day_plus_duration(x, n, "quarter")
}

#' @export
add_months.clock_year_month_day <- function(x, n, ...) {
  calendar_require_minimum_precision(x, "month", "add_months")
  year_month_day_plus_duration(x, n, "month")
}

year_month_day_plus_duration <- function(x, n, precision_n) {
  precision_fields <- calendar_precision(x)

  n <- duration_collect_n(n, precision_n)
  args <- vec_recycle_common(x = x, n = n)
  x <- args$x
  n <- args$n

  names <- names_common(x, n)

  fields <- year_month_day_plus_duration_cpp(x, n, precision_fields, precision_n)

  new_year_month_day_from_fields(fields, precision_fields, names = names)
}

# ------------------------------------------------------------------------------

#' @export
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

#' @export
as_year_month_day.clock_calendar <- function(x) {
  as_year_month_day(as_sys_time(x))
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_year_month_day <- function(x) {
  calendar_require_all_valid(x, "as_sys_time")
  precision <- calendar_precision(x)
  fields <- as_sys_time_year_month_day_cpp(x, precision)
  duration <- new_duration_from_fields(fields, precision)
  new_sys_time(duration, names = names(x))
}

#' @export
as_naive_time.clock_year_month_day <- function(x) {
  as_naive_time(as_sys_time(x))
}

# ------------------------------------------------------------------------------

#' @export
calendar_component_grouper.clock_year_month_day <- function(x, component) {
  switch(
    component,
    year = group_component0,
    month = group_component1,
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
calendar_narrow.clock_year_month_day <- function(x, precision) {
  x_precision <- calendar_precision(x)

  if (x_precision == precision) {
    return(x)
  }

  x_precision_value <- precision_value(x_precision)
  out_precision_value <- precision_value(precision)

  out_fields <- list()
  x_fields <- calendar_fields(x)

  if (out_precision_value >= PRECISION_YEAR) {
    out_fields[["year"]] <- x_fields[["year"]]
  }
  if (out_precision_value >= PRECISION_MONTH) {
    out_fields[["month"]] <- x_fields[["month"]]
  }
  if (out_precision_value >= PRECISION_DAY) {
    out_fields[["day"]] <- x_fields[["day"]]
  }
  if (out_precision_value >= PRECISION_HOUR) {
    out_fields <- calendar_narrow_time(out_fields, out_precision_value, x_fields, x_precision_value)
  }

  new_year_month_day_from_fields(out_fields, precision = precision, names = names(x))
}
