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
  inherits(x, "clock_naive_time")
}

# ------------------------------------------------------------------------------

#' @export
as_naive_time <- function(x) {
  UseMethod("as_naive_time")
}

#' @export
as_naive_time.clock_naive_time <- function(x) {
  x
}

#' @export
as_naive_time.clock_calendar <- function(x) {
  stop_clock_unsupported_calendar_op("as_naive_time")
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_naive_time <- function(x) {
  new_sys_time(time_point_duration(x), names = names(x))
}

#' @export
as_zoned_time.clock_naive_time <- function(x,
                                           zone,
                                           ...,
                                           nonexistent = "error",
                                           ambiguous = "error") {
  # `nonexistent` and `ambiguous` are allowed to be
  # size 1 or the same size as `x`
  x_size <- vec_size(x)
  nonexistent_size <- vec_size(nonexistent)
  ambiguous_size <- vec_size(ambiguous)

  if (nonexistent_size != 1L && nonexistent_size != x_size) {
    abort("`nonexistent` must have length 1, or length equal to the length of `x`.")
  }
  if (ambiguous_size != 1L && ambiguous_size != x_size) {
    abort("`ambiguous` must have length 1, or length equal to the length of `x`.")
  }

  # Promote to at least seconds precision for `zoned_time`
  x <- vec_cast(x, vec_ptype2(x, naive_seconds()))

  zone <- zone_validate(zone)

  duration <- time_point_duration(x)
  precision <- time_point_precision(x)

  fields <- as_zoned_sys_time_from_naive_time_cpp(duration, precision, zone, nonexistent, ambiguous)
  duration <- new_duration_from_fields(fields, precision)

  sys_time <- new_sys_time(duration)

  new_zoned_time(sys_time, zone, names = names(x))
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype2.clock_naive_time.clock_naive_time <- function(x, y, ...) {
  ptype2_time_point_and_time_point(x, y, ...)
}

#' @export
vec_cast.clock_naive_time.clock_naive_time <- function(x, to, ...) {
  cast_time_point_to_time_point(x, to, ...)
}

# ------------------------------------------------------------------------------

#' @method vec_arith clock_naive_time
#' @export
vec_arith.clock_naive_time <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_naive_time", y)
}

#' @method vec_arith.clock_naive_time MISSING
#' @export
vec_arith.clock_naive_time.MISSING <- function(op, x, y, ...) {
  arith_time_point_and_missing(op, x, y, ...)
}

#' @method vec_arith.clock_naive_time clock_naive_time
#' @export
vec_arith.clock_naive_time.clock_naive_time <- function(op, x, y, ...) {
  arith_time_point_and_time_point(op, x, y, ...)
}

#' @method vec_arith.clock_naive_time clock_duration
#' @export
vec_arith.clock_naive_time.clock_duration <- function(op, x, y, ...) {
  arith_time_point_and_duration(op, x, y, ...)
}

#' @method vec_arith.clock_duration clock_naive_time
#' @export
vec_arith.clock_duration.clock_naive_time <- function(op, x, y, ...) {
  arith_duration_and_time_point(op, x, y, ...)
}

#' @method vec_arith.clock_naive_time numeric
#' @export
vec_arith.clock_naive_time.numeric <- function(op, x, y, ...) {
  arith_time_point_and_numeric(op, x, y, ...)
}

#' @method vec_arith.numeric clock_naive_time
#' @export
vec_arith.numeric.clock_naive_time <- function(op, x, y, ...) {
  arith_numeric_and_time_point(op, x, y, ...)
}
