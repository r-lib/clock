#' @export
as_local_date <- function(x)  {
  UseMethod("as_local_date")
}

#' @export
as_local_date.default <- function(x) {
  stop_civil_unsupported_conversion(x, "civil_local_date")
}

#' @export
as_local_date.civil_local_date <- function(x) {
  x
}

#' @export
as_local_date.civil_local_datetime <- function(x) {
  new_local_date(days = field(x, "days"), names = names(x))
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
as_local_datetime.civil_local_datetime <- function(x) {
  x
}

#' @export
as_local_datetime.civil_local_date <- function(x) {
  new_local_datetime(
    days = field(x, "days"),
    time_of_day = time_of_day_along(x),
    names = names(x)
  )
}
