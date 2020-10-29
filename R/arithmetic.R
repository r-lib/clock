#' @export
add_years <- function(x,
                      n,
                      ...,
                      day_resolver = default_day_resolver(),
                      dst_resolver = default_dst_arithmetic_resolver()) {
  add_ymd(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    unit = "year"
  )
}

#' @export
subtract_years <- function(x,
                           n,
                           ...,
                           day_resolver = default_day_resolver(),
                           dst_resolver = default_dst_arithmetic_resolver()) {
  add_years(
    x = x,
    n = -n,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver
  )
}

# ------------------------------------------------------------------------------

#' @export
add_months <- function(x,
                       n,
                       ...,
                       day_resolver = default_day_resolver(),
                       dst_resolver = default_dst_arithmetic_resolver()) {
  add_ymd(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    unit = "month"
  )
}

#' @export
subtract_months <- function(x,
                            n,
                            ...,
                            day_resolver = default_day_resolver(),
                            dst_resolver = default_dst_arithmetic_resolver()) {
  add_months(
    x = x,
    n = -n,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver
  )
}

# ------------------------------------------------------------------------------

#' @export
add_weeks <- function(x,
                      n,
                      ...,
                      dst_resolver = default_dst_arithmetic_resolver()) {
  add_ymd(
    x = x,
    n = n,
    ...,
    day_resolver = default_day_resolver(),
    dst_resolver = dst_resolver,
    unit = "week"
  )
}

#' @export
subtract_weeks <- function(x,
                           n,
                           ...,
                           dst_resolver = default_dst_arithmetic_resolver()) {
  add_weeks(
    x = x,
    n = -n,
    ...,
    dst_resolver = dst_resolver
  )
}

# ------------------------------------------------------------------------------

#' @export
add_days <- function(x,
                     n,
                     ...,
                     dst_resolver = default_dst_arithmetic_resolver()) {
  add_ymd(
    x = x,
    n = n,
    ...,
    day_resolver = default_day_resolver(),
    dst_resolver = dst_resolver,
    unit = "day"
  )
}

#' @export
subtract_days <- function(x,
                          n,
                          ...,
                          dst_resolver = default_dst_arithmetic_resolver()) {
  add_days(
    x = x,
    n = -n,
    ...,
    dst_resolver = dst_resolver
  )
}

# ------------------------------------------------------------------------------

#' @export
add_hours <- function(x, n) {
  add_hms(x, n, unit = "hour")
}

#' @export
subtract_hours <- function(x, n) {
  add_hours(x, -n)
}

# ------------------------------------------------------------------------------

#' @export
add_minutes <- function(x, n) {
  add_hms(x, n, unit = "minute")
}

#' @export
subtract_minutes <- function(x, n) {
  add_minutes(x, -n)
}

# ------------------------------------------------------------------------------

#' @export
add_seconds <- function(x, n) {
  add_hms(x, n, unit = "second")
}

#' @export
subtract_seconds <- function(x, n) {
  add_seconds(x, -n)
}

# ------------------------------------------------------------------------------

add_ymd <- function(x, n, ..., day_resolver, dst_resolver, unit) {
  if (is_local_datetime(x)) {
    add_period_to_local(
      x = x,
      n = n,
      ...,
      day_resolver = day_resolver,
      unit = unit
    )
  } else {
    add_period_to_zoned(
      x = x,
      n = n,
      ...,
      day_resolver = day_resolver,
      dst_resolver = dst_resolver,
      unit = unit
    )
  }
}

add_hms <- function(x, n, ..., unit) {
  if (is_local_datetime(x)) {
    add_period_to_local(
      x = x,
      n = n,
      ...,
      day_resolver = default_day_resolver(),
      unit = unit
    )
  } else {
    add_duration_to_zoned(
      x = x,
      n = n,
      ...,
      unit = unit
    )
  }
}

# ------------------------------------------------------------------------------

add_period_to_zoned <- function(x, n, ..., day_resolver, dst_resolver, unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  validate_day_resolver(day_resolver)
  validate_dst_arithmetic_resolver(dst_resolver)

  x_ct <- to_posixct(x)

  out <- add_period_to_zoned_cpp(
    x = x_ct,
    n = n,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    size = size,
    unit = unit
  )

  from_posixct(out, x)
}

add_duration_to_zoned <- function(x, n, ..., unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  x_ct <- to_posixct(x)

  out <- add_duration_to_zoned_cpp(
    x = x_ct,
    n = n,
    size = size,
    unit = unit
  )

  out
}

add_period_to_local <- function(x, n, ..., day_resolver, unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  validate_day_resolver(day_resolver)

  out <- add_period_to_local_cpp(
    x = x,
    n = n,
    day_resolver = day_resolver,
    size = size,
    unit = unit
  )

  new_local_datetime(out)
}
