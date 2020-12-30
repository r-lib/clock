new_iso <- function(fields, ..., names = NULL, class = NULL) {
  new_calendar(fields, ..., names = names, class = c(class, "clock_iso"))
}

is_iso <- function(x) {
  inherits(x, "clock_iso")
}

field_weeknum <- function(x) {
  field(x, "weeknum")
}
