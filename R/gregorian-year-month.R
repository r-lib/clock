#' @export
year_month <- function(year, month = 1L) {
  x <- year_month_day(year, month)
  new_year_month(field_year(x), field_month(x))
}

#' @export
new_year_month <- function(year = integer(),
                           month = integer(),
                           ...,
                           names = NULL,
                           class = NULL) {
  if (!is_integer(year)) {
    abort("`year` must be an integer vector.")
  }
  if (!is_integer(month)) {
    abort("`month` must be an integer vector.")
  }

  fields <- list(
    year = year,
    month = month
  )

  new_gregorian(
    fields = fields,
    ...,
    names = names,
    class = c(class, "clock_year_month")
  )
}

new_year_month_from_fields <- function(fields, names = NULL) {
  new_year_month(fields$year, fields$month, names = names)
}

#' @export
format.clock_year_month <- function(x, ...) {
  year <- field_year(x)
  month <- field_month(x)

  out <- format_year_month(year, month)
  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_year_month <- function(x, ...) {
  "year_month"
}

#' @export
vec_ptype_abbr.clock_year_month <- function(x, ...) {
  "ym"
}

#' @export
is_year_month <- function(x) {
  inherits(x, "clock_year_month")
}

#' @export
calendar_is_complete.clock_year_month <- function(x) {
  FALSE
}

#' @export
invalid_detect.clock_year_month <- function(x) {
  false_along(x)
}

#' @export
invalid_any.clock_year_month <- function(x) {
  FALSE
}

#' @export
invalid_count.clock_year_month <- function(x) {
  0L
}

#' @export
invalid_resolve.clock_year_month <- function(x, invalid = "last-day") {
  x
}
