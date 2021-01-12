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

precision_value <- function(precision) {
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
    abort("Internal error: Unknown precision.")
  )
}

precision_field <- function(precision) {
  switch(
    precision,
    year = precision,
    quarter = precision,
    month = precision,
    week = precision,
    day = precision,
    hour = precision,
    minute = precision,
    second = precision,
    millisecond = "subsecond",
    microsecond = "subsecond",
    nanosecond = "subsecond",
    abort("Internal error: Unknown precision.")
  )
}

# ------------------------------------------------------------------------------

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

precision_abbr <- function(precision) {
  switch(
    precision,
    millisecond = "milli",
    microsecond = "micro",
    nanosecond = "nano",
    precision # fallthrough
  )
}

is_valid_precision <- function(precision) {
  is_string(precision) && is_true(precision %in% precision_names())
}

is_valid_subsecond_precision <- function(precision) {
  is_valid_precision(precision) && precision %in% c("millisecond", "microsecond", "nanosecond")
}

precision_common2 <- function(x, y) {
  if (precision_value(x) >= precision_value(y)) {
    x
  } else {
    y
  }
}

precision_subsecond_factor <- function(narrow_precision_value, wide_precision_value) {
  if (narrow_precision_value == PRECISION_MILLISECOND) {
    if (wide_precision_value == PRECISION_MILLISECOND) {
      1L
    } else if (wide_precision_value == PRECISION_MICROSECOND) {
      1e3L
    } else if (wide_precision_value == PRECISION_NANOSECOND) {
      1e6L
    } else {
      abort("Internal error: Invalid precision combination.")
    }
  } else if (narrow_precision_value == PRECISION_MICROSECOND) {
    if (wide_precision_value == PRECISION_MICROSECOND) {
      1L
    } else if (wide_precision_value == PRECISION_NANOSECOND) {
      1e3L
    } else {
      abort("Internal error: Invalid precision combination.")
    }
  } else if (narrow_precision_value == PRECISION_NANOSECOND) {
    if (wide_precision_value == PRECISION_NANOSECOND) {
      1L
    } else {
      abort("Internal error: Invalid precision combination.")
    }
  } else {
    abort("Internal error: Invalid precision combination.")
  }
}
