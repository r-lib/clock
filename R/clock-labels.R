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
#' @param day,day_ab `[character(7)]`
#'
#'   Full and abbreviated week day names. Starts with Sunday.
#'
#' @param am_pm `[character(2)]`
#'
#'   Names used for AM and PM.
#'
#' @export
#' @examples
#' clock_labels_lookup("en")
#' clock_labels_lookup("ko")
#' clock_labels_lookup("fr")
clock_labels <- function(month,
                         month_abbrev = month,
                         day,
                         day_ab = day,
                         am_pm = c("AM", "PM")) {
  if (!is_character(month, n = 12L)) {
    abort("`month` must be a character vector of length 12.")
  }
  if (!is_character(month_abbrev, n = 12L)) {
    abort("`month_abbrev` must be a character vector of length 12.")
  }
  if (!is_character(day, n = 7L)) {
    abort("`day` must be a character vector of length 7.")
  }
  if (!is_character(day_ab, n = 7L)) {
    abort("`day_ab` must be a character vector of length 7.")
  }
  if (!is_character(am_pm, n = 2L)) {
    abort("`am_pm` must be a character vector of length 2.")
  }

  structure(
    list(
      month = enc2utf8(month),
      month_abbrev = enc2utf8(month_abbrev),
      day = enc2utf8(day),
      day_ab = enc2utf8(day_ab),
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
  if (!is_character(language, n = 1L)) {
    abort("`language` must be a character vector of length 1.")
  }

  labels <- clock_labels_list[[language]]

  if (is.null(labels)) {
    abort(paste0("Unknown language '", language, "'."))
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

  if (identical(x$day, x$day_ab)) {
    day <- paste0(x$day, collapse = ", ")
  } else {
    day <- paste0(x$day, " (", x$day_ab, ")", collapse = ", ")
  }

  if (identical(x$month, x$month_abbrev)) {
    month <- paste0(x$month, collapse = ", ")
  } else {
    month <- paste0(x$month, " (", x$month_abbrev, ")", collapse = ", ")
  }

  am_pm <- paste0(x$am_pm, collapse = "/")

  cat_wrap("Days:   ", day)
  cat_wrap("Months: ", month)
  cat_wrap("AM/PM:  ", am_pm)
}

is_clock_labels <- function(x) {
  inherits(x, "clock_labels")
}

cat_wrap <- function(header, body) {
  body <- strwrap(body, exdent = nchar(header))
  cat(header, paste(body, collapse = "\n"), "\n", sep = "")
}
