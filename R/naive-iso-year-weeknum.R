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
    day_nonexistent = day_nonexistent
  )

  days <- field(yww, "days")

  new_iso_year_weeknum(days)
}

new_iso_year_weeknum <- function(days = integer(),
                                 ...,
                                 names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }

  fields <- list(
    days = days
  )

  new_naive_iso(
    fields,
    ...,
    names = names,
    class = "civil_naive_iso_year_weeknum"
  )
}

new_iso_year_weeknum_from_fields <- function(fields, names = NULL) {
  new_iso_year_weeknum(
    days = fields$days,
    names = names
  )
}

#' @export
vec_proxy.civil_naive_iso_year_weeknum <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_naive_iso_year_weeknum <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  new_iso_year_weeknum_from_fields(fields, names)
}

#' @export
vec_proxy_equal.civil_naive_iso_year_weeknum <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_naive_iso_year_weeknum <- function(x,
                                                ...,
                                                format = fmt_iso_year_weeknum()) {
  x <- as_naive_datetime(x)
  format(x, ..., format = format)
}

#' @export
vec_ptype_full.civil_naive_iso_year_weeknum <- function(x, ...) {
  "iso_year_weeknum"
}

#' @export
vec_ptype_abbr.civil_naive_iso_year_weeknum <- function(x, ...) {
  "iso_yw"
}

is_iso_year_weeknum <- function(x) {
  inherits(x, "civil_naive_iso_year_weeknum")
}
