#' @export
as_iso_year_weeknum <- function(x, ...)  {
  UseMethod("as_iso_year_weeknum")
}

#' @export
as_iso_year_weeknum.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_naive_iso_year_weeknum")
}

#' @export
as_iso_year_weeknum.civil_naive_iso_year_weeknum <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_iso_year_weeknum.civil_naive <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  days <- floor_days_to_iso_year_weeknum_precision(days)
  new_iso_year_weeknum(days = days, names = names(x))
}

#' @export
as_iso_year_weeknum.civil_zoned <- function(x, ...) {
  x <- as_year_month_day(x)
  as_iso_year_weeknum(x, ...)
}

#' @export
as_iso_year_weeknum.Date <- as_iso_year_weeknum.civil_zoned

#' @export
as_iso_year_weeknum.POSIXt <- as_iso_year_weeknum.civil_zoned

# ------------------------------------------------------------------------------

#' @export
as_iso_year_weeknum_weekday <- function(x, ...)  {
  UseMethod("as_iso_year_weeknum_weekday")
}

#' @export
as_iso_year_weeknum_weekday.default <- function(x, ...) {
  stop_civil_unsupported_conversion(x, "civil_naive_iso_year_weeknum_weekday")
}

#' @export
as_iso_year_weeknum_weekday.civil_naive_iso_year_weeknum_weekday <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_iso_year_weeknum_weekday.civil_naive <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  new_iso_year_weeknum_weekday(days = days, names = names(x))
}

#' @export
as_iso_year_weeknum_weekday.civil_zoned <- function(x, ...) {
  x <- as_year_month_day(x)
  as_iso_year_weeknum_weekday(x, ...)
}

#' @export
as_iso_year_weeknum_weekday.Date <- as_iso_year_weeknum_weekday.civil_zoned

#' @export
as_iso_year_weeknum_weekday.POSIXt <- as_iso_year_weeknum_weekday.civil_zoned
