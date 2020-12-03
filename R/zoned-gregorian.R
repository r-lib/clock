new_zoned_gregorian <- function(fields, zone, ..., names = NULL, class = NULL) {
  new_zoned(fields, zone, ..., names = names, class = c(class, "civil_zoned_gregorian"))
}

is_zoned_gregorian <- function(x) {
  inherits(x, "civil_zoned_gregorian")
}
