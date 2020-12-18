# ------------------------------------------------------------------------------
# year-month

#' @export
vec_cast.clock_year_month.clock_year_month <- function(x, to, ...) {
  x
}

# ------------------------------------------------------------------------------
# year-month-day

#' @export
vec_cast.clock_year_month_day.clock_year_month_day <- function(x, to, ...) {
  x
}
#' @export
vec_cast.clock_year_month_day.clock_year_month <- function(x, to, ...) {
  new_year_month_day(x)
}
#' @export
vec_cast.clock_year_month_day.clock_year_month_weekday <- vec_cast.clock_year_month_day.clock_year_month
#' @export
vec_cast.clock_year_month_day.clock_year_quarternum <- vec_cast.clock_year_month_day.clock_year_month
#' @export
vec_cast.clock_year_month_day.clock_year_quarternum_quarterday <- vec_cast.clock_year_month_day.clock_year_month
#' @export
vec_cast.clock_year_month_day.clock_iso_year_weeknum <- vec_cast.clock_year_month_day.clock_year_month
#' @export
vec_cast.clock_year_month_day.clock_iso_year_weeknum_weekday <- vec_cast.clock_year_month_day.clock_year_month

# ------------------------------------------------------------------------------
# year-month-weekday

#' @export
vec_cast.clock_year_month_weekday.clock_year_month_weekday <- function(x, to, ...) {
  x
}
#' @export
vec_cast.clock_year_month_weekday.clock_year_month <- function(x, to, ...) {
  new_year_month_weekday(x)
}
#' @export
vec_cast.clock_year_month_weekday.clock_year_month_day <- vec_cast.clock_year_month_weekday.clock_year_month
#' @export
vec_cast.clock_year_month_weekday.clock_year_quarternum <- vec_cast.clock_year_month_weekday.clock_year_month
#' @export
vec_cast.clock_year_month_weekday.clock_year_quarternum_quarterday <- vec_cast.clock_year_month_weekday.clock_year_month
#' @export
vec_cast.clock_year_month_weekday.clock_iso_year_weeknum <- vec_cast.clock_year_month_weekday.clock_year_month
#' @export
vec_cast.clock_year_month_weekday.clock_iso_year_weeknum_weekday <- vec_cast.clock_year_month_weekday.clock_year_month

# ------------------------------------------------------------------------------
# year-quarternum

#' @export
vec_cast.clock_year_quarternum.clock_year_quarternum <- function(x, to, ...) {
  if (identical_quarterly_starts(x, to)) {
    x
  } else {
    new_year_quarternum(x, start = get_quarterly_start(to))
  }
}

# ------------------------------------------------------------------------------
# year-quarternum-quarterday

#' @export
vec_cast.clock_year_quarternum_quarterday.clock_year_quarternum_quarterday <- function(x, to, ...) {
  if (identical_quarterly_starts(x, to)) {
    x
  } else {
    new_year_quarternum_quarterday(x, start = get_quarterly_start(to))
  }
}
#' @export
vec_cast.clock_year_quarternum_quarterday.clock_year_quarternum <- function(x, to, ...) {
  new_year_quarternum_quarterday(x, start = get_quarterly_start(to))
}
#' @export
vec_cast.clock_year_quarternum_quarterday.clock_year_month <- vec_cast.clock_year_quarternum_quarterday.clock_year_quarternum
#' @export
vec_cast.clock_year_quarternum_quarterday.clock_year_month_day <- vec_cast.clock_year_quarternum_quarterday.clock_year_quarternum
#' @export
vec_cast.clock_year_quarternum_quarterday.clock_year_month_weekday <- vec_cast.clock_year_quarternum_quarterday.clock_year_quarternum
#' @export
vec_cast.clock_year_quarternum_quarterday.clock_iso_year_weeknum <- vec_cast.clock_year_quarternum_quarterday.clock_year_quarternum
#' @export
vec_cast.clock_year_quarternum_quarterday.clock_iso_year_weeknum_weekday <- vec_cast.clock_year_quarternum_quarterday.clock_year_quarternum

# ------------------------------------------------------------------------------
# iso-year-weeknum

#' @export
vec_cast.clock_iso_year_weeknum.clock_iso_year_weeknum <- function(x, to, ...) {
  x
}

# ------------------------------------------------------------------------------
# iso-year-weeknum-weekday

#' @export
vec_cast.clock_iso_year_weeknum_weekday.clock_iso_year_weeknum_weekday <- function(x, to, ...) {
  x
}
#' @export
vec_cast.clock_iso_year_weeknum_weekday.clock_iso_year_weeknum <- function(x, to, ...) {
  new_iso_year_weeknum_weekday(x)
}
#' @export
vec_cast.clock_iso_year_weeknum_weekday.clock_year_month <- vec_cast.clock_iso_year_weeknum_weekday.clock_iso_year_weeknum
#' @export
vec_cast.clock_iso_year_weeknum_weekday.clock_year_month_day <- vec_cast.clock_iso_year_weeknum_weekday.clock_iso_year_weeknum
#' @export
vec_cast.clock_iso_year_weeknum_weekday.clock_year_month_weekday <- vec_cast.clock_iso_year_weeknum_weekday.clock_iso_year_weeknum
#' @export
vec_cast.clock_iso_year_weeknum_weekday.clock_year_quarternum <- vec_cast.clock_iso_year_weeknum_weekday.clock_iso_year_weeknum
#' @export
vec_cast.clock_iso_year_weeknum_weekday.clock_year_quarternum_quarterday <- vec_cast.clock_iso_year_weeknum_weekday.clock_iso_year_weeknum

# ------------------------------------------------------------------------------
# naive-time-point

#' @export
vec_cast.clock_naive_time_point.clock_naive_time_point <- function(x, to, ...) {
  calendar <- vec_cast(field_calendar(x), field_calendar(to))
  x <- set_calendar(x, calendar)

  x_precision <- get_precision(x)
  to_precision <- get_precision(to)

  if (x_precision == to_precision) {
    x
  } else if (precision_value(x_precision) > precision_value(to_precision)) {
    stop_incompatible_cast(x, to, ...)
  } else {
    adjust_precision(x, to_precision)
  }
}
#' @export
vec_cast.clock_naive_time_point.clock_year_month <- function(x, to, ...) {
  calendar <- vec_cast(x, field_calendar(to))
  seconds_of_day <- seconds_of_day_init(x)
  precision <- get_precision(to)

  if (is_subsecond_precision(precision)) {
    nanoseconds_of_second <- nanoseconds_of_second_init(x)
  } else {
    nanoseconds_of_second <- NULL
  }

  names <- names(calendar)
  calendar <- unname(calendar)

  new_naive_time_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    precision = precision,
    names = names
  )
}
#' @export
vec_cast.clock_naive_time_point.clock_year_month_day <- vec_cast.clock_naive_time_point.clock_year_month
#' @export
vec_cast.clock_naive_time_point.clock_year_month_weekday <- vec_cast.clock_naive_time_point.clock_year_month
#' @export
vec_cast.clock_naive_time_point.clock_year_quarternum <- vec_cast.clock_naive_time_point.clock_year_month
#' @export
vec_cast.clock_naive_time_point.clock_year_quarternum_quarterday <- vec_cast.clock_naive_time_point.clock_year_month
#' @export
vec_cast.clock_naive_time_point.clock_iso_year_weeknum <- vec_cast.clock_naive_time_point.clock_year_month
#' @export
vec_cast.clock_naive_time_point.clock_iso_year_weeknum_weekday <- vec_cast.clock_naive_time_point.clock_year_month

# ------------------------------------------------------------------------------
# zoned-time-point

#' @export
vec_cast.clock_zoned_time_point.clock_zoned_time_point <- function(x, to, ...) {
  calendar <- vec_cast(field_calendar(x), field_calendar(to))
  x <- set_calendar(x, calendar)

  x_zone <- zoned_time_point_zone(x)
  to_zone <- zoned_time_point_zone(to)

  if (x_zone != to_zone) {
    x <- zoned_time_point_set_zone(x, to_zone)
  }

  x_precision <- get_precision(x)
  to_precision <- get_precision(to)

  if (x_precision == to_precision) {
    x
  } else if (precision_value(x_precision) > precision_value(to_precision)) {
    stop_incompatible_cast(x, to, ...)
  } else {
    adjust_precision(x, to_precision)
  }
}
