# ------------------------------------------------------------------------------

get_precision_value <- function(x) {
  precision_value(get_precision(x))
}

# ------------------------------------------------------------------------------

promote_precision_year <- function(x) {
  UseMethod("promote_precision_year")
}

#' @export
promote_precision_year.clock_gregorian <- function(x) {
  if (get_precision_value(x) >= PRECISION_YEAR) {
    x
  } else {
    # as_gregorian_year(x)
  }
}

#' @export
promote_precision_year.clock_iso <- function(x) {
  if (get_precision_value(x) >= PRECISION_YEAR) {
    x
  } else {
    # as_iso_year(x)
  }
}

#' @export
promote_precision_year.clock_quarterly <- function(x) {
  if (get_precision_value(x) >= PRECISION_YEAR) {
    x
  } else {
    # as_quarterly_year(x)
  }
}

#' @export
promote_precision_year.clock_time_point <- function(x) {
  x
}

# ------------------------------------------------------------------------------

promote_precision_quarter <- function(x) {
  UseMethod("promote_precision_quarter")
}

#' @export
promote_precision_quarter.clock_quarterly <- function(x) {
  if (get_precision_value(x) >= PRECISION_QUARTER) {
    x
  } else {
    as_year_quarternum(x)
  }
}

#' @export
promote_precision_quarter.clock_time_point <- function(x) {
  x
}

# ------------------------------------------------------------------------------

promote_precision_month <- function(x) {
  UseMethod("promote_precision_month")
}

#' @export
promote_precision_month.clock_gregorian <- function(x) {
  if (get_precision_value(x) >= PRECISION_MONTH) {
    x
  } else {
    as_year_month(x)
  }
}

#' @export
promote_precision_month.clock_time_point <- function(x) {
  x
}

# ------------------------------------------------------------------------------

promote_precision_week <- function(x) {
  UseMethod("promote_precision_week")
}

#' @export
promote_precision_week.clock_iso <- function(x) {
  if (get_precision_value(x) >= PRECISION_WEEK) {
    x
  } else {
    as_iso_year_weeknum(x)
  }
}

#' @export
promote_precision_week.clock_time_point <- function(x) {
  x
}

# ------------------------------------------------------------------------------

promote_precision_day <- function(x) {
  UseMethod("promote_precision_day")
}

#' @export
promote_precision_day.clock_gregorian <- function(x) {
  if (get_precision_value(x) >= PRECISION_DAY) {
    x
  } else {
    # We prefer promoting things like year-month to year-month-day as opposed
    # to year-month-weekday, as this is probably what the user wanted
    as_year_month_day(x)
  }
}

#' @export
promote_precision_day.clock_iso <- function(x) {
  if (get_precision_value(x) >= PRECISION_DAY) {
    x
  } else {
    as_iso_year_weeknum_weekday(x)
  }
}

#' @export
promote_precision_day.clock_quarterly <- function(x) {
  if (get_precision_value(x) >= PRECISION_DAY) {
    x
  } else {
    as_year_quarternum_quarterday(x)
  }
}

#' @export
promote_precision_day.clock_time_point <- function(x) {
  x
}

# ------------------------------------------------------------------------------

promote_precision_second <- function(x) {
  UseMethod("promote_precision_second")
}

#' @export
promote_precision_second.clock_calendar <- function(x) {
  as_naive_time_point(x, precision = "second")
}

#' @export
promote_precision_second.clock_time_point <- function(x) {
  if (get_precision_value(x) >= PRECISION_SECOND) {
    x
  } else {
    adjust_precision(x, "second")
  }
}

# ------------------------------------------------------------------------------

promote_precision_millisecond <- function(x) {
  UseMethod("promote_precision_millisecond")
}

#' @export
promote_precision_millisecond.clock_calendar <- function(x) {
  as_naive_time_point(x, precision = "millisecond")
}

#' @export
promote_precision_millisecond.clock_time_point <- function(x) {
  if (get_precision_value(x) >= PRECISION_MILLISECOND) {
    x
  } else {
    adjust_precision(x, "millisecond")
  }
}

# ------------------------------------------------------------------------------

promote_precision_microsecond <- function(x) {
  UseMethod("promote_precision_microsecond")
}

#' @export
promote_precision_microsecond.clock_calendar <- function(x) {
  as_naive_time_point(x, precision = "microsecond")
}

#' @export
promote_precision_microsecond.clock_time_point <- function(x) {
  if (get_precision_value(x) >= PRECISION_MICROSECOND) {
    x
  } else {
    adjust_precision(x, "microsecond")
  }
}

# ------------------------------------------------------------------------------

promote_precision_nanosecond <- function(x) {
  UseMethod("promote_precision_nanosecond")
}

#' @export
promote_precision_nanosecond.clock_calendar <- function(x) {
  as_naive_time_point(x, precision = "nanosecond")
}

#' @export
promote_precision_nanosecond.clock_time_point <- function(x) {
  if (get_precision_value(x) >= PRECISION_NANOSECOND) {
    x
  } else {
    adjust_precision(x, "nanosecond")
  }
}
