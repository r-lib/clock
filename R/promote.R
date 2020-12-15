PRECISION_YEAR = 0L
PRECISION_QUARTER = 1L
PRECISION_MONTH = 2L
PRECISION_WEEK = 3L
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
    week = PRECISION_WEEK,
    day = PRECISION_DAY,
    second = PRECISION_SECOND,
    millisecond = PRECISION_MILLISECOND,
    microsecond = PRECISION_MICROSECOND,
    nanosecond = PRECISION_NANOSECOND,
    abort("Unknown precision.")
  )
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
