new_zoned <- function(fields, zone, ..., names = NULL, class = NULL) {
  if (!is_string(zone)) {
    abort("`zone` must be a string.")
  }

  new_civil_rcrd(
    fields = fields,
    zone = zone,
    ...,
    names = names,
    class = c(class, "civil_zoned")
  )
}

is_zoned <- function(x) {
  inherits(x, "civil_zoned")
}

zoned_zone <- function(x) {
  attr(x, "zone", exact = TRUE)
}

pretty_zone <- function(zone) {
  if (identical(zone, "")) {
    "Local"
  } else {
    zone
  }
}
