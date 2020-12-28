#' @export
invalid_detect <- function(x) {
  UseMethod("invalid_detect")
}

#' @export
invalid_detect.clock_calendar <- function(x) {
  abort("Can't detect invalid dates for this calendar type.")
}

# ------------------------------------------------------------------------------

#' @export
invalid_any <- function(x) {
  UseMethod("invalid_any")
}

#' @export
invalid_any.clock_calendar <- function(x) {
  abort("Can't any invalid dates for this calendar type.")
}

# ------------------------------------------------------------------------------

#' @export
invalid_count <- function(x) {
  UseMethod("invalid_count")
}

#' @export
invalid_count.clock_calendar <- function(x) {
  abort("Can't count invalid dates for this calendar type.")
}

# ------------------------------------------------------------------------------

#' @export
invalid_resolve <- function(x, invalid = "last-day") {
  UseMethod("invalid_resolve")
}

#' @export
invalid_resolve.clock_calendar <- function(x, invalid = "last-day") {
  abort("Can't resolve invalid dates for this calendar type.")
}
