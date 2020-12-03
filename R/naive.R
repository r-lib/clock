new_naive <- function(fields, ..., names = NULL, class = NULL) {
  new_civil_rcrd(fields, ..., names = names, class = c(class, "civil_naive"))
}

is_naive <- function(x) {
  inherits(x, "civil_naive")
}
