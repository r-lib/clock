new_sys_time <- function(duration = duration_days()) {
  new_time_point(duration, clock = "sys")
}

#' @export
sys_days <- function(n = integer()) {
  duration <- duration_days(n)
  new_sys_time(duration)
}

#' @export
sys_seconds <- function(n = integer()) {
  duration <- duration_seconds(n)
  new_sys_time(duration)
}

#' @export
is_sys_time <- function(x) {
  is_time_point(x) && time_point_clock(x) == "sys"
}
