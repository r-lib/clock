#' @export
local_datetime <- function(year = NULL,
                           month = NULL,
                           day = NULL,
                           hour = NULL,
                           minute = NULL,
                           second = NULL,
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

  if (is_null(year)) {
    fields$year <- integer()
  }

  if (is_null(month)) {
    fields$month <- rep(1L, size)
  } else if (is_null(year)) {
    abort("Can't specify `month` without `year`.")
  }

  if (is_null(day)) {
    fields$day <- rep(1L, size)
  } else if (is_null(year) || is_null(month)) {
    abort("Can't specify `day` without `year` and `month`.")
  }

  if (is_null(hour)) {
    fields$hour <- rep(0L, size)
  } else if (is_null(year) || is_null(month) || is_null(day)) {
    abort("Can't specify `hour` without `year`, `month`, and `day`.")
  }

  if (is_null(minute)) {
    fields$minute <- rep(0L, size)
  } else if (is_null(year) || is_null(month) || is_null(day) || is_null(hour)) {
    abort("Can't specify `minute` without `year`, `month`, `day`, and `hour`.")
  }

  if (is_null(second)) {
    fields$second <- rep(0L, size)
  } else if (is_null(year) || is_null(month) || is_null(day) || is_null(hour) || is_null(minute)) {
    abort("Can't specify `second` without `year`, `month`, `day`, `hour`, and `minute`.")
  }

  fields <- lapply(fields, vec_cast, to = integer())

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
