new_quarterly <- function(fields, start = 1L, ..., names = NULL, class = NULL) {
  if (!is_number(start)) {
    abort("`start` must be a single number.")
  }

  new_calendar(fields, start = start, ..., names = names, class = c(class, "clock_quarterly"))
}

is_quarterly <- function(x) {
  inherits(x, "clock_quarterly")
}

get_quarterly_start <- function(x) {
  attr(x, "start", exact = TRUE)
}

pretty_quarterly_start <- function(start, abbreviate = FALSE) {
  # TODO: Locale aware?
  if (abbreviate) {
    month.abb[start]
  } else {
    month.name[start]
  }
}

cast_quarterly_start <- function(x) {
  vec_cast(x, integer(), x_arg = "start")
}

field_quarternum <- function(x) {
  field(x, "quarternum")
}
field_quarterday <- function(x) {
  field(x, "quarterday")
}
