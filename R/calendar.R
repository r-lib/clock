new_calendar <- function(fields, ..., names = NULL, class = NULL) {
  new_clock_rcrd(fields, ..., names = names, class = c(class, "clock_calendar"))
}

is_calendar <- function(x) {
  inherits(x, "clock_calendar")
}

calendar_is_complete <- function(x) {
  UseMethod("calendar_is_complete")
}

#' @export
calendar_is_complete.clock_calendar <- function(x) {
  abort("This calendar type has not yet implemented `calendar_is_complete()`.")
}

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
