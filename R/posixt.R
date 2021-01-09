#' @export
as_sys_time.POSIXt <- function(x) {
  # The sys_time that would give the equivalent zoned time when a tz is attached
  get_sys_time(as_zoned_time(x))
}

#' @export
as_naive_time.POSIXt <- function(x) {
  get_naive_time(as_zoned_time(x))
}

#' @export
as_zoned_time.POSIXt <- function(x, ...) {
  check_dots_empty()

  x <- to_posixct(x)

  names <- names(x)
  seconds <- unstructure(x)
  zone <- get_zone(x)

  fields <- to_sys_duration_fields_from_sys_seconds_cpp(seconds)
  duration <- new_duration_from_fields(fields, "second")
  sys_time <- new_sys_time(duration)

  new_zoned_time(sys_time, zone = zone, names = names)
}

#' @export
as_year_month_day.POSIXt <- function(x) {
  as_year_month_day(as_naive_time(x))
}

#' @export
as_year_month_weekday.POSIXt <- function(x) {
  as_year_month_weekday(as_naive_time(x))
}

# ------------------------------------------------------------------------------

# Not using `check_dots_empty()` because that might
# be too aggressive with base generics. Also not passing `...` on to methods
# that do check empty dots.

# Using `tz = ""` to be compatible with the generic of `as.POSIXct()`

#' @export
as.POSIXct.clock_calendar <- function(x,
                                      tz = "",
                                      ...,
                                      nonexistent = "error",
                                      ambiguous = "error") {
  x <- as_naive_time(x)
  as.POSIXct(x, tz = tz, nonexistent = nonexistent, ambiguous = ambiguous)
}

#' @export
as.POSIXct.clock_sys_time <- function(x, tz = "", ...) {
  zone <- zone_standardize(tz)
  x <- time_point_cast(x, "second")
  duration <- time_point_duration(x)

  seconds <- to_sys_seconds_from_sys_duration_fields_cpp(
    fields = duration
  )

  names(seconds) <- names(x)
  new_datetime(seconds, zone)
}

#' @export
as.POSIXct.clock_naive_time <- function(x,
                                        tz = "",
                                        ...,
                                        nonexistent = "error",
                                        ambiguous = "error") {
  x <- as_zoned_time(x, zone = tz, nonexistent = nonexistent, ambiguous = ambiguous)
  as.POSIXct(x)
}

#' @export
as.POSIXct.clock_zoned_time <- function(x, ...) {
  zone <- zoned_time_zone(x)
  sys_time <- zoned_time_sys_time(x)
  as.POSIXct(sys_time, tz = zone)
}

# ------------------------------------------------------------------------------

#' @export
as.POSIXlt.clock_calendar <- function(x,
                                      tz = "",
                                      ...,
                                      nonexistent = "error",
                                      ambiguous = "error") {
  x <- as.POSIXct(x, tz = tz, ..., nonexistent = nonexistent, ambiguous = ambiguous)
  as.POSIXlt(x)
}

#' @export
as.POSIXlt.clock_sys_time <- function(x, tz = "", ...) {
  x <- as.POSIXct(x, tz = tz, ...)
  as.POSIXlt(x)
}

#' @export
as.POSIXlt.clock_naive_time <- as.POSIXlt.clock_calendar

#' @export
as.POSIXlt.clock_zoned_time <- function(x, ...) {
  x <- as.POSIXct(x, ...)
  as.POSIXlt(x)
}

# ------------------------------------------------------------------------------

#' @export
get_year.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_year)
}
#' @export
get_month.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_month)
}
#' @export
get_day.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_day)
}
#' @export
get_hour.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_hour)
}
#' @export
get_minute.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_minute)
}
#' @export
get_second.POSIXt <- function(x) {
  get_posixt_field_year_month_day(x, get_second)
}
get_posixt_field_year_month_day <- function(x, get_fn) {
  x <- as_year_month_day(x)
  get_fn(x)
}

#' @export
get_weekday.POSIXt <- function(x) {
  get_posixt_field_year_month_weekday(x, get_weekday)
}
#' @export
get_weekday_index.POSIXt <- function(x) {
  get_posixt_field_year_month_weekday(x, get_weekday_index)
}
get_posixt_field_year_month_weekday <- function(x, get_fn) {
  x <- as_year_month_weekday(x)
  get_fn(x)
}

#' @export
get_zone.POSIXt <- function(x) {
  zone_standardize(get_tzone(x))
}

#' @export
get_offset.POSIXt <- function(x) {
  x <- as_zoned_time(x)
  get_offset(x)
}

# ------------------------------------------------------------------------------

#' @export
set_zone.POSIXt <- function(x, zone) {
  x <- to_posixct(x)
  zone <- zone_validate(zone)
  set_tzone(x, zone)
}

# ------------------------------------------------------------------------------

#' @export
add_years.POSIXt <- function(x, n, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  add_posixt_duration_year_month_day(x, n, invalid, nonexistent, ambiguous, add_years, ...)
}
#' @export
add_quarters.POSIXt <- function(x, n, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  add_posixt_duration_year_month_day(x, n, invalid, nonexistent, ambiguous, add_quarters, ...)
}
#' @export
add_months.POSIXt <- function(x, n, ..., invalid = "error", nonexistent = "error", ambiguous = "error") {
  add_posixt_duration_year_month_day(x, n, invalid, nonexistent, ambiguous, add_months, ...)
}
add_posixt_duration_year_month_day <- function(x, n, invalid, nonexistent, ambiguous, add_fn, ...) {
  check_dots_empty()
  zone <- get_tzone(x)
  x <- as_year_month_day(x)
  x <- add_fn(x, n)
  x <- invalid_resolve(x, invalid = invalid)
  as.POSIXct(x, tz = zone, nonexistent = nonexistent, ambiguous = ambiguous)
}

#' @export
add_weeks.POSIXt <- function(x, n, ..., nonexistent = "error", ambiguous = "error") {
  add_posixt_duration_naive_time_point(x, n, nonexistent, ambiguous, add_weeks, ...)
}
#' @export
add_days.POSIXt <- function(x, n, ..., nonexistent = "error", ambiguous = "error") {
  add_posixt_duration_naive_time_point(x, n, nonexistent, ambiguous, add_days, ...)
}
add_posixt_duration_naive_time_point <- function(x, n, nonexistent, ambiguous, add_fn, ...) {
  check_dots_empty()
  zone <- get_tzone(x)
  x <- as_naive_time(x)
  x <- add_fn(x, n)
  as.POSIXct(x, tz = zone, nonexistent = nonexistent, ambiguous = ambiguous)
}

#' @export
add_hours.POSIXt <- function(x, n, ...) {
  add_posixt_duration_sys_time_point(x, n, add_hours, ...)
}
#' @export
add_minutes.POSIXt <- function(x, n, ...) {
  add_posixt_duration_sys_time_point(x, n, add_minutes, ...)
}
#' @export
add_seconds.POSIXt <- function(x, n, ...) {
  add_posixt_duration_sys_time_point(x, n, add_seconds, ...)
}
add_posixt_duration_sys_time_point <- function(x, n, add_fn, ...) {
  check_dots_empty()
  zone <- get_tzone(x)
  x <- as_sys_time(x)
  x <- add_fn(x, n)
  as.POSIXct(x, tz = zone)
}
