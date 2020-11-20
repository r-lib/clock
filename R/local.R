new_local <- function(fields, ..., names = NULL, class = NULL) {
  new_civil_rcrd(fields, ..., names = names, class = c(class, "civil_local"))
}

is_local <- function(x) {
  inherits(x, "civil_local")
}
