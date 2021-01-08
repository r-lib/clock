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
  get_fn(x, value)
}

#' @export
get_zone.Date <- function(x) {
  "UTC"
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
