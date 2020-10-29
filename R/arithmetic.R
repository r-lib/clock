#' @export
add_years <- function(x, n, ...) {
  UseMethod("add_years")
}

#' @export
add_years.Date <- function(x,
                           n,
                           ...,
                           day_resolver = default_day_resolver()) {
  x_ct <- to_posixct(x)
  out <- add_years(x_ct, n, day_resolver = day_resolver)
  from_posixct(out, x)
}

#' @export
add_years.POSIXct <- function(x,
                              n,
                              ...,
                              day_resolver = default_day_resolver(),
                              dst_resolver = default_dst_arithmetic_resolver()) {
  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  validate_day_resolver(day_resolver)
  validate_dst_arithmetic_resolver(dst_resolver)

  add_years_posixct_cpp(x, n, day_resolver, dst_resolver, size)
}

#' @export
add_years.POSIXlt <- function(x,
                              n,
                              ...,
                              day_resolver = default_day_resolver(),
                              dst_resolver = default_dst_arithmetic_resolver()) {
  add_years(to_posixct(x), n, day_resolver = day_resolver, dst_resolver = dst_resolver)
}

#' @export
add_years.civil_local_datetime <- function(x,
                                           n,
                                           ...,
                                           day_resolver = default_day_resolver()) {
  n <- vec_cast(n, integer(), x_arg = "n")
  size <- vec_size_common(x = x, n = n)

  validate_day_resolver(day_resolver)

  out <- add_years_local_cpp(x, n, day_resolver, size)

  new_local_datetime(out)
}
