#' @export
in_zone <- function(x, zone, ...) {
  restrict_zoned_or_base(x)
  UseMethod("in_zone")
}

#' @export
in_zone.Date <- function(x, zone, ...) {
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
in_zone.POSIXt <- in_zone.Date

#' @export
in_zone.clock_zoned_time_point <- function(x, zone, ...) {
  check_dots_empty()

  zone <- zone_standardize(zone)

  if (!zone_is_valid(zone)) {
    abort(sprintf("'%s' not found in the timezone database.", zone))
  }

  x <- zoned_time_point_set_zone(x, zone)

  x
}
