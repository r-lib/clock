#' @export
tkns <- function(..., sep = "") {
  paste(..., sep = sep)
}

# ------------------------------------------------------------------------------

#' @export
tkn_weekday_name <- function(..., abbreviate = FALSE) {
  check_dots_empty()

  if (abbreviate) {
    "%a"
  } else {
    "%A"
  }
}

#' @export
tkn_month_name <- function(..., abbreviate = FALSE) {
  check_dots_empty()

  if (abbreviate) {
    "%b"
  } else {
    "%B"
  }
}

#' @export
tkn_date_and_time <- function(..., locale_era = FALSE) {
  check_dots_empty()

  if (locale_era) {
    "%Ec"
  } else {
    "%c"
  }
}

#' @export
tkn_century <- function(..., locale_era = FALSE) {
  check_dots_empty()

  if (locale_era) {
    "%EC"
  } else {
    "%C"
  }
}

#' @export
tkn_day <- function(..., prefix = "zero", locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    if (prefix == "zero") {
      "%Od"
    } else {
      "%Oe"
    }
  } else {
    if (prefix == "zero") {
      "%d"
    } else {
      "%e"
    }
  }
}

#' @export
tkn_iso_year <- function(..., short = FALSE) {
  check_dots_empty()

  if (short) {
    "%g"
  } else {
    "%G"
  }
}

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

#' @export
tkn_day_of_year <- function() {
  "%j"
}

#' @export
tkn_month <- function(..., locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    "%Om"
  } else {
    "%m"
  }
}

#' @export
tkn_minute <- function() {
  "%M"
}

#' @export
tkn_newline <- function() {
  "%n"
}

#' @export
tkn_am_pm <- function() {
  "%p"
}

#' @export
tkn_time <- function(..., twelve = FALSE) {
  check_dots_empty()

  if (twelve) {
    "%r"
  } else {
    "%T"
  }
}

#' @export
tkn_second <- function(..., locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    "%OS"
  } else {
    "%S"
  }
}

#' @export
tkn_tab <- function() {
  "%t"
}

#' @export
tkn_iso_weekday <- function(..., locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    "%Ou"
  } else {
    "%u"
  }
}

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

#' @export
tkn_iso_weeknum <- function(..., locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    "%OV"
  } else {
    "%V"
  }
}

#' @export
tkn_weekday <- function(..., locale_num = FALSE) {
  check_dots_empty()

  if (locale_num) {
    "%Ow"
  } else {
    "%w"
  }
}

#' @export
tkn_year <- function(..., short = FALSE) {
  check_dots_empty()

  if (short) {
    "%y"
  } else {
    "%Y"
  }
}

#' @export
tkn_offset <- function(..., colon = FALSE) {
  check_dots_empty()

  if (colon) {
    "%Ez"
  } else {
    "%z"
  }
}

#' @export
tkn_zone <- function() {
  "%Z"
}

#' @export
tkn_percent <- function() {
  "%%"
}

# ------------------------------------------------------------------------------

#' @export
fmt_ym <- function(...,
                   year = tkn_year(),
                   month = tkn_month(),
                   sep = "-") {
  check_dots_empty()
  tkns(year, month, sep = sep)
}

#' @export
fmt_ymd <- function(...,
                    year = tkn_year(),
                    month = tkn_month(),
                    day = tkn_day(),
                    sep = "-") {
  check_dots_empty()
  tkns(year, month, day, sep = sep)
}

#' @export
fmt_hms <- function(...,
                    hour = tkn_hour(),
                    minute = tkn_minute(),
                    second = tkn_second(),
                    sep = ":") {
  check_dots_empty()
  tkns(hour, minute, second, sep = sep)
}

#' @export
fmt_ymd_hms <- function(...,
                        ymd = fmt_ymd(),
                        hms = fmt_hms(),
                        sep = "T") {
  check_dots_empty()
  tkns(ymd, sep, hms)
}

# ------------------------------------------------------------------------------

#' @export
fmt_week_date <- function() {
  tkns(tkn_year(), "-", "W", tkn_weeknum(), "-", tkn_weekday())
}

#' @export
fmt_ordinal_date <- function() {
  tkns(tkn_year(), "-", tkn_day_of_year())
}

#' @export
fmt_iso_week_date <- function() {
  tkns(tkn_iso_year(), "-", "W", tkn_iso_weeknum(), "-", tkn_iso_weekday())
}

#' @export
fmt_iso_date <- function() {
  fmt_ymd()
}

#' @export
fmt_iso_datetime <- function(..., colon = FALSE, extended = FALSE) {
  check_dots_empty()

  out <- tkns(fmt_ymd_hms(), tkn_offset(colon = colon))

  if (extended) {
    out <- tkns(out, "[", tkn_zone(), "]")
  }

  out
}

# ------------------------------------------------------------------------------

#' @export
fmt_local_year_month <- function() {
  fmt_ym()
}

#' @export
fmt_local_date <- function() {
  fmt_ymd()
}

#' @export
fmt_local_datetime <- function() {
  fmt_ymd_hms()
}

#' @export
fmt_local_nano_datetime <- function() {
  fmt_ymd_hms()
}

# ------------------------------------------------------------------------------

#' @export
fmt_zoned_datetime <- function(..., colon = TRUE, extended = FALSE) {
  fmt_iso_datetime(..., colon = colon, extended = extended)
}

#' @export
fmt_zoned_nano_datetime <- function(..., colon = TRUE, extended = FALSE) {
  fmt_iso_datetime(..., colon = colon, extended = extended)
}

