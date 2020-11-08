# ------------------------------------------------------------------------------

validate_dst_ambiguous <- function(x, arg = "dst_ambiguous") {
  is_valid <- x %in% dst_ambiguous_opts()

  if (any(!is_valid)) {
    options <- paste0(sQuote(dst_ambiguous_opts(), q = FALSE), collapse = ", ")
    abort(sprintf("`%s` must be one of: %s.", arg, options))
  }
}

dst_ambiguous_opts <- function() {
  c("earliest", "latest", "NA", "error")
}

# ------------------------------------------------------------------------------

validate_dst_ambiguous_arithmetic <- function(x, arg = "dst_ambiguous") {
  arg_match0(x, dst_ambiguous_arithmetic_opts(), arg)
}

dst_ambiguous_arithmetic_opts <- function() {
  c("directional", "earliest", "latest", "NA", "error")
}

# ------------------------------------------------------------------------------

validate_dst_nonexistent <- function(x, arg = "dst_nonexistent") {
  is_valid <- x %in% dst_nonexistent_opts()

  if (any(!is_valid)) {
    options <- paste0(sQuote(dst_nonexistent_opts(), q = FALSE), collapse = ", ")
    abort(sprintf("`%s` must be one of: %s.", arg, options))
  }
}

dst_nonexistent_opts <- function() {
  c(
    "roll-forward",
    "roll-backward",
    "shift-forward",
    "shift-backward",
    "NA",
    "error"
  )
}

# ------------------------------------------------------------------------------

validate_dst_nonexistent_arithmetic <- function(x, arg = "dst_nonexistent") {
  arg_match0(x, dst_nonexistent_arithmetic_opts(), arg)
}

dst_nonexistent_arithmetic_opts <- function() {
  c(
    "roll-directional",
    "roll-forward",
    "roll-backward",
    "shift-directional",
    "shift-forward",
    "shift-backward",
    "NA",
    "error"
  )
}

# ------------------------------------------------------------------------------

validate_day_nonexistent <- function(x, arg = "day_nonexistent") {
  arg_match0(x, day_nonexistent_opts(), arg)
}

day_nonexistent_opts <- function() {
  c("last-time", "first-time", "last-day", "first-day", "NA", "error")
}
