naive_days <- function(calendar = new_year_month_day()) {
  as_naive_time(calendar)
}

new_naive_days <- function(ticks = integer(),
                           ...,
                           names = NULL,
                           class = NULL) {
  new_naive_time(
    duration = new_duration(ticks = ticks, precision = "day"),
    ...,
    names = names,
    class = class
  )
}

new_naive_time <- function(duration = duration_days(),
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

  new_clock_rcrd(fields, ..., names = names, class = c(class, "clock_naive_time"))
}

#' @export
is_naive_time <- function(x) {
  inherits(x, "clock_naive_time")
}

#' @export
format.clock_naive_time <- function(x,
                                    ...,
                                    format = NULL,
                                    locale = default_date_locale()) {
  if (!is_date_locale(locale)) {
    abort("`locale` must be a date locale object.")
  }
  if (is_null(format)) {
    duration <- field_duration(x)
    precision <- duration_precision(duration)
    format <- precision_format(precision)
  }

  format_naive_time(x, format, locale)
}

precision_format <- function(precision) {
  switch(
    precision,
    day = "%Y-%m-%d",
    hour = "%Y-%m-%d %H",
    minute = "%Y-%m-%d %H:%M",
    second = "%Y-%m-%d %H:%M:%S",
    millisecond = "%Y-%m-%d %H:%M:%S",
    microsecond = "%Y-%m-%d %H:%M:%S",
    nanosecond = "%Y-%m-%d %H:%M:%S",
    abort("Unknown precision.")
  )
}

format_naive_time <- function(x, format, locale) {
  duration <- field_duration(x)
  precision <- duration_precision(duration)

  ticks <- field_ticks(duration)
  ticks_of_day <- field_ticks_of_day(duration, strict = FALSE)
  ticks_of_second <- field_ticks_of_second(duration, strict = FALSE)

  date_names <- locale$date_names
  decimal_mark <- locale$decimal_mark

  out <- format_naive_time_cpp(
    ticks = ticks,
    ticks_of_day = ticks_of_day,
    ticks_of_second = ticks_of_second,
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

#' @export
vec_ptype_full.clock_naive_time <- function(x, ...) {
  duration <- field_duration(x)
  precision <- duration_precision(duration)
  paste0("naive_time<", precision, ">")
}

#' @export
vec_ptype_abbr.clock_naive_time <- function(x, ...) {
  duration <- field_duration(x)
  precision <- duration_precision(duration)
  precision <- precision_abbr(precision)
  paste0("nt<", precision, ">")
}

field_duration <- function(x) {
  field(x, "duration")
}

time_precision <- function(x) {
  duration_precision(field_duration(x))
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype2.clock_naive_time.clock_naive_time <- function(x, y, ...) {
  x_precision <- time_precision(x)
  y_precision <- time_precision(y)

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
vec_cast.clock_naive_time.clock_naive_time <- function(x, to, ...) {
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

  new_naive_time(duration, ..., names = names(x))
}
