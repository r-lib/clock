#' @export
naive_millisecond_point <- function(calendar, hour = 0L, minute = 0L, second = 0L, millisecond = 0L) {
  naive_subsecond_point(calendar, hour, minute, second, millisecond, "millisecond")
}
#' @export
naive_microsecond_point <- function(calendar, hour = 0L, minute = 0L, second = 0L, microsecond = 0L) {
  naive_subsecond_point(calendar, hour, minute, second, microsecond, "microsecond")
}
#' @export
naive_nanoseconds <- function(calendar, hour = 0L, minute = 0L, second = 0L, nanosecond = 0L) {
  naive_subsecond_point(calendar, hour, minute, second, nanosecond, "nanosecond")
}

naive_subsecond_point <- function(calendar, hour, minute, second, subsecond, precision) {
  args <- list(
    hour = hour,
    minute = minute,
    second = second,
    subsecond = subsecond
  )

  calendar <- promote_precision_day(calendar)

  args <- vec_cast_common(!!!args, .to = integer())
  args <- vec_recycle_common(calendar = calendar, !!!args)

  fields <- convert_calendar_days_hour_minute_second_subsecond_to_naive_fields(
    calendar = args$calendar,
    hour = args$hour,
    minute = args$minute,
    second = args$second,
    subsecond = args$subsecond,
    precision = precision
  )

  new_naive_subsecond_point(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    nanoseconds_of_second = fields$nanoseconds_of_second,
    precision = precision
  )
}

new_naive_subsecond_point <- function(calendar,
                                      seconds_of_day,
                                      nanoseconds_of_second,
                                      precision,
                                      ...,
                                      names = NULL,
                                      class = NULL) {
  if (!is_integer(seconds_of_day)) {
    abort("`seconds_of_day` must be an integer vector.")
  }
  if (!is_integer(nanoseconds_of_second)) {
    abort("`nanoseconds_of_second` must be an integer vector.")
  }

  if (!is_string(precision)) {
    abort("`precision` must be a string.")
  }
  precisions <- c("millisecond", "microsecond", "nanosecond")
  if (!(precision %in% precisions)) {
    abort("`precision` must be one of: 'millisecond', 'microsecond', or 'nanosecond'.")
  }

  fields <- list(
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second
  )

  new_naive_time_point(
    calendar = calendar,
    precision = precision,
    ...,
    fields = fields,
    names = names,
    class = c(class, "clock_naive_subsecond_point")
  )
}

new_naive_subsecond_point_from_fields <- function(fields, precision, names = NULL) {
  new_naive_subsecond_point(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    nanoseconds_of_second = fields$nanoseconds_of_second,
    precision = precision,
    names = names
  )
}

#' @export
vec_proxy.clock_naive_subsecond_point <- function(x, ...) {
  proxy_time_point(x)
}

#' @export
vec_restore.clock_naive_subsecond_point <- function(x, to, ...) {
  fields <- restore_time_point_fields(x)
  names <- restore_time_point_names(x)
  precision <- get_precision(to)
  new_naive_subsecond_point_from_fields(fields, precision, names)
}

#' @export
vec_proxy_equal.clock_naive_subsecond_point <- function(x, ...) {
  proxy_equal_time_point(x)
}

#' @export
format.clock_naive_subsecond_point <- function(x, ..., format = NULL) {
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  nanoseconds_of_second <- field_nanoseconds_of_second(x)
  precision <- get_precision(x)

  # TODO: Better format method that completely uses native code?
  if (is_null(format)) {
    ymd <- format(calendar)
    hms <- format_naive_nanoseconds_of_second(
      calendar,
      seconds_of_day,
      nanoseconds_of_second,
      precision
    )
    out <- paste0(ymd, " ", hms)
    out[is.na(x)] <- NA_character_
  } else {
    out <- format_naive_subsecond_point(
      calendar = calendar,
      seconds_of_day = seconds_of_day,
      nanoseconds_of_second = nanoseconds_of_second,
      format = format,
      precision = precision
    )
  }

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_naive_subsecond_point <- function(x, ...) {
  precision <- get_precision(x)
  cal <- vec_ptype_full(field_calendar(x))
  paste0("time_point<", precision, "><", cal, ">")
}

#' @export
vec_ptype_abbr.clock_naive_subsecond_point <- function(x, ...) {
  precision <- get_precision_abbr(x)
  cal <- vec_ptype_abbr(field_calendar(x))
  paste0("tp<", precision, "><", cal, ">")
}

#' @export
is_naive_subsecond_point <- function(x) {
  inherits(x, "clock_naive_subsecond_point")
}
