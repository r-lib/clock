to_posixct <- function(x) {
  if (is_Date(x)) {
    to_posixct_from_date(x)
  } else if (is_POSIXct(x)) {
    to_posixct_from_posixct(x)
  } else if (is_POSIXlt(x)) {
    to_posixct_from_posixlt(x)
  } else {
    stop_civil_unsupported_class(x)
  }
}

to_posixct_from_date <- function(x) {
  x <- unclass(x)
  x <- x * 86400
  new_datetime(x, "UTC")
}

to_posixct_from_posixct <- function(x) {
  posixct_standardize(x)
}

to_posixct_from_posixlt <- function(x) {
  as.POSIXct.POSIXlt(x)
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

# ------------------------------------------------------------------------------

# `x` is POSIXct
# If `to` is Date, then `x` has a UTC time zone
from_posixct <- function(x, to) {
  if (is_Date(to)) {
    from_posixct_to_date(x)
  } else if (is_POSIXct(to)) {
    from_posixct_to_posixct(x)
  } else if (is_POSIXlt(to)) {
    # Push towards POSIXct
    from_posixct_to_posixct(x)
  }
}

from_posixct_to_date <- function(x) {
  out <- floor(unclass(x) / 86400)
  attr(out, "tzone") <- NULL
  structure(out, class = "Date")
}

from_posixct_to_posixct <- function(x) {
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

cat_line <- function(...) {
  cat(paste0(..., "\n", collapse = ""))
}

unstructure <- function(x) {
  attributes(x) <- NULL
  x
}

get_tzone <- function(x) {
  attr(x, "tzone", exact = TRUE)
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
