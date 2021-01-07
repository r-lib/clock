new_naive_time <- function(duration = duration_days(), ..., names = NULL) {
  check_dots_empty()
  new_time_point(duration, clock = "naive", names = names)
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

# ------------------------------------------------------------------------------

#' @export
as_naive_time <- function(x) {
  UseMethod("as_naive_time")
}

#' @export
as_naive_time.clock_time_point <- function(x) {
  if (is_naive_time(x)) {
    x
  } else if (is_sys_time(x)) {
    new_naive_time(time_point_duration(x), names = names(x))
  } else {
    abort("Internal error: Unknown time point clock.")
  }
}

#' @export
as_naive_time.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("as_naive_time")
}
