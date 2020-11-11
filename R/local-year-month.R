#' @export
local_year_month <- function(year, month = 1L) {
  fields <- list(year = year, month = month)
  size <- vec_size_common(!!!fields)
  fields <- vec_recycle_common(!!!fields, .size = size)
  fields <- vec_cast_common(!!!fields, .to = integer())

  day <- vec_rep(1L, size)

  days <- convert_year_month_day_to_days(
    fields$year,
    fields$month,
    day,
    "last-time"
  )

  new_local_year_month(days)
}

new_local_year_month <- function(days = integer(),
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
    class = "civil_local_year_month"
  )
}

new_local_year_month_from_fields <- function(fields, names) {
  new_local_year_month(
    days = fields$days,
    names = names
  )
}

#' @export
vec_proxy.civil_local_year_month <- function(x, ...) {
  proxy_civil_local(x)
}

#' @export
vec_restore.civil_local_year_month <- function(x, to, ...) {
  fields <- restore_civil_local_fields(x)
  names <- restore_civil_local_names(x)
  new_local_year_month_from_fields(fields, names)
}

#' @export
format.civil_local_year_month <- function(x, ...) {
  days <- field(x, "days")

  fields <- convert_days_to_year_month_day(days)

  year <- fields$year
  month <- fields$month

  year <- format_year(year)
  month <- format_month(month)

  out <- glue(
    "<",
    year, "-", month,
    ">"
  )

  out[is.na(x)] <- NA_character_

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.civil_local_year_month <- function(x, ...) {
  "local_year_month"
}

is_local_year_month <- function(x) {
  inherits(x, "civil_local_year_month")
}
