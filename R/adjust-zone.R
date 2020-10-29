#' @export
adjust_zone_retain_clock <- function(x,
                                     zone,
                                     ...,
                                     dst_resolver = default_dst_resolver()) {
  UseMethod("adjust_zone_retain_clock")
}

#' @export
adjust_zone_retain_clock.Date <- function(x,
                                          zone,
                                          ...,
                                          dst_resolver = default_dst_resolver()) {
  x <- to_posixct(x)
  adjust_zone_retain_clock(x, zone, ..., dst_resolver = dst_resolver)
}

#' @export
adjust_zone_retain_clock.POSIXct <- function(x,
                                             zone,
                                             ...,
                                             dst_resolver = default_dst_resolver()) {
  check_dots_empty()
  validate_dst_resolver(dst_resolver)
  x <- posixct_standardize(x)
  adjust_zone_retain_clock_cpp(x, zone, dst_resolver)
}

#' @export
adjust_zone_retain_clock.POSIXlt <- function(x,
                                             zone,
                                             ...,
                                             dst_resolver = default_dst_resolver()) {
  x <- to_posixct(x)
  adjust_zone_retain_clock(x, zone, ..., dst_resolver = dst_resolver)
}

# ------------------------------------------------------------------------------

#' @export
adjust_zone_retain_instant <- function(x, zone, ...) {
  UseMethod("adjust_zone_retain_instant")
}

#' @export
adjust_zone_retain_instant.Date <- function(x, zone, ...) {
  x <- to_posixct(x)
  adjust_zone_retain_instant(x, zone, ...)
}

#' @export
adjust_zone_retain_instant.POSIXct <- function(x, zone, ...) {
  check_dots_empty()

  x <- posixct_standardize(x)
  zone <- zone_standardize(zone)

  if (!zone_is_valid(zone)) {
    abort(sprintf("'%s' not found in the timezone database.", zone))
  }

  attr(x, "tzone") <- zone

  x
}

#' @export
adjust_zone_retain_instant.POSIXlt <- function(x, zone, ...) {
  x <- to_posixct(x)
  adjust_zone_retain_instant(x, zone, ...)
}
