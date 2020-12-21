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
format.clock_year_month <- function(x, ..., format = NULL, locale = default_date_locale()) {
  if (is.null(format)) {
    format <- "%Y-%m"
  }

  format_calendar_days(x, format, locale)
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
