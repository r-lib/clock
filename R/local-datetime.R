#' @export
local_datetime <- function(year,
                           month = 1L,
                           day = 1L,
                           hour = 0L,
                           minute = 0L,
                           second = 0L,
                           ...,
                           day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second
  )

  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fields <- convert_year_month_day_hour_minute_second_to_local_fields(
    args$year,
    args$month,
    args$day,
    args$hour,
    args$minute,
    args$second,
    day_nonexistent
  )

  new_local_datetime_from_fields(fields)
}

new_local_datetime <- function(days = integer(),
                               time_of_day = integer(),
                               ...,
                               names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }
  if (!is_integer(time_of_day)) {
    abort("`time_of_day` must be an integer.")
  }

  if (length(days) != length(time_of_day)) {
    abort("All elements to `new_local_datetime()` must have the same length.")
  }

  fields <- list(
    days = days,
    time_of_day = time_of_day
  )

  new_local(
    fields,
    ...,
    names = names,
    class = "civil_local_datetime"
  )
}

new_local_datetime_from_fields <- function(fields, names = NULL) {
  new_local_datetime(
    days = fields$days,
    time_of_day = fields$time_of_day,
    names = names
  )
}

#' @export
vec_proxy.civil_local_datetime <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_local_datetime <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  new_local_datetime_from_fields(fields, names)
}

#' @export
vec_proxy_equal.civil_local_datetime <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_local_datetime <- function(x, ...) {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")

  ymd <- convert_local_days_to_year_month_day(days)
  hms <- convert_local_time_of_day_to_hour_minute_second(time_of_day)

  year <- ymd$year
  month <- ymd$month
  day <- ymd$day
  hour <- hms$hour
  minute <- hms$minute
  second <- hms$second

  year <- format_year(year)
  month <- format_month(month)
  day <- format_day(day)
  hour <- format_hour(hour)
  minute <- format_minute(minute)
  second <- format_second(second)

  out <- glue(
    "<",
    year, "-", month, "-", day,
    " ",
    hour, ":", minute, ":", second,
    ">"
  )

  out[is.na(x)] <- NA_character_

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.civil_local_datetime <- function(x, ...) {
  "civil_datetime"
}

#' @export
vec_ptype_abbr.civil_local_datetime <- function(x, ...) {
  "cvl_dttm"
}

is_local_datetime <- function(x) {
  inherits(x, "civil_local_datetime")
}
