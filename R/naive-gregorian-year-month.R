#' @export
year_month <- function(year, month = 1L) {
  args <- list(year = year, month = month)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  day <- ones_along(args$year)
  day_nonexistent <- "last-time"

  fields <- convert_year_month_day_to_local_fields(
    args$year,
    args$month,
    day,
    day_nonexistent
  )

  new_year_month_from_fields(fields)
}

new_year_month <- function(days = integer(),
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
    class = "civil_naive_gregorian_year_month"
  )
}

new_year_month_from_fields <- function(fields, names = NULL) {
  new_year_month(
    days = fields$days,
    names = names
  )
}

#' @export
vec_proxy.civil_naive_gregorian_year_month <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_naive_gregorian_year_month <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  new_year_month_from_fields(fields, names)
}

#' @export
vec_proxy_equal.civil_naive_gregorian_year_month <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_naive_gregorian_year_month <- function(x,
                                                    ...,
                                                    format = fmt_year_month()) {
  x <- as_naive_datetime(x)
  format(x, ..., format = format)
}

#' @export
vec_ptype_full.civil_naive_gregorian_year_month <- function(x, ...) {
  "year_month"
}

#' @export
vec_ptype_abbr.civil_naive_gregorian_year_month <- function(x, ...) {
  "ym"
}

is_year_month <- function(x) {
  inherits(x, "civil_naive_gregorian_year_month")
}
