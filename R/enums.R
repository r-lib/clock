# ------------------------------------------------------------------------------

validate_dst_ambiguous <- function(x, arg = "dst_ambiguous") {
  arg_match0(x, dst_ambiguous_opts(), arg)
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
  arg_match0(x, dst_nonexistent_opts(), arg)
}

dst_nonexistent_opts <- function() {
  c(
    "next",
    "previous",
    "next-shift",
    "previous-shift",
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
    "directional",
    "next",
    "previous",
    "directional-shift",
    "next-shift",
    "previous-shift",
    "NA",
    "error"
  )
}

# ------------------------------------------------------------------------------

validate_day_nonexistent <- function(x, arg = "day_nonexistent") {
  arg_match0(x, day_nonexistent_opts(), arg)
}

day_nonexistent_opts <- function() {
  c("end", "start", "end-keep", "start-keep", "NA", "error")
}
