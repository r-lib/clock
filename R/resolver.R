#' @export
dst_resolver <- function(..., nonexistant = "next", ambiguous = "earliest") {
  check_dots_empty()

  arg_match0(nonexistant, dst_nonexistant_opts(), "nonexistant")
  arg_match0(ambiguous, dst_ambiguous_opts(), "ambiguous")

  out <- list(nonexistant = nonexistant, ambiguous = ambiguous)
  structure(out, class = "civil_dst_resolver")
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
