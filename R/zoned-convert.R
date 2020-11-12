#' @export
as_zoned_nano_datetime <- function(x, ...)  {
  UseMethod("as_zoned_nano_datetime")
}

#' @export
as_zoned_nano_datetime.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_zoned_nano_datetime")
}

#' @export
as_zoned_nano_datetime.civil_local_year_month <- function(x,
                                                          ...,
                                                          zone = "UTC",
                                                          dst_nonexistent = "roll-forward",
                                                          dst_ambiguous = "earliest") {
  x <- as_local_nano_datetime(x)
  as_zoned_nano_datetime(x, ..., zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned_nano_datetime.civil_local_date <- as_zoned_nano_datetime.civil_local_year_month

#' @export
as_zoned_nano_datetime.civil_local_datetime <- as_zoned_nano_datetime.civil_local_year_month

#' @export
as_zoned_nano_datetime.civil_local_nano_datetime <- function(x,
                                                             ...,
                                                             zone = "UTC",
                                                             dst_nonexistent = "roll-forward",
                                                             dst_ambiguous = "earliest") {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  nanos_of_second <- field(x, "nanos_of_second")

  zone <- zone_standardize(zone)

  fields <- convert_nano_datetime_fields_from_local_to_zoned(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  names <- names(x)

  new_zoned_nano_datetime_from_fields(fields, zone, names)
}

#' @export
as_zoned_nano_datetime.Date <- function(x, ...) {
  check_dots_empty()
  x <- to_posixct(x)
  as_zoned_nano_datetime(x)
}

#' @export
as_zoned_nano_datetime.POSIXt <- function(x, ...) {
  check_dots_empty()

  zone <- get_zone(x)
  names <- names(x)

  x <- to_posixct(x)
  seconds <- unstructure(x)

  fields <- convert_sys_seconds_to_sys_days_and_time_of_day(seconds)
  days <- fields$days
  time_of_day <- fields$time_of_day

  nanos_of_second <- zeros_along(x)

  new_zoned_nano_datetime(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    names = names
  )
}

#' @export
as_zoned_nano_datetime.civil_zoned_nano_datetime <- function(x) {
  x
}

# ------------------------------------------------------------------------------
