new_naive_quarterly <- function(fields, start, ..., names = NULL, class = NULL) {
  new_naive(fields, start = start, ..., names = names, class = c(class, "civil_naive_quarterly"))
}

is_naive_quarterly <- function(x) {
  inherits(x, "civil_naive_quarterly")
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
