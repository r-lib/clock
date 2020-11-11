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

  fields <- list(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second
  )

  size <- vec_size_common(!!!fields)
  fields <- vec_recycle_common(!!!fields, .size = size)
  fields <- vec_cast_common(!!!fields, .to = integer())

  days <- convert_year_month_day_to_days(
    fields$year,
    fields$month,
    fields$day,
    day_nonexistent
  )

  time_of_day <- convert_hour_minute_second_to_time_of_day(
    fields$hour,
    fields$minute,
    fields$second
  )

  new_local_datetime(days, time_of_day)
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

new_local_datetime_from_fields <- function(fields, names) {
  new_local_datetime(
    days = fields$days,
    time_of_day = fields$time_of_day,
    names = names
  )
}

#' @export
vec_proxy.civil_local_datetime <- function(x, ...) {
  proxy_civil_local(x)
}

#' @export
vec_restore.civil_local_datetime <- function(x, to, ...) {
  fields <- restore_civil_local_fields(x)
  names <- restore_civil_local_names(x)
  new_local_datetime_from_fields(fields, names)
}

#' @export
format.civil_local_datetime <- function(x, ...) {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")

  ymd <- convert_days_to_year_month_day(days)
  hms <- convert_time_of_day_to_hour_minute_second(time_of_day)

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
  "local_datetime"
}

is_local_datetime <- function(x) {
  inherits(x, "civil_local_datetime")
}
