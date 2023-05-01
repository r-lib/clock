#' Create a clock locale
#'
#' A clock locale contains the information required to format and parse dates.
#' The defaults have been chosen to match US English. A clock locale object can
#' be provided to `format()` methods or parse functions (like
#' [year_month_day_parse()]) to override the defaults.
#'
#' @param labels `[clock_labels / character(1)]`
#'
#'   Character representations of localized weekday names, month names, and
#'   AM/PM names. Either the language code as string (passed on to
#'   [clock_labels_lookup()]), or an object created by [clock_labels()].
#'
#' @param decimal_mark `[character(1)]`
#'
#'   Symbol used for the decimal place when formatting sub-second date-times.
#'   Either `","` or `"."`.
#'
#' @return A `"clock_locale"` object.
#'
#' @export
#' @examples
#' clock_locale()
#' clock_locale(labels = "fr")
clock_locale <- function(labels = "en", decimal_mark = ".") {
  if (is_character(labels)) {
    labels <- clock_labels_lookup(labels)
  }
  check_clock_labels(labels)

  decimal_mark <- arg_match0(decimal_mark, values = c(".", ","))

  new_clock_locale(labels, decimal_mark)
}

new_clock_locale <- function(labels, decimal_mark) {
  structure(
    list(
      labels = labels,
      decimal_mark = decimal_mark
    ),
    class = "clock_locale"
  )
}

#' @export
print.clock_locale <- function (x, ...) {
  cat("<clock_locale>\n")
  cat("Decimal Mark: ", x$decimal_mark, "\n", sep = "")
  print(x$labels)
}

check_clock_locale <- function(x, ..., arg = caller_arg(x), call = caller_env()) {
  check_inherits(x, what = "clock_locale", arg = arg, call = call)
}

