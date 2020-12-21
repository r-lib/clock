format_naive_time_point <- function(x, format, locale) {
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  precision <- get_precision(x)

  if (is_subsecond_precision(precision)) {
    nanoseconds_of_second <- field_nanoseconds_of_second(x)
  } else {
    nanoseconds_of_second <- integer()
  }

  zone <- "UTC"
  naive <- TRUE
  abbreviate_zone <- FALSE

  format_time_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    zone = zone,
    format = format,
    precision = precision,
    naive = naive,
    abbreviate_zone = abbreviate_zone,
    locale = locale
  )
}

format_naive_time_point_subdaily <- function(x, locale) {
  format_naive_time_point(x, "%H:%M:%S", locale)
}

format_calendar_days <- function(calendar, format, locale) {
  calendar <- promote_precision_day(calendar)
  seconds_of_day <- seconds_of_day_init(calendar)
  tp <- new_naive_time_point(calendar, seconds_of_day)
  format_naive_time_point(tp, format, locale)
}

format_zoned_time_point <- function(x, format, abbreviate_zone, locale) {
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  precision <- get_precision(x)

  if (is_subsecond_precision(precision)) {
    nanoseconds_of_second <- field_nanoseconds_of_second(x)
  } else {
    nanoseconds_of_second <- integer()
  }

  zone <- get_zone(x)
  naive <- FALSE

  format_time_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    zone = zone,
    format = format,
    precision = precision,
    naive = naive,
    abbreviate_zone = abbreviate_zone,
    locale = locale
  )
}

format_zoned_time_point_subdaily <- function(x, abbreviate_zone, locale, print_zone_name) {
  if (print_zone_name) {
    format <- "%H:%M:%S%Ez[%Z]"
  } else {
    format <- "%H:%M:%S%Ez"
  }

  format_zoned_time_point(x, format, abbreviate_zone, locale)
}

format_time_point <- function(calendar,
                              seconds_of_day,
                              nanoseconds_of_second,
                              zone,
                              format,
                              precision,
                              naive,
                              abbreviate_zone,
                              locale) {
  date_names <- locale$date_names
  decimal_mark <- locale$decimal_mark

  format_time_point_cpp(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    zon = zone,
    format = format,
    precision = precision,
    naive = naive,
    abbreviate_zone = abbreviate_zone,
    mon = date_names$mon,
    mon_ab = date_names$mon_ab,
    day = date_names$day,
    day_ab = date_names$day_ab,
    am_pm = date_names$am_pm,
    decimal_mark = decimal_mark
  )
}
