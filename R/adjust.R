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
adjust_zone.clock_zoned_time_point <- function(x,
                                               zone,
                                               ...,
                                               dst_nonexistent = "roll-forward",
                                               dst_ambiguous = "earliest") {
  x <- as_naive(x)
  as_zoned(x, zone = zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

# ------------------------------------------------------------------------------

#' @export
adjust_precision <- function(x, precision) {
  UseMethod("adjust_precision")
}

#' @export
adjust_precision.clock_naive_time_point <- function(x, precision) {
  validate_precision(precision)

  nanoseconds_of_second <- adjust_nanoseconds_of_second_precision(x, precision)

  new_naive_time_point(
    calendar = field_calendar(x),
    seconds_of_day = field_seconds_of_day(x),
    nanoseconds_of_second = nanoseconds_of_second,
    precision = precision,
    names = names(x)
  )
}

#' @export
adjust_precision.clock_zoned_time_point <- function(x, precision) {
  validate_precision(precision)

  nanoseconds_of_second <- adjust_nanoseconds_of_second_precision(x, precision)

  new_zoned_time_point(
    calendar = field_calendar(x),
    seconds_of_day = field_seconds_of_day(x),
    nanoseconds_of_second = nanoseconds_of_second,
    precision = precision,
    zone = get_zone(x),
    names = names(x)
  )
}

adjust_nanoseconds_of_second_precision <- function(x, precision) {
  x_precision <- get_precision(x)

  # Requesting 'second' precision nukes `nanoseconds_of_second`
  if (!is_subsecond_precision(precision)) {
    return(NULL)
  }

  # No nanoseconds to begin with, but we need them
  if (!is_subsecond_precision(x_precision)) {
    return(nanoseconds_of_second_init(x))
  }

  nanoseconds_of_second <- field_nanoseconds_of_second(x)

  if (precision_value(x_precision) > precision_value(precision)) {
    nanoseconds_of_second <- downcast_nanoseconds_of_second_precision(nanoseconds_of_second, precision)
  }

  nanoseconds_of_second
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
  restrict_clock_supported(x)
  UseMethod("adjust_year")
}

#' @rdname adjust_year
#' @export
adjust_year.Date <- function(x, value, ..., day_nonexistent = "last-time") {
  x <- as_year_month_day(x)
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
  x <- as_naive_time_point(x)
  out <- adjust_year(x, value, ..., day_nonexistent = day_nonexistent)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname adjust_year
#' @export
adjust_year.clock_zoned_time_point <- function(x,
                                               value,
                                               ...,
                                               day_nonexistent = "last-time",
                                               dst_nonexistent = "roll-forward",
                                               dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_year(x, value, ..., day_nonexistent = day_nonexistent)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname adjust_year
#' @export
adjust_year.clock_naive_time_point <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_naive_time_point_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = adjust_year_calendar)
}

#' @rdname adjust_year
#' @export
adjust_year.clock_calendar <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = adjust_year_calendar)
}

adjust_year_calendar <- function(x, value, ..., day_nonexistent) {
  check_dots_empty()
  UseMethod("adjust_year_calendar")
}

#' @export
adjust_year_calendar.clock_calendar <- function(x, value, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("adjust_year")
}

#' @export
adjust_year_calendar.clock_gregorian <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_year(x)
  adjust_gregorian_calendar(x, value, day_nonexistent, "year")
}

#' @export
adjust_year_calendar.clock_quarterly <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_year(x)
  start <- get_start(x)
  adjust_quarterly_calendar(x, value, start, day_nonexistent, "year")
}

#' @export
adjust_year_calendar.clock_iso <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_year(x)
  adjust_iso_calendar(x, value, day_nonexistent, "year")
}

# ------------------------------------------------------------------------------

#' @export
adjust_quarternum <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_quarternum")
}

#' @export
adjust_quarternum.clock_zoned_time_point <- function(x,
                                                     value,
                                                     ...,
                                                     day_nonexistent = "last-time",
                                                     dst_nonexistent = "roll-forward",
                                                     dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_quarternum(x, value, ..., day_nonexistent = day_nonexistent)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_quarternum.clock_naive_time_point <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_naive_time_point_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = adjust_quarternum_calendar)
}

#' @export
adjust_quarternum.clock_calendar <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = adjust_quarternum_calendar)
}

adjust_quarternum_calendar <- function(x, value, ..., day_nonexistent) {
  check_dots_empty()
  UseMethod("adjust_quarternum_calendar")
}

#' @export
adjust_quarternum_calendar.clock_calendar <- function(x, value, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("adjust_quarternum")
}

#' @export
adjust_quarternum_calendar.clock_quarterly <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_quarter(x)
  start <- get_start(x)
  adjust_quarterly_calendar(x, value, start, day_nonexistent, "quarternum")
}

# ------------------------------------------------------------------------------

#' @export
adjust_quarterday <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_quarterday")
}

#' @export
adjust_quarterday.clock_zoned_time_point <- function(x,
                                                     value,
                                                     ...,
                                                     day_nonexistent = "last-time",
                                                     dst_nonexistent = "roll-forward",
                                                     dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_quarterday(x, value, ..., day_nonexistent = day_nonexistent)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_quarterday.clock_naive_time_point <- function(x, value, ..., day_nonexistent = "last-time") {
  if (identical(value, "last")) {
    value <- -1L
    dispatcher <- adjust_quarterday_last_calendar
  } else {
    dispatcher <- adjust_quarterday_calendar
  }

  adjust_naive_time_point_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = dispatcher)
}

#' @export
adjust_quarterday.clock_calendar <- function(x, value, ..., day_nonexistent = "last-time") {
  if (identical(value, "last")) {
    value <- -1L
    dispatcher <- adjust_quarterday_last_calendar
  } else {
    dispatcher <- adjust_quarterday_calendar
  }

  adjust_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = dispatcher)
}

adjust_quarterday_calendar <- function(x, value, ..., day_nonexistent) {
  check_dots_empty()
  UseMethod("adjust_quarterday_calendar")
}

#' @export
adjust_quarterday_calendar.clock_calendar <- function(x, value, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("adjust_quarterday")
}

#' @export
adjust_quarterday_calendar.clock_quarterly <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_day(x)
  start <- get_start(x)
  adjust_quarterly_calendar(x, value, start, day_nonexistent, "quarterday")
}

adjust_quarterday_last_calendar <- function(x, value, ..., day_nonexistent) {
  check_dots_empty()
  UseMethod("adjust_quarterday_last_calendar")
}

#' @export
adjust_quarterday_last_calendar.clock_calendar <- function(x, value, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("adjust_quarterday_last")
}

#' @export
adjust_quarterday_last_calendar.clock_quarterly <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_day(x)
  start <- get_start(x)
  adjust_quarterly_calendar(x, value, start, day_nonexistent, "last_day_of_quarter")
}

# ------------------------------------------------------------------------------

#' @export
adjust_month <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_month")
}

#' @export
adjust_month.Date <- function(x, value, ..., day_nonexistent = "last-time") {
  x <- as_year_month_day(x)
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
  x <- as_naive_time_point(x)
  out <- adjust_month(x, value, ..., day_nonexistent = day_nonexistent)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_month.clock_zoned_time_point <- function(x,
                                                value,
                                                ...,
                                                day_nonexistent = "last-time",
                                                dst_nonexistent = "roll-forward",
                                                dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_month(x, value, ..., day_nonexistent = day_nonexistent)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_month.clock_naive_time_point <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_naive_time_point_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = adjust_month_calendar)
}

#' @export
adjust_month.clock_calendar <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = adjust_month_calendar)
}

adjust_month_calendar <- function(x, value, ..., day_nonexistent) {
  check_dots_empty()
  UseMethod("adjust_month_calendar")
}

#' @export
adjust_month_calendar.clock_calendar <- function(x, value, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("adjust_month")
}

#' @export
adjust_month_calendar.clock_gregorian <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_month(x)
  adjust_gregorian_calendar(x, value, day_nonexistent, "month")
}

# ------------------------------------------------------------------------------

#' @export
adjust_weeknum <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_weeknum")
}

#' @export
adjust_weeknum.clock_zoned_time_point <- function(x,
                                                  value,
                                                  ...,
                                                  day_nonexistent = "last-time",
                                                  dst_nonexistent = "roll-forward",
                                                  dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_weeknum(x, value, ..., day_nonexistent = day_nonexistent)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_weeknum.clock_naive_time_point <- function(x, value, ..., day_nonexistent = "last-time") {
  if (identical(value, "last")) {
    value <- -1L
    dispatcher <- adjust_weeknum_last_calendar
  } else {
    dispatcher <- adjust_weeknum_calendar
  }

  adjust_naive_time_point_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = dispatcher)
}

#' @export
adjust_weeknum.clock_calendar <- function(x, value, ..., day_nonexistent = "last-time") {
  if (identical(value, "last")) {
    value <- -1L
    dispatcher <- adjust_weeknum_last_calendar
  } else {
    dispatcher <- adjust_weeknum_calendar
  }

  adjust_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = dispatcher)
}

adjust_weeknum_calendar <- function(x, value, ..., day_nonexistent) {
  check_dots_empty()
  UseMethod("adjust_weeknum_calendar")
}

#' @export
adjust_weeknum_calendar.clock_calendar <- function(x, value, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("adjust_weeknum")
}

#' @export
adjust_weeknum_calendar.clock_iso <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_week(x)
  adjust_iso_calendar(x, value, day_nonexistent, "weeknum")
}

adjust_weeknum_last_calendar <- function(x, value, ..., day_nonexistent) {
  check_dots_empty()
  UseMethod("adjust_weeknum_last_calendar")
}

#' @export
adjust_weeknum_last_calendar.clock_calendar <- function(x, value, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("adjust_weeknum")
}

#' @export
adjust_weeknum_last_calendar.clock_iso <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_week(x)
  adjust_iso_calendar(x, value, day_nonexistent, "last_weeknum_of_year")
}

# ------------------------------------------------------------------------------

#' @export
adjust_weekday <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_weekday")
}

#' @export
adjust_weekday.clock_zoned_time_point <- function(x,
                                                  value,
                                                  ...,
                                                  day_nonexistent = "last-time",
                                                  dst_nonexistent = "roll-forward",
                                                  dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_weekday(x, value, ..., day_nonexistent = day_nonexistent)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_weekday.clock_naive_time_point <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_naive_time_point_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = adjust_weekday_calendar)
}

#' @export
adjust_weekday.clock_calendar <- function(x, value, ..., day_nonexistent = "last-time") {
  adjust_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = adjust_weekday_calendar)
}

adjust_weekday_calendar <- function(x, value, ..., day_nonexistent) {
  check_dots_empty()
  UseMethod("adjust_weekday_calendar")
}

#' @export
adjust_weekday_calendar.clock_calendar <- function(x, value, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("adjust_weekday")
}

#' @export
adjust_weekday_calendar.clock_iso <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_day(x)
  adjust_iso_calendar(x, value, day_nonexistent, "weekday")
}

# ------------------------------------------------------------------------------

#' @export
adjust_day <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_day")
}

#' @export
adjust_day.Date <- function(x, value, ..., day_nonexistent = "last-time") {
  x <- as_year_month_day(x)
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
  x <- as_naive_time_point(x)
  out <- adjust_day(x, value, ..., day_nonexistent = day_nonexistent)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_day.clock_zoned_time_point <- function(x,
                                              value,
                                              ...,
                                              day_nonexistent = "last-time",
                                              dst_nonexistent = "roll-forward",
                                              dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_day(x, value, ..., day_nonexistent = day_nonexistent)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_day.clock_naive_time_point <- function(x, value, ..., day_nonexistent = "last-time") {
  if (identical(value, "last")) {
    value <- -1L
    dispatcher <- adjust_day_last_calendar
  } else {
    dispatcher <- adjust_day_calendar
  }

  adjust_naive_time_point_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = dispatcher)
}

#' @export
adjust_day.clock_calendar <- function(x, value, ..., day_nonexistent = "last-time") {
  if (identical(value, "last")) {
    value <- -1L
    dispatcher <- adjust_day_last_calendar
  } else {
    dispatcher <- adjust_day_calendar
  }

  adjust_calendar(x, value, ..., day_nonexistent = day_nonexistent, dispatcher = dispatcher)
}

adjust_day_calendar <- function(x, value, ..., day_nonexistent) {
  check_dots_empty()
  UseMethod("adjust_day_calendar")
}

#' @export
adjust_day_calendar.clock_calendar <- function(x, value, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("adjust_day")
}

#' @export
adjust_day_calendar.clock_gregorian <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_day(x)
  adjust_gregorian_calendar(x, value, day_nonexistent, "day")
}

adjust_day_last_calendar <- function(x, value, ..., day_nonexistent) {
  check_dots_empty()
  UseMethod("adjust_day_last_calendar")
}

#' @export
adjust_day_last_calendar.clock_calendar <- function(x, value, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("adjust_day")
}

#' @export
adjust_day_last_calendar.clock_gregorian <- function(x, value, ..., day_nonexistent) {
  x <- promote_precision_day(x)
  adjust_gregorian_calendar(x, value, day_nonexistent, "last_day_of_month")
}

# ------------------------------------------------------------------------------

#' @export
adjust_hour <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_hour")
}

#' @export
adjust_hour.Date <- function(x, value, ...) {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
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
  x <- as_naive_time_point(x)
  out <- adjust_hour(x, value, ...)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_hour.clock_zoned_time_point <- function(x,
                                               value,
                                               ...,
                                               dst_nonexistent = "roll-forward",
                                               dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_hour(x, value, ...)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_hour.clock_naive_time_point <- function(x, value, ...) {
  adjust_naive_time_point_seconds_of_day(x, value, ..., adjuster = "hour")
}

#' @export
adjust_hour.clock_calendar <- function(x, value, ...) {
  x <- as_naive_time_point(x)
  adjust_hour(x, value, ...)
}

# ------------------------------------------------------------------------------

#' @export
adjust_minute <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_minute")
}

#' @export
adjust_minute.Date <- function(x, value, ...) {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
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
  x <- as_naive_time_point(x)
  out <- adjust_minute(x, value, ...)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_minute.clock_zoned_time_point <- function(x,
                                               value,
                                               ...,
                                               dst_nonexistent = "roll-forward",
                                               dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_minute(x, value, ...)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_minute.clock_naive_time_point <- function(x, value, ...) {
  adjust_naive_time_point_seconds_of_day(x, value, ..., adjuster = "minute")
}

#' @export
adjust_minute.clock_calendar <- function(x, value, ...) {
  x <- as_naive_time_point(x)
  adjust_minute(x, value, ...)
}

# ------------------------------------------------------------------------------

#' @export
adjust_second <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_second")
}

#' @export
adjust_second.Date <- function(x, value, ...) {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
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
  x <- as_naive_time_point(x)
  out <- adjust_second(x, value, ...)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_second.clock_zoned_time_point <- function(x,
                                                 value,
                                                 ...,
                                                 dst_nonexistent = "roll-forward",
                                                 dst_ambiguous = "earliest") {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_second(x, value, ...)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
adjust_second.clock_naive_time_point <- function(x, value, ...) {
  adjust_naive_time_point_seconds_of_day(x, value, ..., adjuster = "second")
}

#' @export
adjust_second.clock_calendar <- function(x, value, ...) {
  x <- as_naive_time_point(x)
  adjust_second(x, value, ...)
}

# ------------------------------------------------------------------------------

#' @export
adjust_millisecond <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_millisecond")
}

#' @export
adjust_millisecond.Date <- function(x, value, ...) {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_millisecond(x, value, ...)
  as_zoned_time_point(out, zone = zone)
}

#' @export
adjust_millisecond.POSIXt <- adjust_millisecond.Date

#' @export
adjust_millisecond.clock_zoned_time_point <- adjust_millisecond.Date

#' @export
adjust_millisecond.clock_naive_time_point <- function(x, value, ...) {
  x <- promote_precision_millisecond(x)
  adjust_naive_time_point_nanoseconds_of_second(x, value, ..., adjuster = "millisecond")
}

#' @export
adjust_millisecond.clock_calendar <- function(x, value, ...) {
  x <- as_naive_time_point(x)
  adjust_millisecond(x, value, ...)
}

# ------------------------------------------------------------------------------

#' @export
adjust_microsecond <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_microsecond")
}

#' @export
adjust_microsecond.Date <- function(x, value, ...) {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_microsecond(x, value, ...)
  as_zoned_time_point(out, zone = zone)
}

#' @export
adjust_microsecond.POSIXt <- adjust_microsecond.Date

#' @export
adjust_microsecond.clock_zoned_time_point <- adjust_microsecond.Date

#' @export
adjust_microsecond.clock_naive_time_point <- function(x, value, ...) {
  x <- promote_precision_microsecond(x)
  adjust_naive_time_point_nanoseconds_of_second(x, value, ..., adjuster = "microsecond")
}

#' @export
adjust_microsecond.clock_calendar <- function(x, value, ...) {
  x <- as_naive_time_point(x)
  adjust_microsecond(x, value, ...)
}

# ------------------------------------------------------------------------------

#' @export
adjust_nanosecond <- function(x, value, ...) {
  restrict_clock_supported(x)
  UseMethod("adjust_nanosecond")
}

#' @export
adjust_nanosecond.Date <- function(x, value, ...) {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- adjust_nanosecond(x, value, ...)
  as_zoned_time_point(out, zone = zone)
}

#' @export
adjust_nanosecond.POSIXt <- adjust_nanosecond.Date

#' @export
adjust_nanosecond.clock_zoned_time_point <- adjust_nanosecond.Date

#' @export
adjust_nanosecond.clock_naive_time_point <- function(x, value, ...) {
  x <- promote_precision_nanosecond(x)
  adjust_naive_time_point_nanoseconds_of_second(x, value, ..., adjuster = "nanosecond")
}

#' @export
adjust_nanosecond.clock_calendar <- function(x, value, ...) {
  x <- as_naive_time_point(x)
  adjust_nanosecond(x, value, ...)
}

# ------------------------------------------------------------------------------

adjust_naive_time_point_nanoseconds_of_second <- function(x, value, ..., adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")

  args <- vec_recycle_common(x = x, value = value)
  x <- args$x
  value <- args$value

  adjust_naive_time_point_nanoseconds_of_second_cpp(x, value, adjuster)
}

# ------------------------------------------------------------------------------

adjust_naive_time_point_seconds_of_day <- function(x, value, ..., adjuster) {
  check_dots_empty()

  x <- promote_precision_second(x)

  value <- vec_cast(value, integer(), x_arg = "value")

  args <- vec_recycle_common(x = x, value = value)
  x <- args$x
  value <- args$value

  adjust_naive_time_point_seconds_of_day_cpp(x, value, adjuster)
}

# ------------------------------------------------------------------------------

adjust_naive_time_point_calendar <- function(x, value, ..., day_nonexistent, dispatcher) {
  value <- vec_cast(value, integer(), x_arg = "value")

  args <- vec_recycle_common(x = x, value = value)
  x <- args$x
  value <- args$value

  calendar <- field_calendar(x)
  result <- dispatcher(calendar, value, ..., day_nonexistent = day_nonexistent)
  calendar <- result$calendar
  x <- set_calendar(x, calendar)

  if (result$any) {
    ok <- result$ok

    seconds_of_day <- field_seconds_of_day(x)
    seconds_of_day <- resolve_seconds_of_day(seconds_of_day, ok, day_nonexistent)
    x <- set_seconds_of_day(x, seconds_of_day)

    if (is_subsecond_time_point(x)) {
      precision <- get_precision(x)
      nanoseconds_of_second <- field_nanoseconds_of_second(x)
      nanoseconds_of_second <- resolve_nanoseconds_of_second(nanoseconds_of_second, ok, day_nonexistent, precision)
      x <- set_nanoseconds_of_second(x, nanoseconds_of_second)
    }
  }

  x
}

# ------------------------------------------------------------------------------

adjust_calendar <- function(x, value, ..., day_nonexistent, dispatcher) {
  value <- vec_cast(value, integer(), x_arg = "value")

  args <- vec_recycle_common(x = x, value = value)
  x <- args$x
  value <- args$value

  result <- dispatcher(x, value, ..., day_nonexistent = day_nonexistent)

  result$calendar
}
