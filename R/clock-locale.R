#' Create a clock locale
#'
#' A clock locale contains the information required to format and parse dates.
#' The defaults have been chosen to match US English. A clock locale object can
#' be provided to `format()` methods or parse functions (like
#' [parse_year_month_day()]) to override the defaults. Alternatively, set the
#' `clock.default_clock_locale` global option to your preferred locale, and that
#' will be used as the default instead.
#'
#' @param mapping `[clock_mapping / character(1)]`
#'
#'   Character representations of localized weekday names, month names, and
#'   AM/PM names. Either the language code as string (passed on to
#'   [clock_mapping_lookup()]), or an object created by [clock_mapping()].
#'
#' @param decimal_mark `[character(1)]`
#'
#'   Symbol used for the decimal place when formatting sub-second date-times.
#'   Either `","` or `"."`.
#'
#' @export
#' @examples
#' clock_locale()
#' clock_locale(mapping = "fr")
#' default_clock_locale()
clock_locale <- function(mapping = "en", decimal_mark = ".") {
  if (is_character(mapping)) {
    mapping <- clock_mapping_lookup(mapping)
  }
  if (!is_clock_mapping(mapping)) {
    abort("`mapping` must be a 'clock_mapping' object.")
  }

  ok <- identical(decimal_mark, ".") || identical(decimal_mark, ",")
  if (!ok) {
    abort("`decimal_mark` must be either ',' or '.'.")
  }

  new_clock_locale(mapping, decimal_mark)
}

#' @rdname clock_locale
#' @export
default_clock_locale <- function() {
  loc <- getOption("clock.default_clock_locale", default = NULL)

  if (is_null(loc)) {
    loc <- clock_locale()
  }

  if (!is_clock_locale(loc)) {
    abort("Default locale object must be a 'clock_locale'.")
  }

  loc
}

new_clock_locale <- function(mapping, decimal_mark) {
  structure(
    list(
      mapping = mapping,
      decimal_mark = decimal_mark
    ),
    class = "clock_locale"
  )
}

#' @export
print.clock_locale <- function (x, ...) {
  cat("<clock_locale>\n")
  cat("Decimal Mark: ", x$decimal_mark, "\n", sep = "")
  print(x$mapping)
}

is_clock_locale <- function(x) {
  inherits(x, "clock_locale")
}
