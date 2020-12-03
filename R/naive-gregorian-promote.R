
LEVEL_NAIVE_GREGORIAN_YEAR = 0L
LEVEL_NAIVE_GREGORIAN_YEAR_MONTH = 1L
LEVEL_NAIVE_GREGORIAN_YEAR_MONTH_DAY = 2L
LEVEL_NAIVE_GREGORIAN_DATETIME = 3L
LEVEL_NAIVE_GREGORIAN_NANO_DATETIME = 4L

naive_gregorian_level <- function(x) {
  if (is_year_month(x)) {
    LEVEL_NAIVE_GREGORIAN_YEAR_MONTH
  } else if (is_year_month_day(x)) {
    LEVEL_NAIVE_GREGORIAN_YEAR_MONTH_DAY
  } else if (is_naive_datetime(x)) {
    LEVEL_NAIVE_GREGORIAN_DATETIME
  } else if (is_naive_nano_datetime(x)) {
    LEVEL_NAIVE_GREGORIAN_NANO_DATETIME
  } else {
    stop_civil_unsupported_class(x)
  }
}

# ------------------------------------------------------------------------------

promote_at_least_year <- function(x) {
  if (is_at_least_year(x)) {
    x
  } else {
    # as_year(x)
  }
}

is_at_least_year <- function(x) {
  naive_gregorian_level(x) >= LEVEL_NAIVE_GREGORIAN_YEAR
}

# ------------------------------------------------------------------------------

promote_at_least_year_month <- function(x) {
  if (is_at_least_year_month(x)) {
    x
  } else {
    as_year_month(x)
  }
}

is_at_least_year_month <- function(x) {
  naive_gregorian_level(x) >= LEVEL_NAIVE_GREGORIAN_YEAR_MONTH
}

# ------------------------------------------------------------------------------

promote_at_least_year_month_day <- function(x) {
  if (is_at_least_year_month_day(x)) {
    x
  } else {
    as_year_month_day(x)
  }
}

is_at_least_year_month_day <- function(x) {
  naive_gregorian_level(x) >= LEVEL_NAIVE_GREGORIAN_YEAR_MONTH_DAY
}

# ------------------------------------------------------------------------------

promote_at_least_naive_datetime <- function(x) {
  if (is_at_least_naive_datetime(x)) {
    x
  } else {
    as_naive_datetime(x)
  }
}

is_at_least_naive_datetime <- function(x) {
  naive_gregorian_level(x) >= LEVEL_NAIVE_GREGORIAN_DATETIME
}

# ------------------------------------------------------------------------------

promote_at_least_naive_nano_datetime <- function(x) {
  if (is_at_least_naive_nano_datetime(x)) {
    x
  } else {
    as_naive_nano_datetime(x)
  }
}

is_at_least_naive_nano_datetime <- function(x) {
  naive_gregorian_level(x) >= LEVEL_NAIVE_GREGORIAN_NANO_DATETIME
}
