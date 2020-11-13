# Conversion to local
# ------------------------------------------------------------------------------

#' @export
as_local <- function(x) {
  restrict_civil_supported(x)
  UseMethod("as_local")
}

#' @export
as_local.civil_local <- function(x) {
  x
}

#' @export
as_local.Date <- function(x) {
  as_local_date(x)
}

#' @export
as_local.POSIXt <- function(x) {
  as_local_datetime(x)
}

#' @export
as_local.civil_zoned_datetime <- function(x) {
  as_local_datetime(x)
}

#' @export
as_local.civil_zoned_nano_datetime <- function(x) {
  as_local_nano_datetime(x)
}

# ------------------------------------------------------------------------------

#' @export
as_local_year_month <- function(x)  {
  UseMethod("as_local_year_month")
}

#' @export
as_local_year_month.default <- function(x) {
  stop_civil_unsupported_conversion(x, "civil_local_year_month")
}

#' @export
as_local_year_month.civil_local_year_month <- function(x) {
  x
}

#' @export
as_local_year_month.civil_local_date <- function(x) {
  days <- field(x, "days")
  days <- floor_days_to_year_month(days)
  new_local_year_month(days = days, names = names(x))
}

#' @export
as_local_year_month.civil_local_datetime <- as_local_year_month.civil_local_date

#' @export
as_local_year_month.civil_local_nano_datetime <- as_local_year_month.civil_local_date

#' @export
as_local_year_month.Date <- function(x) {
  x <- as_local_date(x)
  as_local_year_month(x)
}

#' @export
as_local_year_month.POSIXt <- function(x) {
  x <- as_local_datetime(x)
  as_local_year_month(x)
}

#' @export
as_local_year_month.civil_zoned <- function(x) {
  x <- as_local_date(x)
  as_local_year_month(x)
}

# ------------------------------------------------------------------------------

#' @export
as_local_date <- function(x)  {
  UseMethod("as_local_date")
}

#' @export
as_local_date.default <- function(x) {
  stop_civil_unsupported_conversion(x, "civil_local_date")
}

#' @export
as_local_date.civil_local_year_month <- function(x) {
  new_local_date(days = field(x, "days"), names = names(x))
}

#' @export
as_local_date.civil_local_date <- function(x) {
  x
}

#' @export
as_local_date.civil_local_datetime <- as_local_date.civil_local_year_month

#' @export
as_local_date.civil_local_nano_datetime <- as_local_date.civil_local_year_month

#' @export
as_local_date.Date <- function(x) {
  names <- names(x)
  days <- date_to_days(x)
  new_local_date(days, names = names)
}

#' @export
as_local_date.POSIXt <- function(x) {
  x <- as_local_datetime(x)
  as_local_date(x)
}

#' @export
as_local_date.civil_zoned <- function(x) {
  x <- as_local_datetime(x)
  as_local_date(x)
}

# ------------------------------------------------------------------------------

#' @export
as_local_datetime <- function(x)  {
  UseMethod("as_local_datetime")
}

#' @export
as_local_datetime.default <- function(x) {
  stop_civil_unsupported_conversion(x, "civil_local_datetime")
}

#' @export
as_local_datetime.civil_local_year_month <- function(x) {
  new_local_datetime(
    days = field(x, "days"),
    time_of_day = zeros_along(x),
    names = names(x)
  )
}

#' @export
as_local_datetime.civil_local_date <- as_local_datetime.civil_local_year_month

#' @export
as_local_datetime.civil_local_datetime <- function(x) {
  x
}

#' @export
as_local_datetime.civil_local_nano_datetime <- function(x) {
  new_local_datetime(
    days = field(x, "days"),
    time_of_day = field(x, "time_of_day"),
    names = names(x)
  )
}

#' @export
as_local_datetime.Date <- function(x) {
  x <- as_local_date(x)
  as_local_datetime(x)
}

#' @export
as_local_datetime.POSIXt <- function(x) {
  x <- to_posixct(x)

  names <- names(x)
  seconds <- unstructure(x)
  zone <- get_zone(x)

  fields <- convert_seconds_to_days_and_time_of_day(seconds, zone)

  new_local_datetime_from_fields(fields, names)
}

#' @export
as_local_datetime.civil_zoned_datetime <- function(x) {
  names <- names(x)
  zone <- get_zone(x)

  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  fields <- convert_datetime_fields_from_zoned_to_local(days, time_of_day, zone)
  days <- fields$day
  time_of_day <- fields$time_of_day

  new_local_datetime(
    days = days,
    time_of_day = time_of_day,
    names = names
  )
}

#' @export
as_local_datetime.civil_zoned_nano_datetime <- function(x) {
  x <- as_local_nano_datetime(x)
  as_local_datetime(x)
}

# ------------------------------------------------------------------------------

#' @export
as_local_nano_datetime <- function(x)  {
  UseMethod("as_local_nano_datetime")
}

#' @export
as_local_nano_datetime.default <- function(x) {
  stop_civil_unsupported_conversion(x, "civil_local_nano_datetime")
}

#' @export
as_local_nano_datetime.civil_local_year_month <- function(x) {
  new_local_nano_datetime(
    days = field(x, "days"),
    time_of_day = zeros_along(x),
    nanos_of_second = zeros_along(x),
    names = names(x)
  )
}

#' @export
as_local_nano_datetime.civil_local_date <- as_local_nano_datetime.civil_local_year_month

#' @export
as_local_nano_datetime.civil_local_datetime <- function(x) {
  new_local_nano_datetime(
    days = field(x, "days"),
    time_of_day = field(x, "time_of_day"),
    nanos_of_second = zeros_along(x),
    names = names(x)
  )
}

#' @export
as_local_nano_datetime.civil_local_nano_datetime <- function(x) {
  x
}

#' @export
as_local_nano_datetime.Date <- function(x) {
  x <- as_local_date(x)
  as_local_nano_datetime(x)
}

#' @export
as_local_nano_datetime.POSIXt <- function(x) {
  x <- as_local_datetime(x)
  as_local_nano_datetime(x)
}

#' @export
as_local_nano_datetime.civil_zoned_datetime <- function(x) {
  x <- as_local_datetime(x)
  as_local_nano_datetime(x)
}

#' @export
as_local_nano_datetime.civil_zoned_nano_datetime <- function(x) {
  names <- names(x)
  zone <- get_zone(x)

  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  fields <- convert_datetime_fields_from_zoned_to_local(days, time_of_day, zone)
  days <- fields$day
  time_of_day <- fields$time_of_day

  nanos_of_second <- field(x, "nanos_of_second")

  new_local_nano_datetime(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    names = names
  )
}
