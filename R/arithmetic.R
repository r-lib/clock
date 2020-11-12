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
#' @param dst_nonexistent `[NULL / character]`
#'
#'   Control the behavior when a nonexistent time is generated due to a
#'   daylight savings gap.
#'
#'   - `NULL`:
#'     If `n` is positive, choose `"roll-forward"`.
#'     If `n` is negative, choose `"roll-backward"`.
#'
#'   - `"roll-forward"`: Roll forward to the next valid moment in time.
#'
#'   - `"roll-backward"`: Roll backward to the previous valid moment in time.
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
#' @param dst_ambiguous `[NULL / character]`
#'
#'   Control the behavior when an ambiguous time is generated due to a daylight
#'   savings fallback.
#'
#'   - `NULL`:
#'     If `n` is positive, choose `"earliest"`.
#'     If `n` is negative, choose `"latest"`.
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
#' # Since we are adding a positive number of days, the default
#' # chooses `"roll-forward"` which rolls forward to the
#' # next valid moment in time.
#' add_days(x, 1)
#'
#' # If we approach from the other side of the gap and subtract 1 day, then
#' # the default chooses `"roll-backward"`, which rolls backward
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
  x <- localize(x)
  out <- add_years(x, n, ..., day_nonexistent = day_nonexistent)
  unlocalize(out)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_years.POSIXt <- function(x,
                             n,
                             ...,
                             day_nonexistent = "last-time",
                             dst_nonexistent = NULL,
                             dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- localize(x)
  out <- add_years(x, n, ..., day_nonexistent = day_nonexistent)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  unlocalize(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_years.civil_zoned <- add_years.POSIXt

#' @rdname civil-local-arithmetic
#' @export
add_years.civil_local <- function(x, n, ..., day_nonexistent = "last-time") {
  add_years_local_impl(x, n, ..., day_nonexistent = day_nonexistent)
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
                                  dst_nonexistent = NULL,
                                  dst_ambiguous = NULL) {
  add_years(x, -n, ..., day_nonexistent = day_nonexistent, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_years.civil_zoned <- subtract_years.POSIXt

#' @rdname civil-local-arithmetic
#' @export
subtract_years.civil_local <- function(x, n, ..., day_nonexistent = "last-time") {
  add_years(x, -n, ..., day_nonexistent = day_nonexistent)
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
  x <- localize(x)
  out <- add_months(x, n, ..., day_nonexistent = day_nonexistent)
  unlocalize(out)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_months.POSIXt <- function(x,
                              n,
                              ...,
                              day_nonexistent = "last-time",
                              dst_nonexistent = NULL,
                              dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- localize(x)
  out <- add_months(x, n, ..., day_nonexistent = day_nonexistent)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  unlocalize(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_months.civil_zoned <- add_months.POSIXt

#' @rdname civil-local-arithmetic
#' @export
add_months.civil_local <- function(x, n, ..., day_nonexistent = "last-time") {
  add_months_local_impl(x, n, ..., day_nonexistent = day_nonexistent)
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
                                   dst_nonexistent = NULL,
                                   dst_ambiguous = NULL) {
  add_months(x, -n, ..., day_nonexistent = day_nonexistent, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_months.civil_zoned <- subtract_months.POSIXt

#' @rdname civil-local-arithmetic
#' @export
subtract_months.civil_local <- function(x, n, ..., day_nonexistent = "last-time") {
  add_months(x, -n, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_years_and_months <- function(x, years, months, ...) {
  restrict_civil_supported(x)
  UseMethod("add_years_and_months")
}

#' @rdname civil-zoned-arithmetic
#' @export
add_years_and_months.Date <- function(x,
                                      years,
                                      months,
                                      ...,
                                      day_nonexistent = "last-time") {
  n <- convert_years_and_months_to_n(years, months)
  add_months(x, n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_years_and_months.POSIXt <- function(x,
                                        years,
                                        months,
                                        ...,
                                        day_nonexistent = "last-time",
                                        dst_nonexistent = NULL,
                                        dst_ambiguous = NULL) {
  n <- convert_years_and_months_to_n(years, months)
  add_months(x, n, ..., day_nonexistent = day_nonexistent, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_years_and_months.civil_zoned <- add_years_and_months.POSIXt

#' @rdname civil-local-arithmetic
#' @export
add_years_and_months.civil_local <- function(x, years, months, ..., day_nonexistent = "last-time") {
  n <- convert_years_and_months_to_n(years, months)
  add_months(x, n, ..., day_nonexistent = day_nonexistent)
}

convert_years_and_months_to_n <- function(years, months) {
  # Assert common size
  vec_size_common(years = years, months = months)

  years <- vec_cast(years, integer(), x_arg = "years")
  months <- vec_cast(months, integer(), x_arg = "months")

  # Check offset signs
  not_ok <- ((years > 0L) & (months < 0L)) | ((years < 0L) & (months > 0L))

  if (any(not_ok, na.rm = TRUE)) {
    abort("`years` and `months` must have the same sign.")
  }

  years * 12L + months
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_years_and_months <- function(x, years, months, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_years_and_months")
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_years_and_months.Date <- function(x,
                                           years,
                                           months,
                                           ...,
                                           day_nonexistent = "last-time") {
  add_years_and_months(x, -years, -months, ..., day_nonexistent = day_nonexistent)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_years_and_months.POSIXt <- function(x,
                                             years,
                                             months,
                                             ...,
                                             day_nonexistent = "last-time",
                                             dst_nonexistent = NULL,
                                             dst_ambiguous = NULL) {
  add_years_and_months(x, -years, -months, ..., day_nonexistent = day_nonexistent, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_years_and_months.civil_zoned <- subtract_years_and_months.POSIXt

#' @rdname civil-local-arithmetic
#' @export
subtract_years_and_months.civil_local <- function(x, years, months, ..., day_nonexistent = "last-time") {
  add_years_and_months(x, -years, -months, ..., day_nonexistent = day_nonexistent)
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
  x <- localize(x)
  out <- add_weeks(x = x, n = n, ...)
  unlocalize(out)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_weeks.POSIXt <- function(x,
                             n,
                             ...,
                             dst_nonexistent = NULL,
                             dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- localize(x)
  out <- add_weeks(x, n, ...)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  unlocalize(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_weeks.civil_zoned <- add_weeks.POSIXt

#' @rdname civil-local-arithmetic
#' @export
add_weeks.civil_local <- function(x, n, ...) {
  add_weeks_local_impl(x, n, ...)
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
subtract_weeks.Date <- function(x, n, ...) {
  add_weeks(x, -n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_weeks.POSIXt <- function(x,
                                  n,
                                  ...,
                                  dst_nonexistent = NULL,
                                  dst_ambiguous = NULL) {
  add_weeks(x, -n, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_weeks.civil_zoned <- subtract_weeks.POSIXt

#' @rdname civil-local-arithmetic
#' @export
subtract_weeks.civil_local <- function(x, n, ...) {
  add_weeks(x, -n, ...)
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
  x <- localize(x)
  out <- add_days(x = x, n = n, ...)
  unlocalize(out)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_days.POSIXt <- function(x,
                            n,
                            ...,
                            dst_nonexistent = NULL,
                            dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- localize(x)
  out <- add_days(x, n, ...)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  unlocalize(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_days.civil_zoned <- add_days.POSIXt

#' @rdname civil-local-arithmetic
#' @export
add_days.civil_local <- function(x, n, ...) {
  add_days_local_impl(x, n, ...)
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
subtract_days.Date <- function(x, n, ...) {
  add_days(x, -n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_days.POSIXt <- function(x,
                                 n,
                                 ...,
                                 dst_nonexistent = NULL,
                                 dst_ambiguous = NULL) {
  add_days(x, -n, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_days.civil_zoned <- subtract_days.POSIXt

#' @rdname civil-local-arithmetic
#' @export
subtract_days.civil_local <- function(x, n, ...) {
  add_days(x, -n, ...)
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
  add_hours_zoned_impl(x, n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_hours.POSIXt <- add_hours.Date

#' @rdname civil-zoned-arithmetic
#' @export
add_hours.civil_zoned <- add_hours.Date

#' @rdname civil-local-arithmetic
#' @export
add_hours.civil_local <- function(x, n, ...) {
  add_hours_local_impl(x, n, ...)
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
subtract_hours.POSIXt <- subtract_hours.Date

#' @rdname civil-zoned-arithmetic
#' @export
subtract_hours.civil_zoned <- subtract_hours.Date

#' @rdname civil-local-arithmetic
#' @export
subtract_hours.civil_local <- function(x, n, ...) {
  add_hours(x, -n, ...)
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
  add_minutes_zoned_impl(x, n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_minutes.POSIXt <- add_minutes.Date

#' @rdname civil-zoned-arithmetic
#' @export
add_minutes.civil_zoned <- add_minutes.Date

#' @rdname civil-local-arithmetic
#' @export
add_minutes.civil_local <- function(x, n, ...) {
  add_minutes_local_impl(x, n, ...)
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
subtract_minutes.POSIXt <- subtract_minutes.Date

#' @rdname civil-zoned-arithmetic
#' @export
subtract_minutes.civil_zoned <- subtract_minutes.Date

#' @rdname civil-local-arithmetic
#' @export
subtract_minutes.civil_local <- function(x, n, ...) {
  add_minutes(x, -n, ...)
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
  add_seconds_zoned_impl(x, n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_seconds.POSIXt <- add_seconds.Date

#' @rdname civil-zoned-arithmetic
#' @export
add_seconds.civil_zoned <- add_seconds.Date

#' @rdname civil-local-arithmetic
#' @export
add_seconds.civil_local <- function(x, n, ...) {
  add_seconds_local_impl(x, n, ...)
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
subtract_seconds.POSIXt <- subtract_seconds.Date

#' @rdname civil-zoned-arithmetic
#' @export
subtract_seconds.civil_zoned <- subtract_seconds.Date

#' @rdname civil-local-arithmetic
#' @export
subtract_seconds.civil_local <- function(x, n, ...) {
  add_seconds(x, -n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_milliseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_milliseconds")
}

#' @rdname civil-zoned-arithmetic
#' @export
add_milliseconds.Date <- function(x, n, ...) {
  add_milliseconds_zoned_impl(x, n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_milliseconds.POSIXt <- add_milliseconds.Date

#' @rdname civil-zoned-arithmetic
#' @export
add_milliseconds.civil_zoned <- add_milliseconds.Date

#' @rdname civil-local-arithmetic
#' @export
add_milliseconds.civil_local <- function(x, n, ...) {
  add_milliseconds_local_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_milliseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_milliseconds")
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_milliseconds.Date <- function(x, n, ...) {
  add_milliseconds(x, -n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_milliseconds.POSIXt <- subtract_milliseconds.Date

#' @rdname civil-zoned-arithmetic
#' @export
subtract_milliseconds.civil_zoned <- subtract_milliseconds.Date

#' @rdname civil-local-arithmetic
#' @export
subtract_milliseconds.civil_local <- function(x, n, ...) {
  add_milliseconds(x, -n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_microseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_microseconds")
}

#' @rdname civil-zoned-arithmetic
#' @export
add_microseconds.Date <- function(x, n, ...) {
  add_microseconds_zoned_impl(x, n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_microseconds.POSIXt <- add_microseconds.Date

#' @rdname civil-zoned-arithmetic
#' @export
add_microseconds.civil_zoned <- add_microseconds.Date

#' @rdname civil-local-arithmetic
#' @export
add_microseconds.civil_local <- function(x, n, ...) {
  add_microseconds_local_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_microseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_microseconds")
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_microseconds.Date <- function(x, n, ...) {
  add_microseconds(x, -n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_microseconds.POSIXt <- subtract_microseconds.Date

#' @rdname civil-zoned-arithmetic
#' @export
subtract_microseconds.civil_zoned <- subtract_microseconds.Date

#' @rdname civil-local-arithmetic
#' @export
subtract_microseconds.civil_local <- function(x, n, ...) {
  add_microseconds(x, -n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
add_nanoseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_nanoseconds")
}

#' @rdname civil-zoned-arithmetic
#' @export
add_nanoseconds.Date <- function(x, n, ...) {
  add_nanoseconds_zoned_impl(x, n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
add_nanoseconds.POSIXt <- add_nanoseconds.Date

#' @rdname civil-zoned-arithmetic
#' @export
add_nanoseconds.civil_zoned <- add_nanoseconds.Date

#' @rdname civil-local-arithmetic
#' @export
add_nanoseconds.civil_local <- function(x, n, ...) {
  add_nanoseconds_local_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname civil-arithmetic
#' @export
subtract_nanoseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_nanoseconds")
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_nanoseconds.Date <- function(x, n, ...) {
  add_nanoseconds(x, -n, ...)
}

#' @rdname civil-zoned-arithmetic
#' @export
subtract_nanoseconds.POSIXt <- subtract_nanoseconds.Date

#' @rdname civil-zoned-arithmetic
#' @export
subtract_nanoseconds.civil_zoned <- subtract_nanoseconds.Date

#' @rdname civil-local-arithmetic
#' @export
subtract_nanoseconds.civil_local <- function(x, n, ...) {
  add_nanoseconds(x, -n, ...)
}

# ------------------------------------------------------------------------------

add_years_local_impl <- function(x, n, ..., day_nonexistent, unit) {
  x <- promote_at_least_local_year(x)
  add_years_or_months_local(x, n, ..., day_nonexistent = day_nonexistent, unit = "year")
}
add_months_local_impl <- function(x, n, ..., day_nonexistent, unit) {
  x <- promote_at_least_local_year_month(x)
  add_years_or_months_local(x, n, ..., day_nonexistent = day_nonexistent, unit = "month")
}

add_years_or_months_local <- function(x, n, ..., day_nonexistent, unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  add_years_or_months_local_cpp(x, n, day_nonexistent, unit, size)
}

# ------------------------------------------------------------------------------

add_weeks_local_impl <- function(x, n, ...) {
  x <- promote_at_least_local_year_week(x)
  add_weeks_or_days_local(x, n, ..., unit = "week")
}
add_days_local_impl <- function(x, n, ...) {
  x <- promote_at_least_local_date(x)
  add_weeks_or_days_local(x, n, ..., unit = "day")
}

add_weeks_or_days_local <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  add_weeks_or_days_local_cpp(x, n, unit, size)
}

# ------------------------------------------------------------------------------

add_hours_zoned_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_zoned(x, n, ..., unit = "hour")
}
add_minutes_zoned_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_zoned(x, n, ..., unit = "minute")
}
add_seconds_zoned_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_zoned(x, n, ..., unit = "second")
}

add_hours_or_minutes_or_seconds_zoned <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  x <- promote_at_least_zoned_datetime(x)

  if (is_zoned_nano_datetime(x)) {
    add_hours_or_minutes_or_seconds_zoned_nano_datetime(x, n, unit)
  } else {
    add_hours_or_minutes_or_seconds_zoned_datetime(x, n, unit)
  }
}

add_hours_or_minutes_or_seconds_zoned_nano_datetime <- function(x, n, unit) {
  size <- vec_size_common(x = x, n = n)
  add_hours_or_minutes_or_seconds_zoned_nano_datetime_cpp(x, n, unit, size)
}

add_hours_or_minutes_or_seconds_zoned_datetime <- function(x, n, unit) {
  # Check tidyverse recyclability
  vec_size_common(x = x, n = n)

  if (identical(unit, "hour")) {
    n <- n * seconds_in_hour()
  } else if (identical(unit, "minute")) {
    n <- n * seconds_in_minute()
  } else if (identical(unit, "second")) {
    n <- n
  } else {
    abort("Internal error: Unknown `unit` in hour/minute/second arithmetic.")
  }

  x + n
}

# Purposefully promote to double to avoid possible integer overflow
seconds_in_hour <- function() {
  3600
}
seconds_in_minute <- function() {
  60
}

# ------------------------------------------------------------------------------

add_hours_local_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_local(x, n, ..., unit = "hour")
}
add_minutes_local_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_local(x, n, ..., unit = "minute")
}
add_seconds_local_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_local(x, n, ..., unit = "second")
}

add_hours_or_minutes_or_seconds_local <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  x <- promote_at_least_local_datetime(x)

  add_hours_or_minutes_or_seconds_local_cpp(x, n, unit, size)
}

# ------------------------------------------------------------------------------

add_milliseconds_zoned_impl <- function(x, n, ...) {
  add_milliseconds_or_microseconds_or_nanoseconds_zoned(x, n, ..., unit = "millisecond")
}
add_microseconds_zoned_impl <- function(x, n, ...) {
  add_milliseconds_or_microseconds_or_nanoseconds_zoned(x, n, ..., unit = "microsecond")
}
add_nanoseconds_zoned_impl <- function(x, n, ...) {
  add_milliseconds_or_microseconds_or_nanoseconds_zoned(x, n, ..., unit = "nanosecond")
}

add_milliseconds_or_microseconds_or_nanoseconds_zoned <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  x <- promote_at_least_zoned_nano_datetime(x)

  add_milliseconds_or_microseconds_or_nanoseconds_zoned_cpp(x, n, unit, size)
}

# ------------------------------------------------------------------------------

add_milliseconds_local_impl <- function(x, n, ...) {
  add_milliseconds_or_microseconds_or_nanoseconds_local(x, n, ..., unit = "millisecond")
}
add_microseconds_local_impl <- function(x, n, ...) {
  add_milliseconds_or_microseconds_or_nanoseconds_local(x, n, ..., unit = "microsecond")
}
add_nanoseconds_local_impl <- function(x, n, ...) {
  add_milliseconds_or_microseconds_or_nanoseconds_local(x, n, ..., unit = "nanosecond")
}

add_milliseconds_or_microseconds_or_nanoseconds_local <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  x <- promote_at_least_local_nano_datetime(x)

  add_milliseconds_or_microseconds_or_nanoseconds_local_cpp(x, n, unit, size)
}

# ------------------------------------------------------------------------------

dst_nonexistent_standardize <- function(dst_nonexistent, n) {
  # User specified
  if (!is.null(dst_nonexistent)) {
    return(dst_nonexistent)
  }

  # Directional smart default
  dst_nonexistent <- if_else(
    condition = n >= 0L,
    true = "roll-forward",
    false = "roll-backward",
    na = "roll-forward"
  )

  dst_nonexistent
}

dst_ambiguous_standardize <- function(dst_ambiguous, n) {
  # User specified
  if (!is.null(dst_ambiguous)) {
    return(dst_ambiguous)
  }

  # Directional smart default
  dst_ambiguous <- if_else(
    condition = n >= 0L,
    true = "earliest",
    false = "latest",
    na = "earliest"
  )

  dst_ambiguous
}
