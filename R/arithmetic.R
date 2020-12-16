#' Naive date-time arithmetic
#'
#' @description
#' Naive arithmetic involves adding or subtracting units of time from a datetime
#' that is independent of any time zone. This means that daylight savings time
#' is never an issue while working with a naive datetime. Usually, you will
#' convert to a naive datetime with [as_naive()], perform multiple arithmetic
#' operations or adjustments with it, then convert it back to a zoned datetime
#' with [as_zoned()].
#'
#' Naive arithmetic is usually appropriate when performing multiple arithmetic
#' operations in a row (like adding a set of years, months, and days). The
#' alternative is [zoned arithmetic][clock-zoned-arithmetic], which is simpler
#' and more straightforward when you just need to perform a single operation
#' (like just adding 3 months).
#'
#' @section Nonexistent Days:
#' Naive datetimes are unique because they are allowed to land on _nonexistent
#' days_. These might occur from adding 1 month to `"1971-01-29"`, which
#' theoretically lands on `"1971-02-29"`, a nonexistent day. With zoned
#' arithmetic, you are immediately forced to make a decision on how to handle
#' this nonexistent day. With naive arithmetic, that decision is delayed,
#' allowing you precise control over how to handle this case. As an example, you
#' might choose to add 1 year to this nonexistent date, resulting in
#' `"1972-02-29"`, which does exist due to it being a leap year. Or you might
#' convert it back to a zoned date with [as_zoned()], which forces you
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
#' @param x `[clock_naive]`
#'
#'   A naive date-time vector.
#'
#' @name clock-naive-arithmetic
NULL

#' Zoned date-time arithmetic
#'
#' @description
#' Zoned arithmetic involves adding or subtracting units of time from a datetime
#' that has a time zone attached. This means that all of the complexities of
#' daylight savings time and nonexistent days have to be handled after each
#' individual arithmetic operation. The alternative is
#' [naive arithmetic][clock-naive-arithmetic], which only deals with the
#' complexities of time zones once after all arithmetic has been performed.
#'
#' Zoned arithmetic is usually fine for additions of singular units of time.
#' If you want to add multiple periods, consider switching to naive arithmetic.
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
#' @name clock-zoned-arithmetic
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
#' Date-time arithmetic in clock is broken down into two types: zoned and naive.
#' Each type is documented on its own help page.
#'
#' [Zoned][clock-zoned-arithmetic] dates have a time zone attached. The two
#' base R classes, POSIXct and Date, implement zoned dates (Date is assumed to
#' implicitly have a time zone of UTC).
#'
#' [Naive][clock-naive-arithmetic] dates are independent of a time zone.
#'
#' @param x `[Date / POSIXct / POSIXlt]`
#'
#'   A zoned or naive date-time vector.
#'
#' @param n `[integer]`
#'
#'   An integer vector representing the number of units to add to or
#'   subtract from `x`.
#'
#' @param ... These dots are for future extensions and must be empty.
#'
#' @name clock-arithmetic
NULL

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_years <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_years")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_years.Date <- function(x,
                           n,
                           ...,
                           day_nonexistent = "last-time") {
  x <- as_naive_time_point(x)
  out <- add_years(x, n, ..., day_nonexistent = day_nonexistent)
  as.Date(out)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_years.POSIXt <- function(x,
                             n,
                             ...,
                             day_nonexistent = "last-time",
                             dst_nonexistent = NULL,
                             dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- add_years(x, n, ..., day_nonexistent = day_nonexistent)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_years.clock_zoned_time_point <- function(x,
                                             n,
                                             ...,
                                             day_nonexistent = "last-time",
                                             dst_nonexistent = NULL,
                                             dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- add_years(x, n, ..., day_nonexistent = day_nonexistent)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_years.clock_naive_time_point <- function(x, n, ..., day_nonexistent = "last-time") {
  add_naive_time_point_calendar(x, n, ..., day_nonexistent = day_nonexistent, dispatcher = add_years_calendar)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_years.clock_calendar <- function(x, n, ..., day_nonexistent = "last-time") {
  add_calendar(x, n, ..., day_nonexistent = day_nonexistent, dispatcher = add_years_calendar)
}

add_years_calendar <- function(x, n, ..., day_nonexistent) {
  UseMethod("add_years_calendar")
}

#' @export
add_years_calendar.clock_calendar <- function(x, n, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("add_years")
}

#' @export
add_years_calendar.clock_gregorian <- function(x, n, ..., day_nonexistent) {
  x <- promote_precision_year(x)
  add_gregorian_calendar_years_or_months(x, n, day_nonexistent, "year")
}

#' @export
add_years_calendar.clock_quarterly <- function(x, n, ..., day_nonexistent) {
  x <- promote_precision_year(x)
  start <- get_start(x)
  add_quarterly_calendar_years_or_quarters(x, n, start, day_nonexistent, "year")
}

#' @export
add_years_calendar.clock_iso <- function(x, n, ..., day_nonexistent) {
  x <- promote_precision_year(x)
  add_iso_calendar_years(x, n, day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_years <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_years")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_years.Date <- function(x, n, ..., day_nonexistent = "last-time") {
  add_years(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_years.POSIXt <- function(x,
                                  n,
                                  ...,
                                  day_nonexistent = "last-time",
                                  dst_nonexistent = NULL,
                                  dst_ambiguous = NULL) {
  add_years(x, -n, ..., day_nonexistent = day_nonexistent, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_years.clock_zoned_time_point <- subtract_years.POSIXt

#' @rdname clock-naive-arithmetic
#' @export
subtract_years.clock_naive_time_point <- function(x, n, ..., day_nonexistent = "last-time") {
  add_years(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname clock-naive-arithmetic
#' @export
subtract_years.clock_calendar <- subtract_years.clock_naive_time_point

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_quarters <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_quarters")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_quarters.Date <- function(x,
                              n,
                              ...,
                              day_nonexistent = "last-time") {
  x <- as_naive_time_point(x)
  out <- add_quarters(x, n, ..., day_nonexistent = day_nonexistent)
  as.Date(out)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_quarters.POSIXt <- function(x,
                                n,
                                ...,
                                day_nonexistent = "last-time",
                                dst_nonexistent = NULL,
                                dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- add_quarters(x, n, ..., day_nonexistent = day_nonexistent)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_quarters.clock_zoned_time_point <- function(x,
                                                n,
                                                ...,
                                                day_nonexistent = "last-time",
                                                dst_nonexistent = NULL,
                                                dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- as_naive_time_point(x)
  out <- add_quarters(x, n, ..., day_nonexistent = day_nonexistent)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  as_zoned_time_point(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_quarters.clock_naive_time_point <- function(x, n, ..., day_nonexistent = "last-time") {
  add_naive_time_point_calendar(x, n, ..., day_nonexistent = day_nonexistent, dispatcher = add_quarters_calendar)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_quarters.clock_calendar <- function(x, n, ..., day_nonexistent = "last-time") {
  add_calendar(x, n, ..., day_nonexistent = day_nonexistent, dispatcher = add_quarters_calendar)
}

add_quarters_calendar <- function(x, n, ..., day_nonexistent) {
  UseMethod("add_quarters_calendar")
}

#' @export
add_quarters_calendar.clock_calendar <- function(x, n, ..., day_nonexistent) {
  stop_clock_unsupported_calendar_op("add_quarters")
}

#' @export
add_quarters_calendar.clock_gregorian <- function(x, n, ..., day_nonexistent) {
  x <- promote_precision_month(x)
  add_gregorian_calendar_years_or_months(x, n * 3L, day_nonexistent, "month")
}

#' @export
add_quarters_calendar.clock_quarterly <- function(x, n, ..., day_nonexistent) {
  x <- promote_precision_quarter(x)
  start <- get_start(x)
  add_quarterly_calendar_years_or_quarters(x, n, start, day_nonexistent, "quarter")
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_quarters <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_quarters")
}

#' @rdname clock-naive-arithmetic
#' @export
subtract_quarters.civil_naive <- function(x, n, ..., day_nonexistent = "last-time") {
  add_quarters(x, -n, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_months <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_months")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_months.Date <- function(x,
                            n,
                            ...,
                            day_nonexistent = "last-time") {
  x <- as_naive(x)
  out <- add_months(x, n, ..., day_nonexistent = day_nonexistent)
  as.Date(out)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_months.POSIXt <- function(x,
                              n,
                              ...,
                              day_nonexistent = "last-time",
                              dst_nonexistent = NULL,
                              dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- add_months(x, n, ..., day_nonexistent = day_nonexistent)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_months.civil_zoned <- function(x,
                                   n,
                                   ...,
                                   day_nonexistent = "last-time",
                                   dst_nonexistent = NULL,
                                   dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- add_months(x, n, ..., day_nonexistent = day_nonexistent)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  as_zoned(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-naive-arithmetic
#' @export
add_months.civil_naive_gregorian <- function(x, n, ..., day_nonexistent = "last-time") {
  add_months_gregorian_impl(x, n, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_months <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_months")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_months.Date <- function(x,
                                 n,
                                 ...,
                                 day_nonexistent = "last-time") {
  add_months(x, -n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_months.POSIXt <- function(x,
                                   n,
                                   ...,
                                   day_nonexistent = "last-time",
                                   dst_nonexistent = NULL,
                                   dst_ambiguous = NULL) {
  add_months(x, -n, ..., day_nonexistent = day_nonexistent, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_months.civil_zoned <- subtract_months.POSIXt

#' @rdname clock-naive-arithmetic
#' @export
subtract_months.civil_naive <- function(x, n, ..., day_nonexistent = "last-time") {
  add_months(x, -n, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_years_and_months <- function(x, years, months, ...) {
  restrict_civil_supported(x)
  UseMethod("add_years_and_months")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_years_and_months.Date <- function(x,
                                      years,
                                      months,
                                      ...,
                                      day_nonexistent = "last-time") {
  n <- convert_years_and_months_to_n(years, months)
  add_months(x, n, ..., day_nonexistent = day_nonexistent)
}

#' @rdname clock-zoned-arithmetic
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

#' @rdname clock-zoned-arithmetic
#' @export
add_years_and_months.civil_zoned <- add_years_and_months.POSIXt

#' @rdname clock-naive-arithmetic
#' @export
add_years_and_months.civil_naive_gregorian <- function(x, years, months, ..., day_nonexistent = "last-time") {
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

#' @rdname clock-arithmetic
#' @export
subtract_years_and_months <- function(x, years, months, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_years_and_months")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_years_and_months.Date <- function(x,
                                           years,
                                           months,
                                           ...,
                                           day_nonexistent = "last-time") {
  add_years_and_months(x, -years, -months, ..., day_nonexistent = day_nonexistent)
}

#' @rdname clock-zoned-arithmetic
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

#' @rdname clock-zoned-arithmetic
#' @export
subtract_years_and_months.civil_zoned <- subtract_years_and_months.POSIXt

#' @rdname clock-naive-arithmetic
#' @export
subtract_years_and_months.civil_naive <- function(x, years, months, ..., day_nonexistent = "last-time") {
  add_years_and_months(x, -years, -months, ..., day_nonexistent = day_nonexistent)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_weeks <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_weeks")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_weeks.Date <- function(x, n, ...) {
  x <- as_naive(x)
  out <- add_weeks(x = x, n = n, ...)
  as.Date(out)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_weeks.POSIXt <- function(x,
                             n,
                             ...,
                             dst_nonexistent = NULL,
                             dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- add_weeks(x, n, ...)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_weeks.civil_zoned <- function(x,
                                  n,
                                  ...,
                                  dst_nonexistent = NULL,
                                  dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- add_weeks(x, n, ...)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  as_zoned(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-naive-arithmetic
#' @export
add_weeks.civil_naive_gregorian <- function(x, n, ...) {
  add_weeks_gregorian_impl(x, n, ...)
}

#' @rdname clock-naive-arithmetic
#' @export
add_weeks.civil_naive_quarterly <- function(x, n, ...) {
  add_weeks_quarterly_impl(x, n, ...)
}

#' @rdname clock-naive-arithmetic
#' @export
add_weeks.civil_naive_iso <- function(x, n, ...) {
  add_weeks_iso_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_weeks <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_weeks")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_weeks.Date <- function(x, n, ...) {
  add_weeks(x, -n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_weeks.POSIXt <- function(x,
                                  n,
                                  ...,
                                  dst_nonexistent = NULL,
                                  dst_ambiguous = NULL) {
  add_weeks(x, -n, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_weeks.civil_zoned <- subtract_weeks.POSIXt

#' @rdname clock-naive-arithmetic
#' @export
subtract_weeks.civil_naive <- function(x, n, ...) {
  add_weeks(x, -n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_days <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_days")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_days.Date <- function(x, n, ...) {
  x <- as_naive(x)
  out <- add_days(x = x, n = n, ...)
  as.Date(out)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_days.POSIXt <- function(x,
                            n,
                            ...,
                            dst_nonexistent = NULL,
                            dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- add_days(x, n, ...)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  as.POSIXct(out, tz = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_days.civil_zoned <- function(x,
                                 n,
                                 ...,
                                 dst_nonexistent = NULL,
                                 dst_ambiguous = NULL) {
  zone <- get_zone(x)
  x <- as_naive(x)
  out <- add_days(x, n, ...)
  dst_nonexistent <- dst_nonexistent_standardize(dst_nonexistent, n)
  dst_ambiguous <- dst_ambiguous_standardize(dst_ambiguous, n)
  as_zoned(out, zone = zone, dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-naive-arithmetic
#' @export
add_days.civil_naive_gregorian <- function(x, n, ...) {
  add_days_gregorian_impl(x, n, ...)
}

#' @rdname clock-naive-arithmetic
#' @export
add_days.civil_naive_quarterly <- function(x, n, ...) {
  add_days_quarterly_impl(x, n, ...)
}

#' @rdname clock-naive-arithmetic
#' @export
add_days.civil_naive_iso <- function(x, n, ...) {
  add_days_iso_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_days <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_days")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_days.Date <- function(x, n, ...) {
  add_days(x, -n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_days.POSIXt <- function(x,
                                 n,
                                 ...,
                                 dst_nonexistent = NULL,
                                 dst_ambiguous = NULL) {
  add_days(x, -n, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_days.civil_zoned <- subtract_days.POSIXt

#' @rdname clock-naive-arithmetic
#' @export
subtract_days.civil_naive <- function(x, n, ...) {
  add_days(x, -n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_hours <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_hours")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_hours.Date <- function(x, n, ...) {
  add_hours_posixct_impl(x, n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_hours.POSIXt <- add_hours.Date

#' @rdname clock-zoned-arithmetic
#' @export
add_hours.civil_zoned <- function(x, n, ...) {
  add_hours_zoned_impl(x, n, ...)
}

#' @rdname clock-naive-arithmetic
#' @export
add_hours.civil_naive_gregorian <- function(x, n, ...) {
  add_hours_gregorian_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_hours <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_hours")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_hours.Date <- function(x, n, ...) {
  add_hours(x, -n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_hours.POSIXt <- subtract_hours.Date

#' @rdname clock-zoned-arithmetic
#' @export
subtract_hours.civil_zoned <- subtract_hours.Date

#' @rdname clock-naive-arithmetic
#' @export
subtract_hours.civil_naive <- subtract_hours.Date

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_minutes <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_minutes")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_minutes.Date <- function(x, n, ...) {
  add_minutes_posixct_impl(x, n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_minutes.POSIXt <- add_minutes.Date

#' @rdname clock-zoned-arithmetic
#' @export
add_minutes.civil_zoned <- function(x, n, ...) {
  add_minutes_zoned_impl(x, n, ...)
}

#' @rdname clock-naive-arithmetic
#' @export
add_minutes.civil_naive_gregorian <- function(x, n, ...) {
  add_minutes_gregorian_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_minutes <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_minutes")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_minutes.Date <- function(x, n, ...) {
  add_minutes(x, -n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_minutes.POSIXt <- subtract_minutes.Date

#' @rdname clock-zoned-arithmetic
#' @export
subtract_minutes.civil_zoned <- subtract_minutes.Date

#' @rdname clock-naive-arithmetic
#' @export
subtract_minutes.civil_naive <- subtract_minutes.Date

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_seconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_seconds")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_seconds.Date <- function(x, n, ...) {
  add_seconds_posixct_impl(x, n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_seconds.POSIXt <- add_seconds.Date

#' @rdname clock-zoned-arithmetic
#' @export
add_seconds.civil_zoned <- function(x, n, ...) {
  add_seconds_zoned_impl(x, n, ...)
}

#' @rdname clock-naive-arithmetic
#' @export
add_seconds.civil_naive_gregorian <- function(x, n, ...) {
  add_seconds_gregorian_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_seconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_seconds")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_seconds.Date <- function(x, n, ...) {
  add_seconds(x, -n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_seconds.POSIXt <- subtract_seconds.Date

#' @rdname clock-zoned-arithmetic
#' @export
subtract_seconds.civil_zoned <- subtract_seconds.Date

#' @rdname clock-naive-arithmetic
#' @export
subtract_seconds.civil_naive <- subtract_seconds.Date

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_milliseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_milliseconds")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_milliseconds.Date <- function(x, n, ...) {
  add_milliseconds_zoned_impl(x, n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_milliseconds.POSIXt <- add_milliseconds.Date

#' @rdname clock-zoned-arithmetic
#' @export
add_milliseconds.civil_zoned <- add_milliseconds.Date

#' @rdname clock-naive-arithmetic
#' @export
add_milliseconds.civil_naive_gregorian <- function(x, n, ...) {
  add_milliseconds_gregorian_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_milliseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_milliseconds")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_milliseconds.Date <- function(x, n, ...) {
  add_milliseconds(x, -n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_milliseconds.POSIXt <- subtract_milliseconds.Date

#' @rdname clock-zoned-arithmetic
#' @export
subtract_milliseconds.civil_zoned <- subtract_milliseconds.Date

#' @rdname clock-naive-arithmetic
#' @export
subtract_milliseconds.civil_naive <- subtract_milliseconds.Date

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_microseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_microseconds")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_microseconds.Date <- function(x, n, ...) {
  add_microseconds_zoned_impl(x, n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_microseconds.POSIXt <- add_microseconds.Date

#' @rdname clock-zoned-arithmetic
#' @export
add_microseconds.civil_zoned <- add_microseconds.Date

#' @rdname clock-naive-arithmetic
#' @export
add_microseconds.civil_naive_gregorian <- function(x, n, ...) {
  add_microseconds_gregorian_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_microseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_microseconds")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_microseconds.Date <- function(x, n, ...) {
  add_microseconds(x, -n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_microseconds.POSIXt <- subtract_microseconds.Date

#' @rdname clock-zoned-arithmetic
#' @export
subtract_microseconds.civil_zoned <- subtract_microseconds.Date

#' @rdname clock-naive-arithmetic
#' @export
subtract_microseconds.civil_naive <- subtract_microseconds.Date

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
add_nanoseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("add_nanoseconds")
}

#' @rdname clock-zoned-arithmetic
#' @export
add_nanoseconds.Date <- function(x, n, ...) {
  add_nanoseconds_zoned_impl(x, n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
add_nanoseconds.POSIXt <- add_nanoseconds.Date

#' @rdname clock-zoned-arithmetic
#' @export
add_nanoseconds.civil_zoned <- add_nanoseconds.Date

#' @rdname clock-naive-arithmetic
#' @export
add_nanoseconds.civil_naive_gregorian <- function(x, n, ...) {
  add_nanoseconds_gregorian_impl(x, n, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arithmetic
#' @export
subtract_nanoseconds <- function(x, n, ...) {
  restrict_civil_supported(x)
  UseMethod("subtract_nanoseconds")
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_nanoseconds.Date <- function(x, n, ...) {
  add_nanoseconds(x, -n, ...)
}

#' @rdname clock-zoned-arithmetic
#' @export
subtract_nanoseconds.POSIXt <- subtract_nanoseconds.Date

#' @rdname clock-zoned-arithmetic
#' @export
subtract_nanoseconds.civil_zoned <- subtract_nanoseconds.Date

#' @rdname clock-naive-arithmetic
#' @export
subtract_nanoseconds.civil_naive <- subtract_nanoseconds.Date

# ------------------------------------------------------------------------------

add_years_gregorian_impl <- function(x, n, ..., day_nonexistent) {
  x <- promote_at_least_year(x)
  add_years_or_months_gregorian(x, n, ..., day_nonexistent = day_nonexistent, unit = "year")
}
add_months_gregorian_impl <- function(x, n, ..., day_nonexistent) {
  x <- promote_at_least_year_month(x)
  add_years_or_months_gregorian(x, n, ..., day_nonexistent = day_nonexistent, unit = "month")
}

add_years_or_months_gregorian <- function(x, n, ..., day_nonexistent, unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  add_years_or_months_gregorian_cpp(x, n, day_nonexistent, unit, size)
}

# ------------------------------------------------------------------------------

add_weeks_gregorian_impl <- function(x, n, ...) {
  x <- promote_at_least_year_month_day(x)
  add_weeks_or_days_gregorian(x, n, ..., unit = "week")
}
add_days_gregorian_impl <- function(x, n, ...) {
  x <- promote_at_least_year_month_day(x)
  add_weeks_or_days_gregorian(x, n, ..., unit = "day")
}

add_weeks_or_days_gregorian <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  # Adding weeks/days is the same at the C++ level for quarterly/gregorian
  add_weeks_or_days_cpp(x, n, unit, size)
}

# ------------------------------------------------------------------------------

add_hours_posixct_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_posixct(x, n, ..., unit = "hour")
}
add_minutes_posixct_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_posixct(x, n, ..., unit = "minute")
}
add_seconds_posixct_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_posixct(x, n, ..., unit = "second")
}

add_hours_or_minutes_or_seconds_posixct <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  x <- promote_at_least_posixct(x)

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

  size <- vec_size_common(x = x, n = n)

  # Zoned and Naive sub-daily arithmetic are equivalent at the C++ level
  add_hours_or_minutes_or_seconds_cpp(x, n, unit, size)
}

# ------------------------------------------------------------------------------

add_hours_gregorian_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_gregorian(x, n, ..., unit = "hour")
}
add_minutes_gregorian_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_gregorian(x, n, ..., unit = "minute")
}
add_seconds_gregorian_impl <- function(x, n, ...) {
  add_hours_or_minutes_or_seconds_gregorian(x, n, ..., unit = "second")
}

add_hours_or_minutes_or_seconds_gregorian <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  x <- promote_at_least_naive_datetime(x)

  # Zoned and Naive sub-daily arithmetic are equivalent at the C++ level
  add_hours_or_minutes_or_seconds_cpp(x, n, unit, size)
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

  # Zoned and Naive sub-daily arithmetic are equivalent at the C++ level
  add_milliseconds_or_microseconds_or_nanoseconds_cpp(x, n, unit, size)
}

# ------------------------------------------------------------------------------

add_milliseconds_gregorian_impl <- function(x, n, ...) {
  add_milliseconds_or_microseconds_or_nanoseconds_gregorian(x, n, ..., unit = "millisecond")
}
add_microseconds_gregorian_impl <- function(x, n, ...) {
  add_milliseconds_or_microseconds_or_nanoseconds_gregorian(x, n, ..., unit = "microsecond")
}
add_nanoseconds_gregorian_impl <- function(x, n, ...) {
  add_milliseconds_or_microseconds_or_nanoseconds_gregorian(x, n, ..., unit = "nanosecond")
}

add_milliseconds_or_microseconds_or_nanoseconds_gregorian <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  x <- promote_at_least_naive_nano_datetime(x)

  # Zoned and Naive sub-daily arithmetic are equivalent at the C++ level
  add_milliseconds_or_microseconds_or_nanoseconds_cpp(x, n, unit, size)
}

# ------------------------------------------------------------------------------

add_years_quarterly_impl <- function(x, n, ..., day_nonexistent) {
  x <- promote_at_least_quarterly_year(x)
  add_years_or_quarters_quarterly(x, n, ..., day_nonexistent = day_nonexistent, unit = "year")
}
add_quarters_quarterly_impl <- function(x, n, ..., day_nonexistent) {
  x <- promote_at_least_quarterly_year_quarternum(x)
  add_years_or_quarters_quarterly(x, n, ..., day_nonexistent = day_nonexistent, unit = "quarter")
}

add_years_or_quarters_quarterly <- function(x, n, ..., day_nonexistent, unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)
  start <- get_quarterly_start(x)

  add_years_or_quarters_quarterly_cpp(x, n, start, day_nonexistent, unit, size)
}

# ------------------------------------------------------------------------------

add_weeks_quarterly_impl <- function(x, n, ...) {
  x <- promote_at_least_quarterly_year_quarternum_quarterday(x)
  add_weeks_or_days_quarterly(x, n, ..., unit = "week")
}
add_days_quarterly_impl <- function(x, n, ...) {
  x <- promote_at_least_quarterly_year_quarternum_quarterday(x)
  add_weeks_or_days_quarterly(x, n, ..., unit = "day")
}

add_weeks_or_days_quarterly <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  # Adding weeks/days is the same at the C++ level for quarterly/gregorian
  add_weeks_or_days_cpp(x, n, unit, size)
}

# ------------------------------------------------------------------------------

add_years_iso_impl <- function(x, n, ..., day_nonexistent) {
  x <- promote_at_least_iso_year(x)
  add_years_iso(x, n, ..., day_nonexistent = day_nonexistent)
}

add_years_iso <- function(x, n, ..., day_nonexistent) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  add_years_iso_cpp(x, n, day_nonexistent, size)
}

# ------------------------------------------------------------------------------

add_weeks_iso_impl <- function(x, n, ...) {
  x <- promote_at_least_iso_year_weeknum(x)
  add_weeks_or_days_iso(x, n, ..., unit = "week")
}
add_days_iso_impl <- function(x, n, ...) {
  x <- promote_at_least_iso_year_weeknum_weekday(x)
  add_weeks_or_days_iso(x, n, ..., unit = "day")
}

add_weeks_or_days_iso <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  # Adding weeks/days is the same at the C++ level for iso/gregorian
  add_weeks_or_days_cpp(x, n, unit, size)
}

# ------------------------------------------------------------------------------

add_naive_time_point_calendar <- function(x, n, ..., day_nonexistent, dispatcher) {
  n <- vec_cast(n, integer(), x_arg = "n")

  args <- vec_recycle_common(x = x, n = n)
  x <- args$x
  n <- args$n

  calendar <- field_calendar(x)
  result <- dispatcher(calendar, n, ..., day_nonexistent = day_nonexistent)
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

add_calendar <- function(x, n, ..., day_nonexistent, dispatcher) {
  n <- vec_cast(n, integer(), x_arg = "n")

  args <- vec_recycle_common(x = x, n = n)
  x <- args$x
  n <- args$n

  result <- dispatcher(x, n, ..., day_nonexistent = day_nonexistent)

  result$calendar
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
