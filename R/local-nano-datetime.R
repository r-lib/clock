#' @export
local_nano_datetime <- function(year,
                                month = 1L,
                                day = 1L,
                                hour = 0L,
                                minute = 0L,
                                second = 0L,
                                nanos = 0L,
                                ...,
                                day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second,
    nanos = nanos
  )

  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fields <- convert_year_month_day_hour_minute_second_nanos_to_local_fields(
    args$year,
    args$month,
    args$day,
    args$hour,
    args$minute,
    args$second,
    args$nanos,
    day_nonexistent
  )

  new_local_nano_datetime_from_fields(fields)
}

new_local_nano_datetime <- function(days = integer(),
                                    time_of_day = integer(),
                                    nanos_of_second = integer(),
                                    ...,
                                    names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }
  if (!is_integer(time_of_day)) {
    abort("`time_of_day` must be an integer.")
  }
  if (!is_integer(nanos_of_second)) {
    abort("`nanos_of_second` must be an integer.")
  }

  if (length(days) != length(time_of_day) || length(days) != length(nanos_of_second)) {
    abort("All elements to `new_local_nano_datetime()` must have the same length.")
  }

  fields <- list(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second
  )

  new_local(
    fields,
    ...,
    names = names,
    class = "civil_local_nano_datetime"
  )
}

new_local_nano_datetime_from_fields <- function(fields, names = NULL) {
  new_local_nano_datetime(
    days = fields$days,
    time_of_day = fields$time_of_day,
    nanos_of_second = fields$nanos_of_second,
    names = names
  )
}

#' @export
vec_proxy.civil_local_nano_datetime <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_local_nano_datetime <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  new_local_nano_datetime_from_fields(fields, names)
}

#' @export
vec_proxy_equal.civil_local_nano_datetime <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_local_nano_datetime <- function(x,
                                             ...,
                                             format = fmt_local_nano_datetime(),
                                             locale = Sys.getlocale(category = "LC_TIME")) {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  nanos_of_second <- field(x, "nanos_of_second")

  out <- format_local_nano_datetime(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    format = format,
    locale = locale
  )

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.civil_local_nano_datetime <- function(x, ...) {
  "civil_nano_datetime"
}

#' @export
vec_ptype_abbr.civil_local_nano_datetime <- function(x, ...) {
  "cvl_nano_dttm"
}

is_local_nano_datetime <- function(x) {
  inherits(x, "civil_local_nano_datetime")
}
