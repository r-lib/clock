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

check_precision <- function(x,
                            ...,
                            values = NULL,
                            arg = caller_arg(x),
                            call = caller_env()) {
  values <- values %||% precision_names()
  check_string(x, allow_empty = FALSE, arg = arg, call = call)
  arg_match0(x, values = values, arg_nm = arg, error_call = call)
}

check_precision_subsecond <- function(x, ..., arg = caller_arg(x), call = caller_env()) {
  check_precision(
    x = x,
    values = c("millisecond", "microsecond", "nanosecond"),
    arg = arg,
    call = call
  )
}

precision_to_integer <- function(x) {
  check_string(x, .internal = TRUE)

  switch(
    x,
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
    abort("`x` not recognized.", .internal = TRUE)
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

precision_time_names <- function() {
  c(
    "hour",
    "minute",
    "second",
    "millisecond",
    "microsecond",
    "nanosecond"
  )
}
