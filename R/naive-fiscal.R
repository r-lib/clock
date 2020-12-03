new_naive_fiscal <- function(fields, fiscal_start, ..., names = NULL, class = NULL) {
  new_naive(fields, fiscal_start = fiscal_start, ..., names = names, class = c(class, "civil_naive_fiscal"))
}

is_naive_fiscal <- function(x) {
  inherits(x, "civil_naive_fiscal")
}

get_fiscal_start <- function(x) {
  attr(x, "fiscal_start", exact = TRUE)
}

pretty_fiscal_start <- function(fiscal_start, abbreviate = FALSE) {
  # TODO: Locale aware?
  if (abbreviate) {
    month.abb[fiscal_start]
  } else {
    month.name[fiscal_start]
  }
}

cast_fiscal_start <- function(x) {
  vec_cast(x, integer(), x_arg = "fiscal_start")
}
