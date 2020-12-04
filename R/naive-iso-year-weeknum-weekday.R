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

  days <- convert_iso_year_weeknum_weekday_to_naive_days(
    args$year,
    args$weeknum,
    args$weekday,
    day_nonexistent
  )

  new_iso_year_weeknum_weekday(days = days)
}

new_iso_year_weeknum_weekday <- function(days = integer(),
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
    class = "civil_naive_iso_year_weeknum_weekday"
  )
}

new_iso_year_weeknum_weekday_from_fields <- function(fields, names = NULL) {
  new_iso_year_weeknum_weekday(
    days = fields$days,
    names = names
  )
}

#' @export
vec_proxy.civil_naive_iso_year_weeknum_weekday <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_naive_iso_year_weeknum_weekday <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  new_iso_year_weeknum_weekday_from_fields(fields, names)
}

#' @export
vec_proxy_equal.civil_naive_iso_year_weeknum_weekday <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_naive_iso_year_weeknum_weekday <- function(x,
                                                        ...,
                                                        format = fmt_iso_week_date()) {
  x <- as_naive_datetime(x)
  format(x, ..., format = format)
}

#' @export
vec_ptype_full.civil_naive_iso_year_weeknum_weekday <- function(x, ...) {
  "iso_year_weeknum_weekday"
}

#' @export
vec_ptype_abbr.civil_naive_iso_year_weeknum_weekday <- function(x, ...) {
  "iso_yww"
}

is_iso_year_weeknum_weekday <- function(x) {
  inherits(x, "civil_naive_iso_year_weeknum_weekday")
}
