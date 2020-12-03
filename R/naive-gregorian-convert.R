#' @export
as_year_month <- function(x)  {
  UseMethod("as_year_month")
}

#' @export
as_year_month.default <- function(x) {
  stop_civil_unsupported_conversion(x, "civil_naive_gregorian_year_month")
}

#' @export
as_year_month.civil_naive <- function(x) {
  days <- field(x, "days")
  days <- floor_days_to_year_month_precision(days)
  new_year_month(days = days, names = names(x))
}

#' @export
as_year_month.civil_naive_gregorian_year_month <- function(x) {
  x
}

#' @export
as_year_month.Date <- function(x) {
  x <- as_year_month_day(x)
  as_year_month(x)
}

#' @export
as_year_month.POSIXt <- as_year_month.Date

#' @export
as_year_month.civil_zoned <- as_year_month.Date

# ------------------------------------------------------------------------------

#' @export
as_year_month_day <- function(x)  {
  UseMethod("as_year_month_day")
}

#' @export
as_year_month_day.default <- function(x) {
  stop_civil_unsupported_conversion(x, "civil_naive_gregorian_year_month_day")
}

#' @export
as_year_month_day.civil_naive <- function(x) {
  days <- field(x, "days")
  names <- names(x)
  new_year_month_day(days = days, names = names)
}

#' @export
as_year_month_day.civil_naive_gregorian_year_month_day <- function(x) {
  x
}

#' @export
as_year_month_day.Date <- function(x) {
  names <- names(x)
  days <- date_to_days(x)
  new_year_month_day(days, names = names)
}

#' @export
as_year_month_day.POSIXt <- function(x) {
  x <- as_naive_datetime(x)
  as_year_month_day(x)
}

#' @export
as_year_month_day.civil_zoned <- function(x) {
  x <- as_naive_datetime(x)
  as_year_month_day(x)
}

# ------------------------------------------------------------------------------

#' @export
as_naive_datetime <- function(x)  {
  UseMethod("as_naive_datetime")
}

#' @export
as_naive_datetime.default <- function(x) {
  stop_civil_unsupported_conversion(x, "civil_naive_gregorian_datetime")
}

#' @export
as_naive_datetime.civil_naive <- function(x) {
  days <- field(x, "days")
  time_of_day <- zeros_along(x)
  names <- names(x)
  new_naive_datetime(days = days, time_of_day = time_of_day, names = names)
}

#' @export
as_naive_datetime.civil_naive_gregorian_datetime <- function(x) {
  x
}

#' @export
as_naive_datetime.civil_naive_gregorian_nano_datetime <- function(x) {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  names <- names(x)
  new_naive_datetime(days = days, time_of_day = time_of_day, names = names)
}

#' @export
as_naive_datetime.Date <- function(x) {
  x <- as_year_month_day(x)
  as_naive_datetime(x)
}

#' @export
as_naive_datetime.POSIXt <- function(x) {
  x <- to_posixct(x)

  names <- names(x)
  seconds <- unstructure(x)
  zone <- get_zone(x)

  fields <- convert_sys_seconds_to_local_days_and_time_of_day(seconds, zone)

  new_naive_datetime_from_fields(fields, names)
}

#' @export
as_naive_datetime.civil_zoned_gregorian_datetime <- function(x) {
  names <- names(x)
  zone <- get_zone(x)

  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  fields <- convert_datetime_fields_from_zoned_to_local(days, time_of_day, zone)
  days <- fields$day
  time_of_day <- fields$time_of_day

  new_naive_datetime(days = days, time_of_day = time_of_day, names = names)
}

#' @export
as_naive_datetime.civil_zoned_gregorian_nano_datetime <- function(x) {
  x <- as_naive_nano_datetime(x)
  as_naive_datetime(x)
}

# ------------------------------------------------------------------------------

#' @export
as_naive_nano_datetime <- function(x)  {
  UseMethod("as_naive_nano_datetime")
}

#' @export
as_naive_nano_datetime.default <- function(x) {
  stop_civil_unsupported_conversion(x, "civil_naive_gregorian_nano_datetime")
}

#' @export
as_naive_nano_datetime.civil_naive <- function(x) {
  days <- field(x, "days")
  time_of_day <- zeros_along(x)
  nanos_of_second <- zeros_along(x)
  names <- names(x)

  new_naive_nano_datetime(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    names = names
  )
}

#' @export
as_naive_nano_datetime.civil_naive_gregorian_datetime <- function(x) {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  nanos_of_second <- zeros_along(x)
  names <- names(x)

  new_naive_nano_datetime(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    names = names
  )
}

#' @export
as_naive_nano_datetime.civil_naive_gregorian_nano_datetime <- function(x) {
  x
}

#' @export
as_naive_nano_datetime.Date <- function(x) {
  x <- as_year_month_day(x)
  as_naive_nano_datetime(x)
}

#' @export
as_naive_nano_datetime.POSIXt <- function(x) {
  x <- as_naive_datetime(x)
  as_naive_nano_datetime(x)
}

#' @export
as_naive_nano_datetime.civil_zoned_gregorian_datetime <- function(x) {
  x <- as_naive_datetime(x)
  as_naive_nano_datetime(x)
}

#' @export
as_naive_nano_datetime.civil_zoned_gregorian_nano_datetime <- function(x) {
  names <- names(x)
  zone <- get_zone(x)

  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  fields <- convert_datetime_fields_from_zoned_to_local(days, time_of_day, zone)
  days <- fields$day
  time_of_day <- fields$time_of_day

  nanos_of_second <- field(x, "nanos_of_second")

  new_naive_nano_datetime(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    names = names
  )
}
