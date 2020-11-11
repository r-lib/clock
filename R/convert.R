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
  new_local_year_month(days = field(x, "days"), names = names(x))
}

#' @export
as_local_year_month.civil_local_datetime <- as_local_year_month.civil_local_date

#' @export
as_local_year_month.civil_local_nano_datetime <- as_local_year_month.civil_local_date

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
