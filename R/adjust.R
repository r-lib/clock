#' @export
adjust_year <- function(x,
                        value,
                        ...,
                        day_nonexistent = "last-time",
                        dst_nonexistent = "next",
                        dst_ambiguous = "earliest") {
  adjust(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "year"
  )
}

# ------------------------------------------------------------------------------

#' @export
adjust_month <- function(x,
                         value,
                         ...,
                         day_nonexistent = "last-time",
                         dst_nonexistent = "next",
                         dst_ambiguous = "earliest") {
  adjust(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "month"
  )
}

# ------------------------------------------------------------------------------

#' @export
adjust_day <- function(x,
                       value,
                       ...,
                       day_nonexistent = "last-time",
                       dst_nonexistent = "next",
                       dst_ambiguous = "earliest") {
  adjust(
    x = x,
    value = value,
    ...,
    day_nonexistent = day_nonexistent,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "day"
  )
}

# ------------------------------------------------------------------------------

#' @export
adjust_hour <- function(x,
                        value,
                        ...,
                        dst_nonexistent = "next",
                        dst_ambiguous = "earliest") {
  adjust(
    x = x,
    value = value,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "hour"
  )
}

# ------------------------------------------------------------------------------

#' @export
adjust_minute <- function(x,
                          value,
                          ...,
                          dst_nonexistent = "next",
                          dst_ambiguous = "earliest") {
  adjust(
    x = x,
    value = value,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "minute"
  )
}

# ------------------------------------------------------------------------------

#' @export
adjust_second <- function(x,
                          value,
                          ...,
                          dst_nonexistent = "next",
                          dst_ambiguous = "earliest") {
  adjust(
    x = x,
    value = value,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "second"
  )
}

# ------------------------------------------------------------------------------

#' @export
adjust_last_day_of_month <- function(x,
                                     ...,
                                     dst_nonexistent = "next",
                                     dst_ambiguous = "earliest") {
  adjust(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    day_nonexistent = "last-time",
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    adjuster = "last_day_of_month"
  )
}

# ------------------------------------------------------------------------------

adjust <- function(x,
                   value,
                   ...,
                   day_nonexistent,
                   dst_nonexistent,
                   dst_ambiguous,
                   adjuster) {
  if (is_local_datetime(x)) {
    adjust_local(
      x = x,
      value = value,
      ...,
      day_nonexistent = day_nonexistent,
      adjuster = adjuster
    )
  } else {
    adjust_zoned(
      x = x,
      value = value,
      day_nonexistent = day_nonexistent,
      dst_nonexistent = dst_nonexistent,
      dst_ambiguous = dst_ambiguous,
      adjuster = adjuster
    )
  }
}

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

  if (is_hms_adjuster(adjuster)) {
    out
  } else {
    from_posixct(out, x)
  }
}

adjust_local <- function(x,
                         value,
                         ...,
                         day_nonexistent,
                         adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  validate_day_nonexistent(day_nonexistent)

  out <- adjust_local_cpp(
    x = x,
    value = value,
    day_nonexistent = day_nonexistent,
    size = size,
    adjuster = adjuster
  )

  new_local_datetime(out)
}

# ------------------------------------------------------------------------------

is_hms_adjuster <- function(adjuster) {
  adjuster %in% hms_adjuster()
}

hms_adjuster <- function() {
  c(
    "hour",
    "minute",
    "second"
  )
}

# ------------------------------------------------------------------------------

VALUE_PLACEHOLDER = -1L
