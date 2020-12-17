#' Combine format tokens together
#'
#' `tkns()` provides a simple way to combine multiple tokens or formats together
#' into a new format.
#'
#' @param ... Tokens or formats to combine. Can also contain simple strings.
#' @param sep Separator to use between tokens.
#'
#' @export
#' @examples
#' tkns(tkn_year(), tkn_month(), tkn_day(), sep = "-")
tkns <- function(..., sep = "") {
  paste(..., sep = sep)
}

# ------------------------------------------------------------------------------

#' Tokens for date-time formatting
#'
#' @description
#' This page contains a number of tokens that can be used for date-time
#' formatting through `format(format = )`. All tokens are supported for
#' formatting a clock class (such as [year_month_day()] or [zoned_date_time()]).
#' Most tokens are supported for formatting a POSIXct or Date, but some may
#' be specific to the formatting implementation used by clock.
#'
#' Calling any of these functions simply returns the corresponding string
#' for that token.
#'
#' There are also pre-generated formats for the most common cases, like
#' [fmt_ymd_hms()]. If you need to generate a custom format, multiple tokens
#' can be combined together using `tkns()`. You can also use `glue::glue()`,
#' which can often be more readable.
#'
#' @inheritParams ellipsis::dots_empty
#'
#' @param abbreviate `[logical(1)]`
#'
#'  Should the abbreviated version of this token be used?
#'
#' @param locale_era `[logical(1)]`
#'
#'  Should the alternative locale-dependent era format be used? This prepends
#'  an `E` before the standard token (i.e. `%EY` rather than `%Y`).
#'
#' @param locale_num `[logical(1)]`
#'
#'  Should the alternative locale-dependent number format be used? This prepends
#'  an `O` before the standard token (i.e. `%OH` rather than `%H`).
#'
#' @param pad `[character(1)]`
#'
#'  One of `"zero"` or `"space"`, specifying how to pad days of the month that
#'  have a single digit. For example, `"1"` would be padded to `"01"` or `" 1"`.
#'
#' @param short `[logical(1)]`
#'
#'  If `TRUE`, a two digit year will be printed.
#'
#'  If `FALSE`, a year with at least 4 digits will be printed.
#'
#' @param twelve `[logical(1)]`
#'
#'  If `TRUE`, a 12-hour clock will be used.
#'
#'  If `FALSE`, a 24-hour clock will be used.
#'
#' @param week_start_sunday `[logical(1)]`
#'
#'  If `TRUE`, the week starts on Sunday, which is the US convention.
#'
#'  If `FALSE`, the week starts on Monday, which is the UK convention.
#'
#' @param colon `[logical(1)]`
#'
#'  If `TRUE`, a `":"` will be added between the hours and minutes of the
#'  zone offset.
#'
#' @name clock-tokens
#'
#' @examples
#' # Tokens are just strings that start with a `%`.
#' tkn_day()
#' tkn_weekday()
#'
#' # Multiple tokens can be combined with `tkns()` to create custom formats
#' hms <- tkns(tkn_hour(), tkn_minute(), tkn_second(), sep = ":")
#'
#' # Formats created by tokens can be used in the format method of clock
#' # classes and base R date-time classes
#' x <- naive_datetime(2019, 01, 01, 14, 30, 59)
#' x
#'
#' format(x, format = hms)
#' format(x, format = tkn_time(twelve = TRUE))
#'
#' x_ct <- as.POSIXct(x, tz = "America/New_York")
#'
#' format(x_ct, format = hms)
NULL

#' @rdname clock-tokens
#' @export
tkn_weekday_name <- function(..., abbreviate = FALSE) {
  check_dots_empty()

  if (abbreviate) {
    "%a"
  } else {
    "%A"
  }
}

#' @rdname clock-tokens
#' @export
tkn_month_name <- function(..., abbreviate = FALSE) {
  check_dots_empty()

  if (abbreviate) {
    "%b"
  } else {
    "%B"
  }
}

#' @rdname clock-tokens
#' @export
tkn_date_and_time <- function(..., locale_era = FALSE) {
  check_dots_empty()

  if (locale_era) {
    "%Ec"
  } else {
    "%c"
  }
}

#' @rdname clock-tokens
#' @export
tkn_century <- function(..., locale_era = FALSE) {
  check_dots_empty()

  if (locale_era) {
    "%EC"
  } else {
    "%C"
  }
}

#' @rdname clock-tokens
#' @export
tkn_day <- function(..., pad = "zero", locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    if (pad == "zero") {
      "%Od"
    } else {
      "%Oe"
    }
  } else {
    if (pad == "zero") {
      "%d"
    } else {
      "%e"
    }
  }
}

#' @rdname clock-tokens
#' @export
tkn_iso_year <- function(..., short = FALSE) {
  check_dots_empty()

  if (short) {
    "%g"
  } else {
    "%G"
  }
}

#' @rdname clock-tokens
#' @export
tkn_hour <- function(..., twelve = FALSE, locale_num = FALSE) {
  check_dots_empty()

  if (twelve) {
    if (locale_num) {
      "%OI"
    } else {
      "%I"
    }
  } else {
    if (locale_num) {
      "%OH"
    } else {
      "%H"
    }
  }
}

#' @rdname clock-tokens
#' @export
tkn_day_of_year <- function() {
  "%j"
}

#' @rdname clock-tokens
#' @export
tkn_month <- function(..., locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    "%Om"
  } else {
    "%m"
  }
}

#' @rdname clock-tokens
#' @export
tkn_minute <- function() {
  "%M"
}

#' @rdname clock-tokens
#' @export
tkn_newline <- function() {
  "%n"
}

#' @rdname clock-tokens
#' @export
tkn_am_pm <- function() {
  "%p"
}

#' @rdname clock-tokens
#' @export
tkn_time <- function(..., twelve = FALSE) {
  check_dots_empty()

  if (twelve) {
    "%r"
  } else {
    "%T"
  }
}

#' @rdname clock-tokens
#' @export
tkn_second <- function(..., locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    "%OS"
  } else {
    "%S"
  }
}

#' @rdname clock-tokens
#' @export
tkn_tab <- function() {
  "%t"
}

#' @rdname clock-tokens
#' @export
tkn_iso_weekday <- function(..., locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    "%Ou"
  } else {
    "%u"
  }
}

#' @rdname clock-tokens
#' @export
tkn_weeknum <- function(..., locale_num = FALSE, week_start_sunday = TRUE) {
  check_dots_empty()

  if (week_start_sunday) {
    if (locale_num) {
      "%OU"
    } else {
      "%U"
    }
  } else {
    if (locale_num) {
      "%OW"
    } else {
      "%W"
    }
  }
}

#' @rdname clock-tokens
#' @export
tkn_iso_weeknum <- function(..., locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    "%OV"
  } else {
    "%V"
  }
}

#' @rdname clock-tokens
#' @export
tkn_weekday <- function(..., locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    "%Ow"
  } else {
    "%w"
  }
}

#' @rdname clock-tokens
#' @export
tkn_year <- function(..., short = FALSE) {
  check_dots_empty()

  if (short) {
    "%y"
  } else {
    "%Y"
  }
}

#' @rdname clock-tokens
#' @export
tkn_offset <- function(..., colon = TRUE) {
  check_dots_empty()

  if (colon) {
    "%Ez"
  } else {
    "%z"
  }
}

#' @rdname clock-tokens
#' @export
tkn_zone <- function() {
  "%Z"
}

#' @rdname clock-tokens
#' @export
tkn_percent <- function() {
  "%%"
}

# ------------------------------------------------------------------------------

#' Common date-time formats
#'
#' These functions are pre-generated formats created from combining multiple
#' [tokens][clock-tokens] with [tkns()]. You can use them directly as `format`s
#' in `format()` for clock classes or Date and POSIXct objects.
#'
#' @inheritParams ellipsis::dots_empty
#' @inheritParams tkn_offset
#'
#' @param year `[character(1)]`
#'
#'   The token to use to represent the year.
#'
#' @param month `[character(1)]`
#'
#'   The token to use to represent the month.
#'
#' @param day `[character(1)]`
#'
#'   The token to use to represent the day of the month.
#'
#' @param hour `[character(1)]`
#'
#'   The token to use to represent the hour.
#'
#' @param minute `[character(1)]`
#'
#'   The token to use to represent the minute.
#'
#' @param second `[character(1)]`
#'
#'   The token to use to represent the second.
#'
#' @param ymd `[character(1)]`
#'
#'   The format to use to represent the year, month, and day.
#'
#' @param hms `[character(1)]`
#'
#'   The format to use to represent the hour, minute, and second.
#'
#' @param sep `[character(1)]`
#'
#'   The separator to use between tokens.
#'
#' @param zone_name `[logical(1)]`
#'
#'   If `TRUE`, an extension to the ISO-8601 standard datetime format is used.
#'   This extension adds the time zone name or abbreviation to the end of the
#'   standard format as `[%Z]`.
#'
#'   This is most useful with the `format()` method for clock's zoned datetimes,
#'   as it can generate a textual representation of a zoned datetime that can
#'   be unambiguously parsed back into the original object.
#'
#' @name clock-formats
#' @examples
#' fmt_ymd_hms()
#'
#' x <- as.Date("2018-12-31")
#'
#' # ISO weeks are counted differently from normal weeks!
#' format(x, fmt_week_date())
#' format(x, fmt_iso_week_date())
#'
#' # Date that is "ambiguous" without more information about the DST offset
#' x <- zoned_datetime(
#'   1970, 10, 25, 01, 30,
#'   zone = "America/New_York",
#'   dst_ambiguous = c("earliest", "latest")
#' )
#' x
#'
#' # The ISO datetime format is incomplete because we don't know which
#' # time zone we came from
#' format(x, format = fmt_iso_datetime())
#'
#' # The default print method for POSIXct isn't much better, because the
#' # time zone abbreviation of "EST" is used by multiple different time zones,
#' # not just "America/New_York".
#' format(as.POSIXct(x), usetz = TRUE)
#'
#' # The only way to fully capture the date-time information is to use the
#' # zoned date-time format that prints both the offset and the zone name.
#' format(x, format = fmt_ymd_hms_zoned())
#'
#' # The default format method for zoned datetimes uses this extended format
#' format(x)
NULL

#' @rdname clock-formats
#' @export
fmt_ym <- function(...,
                   year = tkn_year(),
                   month = tkn_month(),
                   sep = "-") {
  check_dots_empty()
  tkns(year, month, sep = sep)
}

#' @rdname clock-formats
#' @export
fmt_ymd <- function(...,
                    year = tkn_year(),
                    month = tkn_month(),
                    day = tkn_day(),
                    sep = "-") {
  check_dots_empty()
  tkns(year, month, day, sep = sep)
}

#' @rdname clock-formats
#' @export
fmt_hms <- function(...,
                    hour = tkn_hour(),
                    minute = tkn_minute(),
                    second = tkn_second(),
                    sep = ":") {
  check_dots_empty()
  tkns(hour, minute, second, sep = sep)
}

#' @rdname clock-formats
#' @export
fmt_ymd_hms <- function(...,
                        ymd = fmt_ymd(),
                        hms = fmt_hms(),
                        sep = " ") {
  check_dots_empty()
  tkns(ymd, sep, hms)
}

#' @rdname clock-formats
#' @export
fmt_ymd_hms_zoned <- function(...,
                              ymd = fmt_ymd(),
                              hms = fmt_hms(),
                              sep = " ",
                              colon = TRUE,
                              zone_name = TRUE) {
  check_dots_empty()

  out <- tkns(ymd, sep, hms)
  out <- tkns(out, tkn_offset(colon = colon))

  if (zone_name) {
    out <- tkns(out, "[", tkn_zone(), "]")
  }

  out
}

#' @rdname clock-formats
#' @export
fmt_week_date <- function() {
  tkns(tkn_year(), "-", "W", tkn_weeknum(), "-", tkn_weekday())
}

#' @rdname clock-formats
#' @export
fmt_ordinal_date <- function() {
  tkns(tkn_year(), "-", tkn_day_of_year())
}

#' @rdname clock-formats
#' @export
fmt_iso_week_date <- function() {
  tkns(tkn_iso_year(), "-", "W", tkn_iso_weeknum(), "-", tkn_iso_weekday())
}

#' @rdname clock-formats
#' @export
fmt_iso_date <- function() {
  fmt_ymd()
}

#' @rdname clock-formats
#' @export
fmt_iso_datetime <- function() {
  fmt_ymd_hms_zoned(sep = "T", colon = FALSE, zone_name = FALSE)
}

#' @rdname clock-formats
#' @export
fmt_iso_year_weeknum_weekday <- function() {
  tkns(tkn_iso_year(), "-", "W", tkn_iso_weeknum(), "-", tkn_iso_weekday())
}

#' @rdname clock-formats
#' @export
fmt_iso_year_weeknum <- function() {
  tkns(tkn_iso_year(), "-", "W", tkn_iso_weeknum())
}

#' @rdname clock-formats
#' @export
fmt_year_month <- function() {
  fmt_ym()
}

#' @rdname clock-formats
#' @export
fmt_year_month_day <- function() {
  fmt_ymd()
}

#' @rdname clock-formats
#' @export
fmt_naive_datetime <- function() {
  fmt_ymd_hms()
}

#' @rdname clock-formats
#' @export
fmt_naive_nano_datetime <- function() {
  fmt_ymd_hms()
}

#' @rdname clock-formats
#' @export
fmt_zoned_datetime <- function(..., zone_name = TRUE) {
  check_dots_empty()
  fmt_ymd_hms_zoned(zone_name = zone_name)
}

#' @rdname clock-formats
#' @export
fmt_zoned_nano_datetime <- function(..., zone_name = TRUE) {
  check_dots_empty()
  fmt_ymd_hms_zoned(zone_name = zone_name)
}

