
LEVEL_NAIVE_FISCAL_YEAR = 0L
LEVEL_NAIVE_FISCAL_YEAR_QUARTER = 1L
LEVEL_NAIVE_FISCAL_YEAR_QUARTER_DAY = 2L

naive_fiscal_level <- function(x) {
  if (is_fiscal_year_quarter(x)) {
    LEVEL_NAIVE_FISCAL_YEAR_QUARTER
  } else if (is_fiscal_year_quarter_day(x)) {
    LEVEL_NAIVE_FISCAL_YEAR_QUARTER_DAY
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

promote_at_least_fiscal_year_quarter <- function(x) {
  if (is_at_least_fiscal_year_quarter(x)) {
    x
  } else {
    as_fiscal_year_quarter(x)
  }
}

is_at_least_fiscal_year_quarter <- function(x) {
  naive_fiscal_level(x) >= LEVEL_NAIVE_FISCAL_YEAR_QUARTER
}

# ------------------------------------------------------------------------------

promote_at_least_fiscal_year_quarter_day <- function(x) {
  if (is_at_least_fiscal_year_quarter_day(x)) {
    x
  } else {
    as_fiscal_year_quarter_day(x)
  }
}

is_at_least_fiscal_year_quarter_day <- function(x) {
  naive_fiscal_level(x) >= LEVEL_NAIVE_FISCAL_YEAR_QUARTER_DAY
}
