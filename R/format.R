format_naive_time_point <- function(x, format) {
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
    abbreviate_zone = abbreviate_zone
  )
}

format_naive_time_point_subdaily <- function(x) {
  format_naive_time_point(x, "%H:%M:%S")
}

format_calendar_days <- function(calendar, format) {
  seconds_of_day <- seconds_of_day_init(calendar)
  tp <- new_naive_time_point(calendar, seconds_of_day)
  format_naive_time_point(tp, format)
}

format_zoned_time_point <- function(x, format, abbreviate_zone) {
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
    abbreviate_zone = abbreviate_zone
  )
}

format_zoned_time_point_subdaily <- function(x, abbreviate_zone, print_zone_name) {
  if (print_zone_name) {
    format <- "%H:%M:%S%Ez[%Z]"
  } else {
    format <- "%H:%M:%S%Ez"
  }

  format_zoned_time_point(x, format, abbreviate_zone)
}
