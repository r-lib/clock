#' @export
as_zoned_time_point <- function(x, ...) {
  UseMethod("as_zoned_time_point")
}

#' @export
as_zoned_time_point.default <- function(x, ...) {
  stop_clock_unsupported_conversion(x, "clock_zoned_time_point")
}

#' @export
as_zoned_time_point.clock_zoned_time_point <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_zoned_time_point.clock_calendar <- function(x,
                                               zone,
                                               ...,
                                               dst_nonexistent = "roll-forward",
                                               dst_ambiguous = "earliest",
                                               precision = "second") {
  x <- as_naive_time_point(x, precision = precision)
  as_zoned_time_point(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned_time_point.clock_naive_time_point <- function(x,
                                                       zone,
                                                       ...,
                                                       dst_nonexistent = "roll-forward",
                                                       dst_ambiguous = "earliest") {
  check_dots_empty()

  precision <- get_precision(x)
  zone <- zone_standardize(zone)

  if (is_subsecond_precision(precision)) {
    # TODO: Should this take precision? To round nanoseconds_of_second correctly?
    fields <- convert_subsecond_point_fields_from_naive_to_zoned(
      calendar = field_calendar(x),
      seconds_of_day = field_seconds_of_day(x),
      nanoseconds_of_second = field_nanoseconds_of_second(x),
      zone = zone,
      dst_nonexistent = dst_nonexistent,
      dst_ambiguous = dst_ambiguous
    )
  } else {
    fields <- convert_second_point_fields_from_naive_to_zoned(
      calendar = field_calendar(x),
      seconds_of_day = field_seconds_of_day(x),
      zone = zone,
      dst_nonexistent = dst_nonexistent,
      dst_ambiguous = dst_ambiguous
    )
  }

  new_zoned_time_point_from_fields(fields, precision, zone, names = names(x))
}

#' @export
as_zoned_time_point.Date <- function(x, ..., precision = "second") {
  x <- as_year_month_day(x)
  as_zoned_time_point(x, ..., precision = precision)
}

#' @export
as_zoned_time_point.POSIXt <- function(x, ..., precision = "second") {
  check_dots_empty()

  x <- to_posixct(x)

  names <- names(x)
  seconds <- unstructure(x)
  zone <- get_zone(x)

  fields <- convert_zoned_seconds_to_zoned_second_point_fields(seconds)

  days <- fields$days
  seconds_of_day <- fields$seconds_of_day

  calendar <- new_year_month_day(days)

  validate_precision(precision)
  if (is_subsecond_precision(precision)) {
    nanoseconds_of_second <- nanoseconds_of_second_init(days)
  } else {
    nanoseconds_of_second <- NULL
  }

  new_zoned_time_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    precision = precision,
    zone = zone,
    names = names
  )
}
