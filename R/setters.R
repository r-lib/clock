#' Calendar setters
#'
#' @description
#' This family of functions sets fields in a calendar vector. Each
#' calendar has its own set of supported setters, which are documented on
#' their own help page:
#'
#' - [year-month-day][year-month-day-setters]
#'
#' - [year-month-weekday][year-month-weekday-setters]
#'
#' - [iso-year-week-day][iso-year-week-day-setters]
#'
#' - [year-quarter-day][year-quarter-day-setters]
#'
#' - [year-day][year-day-setters]
#'
#' There are also convenience methods for setting certain components
#' directly on R's native date and date-time types.
#'
#' - [dates (Date)][Date-setters]
#'
#' - [date-times (POSIXct / POSIXlt)][posixt-setters]
#'
#' Some general rules about setting components on calendar types:
#'
#' - You can only set components that are relevant to the calendar type that
#'   you are working with. For example, you can't set the quarter of a
#'   year-month-day type. You'd have to convert to year-quarter-day first.
#'
#' - You can set a component that is at the current precision, or one level
#'   of precision more precise than the current precision. For example,
#'   you can set the day field of a month precision year-month-day type,
#'   but not the hour field.
#'
#' - Setting a component can result in an _invalid date_, such as
#'   `set_day(year_month_day(2019, 02), 31)`, as long as it is eventually
#'   resolved either manually or with a strategy from [invalid_resolve()].
#'
#' - With sub-second precisions, you can only set the component corresponding
#'   to the precision that you are at. For example, you can set the nanoseconds
#'   of the second while at nanosecond precision, but not milliseconds.
#'
#' @details
#' You cannot set components directly on a time point type, such as
#' sys-time or naive-time. Convert it to a calendar type first. Similarly,
#' a zoned-time must be converted to either a sys-time or naive-time, and
#' then to a calendar type, to be able to set components on it.
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param x `[object]`
#'
#'   An object to set the component for.
#'
#' @param value `[integer]`
#'
#'   The value to set the component to.
#'
#' @return `x` with the component set.
#'
#' @name clock-setters
#' @examples
#' x <- year_month_day(2019, 1:3)
#'
#' # Set the day
#' set_day(x, 12:14)
#'
#' # Set to the "last" day of the month
#' set_day(x, "last")
NULL

#' @rdname clock-setters
#' @export
set_year <- function(x, value, ...) {
  UseMethod("set_year")
}

#' @rdname clock-setters
#' @export
set_quarter <- function(x, value, ...) {
  UseMethod("set_quarter")
}

#' @rdname clock-setters
#' @export
set_month <- function(x, value, ...) {
  UseMethod("set_month")
}

#' @rdname clock-setters
#' @export
set_week <- function(x, value, ...) {
  UseMethod("set_week")
}

#' @rdname clock-setters
#' @export
set_day <- function(x, value, ...) {
  UseMethod("set_day")
}

#' @rdname clock-setters
#' @export
set_hour <- function(x, value, ...) {
  UseMethod("set_hour")
}

#' @rdname clock-setters
#' @export
set_minute <- function(x, value, ...) {
  UseMethod("set_minute")
}

#' @rdname clock-setters
#' @export
set_second <- function(x, value, ...) {
  UseMethod("set_second")
}

#' @rdname clock-setters
#' @export
set_millisecond <- function(x, value, ...) {
  UseMethod("set_millisecond")
}

#' @rdname clock-setters
#' @export
set_microsecond <- function(x, value, ...) {
  UseMethod("set_microsecond")
}

#' @rdname clock-setters
#' @export
set_nanosecond <- function(x, value, ...) {
  UseMethod("set_nanosecond")
}

#' @rdname clock-setters
#' @export
set_index <- function(x, value, ...) {
  UseMethod("set_index")
}
