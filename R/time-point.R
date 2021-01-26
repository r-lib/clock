
#' @export
is_time_point <- function(x) {
  inherits(x, "clock_time_point")
}

# ------------------------------------------------------------------------------

time_point_clock <- function(x) {
  attr(x, "clock", exact = TRUE)
}

time_point_precision <- function(x) {
  attr(x, "precision", exact = TRUE)
}

time_point_duration <- function(x) {
  names <- NULL
  precision <- time_point_precision(x)
  new_duration_from_fields(x, precision, names)
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

  clock <- time_point_clock(x)
  precision <- time_point_precision(x)

  if (is_null(format)) {
    format <- time_point_precision_format(precision)
  }

  date_names <- locale$date_names
  decimal_mark <- locale$decimal_mark

  out <- format_time_point_cpp(
    fields = x,
    clock = clock,
    format = format,
    precision_int = precision,
    mon = date_names$mon,
    mon_ab = date_names$mon_ab,
    day = date_names$day,
    day_ab = date_names$day_ab,
    am_pm = date_names$am_pm,
    decimal_mark = decimal_mark
  )

  names(out) <- clock_rcrd_names(x)

  out
}

time_point_precision_format <- function(precision) {
  precision <- precision_to_string(precision)

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

parse_naive_time <- function(x,
                             format,
                             ...,
                             precision = "second",
                             locale = default_date_locale()) {
  precision <- validate_time_point_precision(precision)
  fields <- parse_time_point(x, format, ..., precision = precision, locale = locale, clock = CLOCK_NAIVE)
  new_naive_time_from_fields(fields, precision, names(x))
}

parse_sys_time <- function(x,
                           format,
                           ...,
                           precision = "second",
                           locale = default_date_locale()) {
  precision <- validate_time_point_precision(precision)
  fields <- parse_time_point(x, format, ..., precision = precision, locale = locale, clock = CLOCK_SYS)
  new_sys_time_from_fields(fields, precision, names(x))
}

parse_time_point <- function(x,
                             format,
                             ...,
                             precision,
                             locale,
                             clock) {
  check_dots_empty()

  if (!is_date_locale(locale)) {
    abort("`locale` must be a 'clock_date_locale'.")
  }

  mapping <- locale$date_names
  mark <- locale$decimal_mark

  parse_time_point_cpp(
    x,
    format,
    precision,
    clock,
    mapping$mon,
    mapping$mon_ab,
    mapping$day,
    mapping$day_ab,
    mapping$am_pm,
    mark
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
  clock_rcrd_proxy(x)
}

#' @export
vec_restore.clock_time_point <- function(x, to, ...) {
  .Call("_clock_time_point_restore", x, to, PACKAGE = "clock")
}

#' @export
vec_proxy_equal.clock_time_point <- function(x, ...) {
  clock_rcrd_proxy_equal(x)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype_full.clock_time_point <- function(x, ...) {
  clock <- time_point_clock(x)
  clock <- clock_to_string(clock)
  precision <- time_point_precision(x)
  precision <- precision_to_string(precision)
  paste0("time_point<", clock, "><", precision, ">")
}

#' @export
vec_ptype_abbr.clock_time_point <- function(x, ...) {
  clock <- time_point_clock(x)
  clock <- clock_to_string(clock)
  precision <- time_point_precision(x)
  precision <- precision_to_string(precision)
  precision <- precision_abbr(precision)
  paste0("tp<", clock, "><", precision, ">")
}

# ------------------------------------------------------------------------------

# Caller guarantees that clocks are identical
ptype2_time_point_and_time_point <- function(x, y, ...) {
  if (time_point_precision(x) >= time_point_precision(y)) {
    x
  } else {
    y
  }
}

# Caller guarantees that clocks are identical
cast_time_point_to_time_point <- function(x, to, ...) {
  x_precision <- time_point_precision(x)
  to_precision <- time_point_precision(to)

  if (x_precision == to_precision) {
    return(x)
  }

  if (x_precision > to_precision) {
    stop_incompatible_cast(x, to, ..., details = "Can't cast to a less precise precision.")
  }

  fields <- duration_cast_cpp(x, x_precision, to_precision)

  names <- clock_rcrd_names(x)
  clock <- time_point_clock(x)

  new_time_point_from_fields(fields, to_precision, clock, names)
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
  time_point_plus_duration(x, n, PRECISION_WEEK, names_common(x, n))
}

#' @export
add_days.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, PRECISION_DAY, names_common(x, n))
}

#' @export
add_hours.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, PRECISION_HOUR, names_common(x, n))
}

#' @export
add_minutes.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, PRECISION_MINUTE, names_common(x, n))
}

#' @export
add_seconds.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, PRECISION_SECOND, names_common(x, n))
}

#' @export
add_milliseconds.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, PRECISION_MILLISECOND, names_common(x, n))
}

#' @export
add_microseconds.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, PRECISION_MICROSECOND, names_common(x, n))
}

#' @export
add_nanoseconds.clock_time_point <- function(x, n, ...) {
  time_point_plus_duration(x, n, PRECISION_NANOSECOND, names_common(x, n))
}

time_point_plus_duration <- function(x, n, precision_n, names) {
  time_point_arith_duration(x, n, precision_n, names, duration_plus)
}
time_point_minus_duration <- function(x, n, precision_n, names) {
  time_point_arith_duration(x, n, precision_n, names, duration_minus)
}

time_point_arith_duration <- function(x, n, precision_n, names, duration_fn) {
  clock <- time_point_clock(x)
  x <- time_point_duration(x)

  n <- duration_collect_n(n, precision_n)

  # Handles recycling and casting
  duration <- duration_fn(x = x, y = n, names = names)

  names <- clock_rcrd_names(duration)
  precision <- duration_precision(duration)

  new_time_point_from_fields(duration, precision, clock, names)
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
as_duration.clock_time_point <- function(x) {
  out <- time_point_duration(x)
  out <- clock_rcrd_set_names(out, clock_rcrd_names(x))
  out
}

#' @export
as_year_month_day.clock_time_point <- function(x) {
  precision <- time_point_precision(x)
  fields <- as_year_month_day_from_sys_time_cpp(x, precision)
  new_year_month_day_from_fields(fields, precision, names = names(x))
}

#' @export
as_year_month_weekday.clock_time_point <- function(x) {
  precision <- time_point_precision(x)
  fields <- as_year_month_weekday_from_sys_time_cpp(x, precision)
  new_year_month_weekday_from_fields(fields, precision, names = names(x))
}

#' @export
as_year_quarter_day.clock_time_point <- function(x, ..., start = 1L) {
  check_dots_empty()
  precision <- time_point_precision(x)
  start <- quarterly_validate_start(start)
  fields <- as_year_quarter_day_from_sys_time_cpp(x, precision, start)
  new_year_quarter_day_from_fields(fields, precision, start, names = names(x))
}

#' @export
as_iso_year_week_day.clock_time_point <- function(x) {
  precision <- time_point_precision(x)
  fields <- as_iso_year_week_day_from_sys_time_cpp(x, precision)
  new_iso_year_week_day_from_fields(fields, precision, names = names(x))
}

# ------------------------------------------------------------------------------

#' @export
time_point_cast <- function(x, precision) {
  if (!is_time_point(x)) {
    abort("`x` must be a 'time_point'.")
  }

  x_precision <- time_point_precision(x)
  precision <- validate_time_point_precision(precision)

  fields <- duration_cast_cpp(x, x_precision, precision)

  names <- clock_rcrd_names(x)
  clock <- time_point_clock(x)

  new_time_point_from_fields(fields, precision, clock, names)
}

# Notes:
# Boundary handling for `time_point_ceiling()` requires a little bit of
# explanation. Both flooring and ceiling while on a boundary returns the
# input at the new precision, without changing the actual value.
# This is mathematically consistent with definitions of floor and
# ceiling, as floor(2) == ceiling(2) == 2. For example:
#
# x <- as_naive_time(year_month_day(1970, 01, 01, 00, 00, 00))
# time_point_ceiling(x, "second") == "1970-01-01T00:00:00"
# time_point_ceiling(x, "day") == "1970-01-01"
# time_point_ceiling(x, "day", n = 2) == "1970-01-01"
# time_point_ceiling(x + 1, "day") == "1970-01-02"
#
# lubridate has special default handling of Date objects that rounds up with ceil
# lubridate::ceiling_date(as.Date("1970-01-01"), "day") == "1970-01-02"
#
# It would not make sense for this to round up, since we consider x and y
# to be exactly identical time points, just with differing precision
# y <- as_naive_time(year_month_day(1970, 01, 01))
# time_point_ceiling(y, "day") == "1970-01-01"
# x == y is TRUE
#
# Should add an example of changing the origin time, like:
# time_point_floor(x - duration_seconds(-86400), "day", n = 2) + duration_seconds(-86400)

#' @export
time_point_floor <- function(x, precision, ..., n = 1L) {
  time_point_rounder(x, precision, n, duration_floor, ...)
}

#' @export
time_point_ceiling <- function(x, precision, ..., n = 1L) {
  time_point_rounder(x, precision, n, duration_ceiling, ...)
}

#' @export
time_point_round <- function(x, precision, ..., n = 1L) {
  time_point_rounder(x, precision, n, duration_round, ...)
}

time_point_rounder <- function(x, precision, n, duration_rounder, ...) {
  check_dots_empty()

  if (!is_time_point(x)) {
    abort("`x` must be a 'time_point'.")
  }

  precision_string <- precision
  precision <- validate_time_point_precision(precision)

  duration <- time_point_duration(x)
  duration <- duration_rounder(duration, precision_string, n = n)

  names <- clock_rcrd_names(x)
  clock <- time_point_clock(x)

  new_time_point_from_fields(duration, precision, clock, names)
}

# ------------------------------------------------------------------------------

validate_time_point_precision <- function(precision) {
  precision <- validate_precision(precision)

  if (!is_valid_time_point_precision(precision)) {
    abort("`precision` must be at least 'day' precision.")
  }

  precision
}

is_valid_time_point_precision <- function(precision) {
  precision >= PRECISION_DAY
}
