#' @export
adjust_year <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_year")
}

#' @export
adjust_year.Date <- function(x,
                             value,
                             ...,
                             day_nonexistent = "last-time") {
  adjust_date(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "year")
}

#' @export
adjust_year.POSIXt <- function(x,
                               value,
                               ...,
                               day_nonexistent = "last-time",
                               dst_nonexistent = "roll-forward",
                               dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "year"
  )
}

#' @export
adjust_year.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "year")
}

# ------------------------------------------------------------------------------

#' @export
adjust_month <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_month")
}

#' @export
adjust_month.Date <- function(x,
                              value,
                              ...,
                              day_nonexistent = "last-time") {
  adjust_date(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "month")
}

#' @export
adjust_month.POSIXt <- function(x,
                                value,
                                ...,
                                day_nonexistent = "last-time",
                                dst_nonexistent = "roll-forward",
                                dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "month"
  )
}

#' @export
adjust_month.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "month")
}

# ------------------------------------------------------------------------------

#' @export
adjust_day <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_day")
}

#' @export
adjust_day.Date <- function(x,
                            value,
                            ...,
                            day_nonexistent = "last-time") {
  adjust_date(x, value, ..., day_nonexistent = day_nonexistent, adjuster = "day")
}

#' @export
adjust_day.POSIXt <- function(x,
                              value,
                              ...,
                              day_nonexistent = "last-time",
                              dst_nonexistent = "roll-forward",
                              dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "day"
  )
}

#' @export
adjust_day.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "day")
}

# ------------------------------------------------------------------------------

#' @export
adjust_hour <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_hour")
}

#' @export
adjust_hour.Date <- function(x, value, ...) {
  adjust_date(x, value, ..., day_nonexistent = "last-time", adjuster = "hour")
}

#' @export
adjust_hour.POSIXt <- function(x,
                               value,
                               ...,
                               dst_nonexistent = "roll-forward",
                               dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "hour"
  )
}

#' @export
adjust_hour.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "hour")
}

# ------------------------------------------------------------------------------

#' @export
adjust_minute <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_minute")
}

#' @export
adjust_minute.Date <- function(x, value, ...) {
  adjust_date(x, value, ..., day_nonexistent = "last-time", adjuster = "minute")
}

#' @export
adjust_minute.POSIXt <- function(x,
                                 value,
                                 ...,
                                 dst_nonexistent = "roll-forward",
                                 dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "minute"
  )
}

#' @export
adjust_minute.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "minute")
}

# ------------------------------------------------------------------------------

#' @export
adjust_second <- function(x, value, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_second")
}

#' @export
adjust_second.Date <- function(x, value, ...) {
  adjust_date(x, value, ..., day_nonexistent = "last-time", adjuster = "second")
}

#' @export
adjust_second.POSIXt <- function(x,
                                 value,
                                 ...,
                                 dst_nonexistent = "roll-forward",
                                 dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = value,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "second"
  )
}

#' @export
adjust_second.civil_local <- function(x, value, ...) {
  adjust_local(x, value, ..., adjuster = "second")
}

# ------------------------------------------------------------------------------

#' @export
adjust_last_day_of_month <- function(x, ...) {
  restrict_civil_supported(x)
  UseMethod("adjust_last_day_of_month")
}

#' @export
adjust_last_day_of_month.Date <- function(x, ...) {
  adjust_date(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    day_nonexistent = "last-time",
    adjuster = "last_day_of_month"
  )
}

#' @export
adjust_last_day_of_month.POSIXt <- function(x,
                                            ...,
                                            dst_nonexistent = "roll-forward",
                                            dst_ambiguous = "earliest") {
  adjust_posixt(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "last_day_of_month"
  )
}

#' @export
adjust_last_day_of_month.civil_local <- function(x, ...) {
  adjust_local(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    adjuster = "last_day_of_month"
  )
}

# ------------------------------------------------------------------------------

adjust_zoned <- function(x,
                         value,
                         ...,
                         day_nonexistent,
                         dst_nonexistent,
                         dst_ambiguous,
                         adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  x_ct <- to_posixct(x)

  validate_day_nonexistent(day_nonexistent)
  validate_dst_nonexistent(dst_nonexistent)
  validate_dst_ambiguous(dst_ambiguous)

  out <- adjust_zoned_cpp(
    x = x_ct,
    value = value,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size,
    adjuster = adjuster
  )

  if (is_time_based_adjuster(adjuster)) {
    out
  } else {
    from_posixct(out, x)
  }
}

adjust_date <- function(x, value, ..., day_nonexistent, adjuster) {
  adjust_zoned(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = "roll-forward",
    dst_ambiguous = "earliest",
    adjuster = adjuster
  )
}

adjust_posixt <- function(x,
                          value,
                          ...,
                          day_nonexistent,
                          dst_nonexistent,
                          dst_ambiguous,
                          adjuster) {
  adjust_zoned(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = adjuster
  )
}

# ------------------------------------------------------------------------------

adjust_local <- function(x, value, ..., adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  to <- x
  x <- to_local_datetime(x)

  out <- adjust_local_cpp(
    x = x,
    value = value,
    size = size,
    adjuster = adjuster
  )

  if (is_time_based_adjuster(adjuster)) {
    out
  } else {
    from_local_datetime(out, to)
  }
}

# ------------------------------------------------------------------------------

is_time_based_adjuster <- function(adjuster) {
  adjuster %in% time_based_adjusters()
}

time_based_adjusters <- function() {
  c(
    "hour",
    "minute",
    "second"
  )
}

# ------------------------------------------------------------------------------

VALUE_PLACEHOLDER = -1L
