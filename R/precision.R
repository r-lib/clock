PRECISION_YEAR = 0L
PRECISION_QUARTER = 1L
PRECISION_MONTH = 2L
PRECISION_WEEK = 3L
PRECISION_DAY = 4L
PRECISION_HOUR = 5L
PRECISION_MINUTE = 6L
PRECISION_SECOND = 7L
PRECISION_MILLISECOND = 8L
PRECISION_MICROSECOND = 9L
PRECISION_NANOSECOND = 10L

# ------------------------------------------------------------------------------

validate_precision_string <- function(precision, arg = "precision") {
  if (!is_string(precision)) {
    abort(paste0("`", arg, "` must be a string."))
  }

  switch(
    precision,
    year = PRECISION_YEAR,
    quarter = PRECISION_QUARTER,
    month = PRECISION_MONTH,
    week = PRECISION_WEEK,
    day = PRECISION_DAY,
    hour = PRECISION_HOUR,
    minute = PRECISION_MINUTE,
    second = PRECISION_SECOND,
    millisecond = PRECISION_MILLISECOND,
    microsecond = PRECISION_MICROSECOND,
    nanosecond = PRECISION_NANOSECOND,
    abort(paste0("`", arg, "` not recognized."))
  )
}

precision_abbr <- function(precision_string) {
  switch(
    precision_string,
    millisecond = "milli",
    microsecond = "micro",
    nanosecond = "nano",
    precision_string # fallthrough
  )
}

is_valid_precision <- function(precision) {
  is.integer(precision) &&
    length(precision) == 1L &&
    precision >= PRECISION_YEAR &&
    precision <= PRECISION_NANOSECOND
}

is_valid_subsecond_precision <- function(precision) {
  is.integer(precision) && precision >= PRECISION_MILLISECOND && precision <= PRECISION_NANOSECOND
}

precision_common2 <- function(x, y) {
  if (x >= y) {
    x
  } else {
    y
  }
}

precision_names <- function() {
  c(
    "year",
    "quarter",
    "month",
    "week",
    "day",
    "hour",
    "minute",
    "second",
    "millisecond",
    "microsecond",
    "nanosecond"
  )
}
