new_calendar <- function(fields, precision, ..., names = NULL, class = NULL) {
  new_clock_rcrd(fields, precision = precision, ..., names = names, class = c(class, "clock_calendar"))
}

is_calendar <- function(x) {
  inherits(x, "clock_calendar")
}

# ------------------------------------------------------------------------------

# - Each subclass implements a `format()` method
# - Unlike vctrs, don't use `print(quote = FALSE)` since we want to match base R
#' @export
obj_print_data.clock_calendar <- function(x, ...) {
  if (vec_size(x) == 0L) {
    return(invisible(x))
  }

  out <- format(x)
  print(out)

  invisible(x)
}

# Align left to match pillar_shaft.Date
# @export - lazy in .onLoad()
pillar_shaft.clock_calendar <- function(x, ...) {
  out <- format(x)
  pillar::new_pillar_shaft_simple(out, align = "left")
}

# ------------------------------------------------------------------------------

#' @export
get_precision.clock_calendar <- function(x) {
  calendar_precision(x)
}

# ------------------------------------------------------------------------------

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

# ------------------------------------------------------------------------------

calendar_require_all_valid <- function(x) {
  if (invalid_any(x)) {
    message <- paste0(
      "Can't convert to a time point when there are invalid dates. ",
      "Resolve them before converting by calling `invalid_resolve()`."
    )
    abort(message)
  }

  invisible(x)
}

# ------------------------------------------------------------------------------

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

# ------------------------------------------------------------------------------

arith_calendar_and_missing <- function(op, x, y, ...) {
  switch (
    op,
    "+" = x,
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_calendar_and_duration <- function(op, x, y, ...) {
  switch (
    op,
    "+" = add_duration(x, y),
    "-" = add_duration(x, -y),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_duration_and_calendar <- function(op, x, y, ...) {
  switch (
    op,
    "+" = add_duration(y, x, swapped = TRUE),
    "-" = stop_incompatible_op(op, x, y, details = "Can't subtract a calendar from a duration.", ...),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_calendar_and_numeric <- function(op, x, y, ...) {
  switch (
    op,
    "+" = add_duration(x, duration_helper(y, calendar_precision(x))),
    "-" = add_duration(x, duration_helper(-y, calendar_precision(x))),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_numeric_and_calendar <- function(op, x, y, ...) {
  switch (
    op,
    "+" = add_duration(y, duration_helper(x, calendar_precision(y)), swapped = TRUE),
    "-" = stop_incompatible_op(op, x, y, details = "Can't subtract a calendar from a duration.", ...),
    stop_incompatible_op(op, x, y, ...)
  )
}


