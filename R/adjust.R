# ------------------------------------------------------------------------------

#' @export
adjust_hour <- function(x,
                        value,
                        ...,
                        dst_resolver = default_dst_resolver()) {
  adjust(
    x = x,
    value = value,
    ...,
    day_resolver = default_day_resolver(),
    dst_resolver = dst_resolver,
    adjuster = "hour"
  )
}

# ------------------------------------------------------------------------------

#' @export
adjust_minute <- function(x,
                          value,
                          ...,
                          dst_resolver = default_dst_resolver()) {
  adjust(
    x = x,
    value = value,
    ...,
    day_resolver = default_day_resolver(),
    dst_resolver = dst_resolver,
    adjuster = "minute"
  )
}

# ------------------------------------------------------------------------------

#' @export
adjust_second <- function(x,
                          value,
                          ...,
                          dst_resolver = default_dst_resolver()) {
  adjust(
    x = x,
    value = value,
    ...,
    day_resolver = default_day_resolver(),
    dst_resolver = dst_resolver,
    adjuster = "second"
  )
}

# ------------------------------------------------------------------------------

#' @export
adjust_day <- function(x,
                       value,
                       ...,
                       day_resolver = default_day_resolver(),
                       dst_resolver = default_dst_resolver()) {
  adjust(
    x = x,
    value = value,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    adjuster = "day"
  )
}

# ------------------------------------------------------------------------------

#' @export
adjust_last_day_of_month <- function(x,
                                     ...,
                                     dst_resolver = default_dst_resolver()) {
  adjust(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    day_resolver = default_day_resolver(),
    dst_resolver = dst_resolver,
    adjuster = "last_day_of_month"
  )
}

# ------------------------------------------------------------------------------

adjust <- function(x, value, ..., day_resolver, dst_resolver, adjuster) {
  if (is_local_datetime(x)) {
    adjust_local(
      x = x,
      value = value,
      ...,
      day_resolver = day_resolver,
      adjuster = adjuster
    )
  } else {
    adjust_zoned(
      x = x,
      value = value,
      day_resolver = day_resolver,
      dst_resolver = dst_resolver,
      adjuster = adjuster
    )
  }
}

adjust_zoned <- function(x, value, ..., day_resolver, dst_resolver, adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  x_ct <- to_posixct(x)

  validate_day_resolver(day_resolver)
  validate_dst_resolver(dst_resolver)

  out <- adjust_zoned_cpp(
    x = x_ct,
    value = value,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    size = size,
    adjuster = adjuster
  )

  if (is_hms_adjuster(adjuster)) {
    out
  } else {
    from_posixct(out, x)
  }
}

adjust_local <- function(x, value, ..., day_resolver, adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  validate_day_resolver(day_resolver)

  out <- adjust_local_cpp(
    x = x,
    value = value,
    day_resolver = day_resolver,
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
