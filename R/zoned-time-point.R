new_zoned_time_point <- function(calendar, precision, zone, ..., fields = NULL, names = NULL, class) {
  if (!is_string(zone)) {
    abort("`zone` must be a string.")
  }

  new_time_point(
    calendar = calendar,
    precision = precision,
    zone = zone,
    ...,
    fields = fields,
    names = names,
    class = c(class, "clock_zoned_time_point")
  )
}

#' @export
is_zoned_time_point <- function(x) {
  inherits(x, "clock_zoned_time_point")
}

# Internal option used when printing in tibbles
zoned_print_zone_name <- function(..., print_zone_name = TRUE) {
  print_zone_name
}

zoned_time_point_zone <- function(x) {
  attr(x, "zone", exact = TRUE)
}

zoned_time_point_set_zone <- function(x, zone) {
  attr(x, "zone") <- zone
  x
}

pretty_zone <- function(zone) {
  if (identical(zone, "")) {
    zone <- zone_current()
    zone <- paste0(zone, " (current)")
  }

  zone
}
