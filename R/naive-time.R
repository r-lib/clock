new_naive_time <- function(duration = duration_days()) {
  new_time_point(duration, clock = "naive")
}

#' @export
naive_days <- function(n = integer()) {
  duration <- duration_days(n)
  new_naive_time(duration)
}

#' @export
naive_seconds <- function(n = integer()) {
  duration <- duration_seconds(n)
  new_naive_time(duration)
}

#' @export
is_naive_time <- function(x) {
  is_time_point(x) && time_point_clock(x) == "naive"
}
