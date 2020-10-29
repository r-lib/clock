# ------------------------------------------------------------------------------
# year

#' @export
add_years <- function(x, n, ...) {
  UseMethod("add_years")
}

#' @export
add_years.Date <- function(x,
                           n,
                           ...,
                           day_resolver = default_day_resolver()) {
  add_period_date(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    unit = "year"
  )
}

#' @export
add_years.POSIXct <- function(x,
                              n,
                              ...,
                              day_resolver = default_day_resolver(),
                              dst_resolver = default_dst_arithmetic_resolver()) {
  add_period_posixct(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    unit = "year"
  )
}

#' @export
add_years.POSIXlt <- function(x,
                              n,
                              ...,
                              day_resolver = default_day_resolver(),
                              dst_resolver = default_dst_arithmetic_resolver()) {
  add_period_posixlt(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    unit = "year"
  )
}

#' @export
add_years.civil_local_datetime <- function(x,
                                           n,
                                           ...,
                                           day_resolver = default_day_resolver()) {
  add_period_local(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    unit = "year"
  )
}

# ------------------------------------------------------------------------------
# month

#' @export
add_months <- function(x, n, ...) {
  UseMethod("add_months")
}

#' @export
add_months.Date <- function(x,
                            n,
                            ...,
                            day_resolver = default_day_resolver()) {
  add_period_date(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    unit = "month"
  )
}

#' @export
add_months.POSIXct <- function(x,
                               n,
                               ...,
                               day_resolver = default_day_resolver(),
                               dst_resolver = default_dst_arithmetic_resolver()) {
  add_period_posixct(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    unit = "month"
  )
}

#' @export
add_months.POSIXlt <- function(x,
                               n,
                               ...,
                               day_resolver = default_day_resolver(),
                               dst_resolver = default_dst_arithmetic_resolver()) {
  add_period_posixlt(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    unit = "month"
  )
}

#' @export
add_months.civil_local_datetime <- function(x,
                                            n,
                                            ...,
                                            day_resolver = default_day_resolver()) {
  add_period_local(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    unit = "month"
  )
}

# ------------------------------------------------------------------------------

add_period_date <- function(x, n, ..., day_resolver, unit) {
  x_ct <- to_posixct(x)

  out <- add_period_posixct(
    x = x_ct,
    n = n,
    ...,
    day_resolver = day_resolver,
    dst_resolver = default_dst_resolver(),
    unit = unit
  )

  from_posixct(out, x)
}

add_period_posixct <- function(x, n, ..., day_resolver, dst_resolver, unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  validate_day_resolver(day_resolver)
  validate_dst_arithmetic_resolver(dst_resolver)

  add_period_posixct_cpp(x, n, day_resolver, dst_resolver, size, unit)
}

add_period_posixlt <- function(x, n, ..., day_resolver, dst_resolver, unit) {
  x <- to_posixct(x)

  add_period_posixct(
    x = x,
    n = n,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    unit = unit
  )
}

add_period_local <- function(x, n, ..., day_resolver, unit) {
  check_dots_empty()

  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  validate_day_resolver(day_resolver)

  out <- add_period_local_cpp(x, n, day_resolver, size, unit)

  new_local_datetime(out)
}
