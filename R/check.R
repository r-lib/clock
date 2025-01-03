check_between_year <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(
    x,
    lower = clock_calendar_year_minimum,
    upper = clock_calendar_year_maximum,
    call = call,
    arg = arg
  )
}
check_between_quarter <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 1L, upper = 4L, call = call, arg = arg)
}
check_between_month <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 1L, upper = 12L, call = call, arg = arg)
}
check_between_week <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 1L, upper = 53L, call = call, arg = arg)
}
check_between_day_of_year <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 1L, upper = 366L, call = call, arg = arg)
}
check_between_day_of_quarter <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 1L, upper = 92L, call = call, arg = arg)
}
check_between_day_of_month <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 1L, upper = 31L, call = call, arg = arg)
}
check_between_day_of_week <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 1L, upper = 7L, call = call, arg = arg)
}
check_between_index_of_week <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 1L, upper = 5L, call = call, arg = arg)
}
check_between_hour <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 0L, upper = 23L, call = call, arg = arg)
}
check_between_minute <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 0L, upper = 59L, call = call, arg = arg)
}
check_between_second <- function(
  x,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  check_between(x, lower = 0L, upper = 59L, call = call, arg = arg)
}
check_between_subsecond <- function(
  x,
  precision,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  upper <- switch(
    precision_to_string(precision),
    millisecond = 999L,
    microsecond = 999999L,
    nanosecond = 999999999L,
    abort("Invalid precision.", .internal = TRUE)
  )
  check_between(x, lower = 0L, upper = upper, call = call, arg = arg)
}

check_between <- function(
  x,
  lower,
  upper,
  ...,
  call = caller_env(),
  arg = caller_arg(x)
) {
  ok <- (x >= lower & x <= upper) | vec_detect_missing(x)

  if (all(ok)) {
    return(invisible(NULL))
  }

  loc <- which(!ok)

  message <- c(
    "{.arg {arg}} must be between [{lower}, {upper}].",
    i = "Invalid results at locations: {loc}."
  )

  cli::cli_abort(message, call = call)
}
