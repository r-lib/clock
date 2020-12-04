new_naive_iso <- function(fields, ..., names = NULL, class = NULL) {
  new_naive(fields, ..., names = names, class = c(class, "civil_naive_iso"))
}

is_naive_iso <- function(x) {
  inherits(x, "civil_naive_iso")
}
