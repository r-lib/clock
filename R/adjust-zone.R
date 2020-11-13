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
  x <- as_local(x)
  as.POSIXct(x, tz = zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_zone_retain_clock.POSIXt <- adjust_zone_retain_clock.Date

#' @export
adjust_zone_retain_clock.civil_zoned <- function(x,
                                                 zone,
                                                 ...,
                                                 dst_nonexistent = "roll-forward",
                                                 dst_ambiguous = "earliest") {
  x <- as_local(x)
  as_zoned(x, zone = zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

# ------------------------------------------------------------------------------

#' @export
adjust_zone_retain_instant <- function(x, zone, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_zone_retain_instant")
}

#' @export
adjust_zone_retain_instant.Date <- function(x, zone, ...) {
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
adjust_zone_retain_instant.POSIXt <- adjust_zone_retain_instant.Date

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
