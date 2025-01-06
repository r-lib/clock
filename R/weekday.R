#' Construct a weekday vector
#'
#' @description
#' A `weekday` is a simple type that represents a day of the week.
#'
#' The most interesting thing about the weekday type is that it implements
#' _circular arithmetic_, which makes determining the "next Monday" or
#' "previous Tuesday" from a sys-time or naive-time easy to compute.
#' See the examples.
#'
#' @inheritParams rlang::args_dots_empty
#'
#' @param code `[integer]`
#'
#'   Integer codes between `[1, 7]` representing days of the week. The
#'   interpretation of these values depends on `encoding`.
#'
#' @param encoding `[character(1)]`
#'
#'   One of:
#'
#'   - `"western"`: Encode weekdays assuming `1 == Sunday` and `7 == Saturday`.
#'
#'   - `"iso"`: Encode weekdays assuming `1 == Monday` and `7 == Sunday`. This
#'   is in line with the ISO standard.
#'
#' @return A weekday vector.
#'
#' @export
#' @examples
#' x <- as_naive_time(year_month_day(2019, 01, 05))
#'
#' # This is a Saturday!
#' as_weekday(x)
#'
#' # Adjust to the next Wednesday
#' wednesday <- weekday(clock_weekdays$wednesday)
#'
#' # This returns the number of days until the next Wednesday using
#' # circular arithmetic
#' # "Wednesday - Saturday = 4 days until next Wednesday"
#' wednesday - as_weekday(x)
#'
#' # Advance to the next Wednesday
#' x_next_wednesday <- x + (wednesday - as_weekday(x))
#'
#' as_weekday(x_next_wednesday)
#'
#' # What about the previous Tuesday?
#' tuesday <- weekday(clock_weekdays$tuesday)
#' x - (as_weekday(x) - tuesday)
#'
#' # What about the next Saturday?
#' # With an additional condition that if today is a Saturday,
#' # then advance to the next one.
#' saturday <- weekday(clock_weekdays$saturday)
#' x + 1L + (saturday - as_weekday(x + 1L))
#'
#' # You can supply an ISO coding for `code` as well, where 1 == Monday.
#' weekday(1:7, encoding = "western")
#' weekday(1:7, encoding = "iso")
weekday <- function(code = integer(), ..., encoding = "western") {
  check_dots_empty()

  # No other helpers retain names, so we don't here either
  code <- unname(code)
  code <- vec_cast(code, integer())

  oob <- (code > 7L | code < 1L) & (!is.na(code))
  if (any(oob)) {
    abort("`code` must be in range of [1, 7].")
  }

  encoding <- check_encoding(encoding)

  # Store as western encoding
  if (is_iso_encoding(encoding)) {
    code <- reencode_iso_to_western(code)
  }

  new_weekday(code)
}

new_weekday <- function(code = integer(), ..., class = NULL) {
  if (!is_integer(code)) {
    stop_input_type(code, "an integer vector")
  }

  new_vctr(
    code,
    ...,
    class = c(class, "clock_weekday"),
    inherit_base_type = FALSE
  )
}

# ------------------------------------------------------------------------------

#' Is `x` a weekday?
#'
#' This function determines if the input is a weekday object.
#'
#' @param x `[object]`
#'
#'   An object.
#'
#' @return `TRUE` if `x` inherits from `"clock_weekday"`, otherwise `FALSE`.
#'
#' @export
#' @examples
#' is_weekday(1)
#' is_weekday(weekday(1))
is_weekday <- function(x) {
  inherits(x, "clock_weekday")
}

check_weekday <- function(x, ..., arg = caller_arg(x), call = caller_env()) {
  check_inherits(x, what = "clock_weekday", arg = arg, call = call)
}

# ------------------------------------------------------------------------------

#' @export
format.clock_weekday <- function(x, ..., labels = "en", abbreviate = TRUE) {
  if (is_character(labels)) {
    labels <- clock_labels_lookup(labels)
  }
  check_clock_labels(labels)

  check_bool(abbreviate)

  if (abbreviate) {
    labels <- labels$weekday_abbrev
  } else {
    labels <- labels$weekday
  }

  out <- format_weekday_cpp(x, labels)

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
vec_ptype.clock_weekday <- function(x, ...) {
  clock_empty_weekday
}

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
vec_proxy_compare.clock_weekday <- function(x, ...) {
  cli::cli_abort(
    paste0(
      "Can't compare or order values of the {.cls clock_weekday} type, ",
      "as this type does not specify a {.str first} day of the week."
    )
  )
}

# ------------------------------------------------------------------------------

#' Convert to a weekday
#'
#' `as_weekday()` converts to a weekday type. This is normally useful for
#' converting to a weekday from a sys-time or naive-time. You can use this
#' function along with the _circular arithmetic_ that weekday implements to
#' easily get to the "next Monday" or "previous Sunday".
#'
#' @inheritParams rlang::args_dots_empty
#'
#' @param x `[object]`
#'
#'   An object to convert to a weekday. Usually a sys-time or naive-time.
#'
#' @return A weekday.
#'
#' @export
#' @examples
#' x <- as_naive_time(year_month_day(2019, 01, 05))
#'
#' # This is a Saturday!
#' as_weekday(x)
#'
#' # See the examples in `?weekday` for more usage.
as_weekday <- function(x, ...) {
  UseMethod("as_weekday")
}

#' @export
as_weekday.clock_weekday <- function(x, ...) {
  check_dots_empty0(...)
  x
}

#' @export
as_weekday.clock_calendar <- function(x, ...) {
  abort(
    c(
      "Can't extract the weekday from a calendar.",
      i = "Do you need to convert to a time point first?"
    )
  )
}

# ------------------------------------------------------------------------------

#' @export
as.character.clock_weekday <- function(x, ...) {
  format(x)
}

# ------------------------------------------------------------------------------

#' Extract underlying weekday codes
#'
#' `weekday_code()` extracts out the integer code for the weekday.
#'
#' @inheritParams weekday
#'
#' @param x `[weekday]`
#'
#'   A weekday vector.
#'
#' @return An integer vector of codes.
#'
#' @export
#' @examples
#' # Here we supply a western encoding to `weekday()`
#' x <- weekday(1:7)
#' x
#'
#' # We can extract out the codes using different encodings
#' weekday_code(x, encoding = "western")
#' weekday_code(x, encoding = "iso")
weekday_code <- function(x, ..., encoding = "western") {
  check_dots_empty0(...)
  check_weekday(x)

  encoding <- check_encoding(encoding)

  x <- unstructure(x)

  if (is_iso_encoding(encoding)) {
    x <- reencode_western_to_iso(x)
  }

  x
}

# ------------------------------------------------------------------------------

#' Convert a weekday to an ordered factor
#'
#' `weekday_factor()` converts a weekday object to an ordered factor. This
#' can be useful in combination with ggplot2, or for modeling.
#'
#' @inheritParams weekday_code
#' @inheritParams clock_locale
#'
#' @param abbreviate `[logical(1)]`
#'
#'   If `TRUE`, the abbreviated weekday names from `labels` will be used.
#'
#'   If `FALSE`, the full weekday names from `labels` will be used.
#'
#' @param encoding `[character(1)]`
#'
#'   One of:
#'
#'   - `"western"`: Encode the weekdays as an ordered factor with levels from
#'     Sunday -> Saturday.
#'
#'   - `"iso"`: Encode the weekdays as an ordered factor with levels from
#'     Monday -> Sunday.
#'
#' @return An ordered factor representing the weekdays.
#'
#' @export
#' @examples
#' x <- weekday(1:7)
#'
#' # Default to Sunday -> Saturday
#' weekday_factor(x)
#'
#' # ISO encoding is Monday -> Sunday
#' weekday_factor(x, encoding = "iso")
#'
#' # With full names
#' weekday_factor(x, abbreviate = FALSE)
#'
#' # Or a different language
#' weekday_factor(x, labels = "fr")
weekday_factor <- function(
  x,
  ...,
  labels = "en",
  abbreviate = TRUE,
  encoding = "western"
) {
  check_dots_empty0(...)
  check_weekday(x)

  if (is_character(labels)) {
    labels <- clock_labels_lookup(labels)
  }
  check_clock_labels(labels)

  check_bool(abbreviate)

  if (abbreviate) {
    labels <- labels$weekday_abbrev
  } else {
    labels <- labels$weekday
  }

  encoding <- check_encoding(encoding)
  x <- weekday_code(x, encoding = encoding)

  if (is_iso_encoding(encoding)) {
    labels <- c(labels[2:7], labels[1L])
  }

  x <- labels[x]

  factor(x, levels = labels, ordered = TRUE)
}

# ------------------------------------------------------------------------------

#' @rdname clock-arith
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
    "-" = stop_incompatible_op(
      op,
      x,
      y,
      details = "Can't subtract a weekday from a duration.",
      ...
    ),
    stop_incompatible_op(op, x, y, ...)
  )
}

#' @method vec_arith.clock_weekday numeric
#' @export
vec_arith.clock_weekday.numeric <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(
      x,
      duration_helper(y, PRECISION_DAY, retain_names = TRUE)
    ),
    "-" = add_duration(
      x,
      duration_helper(-y, PRECISION_DAY, retain_names = TRUE)
    ),
    stop_incompatible_op(op, x, y, ...)
  )
}

#' @method vec_arith.numeric clock_weekday
#' @export
vec_arith.numeric.clock_weekday <- function(op, x, y, ...) {
  switch(
    op,
    "+" = add_duration(
      y,
      duration_helper(x, PRECISION_DAY, retain_names = TRUE),
      swapped = TRUE
    ),
    "-" = stop_incompatible_op(
      op,
      x,
      y,
      details = "Can't subtract a weekday from a duration.",
      ...
    ),
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

  new_duration_from_fields(fields, PRECISION_DAY, names)
}

# ------------------------------------------------------------------------------

#' Arithmetic: weekday
#'
#' @description
#' These are weekday methods for the
#' [arithmetic generics][clock-arithmetic].
#'
#' - `add_days()`
#'
#' Also check out the examples on the [weekday()] page for more advanced
#' usage.
#'
#' @details
#' `x` and `n` are recycled against each other using
#' [tidyverse recycling rules][vctrs::vector_recycling_rules].
#'
#' @inheritParams clock-arithmetic
#'
#' @param x `[clock_weekday]`
#'
#'   A weekday vector.
#'
#' @return `x` after performing the arithmetic.
#'
#' @name weekday-arithmetic
#'
#' @examples
#' saturday <- weekday(clock_weekdays$saturday)
#' saturday
#'
#' add_days(saturday, 1)
#' add_days(saturday, 2)
NULL

#' @rdname weekday-arithmetic
#' @export
add_days.clock_weekday <- function(x, n, ...) {
  n <- duration_collect_n(n, PRECISION_DAY)

  args <- vec_recycle_common(x = x, n = n)
  x <- args$x
  n <- args$n

  names <- names_common(x, n)

  code <- weekday_add_days_cpp(x, n)
  names(code) <- names

  new_weekday(code)
}

# ------------------------------------------------------------------------------

#' @export
vec_math.clock_weekday <- function(.fn, .x, ...) {
  switch(
    .fn,
    is.nan = weekday_is_nan(.x),
    is.finite = weekday_is_finite(.x),
    is.infinite = weekday_is_infinite(.x),
    NextMethod()
  )
}

weekday_is_nan <- function(x) {
  vec_rep(FALSE, vec_size(x))
}

weekday_is_finite <- function(x) {
  !vec_detect_missing(x)
}

weekday_is_infinite <- function(x) {
  vec_rep(FALSE, vec_size(x))
}

# ------------------------------------------------------------------------------

check_encoding <- function(encoding, ..., error_call = caller_env()) {
  arg_match0(encoding, c("western", "iso"), error_call = error_call)
}

is_iso_encoding <- function(encoding) {
  identical(encoding, "iso")
}

reencode_iso_to_western <- function(code) {
  code <- code + 1L
  code[code == 8L] <- 1L
  code
}

reencode_western_to_iso <- function(code) {
  code <- code - 1L
  code[code == 0L] <- 7L
  code
}

# ------------------------------------------------------------------------------

clock_init_weekday_utils <- function(env) {
  assign("clock_empty_weekday", weekday(integer()), envir = env)

  invisible(NULL)
}
