#' @export
iso_year_weeknum_weekday <- function(year, weeknum = 1L, weekday = 1L) {
  if (is_last(weeknum)) {
    weeknum <- -1L
    last <- TRUE
  } else {
    last <- FALSE
  }

  args <- list(year = year, weeknum = weeknum, weekday = weekday)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fields <- collect_iso_year_weeknum_weekday_fields(
    args$year,
    args$weeknum,
    args$weekday,
    last
  )

  new_iso_year_weeknum_weekday_from_fields(fields)
}

#' @export
new_iso_year_weeknum_weekday <- function(year = integer(),
                                         weeknum = integer(),
                                         weekday = integer(),
                                         ...,
                                         names = NULL,
                                         class = NULL) {
  if (!is_integer(year)) {
    abort("`year` must be an integer vector.")
  }
  if (!is_integer(weeknum)) {
    abort("`weeknum` must be an integer vector.")
  }
  if (!is_integer(weekday)) {
    abort("`weekday` must be an integer vector.")
  }

  fields <- list(
    year = year,
    weeknum = weeknum,
    weekday = weekday
  )

  new_iso(
    fields = fields,
    ...,
    names = names,
    class = c(class, "clock_iso_year_weeknum_weekday")
  )
}

new_iso_year_weeknum_weekday_from_fields <- function(fields, names = NULL) {
  new_iso_year_weeknum_weekday(fields$year, fields$weeknum, fields$weekday, names = names)
}

#' @export
format.clock_iso_year_weeknum_weekday <- function(x, ...) {
  format_iso_year_weeknum_weekday(field_year(x), field_weeknum(x), field_weekday(x))
}

#' @export
vec_ptype_full.clock_iso_year_weeknum_weekday <- function(x, ...) {
  calendar_ptype_full(x, "iso_year_weeknum_weekday")
}

#' @export
vec_ptype_abbr.clock_iso_year_weeknum_weekday <- function(x, ...) {
  calendar_ptype_abbr(x, "iso_yww")
}

#' @export
is_iso_year_weeknum_weekday <- function(x) {
  inherits(x, "clock_iso_year_weeknum_weekday")
}

#' @export
calendar_is_complete.clock_iso_year_weeknum_weekday <- function(x) {
  TRUE
}

#' @export
invalid_detect.clock_iso_year_weeknum_weekday <- function(x) {
  invalid_detect_iso_year_weeknum_weekday(field_year(x), field_weeknum(x), field_weekday(x))
}

#' @export
invalid_any.clock_iso_year_weeknum_weekday <- function(x) {
  invalid_any_iso_year_weeknum_weekday(field_year(x), field_weeknum(x), field_weekday(x))
}

#' @export
invalid_count.clock_iso_year_weeknum_weekday <- function(x) {
  invalid_count_iso_year_weeknum_weekday(field_year(x), field_weeknum(x), field_weekday(x))
}

#' @export
invalid_resolve.clock_iso_year_weeknum_weekday <- function(x, invalid = "last-day") {
  if (!invalid_any(x)) {
    return(x)
  }

  fields <- invalid_resolve_iso_year_weeknum_weekday(
    field_year(x),
    field_month(x),
    field_day(x),
    invalid
  )

  new_iso_year_weeknum_weekday_from_fields(fields, names = names(x))
}
