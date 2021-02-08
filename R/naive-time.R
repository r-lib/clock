new_naive_time_from_fields <- function(fields, precision, names) {
  new_time_point_from_fields(fields, precision, CLOCK_NAIVE, names)
}

# ------------------------------------------------------------------------------

naive_days <- function(n = integer()) {
  names <- NULL
  duration <- duration_days(n)
  new_naive_time_from_fields(duration, PRECISION_DAY, names)
}

naive_seconds <- function(n = integer()) {
  names <- NULL
  duration <- duration_seconds(n)
  new_naive_time_from_fields(duration, PRECISION_SECOND, names)
}

# ------------------------------------------------------------------------------

#' Is `x` a naive-time?
#'
#' This function determines if the input is a naive-time object.
#'
#' @param x `[object]`
#'
#'   An object.
#'
#' @return `TRUE` if `x` inherits from `"clock_naive_time"`, otherwise `FALSE`.
#'
#' @export
#' @examples
#' is_naive_time(1)
#' is_naive_time(as_naive_time(duration_days(1)))
is_naive_time <- function(x) {
  inherits(x, "clock_naive_time")
}

# ------------------------------------------------------------------------------

naive_parse <- function(x,
                        ...,
                        format = NULL,
                        precision = "second",
                        locale = clock_locale()) {
  precision <- validate_time_point_precision(precision)

  fields <- time_point_parse(
    x = x,
    ...,
    format = format,
    precision = precision,
    locale = locale,
    clock = CLOCK_NAIVE
  )

  new_naive_time_from_fields(fields, precision, names(x))
}

# ------------------------------------------------------------------------------

#' Convert to a naive-time
#'
#' @description
#' `as_naive_time()` converts `x` to a naive-time.
#'
#' You can convert to a naive-time from any calendar type, as long as it has
#' at least day precision. There also must not be any invalid dates. If invalid
#' dates exist, they must first be resolved with [invalid_resolve()].
#'
#' Converting to a naive-time from a sys-time or zoned-time retains the printed
#' time, but drops the assumption that the time should be interpreted with any
#' specific time zone.
#'
#' Converting to a naive-time from a duration just wraps the duration in a
#' naive-time object, there is no assumption about the time zone. The duration
#' must have at least day precision.
#'
#' There are convenience methods for converting to a naive-time from R's
#' native date and date-time types. Like converting from a zoned-time, these
#' retain the printed time.
#'
#' @param x `[object]`
#'
#'   An object to convert to a naive-time.
#'
#' @return A naive-time vector.
#'
#' @export
#' @examples
#' x <- as.Date("2019-01-01")
#' as_naive_time(x)
#'
#' ym <- year_month_day(2019, 02)
#'
#' # A minimum of day precision is required
#' try(as_naive_time(ym))
#'
#' ymd <- set_day(ym, 10)
#' as_naive_time(ymd)
as_naive_time <- function(x) {
  UseMethod("as_naive_time")
}

#' @export
as_naive_time.clock_naive_time <- function(x) {
  x
}

#' @export
as_naive_time.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("as_naive_time")
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_naive_time <- function(x) {
  new_sys_time_from_fields(x, time_point_precision(x), clock_rcrd_names(x))
}

#' Convert to a zoned-time from a naive-time
#'
#' @description
#' This is a naive-time method for the [as_zoned_time()] generic.
#'
#' Converting to a zoned-time from a naive-time retains the printed time,
#' but changes the underlying duration, depending on the `zone` that you choose.
#'
#' Naive-times are time points with a yet-to-be-determined time zone. By
#' converting them to a zoned-time, all you are doing is specifying that
#' time zone while attempting to keep all other printed information the
#' same (if possible).
#'
#' If you want to retain the underlying duration, try converting to a zoned-time
#' [from a sys-time][as-zoned-time-sys-time], which is a time point
#' interpreted as having a UTC time zone.
#'
#' @section Daylight Saving Time:
#'
#' Converting from a naive-time to a zoned-time is not always possible due to
#' daylight saving time issues. There are two types of these issues:
#'
#' _Nonexistent_ times are the result of daylight saving time "gaps".
#' For example, in the America/New_York time zone, there was a daylight
#' saving time gap 1 second after `"2020-03-08 01:59:59"`, where the clocks
#' changed from `01:59:59 -> 03:00:00`, completely skipping the 2 o'clock hour.
#' This means that if you had a naive time of `"2020-03-08 02:30:00"`, you
#' couldn't convert that straight into a zoned-time with this time zone. To
#' resolve these issues, the `nonexistent` argument can be used to specify
#' one of many nonexistent time resolution strategies.
#'
#' _Ambiguous_ times are the result of daylight saving time "fallbacks".
#' For example, in the America/New_York time zone, there was a daylight
#' saving time fallback 1 second after `"2020-11-01 01:59:59 EDT"`, at which
#' point the clocks "fell backwards" by 1 hour, resulting in a printed time of
#' `"2020-11-01 01:00:00 EST"` (note the EDT->EST shift). This resulted in two
#' 1 o'clock hours for this day, so if you had a naive time of
#' `"2020-11-01 01:30:00"`, you wouldn't be able to convert that directly
#' into a zoned-time with this time zone, as there is no way for clock to know
#' which of the two ambiguous times you wanted. To resolve these issues,
#' the `ambiguous` argument can be used to specify one of many ambiguous
#' time resolution strategies.
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param x `[clock_naive_time]`
#'
#'   A naive-time to convert to a zoned-time.
#'
#' @param zone `[character(1)]`
#'
#'   The zone to convert to.
#'
#' @param nonexistent `[character / NULL]`
#'
#'   One of the following nonexistent time resolution strategies, allowed to be
#'   either length 1, or the same length as the input:
#'
#'   - `"roll-forward"`: The next valid instant in time.
#'
#'   - `"roll-backward"`: The previous valid instant in time.
#'
#'   - `"shift-forward"`: Shift the nonexistent time forward by the size of
#'     the daylight saving time gap.
#'
#'   - `"shift-backward`: Shift the nonexistent time backward by the size of
#'     the daylight saving time gap.
#'
#'   - `"NA"`: Replace nonexistent times with `NA`.
#'
#'   - `"error"`: Error on nonexistent times.
#'
#'   Using either `"roll-forward"` or `"roll-backward"` is generally
#'   recommended over shifting, as these two strategies maintain the
#'   _relative ordering_ between elements of the input.
#'
#'   If `NULL`, defaults to `"error"`.
#'
#'   If `getOption("clock.strict")` is `TRUE`, `nonexistent` must be supplied
#'   and cannot be `NULL`. This is a convenient way to make production code
#'   robust to nonexistent times.
#'
#' @param ambiguous `[character / zoned_time / POSIXct / list(2) / NULL]`
#'
#'   One of the following ambiguous time resolution strategies, allowed to be
#'   either length 1, or the same length as the input:
#'
#'   - `"earliest"`: Of the two possible times, choose the earliest one.
#'
#'   - `"latest"`: Of the two possible times, choose the latest one.
#'
#'   - `"NA"`: Replace ambiguous times with `NA`.
#'
#'   - `"error"`: Error on ambiguous times.
#'
#'   Alternatively, `ambiguous` is allowed to be a zoned_time (or POSIXct) that
#'   is either length 1, or the same length as the input. If an ambiguous time
#'   is encountered, the zoned_time is consulted. If the zoned_time corresponds
#'   to a naive_time that is also ambiguous _and_ uses the same daylight saving
#'   time transition point as the original ambiguous time, then the offset of
#'   the zoned_time is used to resolve the ambiguity. If the ambiguity cannot be
#'   resolved by consulting the zoned_time, then this method falls back to
#'   `NULL`.
#'
#'   Finally, `ambiguous` is allowed to be a list of size 2, where the first
#'   element of the list is a zoned_time (as described above), and the second
#'   element of the list is an ambiguous time resolution strategy to use when
#'   the ambiguous time cannot be resolved by consulting the zoned_time.
#'   Specifying a zoned_time on its own is identical to `list(<zoned_time>,
#'   NULL)`.
#'
#'   If `NULL`, defaults to `"error"`.
#'
#'   If `getOption("clock.strict")` is `TRUE`, `ambiguous` must be supplied and
#'   cannot be `NULL`. Additionally, `ambiguous` cannot be specified as a
#'   zoned_time on its own, as this implies `NULL` for ambiguous times that the
#'   zoned_time cannot resolve. Instead, it must be specified as a list
#'   alongside an ambiguous time resolution strategy as described above. This is
#'   a convenient way to make production code robust to ambiguous times.
#'
#' @return A zoned-time vector.
#'
#' @name as-zoned-time-naive-time
#' @export
#' @examples
#' library(magrittr)
#'
#' x <- as_naive_time(year_month_day(2019, 1, 1))
#'
#' # Converting a naive-time to a zoned-time generally retains the
#' # printed time, while changing the underlying duration.
#' as_zoned_time(x, "America/New_York")
#' as_zoned_time(x, "America/Los_Angeles")
#'
#' # ---------------------------------------------------------------------------
#' # Nonexistent time:
#'
#' new_york <- "America/New_York"
#'
#' # There was a daylight saving gap in the America/New_York time zone on
#' # 2020-03-08 01:59:59 -> 03:00:00, which means that one of these
#' # naive-times don't exist in that time zone. By default, attempting to
#' # convert it to a zoned time will result in an error.
#' nonexistent_time <- year_month_day(2020, 03, 08, c(02, 03), c(45, 30), 00)
#' nonexistent_time <- as_naive_time(nonexistent_time)
#' try(as_zoned_time(nonexistent_time, new_york))
#'
#' # Resolve this by specifying a nonexistent time resolution strategy
#' as_zoned_time(nonexistent_time, new_york, nonexistent = "roll-forward")
#' as_zoned_time(nonexistent_time, new_york, nonexistent = "roll-backward")
#'
#' # Note that rolling backwards will choose the last possible moment in
#' # time at the current precision of the input
#' nonexistent_nanotime <- time_point_cast(nonexistent_time, "nanosecond")
#' nonexistent_nanotime
#' as_zoned_time(nonexistent_nanotime, new_york, nonexistent = "roll-backward")
#'
#' # A word of caution - Shifting does not guarantee that the relative ordering
#' # of the input is maintained
#' shifted <- as_zoned_time(nonexistent_time, new_york, nonexistent = "shift-forward")
#' shifted
#' # 02:45:00 < 03:30:00
#' nonexistent_time[1] < nonexistent_time[2]
#' # 03:45:00 > 03:30:00 (relative ordering is lost)
#' shifted[1] < shifted[2]
#'
#' # ---------------------------------------------------------------------------
#' # Ambiguous time:
#'
#' new_york <- "America/New_York"
#'
#' # There was a daylight saving time fallback in the America/New_York time
#' # zone on 2020-11-01 01:59:59 EDT -> 2020-11-01 01:00:00 EST, resulting
#' # in two 1 o'clock hours. This means that the following naive time is
#' # ambiguous since we don't know which of the two 1 o'clocks it belongs to.
#' # By default, attempting to convert it to a zoned time will result in an
#' # error.
#' ambiguous_time <- year_month_day(2020, 11, 01, 01, 30, 00)
#' ambiguous_time <- as_naive_time(ambiguous_time)
#' try(as_zoned_time(ambiguous_time, new_york))
#'
#' # Resolve this by specifying an ambiguous time resolution strategy
#' earliest <- as_zoned_time(ambiguous_time, new_york, ambiguous = "earliest")
#' latest <- as_zoned_time(ambiguous_time, new_york, ambiguous = "latest")
#' na <- as_zoned_time(ambiguous_time, new_york, ambiguous = "NA")
#' earliest
#' latest
#' na
#'
#' # Now assume that you were given the following zoned-times, i.e.,
#' # you didn't build them from scratch so you already know their otherwise
#' # ambiguous offsets
#' x <- c(earliest, latest)
#' x
#'
#' # To set the seconds to 5 in both, you might try:
#' x_naive <- x %>%
#'   as_naive_time() %>%
#'   as_year_month_day() %>%
#'   set_second(5) %>%
#'   as_naive_time()
#'
#' x_naive
#'
#' # But this fails because you've "lost" the information about which
#' # offsets these ambiguous times started in
#' try(as_zoned_time(x_naive, zoned_zone(x)))
#'
#' # To get around this, you can use that information by specifying
#' # `ambiguous = x`, which will use the offset from `x` to resolve the
#' # ambiguity in `x_naive` as long as `x` is also an ambiguous time with the
#' # same daylight saving time transition point as `x_naive` (i.e. here
#' # everything has a transition point of `"2020-11-01 01:00:00 EST"`).
#' as_zoned_time(x_naive, zoned_zone(x), ambiguous = x)
#'
#' # Say you added one more time to `x` that would not be considered ambiguous
#' # in naive-time
#' x <- c(x, as_zoned_time(as_sys_time(latest) + 3600, zoned_zone(latest)))
#' x
#'
#' # Imagine you want to floor this vector to a multiple of 2 hours, with
#' # an origin of 1am that day. You can do this by subtracting the origin,
#' # flooring, then adding it back
#' origin <- as_duration(as_naive_time(year_month_day(2019, 11, 01, 01, 00, 00)))
#'
#' x_naive <- x %>%
#'   as_naive_time() %>%
#'   add_seconds(-origin) %>%
#'   time_point_floor("hour", n = 2) %>%
#'   add_seconds(origin)
#'
#' x_naive
#'
#' # You again have ambiguous naive-time points, so you might try using
#' # `ambiguous = x`. It looks like this took care of the first two problems,
#' # but we have an issue at location 3.
#' try(as_zoned_time(x_naive, zoned_zone(x), ambiguous = x))
#'
#' # When we floored from 02:30:00 -> 01:00:00, we went from being
#' # unambiguous -> ambiguous. In clock, this is something you must handle
#' # explicitly, and cannot be handled by using information from `x`. You can
#' # handle this while still retaining the behavior for the other two
#' # time points that were ambiguous before and after the floor by passing a
#' # list containing `x` and an ambiguous time resolution strategy to use
#' # when information from `x` can't resolve ambiguities:
#' as_zoned_time(x_naive, zoned_zone(x), ambiguous = list(x, "latest"))
as_zoned_time.clock_naive_time <- function(x,
                                           zone,
                                           ...,
                                           nonexistent = NULL,
                                           ambiguous = NULL) {
  zone <- zone_validate(zone)

  # Promote to at least seconds precision for `zoned_time`
  x <- vec_cast(x, vec_ptype2(x, naive_seconds()))

  size <- vec_size(x)
  precision <- time_point_precision(x)
  names <- clock_rcrd_names(x)

  nonexistent <- validate_nonexistent(nonexistent, size)

  info <- validate_ambiguous(ambiguous, size, zone)
  method <- info$method

  if (identical(method, "string")) {
    ambiguous <- info$ambiguous
    fields <- as_zoned_sys_time_from_naive_time_cpp(x, precision, zone, nonexistent, ambiguous)
  } else if (identical(method, "reference")) {
    reference <- info$reference
    ambiguous <- info$ambiguous
    fields <- as_zoned_sys_time_from_naive_time_with_reference_cpp(x, precision, zone, nonexistent, ambiguous, reference)
  } else {
    abort("Internal error: Unknown ambiguous handling method.")
  }

  new_zoned_time_from_fields(fields, precision, zone, names)
}

validate_nonexistent <- function(nonexistent, size) {
  nonexistent <- strict_validate_nonexistent(nonexistent)

  nonexistent_size <- vec_size(nonexistent)

  if (nonexistent_size != 1L && nonexistent_size != size) {
    abort(paste0("`nonexistent` must have length 1, or ", size, "."))
  }

  if (!is_character(nonexistent)) {
    abort("`nonexistent` must be a character vector, or `NULL`.")
  }

  nonexistent
}

validate_ambiguous <- function(ambiguous, size, zone) {
  if (is_null(ambiguous)) {
    ambiguous <- strict_validate_ambiguous(ambiguous)
    return(list(method = "string", ambiguous = ambiguous))
  }

  if (is_character(ambiguous)) {
    ambiguous <- validate_ambiguous_chr(ambiguous, size)
    return(list(method = "string", ambiguous = ambiguous))
  }

  if (is_zoned_time(ambiguous) || inherits(ambiguous, "POSIXt")) {
    # Implied `NULL`, to be validated by `strict_validate_ambiguous()`
    ambiguous <- list(ambiguous, NULL)
  }

  if (is_list(ambiguous)) {
    result <- validate_ambiguous_list(ambiguous, size, zone)
    reference <- result$reference
    ambiguous <- result$ambiguous
    return(list(method = "reference", reference = reference, ambiguous = ambiguous))
  }

  abort("`ambiguous` must be a character vector, a zoned-time, a POSIXct, or a list.")
}

validate_ambiguous_chr <- function(ambiguous, size) {
  ambiguous_size <- vec_size(ambiguous)

  if (ambiguous_size != 1L && ambiguous_size != size) {
    abort(paste0("`ambiguous` must have length 1, or ", size, "."))
  }

  ambiguous
}

validate_ambiguous_zoned <- function(ambiguous, size, zone) {
  # POSIXt -> zoned_time
  reference <- as_zoned_time(ambiguous)

  reference_size <- vec_size(reference)
  reference_zone <- zoned_time_zone(reference)

  if (reference_size != 1L && reference_size != size) {
    abort(paste0("A zoned-time or POSIXct `ambiguous` must have length 1, or ", size, "."))
  }
  if (reference_zone != zone) {
    abort("A zoned-time or POSIXct `ambiguous` must have the same zone as `zone`.")
  }

  # Force seconds precision to avoid the need for C++ templating
  sys_time <- as_sys_time(reference)
  sys_time <- time_point_floor(sys_time, "second")
  reference <- as_zoned_time(sys_time, reference_zone)

  reference
}

validate_ambiguous_list <- function(ambiguous, size, zone) {
  if (length(ambiguous) != 2L) {
    abort("A list `ambiguous` must have length 2.")
  }

  reference <- ambiguous[[1]]

  if (!is_zoned_time(reference) && !inherits(reference, "POSIXt")) {
    abort("The first element of a list `ambiguous` must be a zoned-time or POSIXt.")
  }

  reference <- validate_ambiguous_zoned(reference, size, zone)

  ambiguous <- ambiguous[[2]]

  if (is_null(ambiguous)) {
    ambiguous <- strict_validate_ambiguous(ambiguous)
  }
  if (!is_character(ambiguous)) {
    abort("The second element of a list `ambiguous` must be a character vector, or `NULL`.")
  }

  ambiguous <- validate_ambiguous_chr(ambiguous, size)

  list(reference = reference, ambiguous = ambiguous)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype2.clock_naive_time.clock_naive_time <- function(x, y, ...) {
  ptype2_time_point_and_time_point(x, y, ...)
}

#' @export
vec_cast.clock_naive_time.clock_naive_time <- function(x, to, ...) {
  cast_time_point_to_time_point(x, to, ...)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arith
#' @method vec_arith clock_naive_time
#' @export
vec_arith.clock_naive_time <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_naive_time", y)
}

#' @method vec_arith.clock_naive_time MISSING
#' @export
vec_arith.clock_naive_time.MISSING <- function(op, x, y, ...) {
  arith_time_point_and_missing(op, x, y, ...)
}

#' @method vec_arith.clock_naive_time clock_naive_time
#' @export
vec_arith.clock_naive_time.clock_naive_time <- function(op, x, y, ...) {
  arith_time_point_and_time_point(op, x, y, ...)
}

#' @method vec_arith.clock_naive_time clock_duration
#' @export
vec_arith.clock_naive_time.clock_duration <- function(op, x, y, ...) {
  arith_time_point_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration clock_naive_time
#' @export
vec_arith.clock_duration.clock_naive_time <- function(op, x, y, ...) {
  arith_duration_and_time_point(op, x, y, ...)
}

#' @method vec_arith.clock_naive_time numeric
#' @export
vec_arith.clock_naive_time.numeric <- function(op, x, y, ...) {
  arith_time_point_and_numeric(op, x, y, ...)
}

#' @method vec_arith.numeric clock_naive_time
#' @export
vec_arith.numeric.clock_naive_time <- function(op, x, y, ...) {
  arith_numeric_and_time_point(op, x, y, ...)
}
