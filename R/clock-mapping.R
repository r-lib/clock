#' Create or retrieve date related mappings
#'
#' @description
#' When parsing and formatting dates, you often need to know how weekdays of
#' the week and months are represented as text. These functions allow you
#' to either create your own mapping, or look one up from a standard set of
#' language specific mappings. The standard list is derived from ICU
#' (<https://unicode-org.github.io/icu/>) via the stringi package.
#'
#' - `clock_mapping_lookup()` looks up a set of mapping information from a
#'   given language code.
#'
#' - `clock_mapping_languages()` lists the language codes that are accepted.
#'
#' - `clock_mapping()` lets you create your own mapping. Use this if the
#'   currently supported languages don't meet your needs.
#'
#' @param mon,mon_ab `[character(12)]`
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
#' clock_mapping_lookup("en")
#' clock_mapping_lookup("ko")
#' clock_mapping_lookup("fr")
clock_mapping <- function(mon,
                          mon_ab = mon,
                          day,
                          day_ab = day,
                          am_pm = c("AM", "PM")) {
  if (!is_character(mon, n = 12L)) {
    abort("`mon` must be a character vector of length 12.")
  }
  if (!is_character(mon_ab, n = 12L)) {
    abort("`mon_ab` must be a character vector of length 12.")
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
      mon = enc2utf8(mon),
      mon_ab = enc2utf8(mon_ab),
      day = enc2utf8(day),
      day_ab = enc2utf8(day_ab),
      am_pm = enc2utf8(am_pm)
    ),
    class = "clock_mapping"
  )
}

#' @rdname clock_mapping
#' @param language `[character(1)]`
#'
#'   A BCP 47 locale, generally constructed from a two or three
#'   digit language code. See `clock_mapping_languages()` for a complete list of
#'   available locales.
#' @export
clock_mapping_lookup <- function(language) {
  if (!is_character(language, n = 1L)) {
    abort("`language` must be a character vector of length 1.")
  }

  mapping <- clock_mappings[[language]]

  if (is.null(mapping)) {
    abort(paste0("Unknown language '", language, "'."))
  }

  mapping
}

#' @export
#' @rdname clock_mapping
clock_mapping_languages <- function() {
  names(clock_mappings)
}

#' @export
print.clock_mapping <- function(x, ...) {
  cat("<clock_mapping>\n")

  if (identical(x$day, x$day_ab)) {
    day <- paste0(x$day, collapse = ", ")
  } else {
    day <- paste0(x$day, " (", x$day_ab, ")", collapse = ", ")
  }

  if (identical(x$mon, x$mon_ab)) {
    mon <- paste0(x$mon, collapse = ", ")
  } else {
    mon <- paste0(x$mon, " (", x$mon_ab, ")", collapse = ", ")
  }

  am_pm <- paste0(x$am_pm, collapse = "/")

  cat_wrap("Days:   ", day)
  cat_wrap("Months: ", mon)
  cat_wrap("AM/PM:  ", am_pm)
}

is_clock_mapping <- function(x) {
  inherits(x, "clock_mapping")
}

cat_wrap <- function(header, body) {
  body <- strwrap(body, exdent = nchar(header))
  cat(header, paste(body, collapse = "\n"), "\n", sep = "")
}
