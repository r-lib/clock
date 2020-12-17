new_calendar <- function(days = integer(), ..., class = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }

  new_vctr(days, ..., class = c(class, "clock_calendar"))
}

is_calendar <- function(x) {
  inherits(x, "clock_calendar")
}

# - Each subclass implements a `format()` method
# - Unlike vctrs, don't use `print(quote = FALSE)` since we want to match base R
#' @export
obj_print_data.clock_calendar <- function(x, ...) {
  if (vec_size(x) == 0L) {
    return(invisible(x))
  }

  out <- format(x)
  print(out)

  invisible(x)
}

# Align left to match pillar_shaft.Date
# @export - lazy in .onLoad()
pillar_shaft.clock_calendar <- function(x, ...) {
  out <- format(x)
  pillar::new_pillar_shaft_simple(out, align = "left")
}
