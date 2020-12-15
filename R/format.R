format_naive_second_point <- function(calendar,
                                      seconds_of_day,
                                      format) {
  nanoseconds_of_second <- integer()
  zone <- "UTC"
  precision <- "second"
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

format_zoned_second_point <- function(calendar,
                                      seconds_of_day,
                                      zone,
                                      format,
                                      abbreviate_zone) {
  nanoseconds_of_second <- integer()
  precision <- "second"
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

format_naive_subsecond_point <- function(calendar,
                                         seconds_of_day,
                                         nanoseconds_of_second,
                                         format,
                                         precision) {
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

format_zoned_subsecond_point <- function(calendar,
                                         seconds_of_day,
                                         nanoseconds_of_second,
                                         precision,
                                         zone,
                                         format,
                                         abbreviate_zone) {
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

format_naive_datetime <- function(days,
                                  time_of_day,
                                  format) {
  nanos_of_second <- integer()

  zone <- "UTC"
  naive <- FALSE
  nano <- FALSE
  abbreviate_zone <- FALSE

  format_civil_rcrd_cpp(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    format = format,
    naive = naive,
    nano = nano,
    abbreviate_zone = abbreviate_zone
  )
}

format_zoned_datetime <- function(days,
                                  time_of_day,
                                  zone,
                                  format,
                                  abbreviate_zone) {
  nanos_of_second <- integer()

  naive <- FALSE
  nano <- FALSE

  format_civil_rcrd_cpp(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    format = format,
    naive = naive,
    nano = nano,
    abbreviate_zone = abbreviate_zone
  )
}

format_zoned_nano_datetime <- function(days,
                                       time_of_day,
                                       nanos_of_second,
                                       zone,
                                       format,
                                       abbreviate_zone) {
  naive <- FALSE
  nano <- TRUE

  format_civil_rcrd_cpp(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    format = format,
    naive = naive,
    nano = nano,
    abbreviate_zone = abbreviate_zone
  )
}

format_naive_nano_datetime <- function(days,
                                       time_of_day,
                                       nanos_of_second,
                                       zone,
                                       format) {
  zone <- "UTC"

  naive <- TRUE
  nano <- TRUE
  abbreviate_zone <- FALSE

  format_civil_rcrd_cpp(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    format = format,
    naive = naive,
    nano = nano,
    abbreviate_zone = abbreviate_zone
  )
}

format_naive_seconds_of_day <- function(calendar, seconds_of_day) {
  format_naive_second_point(calendar, seconds_of_day, "%H:%M:%S")
}

format_zoned_seconds_of_day <- function(calendar, seconds_of_day, zone, abbreviate_zone, print_zone_name) {
  if (print_zone_name) {
    format <- "%H:%M:%S%Ez[%Z]"
  } else {
    format <- "%H:%M:%S%Ez"
  }
  format_zoned_second_point(calendar, seconds_of_day, zone, format, abbreviate_zone)
}

format_naive_nanoseconds_of_second <- function(calendar, seconds_of_day, nanoseconds_of_second, precision) {
  format_naive_subsecond_point(calendar, seconds_of_day, nanoseconds_of_second, "%H:%M:%S", precision)
}

format_zoned_nanoseconds_of_second <- function(calendar, seconds_of_day, nanoseconds_of_second, precision, zone, abbreviate_zone, print_zone_name) {
  if (print_zone_name) {
    format <- "%H:%M:%S%Ez[%Z]"
  } else {
    format <- "%H:%M:%S%Ez"
  }
  format_zoned_subsecond_point(calendar, seconds_of_day, nanoseconds_of_second, precision, zone, format, abbreviate_zone)
}
