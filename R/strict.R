# ------------------------------------------------------------------------------

strict_validate_invalid <- function(invalid) {
  if (!is_null(invalid)) {
    return(invalid)
  }

  if (in_strict_mode()) {
    abort(paste0(
      "The global option, `clock.strict`, is currently set to `TRUE`. ",
      "In this mode, `invalid` must be set and cannot be left as `NULL`."
    ))
  }

  "error"
}

# ------------------------------------------------------------------------------

check_nonexistent_strict <- function(nonexistent, call = caller_env()) {
  if (!is_null(nonexistent)) {
    return(nonexistent)
  }

  if (in_strict_mode()) {
    message <- paste0(
      "The global option, `clock.strict`, is currently set to `TRUE`. ",
      "In this mode, `nonexistent` must be set and cannot be left as `NULL`."
    )
    abort(message, call = call)
  }

  "error"
}

# ------------------------------------------------------------------------------

check_ambiguous_strict <- function(ambiguous, call = caller_env()) {
  if (!is_null(ambiguous)) {
    return(ambiguous)
  }

  if (in_strict_mode()) {
    message <- paste0(
      "The global option, `clock.strict`, is currently set to `TRUE`. ",
      "In this mode, `ambiguous` must be set and cannot be left as `NULL`. ",
      "Additionally, `ambiguous` cannot be set to a zoned-time or POSIXct ",
      "unless it is paired with an ambiguous time resolution strategy, like: ",
      "`list(<zoned-time>, 'earliest')`."
    )
    abort(message, call = call)
  }

  "error"
}

# ------------------------------------------------------------------------------

in_strict_mode <- function() {
  strict <- getOption("clock.strict", default = FALSE)
  is_true(strict)
}
