new_calendar <- function(days = integer(), ..., class = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }

  new_vctr(days, ..., class = c(class, "clock_calendar"))
}

#' @export
is_calendar <- function(x) {
  inherits(x, "clock_calendar")
}
