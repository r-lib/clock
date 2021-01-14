#' @export
new_time_point <- function(duration = duration_days(),
                           clock = "sys",
                           ...,
                           names = NULL,
                           class = NULL) {
  if (!is_duration(duration)) {
    abort("`duration` must be a duration.")
  }

  precision <- duration_precision(duration)
  if (precision_value(precision) < PRECISION_DAY) {
    abort("`duration` must have at least daily precision.")
  }

  fields <- list(duration = duration)

  clock_validate(clock)

  if (clock_is_sys(clock)) {
    class <- c(class, "clock_sys_time", "clock_time_point")
  } else if (clock_is_naive(clock)) {
    class <- c(class, "clock_naive_time", "clock_time_point")
  } else {
    abort("Internal error: Unknown clock.")
  }

  new_clock_rcrd(
    fields = fields,
    ...,
    names = names,
    class = class
  )
}

#' @export
is_time_point <- function(x) {
  inherits(x, "clock_time_point")
}

# ------------------------------------------------------------------------------

time_point_clock <- function(x) {
  UseMethod("time_point_clock")
}
#' @export
time_point_clock.clock_sys_time <- function(x) {
  "sys"
}
#' @export
time_point_clock.clock_naive_time <- function(x) {
  "naive"
}

time_point_duration <- function(x) {
  field_duration(x)
}
time_point_precision <- function(x) {
  duration_precision(time_point_duration(x))
}

field_duration <- function(x) {
  field(x, "duration")
}

# ------------------------------------------------------------------------------

#' @export
format.clock_time_point <- function(x,
                                    ...,
                                    format = NULL,
                                    locale = default_date_locale()) {
  if (!is_date_locale(locale)) {
    abort("`locale` must be a date locale object.")
  }

  duration <- time_point_duration(x)
  clock <- time_point_clock(x)

  precision <- duration_precision(duration)

  if (is_null(format)) {
    format <- time_point_precision_format(precision)
  }

  date_names <- locale$date_names
  decimal_mark <- locale$decimal_mark

  out <- format_time_point_cpp(
    fields = duration,
    clock = clock,
    format = format,
    precision = precision,
    mon = date_names$mon,
    mon_ab = date_names$mon_ab,
    day = date_names$day,
    day_ab = date_names$day_ab,
    am_pm = date_names$am_pm,
    decimal_mark = decimal_mark
  )

  names(out) <- names(x)

  out
}

time_point_precision_format <- function(precision) {
  switch(
    precision,
    day = "%Y-%m-%d",
    hour = "%Y-%m-%dT%H",
    minute = "%Y-%m-%dT%H:%M",
    second = "%Y-%m-%dT%H:%M:%S",
    millisecond = "%Y-%m-%dT%H:%M:%S",
    microsecond = "%Y-%m-%dT%H:%M:%S",
    nanosecond = "%Y-%m-%dT%H:%M:%S",
    abort("Unknown precision.")
  )
}

# ------------------------------------------------------------------------------

# - Each subclass implements a `format()` method
# - Unlike vctrs, don't use `print(quote = FALSE)` since we want to match base R
#' @export
obj_print_data.clock_time_point <- function(x, ...) {
  if (vec_size(x) == 0L) {
    return(invisible(x))
  }

  out <- format(x)
  print(out)

  invisible(x)
}

# Align left to match pillar_shaft.Date
# @export - lazy in .onLoad()
pillar_shaft.clock_time_point <- function(x, ...) {
  out <- format(x)
  pillar::new_pillar_shaft_simple(out, align = "left")
}

# ------------------------------------------------------------------------------

#' @export
vec_proxy.clock_time_point <- function(x, ...) {
  proxy_rcrd(x)
}

#' @export
vec_restore.clock_time_point <- function(x, to, ...) {
  fields <- restore_rcrd_fields(x)
  names <- restore_rcrd_names(x)
  clock <- time_point_clock(to)
  duration <- fields$duration
  new_time_point(duration, clock, names = names)
}

#' @export
vec_proxy_equal.clock_time_point <- function(x, ...) {
  proxy_equal_rcrd(x)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype_full.clock_time_point <- function(x, ...) {
  clock <- time_point_clock(x)
  duration <- time_point_duration(x)
  precision <- duration_precision(duration)
  paste0("time_point<", clock, "><", precision, ">")
}

#' @export
vec_ptype_abbr.clock_time_point <- function(x, ...) {
  clock <- time_point_clock(x)
  duration <- time_point_duration(x)
  precision <- duration_precision(duration)
  precision <- precision_abbr(precision)
  paste0("tp<", clock, "><", precision, ">")
}

# ------------------------------------------------------------------------------

# Caller guarantees that clocks are identical
ptype2_time_point_and_time_point <- function(x, y, ...) {
  x_precision <- time_point_precision(x)
  y_precision <- time_point_precision(y)

  if (x_precision == y_precision) {
    return(x)
  }

  x_precision_value <- precision_value(x_precision)
  y_precision_value <- precision_value(y_precision)

  if (x_precision_value > y_precision_value) {
    x
  } else {
    y
  }
}

# Caller guarantees that clocks are identical
cast_time_point_to_time_point <- function(x, to, ...) {
  x_duration <- field_duration(x)
  to_duration <- field_duration(to)

  x_precision <- duration_precision(x_duration)
  to_precision <- duration_precision(to_duration)

  if (x_precision == to_precision) {
    return(x)
  }

  x_precision_value <- precision_value(x_precision)
  to_precision_value <- precision_value(to_precision)

  if (x_precision_value > to_precision_value) {
    stop_incompatible_cast(x, to, ..., details = "Precision would be lost.")
  }

  duration <- duration_cast(x_duration, to_precision)

  new_time_point(
    duration = duration,
    clock = time_point_clock(x),
    names = names(x)
  )
}

# ------------------------------------------------------------------------------

arith_time_point_and_missing <- function(op, x, y, ...) {
  switch (
    op,
    "+" = x,
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_time_point_and_time_point <- function(op, x, y, ...) {
  switch (
    op,
    "-" = time_point_minus_time_point(x, y, names_common(x, y)),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_time_point_and_duration <- function(op, x, y, ...) {
  switch (
    op,
    "+" = time_point_plus_duration(x, y, duration_precision(y), names_common(x, y)),
    "-" = time_point_minus_duration(x, y, duration_precision(y), names_common(x, y)),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_duration_and_time_point <- function(op, x, y, ...) {
  switch (
    op,
    "+" = time_point_plus_duration(y, x, duration_precision(x), names_common(x, y)),
    "-" = stop_incompatible_op(op, x, y, details = "Can't subtract a time point from a duration.", ...),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_time_point_and_numeric <- function(op, x, y, ...) {
  precision <- time_point_precision(x)

  switch (
    op,
    "+" = time_point_plus_duration(x, y, precision, names_common(x, y)),
    "-" = time_point_minus_duration(x, y, precision, names_common(x, y)),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_numeric_and_time_point <- function(op, x, y, ...) {
  precision <- time_point_precision(y)

  switch (
    op,
    "+" = time_point_plus_duration(y, x, precision, names_common(x, y)),
    "-" = stop_incompatible_op(op, x, y, details = "Can't subtract a time point from a duration.", ...),
    stop_incompatible_op(op, x, y, ...)
  )
}

# ------------------------------------------------------------------------------

#' @export
add_weeks.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, "week", names_common(x, n))
}

#' @export
add_days.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, "day", names_common(x, n))
}

#' @export
add_hours.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, "hour", names_common(x, n))
}

#' @export
add_minutes.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, "minute", names_common(x, n))
}

#' @export
add_seconds.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, "second", names_common(x, n))
}

#' @export
add_milliseconds.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, "millisecond", names_common(x, n))
}

#' @export
add_microseconds.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, "microsecond", names_common(x, n))
}

#' @export
add_nanoseconds.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, "nanosecond", names_common(x, n))
}

time_point_plus_duration <- function(x, n, precision_n, names) {
  time_point_arith_duration(x, n, precision_n, names, duration_plus)
}
time_point_minus_duration <- function(x, n, precision_n, names) {
  time_point_arith_duration(x, n, precision_n, names, duration_minus)
}

time_point_arith_duration <- function(x, n, precision_n, names, duration_fn) {
  clock <- time_point_clock(x)
  duration <- time_point_duration(x)

  n <- duration_collect_n(n, precision_n)

  args <- vec_recycle_common(x = x, n = n, names = names)
  x <- args$x
  n <- args$n
  names <- args$names

  duration <- duration_fn(x = duration, y = n, names = NULL)

  new_time_point(duration, clock, names = names)
}

time_point_minus_time_point <- function(x, y, names) {
  args <- vec_recycle_common(x = x, y = y, names = names)
  x <- args$x
  y <- args$y
  names <- args$names

  x_duration <- time_point_duration(x)
  y_duration <- time_point_duration(y)

  duration_minus(x = x_duration, y = y_duration, names = names)
}

# ------------------------------------------------------------------------------

#' @export
as_year_month_day.clock_time_point <- function(x) {
  duration <- time_point_duration(x)
  precision <- time_point_precision(x)
  fields <- as_year_month_day_from_sys_time_cpp(duration, precision)
  new_year_month_day_from_fields(fields, precision, names = names(x))
}

#' @export
as_year_month_weekday.clock_time_point <- function(x) {
  duration <- time_point_duration(x)
  precision <- time_point_precision(x)
  fields <- as_year_month_weekday_from_sys_time_cpp(duration, precision)
  new_year_month_weekday_from_fields(fields, precision, names = names(x))
}

#' @export
as_year_quarter_day.clock_time_point <- function(x, ..., start = 1L) {
  check_dots_empty()
  duration <- time_point_duration(x)
  precision <- time_point_precision(x)
  start <- quarterly_validate_start(start)
  fields <- as_year_quarter_day_from_sys_time_cpp(duration, precision, start)
  new_year_quarter_day_from_fields(fields, precision, start, names = names(x))
}

#' @export
as_iso_year_week_day.clock_time_point <- function(x) {
  duration <- time_point_duration(x)
  precision <- time_point_precision(x)
  fields <- as_iso_year_week_day_from_sys_time_cpp(duration, precision)
  new_iso_year_week_day_from_fields(fields, precision, names = names(x))
}

# ------------------------------------------------------------------------------

#' @export
time_point_cast <- function(x, precision) {
  if (!is_time_point(x)) {
    abort("`x` must be a 'time_point'.")
  }

  if (!is_valid_time_point_precision(precision)) {
    abort("`precision` must be a valid precision string, and must be at least 'day' precision.")
  }

  duration <- time_point_duration(x)
  duration <- duration_cast(duration, precision)

  new_time_point(duration, clock = time_point_clock(x), names = names(x))
}

#' @export
time_point_floor <- function(x, precision) {
  time_point_rounder(x, precision, duration_floor)
}

#' @export
time_point_ceil <- function(x, precision) {
  time_point_rounder(x, precision, duration_ceil)
}

#' @export
time_point_round <- function(x, precision) {
  time_point_rounder(x, precision, duration_round)
}

time_point_rounder <- function(x, precision, duration_rounder) {
  if (!is_time_point(x)) {
    abort("`x` must be a 'time_point'.")
  }

  if (!is_valid_time_point_precision(precision)) {
    abort("`precision` must be a valid precision string, and must be at least 'day' precision.")
  }

  duration <- time_point_duration(x)
  duration <- duration_rounder(duration, precision)

  new_time_point(duration, clock = time_point_clock(x), names = names(x))
}

# ------------------------------------------------------------------------------

is_valid_time_point_precision <- function(precision) {
  if (!is_valid_precision(precision)) {
    return(FALSE)
  }
  precision_value(precision) >= PRECISION_DAY
}

# ------------------------------------------------------------------------------

clock_validate <- function(clock) {
  is_string(clock) && clock %in% clocks()
}

clocks <- function() {
  c("sys", "naive")
}

clock_is_sys <- function(x) {
  x == "sys"
}
clock_is_naive <- function(x) {
  x == "naive"
}
