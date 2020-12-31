new_sys_time <- function(duration = duration_days()) {
  new_time_point(duration, clock = "sys")
}
sys_days <- function(n = integer()) {
  duration <- duration_days(n)
  new_sys_time(duration)
}
sys_seconds <- function(n = integer()) {
  duration <- duration_seconds(n)
  new_sys_time(duration)
}

is_sys_time <- function(x) {
  is_time_point(x) && time_point_clock(x) == "sys"
}

# ------------------------------------------------------------------------------

new_naive_time <- function(duration = duration_days()) {
  new_time_point(duration, clock = "naive")
}
naive_days <- function(n = integer()) {
  duration <- duration_days(n)
  new_naive_time(duration)
}
naive_seconds <- function(n = integer()) {
  duration <- duration_seconds(n)
  new_naive_time(duration)
}

is_naive_time <- function(x) {
  is_time_point(x) && time_point_clock(x) == "naive"
}

# ------------------------------------------------------------------------------

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
    abort("`duration` must be at least daily precision.")
  }

  fields <- list(duration = duration)

  clock_validate(clock)

  new_clock_rcrd(
    fields = fields,
    clock = clock,
    ...,
    names = names,
    class = c(class, "clock_time_point")
  )
}

#' @export
is_time_point <- function(x) {
  inherits(x, "clock_time_point")
}

# ------------------------------------------------------------------------------

time_point_clock <- function(x) {
  attr(x, "clock", exact = TRUE)
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

  ticks <- field_ticks(duration)
  ticks_of_day <- field_ticks_of_day(duration, strict = FALSE)
  ticks_of_second <- field_ticks_of_second(duration, strict = FALSE)

  date_names <- locale$date_names
  decimal_mark <- locale$decimal_mark

  out <- format_time_point_cpp(
    ticks = ticks,
    ticks_of_day = ticks_of_day,
    ticks_of_second = ticks_of_second,
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

#' @export
vec_ptype2.clock_time_point.clock_time_point <- function(x, y, ...) {
  x_clock <- time_point_clock(x)
  y_clock <- time_point_clock(y)

  if (x_clock != y_clock) {
    stop_incompatible_type(x, y, ..., details = "Clocks can't differ.")
  }

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

#' @export
vec_cast.clock_time_point.clock_time_point <- function(x, to, ...) {
  x_clock <- time_point_clock(x)
  to_clock <- time_point_clock(to)

  if (x_clock != to_clock) {
    stop_incompatible_cast(x, to, ..., details = "Clocks can't differ.")
  }

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
    clock = x_clock,
    ...,
    names = names(x)
  )
}

# ------------------------------------------------------------------------------

clock_validate <- function(clock) {
  is_string(clock) && clock %in% clocks()
}

clocks <- function() {
  c("sys", "naive")
}
