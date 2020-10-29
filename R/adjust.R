# ------------------------------------------------------------------------------
# day of month

#' @export
adjust_day_of_month <- function(x, value, ...) {
  UseMethod("adjust_day_of_month")
}

#' @export
adjust_day_of_month.Date <- function(x,
                                     value,
                                     ...,
                                     day_resolver = default_day_resolver()) {
  adjust_date(
    x = x,
    value = value,
    ...,
    day_resolver = day_resolver,
    adjuster = "day_of_month"
  )
}

#' @export
adjust_day_of_month.POSIXct <- function(x,
                                        value,
                                        ...,
                                        day_resolver = default_day_resolver(),
                                        dst_resolver = default_dst_resolver()) {
  adjust_posixct(
    x = x,
    value = value,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    adjuster = "day_of_month"
  )
}

#' @export
adjust_day_of_month.POSIXlt <- function(x,
                                        value,
                                        ...,
                                        day_resolver = default_day_resolver(),
                                        dst_resolver = default_dst_resolver()) {
  adjust_posixlt(
    x = x,
    value = value,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    adjuster = "day_of_month"
  )
}

#' @export
adjust_day_of_month.civil_local_datetime <- function(x,
                                                     value,
                                                     ...,
                                                     day_resolver = default_day_resolver()) {
  adjust_local(
    x = x,
    value = value,
    ...,
    day_resolver = day_resolver,
    adjuster = "day_of_month"
  )
}

# ------------------------------------------------------------------------------
# last day of month

#' @export
adjust_last_day_of_month <- function(x, ...) {
  UseMethod("adjust_last_day_of_month")
}

#' @export
adjust_last_day_of_month.Date <- function(x, ...) {
  adjust_date(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    day_resolver = default_day_resolver(),
    adjuster = "last_day_of_month"
  )
}

#' @export
adjust_last_day_of_month.POSIXct <- function(x,
                                             ...,
                                             dst_resolver = default_dst_resolver()) {
  adjust_posixct(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    day_resolver = default_day_resolver(),
    dst_resolver = dst_resolver,
    adjuster = "last_day_of_month"
  )
}

#' @export
adjust_last_day_of_month.POSIXlt <- function(x,
                                             ...,
                                             dst_resolver = default_dst_resolver()) {
  adjust_posixlt(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    day_resolver = default_day_resolver(),
    dst_resolver = dst_resolver,
    adjuster = "last_day_of_month"
  )
}

#' @export
adjust_last_day_of_month.civil_local_datetime <- function(x, ...) {
  adjust_local(
    x = x,
    value = VALUE_PLACEHOLDER,
    ...,
    day_resolver = default_day_resolver(),
    adjuster = "last_day_of_month"
  )
}

# ------------------------------------------------------------------------------

adjust_date <- function(x, value, ..., day_resolver, adjuster) {
  x_ct <- to_posixct(x)

  out <- adjust_posixct(
    x_ct,
    value,
    ...,
    day_resolver = day_resolver,
    dst_resolver = default_dst_resolver(),
    adjuster = adjuster
  )

  if (is_hms_adjuster(out)) {
    out
  } else {
    from_posixct(out, x)
  }
}

adjust_posixct <- function(x, value, ..., day_resolver, dst_resolver, adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  x <- posixct_standardize(x)

  validate_day_resolver(day_resolver)
  validate_dst_resolver(dst_resolver)

  adjust_posixct_cpp(x, value, day_resolver, dst_resolver, size, adjuster)
}

adjust_posixlt <- function(x, value, ..., day_resolver, dst_resolver, adjuster) {
  x <- to_posixct(x)

  adjust_posixct(
    x = x,
    value = value,
    ...,
    day_resolver = day_resolver,
    dst_resolver = dst_resolver,
    adjuster = adjuster
  )
}

adjust_local <- function(x, value, ..., day_resolver, adjuster) {
  check_dots_empty()

  value <- vec_cast(value, integer(), x_arg = "value")
  size <- vec_size_common(x = x, value = value)

  validate_day_resolver(day_resolver)

  out <- adjust_local_cpp(x, value, day_resolver, size, adjuster)

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
