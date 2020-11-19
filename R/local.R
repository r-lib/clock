new_local <- function(fields, ..., names = NULL, class = NULL) {
  new_civil_rcrd(fields, ..., names = names, class = c(class, "civil_local"))
}

is_local <- function(x) {
  inherits(x, "civil_local")
}

format_local_body <- function(...,
                              year = NULL,
                              month = NULL,
                              day = NULL,
                              hour = NULL,
                              minute = NULL,
                              second = NULL,
                              nanos = NULL) {
  check_dots_empty()

  body <- NULL

  if (!is.null(year)) {
    year <- format_year(year)
    body <- glue(body, year)
  }
  if (!is.null(month)) {
    month <- format_month(month)
    body <- glue(body, "-", month)
  }
  if (!is.null(day)) {
    day <- format_day(day)
    body <- glue(body, "-", day)
  }

  if (!is.null(hour)) {
    # At least hour, add "T" between ymd and hmd
    body <- glue(body, "T")

    hour <- format_hour(hour)
    body <- glue(body, hour)
  }
  if (!is.null(minute)) {
    minute <- format_minute(minute)
    body <- glue(body, ":", minute)
  }
  if (!is.null(second)) {
    second <- format_second(second)
    body <- glue(body, ":", second)
  }

  if (!is.null(nanos)) {
    # At least nanos, add "." between hms and nanos
    body <- glue(body, ".")

    nanos <- format_nanos(nanos)
    body <- glue(body, nanos)
  }

  body
}
