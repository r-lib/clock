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
  zone <- attr(x, "zone", exact = TRUE)
}

zoned_set_zone <- function(x, zone) {
  attr(x, "zone") <- zone
  x
}

zoned_zone_unambiguous <- function(x) {
  zone <- zoned_zone(x)

  if (identical(zone, "")) {
    zone_current()
  } else {
    zone
  }
}

pretty_zone <- function(zone) {
  if (identical(zone, "")) {
    zone <- zone_current()
    zone <- paste0(zone, " (current)")
  }

  zone
}

format_zoned_body_offset <- function(body, x) {
  offset <- get_offset(x)
  offset <- format_offset(offset)
  glue(body, offset)
}

format_zoned_body_zone <- function(body, x) {
  zone <- zoned_zone_unambiguous(x)
  glue(body, "[", zone, "]")
}
