#' @export
zoned_date_time <- function(year,
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
  out <- naive_date_time(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second,
    ...,
    day_nonexistent = day_nonexistent
  )

  as_zoned_time_point(
    x = out,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

#' @export
zoned_date_nanotime <- function(year,
                                month = 1L,
                                day = 1L,
                                hour = 0L,
                                minute = 0L,
                                second = 0L,
                                nanosecond = 0L,
                                ...,
                                zone = "UTC",
                                day_nonexistent = "last-time",
                                dst_nonexistent = "roll-forward",
                                dst_ambiguous = "earliest") {
  out <- naive_date_nanotime(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second,
    nanosecond = nanosecond,
    ...,
    day_nonexistent = day_nonexistent
  )

  as_zoned_time_point(
    x = out,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

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
  as_zoned_time_point(out, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
zoned_millisecond_point <- function(calendar,
                                    hour = 0L,
                                    minute = 0L,
                                    second = 0L,
                                    millisecond = 0L,
                                    ...,
                                    zone = "UTC",
                                    dst_nonexistent = "roll-forward",
                                    dst_ambiguous = "earliest") {
  out <- naive_millisecond_point(calendar, hour, minute, second, millisecond)
  as_zoned_time_point(out, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
zoned_microsecond_point <- function(calendar,
                                    hour = 0L,
                                    minute = 0L,
                                    second = 0L,
                                    microsecond = 0L,
                                    ...,
                                    zone = "UTC",
                                    dst_nonexistent = "roll-forward",
                                    dst_ambiguous = "earliest") {
  out <- naive_microsecond_point(calendar, hour, minute, second, microsecond)
  as_zoned_time_point(out, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

#' @export
zoned_nanosecond_point <- function(calendar,
                                   hour = 0L,
                                   minute = 0L,
                                   second = 0L,
                                   nanosecond = 0L,
                                   ...,
                                   zone = "UTC",
                                   dst_nonexistent = "roll-forward",
                                   dst_ambiguous = "earliest") {
  out <- naive_nanosecond_point(calendar, hour, minute, second, nanosecond)
  as_zoned_time_point(out, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

new_zoned_time_point <- function(calendar = new_year_month_day(),
                                 seconds_of_day = integer(),
                                 nanoseconds_of_second = NULL,
                                 precision = "second",
                                 zone = "UTC",
                                 ...,
                                 names = NULL,
                                 class = NULL) {
  if (!is_string(zone)) {
    abort("`zone` must be a string.")
  }

  new_time_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    precision = precision,
    zone = zone,
    ...,
    names = names,
    class = c(class, "clock_zoned_time_point")
  )
}

new_zoned_time_point_from_fields <- function(fields, precision, zone, names = NULL) {
  new_zoned_time_point(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    nanoseconds_of_second = fields$nanoseconds_of_second,
    precision = precision,
    zone = zone,
    names = names
  )
}

#' @export
vec_proxy.clock_zoned_time_point <- function(x, ...) {
  proxy_time_point(x)
}

#' @export
vec_restore.clock_zoned_time_point <- function(x, to, ...) {
  fields <- restore_time_point_fields(x)
  names <- restore_time_point_names(x)
  zone <- zoned_time_point_zone(to)
  precision <- get_precision(to)
  new_zoned_time_point_from_fields(fields, precision, zone, names)
}

#' @export
vec_proxy_equal.clock_zoned_time_point <- function(x, ...) {
  proxy_equal_time_point(x)
}

#' @export
format.clock_zoned_time_point <- function(x,
                                          ...,
                                          format = NULL,
                                          abbreviate_zone = FALSE) {
  # TODO: Better format method that completely uses native code?
  if (is_null(format)) {
    calendar <- field_calendar(x)
    ymd <- format(calendar)

    # Collect internal option
    print_zone_name <- zoned_print_zone_name(...)
    hms <- format_zoned_time_point_subdaily(x, abbreviate_zone, print_zone_name)

    out <- paste0(ymd, " ", hms)
    out[is.na(x)] <- NA_character_
  } else {
    out <- format_zoned_time_point(x, format, abbreviate_zone)
  }

  names(out) <- names(x)
  out
}

#' @export
vec_ptype_full.clock_zoned_time_point <- function(x, ...) {
  zone <- zoned_time_point_zone(x)
  zone <- pretty_zone(zone)
  cal <- vec_ptype_full(field_calendar(x))
  precision <- get_precision(x)
  paste0("time_point<", precision, "><", cal, "><", zone, ">")
}

#' @export
vec_ptype_abbr.clock_zoned_time_point <- function(x, ...) {
  zone <- zoned_time_point_zone(x)
  zone <- pretty_zone(zone)
  cal <- vec_ptype_abbr(field_calendar(x))
  precision <- get_precision_abbr(x)
  paste0("tp<", precision, "><", cal, "><", zone, ">")
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
