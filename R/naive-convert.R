# Conversion to naive
# ------------------------------------------------------------------------------

#' @export
as_naive <- function(x) {
  restrict_civil_supported(x)
  UseMethod("as_naive")
}

#' @export
as_naive.civil_naive <- function(x) {
  x
}

#' @export
as_naive.Date <- function(x) {
  as_year_month_day(x)
}

#' @export
as_naive.POSIXt <- function(x) {
  as_naive_datetime(x)
}

#' @export
as_naive.civil_zoned_gregorian_datetime <- function(x) {
  as_naive_datetime(x)
}

#' @export
as_naive.civil_zoned_gregorian_nano_datetime <- function(x) {
  as_naive_nano_datetime(x)
}
