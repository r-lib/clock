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
  zoned_subsecond_point(
    calendar = calendar,
    hour = hour,
    minute = minute,
    second = second,
    subsecond = millisecond,
    ...,
    precision = "millisecond",
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
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
  zoned_subsecond_point(
    calendar = calendar,
    hour = hour,
    minute = minute,
    second = second,
    subsecond = microsecond,
    ...,
    precision = "microsecond",
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
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
  zoned_subsecond_point(
    calendar = calendar,
    hour = hour,
    minute = minute,
    second = second,
    subsecond = nanosecond,
    ...,
    precision = "nanosecond",
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

zoned_subsecond_point <- function(calendar,
                                  hour,
                                  minute,
                                  second,
                                  subsecond,
                                  ...,
                                  precision,
                                  zone,
                                  dst_nonexistent,
                                  dst_ambiguous) {
  out <- naive_subsecond_point(calendar, hour, minute, second, subsecond, precision)
  as_zoned_subsecond_point(out, zone, ..., dst_nonexistent = dst_nonexistent, dst_ambiguous = dst_ambiguous)
}

new_zoned_subsecond_point <- function(calendar,
                                      seconds_of_day,
                                      nanoseconds_of_second,
                                      precision,
                                      zone,
                                      ...,
                                      names = NULL,
                                      class = NULL) {
  if (!is_string(precision)) {
    abort("`precision` must be a string.")
  }
  precisions <- c("millisecond", "microsecond", "nanosecond")
  ok <- precision %in% precisions
  if (!ok) {
    abort("`precision` must be one of: 'millisecond', 'microsecond' or 'nanosecond'.")
  }

  if (!is_integer(seconds_of_day)) {
    abort("`seconds_of_day` must be an integer vector.")
  }
  if (!is_integer(nanoseconds_of_second)) {
    abort("`nanoseconds_of_second` must be an integer vector.")
  }

  fields <- list(
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second
  )

  new_zoned_time_point(
    calendar = calendar,
    precision = precision,
    zone = zone,
    ...,
    fields = fields,
    class = c(class, "clock_zoned_subsecond_point")
  )
}

new_zoned_subsecond_point_from_fields <- function(fields, precision, zone, names = NULL) {
  new_zoned_subsecond_point(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    nanoseconds_of_second = fields$nanoseconds_of_second,
    precision = precision,
    zone = zone,
    names = names
  )
}

#' @export
vec_proxy.clock_zoned_subsecond_point <- function(x, ...) {
  proxy_time_point(x)
}

#' @export
vec_restore.clock_zoned_subsecond_point <- function(x, to, ...) {
  fields <- restore_time_point_fields(x)
  names <- restore_time_point_names(x)
  zone <- zoned_time_point_zone(to)
  new_zoned_second_point_from_fields(fields, zone, names)
}

#' @export
vec_proxy_equal.clock_zoned_subsecond_point <- function(x, ...) {
  proxy_equal_time_point(x)
}

#' @export
format.clock_zoned_subsecond_point <- function(x,
                                               ...,
                                               format = NULL,
                                               abbreviate_zone = FALSE) {
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  nanoseconds_of_second <- field_nanoseconds_of_second(x)
  precision <- get_precision(x)
  zone <- zoned_time_point_zone(x)

  if (is_null(format)) {
    ymd <- format(calendar)

    # Collect internal option
    print_zone_name <- zoned_print_zone_name(...)

    hms <- format_zoned_nanoseconds_of_second(
      calendar = calendar,
      seconds_of_day = seconds_of_day,
      nanoseconds_of_second = nanoseconds_of_second,
      precision = precision,
      zone = zone,
      abbreviate_zone = abbreviate_zone,
      print_zone_name = print_zone_name
    )

    out <- paste0(ymd, " ", hms)
    out[is.na(x)] <- NA_character_
  } else {
    out <- format_zoned_subsecond_point(
      calendar = calendar,
      seconds_of_day = seconds_of_day,
      nanoseconds_of_second = nanoseconds_of_second,
      precision = precision,
      zone = zone,
      format = format,
      abbreviate_zone = abbreviate_zone
    )
  }

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_zoned_subsecond_point <- function(x, ...) {
  zone <- zoned_time_point_zone(x)
  zone <- pretty_zone(zone)
  cal <- vec_ptype_full(field_calendar(x))
  precision <- get_precision(x)
  paste0("time_point<", precision, "><", cal, "><", zone, ">")
}

#' @export
vec_ptype_abbr.clock_zoned_subsecond_point <- function(x, ...) {
  zone <- zoned_time_point_zone(x)
  zone <- pretty_zone(zone)
  cal <- vec_ptype_abbr(field_calendar(x))
  precision <- get_precision_abbr(x)
  paste0("tp<", precision, "><", cal, "><", zone, ">")
}

#' @export
is_zoned_subsecond_point <- function(x) {
  inherits(x, "clock_zoned_subsecond_point")
}
