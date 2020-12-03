format_naive_datetime <- function(days,
                                  time_of_day,
                                  format) {
  zone <- "UTC"
  nanos_of_second <- integer()

  naive <- TRUE
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
