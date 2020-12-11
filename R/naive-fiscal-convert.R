#' @export
as_fiscal_year_quarternum <- function(x, ...)  {
  UseMethod("as_fiscal_year_quarternum")
}

#' @export
as_fiscal_year_quarternum.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_naive_fiscal_year_quarternum")
}

#' @export
as_fiscal_year_quarternum.civil_naive_fiscal_year_quarternum <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_fiscal_year_quarternum.civil_naive_fiscal <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  fiscal_start <- get_fiscal_start(x)
  days <- floor_days_to_year_quarternum_precision(days, fiscal_start)
  new_fiscal_year_quarternum(days = days, fiscal_start = fiscal_start, names = names(x))
}

#' @export
as_fiscal_year_quarternum.civil_naive <- function(x, ..., fiscal_start = 1L) {
  check_dots_empty()
  days <- field(x, "days")
  fiscal_start <- cast_fiscal_start(fiscal_start)
  days <- floor_days_to_year_quarternum_precision(days, fiscal_start)
  new_fiscal_year_quarternum(days = days, fiscal_start = fiscal_start, names = names(x))
}

#' @export
as_fiscal_year_quarternum.civil_zoned <- function(x, ..., fiscal_start = 1L) {
  x <- as_year_month_day(x)
  as_fiscal_year_quarternum(x, ..., fiscal_start = fiscal_start)
}

#' @export
as_fiscal_year_quarternum.Date <- as_fiscal_year_quarternum.civil_zoned

#' @export
as_fiscal_year_quarternum.POSIXt <- as_fiscal_year_quarternum.civil_zoned

# ------------------------------------------------------------------------------

#' @export
as_fiscal_year_quarternum_quarterday <- function(x, ...)  {
  UseMethod("as_fiscal_year_quarternum_quarterday")
}

#' @export
as_fiscal_year_quarternum_quarterday.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_naive_fiscal_year_quarternum_quarterday")
}

#' @export
as_fiscal_year_quarternum_quarterday.civil_naive_fiscal_year_quarternum_quarterday <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_fiscal_year_quarternum_quarterday.civil_naive_fiscal <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  fiscal_start <- get_fiscal_start(x)
  new_fiscal_year_quarternum_quarterday(days = days, fiscal_start = fiscal_start, names = names(x))
}

#' @export
as_fiscal_year_quarternum_quarterday.civil_naive <- function(x, ..., fiscal_start = 1L) {
  check_dots_empty()
  days <- field(x, "days")
  fiscal_start <- cast_fiscal_start(fiscal_start)
  new_fiscal_year_quarternum_quarterday(days = days, fiscal_start = fiscal_start, names = names(x))
}

#' @export
as_fiscal_year_quarternum_quarterday.civil_zoned <- function(x, ..., fiscal_start = 1L) {
  x <- as_year_month_day(x)
  as_fiscal_year_quarternum_quarterday(x, ..., fiscal_start = fiscal_start)
}

#' @export
as_fiscal_year_quarternum_quarterday.Date <- as_fiscal_year_quarternum_quarterday.civil_zoned

#' @export
as_fiscal_year_quarternum_quarterday.POSIXt <- as_fiscal_year_quarternum_quarterday.civil_zoned
