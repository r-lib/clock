#' @export
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

new_date_locale <- function(date_names = date_names_lang("en"), decimal_mark = ".") {
  structure(list(date_names = date_names, decimal_mark = decimal_mark), class = "clock_date_locale")
}

#' @export
print.clock_date_locale <- function (x, ...) {
  cat("<date_locale>\n")
  cat("Decimal Mark: ", x$decimal_mark, "\n", sep = "")
  print(x$date_names)
}
