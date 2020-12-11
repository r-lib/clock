# Conversion to zoned
# ------------------------------------------------------------------------------

#' @export
as_zoned <- function(x, ...) {
  restrict_civil_supported(x)
  UseMethod("as_zoned")
}

#' @export
as_zoned.civil_zoned <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
as_zoned.civil_naive_gregorian <- function(x,
                                           zone,
                                           ...,
                                           dst_nonexistent = "roll-forward",
                                           dst_ambiguous = "earliest") {
  if (is_at_least_naive_nano_datetime(x)) {
    as_zoned_nano_datetime(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
  } else {
    as_zoned_datetime(x, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
  }
}

#' @export
as_zoned.civil_naive_fiscal <- function(x,
                                        zone,
                                        ...,
                                        dst_nonexistent = "roll-forward",
                                        dst_ambiguous = "earliest") {
  # Leaving room for potential `civil_zoned_fiscal` types,
  # such as a `civil_zoned_fiscal_year_quarter_day_time`. We would automatically
  # convert to those through `as_zoned()` rather than to the gregorian zoned-datetime.
  message <- paste0(
    "Can't automatically convert directly from naive fiscal calendar to zoned gregarian calendar. ",
    "Be more specific with `as_zoned_datetime()` or `as_zoned_nano_datetime()`."
  )
  abort(message)
}

#' @export
as_zoned.civil_naive_iso <- function(x,
                                     zone,
                                     ...,
                                     dst_nonexistent = "roll-forward",
                                     dst_ambiguous = "earliest") {
  # Leaving room for potential `civil_zoned_iso` types,
  # such as a `civil_zoned_iso_year_weeknum_weekday_time`. We would automatically
  # convert to those through `as_zoned()` rather than to the gregorian zoned-datetime.
  message <- paste0(
    "Can't automatically convert directly from naive iso calendar to zoned gregarian calendar. ",
    "Be more specific with `as_zoned_datetime()` or `as_zoned_nano_datetime()`."
  )
  abort(message)
}

#' @export
as_zoned.Date <- function(x, ...) {
  check_dots_empty()
  as_zoned_datetime(x)
}

#' @export
as_zoned.POSIXt <- function(x, ...) {
  check_dots_empty()
  as_zoned_datetime(x)
}
