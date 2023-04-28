#' Calendar getters
#'
#' @description
#' This family of functions extract fields from a calendar vector. Each
#' calendar has its own set of supported getters, which are documented on
#' their own help page:
#'
#' - [year-month-day][year-month-day-getters]
#'
#' - [year-month-weekday][year-month-weekday-getters]
#'
#' - [year-week-day][year-week-day-getters]
#'
#' - [iso-year-week-day][iso-year-week-day-getters]
#'
#' - [year-quarter-day][year-quarter-day-getters]
#'
#' - [year-day][year-day-getters]
#'
#' There are also convenience methods for extracting certain components
#' directly from R's native date and date-time types.
#'
#' - [dates (Date)][Date-getters]
#'
#' - [date-times (POSIXct / POSIXlt)][posixt-getters]
#'
#' @details
#' You cannot extract components directly from a time point type, such as
#' sys-time or naive-time. Convert it to a calendar type first. Similarly,
#' a zoned-time must be converted to either a sys-time or naive-time, and
#' then to a calendar type, to be able to extract components from it.
#'
#' @param x `[object]`
#'
#'   An object to get the component from.
#'
#' @return The component.
#'
#' @name clock-getters
#' @examples
#' x <- year_month_day(2019, 1:3, 5:7, 1, 20, 30)
#' get_month(x)
#' get_day(x)
#' get_second(x)
NULL

#' @rdname clock-getters
#' @export
get_year <- function(x) {
  UseMethod("get_year")
}

#' @rdname clock-getters
#' @export
get_quarter <- function(x) {
  UseMethod("get_quarter")
}

#' @rdname clock-getters
#' @export
get_month <- function(x) {
  UseMethod("get_month")
}

#' @rdname clock-getters
#' @export
get_week <- function(x) {
  UseMethod("get_week")
}

#' @rdname clock-getters
#' @export
get_day <- function(x) {
  UseMethod("get_day")
}

#' @rdname clock-getters
#' @export
get_hour <- function(x) {
  UseMethod("get_hour")
}

#' @rdname clock-getters
#' @export
get_minute <- function(x) {
  UseMethod("get_minute")
}

#' @rdname clock-getters
#' @export
get_second <- function(x) {
  UseMethod("get_second")
}

#' @rdname clock-getters
#' @export
get_millisecond <- function(x) {
  UseMethod("get_millisecond")
}

#' @rdname clock-getters
#' @export
get_microsecond <- function(x) {
  UseMethod("get_microsecond")
}

#' @rdname clock-getters
#' @export
get_nanosecond <- function(x) {
  UseMethod("get_nanosecond")
}

#' @rdname clock-getters
#' @export
get_index <- function(x) {
  UseMethod("get_index")
}

# ------------------------------------------------------------------------------

#' @export
get_year.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_quarter.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_month.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_week.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_day.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_hour.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_minute.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_second.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_millisecond.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_microsecond.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_nanosecond.default <- function(x) {
  stop_clock_unsupported(x)
}

#' @export
get_index.default <- function(x) {
  stop_clock_unsupported(x)
}
