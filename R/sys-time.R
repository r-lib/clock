new_sys_time_from_fields <- function(fields, precision, names) {
  new_time_point_from_fields(fields, precision, CLOCK_SYS, names)
}

#' @export
sys_days <- function(n = integer()) {
  names <- NULL
  duration <- duration_days(n)
  new_sys_time_from_fields(duration, PRECISION_DAY, names)
}

#' @export
sys_seconds <- function(n = integer()) {
  names <- NULL
  duration <- duration_seconds(n)
  new_sys_time_from_fields(duration, PRECISION_SECOND, names)
}

# ------------------------------------------------------------------------------

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
  new_naive_time_from_fields(x, time_point_precision(x), clock_rcrd_names(x))
}

#' @export
as_zoned_time.clock_sys_time <- function(x, zone, ...) {
  zone <- zone_validate(zone)

  # Promote to at least seconds precision for `zoned_time`
  x <- vec_cast(x, vec_ptype2(x, sys_seconds()))

  precision <- time_point_precision(x)
  names <- clock_rcrd_names(x)

  new_zoned_time_from_fields(x, precision, zone, names)
}

# ------------------------------------------------------------------------------

#' @export
sys_now <- function() {
  names <- NULL
  fields <- sys_now_cpp()
  new_sys_time_from_fields(fields, PRECISION_NANOSECOND, names)
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

