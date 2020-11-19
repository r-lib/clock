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

# ------------------------------------------------------------------------------

#' @export
get_offset <- function(x) {
  restrict_zoned_or_base(x)
  UseMethod("get_offset")
}

#' @export
get_offset.Date <- function(x) {
  zeros_along(x)
}

#' @export
get_offset.POSIXt <- function(x) {
  x <- as_zoned_datetime(x)
  get_offset(x)
}

#' @export
get_offset.civil_zoned <- function(x) {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  zone <- zoned_zone(x)

  get_offset_cpp(days, time_of_day, zone)
}
