#' @export
is_zoned_time <- function(x) {
  inherits(x, "clock_zoned_time")
}

# ------------------------------------------------------------------------------

zoned_time_zone <- function(x) {
  attr(x, "zone", exact = TRUE)
}
zoned_time_set_zone <- function(x, zone) {
  attr(x, "zone") <- zone
  x
}

zoned_time_precision <- function(x) {
  attr(x, "precision", exact = TRUE)
}

# ------------------------------------------------------------------------------

#' @export
format.clock_zoned_time <- function(x,
                                    ...,
                                    format = NULL,
                                    locale = default_clock_locale(),
                                    abbreviate_zone = FALSE) {
  if (!is_clock_locale(locale)) {
    abort("`locale` must be a 'clock_locale' object.")
  }

  zone <- zoned_time_zone(x)
  precision <- zoned_time_precision(x)

  if (is_null(format)) {
    # Collect internal option
    print_zone_name <- zoned_time_print_zone_name(...)
    format <- zoned_time_format(print_zone_name)
  }

  mapping <- locale$mapping
  decimal_mark <- locale$decimal_mark

  out <- format_zoned_time_cpp(
    fields = x,
    zone = zone,
    abbreviate_zone = abbreviate_zone,
    format = format,
    precision_int = precision,
    mon = mapping$mon,
    mon_ab = mapping$mon_ab,
    day = mapping$day,
    day_ab = mapping$day_ab,
    am_pm = mapping$am_pm,
    decimal_mark = decimal_mark
  )

  names(out) <- clock_rcrd_names(x)

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

parse_zoned_time <- function(x,
                             ...,
                             format = "%Y-%m-%dT%H:%M:%S%Ez[%Z]",
                             precision = "second",
                             locale = default_clock_locale()) {
  check_dots_empty()

  precision <- validate_zoned_time_precision(precision)

  if (!is_clock_locale(locale)) {
    abort("`locale` must be a 'clock_locale' object.")
  }

  mapping <- locale$mapping
  mark <- locale$decimal_mark

  result <- parse_zoned_time_cpp(
    x,
    format,
    precision,
    mapping$mon,
    mapping$mon_ab,
    mapping$day,
    mapping$day_ab,
    mapping$am_pm,
    mark
  )

  new_zoned_time_from_fields(result$fields, precision, result$zone, names(x))
}

# ------------------------------------------------------------------------------

#' @export
vec_proxy.clock_zoned_time <- function(x, ...) {
  .Call("_clock_clock_rcrd_proxy", x, PACKAGE = "clock")
}

#' @export
vec_restore.clock_zoned_time <- function(x, to, ...) {
  .Call("_clock_zoned_time_restore", x, to, PACKAGE = "clock")
}

#' @export
vec_proxy_equal.clock_zoned_time <- function(x, ...) {
  # ptype2 / cast will prevent zoned times with different zones from being
  # compared, so the equality proxy doesn't need to worry about zone.
  clock_rcrd_proxy_equal(x)
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype_full.clock_zoned_time <- function(x, ...) {
  zone <- zone_pretty(zoned_time_zone(x))
  precision <- zoned_time_precision(x)
  precision <- precision_to_string(precision)
  paste0("zoned_time<", precision, "><", zone, ">")
}

#' @export
vec_ptype_abbr.clock_zoned_time <- function(x, ...) {
  zone <- zone_pretty(zoned_time_zone(x))
  precision <- zoned_time_precision(x)
  precision <- precision_to_string(precision)
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

  if (x_precision >= y_precision) {
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

  x_precision <- zoned_time_precision(x)
  to_precision <- zoned_time_precision(to)

  if (x_precision == to_precision) {
    return(x)
  }

  if (x_precision > to_precision) {
    stop_incompatible_cast(x, to, ..., details = "Precision would be lost.")
  }

  fields <- duration_cast_cpp(x, x_precision, to_precision)

  names <- clock_rcrd_names(x)

  new_zoned_time_from_fields(fields, to_precision, x_zone, names)
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
  names <- clock_rcrd_names(x)
  precision <- zoned_time_precision(x)
  new_sys_time_from_fields(x, precision, names)
}

#' @export
as_naive_time.clock_zoned_time <- function(x) {
  names <- clock_rcrd_names(x)
  zone <- zoned_time_zone(x)
  precision <- zoned_time_precision(x)
  fields <- get_naive_time_cpp(x, precision, zone)
  new_naive_time_from_fields(fields, precision, names)
}

# ------------------------------------------------------------------------------

#' @export
zoned_now <- function(zone) {
  names <- NULL
  sys_time <- sys_now()
  precision <- time_point_precision(sys_time)
  zone <- zone_validate(zone)
  new_zoned_time_from_fields(sys_time, precision, zone, names)
}

# ------------------------------------------------------------------------------

#' Retrieve or set the time zone
#'
#' @description
#' `zoned_zone()` retrieves the time zone.
#'
#' `zoned_set_zone()` sets the time zone _without changing the
#' underlying instant_. This means that the result will represent the equivalent
#' time in the new time zone.
#'
#' @param x `[zoned_time / Date / POSIXt]`
#'
#'   A zoned time to get or set the time zone of.
#'
#' @param zone `[character(1)]`
#'
#'   A valid time zone to switch to.
#'
#' @return
#' `zoned_zone()` returns a string containing the time zone.
#'
#' `zoned_set_zone()` returns `x` with an altered time zone attribute. The
#' underlying instant is _not_ changed.
#'
#' @name zoned-zone
#'
#' @examples
#' x <- as.POSIXct("2019-01-01 00:00:00", tz = "America/New_York")
#'
#' zoned_zone(x)
#'
#' # Equivalent UTC time
#' zoned_set_zone(x, "UTC")
#'
#' y <- as_zoned_time(x)
#'
#' # With a zoned-time
#' zoned_set_zone(y, "UTC")
#'
#' # To force a new time zone with the same wall time,
#' # convert to a naive time that has no implied time zone,
#' # then convert back to a zoned time in the new time zone.
#' nt <- as_naive_time(y)
#' nt
#' as_zoned_time(nt, "UTC")
NULL

#' @rdname zoned-zone
#' @export
zoned_zone <- function(x) {
  UseMethod("zoned_zone")
}

#' @export
zoned_zone.clock_zoned_time <- function(x) {
  zoned_time_zone(x)
}

#' @rdname zoned-zone
#' @export
zoned_set_zone <- function(x, zone) {
  UseMethod("zoned_set_zone")
}

#' @export
zoned_set_zone.clock_zoned_time <- function(x, zone) {
  zone <- zone_validate(zone)
  zoned_time_set_zone(x, zone)
}

# ------------------------------------------------------------------------------

#' Retrieve low-level zoned time information
#'
#' @description
#' `zoned_info()` retrieves a set of low-level information generally not
#' required for most date-time manipulations. It returns a data frame with
#' the following columns:
#'
#' - `begin`, `end`: Zoned times specifying the range of the current daylight
#'   saving time rule. The range is a half-open interval of `[begin, end)`.
#'
#' - `offset`: A second precision `duration` specifying the offset from UTC.
#'
#' - `dst`: A logical vector specifying if daylight saving time is currently
#'   active.
#'
#' - `abbreviation`: The time zone abbreviation in use throughout
#'   this `begin` to `end` range.
#'
#' @details
#' If there have never been any daylight saving time transitions, the minimum
#' supported year value is returned for `begin` (typically, a year value of
#' `-32767`).
#'
#' If daylight saving time is no longer used in a time zone, the maximum
#' supported year value is returned for `end` (typically, a year value of
#' `32767`).
#'
#' @param x `[zoned_time]`
#'
#'   A zoned time.
#'
#' @return A data frame of low level information to retrieve information for.
#'
#' @export
#' @examples
#' x <- year_month_day(2021, 03, 14, c(01, 03), c(59, 00), c(59, 00))
#' x <- as_naive_time(x)
#' x <- as_zoned_time(x, "America/New_York")
#'
#' # x[1] is in EST, x[2] is in EDT
#' x
#'
#' info <- zoned_info(x)
#' info
#'
#' # `end` can be used to iterate through daylight saving time transitions
#' # by repeatedly calling `zoned_info()`
#' zoned_info(info$end)
zoned_info <- function(x) {
  UseMethod("zoned_info")
}

#' @export
zoned_info.clock_zoned_time <- function(x) {
  names <- NULL
  zone <- zoned_time_zone(x)
  precision <- zoned_time_precision(x)

  out <- zoned_info_cpp(x, precision, zone)

  out[["begin"]] <- new_zoned_time_from_fields(out[["begin"]], PRECISION_SECOND, zone, names)
  out[["end"]] <- new_zoned_time_from_fields(out[["end"]], PRECISION_SECOND, zone, names)
  out[["offset"]] <- new_duration_from_fields(out[["offset"]], PRECISION_SECOND, names)

  new_data_frame(out)
}

# ------------------------------------------------------------------------------

#' Get the offset from UTC
#'
#' `zoned_offset()` returns the offset from UTC as a duration of seconds.
#'
#' @param x `[zoned_time / Date / POSIXt]`
#'
#'   A zoned time to extract the offset for.
#'
#' @return A seconds precision duration object the same size as `x`.
#' @export
#' @examples
#' # R defines Date as UTC
#' zoned_offset(as.Date("2019-01-01"))
#'
#' x <- year_month_day(2021, 03, 14, hour = c(1, 3))
#' x <- as_naive_time(x)
#' x <- as_zoned_time(x, "America/New_York")
#'
#' # Daylight savings time alters the offset from UTC
#' zoned_offset(x)
#'
#' # Can extract directly from POSIXct if you started with one of those
#' zoned_offset(as.POSIXct(x))
zoned_offset <- function(x) {
  UseMethod("zoned_offset")
}

#' @export
zoned_offset.clock_zoned_time <- function(x) {
  zoned_info(x)$offset
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

# ------------------------------------------------------------------------------

validate_zoned_time_precision <- function(precision) {
  precision <- validate_precision(precision)

  if (!is_valid_zoned_time_precision(precision)) {
    abort("`precision` must be at least 'second' precision.")
  }

  precision
}

is_valid_zoned_time_precision <- function(precision) {
  precision >= PRECISION_SECOND
}
