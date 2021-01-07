new_sys_time <- function(duration = duration_days(), ..., names = NULL) {
  check_dots_empty()
  new_time_point(duration, clock = "sys", names = names)
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

# ------------------------------------------------------------------------------

#' @export
as_sys_time <- function(x) {
  UseMethod("as_sys_time")
}

#' @export
as_sys_time.clock_time_point <- function(x) {
  if (is_sys_time(x)) {
    x
  } else if (is_naive_time(x)) {
    new_sys_time(time_point_duration(x), names = names(x))
  } else {
    abort("Internal error: Unknown time point clock.")
  }
}

#' @export
as_sys_time.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("as_sys_time")
}
