PRECISION_YEAR = 0L
PRECISION_QUARTER = 1L
PRECISION_MONTH = 2L
PRECISION_ISO_WEEK = 3L
PRECISION_DAY = 4L
PRECISION_SECOND = 5L
PRECISION_MILLISECOND = 6L
PRECISION_MICROSECOND = 7L
PRECISION_NANOSECOND = 8L

# ------------------------------------------------------------------------------

get_precision_value <- function(x) {
  switch(
    get_precision(x),
    year = PRECISION_YEAR,
    quarter = PRECISION_QUARTER,
    month = PRECISION_MONTH,
    week = PRECISION_ISO_WEEK,
    day = PRECISION_DAY,
    second = PRECISION_SECOND,
    millisecond = PRECISION_MILLISECOND,
    microsecond = PRECISION_MICROSECOND,
    nanosecond = PRECISION_NANOSECOND,
    abort("Unknown precision.")
  )
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

promote_precision_iso_week <- function(x) {
  UseMethod("promote_precision_iso_week")
}

#' @export
promote_precision_iso_week.clock_iso <- function(x) {
  if (get_precision_value(x) >= PRECISION_ISO_WEEK) {
    x
  } else {
    as_iso_year_weeknum(x)
  }
}

#' @export
promote_precision_iso_week.clock_time_point <- function(x) {
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
