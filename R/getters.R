get_zone <- function(x) {
  UseMethod("get_zone")
}

get_zone.Date <- function(x) {
  "UTC"
}

get_zone.POSIXct <- function(x) {
  tzone <- get_tzone(x)
  zone_standardize(tzone)
}

get_zone.POSIXlt <- function(x) {
  get_zone.POSIXct(x)
}

# ------------------------------------------------------------------------------
