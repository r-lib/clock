as_wrap_point <- function(calendar) {
  names <- names(calendar)
  calendar <- unname(calendar)
  new_wrap_point(calendar, names = names)
}

new_wrap_point <- function(calendar, names) {
  fields <- list(calendar = calendar)

  new_time_point0(
    fields = fields,
    precision = "day",
    names = names,
    class = "clock_wrap_point"
  )
}

unwrap <- function(x) {
  calendar <- field_calendar(x)
  names(calendar) <- names(x)
  calendar
}
