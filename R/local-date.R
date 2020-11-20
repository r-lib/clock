#' @export
local_date <- function(year,
                       month = 1L,
                       day = 1L,
                       ...,
                       day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(year = year, month = month, day = day)
  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fields <- convert_year_month_day_to_local_fields(
    args$year,
    args$month,
    args$day,
    day_nonexistent
  )

  new_local_date_from_fields(fields)
}

new_local_date <- function(days = integer(),
                           ...,
                           names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }

  fields <- list(
    days = days
  )

  new_local(
    fields,
    ...,
    names = names,
    class = "civil_local_date"
  )
}

new_local_date_from_fields <- function(fields, names = NULL) {
  new_local_date(
    days = fields$days,
    names = names
  )
}

#' @export
vec_proxy.civil_local_date <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_local_date <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  new_local_date_from_fields(fields, names)
}

#' @export
vec_proxy_equal.civil_local_date <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_local_date <- function(x,
                                    ...,
                                    format = fmt_local_date(),
                                    locale = "en_US.UTF-8") {
  x <- as_local_datetime(x)
  format(x, ..., format = format, locale = locale)
}

#' @export
vec_ptype_full.civil_local_date <- function(x, ...) {
  "civil_date"
}

#' @export
vec_ptype_abbr.civil_local_date <- function(x, ...) {
  "cvl_date"
}

is_local_date <- function(x) {
  inherits(x, "civil_local_date")
}
