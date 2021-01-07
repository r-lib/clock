#' @export
as_zoned_time_point.Date <- function(x, ..., precision = "second") {
  x <- to_posixct(x)
  as_zoned_time_point(x, ..., precision = precision)
}

#' @export
as_zoned_time_point.POSIXt <- function(x, ..., precision = "second") {
  check_dots_empty()

  x <- to_posixct(x)

  names <- names(x)
  seconds <- unstructure(x)
  zone <- get_zone(x)

  fields <- convert_zoned_seconds_to_zoned_second_point_fields(seconds)

  days <- fields$days
  seconds_of_day <- fields$seconds_of_day

  calendar <- new_year_month_day(days)

  validate_precision(precision)
  if (is_subsecond_precision(precision)) {
    nanoseconds_of_second <- nanoseconds_of_second_init(days)
  } else {
    nanoseconds_of_second <- NULL
  }

  new_zoned_time_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    precision = precision,
    zone = zone,
    names = names
  )
}
