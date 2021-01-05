#' @export
year_month <- function(year, month = 1L) {
  ymd <- year_month_day(year, month)
  new_year_month_day(field_year(ymd), field_month(ymd), precision = "month")
}

#' @export
year_month_day <- function(year, month = 1L, day = 1L) {
  if (is_last(day)) {
    day <- -1L
    last <- TRUE
  } else {
    last <- FALSE
  }

  fields <- list(year = year, month = month, day = day)
  size <- vec_size_common(!!!fields)
  fields <- vec_recycle_common(!!!fields, .size = size)
  fields <- vec_cast_common(!!!fields, .to = integer())

  fields <- collect_year_month_day_fields(
    fields,
    last
  )

  new_year_month_day_from_fields(fields, precision = "day")
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

  mapply(int_assert, fields, names(fields))

  new_gregorian(
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
invalid_resolve.clock_year_month_day <- function(x, invalid = "error") {
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
  set_field_year_month_day(x, value, "year")
}

#' @export
set_month.clock_year_month_day <- function(x, value, ...) {
  calendar_require_minimum_precision(x, "year", "set_month")
  set_field_year_month_day(x, value, "month")
}

#' @export
set_day.clock_year_month_day <- function(x, value, ...) {
  calendar_require_minimum_precision(x, "month", "set_day")
  set_field_year_month_day(x, value, "day")
}

#' @export
set_hour.clock_year_month_day <- function(x, value, ...) {
  calendar_require_minimum_precision(x, "day", "set_hour")
  set_field_year_month_day(x, value, "hour")
}

#' @export
set_minute.clock_year_month_day <- function(x, value, ...) {
  calendar_require_minimum_precision(x, "hour", "set_minute")
  set_field_year_month_day(x, value, "minute")
}

#' @export
set_second.clock_year_month_day <- function(x, value, ...) {
  calendar_require_minimum_precision(x, "minute", "set_second")
  set_field_year_month_day(x, value, "second")
}

#' @export
set_millisecond.clock_year_month_day <- function(x, value, ...) {
  calendar_require_any_of_precisions(x, c("second", "millisecond"), "set_millisecond")
  set_field_year_month_day(x, value, "millisecond")
}

#' @export
set_microsecond.clock_year_month_day <- function(x, value, ...) {
  calendar_require_any_of_precisions(x, c("second", "microsecond"), "set_microsecond")
  set_field_year_month_day(x, value, "microsecond")
}

#' @export
set_nanosecond.clock_year_month_day <- function(x, value, ...) {
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

#' @export
add_years.clock_year_month_day <- function(x, n, ...) {
  add_field_year_month_day(x, n, "year")
}

#' @export
add_months.clock_year_month_day <- function(x, n, ...) {
  calendar_require_minimum_precision(x, "month", "add_months")
  add_field_year_month_day(x, n, "month")
}

add_field_year_month_day <- function(x, n, precision_n) {
  precision_fields <- calendar_precision(x)

  n <- vec_cast(n, integer(), x_arg = "n")
  args <- vec_recycle_common(x = x, n = n)
  x <- args$x
  n <- args$n

  fields <- add_field_year_month_day_cpp(x, n, precision_fields, precision_n)

  new_year_month_day_from_fields(fields, precision_fields, names = names(x))
}
