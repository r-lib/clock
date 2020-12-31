new_zoned_time <- function(sys_time = sys_seconds(),
                           zone = "UTC",
                           ...,
                           names = NULL,
                           class = NULL) {
  if (!is_string(zone)) {
    abort("`zone` must be a string.")
  }
  if (!is_sys_time(sys_time)) {
    abort("`sys_time` must be a 'time_point' using the 'sys' clock.")
  }
  if (time_point_precision(sys_time) == "day") {
    abort("`sys_time` must be at least second precision.")
  }

  fields <- list(sys_time = sys_time)

  new_clock_rcrd(
    fields = fields,
    zone = zone,
    ...,
    names = names,
    class = c(class, "clock_zoned_time")
  )
}

is_zoned_time <- function(x) {
  inherits(x, "clock_zoned_time")
}

# ------------------------------------------------------------------------------

zoned_time_zone <- function(x) {
  attr(x, "zone", exact = TRUE)
}
zoned_time_sys_time <- function(x) {
  field(x, "sys_time")
}
zoned_time_precision <- function(x) {
  time_point_precision(zoned_time_sys_time(x))
}

# ------------------------------------------------------------------------------

#' @export
format.clock_zoned_time <- function(x,
                                    ...,
                                    format = NULL,
                                    locale = default_date_locale(),
                                    abbreviate_zone = FALSE) {
  if (!is_date_locale(locale)) {
    abort("`locale` must be a date locale object.")
  }

  zone <- zoned_time_zone(x)
  sys_time <- zoned_time_sys_time(x)
  duration <- time_point_duration(sys_time)
  precision <- duration_precision(duration)

  if (is_null(format)) {
    # Collect internal option
    print_zone_name <- zoned_time_print_zone_name(...)
    format <- zoned_time_format(print_zone_name)
  }

  ticks <- field_ticks(duration)
  ticks_of_day <- field_ticks_of_day(duration, strict = FALSE)
  ticks_of_second <- field_ticks_of_second(duration, strict = FALSE)

  date_names <- locale$date_names
  decimal_mark <- locale$decimal_mark

  out <- format_zoned_time_cpp(
    ticks = ticks,
    ticks_of_day = ticks_of_day,
    ticks_of_second = ticks_of_second,
    zone = zone,
    abbreviate_zone = abbreviate_zone,
    format = format,
    precision = precision,
    mon = date_names$mon,
    mon_ab = date_names$mon_ab,
    day = date_names$day,
    day_ab = date_names$day_ab,
    am_pm = date_names$am_pm,
    decimal_mark = decimal_mark
  )

  names(out) <- names(x)

  out
}

zoned_time_print_zone_name <- function(..., print_zone_name = TRUE) {
  print_zone_name
}
zoned_time_format <- function(print_zone_name) {
  if (print_zone_name) {
    "%Y-%m-%dT%H:%M:%S%Ez[%Z]"
  } else {
    "%Y-%m-%dT%H:%M:%S%Ez"
  }
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype_full.clock_zoned_time <- function(x, ...) {
  zone <- zone_pretty(zoned_time_zone(x))
  precision <- zoned_time_precision(x)
  paste0("zoned_time<", precision, "><", zone, ">")
}

#' @export
vec_ptype_abbr.clock_zoned_time <- function(x, ...) {
  zone <- zone_pretty(zoned_time_zone(x))
  precision <- zoned_time_precision(x)
  precision <- precision_abbr(precision)
  paste0("zt<", precision, "><", zone, ">")
}

zone_pretty <- function(zone) {
  if (identical(zone, "")) {
    zone <- zone_current()
    zone <- paste0(zone, " (current)")
  }
  zone
}

# ------------------------------------------------------------------------------

# - Pass through internal option to not print zone name
# - Unlike vctrs, don't use `print(quote = FALSE)` since we want to match base R
#' @export
obj_print_data.clock_zoned_time <- function(x, ...) {
  if (vec_size(x) == 0L) {
    return(invisible(x))
  }

  out <- format(x, print_zone_name = FALSE)
  print(out)

  invisible(x)
}

# Align left to match pillar_shaft.Date
# @export - lazy in .onLoad()
pillar_shaft.clock_zoned_time <- function(x, ...) {
  out <- format(x, print_zone_name = FALSE)
  pillar::new_pillar_shaft_simple(out, align = "left")
}
