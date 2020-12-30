#' @export
year_quarternum_quarterday <- function(year,
                                       quarternum = 1L,
                                       quarterday = 1L,
                                       ...,
                                       start = 1L) {
  check_dots_empty()

  if (is_last(quarterday)) {
    quarterday <- -1L
    last <- TRUE
  } else {
    last <- FALSE
  }

  args <- list(year = year, quarternum = quarternum, quarterday = quarterday)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  start <- cast_quarterly_start(start)

  fields <- collect_year_quarternum_quarterday_fields(
    year = args$year,
    quarternum = args$quarternum,
    quarterday = args$quarterday,
    start = start,
    last = last
  )

  new_year_quarternum_quarterday_from_fields(fields, start)
}

#' @export
new_year_quarternum_quarterday <- function(year = integer(),
                                           quarternum = integer(),
                                           quarterday = integer(),
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
  if (!is_integer(quarterday)) {
    abort("`quarterday` must be an integer vector.")
  }

  fields <- list(
    year = year,
    quarternum = quarternum,
    quarterday = quarterday
  )

  new_quarterly(
    fields = fields,
    start = start,
    ...,
    names = names,
    class = c(class, "clock_year_quarternum_quarterday")
  )
}

new_year_quarternum_quarterday_from_fields <- function(fields, start, names = NULL) {
  new_year_quarternum_quarterday(
    year = fields$year,
    quarternum = fields$quarternum,
    quarterday = fields$quarterday,
    start = start,
    names = names
  )
}

#' @export
format.clock_year_quarternum_quarterday <- function(x, ...) {
  out <- format_year_quarternum_quarterday(
    field_year(x),
    field_quarternum(x),
    field_quarterday(x),
    get_quarterly_start(x)
  )

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_year_quarternum_quarterday <- function(x, ...) {
  start <- get_quarterly_start(x)
  start <- pretty_quarterly_start(start)
  class <- paste0("year_quarternum_quarterday<", start, ">")
  calendar_ptype_full(x, class)
}

#' @export
vec_ptype_abbr.clock_year_quarternum_quarterday <- function(x, ...) {
  start <- get_quarterly_start(x)
  start <- pretty_quarterly_start(start, abbreviate = TRUE)
  abbr <- paste0("yqq<", start, ">")
  calendar_ptype_abbr(x, abbr)
}

#' @export
is_year_quarternum_quarterday <- function(x) {
  inherits(x, "clock_year_quarternum_quarterday")
}

#' @export
calendar_is_complete.clock_year_quarternum_quarterday <- function(x) {
  TRUE
}

#' @export
invalid_detect.clock_year_quarternum_quarterday <- function(x) {
  invalid_detect_year_quarternum_quarterday(
    year = field_year(x),
    quarternum = field_quarternum(x),
    quarterday = field_quarterday(x),
    start = get_quarterly_start(x)
  )
}

#' @export
invalid_any.clock_year_quarternum_quarterday <- function(x) {
  invalid_any_year_quarternum_quarterday(
    year = field_year(x),
    quarternum = field_quarternum(x),
    quarterday = field_quarterday(x),
    start = get_quarterly_start(x)
  )
}

#' @export
invalid_count.clock_year_quarternum_quarterday <- function(x) {
  invalid_count_year_quarternum_quarterday(
    year = field_year(x),
    quarternum = field_quarternum(x),
    quarterday = field_quarterday(x),
    start = get_quarterly_start(x)
  )
}

#' @export
invalid_resolve.clock_year_quarternum_quarterday <- function(x, invalid = "last-day") {
  if (!invalid_any(x)) {
    return(x)
  }

  fields <- invalid_resolve_year_quarternum_quarterday(
    year = field_year(x),
    quarternum = field_quarternum(x),
    quarterday = field_quarterday(x),
    start = get_quarterly_start(x),
    invalid = invalid
  )

  new_year_quarternum_quarterday_from_fields(fields, names = names(x))
}
