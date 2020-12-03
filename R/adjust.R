#' @export
adjust_zone <- function(x,
                        zone,
                        ...,
                        dst_nonexistent = "roll-forward",
                        dst_ambiguous = "earliest") {
  restrict_zoned_or_base(x)
  UseMethod("adjust_zone")
}

#' @export
adjust_zone.Date <- function(x,
                             zone,
                             ...,
                             dst_nonexistent = "roll-forward",
                             dst_ambiguous = "earliest") {
  x <- as_naive(x)
  as.POSIXct(x, tz = zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_zone.POSIXt <- adjust_zone.Date

#' @export
adjust_zone.civil_zoned <- function(x,
                                    zone,
                                    ...,
                                    dst_nonexistent = "roll-forward",
                                    dst_ambiguous = "earliest") {
  x <- as_naive(x)
  as_zoned(x, zone = zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

# ------------------------------------------------------------------------------

#' Adjust the year
#'
#' `adjust_year()` adjusts the year of `x` to `value`.
#'
#' @param x `[Date / POSIXct / POSIXlt / civil]`
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
adjust_year.Date <- function(x, value, ..., day_nonexistent = "last-time") {
  x <- as_naive(x)
  out <- adjust_year(x, value, ..., day_nonexistent = day_nonexistent)
  as.Date(out)
}

#' @rdname adjust_year
#' @export
adjust_year.POSIXt <- function(x,
                               value,
                               ...,
                               day_nonexistent = "last-time",
                               dst_nonexistent = "roll-forward",
                               dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_year(x, value, ..., day_nonexistent = day_nonexistent)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname adjust_year
#' @export
adjust_year.civil_zoned <- function(x,
                                    value,
                                    ...,
                                    day_nonexistent = "last-time",
                                    dst_nonexistent = "roll-forward",
                                    dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_year(x, value, ..., day_nonexistent = day_nonexistent)
  as_zoned(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname adjust_year
#' @export
adjust_year.civil_naive_gregorian <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_year_gregorian_impl(x, value, ..., day_nonexistent = day_nonexistent)
}

#' @rdname adjust_year
#' @export
adjust_year.civil_naive_fiscal <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_year_fiscal_impl(x, value, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @export
adjust_quarter <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_quarter")
}

#' @export
adjust_quarter.civil_naive_fiscal <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_quarter_fiscal_impl(x, value, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @export
adjust_last_day_of_quarter <- function(x, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_last_day_of_quarter")
}

#' @export
adjust_last_day_of_quarter.civil_naive_fiscal <- function(x, ...) {
  adjust_last_day_of_quarter_fiscal_impl(x, ...)
}

# ------------------------------------------------------------------------------

#' @export
adjust_month <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_month")
}

#' @export
adjust_month.Date <- function(x, value, ..., day_nonexistent = "last-time") {
  x <- as_naive(x)
  out <- adjust_month(x, value, ..., day_nonexistent = day_nonexistent)
  as.Date(out)
}

#' @export
adjust_month.POSIXt <- function(x,
                                value,
                                ...,
                                day_nonexistent = "last-time",
                                dst_nonexistent = "roll-forward",
                                dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_month(x, value, ..., day_nonexistent = day_nonexistent)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_month.civil_zoned <- function(x,
                                     value,
                                     ...,
                                     day_nonexistent = "last-time",
                                     dst_nonexistent = "roll-forward",
                                     dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_month(x, value, ..., day_nonexistent = day_nonexistent)
  as_zoned(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_month.civil_naive_gregorian <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_month_gregorian_impl(x, value, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @export
adjust_day <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_day")
}

#' @export
adjust_day.Date <- function(x, value, ..., day_nonexistent = "last-time") {
  x <- as_naive(x)
  out <- adjust_day(x, value, ..., day_nonexistent = day_nonexistent)
  as.Date(out)
}

#' @export
adjust_day.POSIXt <- function(x,
                              value,
                              ...,
                              day_nonexistent = "last-time",
                              dst_nonexistent = "roll-forward",
                              dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_day(x, value, ..., day_nonexistent = day_nonexistent)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_day.civil_zoned <- function(x,
                                   value,
                                   ...,
                                   day_nonexistent = "last-time",
                                   dst_nonexistent = "roll-forward",
                                   dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_day(x, value, ..., day_nonexistent = day_nonexistent)
  as_zoned(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_day.civil_naive_gregorian <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_day_gregorian_impl(x, value, ..., day_nonexistent = day_nonexistent)
}

#' @export
adjust_day.civil_naive_fiscal <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_day_fiscal_impl(x, value, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @export
adjust_last_day_of_month <- function(x, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_last_day_of_month")
}

#' @export
adjust_last_day_of_month.Date <- function(x, ...) {
  x <- as_naive(x)
  out <- adjust_last_day_of_month(x, ...)
  as.Date(out)
}

#' @export
adjust_last_day_of_month.POSIXt <- function(x,
                                            ...,
                                            dst_nonexistent = "roll-forward",
                                            dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_last_day_of_month(x, ...)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_last_day_of_month.civil_zoned <- function(x,
                                                 ...,
                                                 dst_nonexistent = "roll-forward",
                                                 dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_last_day_of_month(x, ...)
  as_zoned(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_last_day_of_month.civil_naive_gregorian <- function(x, ...) {
  adjust_last_day_of_month_gregorian_impl(x, ...)
}

# ------------------------------------------------------------------------------

#' @export
adjust_hour <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_hour")
}

#' @export
adjust_hour.Date <- function(x, value, ...) {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_hour(x, value, ...)
  as.POSIXct(out, tz = zone)
}

#' @export
adjust_hour.POSIXt <- function(x,
                               value,
                               ...,
                               dst_nonexistent = "roll-forward",
                               dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_hour(x, value, ...)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_hour.civil_zoned <- function(x,
                                    value,
                                    ...,
                                    dst_nonexistent = "roll-forward",
                                    dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_hour(x, value, ...)
  as_zoned(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_hour.civil_naive_gregorian <- function(x, value, ...) {
  adjust_hour_gregorian_impl(x, value, ...)
}

# ------------------------------------------------------------------------------

#' @export
adjust_minute <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_minute")
}

#' @export
adjust_minute.Date <- function(x, value, ...) {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_minute(x, value, ...)
  as.POSIXct(out, tz = zone)
}

#' @export
adjust_minute.POSIXt <- function(x,
                                 value,
                                 ...,
                                 dst_nonexistent = "roll-forward",
                                 dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_minute(x, value, ...)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_minute.civil_zoned <- function(x,
                                      value,
                                      ...,
                                      dst_nonexistent = "roll-forward",
                                      dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_minute(x, value, ...)
  as_zoned(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_minute.civil_naive_gregorian <- function(x, value, ...) {
  adjust_minute_gregorian_impl(x, value, ...)
}

# ------------------------------------------------------------------------------

#' @export
adjust_second <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_second")
}

#' @export
adjust_second.Date <- function(x, value, ...) {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_second(x, value, ...)
  as.POSIXct(out, tz = zone)
}

#' @export
adjust_second.POSIXt <- function(x,
                                 value,
                                 ...,
                                 dst_nonexistent = "roll-forward",
                                 dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_second(x, value, ...)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_second.civil_zoned <- function(x,
                                      value,
                                      ...,
                                      dst_nonexistent = "roll-forward",
                                      dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_second(x, value, ...)
  as_zoned(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_second.civil_naive_gregorian <- function(x, value, ...) {
  adjust_second_gregorian_impl(x, value, ...)
}

# ------------------------------------------------------------------------------

#' @export
adjust_nanosecond <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_nanosecond")
}

#' @export
adjust_nanosecond.Date <- function(x, value, ...) {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- adjust_nanosecond(x, value, ...)
  as_zoned(out, zone = zone)
}

#' @export
adjust_nanosecond.POSIXt <- adjust_nanosecond.Date

#' @export
adjust_nanosecond.civil_zoned <- adjust_nanosecond.Date

#' @export
adjust_nanosecond.civil_naive_gregorian <- function(x, value, ...) {
  adjust_nanosecond_gregorian_impl(x, value, ...)
}

# ------------------------------------------------------------------------------

adjust_year_gregorian_impl <- function(x, value, ..., day_nonexistent) {
  x <- promote_at_least_year(x)
  adjust_naive_gregorian_days(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "year")
}
adjust_month_gregorian_impl <- function(x, value, ..., day_nonexistent) {
  x <- promote_at_least_year_month(x)
  adjust_naive_gregorian_days(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "month")
}
adjust_day_gregorian_impl <- function(x, value, ..., day_nonexistent) {
  x <- promote_at_least_year_month_day(x)
  adjust_naive_gregorian_days(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "day")
}
adjust_last_day_of_month_gregorian_impl <- function(x, ...) {
  x <- promote_at_least_year_month_day(x)
  adjust_naive_gregorian_days(x, -1L, ..., day_nonexistent = "last-time", adjuster = "last_day_of_month")
}

adjust_naive_gregorian_days <- function(x, value, ..., day_nonexistent, adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  adjust_naive_gregorian_days_cpp(x, value, day_nonexistent, size, adjuster)
}

# ------------------------------------------------------------------------------

adjust_hour_gregorian_impl <- function(x, value, ...) {
  adjust_naive_gregorian_time_of_day(x, value, ..., adjuster = "hour")
}
adjust_minute_gregorian_impl <- function(x, value, ...) {
  adjust_naive_gregorian_time_of_day(x, value, ..., adjuster = "minute")
}
adjust_second_gregorian_impl <- function(x, value, ...) {
  adjust_naive_gregorian_time_of_day(x, value, ..., adjuster = "second")
}

adjust_naive_gregorian_time_of_day <- function(x, value, ..., adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  x <- promote_at_least_naive_datetime(x)

  adjust_naive_gregorian_time_of_day_cpp(x, value, size, adjuster)
}

# ------------------------------------------------------------------------------

adjust_nanosecond_gregorian_impl <- function(x, value, ...) {
  adjust_naive_gregorian_nanos_of_second(x, value, ..., adjuster = "nanosecond")
}

adjust_naive_gregorian_nanos_of_second <- function(x, value, ..., adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  x <- promote_at_least_naive_nano_datetime(x)

  adjust_naive_gregorian_nanos_of_second_cpp(x, value, size, adjuster)
}

# ------------------------------------------------------------------------------

adjust_year_fiscal_impl <- function(x, value, ..., day_nonexistent) {
  x <- promote_at_least_fiscal_year(x)
  adjust_naive_fiscal_days(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "year")
}
adjust_quarter_fiscal_impl <- function(x, value, ..., day_nonexistent) {
  x <- promote_at_least_fiscal_year_quarter(x)
  adjust_naive_fiscal_days(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "quarter")
}
adjust_day_fiscal_impl <- function(x, value, ..., day_nonexistent) {
  x <- promote_at_least_fiscal_year_quarter_day(x)
  adjust_naive_fiscal_days(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "day")
}
adjust_last_day_of_quarter_fiscal_impl <- function(x, ...) {
  x <- promote_at_least_fiscal_year_quarter_day(x)
  adjust_naive_fiscal_days(x, -1L, ..., day_nonexistent = "last-time", adjuster = "last_day_of_quarter")
}

adjust_naive_fiscal_days <- function(x, value, ..., day_nonexistent, adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)
  fiscal_start <- get_fiscal_start(x)

  adjust_naive_fiscal_days_cpp(x, value, fiscal_start, day_nonexistent, size, adjuster)
}
