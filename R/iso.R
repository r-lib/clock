new_iso <- function(days = integer(), ..., class = NULL) {
  new_calendar(days, ..., class = c(class, "clock_iso"))
}

#' @export
is_iso <- function(x) {
  inherits(x, "clock_iso")
}
