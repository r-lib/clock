#' @export
as_year_quarternum <- function(x, ...)  {
  UseMethod("as_year_quarternum")
}

#' @export
as_year_quarternum.default <- function(x, ...) {
  stop_clock_unsupported_conversion(x, "clock_year_quarternum")
}

#' @export
as_year_quarternum.clock_year_quarternum <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_year_quarternum.clock_quarterly <- function(x, ...) {
  check_dots_empty()
  start <- get_quarterly_start(x)
  x <- floor_calendar_days_to_year_quarternum_precision(x, start)
  new_year_quarternum(x, start = start)
}

#' @export
as_year_quarternum.clock_calendar <- function(x, ..., start = 1L) {
  check_dots_empty()
  start <- cast_quarterly_start(start)
  x <- floor_calendar_days_to_year_quarternum_precision(x, start)
  new_year_quarternum(x, start = start)
}

#' @export
as_year_quarternum.clock_naive_time_point <- function(x, ..., start = 1L) {
  x <- as_year_quarternum_quarterday(x, ..., start = start)
  as_year_quarternum(x)
}

#' @export
as_year_quarternum.clock_zoned_time_point <- as_year_quarternum.clock_naive_time_point

#' @export
as_year_quarternum.Date <- as_year_quarternum.clock_naive_time_point

#' @export
as_year_quarternum.POSIXt <- as_year_quarternum.clock_naive_time_point

# ------------------------------------------------------------------------------

#' @export
as_year_quarternum_quarterday <- function(x, ...)  {
  UseMethod("as_year_quarternum_quarterday")
}

#' @export
as_year_quarternum_quarterday.default <- function(x, ...) {
  stop_clock_unsupported_conversion(x, "clock_year_quarternum_quarterday")
}

#' @export
as_year_quarternum_quarterday.clock_year_quarternum_quarterday <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_year_quarternum_quarterday.clock_quarterly <- function(x, ...) {
  check_dots_empty()
  new_year_quarternum_quarterday(x, start = get_quarterly_start(x))
}

#' @export
as_year_quarternum_quarterday.clock_calendar <- function(x, ..., start = 1L) {
  check_dots_empty()
  start <- cast_quarterly_start(start)
  new_year_quarternum_quarterday(x, start = start)
}

#' @export
as_year_quarternum_quarterday.clock_naive_time_point <- function(x, ..., start = 1L) {
  calendar <- field_calendar(x)
  names(calendar) <- names(x)
  as_year_quarternum_quarterday(calendar, ..., start = start)
}

#' @export
as_year_quarternum_quarterday.clock_zoned_time_point <- function(x, ..., start = 1L) {
  x <- as_naive_time_point(x)
  as_year_quarternum_quarterday(x, ..., start = start)
}

#' @export
as_year_quarternum_quarterday.Date <- function(x, ..., start = 1L) {
  x <- date_to_days(x)
  start <- cast_quarterly_start(start)
  new_year_quarternum_quarterday(x, start = start)
}

#' @export
as_year_quarternum_quarterday.POSIXt <- as_year_quarternum_quarterday.clock_zoned_time_point
