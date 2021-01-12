#' @export
year_quarternum <- function(year,
                            quarternum = 1L,
                            ...,
                            start = 1L) {
  x <- year_quarternum_quarterday(
    year = year,
    quarternum = quarternum,
    ...,
    start = start
  )

  start <- get_quarterly_start(x)

  new_year_quarternum(
    year = field_year(x),
    quarternum = field_quarternum(x),
    start = start
  )
}

#' @export
new_year_quarternum <- function(year = integer(),
                                quarternum = integer(),
                                start = 1L,
                                ...,
                                names = NULL,
                                class = NULL) {
  if (!is_integer(year)) {
    abort("`year` must be an integer vector.")
  }
  if (!is_integer(quarternum)) {
    abort("`quarternum` must be an integer vector.")
  }

  fields <- list(
    year = year,
    quarternum = quarternum
  )

  new_quarterly(
    fields = fields,
    start = start,
    ...,
    names = names,
    class = c(class, "clock_year_quarternum")
  )
}

new_year_quarternum_from_fields <- function(fields, start, names = NULL) {
  new_year_quarternum(
    year = fields$year,
    quarternum = fields$quarternum,
    start = start,
    names = names
  )
}

#' @export
format.clock_year_quarternum <- function(x, ...) {
  out <- format_year_quarternum(
    field_year(x),
    field_quarternum(x)
  )

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

#' @export
calendar_is_complete.clock_year_quarternum <- function(x) {
  FALSE
}

#' @export
invalid_detect.clock_year_quarternum <- function(x) {
  false_along(x)
}

#' @export
invalid_any.clock_year_quarternum <- function(x) {
  FALSE
}

#' @export
invalid_count.clock_year_quarternum <- function(x) {
  0L
}

#' @export
invalid_resolve.clock_year_quarternum <- function(x, invalid = "last-day") {
  x
}
