#' @export
as_naive_time_point <- function(x, ...) {
  UseMethod("as_naive_time_point")
}

#' @export
as_naive_time_point.default <- function(x, ...) {
  stop_clock_unsupported_conversion(x, "clock_naive_time_point")
}

#' @export
as_naive_time_point.clock_naive_time_point <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_naive_time_point.clock_calendar <- function(x, ..., precision = "second") {
  check_dots_empty()

  names <- names(x)

  x <- promote_precision_day(x)
  x <- unname(x)

  seconds_of_day <- seconds_of_day_init(x)

  validate_precision(precision)

  if (is_subsecond_precision(precision)) {
    nanoseconds_of_second <- nanoseconds_of_second_init(x)
  } else {
    nanoseconds_of_second <- NULL
  }

  new_naive_time_point(x, seconds_of_day, nanoseconds_of_second, precision, names = names)
}

#' @export
as_naive_time_point.clock_zoned_time_point <- function(x, ...) {
  check_dots_empty()

  zone <- zoned_time_point_zone(x)
  precision <- get_precision(x)

  fields <- convert_second_point_fields_from_zoned_to_naive(
    calendar = field_calendar(x),
    seconds_of_day = field_seconds_of_day(x),
    zone = zone
  )

  if (is_subsecond_precision(precision)) {
    nanos <- list(nanoseconds_of_second = field_nanoseconds_of_second(x))
    fields <- c(fields, nanos)
  }

  new_naive_time_point_from_fields(fields, precision, names = names(x))
}

#' @export
as_naive_time_point.Date <- function(x, ..., precision = "second") {
  x <- as_year_month_day(x)
  as_naive_time_point(x, ..., precision = precision)
}

#' @export
as_naive_time_point.POSIXt <- function(x, ..., precision = "second") {
  check_dots_empty()

  x <- to_posixct(x)

  names <- names(x)
  seconds <- unstructure(x)
  zone <- get_zone(x)

  fields <- convert_zoned_seconds_to_naive_second_point_fields(seconds, zone)

  days <- fields$days
  seconds_of_day <- fields$seconds_of_day

  calendar <- new_year_month_day(days)

  validate_precision(precision)
  if (is_subsecond_precision(precision)) {
    nanoseconds_of_second <- nanoseconds_of_second_init(days)
  } else {
    nanoseconds_of_second <- NULL
  }

  new_naive_time_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    precision = precision,
    names = names
  )
}
