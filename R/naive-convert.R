# Conversion to naive
# ------------------------------------------------------------------------------

#' @export
as_naive <- function(x) {
  UseMethod("as_naive")
}

#' @export
as_naive.clock_naive_time_point <- function(x) {
  x
}

#' @export
as_naive.clock_calendar <- function(x) {
  as_naive_second_point(x)
}

#' @export
as_naive.clock_zoned_second_point <- as_naive.clock_calendar

#' @export
as_naive.clock_zoned_subsecond_point <- function(x) {
  as_naive_subsecond_point(x)
}

#' @export
as_naive.Date <- as_naive.clock_calendar

#' @export
as_naive.POSIXt <- as_naive.clock_calendar
