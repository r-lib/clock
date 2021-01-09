#' @export
year_month_day <- function(year,
                           month = NULL,
                           day = NULL,
                           hour = NULL,
                           minute = NULL,
                           second = NULL) {
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
  } else {
    precision <- "second"
    fields <- list(year = year, month = month, day = day, hour = hour, minute = minute, second = second)
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
  if (!is_valid_year_month_day_precision(precision)) {
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
    int_assert(fields[[i]], fields_names[[i]])
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
  proxy_rcrd(x)
}

#' @export
vec_restore.clock_year_month_day <- function(x, to, ...) {
  fields <- restore_rcrd_fields(x)
  names <- restore_rcrd_names(x)
  precision <- calendar_precision(to)
  new_year_month_day_from_fields(fields, precision, names)
}

#' @export
vec_proxy_equal.clock_year_month_day <- function(x, ...) {
  proxy_equal_rcrd(x)
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

is_valid_year_month_day_precision <- function(precision) {
  is_valid_calendar_precision(precision, c("year", "month"))
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

set_field_year_month_day <- function(x, value, precision_value) {
  if (is_last(value) && identical(precision_value, "day")) {
    return(set_field_year_month_day_last(x))
  }

  precision_fields <- calendar_precision(x)
  precision_out <- precision_common2(precision_fields, precision_value)

  value <- vec_cast(value, integer(), x_arg = "value")
  args <- vec_recycle_common(x = x, value = value)
  x <- args$x
  value <- args$value

  result <- set_field_year_month_day_cpp(x, value, precision_fields, precision_value)
  fields <- result$fields
  field <- precision_field(precision_value)
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
  calendar_require_all_valid(x)
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

field_year <- function(x) {
  field(x, "year")
}
field_month <- function(x) {
  field(x, "month")
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
