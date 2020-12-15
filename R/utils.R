to_posixct <- function(x) {
  if (is_Date(x)) {
    to_posixct_from_date(x)
  } else if (is_POSIXct(x)) {
    to_posixct_from_posixct(x)
  } else if (is_POSIXlt(x)) {
    to_posixct_from_posixlt(x)
  } else {
    stop_clock_unsupported_class(x)
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

ones_along <- function(x) {
  rep(1L, times = vec_size(x))
}
zeros_along <- function(x) {
  vector("integer", length = vec_size(x))
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

is_zoned_or_base <- function(x) {
  is_zoned_time_point(x) || is_Date(x) || is_POSIXct(x) || is_POSIXlt(x)
}

# ------------------------------------------------------------------------------

date_to_days <- function(x) {
  days <- unstructure(x)
  days <- as.integer(x)
  names(days) <- names(x)
  days
}

days_to_date <- function(x, names = NULL) {
  date <- unstructure(x)
  date <- as.double(date)
  names(date) <- names
  new_date(date)
}

# ------------------------------------------------------------------------------

restrict_civil_supported <- function(x) {
  if (is_calendar(x) || is_naive_time_point(x) || is_zoned_or_base(x)) {
    invisible(x)
  } else {
    stop_clock_unsupported_class(x)
  }
}

restrict_zoned_or_base <- function(x) {
  if (is_zoned_or_base(x)) {
    invisible(x)
  } else {
    stop_clock_unsupported_class(x)
  }
}

restrict_zoned <- function(x) {
  if (is_zoned(x)) {
    invisible(x)
  } else {
    stop_clock_unsupported_class(x)
  }
}

restrict_naive <- function(x) {
  if (is_naive(x)) {
    invisible(x)
  } else {
    stop_clock_unsupported_class(x)
  }
}

# ------------------------------------------------------------------------------

# Purposefully promote to double as this either avoids possible integer overflow
# or converts integer fields to double to be used in a POSIXct

seconds_in_day <- function() {
  86400
}
seconds_in_hour <- function() {
  3600
}
seconds_in_minute <- function() {
  60
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
  attr(x, "tzone", exact = TRUE) %||% ""
}

# ------------------------------------------------------------------------------

stop_clock <- function(message, class = character()) {
  rlang::abort(message, class = c(class, "clock_error"))
}

stop_clock_unsupported_class <- function(x) {
  message <- paste0("Unsupported class ", paste_class(x))
  stop_clock(message, class = "clock_error_unsupported_class")
}

stop_clock_unsupported_conversion <- function(x, to_arg) {
  to_arg <- paste0("<", to_arg, ">")
  message <- paste0("Can't convert ", paste_class(x), " to ", to_arg, ".")
  stop_clock(message, "clock_error_unsupported_conversion")
}

paste_class <- function(x) {
  out <- paste0(class(x), collapse = "/")
  paste0("<", out, ">")
}

# ------------------------------------------------------------------------------

has_field <- function(x, field) {
  field %in% fields(x)
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
