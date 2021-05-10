#' Is `x` a zoned-time?
#'
#' This function determines if the input is a zoned-time object.
#'
#' @param x `[object]`
#'
#'   An object.
#'
#' @return `TRUE` if `x` inherits from `"clock_zoned_time"`, otherwise `FALSE`.
#'
#' @export
#' @examples
#' is_zoned_time(1)
#' is_zoned_time(zoned_time_now("America/New_York"))
is_zoned_time <- function(x) {
  inherits(x, "clock_zoned_time")
}

# ------------------------------------------------------------------------------

zoned_time_zone_attribute <- function(x) {
  attr(x, "zone", exact = TRUE)
}
zoned_time_set_zone_attribute <- function(x, zone) {
  attr(x, "zone") <- zone
  x
}

zoned_time_precision_attribute <- function(x) {
  attr(x, "precision", exact = TRUE)
}

# ------------------------------------------------------------------------------

#' Formatting: zoned-time
#'
#' @description
#' This is a zoned-time method for the [format()] generic.
#'
#' This function allows you to format a zoned-time using a flexible `format`
#' string.
#'
#' If `format` is `NULL`, a default format of `"%Y-%m-%d %H:%M:%S%Ez[%Z]"` is
#' used. This maximizes the chance for constructing a string that can be
#' reproducibly parsed into a valid zoned-time.
#'
#' @param x `[clock_zoned_time]`
#'
#'   A zoned-time.
#'
#' @param ... `[dots]`
#'
#'   Not used, but no error will be thrown if not empty to remain compatible
#'   with usage of the `format()` generic.
#'
#' @param format `[character(1) / NULL]`
#'
#'   If `NULL`, a default format is used, which depends on the type of the
#'   input.
#'
#'   Otherwise, a format string which is a combination of:
#'
#'   **Year**
#'
#'   - `%C`: The year divided by 100 using floored division. If the result
#'   is a single decimal digit, it is prefixed with `0`.
#'
#'   - `%y`: The last two decimal digits of the year. If the result is a single
#'   digit it is prefixed by `0`.
#'
#'   - `%Y`: The year as a decimal number. If the result is less than four
#'   digits it is left-padded with `0` to four digits.
#'
#'   **Month**
#'
#'   - `%b`, `%h`: The `locale`'s abbreviated month name.
#'
#'   - `%B`: The `locale`'s full month name.
#'
#'   - `%m`: The month as a decimal number. January is `01`. If the result is a
#'   single digit, it is prefixed with `0`.
#'
#'   **Day**
#'
#'   - `%d`: The day of month as a decimal number. If the result is a single
#'   decimal digit, it is prefixed with `0`.
#'
#'   **Day of the week**
#'
#'   - `%a`: The `locale`'s abbreviated weekday name.
#'
#'   - `%A`: The `locale`'s full weekday name.
#'
#'   - `%w`: The weekday as a decimal number (`0-6`), where Sunday is `0`.
#'
#'   **ISO 8601 week-based year**
#'
#'   - `%g`: The last two decimal digits of the ISO week-based year. If the
#'   result is a single digit it is prefixed by `0`.
#'
#'   - `%G`: The ISO week-based year as a decimal number. If the result is less
#'   than four digits it is left-padded with `0` to four digits.
#'
#'   - `%V`: The ISO week-based week number as a decimal number. If the result
#'   is a single digit, it is prefixed with `0`.
#'
#'   - `%u`: The ISO weekday as a decimal number (`1-7`), where Monday is `1`.
#'
#'   **Week of the year**
#'
#'   - `%U`: The week number of the year as a decimal number. The first Sunday
#'   of the year is the first day of week `01`. Days of the same year prior to
#'   that are in week `00`. If the result is a single digit, it is prefixed with
#'   `0`.
#'
#'   - `%W`: The week number of the year as a decimal number. The first Monday
#'   of the year is the first day of week `01`. Days of the same year prior to
#'   that are in week `00`. If the result is a single digit, it is prefixed with
#'   `0`.
#'
#'   **Day of the year**
#'
#'   - `%j`: The day of the year as a decimal number. January 1 is `001`. If the
#'   result is less than three digits, it is left-padded with `0` to three
#'   digits.
#'
#'   **Date**
#'
#'   - `%D`, `%x`: Equivalent to `%m/%d/%y`.
#'
#'   - `%F`: Equivalent to `%Y-%m-%d`.
#'
#'   **Time of day**
#'
#'   - `%H`: The hour (24-hour clock) as a decimal number. If the result is a
#'   single digit, it is prefixed with `0`.
#'
#'   - `%I`: The hour (12-hour clock) as a decimal number. If the result is a
#'   single digit, it is prefixed with `0`.
#'
#'   - `%M`: The minute as a decimal number. If the result is a single digit, it
#'   is prefixed with `0`.
#'
#'   - `%S`: Seconds as a decimal number. Fractional seconds are printed at the
#'   precision of the input. The character for the decimal point is localized
#'   according to `locale`.
#'
#'   - `%p`: The `locale`'s equivalent of the AM/PM designations associated with
#'   a 12-hour clock.
#'
#'   - `%R`: Equivalent to `%H:%M`.
#'
#'   - `%T`, `%X`: Equivalent to `%H:%M:%S`.
#'
#'   - `%r`: Nearly equivalent to `%I:%M:%S %p`, but seconds are always printed
#'   at second precision.
#'
#'   **Time zone**
#'
#'   - `%z`: The offset from UTC in the ISO 8601 format. For example `-0430`
#'   refers to 4 hours 30 minutes behind UTC. If the offset is zero, `+0000` is
#'   used. The modified command `%Ez` inserts a `:` between the hour and
#'   minutes, like `-04:30`.
#'
#'   - `%Z`: The full time zone name. If `abbreviate_zone` is `TRUE`, the time
#'   zone abbreviation.
#'
#'   **Miscellaneous**
#'
#'   - `%c`: A date and time representation. Similar to, but not exactly the
#'   same as, `%a %b %d %H:%M:%S %Y`.
#'
#'   - `%%`: A `%` character.
#'
#'   - `%n`: A newline character.
#'
#'   - `%t`: A horizontal-tab character.
#'
#' @param locale `[clock_locale]`
#'
#'   A locale object created from [clock_locale()].
#'
#' @param abbreviate_zone `[logical(1)]`
#'
#'   If `TRUE`, `%Z` returns an abbreviated time zone name.
#'
#'   If `FALSE`, `%Z` returns the full time zone name.
#'
#' @return A character vector of the formatted input.
#'
#' @export
#' @examples
#' x <- year_month_day(2019, 1, 1)
#' x <- as_zoned_time(as_naive_time(x), "America/New_York")
#'
#' format(x)
#' format(x, format = "%B %d, %Y")
#' format(x, format = "%B %d, %Y", locale = clock_locale("fr"))
format.clock_zoned_time <- function(x,
                                    ...,
                                    format = NULL,
                                    locale = clock_locale(),
                                    abbreviate_zone = FALSE) {
  if (!is_clock_locale(locale)) {
    abort("`locale` must be a 'clock_locale' object.")
  }

  zone <- zoned_time_zone_attribute(x)
  precision <- zoned_time_precision_attribute(x)

  if (is_null(format)) {
    # Collect internal option
    print_zone_name <- zoned_time_print_zone_name(...)
    format <- zoned_time_format(print_zone_name)
  }

  labels <- locale$labels
  decimal_mark <- locale$decimal_mark

  out <- format_zoned_time_cpp(
    fields = x,
    zone = zone,
    abbreviate_zone = abbreviate_zone,
    format = format,
    precision_int = precision,
    month = labels$month,
    month_abbrev = labels$month_abbrev,
    weekday = labels$weekday,
    weekday_abbrev = labels$weekday_abbrev,
    am_pm = labels$am_pm,
    decimal_mark = decimal_mark
  )

  names(out) <- clock_rcrd_names(x)

  out
}

zoned_time_print_zone_name <- function(..., print_zone_name = TRUE) {
  print_zone_name
}
zoned_time_format <- function(print_zone_name) {
  if (print_zone_name) {
    "%Y-%m-%d %H:%M:%S%Ez[%Z]"
  } else {
    "%Y-%m-%d %H:%M:%S%Ez"
  }
}

# ------------------------------------------------------------------------------

#' Parsing: zoned-time
#'
#' @description
#' There are two parsers into a zoned-time, `zoned_time_parse_complete()` and
#' `zoned_time_parse_abbrev()`.
#'
#' ## zoned_time_parse_complete()
#'
#' `zoned_time_parse_complete()` is a parser for _complete_ date-time strings,
#' like `"2019-01-01 00:00:00-05:00[America/New_York]"`. A complete date-time
#' string has both the time zone offset and full time zone name in the string,
#' which is the only way for the string itself to contain all of the information
#' required to construct a zoned-time. Because of this,
#' `zoned_time_parse_complete()` requires both the `%z` and `%Z` commands to be
#' supplied in the `format` string.
#'
#' The default options assume that `x` should be parsed at second precision,
#' using a `format` string of `"%Y-%m-%d %H:%M:%S%Ez[%Z]"`.
#'
#' ## zoned_time_parse_abbrev()
#'
#' `zoned_time_parse_abbrev()` is a parser for date-time strings containing only
#' a time zone abbreviation, like `"2019-01-01 00:00:00 EST"`. The time zone
#' abbreviation is not enough to identify the full time zone name that the
#' date-time belongs to, so the full time zone name must be supplied as the
#' `zone` argument. However, the time zone abbreviation can help with resolving
#' ambiguity around daylight saving time fallbacks.
#'
#' For `zoned_time_parse_abbrev()`, `%Z` must be supplied and is interpreted as
#' the time zone abbreviation rather than the full time zone name.
#'
#' If used, the `%z` command must parse correctly, but its value will be
#' completely ignored.
#'
#' The default options assume that `x` should be parsed at second precision,
#' using a `format` string of `"%Y-%m-%d %H:%M:%S %Z"`. This default format
#' generally matches what R prints out by default for POSIXct objects.
#'
#' @details
#' If `zoned_time_parse_complete()` is given input that is length zero, all
#' `NA`s, or completely fails to parse, then no time zone will be able to be
#' determined. In that case, the result will use `"UTC"`.
#'
#' If your date-time strings contain time zone offsets (like `-04:00`), but
#' not the full time zone name, you might need [sys_time_parse()].
#'
#' If your date-time strings don't contain time zone offsets or the full time
#' zone name, you might need to use [naive_time_parse()]. From there, if you
#' know the time zone that the date-times are supposed to be in, you can convert
#' to a zoned-time with [as_zoned_time()].
#'
#' @section Full Precision Parsing:
#'
#' It is highly recommended to parse all of the information in the date-time
#' string into a type at least as precise as the string. For example, if your
#' string has fractional seconds, but you only require seconds, specify a
#' sub-second `precision`, then round to seconds manually using whatever
#' convention is appropriate for your use case. Parsing such a string directly
#' into a second precision result is ambiguous and undefined, and is unlikely to
#' work as you might expect.
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param x `[character]`
#'
#'   A character vector to parse.
#'
#' @param zone `[character(1)]`
#'
#'   A full time zone name.
#'
#' @param format `[character / NULL]`
#'
#'   A format string. A combination of the following commands, or `NULL`,
#'   in which case a default format string is used.
#'
#'   A vector of multiple format strings can be supplied. They will be tried in
#'   the order they are provided.
#'
#'   **Year**
#'
#'   - `%C`: The century as a decimal number. The modified command `%NC` where
#'   `N` is a positive decimal integer specifies the maximum number of
#'   characters to read. If not specified, the default is `2`. Leading zeroes
#'   are permitted but not required.
#'
#'   - `%y`: The last two decimal digits of the year. If the century is not
#'   otherwise specified (e.g. with `%C`), values in the range `[69 - 99]` are
#'   presumed to refer to the years `[1969 - 1999]`, and values in the range
#'   `[00 - 68]` are presumed to refer to the years `[2000 - 2068]`. The
#'   modified command `%Ny`, where `N` is a positive decimal integer, specifies
#'   the maximum number of characters to read. If not specified, the default is
#'   `2`. Leading zeroes are permitted but not required.
#'
#'   - `%Y`: The year as a decimal number. The modified command `%NY` where `N`
#'   is a positive decimal integer specifies the maximum number of characters to
#'   read. If not specified, the default is `4`. Leading zeroes are permitted
#'   but not required.
#'
#'   **Month**
#'
#'   - `%b`, `%B`, `%h`: The `locale`'s full or abbreviated case-insensitive
#'   month name.
#'
#'   - `%m`: The month as a decimal number. January is `1`. The modified command
#'   `%Nm` where `N` is a positive decimal integer specifies the maximum number
#'   of characters to read. If not specified, the default is `2`. Leading zeroes
#'   are permitted but not required.
#'
#'   **Day**
#'
#'   - `%d`, `%e`: The day of the month as a decimal number. The modified
#'   command `%Nd` where `N` is a positive decimal integer specifies the maximum
#'   number of characters to read. If not specified, the default is `2`. Leading
#'   zeroes are permitted but not required.
#'
#'   **Day of the week**
#'
#'   - `%a`, `%A`: The `locale`'s full or abbreviated case-insensitive weekday
#'   name.
#'
#'   - `%w`: The weekday as a decimal number (`0-6`), where Sunday is `0`. The
#'   modified command `%Nw` where `N` is a positive decimal integer specifies
#'   the maximum number of characters to read. If not specified, the default is
#'   `1`. Leading zeroes are permitted but not required.
#'
#'   **ISO 8601 week-based year**
#'
#'   - `%g`: The last two decimal digits of the ISO week-based year. The
#'   modified command `%Ng` where `N` is a positive decimal integer specifies
#'   the maximum number of characters to read. If not specified, the default is
#'   `2`. Leading zeroes are permitted but not required.
#'
#'   - `%G`: The ISO week-based year as a decimal number. The modified command
#'   `%NG` where `N` is a positive decimal integer specifies the maximum number
#'   of characters to read. If not specified, the default is `4`. Leading zeroes
#'   are permitted but not required.
#'
#'   - `%V`: The ISO week-based week number as a decimal number. The modified
#'   command `%NV` where `N` is a positive decimal integer specifies the maximum
#'   number of characters to read. If not specified, the default is `2`. Leading
#'   zeroes are permitted but not required.
#'
#'   - `%u`: The ISO weekday as a decimal number (`1-7`), where Monday is `1`.
#'   The modified command `%Nu` where `N` is a positive decimal integer
#'   specifies the maximum number of characters to read. If not specified, the
#'   default is `1`. Leading zeroes are permitted but not required.
#'
#'   **Week of the year**
#'
#'   - `%U`: The week number of the year as a decimal number. The first Sunday
#'   of the year is the first day of week `01`. Days of the same year prior to
#'   that are in week `00`. The modified command `%NU` where `N` is a positive
#'   decimal integer specifies the maximum number of characters to read. If not
#'   specified, the default is `2`. Leading zeroes are permitted but not
#'   required.
#'
#'   - `%W`: The week number of the year as a decimal number. The first Monday
#'   of the year is the first day of week `01`. Days of the same year prior to
#'   that are in week `00`. The modified command `%NW` where `N` is a positive
#'   decimal integer specifies the maximum number of characters to read. If not
#'   specified, the default is `2`. Leading zeroes are permitted but not
#'   required.
#'
#'   **Day of the year**
#'
#'   - `%j`: The day of the year as a decimal number. January 1 is `1`. The
#'   modified command `%Nj` where `N` is a positive decimal integer specifies
#'   the maximum number of characters to read. If not specified, the default is
#'   `3`. Leading zeroes are permitted but not required.
#'
#'   **Date**
#'
#'   - `%D`, `%x`: Equivalent to `%m/%d/%y`.
#'
#'   - `%F`: Equivalent to `%Y-%m-%d`. If modified with a width (like `%NF`),
#'   the width is applied to only `%Y`.
#'
#'   **Time of day**
#'
#'   - `%H`: The hour (24-hour clock) as a decimal number. The modified command
#'   `%NH` where `N` is a positive decimal integer specifies the maximum number
#'   of characters to read. If not specified, the default is `2`. Leading zeroes
#'   are permitted but not required.
#'
#'   - `%I`: The hour (12-hour clock) as a decimal number. The modified command
#'   `%NI` where `N` is a positive decimal integer specifies the maximum number
#'   of characters to read. If not specified, the default is `2`. Leading zeroes
#'   are permitted but not required.
#'
#'   - `%M`: The minutes as a decimal number. The modified command `%NM` where
#'   `N` is a positive decimal integer specifies the maximum number of
#'   characters to read. If not specified, the default is `2`. Leading zeroes
#'   are permitted but not required.
#'
#'   - `%S`: The seconds as a decimal number. Leading zeroes are permitted but
#'   not required. If encountered, the `locale` determines the decimal point
#'   character. Generally, the maximum number of characters to read is
#'   determined by the precision that you are parsing at. For example, a
#'   precision of `"second"` would read a maximum of 2 characters, while a
#'   precision of `"millisecond"` would read a maximum of 6 (2 for the values
#'   before the decimal point, 1 for the decimal point, and 3 for the values
#'   after it). The modified command `%NS`, where `N` is a positive decimal
#'   integer, can be used to exactly specify the maximum number of characters to
#'   read. This is only useful if you happen to have seconds with more than 1
#'   leading zero.
#'
#'   - `%p`: The `locale`'s equivalent of the AM/PM designations associated with
#'   a 12-hour clock. The command `%I` must precede `%p` in the format string.
#'
#'   - `%R`: Equivalent to `%H:%M`.
#'
#'   - `%T`, `%X`: Equivalent to `%H:%M:%S`.
#'
#'   - `%r`: Equivalent to `%I:%M:%S %p`.
#'
#'   **Time zone**
#'
#'   - `%z`: The offset from UTC in the format `[+|-]hh[mm]`. For example
#'   `-0430` refers to 4 hours 30 minutes behind UTC. And `04` refers to 4 hours
#'   ahead of UTC. The modified command `%Ez` parses a `:` between the hours and
#'   minutes and leading zeroes on the hour field are optional:
#'   `[+|-]h[h][:mm]`. For example `-04:30` refers to 4 hours 30 minutes behind
#'   UTC. And `4` refers to 4 hours ahead of UTC.
#'
#'   - `%Z`: The full time zone name or the time zone abbreviation, depending on
#'   the function being used. A single word is parsed. This word can only
#'   contain characters that are alphanumeric, or one of `'_'`, `'/'`, `'-'` or
#'   `'+'`.
#'
#'   **Miscellaneous**
#'
#'   - `%c`: A date and time representation. Equivalent to
#'   `%a %b %d %H:%M:%S %Y`.
#'
#'   - `%%`: A `%` character.
#'
#'   - `%n`: Matches one white space character. `%n`, `%t`, and a space can be
#'   combined to match a wide range of white-space patterns. For example `"%n "`
#'   matches one or more white space characters, and `"%n%t%t"` matches one to
#'   three white space characters.
#'
#'   - `%t`: Matches zero or one white space characters.
#'
#' @param precision `[character(1)]`
#'
#'   A precision for the resulting zoned-time. One of:
#'
#'   - `"second"`
#'
#'   - `"millisecond"`
#'
#'   - `"microsecond"`
#'
#'   - `"nanosecond"`
#'
#'   Setting the `precision` determines how much information `%S` attempts
#'   to parse.
#'
#' @param locale `[clock_locale]`
#'
#'   A locale object created from [clock_locale()].
#'
#' @return A zoned-time.
#'
#' @name zoned-parsing
#'
#' @examples
#' library(magrittr)
#'
#' zoned_time_parse_complete("2019-01-01 01:02:03-05:00[America/New_York]")
#'
#' zoned_time_parse_complete(
#'   "January 21, 2019 -0500 America/New_York",
#'   format = "%B %d, %Y %z %Z"
#' )
#'
#' # Nanosecond precision
#' x <- "2019/12/31 01:05:05.123456700-05:00[America/New_York]"
#' zoned_time_parse_complete(
#'   x,
#'   format = "%Y/%m/%d %H:%M:%S%Ez[%Z]",
#'   precision = "nanosecond"
#' )
#'
#' # The `%z` offset must correspond to the true offset that would be used
#' # if the input was parsed as a naive-time and then converted to a zoned-time
#' # with `as_zoned_time()`. For example, the time that was parsed above used an
#' # offset of `-05:00`. We can confirm that this is correct with:
#' year_month_day(2019, 1, 1, 1, 2, 3) %>%
#'   as_naive_time() %>%
#'   as_zoned_time("America/New_York")
#'
#' # So the following would not parse correctly
#' zoned_time_parse_complete("2019-01-01 01:02:03-04:00[America/New_York]")
#'
#' # `%z` is useful for breaking ties in otherwise ambiguous times. For example,
#' # these two times are on either side of a daylight saving time fallback.
#' # Without the `%z` offset, you wouldn't be able to tell them apart!
#' x <- c(
#'   "1970-10-25 01:30:00-04:00[America/New_York]",
#'   "1970-10-25 01:30:00-05:00[America/New_York]"
#' )
#'
#' zoned_time_parse_complete(x)
#'
#' # If you have date-time strings with time zone abbreviations,
#' # `zoned_time_parse_abbrev()` should be able to help. The `zone` must be
#' # provided, because multiple countries may use the same time zone
#' # abbreviation. For example:
#' x <- "1970-01-01 02:30:30 IST"
#'
#' # IST = India Standard Time
#' zoned_time_parse_abbrev(x, "Asia/Kolkata")
#'
#' # IST = Israel Standard Time
#' zoned_time_parse_abbrev(x, "Asia/Jerusalem")
#'
#' # The time zone abbreviation is mainly useful for resolving ambiguity
#' # around daylight saving time fallbacks. Without the abbreviation, these
#' # date-times would be ambiguous.
#' x <- c(
#'   "1970-10-25 01:30:00 EDT",
#'   "1970-10-25 01:30:00 EST"
#' )
#' zoned_time_parse_abbrev(x, "America/New_York")
NULL

#' @rdname zoned-parsing
#' @export
zoned_time_parse_complete <- function(x,
                                      ...,
                                      format = NULL,
                                      precision = "second",
                                      locale = clock_locale()) {
  check_dots_empty()

  precision <- validate_zoned_time_precision_string(precision)

  if (!is_clock_locale(locale)) {
    abort("`locale` must be a 'clock_locale' object.")
  }

  if (is_null(format)) {
    # Use both %z and %Z
    format <- zoned_time_format(print_zone_name = TRUE)
  }

  labels <- locale$labels
  mark <- locale$decimal_mark

  result <- zoned_time_parse_complete_cpp(
    x,
    format,
    precision,
    labels$month,
    labels$month_abbrev,
    labels$weekday,
    labels$weekday_abbrev,
    labels$am_pm,
    mark
  )

  new_zoned_time_from_fields(result$fields, precision, result$zone, names(x))
}

#' @rdname zoned-parsing
#' @export
zoned_time_parse_abbrev <- function(x,
                                    zone,
                                    ...,
                                    format = NULL,
                                    precision = "second",
                                    locale = clock_locale()) {
  check_dots_empty()

  precision <- validate_zoned_time_precision_string(precision)

  if (!is_clock_locale(locale)) {
    abort("`locale` must be a 'clock_locale' object.")
  }

  if (is_null(format)) {
    # Like what R POSIXct prints
    format <- "%Y-%m-%d %H:%M:%S %Z"
  }

  labels <- locale$labels
  mark <- locale$decimal_mark

  fields <- zoned_time_parse_abbrev_cpp(
    x,
    zone,
    format,
    precision,
    labels$month,
    labels$month_abbrev,
    labels$weekday,
    labels$weekday_abbrev,
    labels$am_pm,
    mark
  )

  new_zoned_time_from_fields(fields, precision, zone, names(x))
}

# ------------------------------------------------------------------------------

# ptype2 / cast will prevent zoned times with different zones from being
# compared, so the equality proxy doesn't need to worry about the `zone`.

#' @export
vec_proxy.clock_zoned_time <- function(x, ...) {
  .Call(`_clock_clock_rcrd_proxy`, x)
}

#' @export
vec_restore.clock_zoned_time <- function(x, to, ...) {
  .Call(`_clock_zoned_time_restore`, x, to)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype_full.clock_zoned_time <- function(x, ...) {
  zone <- zone_pretty(zoned_time_zone_attribute(x))
  precision <- zoned_time_precision_attribute(x)
  precision <- precision_to_string(precision)
  paste0("zoned_time<", precision, "><", zone, ">")
}

#' @export
vec_ptype_abbr.clock_zoned_time <- function(x, ...) {
  zone <- zone_pretty(zoned_time_zone_attribute(x))
  precision <- zoned_time_precision_attribute(x)
  precision <- precision_to_string(precision)
  precision <- precision_abbr(precision)
  paste0("zt<", precision, "><", zone, ">")
}

zone_pretty <- function(zone) {
  if (identical(zone, "")) {
    zone <- zone_current()
    zone <- paste0(zone, " (current)")
  }
  zone
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype.clock_zoned_time <- function(x, ...) {
  zone <- zoned_time_zone_attribute(x)

  ptype_utc <- switch(
    zoned_time_precision_attribute(x) + 1L,
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    abort("Internal error: Invalid precision"),
    clock_empty_zoned_time_utc_second,
    clock_empty_zoned_time_utc_millisecond,
    clock_empty_zoned_time_utc_microsecond,
    clock_empty_zoned_time_utc_nanosecond,
    abort("Internal error: Invalid precision.")
  )

  ptype <- zoned_time_set_zone_attribute(ptype_utc, zone)

  ptype
}

#' @export
vec_ptype2.clock_zoned_time.clock_zoned_time <- function(x, y, ...) {
  x_zone <- zoned_time_zone_attribute(x)
  y_zone <- zoned_time_zone_attribute(y)

  if (x_zone != y_zone) {
    stop_incompatible_type(x, y, ..., details = "Zones can't differ.")
  }

  x_precision <- zoned_time_precision_attribute(x)
  y_precision <- zoned_time_precision_attribute(y)

  if (x_precision >= y_precision) {
    x
  } else {
    y
  }
}

#' @export
vec_cast.clock_zoned_time.clock_zoned_time <- function(x, to, ...) {
  x_zone <- zoned_time_zone_attribute(x)
  to_zone <- zoned_time_zone_attribute(to)

  if (x_zone != to_zone) {
    stop_incompatible_cast(x, to, ..., details = "Zones can't differ.")
  }

  x_precision <- zoned_time_precision_attribute(x)
  to_precision <- zoned_time_precision_attribute(to)

  if (x_precision == to_precision) {
    return(x)
  }

  if (x_precision > to_precision) {
    stop_incompatible_cast(x, to, ..., details = "Precision would be lost.")
  }

  fields <- duration_cast_cpp(x, x_precision, to_precision)

  names <- clock_rcrd_names(x)

  new_zoned_time_from_fields(fields, to_precision, x_zone, names)
}

# ------------------------------------------------------------------------------

#' @export
print.clock_zoned_time <- function(x, ..., max = NULL) {
  clock_print(x, max)
}

# - Pass through internal option to not print zone name
# - Unlike vctrs, don't use `print(quote = FALSE)` since we want to match base R
#' @export
obj_print_data.clock_zoned_time <- function(x, ..., max) {
  if (vec_is_empty(x)) {
    return(invisible(x))
  }

  x <- max_slice(x, max)

  out <- format(x, print_zone_name = FALSE)

  # Pass `max` to avoid base R's default footer
  print(out, max = max)

  invisible(x)
}

#' @export
obj_print_footer.clock_zoned_time <- function(x, ..., max) {
  clock_print_footer(x, max)
}

# Align left to match pillar_shaft.Date
# @export - lazy in .onLoad()
pillar_shaft.clock_zoned_time <- function(x, ...) {
  out <- format(x, print_zone_name = FALSE)
  pillar::new_pillar_shaft_simple(out, align = "left")
}

# ------------------------------------------------------------------------------

#' Convert to a zoned-time
#'
#' @description
#' `as_zoned_time()` converts `x` to a zoned-time. You generally convert
#' to a zoned time from either a sys-time or a naive time. Each are documented
#' on their own page:
#'
#' - [sys-time][as-zoned-time-sys-time]
#'
#' - [naive-time][as-zoned-time-naive-time]
#'
#' There are also convenience methods for converting to a zoned time from
#' native R date and date-time types:
#'
#' - [dates (Date)][as-zoned-time-Date]
#'
#' - [date-times (POSIXct / POSIXlt)][as-zoned-time-posixt]
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param x `[object]`
#'
#'   An object to convert to a zoned-time.
#'
#' @return A zoned-time vector.
#'
#' @export
#' @examples
#' x <- as.Date("2019-01-01")
#' as_zoned_time(x, "Europe/London")
#'
#' y <- as_naive_time(year_month_day(2019, 2, 1))
#' as_zoned_time(y, zone = "America/New_York")
as_zoned_time <- function(x, ...) {
  UseMethod("as_zoned_time")
}

#' @export
as_zoned_time.default <- function(x, ...) {
  stop_clock_unsupported_conversion(x, "clock_zoned_time")
}

#' @export
as_zoned_time.clock_zoned_time <- function(x, ...) {
  x
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_zoned_time <- function(x) {
  names <- clock_rcrd_names(x)
  precision <- zoned_time_precision_attribute(x)
  new_sys_time_from_fields(x, precision, names)
}

#' @export
as_naive_time.clock_zoned_time <- function(x) {
  names <- clock_rcrd_names(x)
  zone <- zoned_time_zone_attribute(x)
  precision <- zoned_time_precision_attribute(x)
  fields <- get_naive_time_cpp(x, precision, zone)
  new_naive_time_from_fields(fields, precision, names)
}

#' @export
as.character.clock_zoned_time <- function(x, ...) {
  format(x)
}

# ------------------------------------------------------------------------------

#' What is the current zoned-time?
#'
#' @description
#' `zoned_time_now()` returns the current time in the corresponding `zone`. It
#' is a wrapper around [sys_time_now()] that attaches the time zone.
#'
#' @details
#' The time is returned with a nanosecond precision, but the actual amount
#' of data returned is OS dependent. Usually, information at at least the
#' microsecond level is returned, with some platforms returning nanosecond
#' information.
#'
#' @param zone `[character(1)]`
#'
#'   A time zone to get the current time for.
#'
#' @return A zoned-time of the current time.
#'
#' @export
#' @examples
#' x <- zoned_time_now("America/New_York")
zoned_time_now <- function(zone) {
  names <- NULL
  sys_time <- sys_time_now()
  precision <- time_point_precision_attribute(sys_time)
  zone <- zone_validate(zone)
  new_zoned_time_from_fields(sys_time, precision, zone, names)
}

# ------------------------------------------------------------------------------

#' Get or set the time zone
#'
#' @description
#' `zoned_time_zone()` gets the time zone.
#'
#' `zoned_time_set_zone()` sets the time zone _without changing the
#' underlying instant_. This means that the result will represent the equivalent
#' time in the new time zone.
#'
#' @param x `[zoned_time / Date / POSIXt]`
#'
#'   A zoned time to get or set the time zone of.
#'
#' @param zone `[character(1)]`
#'
#'   A valid time zone to switch to.
#'
#' @return
#' `zoned_time_zone()` returns a string containing the time zone.
#'
#' `zoned_time_set_zone()` returns `x` with an altered time zone attribute. The
#' underlying instant is _not_ changed.
#'
#' @name zoned-zone
#'
#' @examples
#' x <- year_month_day(2019, 1, 1)
#' x <- as_zoned_time(as_naive_time(x), "America/New_York")
#' x
#'
#' zoned_time_zone(x)
#'
#' # Equivalent UTC time
#' zoned_time_set_zone(x, "UTC")
#'
#' # To force a new time zone with the same printed time,
#' # convert to a naive time that has no implied time zone,
#' # then convert back to a zoned time in the new time zone.
#' nt <- as_naive_time(x)
#' nt
#' as_zoned_time(nt, "UTC")
NULL

#' @rdname zoned-zone
#' @export
zoned_time_zone <- function(x) {
  UseMethod("zoned_time_zone")
}

#' @export
zoned_time_zone.clock_zoned_time <- function(x) {
  zoned_time_zone_attribute(x)
}

#' @rdname zoned-zone
#' @export
zoned_time_set_zone <- function(x, zone) {
  UseMethod("zoned_time_set_zone")
}

#' @export
zoned_time_set_zone.clock_zoned_time <- function(x, zone) {
  zone <- zone_validate(zone)
  zoned_time_set_zone_attribute(x, zone)
}

# ------------------------------------------------------------------------------

#' Precision: zoned-time
#'
#' `zoned_time_precision()` extracts the precision from a zoned-time. It
#' returns the precision as a single string.
#'
#' @param x `[clock_zoned_time]`
#'
#'   A zoned-time.
#'
#' @return A single string holding the precision of the zoned-time.
#'
#' @export
#' @examples
#' zoned_time_precision(zoned_time_now("America/New_York"))
zoned_time_precision <- function(x) {
  if (!is_zoned_time(x)) {
    abort("`x` must be a 'clock_zoned_time'.")
  }
  precision <- zoned_time_precision_attribute(x)
  precision <- precision_to_string(precision)
  precision
}

# ------------------------------------------------------------------------------

#' @export
add_years.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_years")
}

#' @export
add_quarters.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_quarters")
}

#' @export
add_months.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_months")
}

#' @export
add_weeks.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_weeks")
}

#' @export
add_days.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_days")
}

#' @export
add_hours.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_hours")
}

#' @export
add_minutes.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_minutes")
}

#' @export
add_seconds.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_seconds")
}

#' @export
add_milliseconds.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_milliseconds")
}

#' @export
add_microseconds.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_microseconds")
}

#' @export
add_nanoseconds.clock_zoned_time <- function(x, n, ...) {
  stop_clock_unsupported_zoned_time_op("add_nanoseconds")
}

# ------------------------------------------------------------------------------

zone_validate <- function(zone) {
  if (!is_string(zone)) {
    abort("`zone` must be a single string.")
  }

  if (!zone_is_valid(zone)) {
    message <- paste0("'", zone, "' is not a known time zone.")
    abort(message)
  }

  zone
}

# ------------------------------------------------------------------------------

validate_zoned_time_precision_string <- function(precision) {
  precision <- validate_precision_string(precision)

  if (!is_valid_zoned_time_precision(precision)) {
    abort("`precision` must be at least 'second' precision.")
  }

  precision
}

is_valid_zoned_time_precision <- function(precision) {
  precision >= PRECISION_SECOND
}

# ------------------------------------------------------------------------------

clock_init_zoned_time_utils <- function(env) {
  assign("clock_empty_zoned_time_utc_second", as_zoned_time(as_sys_time(duration_seconds()), "UTC"), envir = env)
  assign("clock_empty_zoned_time_utc_millisecond", as_zoned_time(as_sys_time(duration_milliseconds()), "UTC"), envir = env)
  assign("clock_empty_zoned_time_utc_microsecond", as_zoned_time(as_sys_time(duration_microseconds()), "UTC"), envir = env)
  assign("clock_empty_zoned_time_utc_nanosecond", as_zoned_time(as_sys_time(duration_nanoseconds()), "UTC"), envir = env)

  invisible(NULL)
}
