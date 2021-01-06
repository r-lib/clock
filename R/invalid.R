#' @export
invalid_detect <- function(x) {
  UseMethod("invalid_detect")
}

#' @export
invalid_detect.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("invalid_detect")
}

# ------------------------------------------------------------------------------

#' @export
invalid_any <- function(x) {
  UseMethod("invalid_any")
}

#' @export
invalid_any.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("invalid_any")
}

# ------------------------------------------------------------------------------

#' @export
invalid_count <- function(x) {
  UseMethod("invalid_count")
}

#' @export
invalid_count.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("invalid_count")
}

# ------------------------------------------------------------------------------

#' @export
invalid_resolve <- function(x, invalid = "last-day") {
  UseMethod("invalid_resolve")
}

#' @export
invalid_resolve.clock_calendar <- function(x, invalid = "last-day") {
  stop_clock_unsupported_calendar_op("invalid_resolve")
}
