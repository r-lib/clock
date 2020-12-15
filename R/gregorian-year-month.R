#' @export
year_month <- function(year, month = 1L) {
  day <- ones_along(year)

  ymd <- year_month_day(
    year = year,
    month = month,
    day = day
  )

  days <- unstructure(ymd)

  new_year_month(days)
}

#' @export
new_year_month <- function(days = integer(), ..., class = NULL) {
  new_gregorian(days, ..., class = c(class, "clock_year_month"))
}

#' @export
format.clock_year_month <- function(x, ..., format = "%Y-%m") {
  # TODO: Formalize this
  seconds_of_day <- zeros_along(x)
  format_naive_second_point(x, seconds_of_day, format)
}

#' @export
vec_ptype_full.clock_year_month <- function(x, ...) {
  "year_month"
}

#' @export
vec_ptype_abbr.clock_year_month <- function(x, ...) {
  "ym"
}

#' @export
is_year_month <- function(x) {
  inherits(x, "clock_year_month")
}
