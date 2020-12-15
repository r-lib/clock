#' @export
naive_date_nanotime <- function(year,
                                month = 1L,
                                day = 1L,
                                hour = 0L,
                                minute = 0L,
                                second = 0L,
                                nanosecond = 0L,
                                ...,
                                day_nonexistent = "last-time") {
  check_dots_empty()

  args <- list(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second,
    nanosecond = nanosecond
  )

  size <- vec_size_common(!!!args)
  args <- vec_recycle_common(!!!args, .size = size)
  args <- vec_cast_common(!!!args, .to = integer())

  fields <- convert_year_month_day_hour_minute_second_nanosecond_to_naive_fields(
    args$year,
    args$month,
    args$day,
    args$hour,
    args$minute,
    args$second,
    args$nanosecond,
    day_nonexistent
  )

  days <- fields$days
  seconds_of_day <- fields$seconds_of_day
  nanoseconds_of_second <- fields$nanoseconds_of_second

  calendar <- new_year_month_day(days)

  new_naive_date_nanotime(calendar, seconds_of_day, nanoseconds_of_second)
}

#' @export
new_naive_date_nanotime <- function(calendar,
                                    seconds_of_day,
                                    nanoseconds_of_second,
                                    ...,
                                    names = NULL) {
  if (!is_year_month_day(calendar)) {
    abort("`calendar` must be a 'clock_year_month_day' calendar.")
  }

  new_naive_subsecond_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    precision = "nanosecond",
    ...,
    names = names,
    class = "clock_naive_date_nanotime"
  )
}

new_naive_date_nanotime_from_fields <- function(fields, names = NULL) {
  new_naive_date_nanotime(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    nanoseconds_of_second = fields$nanoseconds_of_second,
    names = names
  )
}

#' @export
vec_proxy.clock_naive_date_nanotime <- function(x, ...) {
  proxy_time_point(x)
}

#' @export
vec_restore.clock_naive_date_nanotime <- function(x, to, ...) {
  fields <- restore_time_point_fields(x)
  names <- restore_time_point_names(x)
  new_naive_date_nanotime_from_fields(fields, names)
}

#' @export
vec_proxy_equal.clock_naive_date_nanotime <- function(x, ...) {
  proxy_equal_time_point(x)
}

#' @export
vec_ptype_full.clock_naive_date_nanotime <- function(x, ...) {
  "date_time<nano>"
}

#' @export
vec_ptype_abbr.clock_naive_date_nanotime <- function(x, ...) {
  "dt_tm<nano>"
}

#' @export
is_naive_date_nanotime <- function(x) {
  inherits(x, "clock_naive_date_nanotime")
}
