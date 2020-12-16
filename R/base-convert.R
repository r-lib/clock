# ------------------------------------------------------------------------------

# Not using `check_dots_empty()` because that might
# be too aggressive with base generics

#' @export
as.Date.clock_calendar <- function(x, ...) {
  days <- unstructure(x)
  names <- names(x)
  days_to_date(days, names)
}

#' @export
as.Date.clock_time_point <- function(x, ...) {
  # Retains the instant for zoned time points, like as.Date(<POSIXct>)
  calendar <- field_calendar(x)
  as.Date(calendar, ...)
}

# ------------------------------------------------------------------------------

# Not using `check_dots_empty()` because that might
# be too aggressive with base generics

#' @export
as.POSIXct.clock_calendar <- function(x,
                                      tz = "",
                                      ...,
                                      dst_nonexistent = "roll-forward",
                                      dst_ambiguous = "earliest") {
  x <- as_naive_time_point(x)
  as.POSIXct(x, tz = tz, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as.POSIXct.clock_naive_time_point <- function(x,
                                              tz = "",
                                              ...,
                                              dst_nonexistent = "roll-forward",
                                              dst_ambiguous = "earliest") {
  # Using `tz = ""` to be compatible with the generic of `as.POSIXct()`
  zone <- zone_standardize(tz)

  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)

  seconds <- convert_naive_second_point_fields_to_zoned_seconds(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  names(seconds) <- names(x)

  new_datetime(seconds, zone)
}

#' @export
as.POSIXct.clock_zoned_time_point <- function(x, ...) {
  # Keeps zone of `x`.
  # Should use `adjust_zone()` first if that is required.
  zone <- zoned_time_point_zone(x)

  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)

  days <- unstructure(calendar)

  seconds <- days * seconds_in_day() + seconds_of_day

  names(seconds) <- names(x)

  new_datetime(seconds, zone)
}

# ------------------------------------------------------------------------------

#' @export
as.POSIXlt.clock_calendar <- function(x,
                                      tz = "",
                                      ...,
                                      dst_nonexistent = "roll-forward",
                                      dst_ambiguous = "earliest") {
  x <- as.POSIXct(x, tz = tz, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
  as.POSIXlt(x)
}

#' @export
as.POSIXlt.clock_naive_time_point <- as.POSIXlt.clock_calendar

#' @export
as.POSIXlt.civil_zoned_time_point <- function(x, ...) {
  x <- as.POSIXct(x, ...)
  as.POSIXlt(x)
}
