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

  new_naive_date_time(calendar, seconds_of_day)
}

#' @export
new_naive_date_time <- function(calendar,
                                seconds_of_day,
                                ...,
                                names = NULL) {
  if (!is_year_month_day(calendar)) {
    abort("`calendar` must be a 'clock_year_month_day' calendar.")
  }

  new_naive_second_point(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    ...,
    names = names,
    class = "clock_naive_date_time"
  )
}

new_naive_date_time_from_fields <- function(fields, names = NULL) {
  new_naive_date_time(
    calendar = fields$calendar,
    seconds_of_day = fields$seconds_of_day,
    names = names
  )
}

#' @export
vec_proxy.clock_naive_date_time <- function(x, ...) {
  proxy_time_point(x)
}

#' @export
vec_restore.clock_naive_date_time <- function(x, to, ...) {
  fields <- restore_time_point_fields(x)
  names <- restore_time_point_names(x)
  new_naive_date_time_from_fields(fields, names)
}

#' @export
vec_proxy_equal.clock_naive_date_time <- function(x, ...) {
  proxy_equal_time_point(x)
}

#' @export
vec_ptype_full.clock_naive_date_time <- function(x, ...) {
  "date_time<second>"
}

#' @export
vec_ptype_abbr.clock_naive_date_time <- function(x, ...) {
  "dt_tm<sec>"
}

#' @export
is_naive_date_time <- function(x) {
  inherits(x, "clock_naive_date_time")
}
