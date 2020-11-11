#' @export
local_date <- function(year,
                       month = 1L,
                       day = 1L,
                       ...,
                       day_nonexistent = "last-time") {
  check_dots_empty()

  fields <- list(year = year, month = month, day = day)
  size <- vec_size_common(!!!fields)
  fields <- vec_recycle_common(!!!fields, .size = size)
  fields <- vec_cast_common(!!!fields, .to = integer())

  days <- convert_year_month_day_to_days(
    fields$year,
    fields$month,
    fields$day,
    day_nonexistent
  )

  new_local_date(days)
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

new_local_date_from_fields <- function(fields, names) {
  new_local_date(
    days = fields$days,
    names = names
  )
}

#' @export
vec_proxy.civil_local_date <- function(x, ...) {
  proxy_civil_local(x)
}

#' @export
vec_restore.civil_local_date <- function(x, to, ...) {
  fields <- restore_civil_local_fields(x)
  names <- restore_civil_local_names(x)
  new_local_date_from_fields(fields, names)
}

#' @export
format.civil_local_date <- function(x, ...) {
  days <- field(x, "days")

  fields <- convert_days_to_year_month_day(days)

  year <- fields$year
  month <- fields$month
  day <- fields$day

  year <- format_year(year)
  month <- format_month(month)
  day <- format_day(day)

  out <- glue(
    "<",
    year, "-", month, "-", day,
    ">"
  )

  out[is.na(x)] <- NA_character_

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.civil_local_date <- function(x, ...) {
  "local_date"
}

is_local_date <- function(x) {
  inherits(x, "civil_local_date")
}
