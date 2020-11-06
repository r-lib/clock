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

to_local_datetime <- function(x) {
  if (is_local_date(x)) {
    to_local_datetime_from_local_date(x)
  } else if (is_local_datetime(x)) {
    to_local_datetime_from_local_datetime(x)
  } else {
    abort("Internal error: Unexpected class.")
  }
}

to_local_datetime_from_local_date <- function(x) {
  zero <- rep.int(0L, vec_size(x))

  out <- new_local_datetime(
    year = field(x, "year"),
    month = field(x, "month"),
    day = field(x, "day"),
    hour = zero,
    minute = zero,
    second = zero,
    zone = local_zone(x),
    names = names(x)
  )

  out
}

to_local_datetime_from_local_datetime <- function(x) {
  x
}

# ------------------------------------------------------------------------------

from_local_datetime <- function(x, to) {
  if (is_local_date(to)) {
    from_local_datetime_to_local_date(x)
  } else if (is_local_datetime(to)) {
    from_local_datetime_to_local_datetime(x)
  } else {
    abort("Internal error: Unexpected `to` class.")
  }
}

from_local_datetime_to_local_date <- function(x) {
  out <- new_local_date(
    year = field(x, "year"),
    month = field(x, "month"),
    day = field(x, "day"),
    zone = local_zone(x),
    names = names(x)
  )

  out
}

from_local_datetime_to_local_datetime <- function(x) {
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

restrict_civil_supported <- function(x) {
  if (is_Date(x) || is_POSIXct(x) || is_POSIXlt(x) || is_local(x)) {
    invisible(x)
  } else {
    stop_civil_unsupported_class(x)
  }
}

restrict_local <- function(x) {
  if (is_local(x)) {
    invisible(x)
  } else {
    stop_civil_unsupported_class(x)
  }
}

# ------------------------------------------------------------------------------

glue <- function(...) {
  do.call(paste0, vctrs::vec_recycle_common(...))
}

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
