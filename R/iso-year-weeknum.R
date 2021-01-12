#' @export
iso_year_weeknum <- function(year, weeknum = 1L) {
  x <- iso_year_weeknum_weekday(year, weeknum)
  new_iso_year_weeknum(field_year(x), field_weeknum(x))
}

#' @export
new_iso_year_weeknum <- function(year = integer(),
                                 weeknum = integer(),
                           ...,
                           names = NULL,
                           class = NULL) {
  if (!is_integer(year)) {
    abort("`year` must be an integer vector.")
  }
  if (!is_integer(weeknum)) {
    abort("`weeknum` must be an integer vector.")
  }

  fields <- list(
    year = year,
    weeknum = weeknum
  )

  new_iso(
    fields = fields,
    ...,
    names = names,
    class = c(class, "clock_iso_year_weeknum")
  )
}

new_iso_year_weeknum_from_fields <- function(fields, names = NULL) {
  new_iso_year_weeknum(fields$year, fields$weeknum, names = names)
}

#' @export
format.clock_iso_year_weeknum <- function(x, ...) {
  year <- field_year(x)
  weeknum <- field_weeknum(x)

  out <- format_iso_year_weeknum(year, weeknum)
  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_iso_year_weeknum <- function(x, ...) {
  calendar_ptype_full(x, "iso_year_weeknum")
}

#' @export
vec_ptype_abbr.clock_iso_year_weeknum <- function(x, ...) {
  calendar_ptype_abbr(x, "iso_yw")
}

#' @export
is_iso_year_weeknum <- function(x) {
  inherits(x, "clock_iso_year_weeknum")
}

#' @export
calendar_is_complete.clock_iso_year_weeknum <- function(x) {
  FALSE
}

#' @export
invalid_detect.clock_iso_year_weeknum <- function(x) {
  invalid_detect_iso_year_weeknum(field_year(x), field_weeknum(x))
}

#' @export
invalid_any.clock_iso_year_weeknum <- function(x) {
  invalid_any_iso_year_weeknum(field_year(x), field_weeknum(x))
}

#' @export
invalid_count.clock_iso_year_weeknum <- function(x) {
  invalid_count_iso_year_weeknum(field_year(x), field_weeknum(x))
}

#' @export
invalid_resolve.clock_iso_year_weeknum <- function(x, invalid = "last-day") {
  if (!invalid_any(x)) {
    return(x)
  }

  fields <- invalid_resolve_iso_year_weeknum(
    field_year(x),
    field_weeknum(x),
    invalid
  )

  new_iso_year_weeknum_from_fields(fields, names = names(x))
}
