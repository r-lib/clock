new_gregorian <- function(days = integer(), ..., class = NULL) {
  new_calendar(days, ..., class = c(class, "clock_gregorian"))
}

#' @export
is_gregorian <- function(x) {
  inherits(x, "clock_gregorian")
}
