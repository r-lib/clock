
LEVEL_LOCAL_YEAR = 0L
LEVEL_LOCAL_YEAR_QUARTER = 1L
LEVEL_LOCAL_YEAR_MONTH = 2L
LEVEL_LOCAL_YEAR_WEEK = 3L
LEVEL_LOCAL_DATE = 4L
LEVEL_LOCAL_DATETIME = 5L
LEVEL_LOCAL_NANO_DATETIME = 6L

local_level <- function(x) {
  if (is_local_year_month(x)) {
    LEVEL_LOCAL_YEAR_MONTH
  } else if (is_local_date(x)) {
    LEVEL_LOCAL_DATE
  } else if (is_local_datetime(x)) {
    LEVEL_LOCAL_DATETIME
  } else {
    stop_civil_unsupported_class(x)
  }
}

# ------------------------------------------------------------------------------

promote_at_least_local_year <- function(x) {
  if (is_at_least_local_year(x)) {
    x
  } else {
    # as_local_year(x)
  }
}

is_at_least_local_year <- function(x) {
  local_level(x) >= LEVEL_LOCAL_YEAR
}

# ------------------------------------------------------------------------------

promote_at_least_local_year_quarter <- function(x) {
  if (is_at_least_local_year_quarter(x)) {
    x
  } else {
    # as_local_year_quarter(x)
  }
}

is_at_least_local_year_quarter <- function(x) {
  local_level(x) >= LEVEL_LOCAL_YEAR_QUARTER
}

# ------------------------------------------------------------------------------

promote_at_least_local_year_month <- function(x) {
  if (is_at_least_local_year_month(x)) {
    x
  } else {
    as_local_year_month(x)
  }
}

is_at_least_local_year_month <- function(x) {
  local_level(x) >= LEVEL_LOCAL_YEAR_MONTH
}

# ------------------------------------------------------------------------------

promote_at_least_local_year_week <- function(x) {
  if (is_at_least_local_year_week(x)) {
    x
  } else {
    # as_local_year_week(x)
  }
}

is_at_least_local_year_week <- function(x) {
  local_level(x) >= LEVEL_LOCAL_YEAR_WEEK
}

# ------------------------------------------------------------------------------

promote_at_least_local_date <- function(x) {
  if (is_at_least_local_date(x)) {
    x
  } else {
    as_local_date(x)
  }
}

is_at_least_local_date <- function(x) {
  local_level(x) >= LEVEL_LOCAL_DATE
}

# ------------------------------------------------------------------------------

promote_at_least_local_datetime <- function(x) {
  if (is_at_least_local_datetime(x)) {
    x
  } else {
    as_local_datetime(x)
  }
}

is_at_least_local_datetime <- function(x) {
  local_level(x) >= LEVEL_LOCAL_DATETIME
}

# ------------------------------------------------------------------------------

promote_at_least_local_nano_datetime <- function(x) {
  if (is_at_least_local_nano_datetime(x)) {
    x
  } else {
    # as_local_nano_datetime(x)
  }
}

is_at_least_local_nano_datetime <- function(x) {
  local_level(x) >= LEVEL_LOCAL_NANO_DATETIME
}
