#' @export
localize <- function(x) {
  if (is_Date(x)) {
    localize_date(x)
  } else if (is_POSIXct(x) || is_POSIXlt(x)) {
    localize_posixt(x)
  } else {
    stop_civil_unsupported_class(x)
  }
}

localize_date <- function(x) {
  names <- names(x)
  days <- date_to_days(x)
  new_local_date(days, names = names)
}

localize_posixt <- function(x) {
  x <- to_posixct(x)

  names <- names(x)
  seconds <- unstructure(x)
  zone <- get_zone(x)

  fields <- convert_seconds_to_days_and_time_of_day(seconds, zone)

  new_local_datetime_from_fields(fields, names)
}

#' @export
unlocalize <- function(x, ...) {
  restrict_local(x)
  UseMethod("unlocalize")
}

#' @export
unlocalize.civil_local_date <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  days_to_date(days, names(x))
}

#' @export
unlocalize.civil_local_datetime <- function(x,
                                            zone,
                                            ...,
                                            dst_nonexistent = "roll-forward",
                                            dst_ambiguous = "earliest") {
  check_dots_empty()

  if (missing(zone)) {
    abort(paste0(
      "In `unlocalize()`, `zone` is missing. ",
      "This argument is required with 'local_datetime' input."
    ))
  }

  zone <- zone_standardize(zone)

  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")

  seconds <- convert_days_and_time_of_day_to_seconds(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  names(seconds) <- names(x)

  new_datetime(seconds, zone)
}
