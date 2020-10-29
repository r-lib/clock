#' @export
day_resolver <- function(..., nonexistant = "end") {
  check_dots_empty()

  arg_match0(nonexistant, day_nonexistant_opts(), "nonexistant")

  out <- list(nonexistant = nonexistant)
  structure(out, class = "civil_day_resolver")
}

#' @export
default_day_resolver <- function() {
  day_resolver()
}

#' @export
print.civil_day_resolver <- function(x, ...) {
  cat_line("<day_resolver>")
  cat_line("nonexistant: ", x$nonexistant)
  invisible(x)
}

day_nonexistant_opts <- function() {
  c("end", "start", "end-keep", "start-keep", "NA", "error")
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
                                    nonexistant = "directional",
                                    ambiguous = "directional") {
  check_dots_empty()

  arg_match0(nonexistant, dst_arithmetic_nonexistant_opts(), "nonexistant")
  arg_match0(ambiguous, dst_arithmetic_ambiguous_opts(), "ambiguous")

  new_dst_resolver(nonexistant, ambiguous, class = "civil_dst_arithmetic_resolver")
}

#' @export
default_dst_arithmetic_resolver <- function() {
  dst_arithmetic_resolver()
}

#' @export
print.civil_dst_arithmetic_resolver <- function(x, ...) {
  cat_line("<dst_arithmetic_resolver>")
  cat_line("nonexistant: ", x$nonexistant)
  cat_line("ambiguous: ", x$ambiguous)
  invisible(x)
}

dst_arithmetic_nonexistant_opts <- function() {
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

new_dst_resolver <- function(nonexistant = "next",
                             ambiguous = "earliest",
                             class = character()) {
  out <- list(nonexistant = nonexistant, ambiguous = ambiguous)
  structure(out, class = c(class, "civil_dst_resolver"))
}

#' @export
dst_resolver <- function(..., nonexistant = "next", ambiguous = "earliest") {
  check_dots_empty()

  arg_match0(nonexistant, dst_nonexistant_opts(), "nonexistant")
  arg_match0(ambiguous, dst_ambiguous_opts(), "ambiguous")

  new_dst_resolver(nonexistant, ambiguous)
}

#' @export
default_dst_resolver <- function() {
  dst_resolver()
}

#' @export
print.civil_dst_resolver <- function(x, ...) {
  cat_line("<dst_resolver>")
  cat_line("nonexistant: ", x$nonexistant)
  cat_line("ambiguous: ", x$ambiguous)
  invisible(x)
}

dst_nonexistant_opts <- function() {
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
