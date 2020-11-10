#' Adjust the year
#'
#' `adjust_year()` adjusts the year of `x` to `value`.
#'
#' @param x `[Date / POSIXct / POSIXlt / local_datetime]`
#'
#'   A date-time vector.
#'
#' @param value `[integer]`
#'
#'   An integer vector containing the value to adjust to.
#'
#' @param day_nonexistent `[character(1)]`
#'
#'   Control the behavior when a nonexistent day is generated. This only happens
#'   when adjusting years, months, or days.
#'
#'   - `"last-time"`: Adjust to the last possible time of the current month.
#'
#'   - `"first-time"`: Adjust to the first possible time of the following month.
#'
#'   - `"last-day"`: Adjust to the last day of the current month. For
#'     date-times, the sub-daily components are kept.
#'
#'   - `"first-day"`: Adjust to the first day of the following month. For
#'     date-times, the sub-daily components are kept.
#'
#'   - `"NA"`: Replace the nonexistent date with `NA`.
#'
#'   - `"error"`: Error on nonexistent dates.
#'
#' @export
#' @examples
#' x <- as.Date(c("1970-01-01", "1971-01-01"))
#' adjust_year(x, 1972)
#'
#' # Leap day
#' x <- as.Date("1972-02-29")
#'
#' # "1973-02-29" doesn't exist. By default this rolls back to the
#' # the last day in that month.
#' adjust_year(x, 1973)
#'
#' # But you can adjust that behavior
#' adjust_year(x, 1973, day_nonexistent = "first-day")
#' adjust_year(x, 1973, day_nonexistent = "NA")
#'
#' # It is possible to adjust into a daylight savings time gap.
#' # Due to a change in how daylight savings was handled, the Pacific
#' # island of Samoa skipped all of "2011-12-30" and went straight to the
#' # 31st. This is considered a "nonexistent" time due to DST.
#' x <- as.POSIXct("2010-12-30 02:00:00", "Pacific/Apia")
#'
#' # So "2011-12-30 02:00:00" doesn't exist, and by default we "roll forward"
#' # to the next possible time, which is "2011-12-31 00:00:00".
#' adjust_year(x, 2011)
#'
#' # But you can adjust that too
#' adjust_year(x, 2011, dst_nonexistent = "roll-backward")
#' adjust_year(x, 2011, dst_nonexistent = "shift-forward")
adjust_year <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_year")
}

#' @rdname adjust_year
#' @export
adjust_year.Date <- function(x,
                             value,
                             ...,
                             day_nonexistent = "last-time") {
  adjust_date(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "year")
}

#' @rdname adjust_year
#' @export
adjust_year.POSIXt <- function(x,
                               value,
                               ...,
                               day_nonexistent = "last-time",
                               dst_nonexistent = "roll-forward",
                               dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "year"
  )
}

#' @rdname adjust_year
#' @export
adjust_year.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "year")
}

# ------------------------------------------------------------------------------

#' @export
adjust_month <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_month")
}

#' @export
adjust_month.Date <- function(x,
                              value,
                              ...,
                              day_nonexistent = "last-time") {
  adjust_date(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "month")
}

#' @export
adjust_month.POSIXt <- function(x,
                                value,
                                ...,
                                day_nonexistent = "last-time",
                                dst_nonexistent = "roll-forward",
                                dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "month"
  )
}

#' @export
adjust_month.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "month")
}

# ------------------------------------------------------------------------------

#' @export
adjust_day <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_day")
}

#' @export
adjust_day.Date <- function(x,
                            value,
                            ...,
                            day_nonexistent = "last-time") {
  adjust_date(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "day")
}

#' @export
adjust_day.POSIXt <- function(x,
                              value,
                              ...,
                              day_nonexistent = "last-time",
                              dst_nonexistent = "roll-forward",
                              dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "day"
  )
}

#' @export
adjust_day.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "day")
}

# ------------------------------------------------------------------------------

#' @export
adjust_hour <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_hour")
}

#' @export
adjust_hour.Date <- function(x, value, ...) {
  adjust_date(x, value, ..., day_nonexistent = "last-time", adjuster = "hour")
}

#' @export
adjust_hour.POSIXt <- function(x,
                               value,
                               ...,
                               dst_nonexistent = "roll-forward",
                               dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "hour"
  )
}

#' @export
adjust_hour.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "hour")
}

# ------------------------------------------------------------------------------

#' @export
adjust_minute <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_minute")
}

#' @export
adjust_minute.Date <- function(x, value, ...) {
  adjust_date(x, value, ..., day_nonexistent = "last-time", adjuster = "minute")
}

#' @export
adjust_minute.POSIXt <- function(x,
                                 value,
                                 ...,
                                 dst_nonexistent = "roll-forward",
                                 dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "minute"
  )
}

#' @export
adjust_minute.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "minute")
}

# ------------------------------------------------------------------------------

#' @export
adjust_second <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_second")
}

#' @export
adjust_second.Date <- function(x, value, ...) {
  adjust_date(x, value, ..., day_nonexistent = "last-time", adjuster = "second")
}

#' @export
adjust_second.POSIXt <- function(x,
                                 value,
                                 ...,
                                 dst_nonexistent = "roll-forward",
                                 dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "second"
  )
}

#' @export
adjust_second.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "second")
}

# ------------------------------------------------------------------------------

#' @export
adjust_last_day_of_month <- function(x, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_last_day_of_month")
}

#' @export
adjust_last_day_of_month.Date <- function(x, ...) {
  adjust_date(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    day_nonexistent = "last-time",
    adjuster = "last_day_of_month"
  )
}

#' @export
adjust_last_day_of_month.POSIXt <- function(x,
                                            ...,
                                            dst_nonexistent = "roll-forward",
                                            dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "last_day_of_month"
  )
}

#' @export
adjust_last_day_of_month.civil_local <- function(x, ...) {
  adjust_local(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    adjuster = "last_day_of_month"
  )
}

# ------------------------------------------------------------------------------

adjust_zoned <- function(x,
                         value,
                         ...,
                         day_nonexistent,
                         dst_nonexistent,
                         dst_ambiguous,
                         adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  x_ct <- to_posixct(x)

  out <- adjust_zoned_cpp(
    x = x_ct,
    value = value,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size,
    adjuster = adjuster
  )

  if (is_time_based_adjuster(adjuster)) {
    out
  } else {
    from_posixct(out, x)
  }
}

adjust_date <- function(x, value, ..., day_nonexistent, adjuster) {
  adjust_zoned(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = "roll-forward",
    dst_ambiguous = "earliest",
    adjuster = adjuster
  )
}

adjust_posixt <- function(x,
                          value,
                          ...,
                          day_nonexistent,
                          dst_nonexistent,
                          dst_ambiguous,
                          adjuster) {
  adjust_zoned(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = adjuster
  )
}

# ------------------------------------------------------------------------------

adjust_local <- function(x, value, ..., adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  to <- x
  x <- to_local_datetime(x)

  out <- adjust_local_cpp(
    x = x,
    value = value,
    size = size,
    adjuster = adjuster
  )

  if (is_time_based_adjuster(adjuster)) {
    out
  } else {
    from_local_datetime(out, to)
  }
}

# ------------------------------------------------------------------------------

is_time_based_adjuster <- function(adjuster) {
  adjuster %in% time_based_adjusters()
}

time_based_adjusters <- function() {
  c(
    "hour",
    "minute",
    "second"
  )
}

# ------------------------------------------------------------------------------

VALUE_PLACEHOLDER = -1L
