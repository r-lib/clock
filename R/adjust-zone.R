#' @export
adjust_zone_retain_clock <- function(x,
                                     zone,
                                     ...,
                                     dst_nonexistent = "next",
                                     dst_ambiguous = "earliest") {
  check_dots_empty()
  validate_dst_nonexistent(dst_nonexistent)
  validate_dst_ambiguous(dst_ambiguous)
  x <- to_posixct(x)
  adjust_zone_retain_clock_cpp(x, zone, dst_nonexistent, dst_ambiguous)
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
