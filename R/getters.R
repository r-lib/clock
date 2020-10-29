#' @export
get_zone <- function(x) {
  UseMethod("get_zone")
}

#' @export
get_zone.Date <- function(x) {
  "UTC"
}

#' @export
get_zone.POSIXct <- function(x) {
  tzone <- get_tzone(x)
  zone_standardize(tzone)
}

#' @export
get_zone.POSIXlt <- function(x) {
  get_zone.POSIXct(x)
}

# ------------------------------------------------------------------------------
