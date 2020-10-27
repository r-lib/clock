cast_posixct <- function(x) {
  if (is_Date(x)) {
    cast_posixct_from_date(x)
  } else if (is_POSIXct(x)) {
    cast_posixct_from_posixct(x)
  } else if (is_POSIXlt(x)) {
    cast_posixct_from_posixlt(x)
  } else {
    stop_civil_unsupported_class(x)
  }
}

cast_posixct_from_date <- function(x) {
  x <- unclass(x)
  x <- x * 86400
  new_datetime(x, "UTC")
}

cast_posixct_from_posixct <- function(x) {
  if (identical(typeof(x), "double")) {
    return(x)
  }

  # Convert rare integer POSIXct to double
  # Preserves names
  storage.mode(x) <- "double"

  x
}

cast_posixct_from_posixlt <- function(x) {
  as.POSIXct.POSIXlt(x)
}

# ------------------------------------------------------------------------------

# `x` is POSIXct
# If `to` is Date, then `x` has a UTC time zone
restore <- function(x, to) {
  if (is_Date(to)) {
    restore_to_date(x)
  } else if (is_POSIXct(to)) {
    restore_to_posixct(x)
  } else if (is_POSIXlt(to)) {
    # Push towards POSIXct
    restore_to_posixct(x)
  }
}

restore_to_date <- function(x) {
  out <- floor(unclass(x) / 86400)
  attr(out, "tzone") <- NULL
  structure(out, class = "Date")
}

restore_to_posixct <- function(x) {
  x
}

# ------------------------------------------------------------------------------

is_Date <- function(x) {
  inherits(x, "Date")
}

is_POSIXct <- function(x) {
  inherits(x, "POSIXct")
}

is_POSIXlt <- function(x) {
  inherits(x, "POSIXlt")
}

# ------------------------------------------------------------------------------

is_time_based_unit <- function(unit) {
  identical(unit, "hour") ||
    identical(unit, "minute") ||
    identical(unit, "second")
}

# ------------------------------------------------------------------------------

stop_civil <- function(message, class = character()) {
  rlang::abort(message, class = c(class, "civil_error"))
}

stop_civil_unsupported_class <- function(x) {
  message <- paste0("Unsupported class ", paste_class(x))
  stop_civil(message, class = "civil_error_unsupported_class")
}

paste_class <- function(x) {
  out <- paste0(class(x), collapse = "/")
  paste0("<", out, ">")
}
