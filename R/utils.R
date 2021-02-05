to_posixct <- function(x) {
  if (is_POSIXct(x)) {
    posixct_standardize(x)
  } else if (is_POSIXlt(x)) {
    to_posixct_from_posixlt(x)
  } else {
    abort("Internal error: Should either be POSIXct/POSIXlt.")
  }
}

is_POSIXct <- function(x) {
  inherits(x, "POSIXct")
}
is_POSIXlt <- function(x) {
  inherits(x, "POSIXlt")
}

posixct_standardize <- function(x) {
  if (identical(typeof(x), "double")) {
    return(x)
  }

  # Convert rare integer POSIXct to double
  # Preserves names
  storage.mode(x) <- "double"

  x
}
to_posixct_from_posixlt <- function(x) {
  as.POSIXct.POSIXlt(x)
}

# ------------------------------------------------------------------------------

ones_along <- function(x, na_propagate = FALSE) {
  out <- rep(1L, vec_size(x))

  if (!na_propagate) {
    return(out)
  }

  na <- vec_equal_na(x)
  if (any(na)) {
    out[na] <- NA_integer_
  }

  out
}
zeros_along <- function(x, na_propagate = FALSE) {
  out <- vector("integer", length = vec_size(x))

  if (!na_propagate) {
    return(out)
  }

  na <- vec_equal_na(x)
  if (any(na)) {
    out[na] <- NA_integer_
  }

  out
}

# ------------------------------------------------------------------------------

unstructure <- function(x) {
  set_attributes(x, NULL)
}

set_attributes <- function(x, attributes) {
  attributes(x) <- attributes
  x
}

# ------------------------------------------------------------------------------

posixt_tzone <- function(x) {
  attr(x, "tzone", exact = TRUE) %||% ""
}
posixt_set_tzone <- function(x, tzone) {
  attr(x, "tzone") <- tzone
  x
}

# ------------------------------------------------------------------------------

stop_clock <- function(message, class = character()) {
  rlang::abort(message, class = c(class, "clock_error"))
}

stop_clock_unsupported_conversion <- function(x, to_arg) {
  to_arg <- paste0("<", to_arg, ">")
  message <- paste0("Can't convert ", paste_class(x), " to ", to_arg, ".")
  stop_clock(message, "clock_error_unsupported_conversion")
}

stop_clock_unsupported_calendar_op <- function(op) {
  message <- paste0("This calendar doesn't support `", op, "()`.")
  stop_clock(message, "clock_error_unsupported_calendar_op")
}

stop_clock_unsupported_time_point_op <- function(op) {
  message <- paste0("Time points don't support `", op, "()`.")
  stop_clock(message, "clock_error_unsupported_time_point_op")
}

# Thrown from C++
stop_clock_invalid_date <- function(i) {
  message <- paste0(
    "Invalid date found at location ", i, ". ",
    "Resolve invalid date issues by specifying the `invalid` argument."
  )
  stop_clock(message, "clock_error_invalid_date")
}

# Thrown from C++
stop_clock_nonexistent_time <- function(i) {
  message <- paste0(
    "Nonexistent time due to daylight saving time at location ", i, ". ",
    "Resolve nonexistent time issues by specifying the `nonexistent` argument."
  )
  stop_clock(message, "clock_error_nonexistent_time")
}

# Thrown from C++
stop_clock_ambiguous_time <- function(i) {
  message <- paste0(
    "Ambiguous time due to daylight saving time at location ", i, ". ",
    "Resolve ambiguous time issues by specifying the `ambiguous` argument."
  )
  stop_clock(message, "clock_error_ambiguous_time")
}

paste_class <- function(x) {
  out <- paste0(class(x), collapse = "/")
  paste0("<", out, ">")
}

# ------------------------------------------------------------------------------

is_number <- function(x) {
  if (length(x) != 1L) {
    FALSE
  } else if (typeof(x) != "integer") {
    FALSE
  } else if (is.na(x)) {
    FALSE
  } else {
    TRUE
  }
}

is_last <- function(x) {
  identical(x, "last")
}

if_else <- function(condition, true, false, na = NULL) {
  vec_assert(condition, logical())

  # output size from `condition`
  size <- vec_size(condition)

  # output type from `true`/`false`/`na`
  ptype <- vec_ptype_common(true = true, false = false, na = na)

  args <- vec_recycle_common(true = true, false = false, na = na, .size = size)
  args <- vec_cast_common(!!!args, .to = ptype)

  out <- vec_init(ptype, size)

  loc_true <- condition
  loc_false <- !condition

  out <- vec_assign(out, loc_true, vec_slice(args$true, loc_true))
  out <- vec_assign(out, loc_false, vec_slice(args$false, loc_false))

  if (!is_null(na)) {
    loc_na <- vec_equal_na(condition)
    out <- vec_assign(out, loc_na, vec_slice(args$na, loc_na))
  }

  out
}
