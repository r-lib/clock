# Conversion to zoned
# ------------------------------------------------------------------------------

#' @export
as_zoned <- function(x, ...) {
  UseMethod("as_zoned")
}

#' @export
as_zoned.clock_zoned_time_point <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_zoned.clock_calendar <- function(x,
                                    zone,
                                    ...,
                                    dst_nonexistent = "roll-forward",
                                    dst_ambiguous = "earliest") {
  as_zoned_second_point(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned.clock_naive_second_point <- as_zoned.clock_calendar

#' @export
as_zoned.clock_naive_subsecond_point <- function(x,
                                                 zone,
                                                 ...,
                                                 dst_nonexistent = "roll-forward",
                                                 dst_ambiguous = "earliest") {
  as_zoned_subsecond_point(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned.clock_naive_date_time <- function(x,
                                           zone,
                                           ...,
                                           dst_nonexistent = "roll-forward",
                                           dst_ambiguous = "earliest") {
  as_zoned_date_time(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned.clock_naive_date_nanotime <- function(x,
                                               zone,
                                               ...,
                                               dst_nonexistent = "roll-forward",
                                               dst_ambiguous = "earliest") {
  as_zoned_date_nanotime(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
as_zoned.Date <- function(x, ...) {
  check_dots_empty()
  as_zoned_datetime(x)
}

#' @export
as_zoned.POSIXt <- function(x, ...) {
  check_dots_empty()
  as_zoned_datetime(x)
}
