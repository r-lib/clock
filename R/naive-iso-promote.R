
LEVEL_NAIVE_ISO_YEAR = 0L
LEVEL_NAIVE_ISO_YEAR_WEEKNUM = 1L
LEVEL_NAIVE_ISO_YEAR_WEEKNUM_WEEKDAY = 2L

naive_iso_level <- function(x) {
  if (is_iso_year_weeknum(x)) {
    LEVEL_NAIVE_ISO_YEAR_WEEKNUM
  } else if (is_iso_year_weeknum_weekday(x)) {
    LEVEL_NAIVE_ISO_YEAR_WEEKNUM_WEEKDAY
  } else {
    stop_civil_unsupported_class(x)
  }
}

# ------------------------------------------------------------------------------

promote_at_least_iso_year <- function(x) {
  if (is_at_least_iso_year(x)) {
    x
  } else {
    # as_iso_year(x)
  }
}

is_at_least_iso_year <- function(x) {
  naive_iso_level(x) >= LEVEL_NAIVE_ISO_YEAR
}

# ------------------------------------------------------------------------------

promote_at_least_iso_year_weeknum <- function(x) {
  if (is_at_least_iso_year_weeknum(x)) {
    x
  } else {
    as_iso_year_weeknum(x)
  }
}

is_at_least_iso_year_weeknum <- function(x) {
  naive_iso_level(x) >= LEVEL_NAIVE_ISO_YEAR_WEEKNUM
}

# ------------------------------------------------------------------------------

promote_at_least_iso_year_weeknum_weekday <- function(x) {
  if (is_at_least_iso_year_weeknum_weekday(x)) {
    x
  } else {
    as_iso_year_weeknum_weekday(x)
  }
}

is_at_least_iso_year_weeknum_weekday <- function(x) {
  naive_iso_level(x) >= LEVEL_NAIVE_ISO_YEAR_WEEKNUM_WEEKDAY
}
