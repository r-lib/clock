#' @export
as_quarterly_year_quarternum <- function(x, ...)  {
  UseMethod("as_quarterly_year_quarternum")
}

#' @export
as_quarterly_year_quarternum.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_naive_quarterly_year_quarternum")
}

#' @export
as_quarterly_year_quarternum.civil_naive_quarterly_year_quarternum <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_quarterly_year_quarternum.civil_naive_quarterly <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  start <- get_quarterly_start(x)
  days <- floor_days_to_year_quarternum_precision(days, start)
  new_quarterly_year_quarternum(days = days, start = start, names = names(x))
}

#' @export
as_quarterly_year_quarternum.civil_naive <- function(x, ..., start = 1L) {
  check_dots_empty()
  days <- field(x, "days")
  start <- cast_quarterly_start(start)
  days <- floor_days_to_year_quarternum_precision(days, start)
  new_quarterly_year_quarternum(days = days, start = start, names = names(x))
}

#' @export
as_quarterly_year_quarternum.civil_zoned <- function(x, ..., start = 1L) {
  x <- as_year_month_day(x)
  as_quarterly_year_quarternum(x, ..., start = start)
}

#' @export
as_quarterly_year_quarternum.Date <- as_quarterly_year_quarternum.civil_zoned

#' @export
as_quarterly_year_quarternum.POSIXt <- as_quarterly_year_quarternum.civil_zoned

# ------------------------------------------------------------------------------

#' @export
as_quarterly_year_quarternum_quarterday <- function(x, ...)  {
  UseMethod("as_quarterly_year_quarternum_quarterday")
}

#' @export
as_quarterly_year_quarternum_quarterday.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_naive_quarterly_year_quarternum_quarterday")
}

#' @export
as_quarterly_year_quarternum_quarterday.civil_naive_quarterly_year_quarternum_quarterday <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_quarterly_year_quarternum_quarterday.civil_naive_quarterly <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  start <- get_quarterly_start(x)
  new_quarterly_year_quarternum_quarterday(days = days, start = start, names = names(x))
}

#' @export
as_quarterly_year_quarternum_quarterday.civil_naive <- function(x, ..., start = 1L) {
  check_dots_empty()
  days <- field(x, "days")
  start <- cast_quarterly_start(start)
  new_quarterly_year_quarternum_quarterday(days = days, start = start, names = names(x))
}

#' @export
as_quarterly_year_quarternum_quarterday.civil_zoned <- function(x, ..., start = 1L) {
  x <- as_year_month_day(x)
  as_quarterly_year_quarternum_quarterday(x, ..., start = start)
}

#' @export
as_quarterly_year_quarternum_quarterday.Date <- as_quarterly_year_quarternum_quarterday.civil_zoned

#' @export
as_quarterly_year_quarternum_quarterday.POSIXt <- as_quarterly_year_quarternum_quarterday.civil_zoned
