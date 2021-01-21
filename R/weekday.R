#' @export
weekday <- function(day = integer()) {
  # No other helpers retain names, so we don't here either
  day <- unname(day)
  day <- vec_cast(day, integer(), x_arg = "day")

  oob <- (day > 7L | day < 0L) & (!is.na(day))
  if (any(oob)) {
    message <- paste0("`day` must be in range of [0, 7].")
    abort(message)
  }

  zeros <- day == 0L
  sevens <- day == 7L

  has_zeros <- any(zeros, na.rm = TRUE)
  has_sevens <- any(sevens, na.rm = TRUE)

  if (has_zeros && has_sevens) {
    abort("`day` can't contain both 0 and 7.")
  }

  # Store as C encoding
  if (has_sevens) {
    day[sevens] <- 0L
  }

  new_weekday(day)
}

#' @export
new_weekday <- function(day = integer(), ..., class = NULL) {
  if (!is_integer(day)) {
    abort("`day` must be an integer vector.")
  }

  new_vctr(
    day,
    ...,
    class = c(class, "clock_weekday"),
    inherit_base_type = FALSE
  )
}

#' @export
is_weekday <- function(x) {
  inherits(x, "clock_weekday")
}

# ------------------------------------------------------------------------------

#' @export
format.clock_weekday <- function(x, ..., locale = default_date_locale()) {
  if (!is_date_locale(locale)) {
    abort("`locale` must be a date locale object.")
  }

  day_ab <- locale$date_names$day_ab

  out <- format_weekday_cpp(x, day_ab)
  names(out) <- names(x)

  out
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype_full.clock_weekday <- function(x, ...) {
  "weekday"
}

#' @export
vec_ptype_abbr.clock_weekday <- function(x, ...) {
  "weekday"
}

# ------------------------------------------------------------------------------

#' @export
vec_ptype2.clock_weekday.clock_weekday <- function(x, y, ...) {
  x
}

#' @export
vec_cast.clock_weekday.clock_weekday <- function(x, to, ...) {
  x
}

# ------------------------------------------------------------------------------

#' @export
as_weekday <- function(x) {
  UseMethod("as_weekday")
}

#' @export
as_weekday.clock_weekday <- function(x) {
  x
}

#' @export
as_weekday.clock_time_point <- function(x) {
  x <- time_point_cast(x, "day")
  day <- weekday_from_time_point_cpp(x)
  names(day) <- clock_rcrd_names(x)
  new_weekday(day)
}

#' @export
as_weekday.clock_calendar <- function(x) {
  abort("Can't extract the weekday from a calendar. Convert to a time point first.")
}

# ------------------------------------------------------------------------------

#' @export
weekday_c_encoding <- function(x) {
  if (!is_weekday(x)) {
    abort("`x` must be a 'clock_weekday'.")
  }

  unstructure(x)
}

#' @export
weekday_iso_encoding <- function(x) {
  x <- weekday_c_encoding(x)
  zeros <- x == 0L
  x[zeros] <- 7L
  x
}

# ------------------------------------------------------------------------------

#' @method vec_arith clock_weekday
#' @export
vec_arith.clock_weekday <- function(op, x, y, ...) {
  UseMethod("vec_arith.clock_weekday", y)
}

#' @method vec_arith.clock_weekday MISSING
#' @export
vec_arith.clock_weekday.MISSING <- function(op, x, y, ...) {
  switch(
    op,
    "+" = x,
    stop_incompatible_op(op, x, y, ...)
  )
}

#' @method vec_arith.clock_weekday clock_weekday
#' @export
vec_arith.clock_weekday.clock_weekday <- function(op, x, y, ...) {
  switch(
    op,
    "-" = weekday_minus_weekday(op, x, y, ...),
    stop_incompatible_op(op, x, y, ...)
  )
}

#' @method vec_arith.clock_weekday clock_duration
#' @export
vec_arith.clock_weekday.clock_duration <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(x, y),
    "-" = add_duration(x, -y),
    stop_incompatible_op(op, x, y, ...)
  )
}

#' @method vec_arith.clock_duration clock_weekday
#' @export
vec_arith.clock_duration.clock_weekday <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(y, x, swapped = TRUE),
    "-" = stop_incompatible_op(op, x, y, details = "Can't subtract a weekday from a duration.", ...),
    stop_incompatible_op(op, x, y, ...)
  )
}

#' @method vec_arith.clock_weekday numeric
#' @export
vec_arith.clock_weekday.numeric <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(x, duration_helper(y, "day", retain_names = TRUE)),
    "-" = add_duration(x, duration_helper(-y, "day", retain_names = TRUE)),
    stop_incompatible_op(op, x, y, ...)
  )
}

#' @method vec_arith.numeric clock_weekday
#' @export
vec_arith.numeric.clock_weekday <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(y, duration_helper(x, "day", retain_names = TRUE), swapped = TRUE),
    "-" = stop_incompatible_op(op, x, y, details = "Can't subtract a weekday from a duration.", ...),
    stop_incompatible_op(op, x, y, ...)
  )
}

# Subtraction between weekdays tells days to next weekday
weekday_minus_weekday <- function(op, x, y, ...) {
  args <- vec_recycle_common(x = x, y = y)
  x <- args$x
  y <- args$y

  names <- names_common(x, y)

  fields <- weekday_minus_weekday_cpp(x, y)

  new_duration_from_fields(fields, "day", names = names)
}

# ------------------------------------------------------------------------------

#' @export
add_days.clock_weekday <- function(x, n, ...) {
  n <- duration_collect_n(n, "day")

  args <- vec_recycle_common(x = x, n = n)
  x <- args$x
  n <- args$n

  names <- names_common(x, n)

  day <- weekday_add_days_cpp(x, n)
  names(day) <- names

  new_weekday(day)
}

