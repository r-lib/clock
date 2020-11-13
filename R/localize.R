#' @export
localize <- function(x) {
  restrict_civil_supported(x)
  UseMethod("localize")
}

#' @export
localize.civil_local <- function(x) {
  x
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

# ------------------------------------------------------------------------------

#' @export
unlocalize <- function(x, ...) {
  restrict_civil_supported(x)
  UseMethod("unlocalize")
}

#' @export
unlocalize.civil_local_year_month <- function(x, ...) {
  check_dots_empty()
  as.Date(x, ...)
}

#' @export
unlocalize.civil_local_date <- unlocalize.civil_local_year_month

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

  as.POSIXct(
    x = x,
    tz = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

#' @export
unlocalize.civil_local_nano_datetime <- function(x,
                                                 zone,
                                                 ...,
                                                 dst_nonexistent = "roll-forward",
                                                 dst_ambiguous = "earliest") {
  check_dots_empty()

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

#' @export
unlocalize.Date <- function(x, ...) {
  check_dots_empty()
  x
}

#' @export
unlocalize.POSIXt <- unlocalize.Date

#' @export
unlocalize.civil_zoned <- unlocalize.Date
