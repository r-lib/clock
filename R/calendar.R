new_calendar <- function(fields, precision, ..., names = NULL, class = NULL) {
  new_clock_rcrd(fields, precision = precision, ..., names = names, class = c(class, "clock_calendar"))
}

is_calendar <- function(x) {
  inherits(x, "clock_calendar")
}

calendar_precision <- function(x) {
  attr(x, "precision", exact = TRUE)
}

calendar_require_minimum_precision <- function(x, precision, fn) {
  if (!calendar_has_minimum_precision(x, precision)) {
    msg <- paste0("`", fn, "()` requires a minimum precision of '", precision, "'.")
    abort(msg)
  }
  invisible(x)
}

calendar_has_minimum_precision <- function(x, precision) {
  x_precision <- calendar_precision(x)
  precision_value(x_precision) >= precision_value(precision)
}

calendar_require_precision <- function(x, precision, fn) {
  if (!calendar_has_precision(x, precision)) {
    msg <- paste0("`", fn, "()` requires a precision of '", precision, "'.")
    abort(msg)
  }
  invisible(x)
}

calendar_require_any_of_precisions <- function(x, precisions, fn) {
  results <- vapply(precisions, calendar_has_precision, FUN.VALUE = logical(1), x = x)
  if (!any(results)) {
    msg <- paste0("`", fn, "()` does not support a precision of '", calendar_precision(x), "'.")
    abort(msg)
  }
  invisible(x)
}

calendar_has_precision <- function(x, precision) {
  calendar_precision(x) == precision
}

calendar_ptype_full <- function(x, class) {
  count <- invalid_count(x)
  precision <- calendar_precision(x)
  paste0(class, "<", precision, ">[invalid=", count, "]")
}

calendar_ptype_abbr <- function(x, abbr) {
  count <- invalid_count(x)
  precision <- calendar_precision(x)
  precision <- precision_abbr(precision)
  paste0(abbr, "<", precision, ">[i=", count, "]")
}

