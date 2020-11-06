#' Local date-time arithmetic
#'
#' @description
#' Local arithmetic involves adding or subtracting units of time from a datetime
#' that is independent of any time zone. This means that daylight savings time
#' is never an issue while working with a local datetime. Usually, you will
#' convert to a local datetime with [localize()], perform multiple arithmetic
#' operations or adjustments with it, then convert it back to a zoned datetime
#' with [unlocalize()].
#'
#' Local arithmetic is usually appropriate when performing multiple arithmetic
#' operations in a row (like adding a set of years, months, and days). The
#' alternative is [zoned arithmetic][civil-zoned-arithmetic], which is simpler
#' and more straightforward when you just need to perform a single operation
#' (like just adding 3 months).
#'
#' @section Nonexistent Days:
#' Local datetimes are unique because they are allowed to land on _nonexistent
#' days_. These might occur from adding 1 month to `"1971-01-29"`, which
#' theoretically lands on `"1971-02-29"`, a nonexistent day. With zoned
#' arithmetic, you are immediately forced to make a decision on how to handle
#' this nonexistent day. With local arithmetic, that decision is delayed,
#' allowing you precise control over how to handle this case. As an example, you
#' might choose to add 1 year to this nonexistent date, resulting in
#' `"1972-02-29"`, which does exist due to it being a leap year. Or you might
#' convert it back to a zoned date with [unlocalize()], which forces you
#' to deal with the nonexistent date using the `day_nonexistent` argument.
#' The default would choose the last real day in February of that year,
#' resulting in `"1971-02-28"`.
#'
#' There are operations that force you to resolve nonexistent days. Using
#' `add_days()`, `add_hours()`, `add_minutes()`, or `add_seconds()` will
#' force you to resolve the nonexistent day by using the `day_nonexistent`
#' argument. After the nonexistent day has been resolved, the unit of time
#' is added.
#'
#' @inheritParams add_years.Date
#'
#' @param x `[local_date / local_datetime]`
#'
#'   A local date-time vector.
#'
#' @name civil-local-arithmetic
NULL

#' Zoned date-time arithmetic
#'
#' @description
#' Zoned arithmetic involves adding or subtracting units of time from a datetime
#' that has a time zone attached. This means that all of the complexities of
#' daylight savings time and nonexistent days have to be handled after each
#' individual arithmetic operation. The alternative is
#' [local arithmetic][civil-local-arithmetic], which only deals with the
#' complexities of time zones once after all arithmetic has been performed.
#'
#' Zoned arithmetic is usually fine for additions of singular units of time.
#' If you want to add multiple periods, consider switching to local arithmetic.
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
#' @param x `[Date / POSIXct / POSIXlt]`
#'
#'   A zoned date-time vector.
#'
#' @param n `[integer]`
#'
#'   An integer vector representing the number of units to add to or
#'   subtract from `x`.
#'
#' @param day_nonexistent `[character(1)]`
#'
#'   Control the behavior when a nonexistent day is generated.
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
#'   _Warning_: When used in arithmetic with date-times, `"last-day"` and
#'   `"first-day"` do not guarantee that the relative ordering of `x` is
#'   maintained.
#'
#' @param dst_nonexistent `[character(1)]`
#'
#'   Control the behavior when a nonexistent time is generated due to a
#'   daylight savings gap.
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
#'   savings fallback.
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
#' @name civil-zoned-arithmetic
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

#' Date-time arithmetic
#'
#' @description
#' Date-time arithmetic in civil is broken down into two types: zoned and local.
#' Each type is documented on its own help page.
#'
#' [Zoned][civil-zoned-arithmetic] dates have a time zone attached. The two
#' base R classes, POSIXct and Date, implement zoned dates (Date is assumed to
#' implicitly have a time zone of UTC).
#'
#' [Local][civil-local-arithmetic] dates are independent of a time zone. The
#' two civil objects, local_date and local_datetime, implement local dates.
#'
#' @param x `[Date / POSIXct / POSIXlt / local_date / local_datetime]`
#'
#'   A zoned or local date-time vector.
#'
#' @param n `[integer]`
#'
#'   An integer vector representing the number of units to add to or
#'   subtract from `x`.
#'
#' @param ... These dots are for future extensions and must be empty.
#'
#' @name civil-arithmetic
NULL

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_years <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_years")
}

#' @rdname civil-zoned-arithmetic
#' @export
add_years.Date <- function(x,
                           n,
                           ...,
                           day_nonexistent = "last-time") {
  add_ymd_to_date(x, n, ..., day_nonexistent = day_nonexistent, unit = "year")
}

#' @rdname civil-zoned-arithmetic
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

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
subtract_years.Date <- function(x,
                                n,
                                ...,
                                day_nonexistent = "last-time") {
  add_years(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname civil-zoned-arithmetic
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

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
add_months.Date <- function(x,
                            n,
                            ...,
                            day_nonexistent = "last-time") {
  add_ymd_to_date(x, n, ..., day_nonexistent = day_nonexistent, unit = "month")
}

#' @rdname civil-zoned-arithmetic
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

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
subtract_months.Date <- function(x,
                                 n,
                                 ...,
                                 day_nonexistent = "last-time") {
  add_months(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname civil-zoned-arithmetic
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

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
add_weeks.Date <- function(x, n, ...) {
  add_ymd_to_date(x, n, ..., day_nonexistent = "last-time", unit = "week")
}

#' @rdname civil-zoned-arithmetic
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

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
subtract_weeks.Date <- function(x,
                                n,
                                ...,
                                day_nonexistent = "last-time") {
  add_weeks(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname civil-zoned-arithmetic
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

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
add_days.Date <- function(x, n, ...) {
  add_ymd_to_date(x, n, ..., day_nonexistent = "last-time", unit = "day")
}

#' @rdname civil-zoned-arithmetic
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

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
subtract_days.Date <- function(x,
                               n,
                               ...,
                               day_nonexistent = "last-time") {
  add_days(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname civil-zoned-arithmetic
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

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
add_hours.Date <- function(x, n, ...) {
  add_hms_to_date(x, n, ..., unit = "hour")
}

#' @rdname civil-zoned-arithmetic
#' @export
add_hours.POSIXt <- function(x, n, ...) {
  add_hms_to_posixt(x, n, ..., unit = "hour")
}

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
subtract_hours.Date <- function(x, n, ...) {
  add_hours(x, -n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_hours.POSIXt <- function(x, n, ...) {
  add_hours(x, -n, ...)
}

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
add_minutes.Date <- function(x, n, ...) {
  add_hms_to_date(x, n, ..., unit = "minute")
}

#' @rdname civil-zoned-arithmetic
#' @export
add_minutes.POSIXt <- function(x, n, ...) {
  add_hms_to_posixt(x, n, ..., unit = "minute")
}

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
subtract_minutes.Date <- function(x, n, ...) {
  add_minutes(x, -n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_minutes.POSIXt <- function(x, n, ...) {
  add_minutes(x, -n, ...)
}

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
add_seconds.Date <- function(x, n, ...) {
  add_hms_to_date(x, n, ..., unit = "second")
}

#' @rdname civil-zoned-arithmetic
#' @export
add_seconds.POSIXt <- function(x, n, ...) {
  add_hms_to_posixt(x, n, ..., unit = "second")
}

#' @rdname civil-local-arithmetic
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

#' @rdname civil-zoned-arithmetic
#' @export
subtract_seconds.Date <- function(x, n, ...) {
  add_seconds(x, -n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_seconds.POSIXt <- function(x, n, ...) {
  add_seconds(x, -n, ...)
}

#' @rdname civil-local-arithmetic
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
