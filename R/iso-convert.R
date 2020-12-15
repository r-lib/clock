#' @export
as_iso_year_weeknum <- function(x)  {
  UseMethod("as_iso_year_weeknum")
}

#' @export
as_iso_year_weeknum.default <- function(x) {
  stop_civil_unsupported_conversion(x, "clock_iso_year_weeknum")
}

#' @export
as_iso_year_weeknum.clock_iso_year_weeknum <- function(x) {
  x
}

#' @export
as_iso_year_weeknum.clock_calendar <- function(x) {
  x <- floor_calendar_days_to_iso_year_weeknum_precision(x)
  new_iso_year_weeknum(x)
}

#' @export
as_iso_year_weeknum.clock_naive_time_point <- function(x) {
  x <- as_iso_year_weeknum_weekday(x)
  as_iso_year_weeknum(x)
}

#' @export
as_iso_year_weeknum.clock_zoned_time_point <- as_iso_year_weeknum.clock_naive_time_point

#' @export
as_iso_year_weeknum.Date <- as_iso_year_weeknum.clock_naive_time_point

#' @export
as_iso_year_weeknum.POSIXt <- as_iso_year_weeknum.clock_naive_time_point

# ------------------------------------------------------------------------------

#' @export
as_iso_year_weeknum_weekday <- function(x)  {
  UseMethod("as_iso_year_weeknum_weekday")
}

#' @export
as_iso_year_weeknum_weekday.default <- function(x) {
  stop_civil_unsupported_conversion(x, "clock_iso_year_weeknum_weekday")
}

#' @export
as_iso_year_weeknum_weekday.clock_iso_year_weeknum_weekday <- function(x) {
  x
}

#' @export
as_iso_year_weeknum_weekday.clock_calendar <- function(x) {
  new_iso_year_weeknum_weekday(x)
}

#' @export
as_iso_year_weeknum_weekday.clock_naive_time_point <- function(x) {
  calendar <- field_calendar(x)
  names(calendar) <- names(x)
  as_iso_year_weeknum_weekday(calendar)
}

#' @export
as_iso_year_weeknum_weekday.clock_zoned_time_point <- function(x) {
  x <- as_naive_second_point(x)
  as_iso_year_weeknum_weekday(x)
}

#' @export
as_iso_year_weeknum_weekday.Date <- function(x) {
  x <- date_to_days(x)
  new_iso_year_weeknum_weekday(x)
}

#' @export
as_iso_year_weeknum_weekday.POSIXt <- as_iso_year_weeknum_weekday.clock_zoned_time_point
