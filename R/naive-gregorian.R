new_naive_gregorian <- function(fields, ..., names = NULL, class = NULL) {
  new_naive(fields, ..., names = names, class = c(class, "civil_naive_gregorian"))
}

is_naive_gregorian <- function(x) {
  inherits(x, "civil_naive_gregorian")
}
