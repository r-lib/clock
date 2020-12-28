new_gregorian <- function(fields, ..., names = NULL, class = NULL) {
  new_calendar(fields, ..., names = names, class = c(class, "clock_gregorian"))
}

is_gregorian <- function(x) {
  inherits(x, "clock_gregorian")
}

field_year <- function(x) {
  field(x, "year")
}
field_month <- function(x) {
  field(x, "month")
}
field_day <- function(x) {
  field(x, "day")
}
field_weekday <- function(x) {
  field(x, "weekday")
}
field_index <- function(x) {
  field(x, "index")
}

cal <- list(
  sunday = 1L,
  monday = 2L,
  tuesday = 3L,
  wednesday = 4L,
  thursday = 5L,
  friday = 6L,
  saturday = 7L,

  january = 1L,
  february = 2L,
  march = 3L,
  april = 4L,
  may = 5L,
  june = 6L,
  july = 7L,
  august = 8L,
  september = 9L,
  october = 10L,
  november = 11L,
  december = 12L
)
