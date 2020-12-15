# ------------------------------------------------------------------------------

#' @export
as_naive_second_point <- function(x) {
  UseMethod("as_naive_second_point")
}

#' @export
as_naive_second_point.default <- function(x) {
  stop_clock_unsupported_conversion(x, "clock_naive_second_point")
}

#' @export
as_naive_second_point.clock_naive_second_point <- function(x) {
  x
}

#' @export
as_naive_second_point.clock_calendar <- function(x) {
  x <- promote_precision_day(x)
  names <- names(x)
  x <- unname(x)
  seconds_of_day <- zeros_along(x)
  new_naive_second_point(x, seconds_of_day, names = names)
}

#' @export
as_naive_second_point.clock_naive_subsecond_point <- function(x) {
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  new_naive_second_point(calendar, seconds_of_day, names = names(x))
}

#' @export
as_naive_second_point.clock_zoned_second_point <- function(x) {
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  zone <- zoned_time_point_zone(x)

  fields <- convert_second_point_fields_from_zoned_to_naive(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    zone = zone
  )

  new_naive_second_point_from_fields(fields, names = names(x))
}

#' @export
as_naive_second_point.clock_zoned_subsecond_point <- as_naive_second_point.clock_zoned_second_point

#' @export
as_naive_second_point.Date <- function(x) {
  x <- to_posixct(x)
  as_naive_second_point(x)
}

#' @export
as_naive_second_point.POSIXt <- function(x) {
  x <- to_posixct(x)

  names <- names(x)
  seconds <- unstructure(x)
  zone <- get_zone(x)

  fields <- convert_zoned_seconds_to_naive_second_point_fields(seconds, zone)

  days <- fields$days
  seconds_of_day <- fields$seconds_of_day

  calendar <- new_year_month_day(days)

  new_naive_second_point(calendar, seconds_of_day, names = names)
}

# ------------------------------------------------------------------------------

#' @export
as_naive_subsecond_point <- function(x, ...) {
  UseMethod("as_naive_subsecond_point")
}

#' @export
as_naive_subsecond_point.default <- function(x, ...) {
  stop_clock_unsupported_conversion(x, "civil_naive_subsecond_point")
}

#' @export
as_naive_subsecond_point.clock_naive_subsecond_point <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_naive_subsecond_point.clock_calendar <- function(x, ..., precision = "nanosecond") {
  x <- as_naive_second_point(x)
  as_naive_subsecond_point(x, ..., precision = precision)
}

#' @export
as_naive_subsecond_point.clock_naive_second_point <- function(x, ..., precision = "nanosecond") {
  check_dots_empty()
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  nanoseconds_of_second <- zeros_along(calendar)
  new_naive_subsecond_point(calendar, seconds_of_day, nanoseconds_of_second, precision, names = names(x))
}

#' @export
as_naive_subsecond_point.clock_zoned_second_point <- function(x, ..., precision = "nanosecond") {
  x <- as_naive_second_point(x)
  as_naive_subsecond_point(x, ..., precision = precision)
}

#' @export
as_naive_subsecond_point.clock_zoned_subsecond_point <- function(x, ...) {
  check_dots_empty()
  precision <- get_precision(x)
  nanoseconds_of_second <- field_nanoseconds_of_second(x)
  x <- as_naive_second_point(x)
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  new_naive_subsecond_point(calendar, seconds_of_day, nanoseconds_of_second, precision, names = names(x))
}

#' @export
as_naive_subsecond_point.Date <- as_naive_subsecond_point.clock_zoned_second_point

#' @export
as_naive_subsecond_point.POSIXt <- as_naive_subsecond_point.clock_zoned_second_point
