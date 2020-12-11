#' @export
as_fiscal_year_quarter <- function(x, ...)  {
  UseMethod("as_fiscal_year_quarter")
}

#' @export
as_fiscal_year_quarter.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_naive_fiscal_year_quarter")
}

#' @export
as_fiscal_year_quarter.civil_naive_fiscal_year_quarter <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_fiscal_year_quarter.civil_naive_fiscal <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  fiscal_start <- get_fiscal_start(x)
  days <- floor_days_to_year_quarter_precision(days, fiscal_start)
  new_fiscal_year_quarter(days = days, fiscal_start = fiscal_start, names = names(x))
}

#' @export
as_fiscal_year_quarter.civil_naive_gregorian <- function(x, ..., fiscal_start = 1L) {
  check_dots_empty()
  days <- field(x, "days")
  fiscal_start <- cast_fiscal_start(fiscal_start)
  days <- floor_days_to_year_quarter_precision(days, fiscal_start)
  new_fiscal_year_quarter(days = days, fiscal_start = fiscal_start, names = names(x))
}

#' @export
as_fiscal_year_quarter.civil_zoned <- function(x, ..., fiscal_start = 1L) {
  x <- as_year_month_day(x)
  as_fiscal_year_quarter(x, ..., fiscal_start = fiscal_start)
}

#' @export
as_fiscal_year_quarter.Date <- as_fiscal_year_quarter.civil_zoned

#' @export
as_fiscal_year_quarter.POSIXt <- as_fiscal_year_quarter.civil_zoned

# ------------------------------------------------------------------------------

#' @export
as_fiscal_year_quarter_day <- function(x, ...)  {
  UseMethod("as_fiscal_year_quarter_day")
}

#' @export
as_fiscal_year_quarter_day.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_naive_fiscal_year_quarter_day")
}

#' @export
as_fiscal_year_quarter_day.civil_naive_fiscal_year_quarter_day <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_fiscal_year_quarter_day.civil_naive_fiscal <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  fiscal_start <- get_fiscal_start(x)
  new_fiscal_year_quarter_day(days = days, fiscal_start = fiscal_start, names = names(x))
}

#' @export
as_fiscal_year_quarter_day.civil_naive_gregorian <- function(x, ..., fiscal_start = 1L) {
  check_dots_empty()
  days <- field(x, "days")
  fiscal_start <- cast_fiscal_start(fiscal_start)
  new_fiscal_year_quarter_day(days = days, fiscal_start = fiscal_start, names = names(x))
}

#' @export
as_fiscal_year_quarter_day.civil_zoned <- function(x, ..., fiscal_start = 1L) {
  x <- as_year_month_day(x)
  as_fiscal_year_quarter_day(x, ..., fiscal_start = fiscal_start)
}

#' @export
as_fiscal_year_quarter_day.Date <- as_fiscal_year_quarter_day.civil_zoned

#' @export
as_fiscal_year_quarter_day.POSIXt <- as_fiscal_year_quarter_day.civil_zoned
