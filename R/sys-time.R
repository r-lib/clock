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
  inherits(x, "clock_sys_time")
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time <- function(x) {
  UseMethod("as_sys_time")
}

#' @export
as_sys_time.clock_sys_time <- function(x) {
  x
}

#' @export
as_sys_time.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("as_sys_time")
}

# ------------------------------------------------------------------------------

#' @export
as_naive_time.clock_sys_time <- function(x) {
  new_naive_time(time_point_duration(x), names = names(x))
}

#' @export
as_zoned_time.clock_sys_time <- function(x, zone, ...) {
  zone <- zone_validate(zone)

  # Promote to at least seconds precision for `zoned_time`
  x <- vec_cast(x, vec_ptype2(x, sys_seconds()))

  names <- names(x)
  sys_time <- unname(x)

  new_zoned_time(sys_time, zone, names = names)
}

# ------------------------------------------------------------------------------

#' @export
sys_now <- function() {
  fields <- sys_now_cpp()
  duration <- new_duration_from_fields(fields, "nanosecond")
  new_sys_time(duration)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype2.clock_sys_time.clock_sys_time <- function(x, y, ...) {
  ptype2_time_point_and_time_point(x, y, ...)
}

#' @export
vec_cast.clock_sys_time.clock_sys_time <- function(x, to, ...) {
  cast_time_point_to_time_point(x, to, ...)
}

# ------------------------------------------------------------------------------

#' @method vec_arith clock_sys_time
#' @export
vec_arith.clock_sys_time <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_sys_time", y)
}

#' @method vec_arith.clock_sys_time MISSING
#' @export
vec_arith.clock_sys_time.MISSING <- function(op, x, y, ...) {
  arith_time_point_and_missing(op, x, y, ...)
}

#' @method vec_arith.clock_sys_time clock_sys_time
#' @export
vec_arith.clock_sys_time.clock_sys_time <- function(op, x, y, ...) {
  arith_time_point_and_time_point(op, x, y, ...)
}

#' @method vec_arith.clock_sys_time clock_duration
#' @export
vec_arith.clock_sys_time.clock_duration <- function(op, x, y, ...) {
  arith_time_point_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration clock_sys_time
#' @export
vec_arith.clock_duration.clock_sys_time <- function(op, x, y, ...) {
  arith_duration_and_time_point(op, x, y, ...)
}

#' @method vec_arith.clock_sys_time numeric
#' @export
vec_arith.clock_sys_time.numeric <- function(op, x, y, ...) {
  arith_time_point_and_numeric(op, x, y, ...)
}

#' @method vec_arith.numeric clock_sys_time
#' @export
vec_arith.numeric.clock_sys_time <- function(op, x, y, ...) {
  arith_numeric_and_time_point(op, x, y, ...)
}

