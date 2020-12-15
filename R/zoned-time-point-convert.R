# ------------------------------------------------------------------------------

#' @export
as_zoned_second_point <- function(x, ...) {
  UseMethod("as_zoned_second_point")
}

#' @export
as_zoned_second_point.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "clock_zoned_second_point")
}

#' @export
as_zoned_second_point.clock_zoned_second_point <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_zoned_second_point.clock_calendar <- function(x,
                                                 zone,
                                                 ...,
                                                 dst_nonexistent = "roll-forward",
                                                 dst_ambiguous = "earliest") {
  x <- as_naive_second_point(x)
  as_zoned_second_point(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned_second_point.clock_naive_time_point <- function(x,
                                                         zone,
                                                         ...,
                                                         dst_nonexistent = "roll-forward",
                                                         dst_ambiguous = "earliest") {
  check_dots_empty()

  x <- as_naive_second_point(x)

  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  zone <- zone_standardize(zone)

  fields <- convert_second_point_fields_from_naive_to_zoned(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  new_zoned_second_point_from_fields(fields, zone, names = names(x))
}

#' @export
as_zoned_second_point.clock_zoned_subsecond_point <- function(x, ...) {
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  zone <- zoned_time_point_zone(x)
  new_zoned_second_point(calendar, seconds_of_day, zone, names = names(x))
}

#' @export
as_zoned_second_point.Date <- function(x, ...) {
  x <- to_posixct(x)
  as_zoned_second_point(x)
}

#' @export
as_zoned_second_point.POSIXt <- function(x, ...) {
  x <- to_posixct(x)

  names <- names(x)
  seconds <- unstructure(x)
  zone <- get_zone(x)

  fields <- convert_zoned_seconds_to_zoned_second_point_fields(seconds)

  days <- fields$days
  seconds_of_day <- fields$seconds_of_day

  calendar <- new_year_month_day(days)

  new_zoned_second_point(calendar, seconds_of_day, zone, names = names)
}

# ------------------------------------------------------------------------------

#' @export
as_zoned_subsecond_point <- function(x, ...) {
  UseMethod("as_zoned_subsecond_point")
}

#' @export
as_zoned_subsecond_point.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "clock_zoned_subsecond_point")
}

#' @export
as_zoned_subsecond_point.clock_zoned_subsecond_point <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_zoned_subsecond_point.clock_calendar <- function(x,
                                                    zone,
                                                    ...,
                                                    dst_nonexistent = "roll-forward",
                                                    dst_ambiguous = "earliest",
                                                    precision = "nanosecond") {
  x <- as_zoned_second_point(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
  as_zoned_subsecond_point(x, precision = precision)
}

#' @export
as_zoned_subsecond_point.clock_naive_second_point <- as_zoned_subsecond_point.clock_calendar

#' @export
as_zoned_subsecond_point.clock_naive_subsecond_point <- function(x,
                                                                 zone,
                                                                 ...,
                                                                 dst_nonexistent = "roll-forward",
                                                                 dst_ambiguous = "earliest") {
  check_dots_empty()

  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  nanoseconds_of_second <- field_nanoseconds_of_second(x)

  precision <- get_precision(x)
  zone <- zone_standardize(zone)

  fields <- convert_subsecond_point_fields_from_naive_to_zoned(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  new_zoned_subsecond_point_from_fields(fields, precision, zone, names = names(x))
}

#' @export
as_zoned_subsecond_point.clock_zoned_second_point <- function(x, ..., precision = "nanosecond") {
  check_dots_empty()
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  nanoseconds_of_second <- zeros_along(calendar)
  zone <- zoned_time_point_zone(x)
  new_zoned_subsecond_point(calendar, seconds_of_day, nanoseconds_of_second, precision, zone, names = names(x))
}

#' @export
as_zoned_subsecond_point.Date <- function(x, ..., precision = "nanosecond") {
  x <- as_zoned_second_point(x)
  as_zoned_subsecond_point(x, ..., precision = precision)
}

#' @export
as_zoned_subsecond_point.POSIXt <- as_zoned_subsecond_point.Date

# ------------------------------------------------------------------------------

#' @export
as_zoned_date_time <- function(x, ...)  {
  UseMethod("as_zoned_date_time")
}

#' @export
as_zoned_date_time.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "clock_zoned_date_time")
}

#' @export
as_zoned_date_time.clock_zoned_date_time <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_zoned_date_time.clock_calendar <- function(x,
                                              zone,
                                              ...,
                                              dst_nonexistent = "roll-forward",
                                              dst_ambiguous = "earliest") {
  x <- as_zoned_second_point(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
  as_zoned_date_time(x)
}

#' @export
as_zoned_date_time.clock_naive_time_point <- as_zoned_date_time.clock_calendar

#' @export
as_zoned_date_time.clock_zoned_time_point <- function(x, ...) {
  x <- as_zoned_second_point(x, ...)
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  calendar <- as_year_month_day(calendar)
  zone <- zoned_time_point_zone(x)
  new_zoned_date_time(calendar, seconds_of_day, zone, names = names(x))
}

#' @export
as_zoned_date_time.Date <- function(x, ...) {
  x <- as_zoned_second_point(x, ...)
  as_zoned_date_time(x)
}

#' @export
as_zoned_date_time.POSIXt <- as_zoned_date_time.Date

# ------------------------------------------------------------------------------

#' @export
as_zoned_date_nanotime <- function(x, ...)  {
  UseMethod("as_zoned_date_nanotime")
}

#' @export
as_zoned_date_nanotime.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "clock_zoned_date_nanotime")
}

#' @export
as_zoned_date_nanotime.clock_zoned_date_nanotime <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_zoned_date_nanotime.clock_calendar <- function(x,
                                                  zone,
                                                  ...,
                                                  dst_nonexistent = "roll-forward",
                                                  dst_ambiguous = "earliest") {
  x <- as_zoned_subsecond_point(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous, precision = "nanosecond")
  as_zoned_date_nanotime(x)
}

#' @export
as_zoned_date_nanotime.clock_naive_second_point <- as_zoned_date_nanotime.clock_calendar

#' @export
as_zoned_date_nanotime.clock_naive_subsecond_point <- function(x,
                                                               zone,
                                                               ...,
                                                               dst_nonexistent = "roll-forward",
                                                               dst_ambiguous = "earliest") {
  x <- as_zoned_subsecond_point(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
  as_zoned_date_nanotime(x)
}

#' @export
as_zoned_date_nanotime.clock_zoned_second_point <- function(x, ...) {
  x <- as_zoned_subsecond_point(x, precision = "nanosecond")
  as_zoned_date_nanotime(x)
}

#' @export
as_zoned_date_nanotime.clock_zoned_subsecond_point <- function(x, ...) {
  check_dots_empty()
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  nanoseconds_of_second <- field_nanoseconds_of_second(x)
  calendar <- as_year_month_day(calendar)
  zone <- zoned_time_point_zone(x)
  new_zoned_date_nanotime(calendar, seconds_of_day, nanoseconds_of_second, zone, names = names(x))
}

#' @export
as_zoned_date_nanotime.Date <- function(x, ...) {
  x <- as_zoned_subsecond_point(x, ..., precision = "nanosecond")
  as_zoned_date_nanotime(x)
}

#' @export
as_zoned_date_nanotime.POSIXt <- as_zoned_date_nanotime.Date
