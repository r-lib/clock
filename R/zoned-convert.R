# Conversion to zoned
# ------------------------------------------------------------------------------

#' @export
as_zoned <- function(x, ...) {
  restrict_civil_supported(x)
  UseMethod("as_zoned")
}

#' @export
as_zoned.civil_local <- function(x,
                                 zone,
                                 ...,
                                 dst_nonexistent = "roll-forward",
                                 dst_ambiguous = "earliest") {
  as_zoned_datetime(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned.civil_local_nano_datetime <- function(x,
                                               zone,
                                               ...,
                                               dst_nonexistent = "roll-forward",
                                               dst_ambiguous = "earliest") {
  as_zoned_nano_datetime(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned.Date <- function(x, ...) {
  check_dots_empty()
  as_zoned_datetime(x)
}

#' @export
as_zoned.POSIXt <- function(x, ...) {
  check_dots_empty()
  as_zoned_datetime(x)
}

#' @export
as_zoned.civil_zoned <- function(x, ...) {
  check_dots_empty()
  x
}

# ------------------------------------------------------------------------------

# Not using `check_dots_empty()` because that might
# be too aggressive with base generics

#' @export
as.Date.civil_local <- function(x, ...) {
  days <- field(x, "days")
  days_to_date(days, names(x))
}

#' @export
as.Date.civil_zoned <- function(x, ...) {
  # Retain instant, like `as.Date(<POSIXct>)`. This is like `in_zone(x, "UTC")`
  days <- field(x, "days")
  days_to_date(days, names(x))
}

# ------------------------------------------------------------------------------

# Not using `check_dots_empty()` because that might
# be too aggressive with base generics

#' @export
as.POSIXct.civil_local <- function(x,
                                   tz = "",
                                   ...,
                                   dst_nonexistent = "roll-forward",
                                   dst_ambiguous = "earliest") {
  x <- promote_at_least_local_datetime(x)

  # Using `tz = ""` to be compatible with the generic of `as.POSIXct()`
  zone <- zone_standardize(tz)

  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")

  seconds <- convert_local_days_and_time_of_day_to_sys_seconds(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  names(seconds) <- names(x)

  new_datetime(seconds, zone)
}

#' @export
as.POSIXct.civil_zoned <- function(x, ...) {
  # Keeps zone of `civil_zoned`.
  # Should use `adjust_zone_retain_*()` first if that is required.
  zone <- zoned_zone(x)

  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")

  seconds <- days * seconds_in_day() + time_of_day

  names(seconds) <- names(x)

  new_datetime(seconds, zone)
}

# ------------------------------------------------------------------------------

#' @export
as.POSIXlt.civil_local <- function(x,
                                   tz = "",
                                   ...,
                                   dst_nonexistent = "roll-forward",
                                   dst_ambiguous = "earliest") {
  x <- as.POSIXct(x, tz = tz, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
  as.POSIXlt(x)
}

#' @export
as.POSIXlt.civil_zoned <- function(x, ...) {
  x <- as.POSIXct(x, ...)
  as.POSIXlt(x)
}

# ------------------------------------------------------------------------------

#' @export
as_zoned_datetime <- function(x, ...)  {
  UseMethod("as_zoned_datetime")
}

#' @export
as_zoned_datetime.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_zoned_datetime")
}

#' @export
as_zoned_datetime.civil_local <- function(x,
                                          zone,
                                          ...,
                                          dst_nonexistent = "roll-forward",
                                          dst_ambiguous = "earliest") {
  x <- as_local_datetime(x)
  as_zoned_datetime(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned_datetime.civil_local_datetime <- function(x,
                                                   zone,
                                                   ...,
                                                   dst_nonexistent = "roll-forward",
                                                   dst_ambiguous = "earliest") {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")

  zone <- zone_standardize(zone)

  fields <- convert_datetime_fields_from_local_to_zoned(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  names <- names(x)

  new_zoned_datetime_from_fields(fields, zone, names)
}

#' @export
as_zoned_datetime.Date <- function(x, ...) {
  check_dots_empty()
  x <- to_posixct(x)
  as_zoned_datetime(x)
}

#' @export
as_zoned_datetime.POSIXt <- function(x, ...) {
  check_dots_empty()

  zone <- get_zone(x)
  names <- names(x)

  x <- to_posixct(x)
  seconds <- unstructure(x)

  fields <- convert_sys_seconds_to_sys_days_and_time_of_day(seconds)
  days <- fields$days
  time_of_day <- fields$time_of_day

  new_zoned_datetime(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    names = names
  )
}

#' @export
as_zoned_datetime.civil_zoned_datetime <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_zoned_datetime.civil_zoned_nano_datetime <- function(x, ...) {
  check_dots_empty()

  new_zoned_datetime(
    days = field(x, "days"),
    time_of_day = field(x, "time_of_day"),
    zone = zoned_zone(x),
    names = names(x)
  )
}

# ------------------------------------------------------------------------------

#' @export
as_zoned_nano_datetime <- function(x, ...)  {
  UseMethod("as_zoned_nano_datetime")
}

#' @export
as_zoned_nano_datetime.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_zoned_nano_datetime")
}

#' @export
as_zoned_nano_datetime.civil_local <- function(x,
                                               zone,
                                               ...,
                                               dst_nonexistent = "roll-forward",
                                               dst_ambiguous = "earliest") {
  x <- as_local_nano_datetime(x)
  as_zoned_nano_datetime(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned_nano_datetime.civil_local_nano_datetime <- function(x,
                                                             zone,
                                                             ...,
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
  x <- as_zoned_datetime(x, ...)
  as_zoned_nano_datetime(x)
}

#' @export
as_zoned_nano_datetime.POSIXt <- function(x, ...) {
  x <- as_zoned_datetime(x, ...)
  as_zoned_nano_datetime(x)
}

#' @export
as_zoned_nano_datetime.civil_zoned_datetime <- function(x, ...) {
  check_dots_empty()

  new_zoned_nano_datetime(
    days = field(x, "days"),
    time_of_day = field(x, "time_of_day"),
    nanos_of_second = zeros_along(x),
    zone = zoned_zone(x),
    names = names(x)
  )
}

#' @export
as_zoned_nano_datetime.civil_zoned_nano_datetime <- function(x, ...) {
  check_dots_empty()
  x
}
