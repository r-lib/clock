
LEVEL_NAIVE_QUARTERLY_YEAR = 0L
LEVEL_NAIVE_QUARTERLY_YEAR_QUARTERNUM = 1L
LEVEL_NAIVE_QUARTERLY_YEAR_QUARTERNUM_QUARTERDAY = 2L

naive_quarterly_level <- function(x) {
  if (is_year_quarternum(x)) {
    LEVEL_NAIVE_QUARTERLY_YEAR_QUARTERNUM
  } else if (is_year_quarternum_quarterday(x)) {
    LEVEL_NAIVE_QUARTERLY_YEAR_QUARTERNUM_QUARTERDAY
  } else {
    stop_civil_unsupported_class(x)
  }
}

# ------------------------------------------------------------------------------

promote_at_least_quarterly_year <- function(x) {
  if (is_at_least_quarterly_year(x)) {
    x
  } else {
    abort("Internal error: Should never get here.")
  }
}

is_at_least_quarterly_year <- function(x) {
  naive_quarterly_level(x) >= LEVEL_NAIVE_QUARTERLY_YEAR
}

# ------------------------------------------------------------------------------

promote_at_least_quarterly_year_quarternum <- function(x) {
  if (is_at_least_quarterly_year_quarternum(x)) {
    x
  } else {
    as_year_quarternum(x)
  }
}

is_at_least_quarterly_year_quarternum <- function(x) {
  naive_quarterly_level(x) >= LEVEL_NAIVE_QUARTERLY_YEAR_QUARTERNUM
}

# ------------------------------------------------------------------------------

promote_at_least_quarterly_year_quarternum_quarterday <- function(x) {
  if (is_at_least_quarterly_year_quarternum_quarterday(x)) {
    x
  } else {
    as_year_quarternum_quarterday(x)
  }
}

is_at_least_quarterly_year_quarternum_quarterday <- function(x) {
  naive_quarterly_level(x) >= LEVEL_NAIVE_QUARTERLY_YEAR_QUARTERNUM_QUARTERDAY
}
