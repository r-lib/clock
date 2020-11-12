#' @export
adjust_zone_retain_clock <- function(x,
                                     zone,
                                     ...,
                                     dst_nonexistent = "roll-forward",
                                     dst_ambiguous = "earliest") {
  UseMethod("adjust_zone_retain_clock")
}

#' @export
adjust_zone_retain_clock.Date <- function(x,
                                          zone,
                                          ...,
                                          dst_nonexistent = "roll-forward",
                                          dst_ambiguous = "earliest") {
  x <- to_posixct(x)
  adjust_zone_retain_clock(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_zone_retain_clock.POSIXt <- function(x,
                                            zone,
                                            ...,
                                            dst_nonexistent = "roll-forward",
                                            dst_ambiguous = "earliest") {
  check_dots_empty()
  x <- to_posixct(x)
  adjust_zone_retain_clock_cpp(x, zone, dst_nonexistent, dst_ambiguous)
}

#' @export
adjust_zone_retain_clock.civil_zoned_nano_datetime <- function(x,
                                                               zone,
                                                               ...,
                                                               dst_nonexistent = "roll-forward",
                                                               dst_ambiguous = "earliest") {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  nanos_of_second <- field(x, "nanos_of_second")

  names <- names(x)

  x_zone <- get_zone(x)
  zone <- zone_standardize(zone)

  fields <- convert_datetime_fields_from_zoned_to_local(
    days = days,
    time_of_day = time_of_day,
    zone = x_zone
  )

  days <- fields$days
  time_of_day <- fields$time_of_day

  fields <- convert_nano_datetime_fields_from_local_to_zoned(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  new_zoned_nano_datetime_from_fields(fields, zone, names)
}

# ------------------------------------------------------------------------------

#' @export
adjust_zone_retain_instant <- function(x, zone, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_zone_retain_instant")
}

#' @export
adjust_zone_retain_instant.Date <- function(x, zone, ...) {
  x <- to_posixct(x)
  adjust_zone_retain_instant(x, zone, ...)
}

#' @export
adjust_zone_retain_instant.POSIXt <- function(x, zone, ...) {
  check_dots_empty()

  x <- to_posixct(x)
  zone <- zone_standardize(zone)

  if (!zone_is_valid(zone)) {
    abort(sprintf("'%s' not found in the timezone database.", zone))
  }

  attr(x, "tzone") <- zone

  x
}

#' @export
adjust_zone_retain_instant.civil_zoned <- function(x, zone, ...) {
  check_dots_empty()

  zone <- zone_standardize(zone)

  if (!zone_is_valid(zone)) {
    abort(sprintf("'%s' not found in the timezone database.", zone))
  }

  x <- zoned_set_zone(x, zone)

  x
}
