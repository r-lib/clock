#' Date-time arithmetic
#'
#' @description
#' Add or subtract units of time to a date-time object.
#'
#' For many detailed examples and extended explanations, see the vignette
#' on arithmetic: `vignette("civil-arithmetic")`.
#'
#' The following add _periods_ of time. Periods are units of time that do
#' not have a fixed constant duration (i.e. a "month" may be 30 or 31 days).
#'
#' * `add_years()`
#' * `add_months()`
#' * `add_days()`
#'
#' The following add _durations_ of time. Durations are fixed units of time
#' based on a set number of seconds.
#'
#' * `add_hours()`
#' * `add_minutes()`
#' * `add_seconds()`
#'
#' When nonexistent or ambiguous date-times are landed on, the
#' `day_nonexistent`, `dst_nonexistent`, and `dst_ambiguous` options are
#' consulted to resolve any issues.
#'
#' @param x `[Date / POSIXct / POSIXlt / local_datetime]`
#'
#'   A date-time vector.
#'
#' @param n `[integer]`
#'
#'   An integer vector representing the number of units to add to or
#'   subtract from `x`.
#'
#' @param day_nonexistent `[character(1)]`
#'
#'   Control the behavior when a nonexistent day is generated. This only happens
#'   when adding years or months.
#'
#'   One example is adding a month to March 31st, which theoretically lands on
#'   the nonexistent day of April 31st.
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
#'   _Warning_: When used in arithmetic, `"last-day"` and `"first-day"` do not
#'   guarantee that the relative ordering of `x` is maintained.
#'
#' @param dst_nonexistent `[character(1)]`
#'
#'   Control the behavior when a nonexistent time is generated due to a
#'   daylight savings gap. Only applicable for POSIXct and POSIXlt.
#'
#'   One example is adding a day to `"1970-04-25 02:30:00"` in the time zone of
#'   `"America/New_York"`. This lands on the nonexistent time of `"1970-04-26
#'   02:30:00"`, which does not exist due to a daylight savings gap. On this
#'   day, adding a duration of 1 second to `01:59:59` jumps forward to
#'   `03:00:00`.
#'
#'   - `"roll-directional"`:
#'     If `n` is positive, choose `"roll-forward"`.
#'     If `n` is negative, choose `"roll-backward"`.
#'
#'   - `"roll-forward"`: Roll forward to the next valid moment in time.
#'
#'   - `"roll-backward"`: Roll backward to the previous valid moment in time.
#'
#'   - `"shift-directional"`:
#'     If `n` is positive, choose `"shift-forward"`.
#'     If `n` is negative, choose `"shift-backward"`.
#'
#'   - `"shift-forward"`: Shift the nonexistent time forward by the
#'     length of the daylight savings gap (which is usually 1 hour).
#'
#'   - `"shift-backward"`: Shift the nonexistent time backward
#'     by the length of the daylight savings gap (which is usually 1 hour).
#'
#'   - `"NA"`: Replace the nonexistent time with `NA`.
#'
#'   - `error`: Error on nonexistent times.
#'
#'   _Warning_: When used in arithmetic, `"shift-directional"`,
#'   `"shift-forward"`, and `"shift-backward"` do not guarantee that the
#'   relative ordering of `x` is maintained.
#'
#' @param dst_ambiguous `[character(1)]`
#'
#'   Control the behavior when an ambiguous time is generated due to a daylight
#'   savings fallback. Only applicable for POSIXct and POSIXlt.
#'
#'   One example is adding a day to `"1970-10-24 01:30:00"` in the time zone
#'   of `"America/New_York"`. On the 25th, the clock read `01:00:00` two
#'   separate times. One second after `01:59:59 EDT`, clocks "rolled back" to
#'   `01:00:00 EST`. Adding a day to date-time mentioned before lands us in
#'   this ambiguous zone.
#'
#'   - `"directional"`: If `n` is positive, choose `"earliest"`. If `n` is
#'     negative, choose `"latest"`.
#'
#'   - `"earliest"`: Choose the earliest of the two possible ambiguous times.
#'
#'   - `"latest"`: Choose the latest of the two possible ambiguous times.
#'
#'   - `"NA"`: Replace the ambiguous time with `NA`.
#'
#'   - `"error"`: Error on ambiguous times.
#'
#' @param ... These dots are for future extensions and must be empty.
#'
#' @name civil-arithmetic
#'
#' @examples
#' x <- as.Date("2019-01-31")
#'
#' add_days(x, 2)
#'
#' # Adding one month theoretically lands us on
#' # "2019-02-31"
#' # but this doesn't exist, so `day_nonexistent`
#' # is consulted
#' add_months(x, 1)
#' add_months(x, 1, day_nonexistent = "first-time")
#'
#' x <- as.POSIXct("1970-04-25 02:30:00", "America/New_York")
#' y <- as.POSIXct("1970-04-27 02:30:00", "America/New_York")
#'
#' # Adding 1 day to `x` results in the nonexistent time of:
#' # "1970-04-26 02:30:00"
#' # at which point we have to make a decision about how to proceed using
#' # `dst_nonexistent`.
#' #
#' # The default is to use `"roll-directional"`. Since we are adding a positive
#' # number of days, this chooses `"roll-forward"` which rolls forward to the
#' # next valid moment in time.
#' add_days(x, 1)
#'
#' # If we approach from the other side of the gap and subtract 1 day, then
#' # `"roll-directional"` chooses `"roll-backward"`, which rolls backward
#' # to the previous valid moment in time.
#' subtract_days(y, 1)
#'
#' # If you want to force one of these options,
#' # you can set `dst_nonexistent` directly
#' add_days(x, 1, dst_nonexistent = "roll-backward")
#' add_days(x, 1, dst_nonexistent = "NA")
NULL

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_years <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_years")
}

#' @rdname civil-arithmetic
#' @export
add_years.Date <- function(x,
                           n,
                           ...,
                           day_nonexistent = "last-time") {
  add_ymd_to_date(x, n, ..., day_nonexistent = day_nonexistent, unit = "year")
}

#' @rdname civil-arithmetic
#' @export
add_years.POSIXt <- function(x,
                             n,
                             ...,
                             day_nonexistent = "last-time",
                             dst_nonexistent = "roll-directional",
                             dst_ambiguous = "directional") {
  add_ymd_to_posixt(
    x = x,
    n = n,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    unit = "year"
  )
}

#' @rdname civil-arithmetic
#' @export
add_years.civil_local <- function(x, n, ...) {
  add_ym_to_local(x = x, n = n, ..., unit = "year")
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_years <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_years")
}

#' @rdname civil-arithmetic
#' @export
subtract_years.Date <- function(x,
                                n,
                                ...,
                                day_nonexistent = "last-time") {
  add_years(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname civil-arithmetic
#' @export
subtract_years.POSIXt <- function(x,
                                  n,
                                  ...,
                                  day_nonexistent = "last-time",
                                  dst_nonexistent = "roll-directional",
                                  dst_ambiguous = "directional") {
  add_years(
    x = x,
    n = -n,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

#' @rdname civil-arithmetic
#' @export
subtract_years.civil_local <- function(x, n, ...) {
  add_years(x, -n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_months <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_months")
}

#' @rdname civil-arithmetic
#' @export
add_months.Date <- function(x,
                            n,
                            ...,
                            day_nonexistent = "last-time") {
  add_ymd_to_date(x, n, ..., day_nonexistent = day_nonexistent, unit = "month")
}

#' @rdname civil-arithmetic
#' @export
add_months.POSIXt <- function(x,
                              n,
                              ...,
                              day_nonexistent = "last-time",
                              dst_nonexistent = "roll-directional",
                              dst_ambiguous = "directional") {
  add_ymd_to_posixt(
    x = x,
    n = n,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    unit = "month"
  )
}

#' @rdname civil-arithmetic
#' @export
add_months.civil_local <- function(x, n, ...) {
  add_ym_to_local(x = x, n = n, ..., unit = "month")
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_months <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_months")
}

#' @rdname civil-arithmetic
#' @export
subtract_months.Date <- function(x,
                                 n,
                                 ...,
                                 day_nonexistent = "last-time") {
  add_months(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname civil-arithmetic
#' @export
subtract_months.POSIXt <- function(x,
                                   n,
                                   ...,
                                   day_nonexistent = "last-time",
                                   dst_nonexistent = "roll-directional",
                                   dst_ambiguous = "directional") {
  add_months(
    x = x,
    n = -n,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

#' @rdname civil-arithmetic
#' @export
subtract_months.civil_local <- function(x, n, ...) {
  add_months(x, -n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_weeks <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_weeks")
}

#' @rdname civil-arithmetic
#' @export
add_weeks.Date <- function(x, n, ...) {
  add_ymd_to_date(x, n, ..., day_nonexistent = "last-time", unit = "week")
}

#' @rdname civil-arithmetic
#' @export
add_weeks.POSIXt <- function(x,
                             n,
                             ...,
                             dst_nonexistent = "roll-directional",
                             dst_ambiguous = "directional") {
  add_ymd_to_posixt(
    x = x,
    n = n,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    unit = "week"
  )
}

#' @rdname civil-arithmetic
#' @export
add_weeks.civil_local <- function(x,
                                  n,
                                  ...,
                                  day_nonexistent = "last-time") {
  add_dhms_to_local(x = x, n = n, ..., day_nonexistent = day_nonexistent, unit = "week")
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_weeks <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_weeks")
}

#' @rdname civil-arithmetic
#' @export
subtract_weeks.Date <- function(x,
                                n,
                                ...,
                                day_nonexistent = "last-time") {
  add_weeks(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname civil-arithmetic
#' @export
subtract_weeks.POSIXt <- function(x,
                                  n,
                                  ...,
                                  day_nonexistent = "last-time",
                                  dst_nonexistent = "roll-directional",
                                  dst_ambiguous = "directional") {
  add_weeks(
    x = x,
    n = -n,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

#' @rdname civil-arithmetic
#' @export
subtract_weeks.civil_local <- function(x,
                                       n,
                                       ...,
                                       day_nonexistent = "last-time") {
  add_weeks(x, -n, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_days <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_days")
}

#' @rdname civil-arithmetic
#' @export
add_days.Date <- function(x, n, ...) {
  add_ymd_to_date(x, n, ..., day_nonexistent = "last-time", unit = "day")
}

#' @rdname civil-arithmetic
#' @export
add_days.POSIXt <- function(x,
                            n,
                            ...,
                            dst_nonexistent = "roll-directional",
                            dst_ambiguous = "directional") {
  add_ymd_to_posixt(
    x = x,
    n = n,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    unit = "day"
  )
}

#' @rdname civil-arithmetic
#' @export
add_days.civil_local <- function(x,
                                 n,
                                 ...,
                                 day_nonexistent = "last-time") {
  add_dhms_to_local(x = x, n = n, ..., day_nonexistent = day_nonexistent, unit = "day")
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_days <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_days")
}

#' @rdname civil-arithmetic
#' @export
subtract_days.Date <- function(x,
                               n,
                               ...,
                               day_nonexistent = "last-time") {
  add_days(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname civil-arithmetic
#' @export
subtract_days.POSIXt <- function(x,
                                 n,
                                 ...,
                                 day_nonexistent = "last-time",
                                 dst_nonexistent = "roll-directional",
                                 dst_ambiguous = "directional") {
  add_days(
    x = x,
    n = -n,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

#' @rdname civil-arithmetic
#' @export
subtract_days.civil_local <- function(x,
                                      n,
                                      ...,
                                      day_nonexistent = "last-time") {
  add_days(x, -n, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_hours <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_hours")
}

#' @rdname civil-arithmetic
#' @export
add_hours.Date <- function(x, n, ...) {
  add_hms_to_date(x, n, ..., unit = "hour")
}

#' @rdname civil-arithmetic
#' @export
add_hours.POSIXt <- function(x, n, ...) {
  add_hms_to_posixt(x, n, ..., unit = "hour")
}

#' @rdname civil-arithmetic
#' @export
add_hours.civil_local <- function(x,
                                  n,
                                  ...,
                                  day_nonexistent = "last-time") {
  add_dhms_to_local(x, n, ..., day_nonexistent = day_nonexistent, unit = "hour")
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_hours <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_hours")
}

#' @rdname civil-arithmetic
#' @export
subtract_hours.Date <- function(x, n, ...) {
  add_hours(x, -n, ...)
}

#' @rdname civil-arithmetic
#' @export
subtract_hours.POSIXt <- function(x, n, ...) {
  add_hours(x, -n, ...)
}

#' @rdname civil-arithmetic
#' @export
subtract_hours.civil_local <- function(x,
                                       n,
                                       ...,
                                       day_nonexistent = "last-time") {
  add_hours(x, -n, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_minutes <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_minutes")
}

#' @rdname civil-arithmetic
#' @export
add_minutes.Date <- function(x, n, ...) {
  add_hms_to_date(x, n, ..., unit = "minute")
}

#' @rdname civil-arithmetic
#' @export
add_minutes.POSIXt <- function(x, n, ...) {
  add_hms_to_posixt(x, n, ..., unit = "minute")
}

#' @rdname civil-arithmetic
#' @export
add_minutes.civil_local <- function(x,
                                    n,
                                    ...,
                                    day_nonexistent = "last-time") {
  add_dhms_to_local(x, n, ..., day_nonexistent = day_nonexistent, unit = "minute")
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_minutes <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_minutes")
}

#' @rdname civil-arithmetic
#' @export
subtract_minutes.Date <- function(x, n, ...) {
  add_minutes(x, -n, ...)
}

#' @rdname civil-arithmetic
#' @export
subtract_minutes.POSIXt <- function(x, n, ...) {
  add_minutes(x, -n, ...)
}

#' @rdname civil-arithmetic
#' @export
subtract_minutes.civil_local <- function(x,
                                         n,
                                         ...,
                                         day_nonexistent = "last-time") {
  add_minutes(x, -n, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_seconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_seconds")
}

#' @rdname civil-arithmetic
#' @export
add_seconds.Date <- function(x, n, ...) {
  add_hms_to_date(x, n, ..., unit = "second")
}

#' @rdname civil-arithmetic
#' @export
add_seconds.POSIXt <- function(x, n, ...) {
  add_hms_to_posixt(x, n, ..., unit = "second")
}

#' @rdname civil-arithmetic
#' @export
add_seconds.civil_local <- function(x,
                                    n,
                                    ...,
                                    day_nonexistent = "last-time") {
  add_dhms_to_local(x, n, ..., day_nonexistent = day_nonexistent, unit = "second")
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_seconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_seconds")
}

#' @rdname civil-arithmetic
#' @export
subtract_seconds.Date <- function(x, n, ...) {
  add_seconds(x, -n, ...)
}

#' @rdname civil-arithmetic
#' @export
subtract_seconds.POSIXt <- function(x, n, ...) {
  add_seconds(x, -n, ...)
}

#' @rdname civil-arithmetic
#' @export
subtract_seconds.civil_local <- function(x,
                                         n,
                                         ...,
                                         day_nonexistent = "last-time") {
  add_seconds(x, -n, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

add_ymd <- function(x,
                    n,
                    ...,
                    day_nonexistent,
                    dst_nonexistent,
                    dst_ambiguous,
                    unit) {
  if (is_local(x)) {
    add_period_to_local(
      x = x,
      n = n,
      ...,
      day_nonexistent = day_nonexistent,
      unit = unit
    )
  } else {
    add_period_to_zoned(
      x = x,
      n = n,
      ...,
      day_nonexistent = day_nonexistent,
      dst_nonexistent = dst_nonexistent,
      dst_ambiguous = dst_ambiguous,
      unit = unit
    )
  }
}

add_hms <- function(x, n, ..., day_nonexistent, unit) {
  if (is_local(x)) {
    add_period_to_local(
      x = x,
      n = n,
      ...,
      day_nonexistent = day_nonexistent,
      unit = unit
    )
  } else {
    add_duration_to_zoned(
      x = x,
      n = n,
      ...,
      unit = unit
    )
  }
}

# ------------------------------------------------------------------------------

add_period_to_zoned <- function(x,
                                n,
                                ...,
                                day_nonexistent,
                                dst_nonexistent,
                                dst_ambiguous,
                                unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  validate_day_nonexistent(day_nonexistent)
  validate_dst_nonexistent_arithmetic(dst_nonexistent)
  validate_dst_ambiguous_arithmetic(dst_ambiguous)

  x_ct <- to_posixct(x)

  out <- add_period_to_zoned_cpp(
    x = x_ct,
    n = n,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size,
    unit = unit
  )

  from_posixct(out, x)
}

add_ymd_to_date <- function(x,
                            n,
                            ...,
                            day_nonexistent,
                            unit) {
  add_period_to_zoned(
    x = x,
    n = n,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = "roll-directional",
    dst_ambiguous = "directional",
    unit = unit
  )
}
add_ymd_to_posixt <- function(x,
                              n,
                              ...,
                              day_nonexistent,
                              dst_nonexistent,
                              dst_ambiguous,
                              unit) {
  add_period_to_zoned(
    x = x,
    n = n,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    unit = unit
  )
}

# ------------------------------------------------------------------------------

add_duration_to_zoned <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  x_ct <- to_posixct(x)

  out <- add_duration_to_zoned_cpp(
    x = x_ct,
    n = n,
    size = size,
    unit = unit
  )

  out
}

add_hms_to_date <- function(x, n, ..., unit) {
  add_duration_to_zoned(x, n, ..., unit = unit)
}
add_hms_to_posixt <- function(x, n, ..., unit) {
  add_duration_to_zoned(x, n, ..., unit = unit)
}

# ------------------------------------------------------------------------------

add_period_to_local <- function(x, n, ..., day_nonexistent, unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  validate_day_nonexistent(day_nonexistent)

  x_ld <- to_local_datetime(x)

  out <- add_period_to_local_cpp(
    x = x_ld,
    n = n,
    day_nonexistent = day_nonexistent,
    size = size,
    unit = unit
  )

  if (is_time_based_unit(unit)) {
    out
  } else {
    from_local_datetime(out, x)
  }
}

add_ym_to_local <- function(x, n, ..., unit) {
  add_period_to_local(x, n, ..., day_nonexistent = "last-time", unit = unit)
}
add_dhms_to_local <- function(x, n, ..., day_nonexistent, unit) {
  add_period_to_local(x, n, ..., day_nonexistent = day_nonexistent, unit = unit)
}
