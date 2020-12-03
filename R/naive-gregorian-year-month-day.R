#' @export
year_month_day <- function(year,
                           month = 1L,
                           day = 1L,
                           ...,
                           day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(year = year, month = month, day = day)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fields <- convert_year_month_day_to_naive_fields(
    args$year,
    args$month,
    args$day,
    day_nonexistent
  )

  new_year_month_day_from_fields(fields)
}

new_year_month_day <- function(days = integer(),
                               ...,
                               names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }

  fields <- list(
    days = days
  )

  new_naive_gregorian(
    fields,
    ...,
    names = names,
    class = "civil_naive_gregorian_year_month_day"
  )
}

new_year_month_day_from_fields <- function(fields, names = NULL) {
  new_year_month_day(
    days = fields$days,
    names = names
  )
}

#' @export
vec_proxy.civil_naive_gregorian_year_month_day <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_naive_gregorian_year_month_day <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  new_year_month_day_from_fields(fields, names)
}

#' @export
vec_proxy_equal.civil_naive_gregorian_year_month_day <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_naive_gregorian_year_month_day <- function(x,
                                                        ...,
                                                        format = fmt_year_month_day()) {
  x <- as_naive_datetime(x)
  format(x, ..., format = format)
}

#' @export
vec_ptype_full.civil_naive_gregorian_year_month_day <- function(x, ...) {
  "year_month_day"
}

#' @export
vec_ptype_abbr.civil_naive_gregorian_year_month_day <- function(x, ...) {
  "ymd"
}

is_year_month_day <- function(x) {
  inherits(x, "civil_naive_gregorian_year_month_day")
}
