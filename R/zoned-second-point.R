#' @export
zoned_second_point <- function(calendar,
                               hour = 0L,
                               minute = 0L,
                               second = 0L,
                               ...,
                               zone = "UTC",
                               dst_nonexistent = "roll-forward",
                               dst_ambiguous = "earliest") {
  out <- naive_second_point(calendar, hour, minute, second)
  as_zoned_second_point(out, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

new_zoned_second_point <- function(calendar,
                                   seconds_of_day,
                                   zone,
                                   ...,
                                   names = NULL,
                                   class = NULL) {
  if (!is_integer(seconds_of_day)) {
    abort("`seconds_of_day` must be an integer vector.")
  }

  fields <- list(seconds_of_day = seconds_of_day)

  new_zoned_time_point(
    calendar = calendar,
    precision = "second",
    zone = zone,
    ...,
    fields = fields,
    names = names,
    class = c(class, "clock_zoned_second_point")
  )
}

new_zoned_second_point_from_fields <- function(fields, zone, names = NULL) {
  new_zoned_second_point(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    zone = zone,
    names = names
  )
}

#' @export
vec_proxy.clock_zoned_second_point <- function(x, ...) {
  proxy_time_point(x)
}

#' @export
vec_restore.clock_zoned_second_point <- function(x, to, ...) {
  fields <- restore_time_point_fields(x)
  names <- restore_time_point_names(x)
  zone <- zoned_time_point_zone(to)
  new_zoned_second_point_from_fields(fields, zone, names)
}

#' @export
vec_proxy_equal.clock_zoned_second_point <- function(x, ...) {
  proxy_equal_time_point(x)
}

#' @export
format.clock_zoned_second_point <- function(x,
                                            ...,
                                            format = NULL,
                                            abbreviate_zone = FALSE) {
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  zone <- zoned_time_point_zone(x)

  if (is_null(format)) {
    ymd <- format(calendar)

    # Collect internal option
    print_zone_name <- zoned_print_zone_name(...)

    hms <- format_zoned_seconds_of_day(
      calendar = calendar,
      seconds_of_day = seconds_of_day,
      zone = zone,
      abbreviate_zone = abbreviate_zone,
      print_zone_name = print_zone_name
    )

    out <- paste0(ymd, " ", hms)
    out[is.na(x)] <- NA_character_
  } else {
    out <- format_zoned_second_point(
      calendar = calendar,
      seconds_of_day = seconds_of_day,
      zone = zone,
      format = format,
      abbreviate_zone = abbreviate_zone
    )
  }

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_zoned_second_point <- function(x, ...) {
  zone <- zoned_time_point_zone(x)
  zone <- pretty_zone(zone)
  cal <- vec_ptype_full(field_calendar(x))
  paste0("time_point<second><", cal, "><", zone, ">")
}

#' @export
vec_ptype_abbr.clock_zoned_second_point <- function(x, ...) {
  zone <- zoned_time_point_zone(x)
  zone <- pretty_zone(zone)
  cal <- vec_ptype_abbr(field_calendar(x))
  paste0("tp<sec><", cal, "><", zone, ">")
}

#' @export
is_zoned_second_point <- function(x) {
  inherits(x, "clock_zoned_second_point")
}
