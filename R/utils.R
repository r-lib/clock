to_posixct <- function(x) {
  if (is_POSIXct(x)) {
    posixct_standardize(x)
  } else if (is_POSIXlt(x)) {
    to_posixct_from_posixlt(x)
  } else {
    abort("Should either be POSIXct/POSIXlt.", .internal = TRUE)
  }
}

is_POSIXt <- function(x) {
  inherits(x, "POSIXt")
}
is_POSIXct <- function(x) {
  inherits(x, "POSIXct")
}
is_POSIXlt <- function(x) {
  inherits(x, "POSIXlt")
}

check_posixt <- function(x, ..., arg = caller_arg(x), call = caller_env()) {
  check_inherits(x, what = "POSIXt", arg = arg, call = call)
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

date_standardize <- function(x) {
  if (identical(typeof(x), "double")) {
    return(x)
  }

  # Convert somewhat rare integer Date to double.
  # Preserves names.
  storage.mode(x) <- "double"

  x
}

# ------------------------------------------------------------------------------

ones_along <- function(x, na_propagate = FALSE) {
  out <- rep(1L, vec_size(x))

  if (!na_propagate) {
    return(out)
  }

  na <- vec_detect_missing(x)
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

  na <- vec_detect_missing(x)
  if (any(na)) {
    out[na] <- NA_integer_
  }

  out
}

# ------------------------------------------------------------------------------

unstructure <- function(x) {
  set_attributes(x, NULL)
}

vec_unstructure <- function(x) {
  # Must unclass first because `names()` might not be the same length before
  # and after unclassing
  x <- unclass(x)
  out <- unstructure(x)
  names(out) <- names(x)
  out
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
    warning(
      paste0(
        "POSIXt input had a corrupt time zone attribute of `character(0)`. ",
        "Defaulting to the current zone by assuming the zone is `\"\"`."
      )
    )
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

stop_clock <- function(message, ..., call = caller_env(), class = character()) {
  rlang::abort(message, ..., call = call, class = c(class, "clock_error"))
}

stop_clock_unsupported <- function(
  x,
  ...,
  details = NULL,
  call = caller_env()
) {
  class <- class(x)[[1L]]
  message <- cli::format_inline(
    "Can't perform this operation on a {.cls {class}}."
  )
  message <- c(message, details)
  stop_clock(message, ..., call = call, class = "clock_error_unsupported")
}

stop_clock_unsupported_conversion <- function(
  x,
  to_arg,
  ...,
  call = caller_env()
) {
  class <- class(x)[[1L]]
  message <- cli::format_inline(
    "Can't convert {.cls {class}} to {.cls {to_arg}}."
  )
  stop_clock(
    message,
    ...,
    call = call,
    class = "clock_error_unsupported_conversion"
  )
}

# Thrown from C++
stop_clock_invalid_date <- function(i, call) {
  message <- c(
    cli::format_inline("Invalid date found at location {i}."),
    i = cli::format_inline(
      "Resolve invalid date issues by specifying the {.arg invalid} argument."
    )
  )
  stop_clock(message, call = call, class = "clock_error_invalid_date")
}

# Thrown from C++
stop_clock_nonexistent_time <- function(i, call) {
  message <- c(
    cli::format_inline(
      "Nonexistent time due to daylight saving time at location {i}."
    ),
    i = cli::format_inline(
      "Resolve nonexistent time issues by specifying the {.arg nonexistent} argument."
    )
  )
  stop_clock(message, call = call, class = "clock_error_nonexistent_time")
}

# Thrown from C++
stop_clock_ambiguous_time <- function(i, call) {
  message <- c(
    cli::format_inline(
      "Ambiguous time due to daylight saving time at location {i}."
    ),
    i = cli::format_inline(
      "Resolve ambiguous time issues by specifying the {.arg ambiguous} argument."
    )
  )
  stop_clock(message, call = call, class = "clock_error_ambiguous_time")
}

# ------------------------------------------------------------------------------

warn_clock <- function(message, class = character()) {
  rlang::warn(message, class = c(class, "clock_warning"))
}

# Thrown from C++
warn_clock_parse_failures <- function(n, first) {
  if (n == 0) {
    abort("Internal error: warning thrown with zero failures.")
  } else if (n == 1) {
    message <- paste0(
      "Failed to parse 1 string at location ",
      first,
      ". ",
      "Returning `NA` at that location."
    )
  } else {
    message <- paste0(
      "Failed to parse ",
      n,
      " strings, beginning at location ",
      first,
      ". ",
      "Returning `NA` at the locations where there were parse failures."
    )
  }

  warn_clock(message, "clock_warning_parse_failures")
}

# Thrown from C++
warn_clock_format_failures <- function(n, first) {
  if (n == 0) {
    abort("Internal error: warning thrown with zero failures.")
  } else if (n == 1) {
    message <- paste0(
      "Failed to format 1 string at location ",
      first,
      ". ",
      "Returning `NA` at that location."
    )
  } else {
    message <- paste0(
      "Failed to format ",
      n,
      " strings, beginning at location ",
      first,
      ". ",
      "Returning `NA` at the locations where there were format failures."
    )
  }

  warn_clock(message, "clock_warning_format_failures")
}

# ------------------------------------------------------------------------------

max_collect <- function(max, ..., error_call = caller_env()) {
  if (is_null(max)) {
    max <- getOption("max.print", default = 1000L)
  }

  check_number_whole(max, min = 0, call = error_call)
  max <- vec_cast(max, integer(), call = error_call)

  max
}

max_slice <- function(x, max) {
  if (max < vec_size(x)) {
    vec_slice(x, seq_len(max))
  } else {
    x
  }
}

clock_print <- function(x, max, ..., error_call = caller_env()) {
  max <- max_collect(max, error_call = error_call)
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
    "Omitted ",
    n_omitted,
    value,
    "\n",
    sep = ""
  )

  invisible(x)
}

# ------------------------------------------------------------------------------

df_list_propagate_missing <- function(x, ..., size = NULL) {
  check_dots_empty0(...)

  x <- new_data_frame(x, n = size)

  complete <- vec_detect_complete(x)
  if (all(complete)) {
    return(vec_unstructure(x))
  }

  incomplete <- !complete
  missing <- vec_detect_missing(x)

  aligned <- missing == incomplete
  if (all(aligned)) {
    # Already fully missing where incomplete
    return(vec_unstructure(x))
  }

  # Propagate missings
  x <- vec_assign(x, incomplete, NA)

  vec_unstructure(x)
}

# ------------------------------------------------------------------------------

check_inherits <- function(
  x,
  what,
  ...,
  allow_null = FALSE,
  arg = caller_arg(x),
  call = caller_env()
) {
  if (!missing(x)) {
    if (inherits(x, what)) {
      return(invisible(NULL))
    }
    if (allow_null && is_null(x)) {
      return(invisible(NULL))
    }
  }

  stop_input_type(
    x = x,
    what = cli::format_inline("a <{what}>"),
    arg = arg,
    call = call
  )
}

check_no_missing <- function(x, ..., arg = caller_arg(x), call = caller_env()) {
  if (!vec_any_missing(x)) {
    return(invisible(NULL))
  }

  loc <- vec_detect_missing(x)
  loc <- which(loc)

  message <- c(
    "{.arg {arg}} can't contain missing values.",
    i = "The following locations are missing: {loc}."
  )

  cli::cli_abort(message, call = call)
}

# ------------------------------------------------------------------------------

vec_drop_infinite <- function(x) {
  infinite <- is.infinite(x)

  if (any(infinite)) {
    x <- vec_slice(x, !infinite)
  }

  x
}

# ------------------------------------------------------------------------------

is_last <- function(x) {
  identical(x, "last")
}
