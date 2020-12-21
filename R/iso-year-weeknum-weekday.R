#' @export
iso_year_weeknum_weekday <- function(year,
                                     weeknum = 1L,
                                     weekday = 1L,
                                     ...,
                                     day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(year = year, weeknum = weeknum, weekday = weekday)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  days <- convert_iso_year_weeknum_weekday_to_calendar_days(
    args$year,
    args$weeknum,
    args$weekday,
    day_nonexistent
  )

  new_iso_year_weeknum_weekday(days = days)
}

#' @export
new_iso_year_weeknum_weekday <- function(days = integer(), ..., class = NULL) {
  new_iso(days, ..., class = c(class, "clock_iso_year_weeknum_weekday"))
}

#' @export
format.clock_iso_year_weeknum_weekday <- function(x, ..., format = NULL, locale = default_date_locale()) {
  if (is_null(format)) {
    format <- "%G-W%V-%u"
  }

  format_calendar_days(x, format, locale)
}

#' @export
vec_ptype_full.clock_iso_year_weeknum_weekday <- function(x, ...) {
  "iso_year_weeknum_weekday"
}

#' @export
vec_ptype_abbr.clock_iso_year_weeknum_weekday <- function(x, ...) {
  "iso_yww"
}

#' @export
is_iso_year_weeknum_weekday <- function(x) {
  inherits(x, "clock_iso_year_weeknum_weekday")
}
