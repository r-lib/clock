#' Get or set the time zone
#'
#' @description
#' `r lifecycle::badge("deprecated")`
#'
#' - `date_zone()` is deprecated in favor of [date_time_zone()].
#'
#' - `date_set_zone()` is deprecated in favor of [date_time_set_zone()].
#'
#' @inheritParams date_time_zone
#'
#' @keywords internal
#' @name date-zone
NULL

#' @rdname date-zone
#' @export
date_zone <- function(x) {
  # - soft-deprecated: 0.7.0
  lifecycle::deprecate_soft(
    when = "0.7.0",
    what = "date_zone()",
    with = "date_time_zone()"
  )
  UseMethod("date_zone")
}

#' @export
date_zone.Date <- function(x) {
  abort("Can't get the zone of a 'Date'.")
}

#' @export
date_zone.POSIXt <- function(x) {
  posixt_tzone(x)
}

#' @rdname date-zone
#' @export
date_set_zone <- function(x, zone) {
  # - soft-deprecated: 0.7.0
  lifecycle::deprecate_soft(
    when = "0.7.0",
    what = "date_set_zone()",
    with = "date_time_set_zone()"
  )
  UseMethod("date_set_zone")
}

#' @export
date_set_zone.Date <- function(x, zone) {
  abort("Can't set the zone of a 'Date'.")
}

#' @export
date_set_zone.POSIXt <- function(x, zone) {
  x <- to_posixct(x)
  check_zone(zone)
  posixt_set_tzone(x, zone)
}
