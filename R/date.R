#' @export
as_sys_time.Date <- function(x) {
  names <- names(x)
  x <- unstructure(x)
  x <- duration_days(x)
  new_sys_time(x, names = names)
}

#' @export
as_naive_time.Date <- function(x) {
  as_naive_time(as_sys_time(x))
}

#' @export
as_zoned_time.Date <- function(x, ...) {
  check_dots_empty()

  x <- as_sys_time(x)
  x <- time_point_cast(x, "second")

  names <- names(x)
  x <- unname(x)

  new_zoned_time(x, zone = "UTC", names = names)
}

#' @export
as_year_month_day.Date <- function(x) {
  as_year_month_day(as_sys_time(x))
}

#' @export
as_year_month_weekday.Date <- function(x) {
  as_year_month_weekday(as_sys_time(x))
}

# ------------------------------------------------------------------------------

# Not using `check_dots_empty()` because that might
# be too aggressive with base generics

#' @export
as.Date.clock_calendar <- function(x, ...) {
  as.Date(as_sys_time(x))
}

#' @export
as.Date.clock_time_point <- function(x, ...) {
  names <- names(x)
  x <- time_point_cast(x, "day")
  x <- time_point_duration(x)
  x <- field_ticks(x)
  x <- as.double(x)
  names(x) <- names
  new_date(x)
}

#' @export
as.Date.clock_zoned_time <- function(x, ...) {
  as.Date(as_sys_time(x))
}

# ------------------------------------------------------------------------------

#' @export
get_year.Date <- function(x) {
  get_date_field_year_month_day(x, get_year)
}
#' @export
get_month.Date <- function(x) {
  get_date_field_year_month_day(x, get_month)
}
#' @export
get_day.Date <- function(x) {
  get_date_field_year_month_day(x, get_day)
}
get_date_field_year_month_day <- function(x, get_fn) {
  x <- as_year_month_day(x)
  get_fn(x)
}

#' @export
get_weekday.Date <- function(x) {
  get_date_field_year_month_weekday(x, get_weekday)
}
#' @export
get_weekday_index.Date <- function(x) {
  get_date_field_year_month_weekday(x, get_weekday_index)
}
get_date_field_year_month_weekday <- function(x, get_fn) {
  x <- as_year_month_weekday(x)
  get_fn(x)
}

#' @export
get_zone.Date <- function(x) {
  "UTC"
}

#' @export
get_offset.Date <- function(x) {
  zeros_along(x, na_propagate = TRUE)
}

# ------------------------------------------------------------------------------

#' @export
set_year.Date <- function(x, value, ..., invalid = "error") {
  set_date_field_year_month_day(x, value, invalid, set_year, ...)
}
#' @export
set_month.Date <- function(x, value, ..., invalid = "error") {
  set_date_field_year_month_day(x, value, invalid, set_month, ...)
}
#' @export
set_day.Date <- function(x, value, ..., invalid = "error") {
  set_date_field_year_month_day(x, value, invalid, set_day, ...)
}
set_date_field_year_month_day <- function(x, value, invalid, set_fn, ...) {
  check_dots_empty()
  x <- as_year_month_day(x)
  x <- set_fn(x, value)
  x <- invalid_resolve(x, invalid = invalid)
  as.Date(x)
}

#' @export
set_zone.Date <- function(x, zone) {
  abort("'Date' objects are required to be UTC.")
}

# ------------------------------------------------------------------------------

#' @export
add_years.Date <- function(x, n, ..., invalid = "error") {
  add_date_duration_year_month_day(x, n, invalid, add_years, ...)
}
#' @export
add_quarters.Date <- function(x, n, ..., invalid = "error") {
  add_date_duration_year_month_day(x, n, invalid, add_quarters, ...)
}
#' @export
add_months.Date <- function(x, n, ..., invalid = "error") {
  add_date_duration_year_month_day(x, n, invalid, add_months, ...)
}
add_date_duration_year_month_day <- function(x, n, invalid, add_fn, ...) {
  check_dots_empty()
  x <- as_year_month_day(x)
  x <- add_fn(x, n)
  x <- invalid_resolve(x, invalid = invalid)
  as.Date(x)
}

#' @export
add_weeks.Date <- function(x, n, ...) {
  add_date_duration_time_point(x, n, add_weeks, ...)
}
#' @export
add_days.Date <- function(x, n, ...) {
  add_date_duration_time_point(x, n, add_days, ...)
}
add_date_duration_time_point <- function(x, n, add_fn, ...) {
  check_dots_empty()
  x <- as_sys_time(x)
  x <- add_fn(x, n)
  as.Date(x)
}
