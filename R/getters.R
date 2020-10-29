#' @export
get_zone <- function(x) {
  x <- to_posixct(x)
  tzone <- get_tzone(x)
  zone_standardize(tzone)
}
