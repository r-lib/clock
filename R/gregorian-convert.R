# ------------------------------------------------------------------------------

#' @export
as_year_month_weekday <- function(x)  {
  UseMethod("as_year_month_weekday")
}

#' @export
as_year_month_weekday.default <- function(x) {
  stop_clock_unsupported_conversion(x, "clock_year_month_weekday")
}

#' @export
as_year_month_weekday.clock_year_month_weekday <- function(x) {
  x
}

#' @export
as_year_month_weekday.clock_calendar <- function(x) {
  new_year_month_weekday(x)
}

#' @export
as_year_month_weekday.clock_naive_time_point <- function(x) {
  calendar <- field_calendar(x)
  names(calendar) <- names(x)
  as_year_month_weekday(calendar)
}

#' @export
as_year_month_weekday.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  as_year_month_weekday(x)
}

#' @export
as_year_month_weekday.Date <- function(x) {
  x <- date_to_days(x)
  new_year_month_weekday(x)
}

#' @export
as_year_month_weekday.POSIXt <- as_year_month_weekday.clock_zoned_time_point
