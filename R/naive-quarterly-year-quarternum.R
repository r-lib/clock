#' @export
quarterly_year_quarternum <- function(year, quarternum = 1L, ..., start = 1L) {
  quarterday <- ones_along(year)

  yqnqd <- quarterly_year_quarternum_quarterday(
    year = year,
    quarternum = quarternum,
    quarterday = quarterday,
    ...,
    start = start
  )

  days <- field(yqnqd, "days")
  start <- get_quarterly_start(yqnqd)

  new_quarterly_year_quarternum(days, start)
}

new_quarterly_year_quarternum <- function(days = integer(),
                                          start = 1L,
                                          ...,
                                          names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }
  if (!is_number(start)) {
    abort("`start` must be a single number.")
  }

  fields <- list(
    days = days
  )

  new_naive_quarterly(
    fields,
    start = start,
    ...,
    names = names,
    class = "civil_naive_quarterly_year_quarternum"
  )
}

new_quarterly_year_quarternum_from_fields <- function(fields, start, names = NULL) {
  new_quarterly_year_quarternum(
    days = fields$days,
    start = start,
    names = names
  )
}

#' @export
vec_proxy.civil_naive_quarterly_year_quarternum <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_naive_quarterly_year_quarternum <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  start <- get_quarterly_start(to)
  new_quarterly_year_quarternum_from_fields(fields, start, names)
}

#' @export
vec_proxy_equal.civil_naive_quarterly_year_quarternum <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_naive_quarterly_year_quarternum <- function(x, ...) {
  # TODO: Support format = fmt_quarterly_year_quarternum() and the "%q" token and
  # pass directly to a formatter
  days <- field(x, "days")
  start <- get_quarterly_start(x)

  fields <- convert_naive_days_to_quarterly_year_quarternum_quarterday(days, start)
  year <- fields$year
  quarternum <- fields$quarternum

  year <- sprintf("%04i", year)
  quarternum <- sprintf("%i", quarternum)

  out <- paste0(year, "-Q", quarternum)

  out[is.na(x)] <- NA_character_
  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.civil_naive_quarterly_year_quarternum <- function(x, ...) {
  start <- get_quarterly_start(x)
  start <- pretty_quarterly_start(start)
  paste0("quarterly_year_quarternum<", start, ">")
}

#' @export
vec_ptype_abbr.civil_naive_quarterly_year_quarternum <- function(x, ...) {
  start <- get_quarterly_start(x)
  start <- pretty_quarterly_start(start, abbreviate = TRUE)
  paste0("quarterly_yq<", start, ">")
}

is_quarterly_year_quarternum <- function(x) {
  inherits(x, "civil_naive_quarterly_year_quarternum")
}
