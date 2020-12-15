#' @export
naive_date_time <- function(year,
                            month = 1L,
                            day = 1L,
                            hour = 0L,
                            minute = 0L,
                            second = 0L,
                            ...,
                            day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second
  )

  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fields <- convert_year_month_day_hour_minute_second_to_naive_fields(
    args$year,
    args$month,
    args$day,
    args$hour,
    args$minute,
    args$second,
    day_nonexistent
  )

  days <- fields$days
  seconds_of_day <- fields$seconds_of_day

  calendar <- new_year_month_day(days)

  new_naive_second_point(calendar, seconds_of_day)
}

#' @export
naive_second_point <- function(calendar, hour = 0L, minute = 0L, second = 0L) {
  args <- list(
    hour = hour,
    minute = minute,
    second = second
  )

  calendar <- promote_precision_day(calendar)

  args <- vec_cast_common(!!!args, .to = integer())
  args <- vec_recycle_common(calendar = calendar, !!!args)

  fields <- convert_calendar_days_hour_minute_second_to_naive_fields(
    calendar = args$calendar,
    hour = args$hour,
    minute = args$minute,
    second = args$second
  )

  new_naive_second_point(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day
  )
}

#' @export
new_naive_second_point <- function(calendar,
                                   seconds_of_day,
                                   ...,
                                   names = NULL,
                                   class = NULL) {
  if (!is_integer(seconds_of_day)) {
    abort("`seconds_of_day` must be an integer vector.")
  }

  fields <- list(seconds_of_day = seconds_of_day)

  new_naive_time_point(
    calendar = calendar,
    precision = "second",
    ...,
    fields = fields,
    names = names,
    class = c(class, "clock_naive_second_point")
  )
}

new_naive_second_point_from_fields <- function(fields, names = NULL) {
  new_naive_second_point(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    names = names
  )
}

#' @export
vec_proxy.clock_naive_second_point <- function(x, ...) {
  proxy_time_point(x)
}

#' @export
vec_restore.clock_naive_second_point <- function(x, to, ...) {
  fields <- restore_time_point_fields(x)
  names <- restore_time_point_names(x)
  new_naive_seconds_from_fields(fields, names)
}

#' @export
vec_proxy_equal.clock_naive_second_point <- function(x, ...) {
  proxy_equal_time_point(x)
}

#' @export
format.clock_naive_second_point <- function(x, ..., format = NULL) {
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)

  # TODO: Improve with native format code
  if (is_null(format)) {
    ymd <- format(calendar)
    hms <- format_naive_seconds_of_day(calendar, seconds_of_day)
    out <- paste0(ymd, " ", hms)
    out[is.na(x)] <- NA_character_
  } else {
    out <- format_naive_second_point(
      calendar = calendar,
      seconds_of_day = seconds_of_day,
      format = format
    )
  }

  names(out) <- names(x)

  out
}

#' @export
vec_ptype_full.clock_naive_second_point <- function(x, ...) {
  cal <- vec_ptype_full(field_calendar(x))
  paste0("time_point<second><", cal, ">")
}

#' @export
vec_ptype_abbr.clock_naive_second_point <- function(x, ...) {
  cal <- vec_ptype_abbr(field_calendar(x))
  paste0("tp<sec><", cal, ">")
}

#' @export
is_naive_second_point <- function(x) {
  inherits(x, "clock_naive_second_point")
}
