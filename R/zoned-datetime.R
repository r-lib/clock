#' @export
zoned_datetime <- function(year,
                           month = 1L,
                           day = 1L,
                           hour = 0L,
                           minute = 0L,
                           second = 0L,
                           ...,
                           zone = "UTC",
                           day_nonexistent = "last-time",
                           dst_nonexistent = "roll-forward",
                           dst_ambiguous = "earliest") {
  out <- local_datetime(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second,
    ...,
    day_nonexistent = day_nonexistent
  )

  as_zoned_datetime(
    x = out,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

new_zoned_datetime <- function(days = integer(),
                               time_of_day = integer(),
                               zone = "UTC",
                               ...,
                               names = NULL) {
  if (!is_integer(days)) {
    abort("`days` must be an integer.")
  }
  if (!is_integer(time_of_day)) {
    abort("`time_of_day` must be an integer.")
  }

  if (length(days) != length(time_of_day)) {
    abort("All elements to `new_zoned_datetime()` must have the same length.")
  }

  fields <- list(
    days = days,
    time_of_day = time_of_day
  )

  new_zoned(
    fields,
    zone = zone,
    ...,
    names = names,
    class = "civil_zoned_datetime"
  )
}

new_zoned_datetime_from_fields <- function(fields, zone, names = NULL) {
  new_zoned_datetime(
    days = fields$days,
    time_of_day = fields$time_of_day,
    zone = zone,
    names = names
  )
}

#' @export
vec_proxy.civil_zoned_datetime <- function(x, ...) {
  proxy_civil_rcrd(x)
}

#' @export
vec_restore.civil_zoned_datetime <- function(x, to, ...) {
  fields <- restore_civil_rcrd_fields(x)
  names <- restore_civil_rcrd_names(x)
  zone <- zoned_zone(to)
  new_zoned_datetime_from_fields(fields, zone, names)
}

#' @export
vec_proxy_equal.civil_zoned_datetime <- function(x, ...) {
  proxy_equal_civil_rcrd(x)
}

#' @export
format.civil_zoned_datetime <- function(x,
                                        ...,
                                        format = fmt_zoned_datetime(),
                                        abbreviate_zone = FALSE) {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")

  zone <- zoned_zone(x)

  out <- format_zoned_datetime(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    format = format,
    abbreviate_zone = abbreviate_zone
  )

  names(out) <- names(x)

  out
}

#' @export
obj_print_data.civil_zoned_datetime <- function(x, ...) {
  # Default format() for zoned datetimes uses the zone name.
  # While this is good for a purely textual representation, it is
  # repetitive here since the `obj_print_header()` data has the time zone name.
  format <- fmt_zoned_datetime(zone_name = FALSE)
  obj_print_data_civil_rcrd(x, format)
}

# @export - lazy in .onLoad()
pillar_shaft.civil_zoned_datetime <- function(x, ...) {
  # Tibble header will already contain the time zone name
  format <- fmt_zoned_datetime(zone_name = FALSE)
  x <- format(x, format = format)
  pillar::new_pillar_shaft_simple(x)
}

#' @export
vec_ptype_full.civil_zoned_datetime <- function(x, ...) {
  zone <- zoned_zone(x)
  zone <- pretty_zone(zone)
  paste0("civil_datetime<", zone, ">")
}

#' @export
vec_ptype_abbr.civil_zoned_datetime <- function(x, ...) {
  zone <- zoned_zone(x)
  zone <- pretty_zone(zone)
  paste0("cvl_dttm<", zone, ">")
}

is_zoned_datetime <- function(x) {
  inherits(x, "civil_zoned_datetime")
}
