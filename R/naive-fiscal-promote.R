
LEVEL_NAIVE_FISCAL_YEAR = 0L
LEVEL_NAIVE_FISCAL_YEAR_QUARTERNUM = 1L
LEVEL_NAIVE_FISCAL_YEAR_QUARTERNUM_QUARTERDAY = 2L

naive_fiscal_level <- function(x) {
  if (is_fiscal_year_quarternum(x)) {
    LEVEL_NAIVE_FISCAL_YEAR_QUARTERNUM
  } else if (is_fiscal_year_quarternum_quarterday(x)) {
    LEVEL_NAIVE_FISCAL_YEAR_QUARTERNUM_QUARTERDAY
  } else {
    stop_civil_unsupported_class(x)
  }
}

# ------------------------------------------------------------------------------

promote_at_least_fiscal_year <- function(x) {
  if (is_at_least_fiscal_year(x)) {
    x
  } else {
    abort("Internal error: Should never get here.")
  }
}

is_at_least_fiscal_year <- function(x) {
  naive_fiscal_level(x) >= LEVEL_NAIVE_FISCAL_YEAR
}

# ------------------------------------------------------------------------------

promote_at_least_fiscal_year_quarternum <- function(x) {
  if (is_at_least_fiscal_year_quarternum(x)) {
    x
  } else {
    as_fiscal_year_quarternum(x)
  }
}

is_at_least_fiscal_year_quarternum <- function(x) {
  naive_fiscal_level(x) >= LEVEL_NAIVE_FISCAL_YEAR_QUARTERNUM
}

# ------------------------------------------------------------------------------

promote_at_least_fiscal_year_quarternum_quarterday <- function(x) {
  if (is_at_least_fiscal_year_quarternum_quarterday(x)) {
    x
  } else {
    as_fiscal_year_quarternum_quarterday(x)
  }
}

is_at_least_fiscal_year_quarternum_quarterday <- function(x) {
  naive_fiscal_level(x) >= LEVEL_NAIVE_FISCAL_YEAR_QUARTERNUM_QUARTERDAY
}
