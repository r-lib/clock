#' @export
get_zone <- function(x) {
  UseMethod("get_zone")
}

# ------------------------------------------------------------------------------

#' @export
get_offset <- function(x) {
  UseMethod("get_offset")
}

# ------------------------------------------------------------------------------

#' @export
get_year <- function(x) {
  UseMethod("get_year")
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

# ------------------------------------------------------------------------------

#' @export
get_quarternum <- function(x) {
  UseMethod("get_quarternum")
}

#' @export
get_quarternum.clock_quarterly <- function(x) {
  start <- get_start(x)
  yqnqd <- convert_calendar_days_to_year_quarternum_quarterday(x, start)
  yqnqd$quarternum
}

# ------------------------------------------------------------------------------

#' @export
get_quarterday <- function(x) {
  UseMethod("get_quarterday")
}

#' @export
get_quarterday.clock_quarterly <- function(x) {
  start <- get_start(x)
  yqnqd <- convert_calendar_days_to_year_quarternum_quarterday(x, start)
  yqnqd$quarterday
}

# ------------------------------------------------------------------------------

#' @export
get_weeknum <- function(x) {
  UseMethod("get_weeknum")
}

#' @export
get_weeknum.clock_iso <- function(x) {
  ywnwd <- convert_calendar_days_to_iso_year_weeknum_weekday(x)
  ywnwd$weeknum
}

# ------------------------------------------------------------------------------

#' @export
get_weekday <- function(x) {
  UseMethod("get_weekday")
}

#' @export
get_weekday.clock_iso <- function(x) {
  # [Monday, Sunday] -> [1, 7]
  ywnwd <- convert_calendar_days_to_iso_year_weeknum_weekday(x)
  ywnwd$weekday
}

# ------------------------------------------------------------------------------

#' @export
get_weekday_index <- function(x) {
  UseMethod("get_weekday_index")
}

# ------------------------------------------------------------------------------

#' @export
get_month <- function(x) {
  UseMethod("get_month")
}

# ------------------------------------------------------------------------------

#' @export
get_day <- function(x) {
  UseMethod("get_day")
}

# ------------------------------------------------------------------------------

#' @export
get_hour <- function(x) {
  UseMethod("get_hour")
}

# ------------------------------------------------------------------------------

#' @export
get_minute <- function(x) {
  UseMethod("get_minute")
}

# ------------------------------------------------------------------------------

#' @export
get_second <- function(x) {
  UseMethod("get_second")
}

# ------------------------------------------------------------------------------

#' @export
get_millisecond <- function(x) {
  UseMethod("get_millisecond")
}

# ------------------------------------------------------------------------------

#' @export
get_microsecond <- function(x) {
  UseMethod("get_microsecond")
}

# ------------------------------------------------------------------------------

#' @export
get_nanosecond <- function(x) {
  UseMethod("get_nanosecond")
}

# ------------------------------------------------------------------------------

#' @export
get_start <- function(x) {
  UseMethod("get_start")
}

#' @export
get_start.clock_quarterly <- function(x) {
  get_quarterly_start(x)
}

# ------------------------------------------------------------------------------

#' @export
get_precision <- function(x) {
  UseMethod("get_precision")
}
