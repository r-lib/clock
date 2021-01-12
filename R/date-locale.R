#' Create a date locale
#'
#' A date locale contains the information required to format and parse dates and
#' date times. The defaults have been chosen to match US English. A date locale
#' object can be provided to `format()` methods to override the defaults.
#' Alternatively, set the `clock.default_date_locale` global option to your
#' preferred date locale, and that will be used as the default instead.
#'
#' @param date_names Character representations of localized weekday names,
#'   month names, and AM/PM names. Either the language code as string
#'   (passed on to [date_names_lang()]) or an object created by [date_names()].
#'
#' @param decimal_mark Symbol used for the decimal place when formatting
#'   sub-second date-times. Either `","` or `"."`.
#'
#' @export
#' @examples
#' date_locale()
#' date_locale(date_names = "fr")
#' default_date_locale()
date_locale <- function(date_names = "en", decimal_mark = ".") {
  if (is.character(date_names)) {
    date_names <- date_names_lang(date_names)
  }
  if (!is.date_names(date_names)) {
    abort("`date_names` must be a 'date.names' object.")
  }

  ok <- decimal_mark %in% c(".", ",")
  if (!ok) {
    abort("`decimal_mark` must be either ',' or '.'.")
  }

  new_date_locale(date_names, decimal_mark)
}

#' @rdname date_locale
#' @export
default_date_locale <- function() {
  loc <- getOption("clock.default_date_locale", default = NULL)
  if (is.null(loc)) {
    loc <- date_locale()
    options(clock.default_date_locale = loc)
  }
  loc
}

new_date_locale <- function(date_names = date_names_lang("en"), decimal_mark = ".") {
  structure(list(date_names = date_names, decimal_mark = decimal_mark), class = "clock_date_locale")
}

#' @export
print.clock_date_locale <- function (x, ...) {
  cat("<date_locale>\n")
  cat("Decimal Mark: ", x$decimal_mark, "\n", sep = "")
  print(x$date_names)
}

is_date_locale <- function(x) {
  inherits(x, "clock_date_locale")
}
