#' @export
new_zoned_time <- function(sys_time = sys_seconds(),
                           zone = "UTC",
                           ...,
                           names = NULL,
                           class = NULL) {
  if (!is_string(zone)) {
    abort("`zone` must be a string.")
  }
  if (!is_sys_time(sys_time)) {
    abort("`sys_time` must be a 'clock_sys_time'.")
  }
  if (precision_value(time_point_precision(sys_time)) < PRECISION_SECOND) {
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

#' @export
is_zoned_time <- function(x) {
  inherits(x, "clock_zoned_time")
}

# ------------------------------------------------------------------------------

zoned_time_zone <- function(x) {
  attr(x, "zone", exact = TRUE)
}
zoned_time_zone_set <- function(x, zone) {
  attr(x, "zone") <- zone
  x
}

zoned_time_sys_time <- function(x) {
  field(x, "sys_time")
}
zoned_time_naive_time <- function(x) {
  sys_time <- zoned_time_sys_time(x)
  zone <- zoned_time_zone(x)

  duration <- time_point_duration(sys_time)
  precision <- time_point_precision(sys_time)

  fields <- get_naive_time_cpp(duration, precision, zone)
  duration <- new_duration_from_fields(fields, precision)

  new_naive_time(duration)
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

  date_names <- locale$date_names
  decimal_mark <- locale$decimal_mark

  out <- format_zoned_time_cpp(
    fields = duration,
    zone = zone,
    abbreviate_zone = abbreviate_zone,
    format = format,
    precision_string = precision,
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
vec_proxy.clock_zoned_time <- function(x, ...) {
  names <- names(x)
  zoned_time_proxy(x, names)
}

zoned_time_proxy <- function(x, names = NULL) {
  x <- zoned_time_sys_time(x)
  time_point_proxy(x, names)
}

#' @export
vec_restore.clock_zoned_time <- function(x, to, ...) {
  names <- restore_rcrd_names(x)
  zoned_time_restore(x, to, names)
}

zoned_time_restore <- function(x, to, names = NULL) {
  zone <- zoned_time_zone(to)
  to <- zoned_time_sys_time(to)
  x <- time_point_restore(x, to)
  new_zoned_time(x, zone, names = names)
}

#' @export
vec_proxy_equal.clock_zoned_time <- function(x, ...) {
  # ptype2 / cast will prevent zoned times with different zones from being
  # compared, so the equality proxy doesn't need to worry about zone.
  zoned_time_proxy_equal(x)
}

zoned_time_proxy_equal <- function(x) {
  x <- zoned_time_sys_time(x)
  time_point_proxy_equal(x)
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

#' @export
vec_ptype2.clock_zoned_time.clock_zoned_time <- function(x, y, ...) {
  x_zone <- zoned_time_zone(x)
  y_zone <- zoned_time_zone(y)

  if (x_zone != y_zone) {
    stop_incompatible_type(x, y, ..., details = "Zones can't differ.")
  }

  x_precision <- zoned_time_precision(x)
  y_precision <- zoned_time_precision(y)

  if (x_precision == y_precision) {
    return(x)
  }

  x_precision_value <- precision_value(x_precision)
  y_precision_value <- precision_value(y_precision)

  if (x_precision_value > y_precision_value) {
    x
  } else {
    y
  }
}

#' @export
vec_cast.clock_zoned_time.clock_zoned_time <- function(x, to, ...) {
  x_zone <- zoned_time_zone(x)
  to_zone <- zoned_time_zone(to)

  if (x_zone != to_zone) {
    stop_incompatible_cast(x, to, ..., details = "Zones can't differ.")
  }

  x_sys_time <- zoned_time_sys_time(x)
  to_sys_time <- zoned_time_sys_time(to)

  x_duration <- time_point_duration(x_sys_time)
  to_duration <- time_point_duration(to_sys_time)

  x_precision <- duration_precision(x_duration)
  to_precision <- duration_precision(to_duration)

  if (x_precision == to_precision) {
    return(x)
  }

  x_precision_value <- precision_value(x_precision)
  to_precision_value <- precision_value(to_precision)

  if (x_precision_value > to_precision_value) {
    stop_incompatible_cast(x, to, ..., details = "Precision would be lost.")
  }

  duration <- duration_cast(x_duration, to_precision)

  sys_time <- new_sys_time(duration)

  new_zoned_time(sys_time, x_zone, names = names(x))
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

# ------------------------------------------------------------------------------

#' @export
get_zone.clock_zoned_time <- function(x) {
  zoned_time_zone(x)
}

# ------------------------------------------------------------------------------

#' @export
as_zoned_time <- function(x, ...) {
  UseMethod("as_zoned_time")
}

#' @export
as_zoned_time.default <- function(x, ...) {
  stop_clock_unsupported_conversion(x, "clock_zoned_time")
}

#' @export
as_zoned_time.clock_zoned_time <- function(x, ...) {
  x
}

# ------------------------------------------------------------------------------

#' @export
as_sys_time.clock_zoned_time <- function(x) {
  out <- zoned_time_sys_time(x)
  names(out) <- names(x)
  out
}

#' @export
as_naive_time.clock_zoned_time <- function(x) {
  out <- zoned_time_naive_time(x)
  names(out) <- names(x)
  out
}

# ------------------------------------------------------------------------------

#' @export
zoned_now <- function(zone) {
  sys_time <- sys_now()
  zone <- zone_validate(zone)
  new_zoned_time(sys_time, zone = zone)
}

# ------------------------------------------------------------------------------

#' @export
get_offset.clock_zoned_time <- function(x) {
  zone <- zoned_time_zone(x)
  sys_time <- zoned_time_sys_time(x)
  duration <- time_point_duration(sys_time)
  precision <- time_point_precision(sys_time)
  get_offset_cpp(duration, precision, zone)
}

# ------------------------------------------------------------------------------

#' @export
set_zone.clock_zoned_time <- function(x, zone) {
  zone <- zone_validate(zone)
  zoned_time_zone_set(x, zone)
}

# ------------------------------------------------------------------------------

zone_validate <- function(zone) {
  zone <- zone_standardize(zone)

  if (!zone_is_valid(zone)) {
    message <- paste0("'", zone, "' is not a known time zone.")
    abort(message)
  }

  zone
}
