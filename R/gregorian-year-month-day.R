#' @export
year_month_day <- function(year, month = 1L, day = 1L) {
  if (is_last(day)) {
    day <- -1L
    last <- TRUE
  } else {
    last <- FALSE
  }

  args <- list(year = year, month = month, day = day)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fields <- collect_year_month_day_fields(
    args$year,
    args$month,
    args$day,
    last
  )

  new_year_month_day_from_fields(fields)
}

#' @export
new_year_month_day <- function(year = integer(),
                               month = integer(),
                               day = integer(),
                               ...,
                               names = NULL,
                               class = NULL) {
  if (!is_integer(year)) {
    abort("`year` must be an integer vector.")
  }
  if (!is_integer(month)) {
    abort("`month` must be an integer vector.")
  }
  if (!is_integer(day)) {
    abort("`day` must be an integer vector.")
  }

  fields <- list(
    year = year,
    month = month,
    day = day
  )

  new_gregorian(
    fields = fields,
    ...,
    names = names,
    class = c(class, "clock_year_month_day")
  )
}

new_year_month_day_from_fields <- function(fields, names = NULL) {
  new_year_month_day(fields$year, fields$month, fields$day, names = names)
}

#' @export
format.clock_year_month_day <- function(x, ...) {
  year <- field_year(x)
  month <- field_month(x)
  day <- field_day(x)

  out <- format_year_month_day(year, month, day)
  names(out) <- names(x)
  out
}

#' @export
vec_ptype_full.clock_year_month_day <- function(x, ...) {
  calendar_ptype_full(x, "year_month_day")
}

#' @export
vec_ptype_abbr.clock_year_month_day <- function(x, ...) {
  calendar_ptype_abbr(x, "ymd")
}

#' @export
is_year_month_day <- function(x) {
  inherits(x, "clock_year_month_day")
}

#' @export
calendar_is_complete.clock_year_month_day <- function(x) {
  TRUE
}

#' @export
invalid_detect.clock_year_month_day <- function(x) {
  invalid_detect_year_month_day(field_year(x), field_month(x), field_day(x))
}

#' @export
invalid_any.clock_year_month_day <- function(x) {
  invalid_any_year_month_day(field_year(x), field_month(x), field_day(x))
}

#' @export
invalid_count.clock_year_month_day <- function(x) {
  invalid_count_year_month_day(field_year(x), field_month(x), field_day(x))
}

#' @export
invalid_resolve.clock_year_month_day <- function(x, invalid = "last-day") {
  if (!invalid_any(x)) {
    return(x)
  }

  fields <- invalid_resolve_year_month_day(
    field_year(x),
    field_month(x),
    field_day(x),
    invalid
  )

  new_year_month_day_from_fields(fields, names = names(x))
}
