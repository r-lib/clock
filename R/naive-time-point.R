#' @export
naive_date_time <- function(year,
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

  fields <- convert_year_month_day_hour_minute_second_to_naive_second_point_fields(
    args$year,
    args$month,
    args$day,
    args$hour,
    args$minute,
    args$second,
    day_nonexistent
  )

  days <- fields$days
  seconds_of_day <- fields$seconds_of_day

  calendar <- new_year_month_day(days)

  new_naive_time_point(calendar, seconds_of_day, precision = "second")
}

#' @export
naive_date_nanotime <- function(year,
                                month = 1L,
                                day = 1L,
                                hour = 0L,
                                minute = 0L,
                                second = 0L,
                                nanosecond = 0L,
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
    nanosecond = nanosecond
  )

  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fields <- convert_year_month_day_hour_minute_second_nanosecond_to_naive_subsecond_point_fields(
    args$year,
    args$month,
    args$day,
    args$hour,
    args$minute,
    args$second,
    args$nanosecond,
    day_nonexistent
  )

  days <- fields$days
  seconds_of_day <- fields$seconds_of_day
  nanoseconds_of_second <- fields$nanoseconds_of_second

  calendar <- new_year_month_day(days)

  new_naive_time_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    precision = "nanosecond"
  )
}

naive_time_point <- function(precision, calendar, hour, minute, second, subsecond = NULL) {
  args <- list(
    hour = hour,
    minute = minute,
    second = second,
    subsecond = subsecond
  )

  calendar <- promote_precision_day(calendar)

  args <- vec_cast_common(!!!args, .to = integer())
  args <- vec_recycle_common(calendar = calendar, !!!args)

  if (is_null(subsecond)) {
    fields <- convert_calendar_days_hour_minute_second_to_naive_second_point_fields(
      calendar = args$calendar,
      hour = args$hour,
      minute = args$minute,
      second = args$second
    )
  } else {
    fields <- convert_calendar_days_hour_minute_second_subsecond_to_naive_subsecond_point_fields(
      calendar = args$calendar,
      hour = args$hour,
      minute = args$minute,
      second = args$second,
      subsecond = args$subsecond,
      precision = precision
    )
  }

  new_naive_time_point(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    nanoseconds_of_second = fields$nanoseconds_of_second,
    precision = precision
  )
}

#' @export
naive_second_point <- function(calendar, hour = 0L, minute = 0L, second = 0L) {
  naive_time_point("second", calendar, hour, minute, second)
}
#' @export
naive_millisecond_point <- function(calendar, hour = 0L, minute = 0L, second = 0L, millisecond = 0L) {
  naive_time_point("millisecond", calendar, hour, minute, second, millisecond)
}
#' @export
naive_microsecond_point <- function(calendar, hour = 0L, minute = 0L, second = 0L, microsecond = 0L) {
  naive_time_point("microsecond", calendar, hour, minute, second, microsecond)
}
#' @export
naive_nanosecond_point <- function(calendar, hour = 0L, minute = 0L, second = 0L, nanosecond = 0L) {
  naive_time_point("nanosecond", calendar, hour, minute, second, nanosecond)
}

new_naive_time_point <- function(calendar = new_year_month_day(),
                                 seconds_of_day = integer(),
                                 nanoseconds_of_second = NULL,
                                 precision = "second",
                                 ...,
                                 names = NULL,
                                 class = NULL) {
  new_time_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    precision = precision,
    ...,
    names = names,
    class = c(class, "clock_naive_time_point")
  )
}

new_naive_time_point_from_fields <- function(fields, precision, names = NULL) {
  new_naive_time_point(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    nanoseconds_of_second = fields$nanoseconds_of_second,
    precision = precision,
    names = names
  )
}

#' @export
vec_proxy.clock_naive_time_point <- function(x, ...) {
  proxy_time_point(x)
}

#' @export
vec_restore.clock_naive_time_point <- function(x, to, ...) {
  fields <- restore_time_point_fields(x)
  names <- restore_time_point_names(x)
  precision <- get_precision(to)
  new_naive_time_point_from_fields(fields, precision, names)
}

#' @export
vec_proxy_equal.clock_naive_time_point <- function(x, ...) {
  proxy_equal_time_point(x)
}

#' @export
format.clock_naive_time_point <- function(x, ..., format = NULL, locale = default_date_locale()) {
  if (is_null(format)) {
    calendar <- field_calendar(x)
    ymd <- format(calendar, locale = locale)
    hms <- format_naive_time_point_subdaily(x, locale)
    out <- paste0(ymd, " ", hms)
    out[is.na(x)] <- NA_character_
  } else {
    out <- format_naive_time_point(x, format, locale)
  }

  names(out) <- names(x)
  out
}

#' @export
vec_ptype_full.clock_naive_time_point <- function(x, ...) {
  precision <- get_precision(x)
  cal <- vec_ptype_full(field_calendar(x))
  paste0("time_point<", precision, "><", cal, ">")
}

#' @export
vec_ptype_abbr.clock_naive_time_point <- function(x, ...) {
  precision <- get_precision_abbr(x)
  cal <- vec_ptype_abbr(field_calendar(x))
  paste0("tp<", precision, "><", cal, ">")
}

#' @export
is_naive_time_point <- function(x) {
  inherits(x, "clock_naive_time_point")
}
