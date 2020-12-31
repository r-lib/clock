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

calendar_ptype_full <- function(x, class) {
  count <- invalid_count(x)
  paste0(class, "[invalid=", count, "]")
}

calendar_ptype_abbr <- function(x, abbr) {
  count <- invalid_count(x)
  paste0(abbr, "[i=", count, "]")
}
