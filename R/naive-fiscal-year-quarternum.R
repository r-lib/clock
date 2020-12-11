#' @export
fiscal_year_quarternum <- function(year, quarternum = 1L, ..., fiscal_start = 1L) {
  quarterday <- ones_along(year)

  yqnqd <- fiscal_year_quarternum_quarterday(
    year = year,
    quarternum = quarternum,
    quarterday = quarterday,
    ...,
    fiscal_start = fiscal_start
  )

  days <- field(yqnqd, "days")
  fiscal_start <- get_fiscal_start(yqnqd)

  new_fiscal_year_quarternum(days, fiscal_start)
}

new_fiscal_year_quarternum <- function(days = integer(),
                                       fiscal_start = 1L,
                                       ...,
                                       names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }
  if (!is_number(fiscal_start)) {
    abort("`fiscal_start` must be a single number.")
  }

  fields <- list(
    days = days
  )

  new_naive_fiscal(
    fields,
    fiscal_start = fiscal_start,
    ...,
    names = names,
    class = "civil_naive_fiscal_year_quarternum"
  )
}

new_fiscal_year_quarternum_from_fields <- function(fields, fiscal_start, names = NULL) {
  new_fiscal_year_quarternum(
    days = fields$days,
    fiscal_start = fiscal_start,
    names = names
  )
}

#' @export
vec_proxy.civil_naive_fiscal_year_quarternum <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_naive_fiscal_year_quarternum <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  fiscal_start <- get_fiscal_start(to)
  new_fiscal_year_quarternum_from_fields(fields, fiscal_start, names)
}

#' @export
vec_proxy_equal.civil_naive_fiscal_year_quarternum <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_naive_fiscal_year_quarternum <- function(x, ...) {
  # TODO: Support format = fmt_fiscal_year_quarternum() and the "%q" token and
  # pass directly to a formatter
  days <- field(x, "days")
  fiscal_start <- get_fiscal_start(x)

  fields <- convert_naive_days_to_fiscal_year_quarternum_quarterday(days, fiscal_start)
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
vec_ptype_full.civil_naive_fiscal_year_quarternum <- function(x, ...) {
  fiscal_start <- get_fiscal_start(x)
  fiscal_start <- pretty_fiscal_start(fiscal_start)
  paste0("fiscal_year_quarternum<", fiscal_start, ">")
}

#' @export
vec_ptype_abbr.civil_naive_fiscal_year_quarternum <- function(x, ...) {
  fiscal_start <- get_fiscal_start(x)
  fiscal_start <- pretty_fiscal_start(fiscal_start, abbreviate = TRUE)
  paste0("fiscal_yq<", fiscal_start, ">")
}

is_fiscal_year_quarternum <- function(x) {
  inherits(x, "civil_naive_fiscal_year_quarternum")
}
