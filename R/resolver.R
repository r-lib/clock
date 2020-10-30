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

# ------------------------------------------------------------------------------

#' @export
day_resolver <- function(..., nonexistent = "end") {
  check_dots_empty()

  arg_match0(nonexistent, day_nonexistent_opts(), "nonexistent")

  out <- list(nonexistent = nonexistent)
  structure(out, class = "civil_day_resolver")
}

#' @export
default_day_resolver <- function() {
  day_resolver()
}

#' @export
print.civil_day_resolver <- function(x, ...) {
  cat_line("<day_resolver>")
  cat_line("nonexistent: ", x$nonexistent)
  invisible(x)
}



is_day_resolver <- function(x) {
  inherits(x, "civil_day_resolver")
}

validate_day_resolver <- function(x) {
  if (is_day_resolver(x)) {
    invisible(x)
  } else {
    abort("`day_resolver` must be created by `day_resolver()`.")
  }
}

# ------------------------------------------------------------------------------

#' @export
dst_arithmetic_resolver <- function(...,
                                    nonexistent = "directional",
                                    ambiguous = "directional") {
  check_dots_empty()

  arg_match0(nonexistent, dst_arithmetic_nonexistent_opts(), "nonexistent")
  arg_match0(ambiguous, dst_arithmetic_ambiguous_opts(), "ambiguous")

  new_dst_resolver(nonexistent, ambiguous, class = "civil_dst_arithmetic_resolver")
}

#' @export
default_dst_arithmetic_resolver <- function() {
  dst_arithmetic_resolver()
}

#' @export
print.civil_dst_arithmetic_resolver <- function(x, ...) {
  cat_line("<dst_arithmetic_resolver>")
  cat_line("nonexistent: ", x$nonexistent)
  cat_line("ambiguous: ", x$ambiguous)
  invisible(x)
}

dst_arithmetic_nonexistent_opts <- function() {
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
dst_arithmetic_ambiguous_opts <- function() {
  c("directional", "earliest", "latest", "NA", "error")
}

validate_dst_arithmetic_resolver <- function(x) {
  if (is_dst_resolver(x)) {
    invisible(x)
  } else {
    abort("`dst_resolver` must be created by `dst_resolver()` or `dst_arithmetic_resolver()`.")
  }
}

# ------------------------------------------------------------------------------

new_dst_resolver <- function(nonexistent = "next",
                             ambiguous = "earliest",
                             class = character()) {
  out <- list(nonexistent = nonexistent, ambiguous = ambiguous)
  structure(out, class = c(class, "civil_dst_resolver"))
}

#' @export
dst_resolver <- function(..., nonexistent = "next", ambiguous = "earliest") {
  check_dots_empty()

  arg_match0(nonexistent, dst_nonexistent_opts(), "nonexistent")
  arg_match0(ambiguous, dst_ambiguous_opts(), "ambiguous")

  new_dst_resolver(nonexistent, ambiguous)
}

#' @export
default_dst_resolver <- function() {
  dst_resolver()
}

#' @export
print.civil_dst_resolver <- function(x, ...) {
  cat_line("<dst_resolver>")
  cat_line("nonexistent: ", x$nonexistent)
  cat_line("ambiguous: ", x$ambiguous)
  invisible(x)
}

dst_nonexistent_opts <- function() {
  c("next", "previous", "next-shift", "previous-shift", "NA", "error")
}
dst_ambiguous_opts <- function() {
  c("earliest", "latest", "NA", "error")
}

is_dst_resolver <- function(x) {
  inherits(x, "civil_dst_resolver")
}

validate_dst_resolver <- function(x) {
  if (is_dst_resolver(x)) {
    invisible(x)
  } else {
    abort("`dst_resolver` must be created by `dst_resolver()`.")
  }
}
