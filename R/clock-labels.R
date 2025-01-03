#' Create or retrieve date related labels
#'
#' @description
#' When parsing and formatting dates, you often need to know how weekdays of
#' the week and months are represented as text. These functions allow you
#' to either create your own labels, or look them up from a standard set of
#' language specific labels. The standard list is derived from ICU
#' (<https://unicode-org.github.io/icu/>) via the stringi package.
#'
#' - `clock_labels_lookup()` looks up a set of labels from a
#'   given language code.
#'
#' - `clock_labels_languages()` lists the language codes that are accepted.
#'
#' - `clock_labels()` lets you create your own set of labels. Use this if the
#'   currently supported languages don't meet your needs.
#'
#' @param month,month_abbrev `[character(12)]`
#'
#'   Full and abbreviated month names. Starts with January.
#'
#' @param weekday,weekday_abbrev `[character(7)]`
#'
#'   Full and abbreviated weekday names. Starts with Sunday.
#'
#' @param am_pm `[character(2)]`
#'
#'   Names used for AM and PM.
#'
#' @return A `"clock_labels"` object.
#'
#' @export
#' @examples
#' clock_labels_lookup("en")
#' clock_labels_lookup("ko")
#' clock_labels_lookup("fr")
clock_labels <- function(
  month,
  month_abbrev = month,
  weekday,
  weekday_abbrev = weekday,
  am_pm
) {
  if (!is_character(month, n = 12L)) {
    abort("`month` must be a character vector of length 12.")
  }
  if (!is_character(month_abbrev, n = 12L)) {
    abort("`month_abbrev` must be a character vector of length 12.")
  }
  if (!is_character(weekday, n = 7L)) {
    abort("`weekday` must be a character vector of length 7.")
  }
  if (!is_character(weekday_abbrev, n = 7L)) {
    abort("`weekday_abbrev` must be a character vector of length 7.")
  }
  if (!is_character(am_pm, n = 2L)) {
    abort("`am_pm` must be a character vector of length 2.")
  }

  structure(
    list(
      month = enc2utf8(month),
      month_abbrev = enc2utf8(month_abbrev),
      weekday = enc2utf8(weekday),
      weekday_abbrev = enc2utf8(weekday_abbrev),
      am_pm = enc2utf8(am_pm)
    ),
    class = "clock_labels"
  )
}

#' @rdname clock_labels
#' @param language `[character(1)]`
#'
#'   A BCP 47 locale, generally constructed from a two or three
#'   digit language code. See `clock_labels_languages()` for a complete list of
#'   available locales.
#' @export
clock_labels_lookup <- function(language) {
  check_string(language)

  labels <- clock_labels_list[[language]]

  if (is.null(labels)) {
    cli::cli_abort("Can't find a locale for {.str {language}}.")
  }

  labels
}

#' @export
#' @rdname clock_labels
clock_labels_languages <- function() {
  names(clock_labels_list)
}

#' @export
print.clock_labels <- function(x, ...) {
  cat("<clock_labels>\n")

  if (identical(x$weekday, x$weekday_abbrev)) {
    weekday <- paste0(x$weekday, collapse = ", ")
  } else {
    weekday <- paste0(x$weekday, " (", x$weekday_abbrev, ")", collapse = ", ")
  }

  if (identical(x$month, x$month_abbrev)) {
    month <- paste0(x$month, collapse = ", ")
  } else {
    month <- paste0(x$month, " (", x$month_abbrev, ")", collapse = ", ")
  }

  am_pm <- paste0(x$am_pm, collapse = "/")

  cat_wrap("Weekdays: ", weekday)
  cat_wrap("Months:   ", month)
  cat_wrap("AM/PM:    ", am_pm)
}

check_clock_labels <- function(
  x,
  ...,
  arg = caller_arg(x),
  call = caller_env()
) {
  check_inherits(x, what = "clock_labels", arg = arg, call = call)
}

cat_wrap <- function(header, body) {
  body <- strwrap(body, exdent = nchar(header))
  cat(header, paste(body, collapse = "\n"), "\n", sep = "")
}
