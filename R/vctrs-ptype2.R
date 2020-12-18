# ------------------------------------------------------------------------------
# year-month

#' @export
vec_ptype2.clock_year_month.clock_year_month <- function(x, y, ...) {
  x
}
#' @export
vec_ptype2.clock_year_month.clock_year_month_day <- function(x, y, ...) {
  y
}
#' @export
vec_ptype2.clock_year_month.clock_year_month_weekday <- function(x, y, ...) {
  y
}
#' @export
vec_ptype2.clock_year_month.clock_naive_time_point <- function(x, y, ...) {
  vec_ptype2(x, field_calendar(y))
  y
}

# ------------------------------------------------------------------------------
# year-month-day

#' @export
vec_ptype2.clock_year_month_day.clock_year_month_day <- function(x, y, ...) {
  x
}
#' @export
vec_ptype2.clock_year_month_day.clock_year_month <- function(x, y, ...) {
  x
}
#' @export
vec_ptype2.clock_year_month_day.clock_naive_time_point <- function(x, y, ...) {
  vec_ptype2(x, field_calendar(y))
  y
}

# ------------------------------------------------------------------------------
# year-month-weekday

#' @export
vec_ptype2.clock_year_month_weekday.clock_year_month_weekday <- function(x, y, ...) {
  x
}
#' @export
vec_ptype2.clock_year_month_weekday.clock_year_month <- function(x, y, ...) {
  x
}
#' @export
vec_ptype2.clock_year_month_weekday.clock_naive_time_point <- function(x, y, ...) {
  vec_ptype2(x, field_calendar(y))
  y
}

# ------------------------------------------------------------------------------
# year-quarternum

#' @export
vec_ptype2.clock_year_quarternum.clock_year_quarternum <- function(x, y, ...) {
  if (identical_quarterly_starts(x, y)) {
    x
  } else {
    stop_incompatible_type(x, y, ...)
  }
}
#' @export
vec_ptype2.clock_year_quarternum.clock_year_quarternum_quarterday <- function(x, y, ...) {
  if (identical_quarterly_starts(x, y)) {
    y
  } else {
    stop_incompatible_type(x, y, ...)
  }
}
#' @export
vec_ptype2.clock_year_quarternum.clock_naive_time_point <- function(x, y, ...) {
  vec_ptype2(x, field_calendar(y))
  y
}

identical_quarterly_starts <- function(x, y) {
  identical(get_quarterly_start(x), get_quarterly_start(y))
}

# ------------------------------------------------------------------------------
# year-quarternum-quarterday

#' @export
vec_ptype2.clock_year_quarternum_quarterday.clock_year_quarternum_quarterday <- function(x, y, ...) {
  if (identical_quarterly_starts(x, y)) {
    x
  } else {
    stop_incompatible_type(x, y, ...)
  }
}
#' @export
vec_ptype2.clock_year_quarternum_quarterday.clock_year_quarternum <- function(x, y, ...) {
  if (identical_quarterly_starts(x, y)) {
    x
  } else {
    stop_incompatible_type(x, y, ...)
  }
}
#' @export
vec_ptype2.clock_year_quarternum_quarterday.clock_naive_time_point <- function(x, y, ...) {
  vec_ptype2(x, field_calendar(y))
  y
}

# ------------------------------------------------------------------------------
# iso-year-weeknum

#' @export
vec_ptype2.clock_iso_year_weeknum.clock_iso_year_weeknum <- function(x, y, ...) {
  x
}
#' @export
vec_ptype2.clock_iso_year_weeknum.clock_iso_year_weeknum_weekday <- function(x, y, ...) {
  y
}
#' @export
vec_ptype2.clock_iso_year_weeknum.clock_naive_time_point <- function(x, y, ...) {
  vec_ptype2(x, field_calendar(y))
  y
}

# ------------------------------------------------------------------------------
# iso-year-weeknum-weekday

#' @export
vec_ptype2.clock_iso_year_weeknum_weekday.clock_iso_year_weeknum_weekday <- function(x, y, ...) {
  x
}
#' @export
vec_ptype2.clock_iso_year_weeknum_weekday.clock_iso_year_weeknum <- function(x, y, ...) {
  x
}
#' @export
vec_ptype2.clock_iso_year_weeknum_weekday.clock_naive_time_point <- function(x, y, ...) {
  vec_ptype2(x, field_calendar(y))
  y
}

# ------------------------------------------------------------------------------
# naive-time-point

#' @export
vec_ptype2.clock_naive_time_point.clock_naive_time_point <- function(x, y, ...) {
  vec_ptype2(field_calendar(x), field_calendar(y))

  if (get_precision_value(x) > get_precision_value(y)) {
    x
  } else {
    y
  }
}

# Note: Can always losslessly convert up from a calendar type to a naive time point.

#' @export
vec_ptype2.clock_naive_time_point.clock_year_month <- function(x, y, ...) {
  vec_ptype2(field_calendar(x), y)
  x
}
#' @export
vec_ptype2.clock_naive_time_point.clock_year_month_day <- vec_ptype2.clock_naive_time_point.clock_year_month
#' @export
vec_ptype2.clock_naive_time_point.clock_year_month_weekday <- vec_ptype2.clock_naive_time_point.clock_year_month
#' @export
vec_ptype2.clock_naive_time_point.clock_year_quarternum <- vec_ptype2.clock_naive_time_point.clock_year_month
#' @export
vec_ptype2.clock_naive_time_point.clock_year_quarternum_quarterday <- vec_ptype2.clock_naive_time_point.clock_year_month
#' @export
vec_ptype2.clock_naive_time_point.clock_iso_year_weeknum <- vec_ptype2.clock_naive_time_point.clock_year_month
#' @export
vec_ptype2.clock_naive_time_point.clock_iso_year_weeknum_weekday <- vec_ptype2.clock_naive_time_point.clock_year_month

# ------------------------------------------------------------------------------
# zoned-time-point

# Note: Can't always losslessly convert up from a calendar type to a
# zoned time point, so force users to go through `as_zoned_time_point()` to
# handle any issues with the extra arguments there.

#' @export
vec_ptype2.clock_zoned_time_point.clock_zoned_time_point <- function(x, y, ...) {
  vec_ptype2(field_calendar(x), field_calendar(y))

  if (!identical(zoned_time_point_zone(x), zoned_time_point_zone(y))) {
    stop_incompatible_type(x, y, ...)
  }

  if (get_precision_value(x) > get_precision_value(y)) {
    x
  } else {
    y
  }
}
