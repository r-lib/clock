format_local_datetime <- function(days,
                                  time_of_day,
                                  format,
                                  locale) {
  zone <- "UTC"
  nanos_of_second <- integer()

  local <- TRUE
  nano <- FALSE
  abbreviate_zone <- FALSE

  format_civil_rcrd_cpp(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    format = format,
    locale = locale,
    local = local,
    nano = nano,
    abbreviate_zone = abbreviate_zone
  )
}

format_zoned_datetime <- function(days,
                                  time_of_day,
                                  zone,
                                  format,
                                  locale,
                                  abbreviate_zone) {
  nanos_of_second <- integer()

  local <- FALSE
  nano <- FALSE

  format_civil_rcrd_cpp(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    format = format,
    locale = locale,
    local = local,
    nano = nano,
    abbreviate_zone = abbreviate_zone
  )
}

format_zoned_nano_datetime <- function(days,
                                       time_of_day,
                                       nanos_of_second,
                                       zone,
                                       format,
                                       locale,
                                       abbreviate_zone) {
  local <- FALSE
  nano <- TRUE

  format_civil_rcrd_cpp(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    format = format,
    locale = locale,
    local = local,
    nano = nano,
    abbreviate_zone = abbreviate_zone
  )
}

format_local_nano_datetime <- function(days,
                                       time_of_day,
                                       nanos_of_second,
                                       zone,
                                       format,
                                       locale) {
  zone <- "UTC"

  local <- TRUE
  nano <- TRUE
  abbreviate_zone <- FALSE

  format_civil_rcrd_cpp(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    format = format,
    locale = locale,
    local = local,
    nano = nano,
    abbreviate_zone = abbreviate_zone
  )
}
