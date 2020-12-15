new_day_point <- function(calendar, ..., names = NULL) {
  new_time_point(
    calendar = calendar,
    precision = "day",
    names = names,
    class = "clock_day_point"
  )
}
