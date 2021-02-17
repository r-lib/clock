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
  tzone <- attr(x, "tzone", exact = TRUE)
  posixt_tzone_standardize(tzone)
}
posixt_set_tzone <- function(x, tzone) {
  attr(x, "tzone") <- tzone
  x
}

# Standardize an R time zone attribute
#
# This is slightly different from the lubridate version. For POSIXlt objects,
# the time zone attribute might be a character vector of length 3. If the first
# element is `""` (which happens on a Mac with `as.POSIXlt(Sys.time())`), then
# lubridate will look to the second element and will use that as the time zone.
# I think this is incorrect, because those are always time zone abbreviations,
# and will fail to load because they aren't true time zone names. I think that
# is the reason Vitalie opened the issue noted below, and the reason for the
# time zone map in lubridate. This function works more like
# `lubridate:::tz.POSIXt()` which just takes the first element of the tzone
# attribute. https://github.com/google/cctz/issues/46
# https://github.com/tidyverse/lubridate/blob
posixt_tzone_standardize <- function(tzone) {
  if (is_null(tzone)) {
    # Like `Sys.time()`
    return("")
  }

  if (!is_character(tzone)) {
    abort("A POSIXt time zone should either be a character vector or `NULL`.")
  }

  n <- length(tzone)

  if (n == 0L) {
    warning(paste0(
      "POSIXt input had a corrupt time zone attribute of `character(0)`. ",
      "Defaulting to the current zone by assuming the zone is `\"\"`."
    ))
    return("")
  }

  if (n == 1L) {
    return(tzone)
  }

  # Otherwise `n > 1`, likely `n == 3` for a POSIXt with time zone
  # abbreviations. The first element is either a full time zone name, or `""`,
  # so we use that
  tzone <- tzone[[1]]

  tzone
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

stop_clock_unsupported_zoned_time_op <- function(op) {
  message <- paste0("Zoned-times don't support `", op, "()`.")
  stop_clock(message, "clock_error_unsupported_zoned_time_op")
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

warn_clock <- function(message, class = character()) {
  rlang::warn(message, class = c(class, "clock_warning"))
}

# Thrown from C++
warn_clock_parse_failures <- function(n, first) {
  if (n == 0) {
    abort("Internal error: warning thrown with zero parse failures.")
  } else if (n == 1) {
    message <- paste0(
      "Failed to parse 1 string at location ", first, ". ",
      "Returning `NA` at that location."
    )
  } else {
    message <- paste0(
      "Failed to parse ", n, " strings, beginning at location ", first, ". ",
      "Returning `NA` at the locations where there were parse failures."
    )
  }

  warn_clock(message, "clock_warning_parse_failures")
}

# ------------------------------------------------------------------------------

max_collect <- function(max) {
  if (is_null(max)) {
    max <- getOption("max.print", default = 1000L)
  }

  max <- vec_cast(max, integer(), x_arg = "max")

  if (!is_number(max)) {
    abort("`max` must be a single number, or `NULL`.")
  }
  if (max <= 0L) {
    abort("`max` must be a positive number.")
  }

  max
}

max_slice <- function(x, max) {
  if (max < vec_size(x)) {
    vec_slice(x, seq_len(max))
  } else {
    x
  }
}

clock_print <- function(x, max) {
  max <- max_collect(max)
  obj_print(x, max = max)
  invisible(x)
}

clock_print_footer <- function(x, max) {
  size <- vec_size(x)

  if (max >= size) {
    return(invisible(x))
  }

  n_omitted <- size - max

  if (n_omitted == 1L) {
    value <- " value."
  } else {
    value <- " values."
  }

  cat(
    "Reached `max` or `getOption('max.print')`. ",
    "Omitted ", n_omitted, value,
    "\n",
    sep = ""
  )

  invisible(x)
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
