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
#'   - `"directional"`: If `n` is positive, choose `"next"`. If `n` is negative,
#'     choose `"previous"`.
#'
#'   - `"next"`: Adjust forward to the next instant in time. This
#'     adjusts minutes and seconds to `HH:00:00`.
#'
#'   - `"previous"`: Adjust backwards to the previous instant in time. This
#'     adjusts minutes and seconds to `HH:59:59`.
#'
#'   - `"directional-shift"`: If `n` is positive, choose `"next-shift"`. If
#'     `n` is negative, choose `"previous-shift"`.
#'
#'   - `"next-shift"`: Adjust by shifting the nonexistent time forward by the
#'     length of the daylight savings gap (which is usually 1 hour).
#'
#'   - `"previous-shift"`: Adjust by shifting the nonexistent time backwards
#'     by the length of the daylight savings gap (which is usually 1 hour).
#'
#'   - `"NA"`: Replace the nonexistent time with `NA`.
#'
#'   - `error`: Error on nonexistent times.
#'
#'   _Warning_: When used in arithmetic, `"directional-shift"`, `"next-shift"`,
#'   and `"previous-shift"` do not guarantee that the relative ordering of `x`
#'   is maintained.
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
#' # The default is to use `"directional"`. Since we are adding a positive
#' # number of days, this chooses `"next"` which adjusts to the next possible
#' # instant in time.
#' add_days(x, 1)
#'
#' # If we approach from the other side of the gap and subtract 1 day, then
#' # `"directional"` chooses `"previous"`, which adjusts to the previous
#' # possible instant in time.
#' subtract_days(y, 1)
#'
#' # If you want to force one of these options,
#' # you can set `dst_nonexistent` directly
#' add_days(x, 1, dst_nonexistent = "previous")
#' add_days(x, 1, dst_nonexistent = "NA")
NULL

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_years <- function(x,
                      n,
                      ...,
                      day_nonexistent = "last-time",
                      dst_nonexistent = "directional",
                      dst_ambiguous = "directional") {
  add_ymd(
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
subtract_years <- function(x,
                           n,
                           ...,
                           day_nonexistent = "last-time",
                           dst_nonexistent = "directional",
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

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_months <- function(x,
                       n,
                       ...,
                       day_nonexistent = "last-time",
                       dst_nonexistent = "directional",
                       dst_ambiguous = "directional") {
  add_ymd(
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
subtract_months <- function(x,
                            n,
                            ...,
                            day_nonexistent = "last-time",
                            dst_nonexistent = "directional",
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

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_weeks <- function(x,
                      n,
                      ...,
                      dst_nonexistent = "directional",
                      dst_ambiguous = "directional") {
  add_ymd(
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
subtract_weeks <- function(x,
                           n,
                           ...,
                           dst_nonexistent = "directional",
                           dst_ambiguous = "directional") {
  add_weeks(
    x = x,
    n = -n,
    ...,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_days <- function(x,
                     n,
                     ...,
                     dst_nonexistent = "directional",
                     dst_ambiguous = "directional") {
  add_ymd(
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
subtract_days <- function(x,
                          n,
                          ...,
                          dst_nonexistent = "directional",
                          dst_ambiguous = "directional") {
  add_days(
    x = x,
    n = -n,
    ...,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_hours <- function(x, n) {
  add_hms(x, n, unit = "hour")
}

#' @rdname civil-arithmetic
#' @export
subtract_hours <- function(x, n) {
  add_hours(x, -n)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_minutes <- function(x, n) {
  add_hms(x, n, unit = "minute")
}

#' @rdname civil-arithmetic
#' @export
subtract_minutes <- function(x, n) {
  add_minutes(x, -n)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_seconds <- function(x, n) {
  add_hms(x, n, unit = "second")
}

#' @rdname civil-arithmetic
#' @export
subtract_seconds <- function(x, n) {
  add_seconds(x, -n)
}

# ------------------------------------------------------------------------------

add_ymd <- function(x,
                    n,
                    ...,
                    day_nonexistent,
                    dst_nonexistent,
                    dst_ambiguous,
                    unit) {
  if (is_local_datetime(x)) {
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

add_hms <- function(x, n, ..., unit) {
  if (is_local_datetime(x)) {
    add_period_to_local(
      x = x,
      n = n,
      ...,
      day_nonexistent = "last-time",
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

add_period_to_local <- function(x, n, ..., day_nonexistent, unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  validate_day_nonexistent(day_nonexistent)

  out <- add_period_to_local_cpp(
    x = x,
    n = n,
    day_nonexistent = day_nonexistent,
    size = size,
    unit = unit
  )

  new_local_datetime(out)
}
