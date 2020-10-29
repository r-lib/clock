#' @export
civil_in_zone <- function(x, zone) {
  if (!zone_is_valid(zone)) {
    abort("`zone` is not a recognized time zone name.")
  }
  x <- to_posixct(x)
  attr(x, "tzone") <- zone
  x
}

#' @export
civil_force_zone <- function(x,
                             zone,
                             ...,
                             dst_nonexistant = "next",
                             dst_ambiguous = "earliest") {
  check_dots_empty()
  x <- to_posixct(x)
  civil_force_zone_cpp(x, zone, dst_nonexistant, dst_ambiguous)
}
