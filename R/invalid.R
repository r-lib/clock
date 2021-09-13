#' Invalid calendar dates
#'
#' @description
#' This family of functions is for working with _invalid_ calendar dates.
#'
#' Invalid dates represent dates made up of valid individual components, which
#' taken as a whole don't represent valid calendar dates. For example, for
#' [year_month_day()] the following component ranges are valid:
#' `year: [-32767, 32767]`, `month: [1, 12]`, `day: [1, 31]`.
#' However, the date `2019-02-31` doesn't exist even though it is made up
#' of valid components. This is an example of an invalid date.
#'
#' Invalid dates are allowed in clock, provided that they are eventually
#' resolved by using `invalid_resolve()` or by manually resolving them through
#' arithmetic or setter functions.
#'
#' @details
#' Invalid dates must be resolved before converting them to a time point.
#'
#' It is recommended to use `"previous"` or `"next"` for resolving invalid
#' dates, as these ensure that _relative ordering_ among `x` is maintained.
#' This is a often a very important property to maintain when doing time series
#' data analysis. See the examples for more information.
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param x `[calendar]`
#'
#'   A calendar vector.
#'
#' @param invalid `[character(1) / NULL]`
#'
#'   One of the following invalid date resolution strategies:
#'
#'   - `"previous"`: The previous valid instant in time.
#'
#'   - `"previous-day"`: The previous valid day in time, keeping the time of
#'     day.
#'
#'   - `"next"`: The next valid instant in time.
#'
#'   - `"next-day"`: The next valid day in time, keeping the time of day.
#'
#'   - `"overflow"`: Overflow by the number of days that the input is invalid
#'     by. Time of day is dropped.
#'
#'   - `"overflow-day"`: Overflow by the number of days that the input is
#'     invalid by. Time of day is kept.
#'
#'   - `"NA"`: Replace invalid dates with `NA`.
#'
#'   - `"error"`: Error on invalid dates.
#'
#'   Using either `"previous"` or `"next"` is generally recommended, as these
#'   two strategies maintain the _relative ordering_ between elements of the
#'   input.
#'
#'   If `NULL`, defaults to `"error"`.
#'
#'   If `getOption("clock.strict")` is `TRUE`, `invalid` must be supplied and
#'   cannot be `NULL`. This is a convenient way to make production code robust
#'   to invalid dates.
#'
#' @return
#' - `invalid_detect()`: Returns a logical vector detecting invalid dates.
#'
#' - `invalid_any()`: Returns `TRUE` if any invalid dates are detected.
#'
#' - `invalid_count()`: Returns a single integer containing the number of
#'   invalid dates.
#'
#' - `invalid_remove()`: Returns `x` with invalid dates removed.
#'
#' - `invalid_resolve()`: Returns `x` with invalid dates resolved using the
#'   `invalid` strategy.
#'
#' @name clock-invalid
#' @examples
#' # Invalid date
#' x <- year_month_day(2019, 04, 30:31, c(3, 2), 30, 00)
#' x
#'
#' invalid_detect(x)
#'
#' # Previous valid moment in time
#' x_previous <- invalid_resolve(x, invalid = "previous")
#' x_previous
#'
#' # Previous valid day, retaining time of day
#' x_previous_day <- invalid_resolve(x, invalid = "previous-day")
#' x_previous_day
#'
#' # Note that `"previous"` retains the relative ordering in `x`
#' x[1] < x[2]
#' x_previous[1] < x_previous[2]
#'
#' # But `"previous-day"` here does not!
#' x_previous_day[1] < x_previous_day[2]
#'
#' # Remove invalid dates entirely
#' invalid_remove(x)
#'
#' y <- year_quarter_day(2019, 1, 90:92)
#' y
#'
#' # Overflow rolls forward by the number of days between `y` and the previous
#' # valid date
#' invalid_resolve(y, invalid = "overflow")
NULL

# ------------------------------------------------------------------------------

#' @rdname clock-invalid
#' @export
invalid_detect <- function(x) {
  UseMethod("invalid_detect")
}

#' @export
invalid_detect.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("invalid_detect")
}

# ------------------------------------------------------------------------------

#' @rdname clock-invalid
#' @export
invalid_any <- function(x) {
  UseMethod("invalid_any")
}

#' @export
invalid_any.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("invalid_any")
}

# ------------------------------------------------------------------------------

#' @rdname clock-invalid
#' @export
invalid_count <- function(x) {
  UseMethod("invalid_count")
}

#' @export
invalid_count.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("invalid_count")
}

# ------------------------------------------------------------------------------

#' @rdname clock-invalid
#' @export
invalid_remove <- function(x) {
  UseMethod("invalid_remove")
}

#' @export
invalid_remove.clock_calendar <- function(x) {
  x[!invalid_detect(x)]
}

# ------------------------------------------------------------------------------

#' @rdname clock-invalid
#' @export
invalid_resolve <- function(x, ..., invalid = NULL) {
  UseMethod("invalid_resolve")
}

#' @export
invalid_resolve.clock_calendar <- function(x, ..., invalid = NULL) {
  stop_clock_unsupported_calendar_op("invalid_resolve")
}

validate_invalid <- function(invalid) {
  invalid <- strict_validate_invalid(invalid)

  if (!is_string(invalid)) {
    abort("`invalid` must be a character vector with length 1.")
  }

  invalid
}
