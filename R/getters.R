#' @export
get_zone <- function(x) {
  restrict_zoned_or_base(x)
  UseMethod("get_zone")
}

#' @export
get_zone.clock_zoned_time_point <- function(x) {
  zoned_time_point_zone(x)
}

#' @export
get_zone.Date <- function(x) {
  "UTC"
}

#' @export
get_zone.POSIXt <- function(x) {
  zone_standardize(get_tzone(x))
}

# ------------------------------------------------------------------------------

#' @export
get_offset <- function(x) {
  restrict_zoned_or_base(x)
  UseMethod("get_offset")
}

#' @export
get_offset.clock_zoned_time_point <- function(x) {
  calendar <- field_calendar(x)
  seconds_of_day <- field_seconds_of_day(x)
  zone <- get_zone(x)
  get_offset_cpp(calendar, seconds_of_day, zone)
}

#' @export
get_offset.Date <- function(x) {
  zeros_along(x, na_propagate = TRUE)
}

#' @export
get_offset.POSIXt <- function(x) {
  x <- as_zoned_time_point(x)
  get_offset(x)
}

# ------------------------------------------------------------------------------

#' @export
get_year <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_year")
}

#' @export
get_year.clock_gregorian <- function(x) {
  ymd <- convert_calendar_days_to_year_month_day(x)
  ymd$year
}

#' @export
get_year.clock_quarterly <- function(x) {
  start <- get_quarterly_start(x)
  yqnqd <- convert_calendar_days_to_year_quarternum_quarterday(x, start)
  yqnqd$year
}

#' @export
get_year.clock_iso <- function(x) {
  yww <- convert_calendar_days_to_iso_year_weeknum_weekday(x)
  yww$year
}

#' @export
get_year.clock_naive_time_point <- function(x) {
  calendar <- field_calendar(x)
  get_year(calendar)
}

#' @export
get_year.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  get_year(x)
}

#' @export
get_year.Date <- function(x) {
  x <- as_year_month_day(x)
  get_year(x)
}

#' @export
get_year.POSIXt <- function(x) {
  x <- as_zoned_time_point(x)
  get_year(x)
}

# ------------------------------------------------------------------------------

#' @export
get_quarternum <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_quarternum")
}

#' @export
get_quarternum.clock_quarterly <- function(x) {
  start <- get_start(x)
  yqnqd <- convert_calendar_days_to_year_quarternum_quarterday(x, start)
  yqnqd$quarternum
}

#' @export
get_quarternum.clock_naive_time_point <- function(x) {
  calendar <- field_calendar(x)
  get_quarternum(calendar)
}

#' @export
get_quarternum.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  get_quarternum(x)
}

# ------------------------------------------------------------------------------

#' @export
get_quarterday <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_quarterday")
}

#' @export
get_quarterday.clock_quarterly <- function(x) {
  start <- get_start(x)
  yqnqd <- convert_calendar_days_to_year_quarternum_quarterday(x, start)
  yqnqd$quarterday
}

#' @export
get_quarterday.clock_naive_time_point <- function(x) {
  calendar <- field_calendar(x)
  get_quarterday(calendar)
}

#' @export
get_quarterday.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  get_quarterday(x)
}

# ------------------------------------------------------------------------------

#' @export
get_weeknum <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_weeknum")
}

#' @export
get_weeknum.clock_iso <- function(x) {
  ywnwd <- convert_calendar_days_to_iso_year_weeknum_weekday(x)
  ywnwd$weeknum
}

#' @export
get_weeknum.clock_naive_time_point <- function(x) {
  calendar <- field_calendar(x)
  get_weeknum(calendar)
}

#' @export
get_weeknum.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  get_weeknum(x)
}

# ------------------------------------------------------------------------------

#' @export
get_weekday <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_weekday")
}

#' @export
get_weekday.clock_iso <- function(x) {
  ywnwd <- convert_calendar_days_to_iso_year_weeknum_weekday(x)
  ywnwd$weekday
}

#' @export
get_weekday.clock_naive_time_point <- function(x) {
  calendar <- field_calendar(x)
  get_weekday(calendar)
}

#' @export
get_weekday.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  get_weekday(x)
}

# ------------------------------------------------------------------------------

#' @export
get_month <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_month")
}

#' @export
get_month.clock_gregorian <- function(x) {
  ymd <- convert_calendar_days_to_year_month_day(x)
  ymd$month
}

#' @export
get_month.clock_naive_time_point <- function(x) {
  calendar <- field_calendar(x)
  get_month(calendar)
}

#' @export
get_month.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  get_month(x)
}

#' @export
get_month.Date <- function(x) {
  x <- as_year_month_day(x)
  get_month(x)
}

#' @export
get_month.POSIXt <- function(x) {
  x <- as_zoned_time_point(x)
  get_month(x)
}

# ------------------------------------------------------------------------------

#' @export
get_day <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_day")
}

#' @export
get_day.clock_gregorian <- function(x) {
  ymd <- convert_calendar_days_to_year_month_day(x)
  ymd$day
}

#' @export
get_day.clock_naive_time_point <- function(x) {
  calendar <- field_calendar(x)
  get_day(calendar)
}

#' @export
get_day.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  get_day(x)
}

#' @export
get_day.Date <- function(x) {
  x <- as_year_month_day(x)
  get_day(x)
}

#' @export
get_day.POSIXt <- function(x) {
  x <- as_zoned_time_point(x)
  get_day(x)
}

# ------------------------------------------------------------------------------

#' @export
get_hour <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_hour")
}

#' @export
get_hour.clock_calendar <- function(x) {
  zeros_along(x, na_propagate = TRUE)
}

#' @export
get_hour.clock_naive_time_point <- function(x) {
  seconds_of_day <- field_seconds_of_day(x)
  hms <- convert_seconds_of_day_to_hour_minute_second(seconds_of_day)
  hms$hour
}

#' @export
get_hour.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  get_hour(x)
}

#' @export
get_hour.Date <- get_hour.clock_calendar

#' @export
get_hour.POSIXt <- function(x) {
  x <- as_zoned_time_point(x)
  get_hour(x)
}

# ------------------------------------------------------------------------------

#' @export
get_minute <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_minute")
}

#' @export
get_minute.clock_calendar <- function(x) {
  zeros_along(x, na_propagate = TRUE)
}

#' @export
get_minute.clock_naive_time_point <- function(x) {
  seconds_of_day <- field_seconds_of_day(x)
  hms <- convert_seconds_of_day_to_hour_minute_second(seconds_of_day)
  hms$minute
}

#' @export
get_minute.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  get_minute(x)
}

#' @export
get_minute.Date <- get_minute.clock_calendar

#' @export
get_minute.POSIXt <- function(x) {
  x <- as_zoned_time_point(x)
  get_minute(x)
}

# ------------------------------------------------------------------------------

#' @export
get_second <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_second")
}

#' @export
get_second.clock_calendar <- function(x) {
  zeros_along(x, na_propagate = TRUE)
}

#' @export
get_second.clock_naive_time_point <- function(x) {
  seconds_of_day <- field_seconds_of_day(x)
  hms <- convert_seconds_of_day_to_hour_minute_second(seconds_of_day)
  hms$second
}

#' @export
get_second.clock_zoned_time_point <- function(x) {
  x <- as_naive_time_point(x)
  get_second(x)
}

#' @export
get_second.Date <- get_second.clock_calendar

#' @export
get_second.POSIXt <- function(x) {
  x <- as_zoned_time_point(x)
  get_second(x)
}

# ------------------------------------------------------------------------------

#' @export
get_millisecond <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_millisecond")
}

#' @export
get_millisecond.clock_calendar <- function(x) {
  zeros_along(x, na_propagate = TRUE)
}

#' @export
get_millisecond.clock_time_point <- function(x) {
  if (is_subsecond_time_point(x)) {
    field_nanoseconds_of_second(x) %/% 1e6L
  } else {
    zeros_along(x, na_propagate = TRUE)
  }
}

#' @export
get_millisecond.Date <- get_millisecond.clock_calendar

#' @export
get_millisecond.POSIXt <- get_millisecond.clock_calendar

# ------------------------------------------------------------------------------

#' @export
get_microsecond <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_microsecond")
}

#' @export
get_microsecond.clock_calendar <- function(x) {
  zeros_along(x, na_propagate = TRUE)
}

#' @export
get_microsecond.clock_time_point <- function(x) {
  if (is_subsecond_time_point(x)) {
    field_nanoseconds_of_second(x) %/% 1e3L
  } else {
    zeros_along(x, na_propagate = TRUE)
  }
}

#' @export
get_microsecond.Date <- get_microsecond.clock_calendar

#' @export
get_microsecond.POSIXt <- get_microsecond.clock_calendar

# ------------------------------------------------------------------------------

#' @export
get_nanosecond <- function(x) {
  restrict_clock_supported(x)
  UseMethod("get_nanosecond")
}

#' @export
get_nanosecond.clock_calendar <- function(x) {
  zeros_along(x, na_propagate = TRUE)
}

#' @export
get_nanosecond.clock_time_point <- function(x) {
  if (is_subsecond_time_point(x)) {
    field_nanoseconds_of_second(x)
  } else {
    zeros_along(x, na_propagate = TRUE)
  }
}

#' @export
get_nanosecond.Date <- get_nanosecond.clock_calendar

#' @export
get_nanosecond.POSIXt <- get_nanosecond.clock_calendar

# ------------------------------------------------------------------------------

#' @export
get_start <- function(x) {
  UseMethod("get_start")
}

#' @export
get_start.clock_quarterly <- function(x) {
  get_quarterly_start(x)
}

#' @export
get_start.clock_time_point <- function(x) {
  get_start(field_calendar(x))
}

# ------------------------------------------------------------------------------

#' @export
get_precision <- function(x) {
  UseMethod("get_precision")
}

#' @export
get_precision.clock_year_month <- function(x) {
  "month"
}

#' @export
get_precision.clock_year_month_day <- function(x) {
  "day"
}

#' @export
get_precision.clock_iso_year_weeknum <- function(x) {
  "week"
}

#' @export
get_precision.clock_iso_year_weeknum_weekday <- function(x) {
  "day"
}

#' @export
get_precision.clock_year_quarternum <- function(x) {
  "quarter"
}

#' @export
get_precision.clock_year_quarternum_quarterday <- function(x) {
  "day"
}

#' @export
get_precision.clock_time_point <- function(x) {
  attr(x, "precision", exact = TRUE)
}
