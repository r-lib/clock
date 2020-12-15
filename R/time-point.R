# Note: `class` is required as we consider this an abstract type that should
# never exist on its own in the wild.

new_time_point <- function(calendar, precision, ..., fields = NULL, names = NULL, class) {
  if (!is_calendar(calendar)) {
    abort("`calendar` must be a calendar.")
  }
  if (!identical(get_precision(calendar), "day")) {
    abort("`calendar` must have a precision of 'day'.")
  }

  if (!is_string(precision)) {
    abort("`precision` must be a string.")
  }

  size <- vec_size(calendar)
  validate_names(names, size)

  calendar <- list(calendar = calendar)
  fields <- c(calendar, fields)

  new_rcrd(
    fields = fields,
    precision = precision,
    ...,
    `clock_time_point:::names` = names,
    class = c(class, "clock_time_point")
  )
}

#' @export
is_time_point <- function(x) {
  inherits(x, "clock_time_point")
}

# ------------------------------------------------------------------------------

field_calendar <- function(x) {
  field(x, "calendar")
}
field_seconds_of_day <- function(x) {
  field(x, "seconds_of_day")
}
field_nanoseconds_of_second <- function(x) {
  field(x, "nanoseconds_of_second")
}

# ------------------------------------------------------------------------------

get_precision_abbr <- function(x) {
  precision <- get_precision(x)

  if (identical(precision, "second")) {
    "sec"
  } else if (identical(precision, "millisecond")) {
    "milli"
  } else if (identical(precision, "microsecond")) {
    "micro"
  } else if (identical(precision, "nanosecond")) {
    "nano"
  } else {
    abort("Internal error: Unknown precision to get an abbreviation for.")
  }
}

# ------------------------------------------------------------------------------

#' @export
names.clock_time_point <- function(x) {
  attr(x, "clock_time_point:::names", exact = TRUE)
}

#' @export
`names<-.clock_time_point` <- function(x, value) {
  attrib <- attributes(x)

  # Remove names
  if (is.null(value)) {
    attrib[["clock_time_point:::names"]] <- NULL
    attributes(x) <- attrib

    return(x)
  }

  size <- vec_size(x)
  value <- as_names(value, size)

  attrib[["clock_time_point:::names"]] <- value
  attributes(x) <- attrib

  x
}

as_names <- function(x, size) {
  x <- unstructure(x)

  if (!is_character(x)) {
    x <- as.character(x)
  }

  validate_names(x, size)

  x
}

validate_names <- function(names, size) {
  if (is_null(names)) {
    return(invisible(names))
  }

  if (!is_character(names)) {
    abort("Names must be a character vector.")
  }

  if (length(names) != size) {
    abort(paste0("Names must have length ", size, " not ", length(names), "."))
  }

  if (any(is.na(names))) {
    abort("Names cannot be `NA`.")
  }

  invisible(names)
}

# ------------------------------------------------------------------------------

# - `[.vctrs_rcrd` accidentally allows subsetting fields through `...`
#   https://github.com/r-lib/vctrs/issues/1295

#' @export
`[.clock_time_point` <- function(x, i) {
  i <- maybe_missing(i, default = TRUE)
  vec_slice(x, i)
}

# - `[[.vctrs_rcrd` doesn't drop names because names aren't supported for rcrds
# - `[[.vctrs_rcrd` allows selections of size >1
#   https://github.com/r-lib/vctrs/issues/1294

#' @export
`[[.clock_time_point` <- function(x, i) {
  size <- vec_size(x)
  names <- names(x)

  i <- vec_as_location2(i, n = size, names = names, arg = "i")

  # Unname - `[[` never returns input with names
  x <- unname(x)

  vec_slice(x, i)
}

# ------------------------------------------------------------------------------

# - Each subclass implements a `format()` method
# - Unlike vctrs, don't use `print(quote = FALSE)` since we want to match base R
# - We also pass a hidden option, `print_zone_name = FALSE` to not print the
#   zone name when printing and when in tibbles. We generally do want this
#   to print when `format()` is called on its own, as this makes a reproducible
#   time point string.

#' @export
obj_print_data.clock_time_point <- function(x, ...) {
  if (vec_size(x) == 0L) {
    return(invisible(x))
  }

  out <- format(x, print_zone_name = FALSE)

  print(out)
  invisible(x)
}

# @export - lazy in .onLoad()
pillar_shaft.clock_time_point <- function(x, ...) {
  out <- format(x, print_zone_name = FALSE)
  pillar::new_pillar_shaft_simple(out, align = "left")
}

# ------------------------------------------------------------------------------

proxy_time_point <- function(x) {
  out <- unclass(x)
  out[["clock_time_point:::names"]] <- names(x)
  out <- new_data_frame(out)
  out
}

proxy_equal_time_point <- function(x) {
  out <- unclass(x)
  out <- new_data_frame(out)
  out
}

restore_time_point_fields <- function(x) {
  x[["clock_time_point:::names"]] <- NULL
  names <- names(x)
  x <- unstructure(x)
  names(x) <- names
  x
}

restore_time_point_names <- function(x) {
  names <- x[["clock_time_point:::names"]]
  names <- repair_na_names(names)
  names
}

# Patch required to repair `NA` names generated by `vec_slice()`
# and `chr_slice()`. Same idea as the C level `repair_na_names()` in
# https://github.com/r-lib/vctrs/blob/7f5134f3b0ae747407321c57451f2eee60722ce3/src/slice.c#L265
# Should go away if we get names support in a vctrs rcrd
repair_na_names <- function(names) {
  na <- is.na(names)

  if (any(na)) {
    names[na] <- ""
  }

  names
}

