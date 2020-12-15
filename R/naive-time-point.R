new_naive_time_point <- function(calendar, precision, ..., fields = NULL, names = NULL, class) {
  new_time_point(
    calendar = calendar,
    precision = precision,
    ...,
    fields = fields,
    names = names,
    class = c(class, "clock_naive_time_point")
  )
}

#' @export
is_naive_time_point <- function(x) {
  inherits(x, "clock_naive_time_point")
}
