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
get_zone.Date <- function(x) {
  "UTC"
}

# ------------------------------------------------------------------------------

#' @export
in_zone.Date <- function(x, zone) {
  in_zone(to_posixct(x), zone)
}
