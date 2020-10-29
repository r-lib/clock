#' @export
adjust_zone_retain_clock <- function(x,
                                     zone,
                                     ...,
                                     dst_resolver = default_dst_resolver()) {
  check_dots_empty()
  validate_dst_resolver(dst_resolver)
  x <- to_posixct(x)
  adjust_zone_retain_clock_cpp(x, zone, dst_resolver)
}

#' @export
adjust_zone_retain_instant <- function(x, zone, ...) {
  check_dots_empty()

  x <- to_posixct(x)
  zone <- zone_standardize(zone)

  if (!zone_is_valid(zone)) {
    abort(sprintf("'%s' not found in the timezone database.", zone))
  }

  attr(x, "tzone") <- zone

  x
}
