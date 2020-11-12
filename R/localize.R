#' @export
localize <- function(x) {
  restrict_civil_supported(x)
  UseMethod("localize")
}

#' @export
localize.Date <- function(x) {
  as_local_date(x)
}

#' @export
localize.POSIXt <- function(x) {
  as_local_datetime(x)
}

#' @export
localize.civil_zoned_nano_datetime <- function(x) {
  as_local_nano_datetime(x)
}

#' @export
localize.civil_local <- function(x) {
  x
}

# ------------------------------------------------------------------------------

#' @export
unlocalize <- function(x, ...) {
  restrict_local(x)
  UseMethod("unlocalize")
}

#' @export
unlocalize.civil_local_year_month <- function(x, ...) {
  unlocalize_to_date(x, ...)
}

#' @export
unlocalize.civil_local_date <- function(x, ...) {
  unlocalize_to_date(x, ...)
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

  seconds <- convert_local_days_and_time_of_day_to_sys_seconds(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  names(seconds) <- names(x)

  new_datetime(seconds, zone)
}

#' @export
unlocalize.civil_local_nano_datetime <- function(x,
                                                 zone,
                                                 ...,
                                                 dst_nonexistent = "roll-forward",
                                                 dst_ambiguous = "earliest") {
  if (missing(zone)) {
    abort(paste0(
      "In `unlocalize()`, `zone` is missing. ",
      "This argument is required with 'local_nano_datetime' input."
    ))
  }

  as_zoned_nano_datetime(
    x = x,
    ...,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

unlocalize_to_date <- function(x, ...) {
  check_dots_empty()
  days <- field(x, "days")
  days_to_date(days, names(x))
}
