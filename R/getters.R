#' @export
get_zone <- function(x) {
  if (is_Date(x)) {
    "UTC"
  } else if (is_POSIXct(x) || is_POSIXlt(x)) {
    zone_standardize(get_tzone(x))
  } else if (is_zoned(x)) {
    zoned_time_point_zone(x)
  } else {
    stop_civil_unsupported_class(x)
  }
}

# ------------------------------------------------------------------------------

#' @export
get_offset <- function(x) {
  restrict_zoned_or_base(x)
  UseMethod("get_offset")
}

#' @export
get_offset.Date <- function(x) {
  zeros_along(x)
}

#' @export
get_offset.POSIXt <- function(x) {
  x <- as_zoned_datetime(x)
  get_offset(x)
}

#' @export
get_offset.civil_zoned <- function(x) {
  days <- field(x, "days")
  time_of_day <- field(x, "time_of_day")
  zone <- zoned_zone(x)

  get_offset_cpp(days, time_of_day, zone)
}

# ------------------------------------------------------------------------------

#' @export
get_year <- function(x) {
  restrict_civil_supported(x)
  UseMethod("get_year")
}

#' @export
get_year.Date <- function(x) {
  x <- as_year_month_day(x)
  get_year(x)
}

#' @export
get_year.POSIXt <- function(x) {
  x <- as_zoned_datetime(x)
  get_year(x)
}

#' @export
get_year.civil_naive_gregorian <- function(x) {
  days <- field(x, "days")
  ymd <- convert_naive_days_to_year_month_day(days)
  ymd$year
}

#' @export
get_year.civil_naive_quarterly <- function(x) {
  days <- field(x, "days")
  start <- get_quarterly_start(x)
  yqnqd <- convert_naive_days_to_quarterly_year_quarternum_quarterday(days, start)
  yqnqd$year
}

#' @export
get_year.civil_naive_iso <- function(x) {
  days <- field(x, "days")
  yww <- convert_naive_days_to_iso_year_weeknum_weekday(days)
  yww$year
}

#' @export
get_year.civil_zoned <- function(x) {
  x <- as_naive(x)
  get_year(x)
}

# ------------------------------------------------------------------------------

#' @export
get_quarternum <- function(x, ...) {
  restrict_civil_supported(x)
  UseMethod("get_quarternum")
}

#' @export
get_quarternum.civil_naive_quarterly <- function(x, ...) {
  days <- field(x, "days")
  start <- get_quarterly_start(x)
  yqnqd <- convert_naive_days_to_quarterly_year_quarternum_quarterday(days, start)
  yqnqd$quarternum
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
