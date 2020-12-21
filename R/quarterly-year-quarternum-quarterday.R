#' @export
year_quarternum_quarterday <- function(year,
                                       quarternum = 1L,
                                       quarterday = 1L,
                                       ...,
                                       start = 1L,
                                       day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(year = year, quarternum = quarternum, quarterday = quarterday)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  start <- cast_quarterly_start(start)

  days <- convert_year_quarternum_quarterday_to_calendar_days(
    year = args$year,
    quarternum = args$quarternum,
    quarterday = args$quarterday,
    start = start,
    day_nonexistent = day_nonexistent
  )

  new_year_quarternum_quarterday(days, start)
}

new_year_quarternum_quarterday <- function(days = integer(), start = 1L, ...) {
  new_quarterly(days, start, ..., class = "clock_year_quarternum_quarterday")
}

#' @export
format.clock_year_quarternum_quarterday <- function(x, ..., format = NULL, locale = default_date_locale()) {
  if (!is.null(format)) {
    out <- format_calendar_days(x, format, locale)
    return(out)
  }

  start <- get_quarterly_start(x)

  yqq <- convert_calendar_days_to_year_quarternum_quarterday(x, start)
  year <- yqq$year
  quarternum <- yqq$quarternum
  quarterday <- yqq$quarterday

  year <- sprintf("%04i", year)
  quarternum <- sprintf("%i", quarternum)
  quarterday <- sprintf("%02i", quarterday)

  out <- paste0(year, "-Q", quarternum, "-", quarterday)

  out[is.na(x)] <- NA_character_
  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_year_quarternum_quarterday <- function(x, ...) {
  start <- get_quarterly_start(x)
  start <- pretty_quarterly_start(start)
  paste0("year_quarternum_quarterday<", start, ">")
}

#' @export
vec_ptype_abbr.clock_year_quarternum_quarterday <- function(x, ...) {
  start <- get_quarterly_start(x)
  start <- pretty_quarterly_start(start, abbreviate = TRUE)
  paste0("yqq<", start, ">")
}

#' @export
is_year_quarternum_quarterday <- function(x) {
  inherits(x, "clock_year_quarternum_quarterday")
}
