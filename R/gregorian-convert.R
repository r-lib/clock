#' @export
as_year_month <- function(x)  {
  UseMethod("as_year_month")
}

#' @export
as_year_month.default <- function(x) {
  stop_clock_unsupported_conversion(x, "clock_year_month")
}

#' @export
as_year_month.clock_year_month <- function(x) {
  x
}

#' @export
as_year_month.clock_calendar <- function(x) {
  x <- floor_calendar_days_to_year_month_precision(x)
  new_year_month(x)
}

#' @export
as_year_month.clock_naive_time_point <- function(x) {
  x <- as_year_month_day(x)
  as_year_month(x)
}

#' @export
as_year_month.clock_zoned_time_point <- as_year_month.clock_naive_time_point

#' @export
as_year_month.Date <- as_year_month.clock_naive_time_point

#' @export
as_year_month.POSIXt <- as_year_month.clock_naive_time_point

# ------------------------------------------------------------------------------

#' @export
as_year_month_day <- function(x)  {
  UseMethod("as_year_month_day")
}

#' @export
as_year_month_day.default <- function(x) {
  stop_clock_unsupported_conversion(x, "clock_year_month_day")
}

#' @export
as_year_month_day.clock_year_month_day <- function(x) {
  x
}

#' @export
as_year_month_day.clock_calendar <- function(x) {
  new_year_month_day(x)
}

#' @export
as_year_month_day.clock_naive_time_point <- function(x) {
  calendar <- field_calendar(x)
  names(calendar) <- names(x)
  as_year_month_day(calendar)
}

#' @export
as_year_month_day.clock_zoned_time_point <- function(x) {
  x <- as_naive_second_point(x)
  as_year_month_day(x)
}

#' @export
as_year_month_day.Date <- function(x) {
  x <- date_to_days(x)
  new_year_month_day(x)
}

#' @export
as_year_month_day.POSIXt <- as_year_month_day.clock_zoned_time_point
