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

check_naive_time <- function(x, ..., arg = caller_arg(x), call = caller_env()) {
  check_inherits(x, what = "clock_naive_time", arg = arg, call = call)
}

# ------------------------------------------------------------------------------

#' Parsing: naive-time
#'
#' @description
#' `naive_time_parse()` is a parser into a naive-time.
#'
#' `naive_time_parse()` is useful when you have date-time strings like
#' `"2020-01-01T01:04:30"`. If there is no attached UTC offset or time zone
#' name, then parsing this string as a naive-time is your best option. If
#' you know that this string should be interpreted in a specific time zone,
#' parse as a naive-time, then use [as_zoned_time()].
#'
#' The default options assume that `x` should be parsed at second precision,
#' using a `format` string of `"%Y-%m-%dT%H:%M:%S"`. This matches the default
#' result from calling `format()` on a naive-time.
#'
#' _`naive_time_parse()` ignores both the `%z` and `%Z` commands._
#'
#' If your date-time strings contain a full time zone name and a UTC offset, use
#' [zoned_time_parse_complete()]. If they contain a time zone abbreviation, use
#' [zoned_time_parse_abbrev()].
#'
#' If your date-time strings contain a UTC offset, but not a full time zone
#' name, use [sys_time_parse()].
#'
#' @inheritSection zoned-parsing Full Precision Parsing
#'
#' @inheritParams sys_time_parse
#'
#' @return A naive-time.
#'
#' @export
#' @examples
#' naive_time_parse("2020-01-01T05:06:07")
#'
#' # Day precision
#' naive_time_parse("2020-01-01", precision = "day")
#'
#' # Nanosecond precision, but using a day based format
#' naive_time_parse("2020-01-01", format = "%Y-%m-%d", precision = "nanosecond")
#'
#' # Remember that the `%z` and `%Z` commands are ignored entirely!
#' naive_time_parse(
#'   "2020-01-01 -4000 America/New_York",
#'   format = "%Y-%m-%d %z %Z"
#' )
#'
#' # ---------------------------------------------------------------------------
#' # Fractional seconds and POSIXct
#'
#' # If you have a string with fractional seconds and want to convert it to
#' # a POSIXct, remember that clock treats POSIXct as a second precision type.
#' # Ideally, you'd use a clock type that can support fractional seconds, but
#' # if you really want to parse it into a POSIXct, the correct way to do so
#' # is to parse the full fractional time point with the correct `precision`,
#' # then round to seconds using whatever convention you require, and finally
#' # convert that to POSIXct.
#' x <- c("2020-01-01T00:00:00.123", "2020-01-01T00:00:00.555")
#'
#' # First, parse string with full precision
#' x <- naive_time_parse(x, precision = "millisecond")
#' x
#'
#' # Then round to second with a floor, ceiling, or round to nearest
#' time_point_floor(x, "second")
#' time_point_round(x, "second")
#'
#' # Finally, convert to POSIXct
#' as_date_time(time_point_round(x, "second"), zone = "UTC")
naive_time_parse <- function(x,
                             ...,
                             format = NULL,
                             precision = "second",
                             locale = clock_locale()) {
  check_dots_empty0(...)
  check_time_point_precision(precision)
  precision <- precision_to_integer(precision)

  fields <- time_point_parse(
    x = x,
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
#' @inheritParams rlang::args_dots_empty
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
as_naive_time <- function(x, ...) {
  UseMethod("as_naive_time")
}

#' @export
as_naive_time.default <- function(x, ...) {
  stop_clock_unsupported(x)
}

#' @export
as_naive_time.clock_naive_time <- function(x, ...) {
  check_dots_empty0(...)
  x
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_naive_time <- function(x, ...) {
  check_dots_empty0(...)
  new_sys_time_from_fields(x, time_point_precision_attribute(x), clock_rcrd_names(x))
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
#' @inheritParams rlang::args_dots_empty
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
#' shifted <- as_zoned_time(
#'   nonexistent_time,
#'   new_york,
#'   nonexistent = "shift-forward"
#' )
#' shifted
#'
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
#' try(as_zoned_time(x_naive, zoned_time_zone(x)))
#'
#' # To get around this, you can use that information by specifying
#' # `ambiguous = x`, which will use the offset from `x` to resolve the
#' # ambiguity in `x_naive` as long as `x` is also an ambiguous time with the
#' # same daylight saving time transition point as `x_naive` (i.e. here
#' # everything has a transition point of `"2020-11-01 01:00:00 EST"`).
#' as_zoned_time(x_naive, zoned_time_zone(x), ambiguous = x)
#'
#' # Say you added one more time to `x` that would not be considered ambiguous
#' # in naive-time
#' x <- c(x, as_zoned_time(as_sys_time(latest) + 3600, zoned_time_zone(latest)))
#' x
#'
#' # Imagine you want to floor this vector to a multiple of 2 hours, with
#' # an origin of 1am that day. You can do this by subtracting the origin,
#' # flooring, then adding it back
#' origin <- year_month_day(2019, 11, 01, 01, 00, 00) %>%
#'   as_naive_time() %>%
#'   as_duration()
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
#' try(as_zoned_time(x_naive, zoned_time_zone(x), ambiguous = x))
#'
#' # When we floored from 02:30:00 -> 01:00:00, we went from being
#' # unambiguous -> ambiguous. In clock, this is something you must handle
#' # explicitly, and cannot be handled by using information from `x`. You can
#' # handle this while still retaining the behavior for the other two
#' # time points that were ambiguous before and after the floor by passing a
#' # list containing `x` and an ambiguous time resolution strategy to use
#' # when information from `x` can't resolve ambiguities:
#' as_zoned_time(x_naive, zoned_time_zone(x), ambiguous = list(x, "latest"))
as_zoned_time.clock_naive_time <- function(x,
                                           zone,
                                           ...,
                                           nonexistent = NULL,
                                           ambiguous = NULL) {
  check_dots_empty0(...)

  check_zone(zone)

  # Promote to at least seconds precision for `zoned_time`
  ptype <- vec_ptype2(x, clock_empty_naive_time_second, y_arg = "")
  x <- vec_cast(x, ptype)

  size <- vec_size(x)
  precision <- time_point_precision_attribute(x)
  names <- clock_rcrd_names(x)

  nonexistent <- check_nonexistent(nonexistent, size)

  info <- check_ambiguous(ambiguous, size, zone)
  method <- info$method

  if (identical(method, "string")) {
    ambiguous <- info$ambiguous
    fields <- as_zoned_sys_time_from_naive_time_cpp(x, precision, zone, nonexistent, ambiguous, current_env())
  } else if (identical(method, "reference")) {
    reference <- info$reference
    ambiguous <- info$ambiguous
    fields <- as_zoned_sys_time_from_naive_time_with_reference_cpp(x, precision, zone, nonexistent, ambiguous, reference, current_env())
  } else {
    abort("Internal error: Unknown ambiguous handling method.")
  }

  new_zoned_time_from_fields(fields, precision, zone, names)
}

check_nonexistent <- function(nonexistent, size, ..., call = caller_env()) {
  check_dots_empty0(...)

  nonexistent <- check_nonexistent_strict(nonexistent, call = call)

  nonexistent_size <- vec_size(nonexistent)

  if (nonexistent_size != 1L && nonexistent_size != size) {
    cli::cli_abort("{.arg nonexistent} must have length 1 or {size}.", call = call)
  }

  check_character(nonexistent, allow_null = TRUE, call = call)

  nonexistent
}

check_ambiguous <- function(ambiguous, size, zone, ..., call = caller_env()) {
  check_dots_empty0(...)

  if (is_null(ambiguous)) {
    ambiguous <- check_ambiguous_strict(ambiguous, call = call)
    return(list(method = "string", ambiguous = ambiguous))
  }

  if (is_character(ambiguous)) {
    ambiguous <- check_ambiguous_chr(ambiguous, size, call = call)
    return(list(method = "string", ambiguous = ambiguous))
  }

  if (is_zoned_time(ambiguous) || inherits(ambiguous, "POSIXt")) {
    # Implied `NULL`, to be validated by `check_ambiguous_strict()`
    ambiguous <- list(ambiguous, NULL)
  }

  if (is_list(ambiguous)) {
    result <- check_ambiguous_list(ambiguous, size, zone, call = call)
    reference <- result$reference
    ambiguous <- result$ambiguous
    return(list(method = "reference", reference = reference, ambiguous = ambiguous))
  }

  cli::cli_abort(
    "{.arg ambiguous} must be a character vector, a zoned-time, a POSIXct, or a list, not {.obj_type_friendly {ambiguous}}.",
    call = call
  )
}

check_ambiguous_chr <- function(ambiguous, size, call) {
  ambiguous_size <- vec_size(ambiguous)

  if (ambiguous_size != 1L && ambiguous_size != size) {
    cli::cli_abort("{.arg ambiguous} must have length 1 or {size}.", call = call)
  }

  ambiguous
}

check_ambiguous_zoned <- function(ambiguous, size, zone, call) {
  # POSIXt -> zoned_time
  reference <- as_zoned_time(ambiguous)

  reference_size <- vec_size(reference)
  reference_zone <- zoned_time_zone_attribute(reference)

  if (reference_size != 1L && reference_size != size) {
    cli::cli_abort("A zoned-time or POSIXct {.arg ambiguous} must have length 1 or {size}.", call = call)
  }
  if (reference_zone != zone) {
    cli::cli_abort("A zoned-time or POSIXct {.arg ambiguous} must have the same zone as {.arg zone}.", call = call)
  }

  # Force seconds precision to avoid the need for C++ templating
  sys_time <- as_sys_time(reference)
  sys_time <- time_point_floor(sys_time, "second")
  reference <- as_zoned_time(sys_time, reference_zone)

  reference
}

check_ambiguous_list <- function(ambiguous, size, zone, call) {
  if (length(ambiguous) != 2L) {
    cli::cli_abort("A list {.arg ambiguous} must have length 2.", call = call)
  }

  reference <- ambiguous[[1]]

  if (!is_zoned_time(reference) && !inherits(reference, "POSIXt")) {
    cli::cli_abort("The first element of a list {.arg ambiguous} must be a zoned-time or POSIXt.", call = call)
  }

  reference <- check_ambiguous_zoned(reference, size, zone, call = call)

  ambiguous <- ambiguous[[2]]

  if (is_null(ambiguous)) {
    ambiguous <- check_ambiguous_strict(ambiguous, call = call)
  }
  if (!is_character(ambiguous)) {
    cli::cli_abort("The second element of a list {.arg ambiguous} must be a character vector, or `NULL`.", call = call)
  }

  ambiguous <- check_ambiguous_chr(ambiguous, size, call = call)

  list(reference = reference, ambiguous = ambiguous)
}


#' @export
as.character.clock_naive_time <- function(x, ...) {
  format(x)
}

# ------------------------------------------------------------------------------

#' Info: naive-time
#'
#' @description
#' `naive_time_info()` retrieves a set of low-level information generally not
#' required for most date-time manipulations. It is used implicitly
#' by `as_zoned_time()` when converting from a naive-time.
#'
#' It returns a data frame with the following columns:
#'
#' - `type`: A character vector containing one of:
#'
#'   - `"unique"`: The naive-time maps uniquely to a zoned-time that can be
#'   created with `zone`.
#'
#'   - `"nonexistent"`: The naive-time does not exist as a zoned-time that can
#'   be created with `zone`.
#'
#'   - `"ambiguous"`: The naive-time exists twice as a zoned-time that can be
#'   created with `zone`.
#'
#' - `first`: A [sys_time_info()] data frame.
#'
#' - `second`: A [sys_time_info()] data frame.
#'
#' ## type == "unique"
#'
#' - `first` will be filled out with sys-info representing daylight saving time
#' information for that time point in `zone`.
#'
#' - `second` will contain only `NA` values, as there is no ambiguity to
#' represent information for.
#'
#' ## type == "nonexistent"
#'
#' - `first` will be filled out with the sys-info that ends just prior to `x`.
#'
#' - `second` will be filled out with the sys-info that begins just after `x`.
#'
#' ## type == "ambiguous"
#'
#' - `first` will be filled out with the sys-info that ends just after `x`.
#'
#' - `second` will be filled out with the sys-info that starts just before `x`.
#'
#' @details
#' If the tibble package is installed, it is recommended to convert the output
#' to a tibble with `as_tibble()`, as that will print the df-cols much nicer.
#'
#' @param x `[clock_naive_time]`
#'
#'   A naive-time.
#'
#' @param zone `[character]`
#'
#'   A valid time zone name.
#'
#'   Unlike most functions in clock, in `naive_time_info()` `zone` is vectorized
#'   and is recycled against `x`.
#'
#' @return A data frame of low level information.
#'
#' @export
#' @examples
#' library(vctrs)
#'
#' x <- year_month_day(1970, 04, 26, 02, 30, 00)
#' x <- as_naive_time(x)
#'
#' # Maps uniquely to a time in London
#' naive_time_info(x, "Europe/London")
#'
#' # This naive-time never existed in New York!
#' # A DST gap jumped the time from 01:59:59 -> 03:00:00,
#' # skipping the 2 o'clock hour
#' zone <- "America/New_York"
#' info <- naive_time_info(x, zone)
#' info
#'
#' # You can recreate various `nonexistent` strategies with this info
#' as_zoned_time(x, zone, nonexistent = "roll-forward")
#' as_zoned_time(info$first$end, zone)
#'
#' as_zoned_time(x, zone, nonexistent = "roll-backward")
#' as_zoned_time(info$first$end - 1, zone)
#'
#' as_zoned_time(x, zone, nonexistent = "shift-forward")
#' as_zoned_time(as_sys_time(x) - info$first$offset, zone)
#'
#' as_zoned_time(x, zone, nonexistent = "shift-backward")
#' as_zoned_time(as_sys_time(x) - info$second$offset, zone)
#'
#' # ---------------------------------------------------------------------------
#' # Normalizing to UTC
#'
#' # Imagine you had the following printed times, and knowledge that they
#' # are to be interpreted as in the corresponding time zones
#' df <- data_frame(
#'   x = c("2020-01-05 02:30:00", "2020-06-03 12:20:05"),
#'   zone = c("America/Los_Angeles", "Europe/London")
#' )
#'
#' # The times are assumed to be naive-times, i.e. if you lived in the `zone`
#' # at the moment the time was recorded, then you would have seen that time
#' # printed on the clock. Currently, these are strings. To convert them to
#' # a time based type, you'll have to acknowledge that R only lets you have
#' # 1 time zone in a vector of date-times at a time. So you'll need to
#' # normalize these naive-times. The easiest thing to normalize them to
#' # is UTC.
#' df$naive <- naive_time_parse(df$x)
#'
#' # Get info about the naive times using a vector of zones
#' info <- naive_time_info(df$naive, df$zone)
#' info
#'
#' # We'll assume that some system generated these naive-times with no
#' # chance of them ever being nonexistent or ambiguous. So now all we have
#' # to do is use the offset to convert the naive-time to a sys-time. The
#' # relationship used is:
#' # offset = naive_time - sys_time
#' df$sys <- as_sys_time(df$naive) - info$first$offset
#' df
#'
#' # At this point, both times are in UTC. From here, you can convert them
#' # both to either America/Los_Angeles or Europe/London as required.
#' as_zoned_time(df$sys, "America/Los_Angeles")
#' as_zoned_time(df$sys, "Europe/London")
naive_time_info <- function(x, zone) {
  check_naive_time(x)
  check_character(zone)

  precision <- time_point_precision_attribute(x)

  # Recycle `x` to the common size. `zone` is recycled internally as required,
  # which is more efficient than reloading the time zone repeatedly.
  size <- vec_size_common(x = x, zone = zone)
  x <- vec_recycle(x, size)

  fields <- naive_time_info_cpp(x, precision, zone)

  new_naive_time_info_from_fields(fields)
}

new_naive_time_info_from_fields <- function(fields) {
  fields[["first"]] <- new_sys_time_info_from_fields(fields[["first"]])
  fields[["second"]] <- new_sys_time_info_from_fields(fields[["second"]])
  new_data_frame(fields)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype_full.clock_naive_time <- function(x, ...) {
  time_point_ptype(x, type = "full")
}

#' @export
vec_ptype_abbr.clock_naive_time <- function(x, ...) {
  time_point_ptype(x, type = "abbr")
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype.clock_naive_time <- function(x, ...) {
  switch(
    time_point_precision_attribute(x) + 1L,
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    clock_empty_naive_time_day,
    clock_empty_naive_time_hour,
    clock_empty_naive_time_minute,
    clock_empty_naive_time_second,
    clock_empty_naive_time_millisecond,
    clock_empty_naive_time_microsecond,
    clock_empty_naive_time_nanosecond,
    abort("Internal error: Invalid precision.")
  )
}

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

# ------------------------------------------------------------------------------

clock_init_naive_time_utils <- function(env) {
  day <- as_naive_time(year_month_day(integer(), integer(), integer()))

  assign("clock_empty_naive_time_day", day, envir = env)
  assign("clock_empty_naive_time_hour", time_point_cast(day, "hour"), envir = env)
  assign("clock_empty_naive_time_minute", time_point_cast(day, "minute"), envir = env)
  assign("clock_empty_naive_time_second", time_point_cast(day, "second"), envir = env)
  assign("clock_empty_naive_time_millisecond", time_point_cast(day, "millisecond"), envir = env)
  assign("clock_empty_naive_time_microsecond", time_point_cast(day, "microsecond"), envir = env)
  assign("clock_empty_naive_time_nanosecond", time_point_cast(day, "nanosecond"), envir = env)

  invisible(NULL)
}
