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

  days <- convert_quarterly_year_quarternum_quarterday_to_naive_days(
    year = args$year,
    quarternum = args$quarternum,
    quarterday = args$quarterday,
    start = start,
    day_nonexistent = day_nonexistent
  )

  new_year_quarternum_quarterday(days, start)
}

new_year_quarternum_quarterday <- function(days = integer(),
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
    class = "civil_naive_quarterly_year_quarternum_quarterday"
  )
}

new_year_quarternum_quarterday_from_fields <- function(fields, start, names = NULL) {
  new_year_quarternum_quarterday(
    days = fields$days,
    start = start,
    names = names
  )
}

#' @export
vec_proxy.civil_naive_quarterly_year_quarternum_quarterday <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_naive_quarterly_year_quarternum_quarterday <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  start <- get_quarterly_start(to)
  new_year_quarternum_quarterday_from_fields(fields, start, names)
}

#' @export
vec_proxy_equal.civil_naive_quarterly_year_quarternum_quarterday <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_naive_quarterly_year_quarternum_quarterday <- function(x, ...) {
  # TODO: Support format = fmt_year_quarternum_quarterday() and the "%q" token and
  # pass directly to a formatter
  days <- field(x, "days")
  start <- get_quarterly_start(x)

  fields <- convert_naive_days_to_quarterly_year_quarternum_quarterday(days, start)
  year <- fields$year
  quarternum <- fields$quarternum
  quarterday <- fields$quarterday

  year <- sprintf("%04i", year)
  quarternum <- sprintf("%i", quarternum)
  quarterday <- sprintf("%02i", quarterday)

  out <- paste0(year, "-Q", quarternum, "-", quarterday)

  out[is.na(x)] <- NA_character_
  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.civil_naive_quarterly_year_quarternum_quarterday <- function(x, ...) {
  start <- get_quarterly_start(x)
  start <- pretty_quarterly_start(start)
  paste0("year_quarternum_quarterday<", start, ">")
}

#' @export
vec_ptype_abbr.civil_naive_quarterly_year_quarternum_quarterday <- function(x, ...) {
  start <- get_quarterly_start(x)
  start <- pretty_quarterly_start(start, abbreviate = TRUE)
  paste0("yqq<", start, ">")
}

is_year_quarternum_quarterday <- function(x) {
  inherits(x, "civil_naive_quarterly_year_quarternum_quarterday")
}
