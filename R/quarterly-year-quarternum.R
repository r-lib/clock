#' @export
year_quarternum <- function(year, quarternum = 1L, ..., start = 1L) {
  quarterday <- ones_along(year)

  yqnqd <- year_quarternum_quarterday(
    year = year,
    quarternum = quarternum,
    quarterday = quarterday,
    ...,
    start = start
  )

  days <- unstructure(yqnqd)
  start <- get_quarterly_start(yqnqd)

  new_year_quarternum(days, start)
}

new_year_quarternum <- function(days = integer(), start = 1L, ...) {
  new_quarterly(days, start, ..., class = "clock_year_quarternum")
}

#' @export
format.clock_year_quarternum <- function(x, ...) {
  # TODO: Proper format argument
  start <- get_quarterly_start(x)

  yqq <- convert_calendar_days_to_year_quarternum_quarterday(x, start)
  year <- yqq$year
  quarternum <- yqq$quarternum

  year <- sprintf("%04i", year)
  quarternum <- sprintf("%i", quarternum)

  out <- paste0(year, "-Q", quarternum)

  out[is.na(x)] <- NA_character_
  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_year_quarternum <- function(x, ...) {
  start <- get_quarterly_start(x)
  start <- pretty_quarterly_start(start)
  paste0("year_quarternum<", start, ">")
}

#' @export
vec_ptype_abbr.clock_year_quarternum <- function(x, ...) {
  start <- get_quarterly_start(x)
  start <- pretty_quarterly_start(start, abbreviate = TRUE)
  paste0("yq<", start, ">")
}

#' @export
is_year_quarternum <- function(x) {
  inherits(x, "clock_year_quarternum")
}
