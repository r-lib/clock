#' @export
iso_year_weeknum <- function(year,
                             weeknum = 1L,
                             ...,
                             day_nonexistent = "last-time") {
  weekday <- ones_along(year)

  yww <- iso_year_weeknum_weekday(
    year = year,
    weeknum = weeknum,
    weekday = weekday,
    ...,
    day_nonexistent = day_nonexistent
  )

  days <- unstructure(yww)

  new_iso_year_weeknum(days = days)
}

#' @export
new_iso_year_weeknum <- function(days = integer(), ..., class = NULL) {
  new_iso(days, ..., class = c(class, "clock_iso_year_weeknum"))
}

#' @export
format.clock_iso_year_weeknum <- function(x, ..., format = "%G-W%V") {
  # TODO: Formalize this
  seconds_of_day <- zeros_along(x)
  format_naive_second_point(x, seconds_of_day, format)
}

#' @export
vec_ptype_full.clock_iso_year_weeknum <- function(x, ...) {
  "iso_year_weeknum"
}

#' @export
vec_ptype_abbr.clock_iso_year_weeknum <- function(x, ...) {
  "iso_yw"
}

#' @export
is_iso_year_weeknum <- function(x) {
  inherits(x, "clock_iso_year_weeknum")
}
