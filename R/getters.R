#' @export
get_zone <- function(x) {
  if (is_Date(x)) {
    "UTC"
  } else if (is_POSIXct(x) || is_POSIXlt(x)) {
    zone_standardize(get_tzone(x))
  } else if (is_zoned(x)) {
    zoned_zone(x)
  } else {
    stop_civil_unsupported_class(x)
  }
}
