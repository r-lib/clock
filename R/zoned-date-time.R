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

  as_zoned_date_time(
    x = out,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )
}

new_zoned_date_time <- function(calendar,
                                seconds_of_day,
                                zone,
                                ...,
                                names = NULL) {
  if (!is_year_month_day(calendar)) {
    abort("`calendar` must be a 'clock_year_month_day' calendar.")
  }

  new_zoned_second_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    zone = zone,
    ...,
    names = names,
    class = "clock_zoned_date_time"
  )
}

new_zoned_date_time_from_fields <- function(fields, zone, names = NULL) {
  new_zoned_date_time(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    zone = zone,
    names = names
  )
}

#' @export
vec_proxy.clock_zoned_date_time <- function(x, ...) {
  proxy_time_point(x)
}

#' @export
vec_restore.clock_zoned_date_time <- function(x, to, ...) {
  fields <- restore_time_point_fields(x)
  names <- restore_time_point_names(x)
  zone <- zoned_time_point_zone(to)
  new_zoned_date_time_from_fields(fields, zone, names)
}

#' @export
vec_proxy_equal.clock_zoned_date_time <- function(x, ...) {
  proxy_equal_time_point(x)
}

#' @export
vec_ptype_full.clock_zoned_date_time <- function(x, ...) {
  zone <- zoned_time_point_zone(x)
  zone <- pretty_zone(zone)
  paste0("date_time<second><", zone, ">")
}

#' @export
vec_ptype_abbr.clock_zoned_date_time <- function(x, ...) {
  zone <- zoned_time_point_zone(x)
  zone <- pretty_zone(zone)
  paste0("dt_tm<sec><", zone, ">")
}

#' @export
is_zoned_date_time <- function(x) {
  inherits(x, "clock_zoned_date_time")
}
