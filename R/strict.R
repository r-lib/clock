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

in_strict_mode <- function() {
  strict <- getOption("clock.strict", default = FALSE)
  is_true(strict)
}
