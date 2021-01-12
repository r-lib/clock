new_calendar <- function(fields, precision, ..., names = NULL, class = NULL) {
  new_clock_rcrd(fields, precision = precision, ..., names = names, class = c(class, "clock_calendar"))
}

is_calendar <- function(x) {
  inherits(x, "clock_calendar")
}

# ------------------------------------------------------------------------------

# - Each subclass implements a `format()` method
# - Unlike vctrs, don't use `print(quote = FALSE)` since we want to match base R
#' @export
obj_print_data.clock_calendar <- function(x, ...) {
  if (vec_size(x) == 0L) {
    return(invisible(x))
  }

  out <- format(x)
  print(out)

  invisible(x)
}

# Align left to match pillar_shaft.Date
# @export - lazy in .onLoad()
pillar_shaft.clock_calendar <- function(x, ...) {
  out <- format(x)
  pillar::new_pillar_shaft_simple(out, align = "left")
}

# ------------------------------------------------------------------------------

#' @export
calendar_group <- function(x, precision, ..., n = 1L) {
  UseMethod("calendar_group")
}

#' @export
calendar_group.clock_calendar <- function(x, precision, ..., n = 1L) {
  check_dots_empty()

  if (!calendar_is_valid_precision(x, precision)) {
    abort(paste0("`precision` must be a valid precision for a '", calendar_name(x), "'."))
  }

  n <- vec_cast(n, integer(), x_arg = "n")
  if (!is_number(n)) {
    abort("`n` must be a single number.")
  }
  if (n <= 0L) {
    abort("`n` must be a positive number.")
  }

  x_precision <- calendar_precision(x)

  if (precision_value(x_precision) < precision_value(precision)) {
    abort("Can't floor to a precision that is more precise than `x`.")
  }

  x <- calendar_cast(x, precision)

  component <- calendar_precision_to_component(x, precision)
  group_component_fn <- calendar_component_grouper(x, component)

  if (n != 1L) {
    value <- calendar_get_component(x, component)
    value <- group_component_fn(value, n)
    x <- calendar_set_component(x, value, component)
  }

  x
}

# Internal generic
calendar_component_grouper <- function(x, component) {
  UseMethod("calendar_component_grouper")
}

#' @export
calendar_component_grouper.clock_calendar <- function(x, component) {
  stop_clock_unsupported_calendar_op("calendar_component_grouper")
}

group_component0 <- function(x, n) {
  (x %/% n) * n
}
group_component1 <- function(x, n) {
  ((x - 1L) %/% n) * n + 1L
}

# ------------------------------------------------------------------------------

# Note:
# Calendar casting is the one place where we assume a "default" value when
# casting to a more precise precision. The default is 1 for months or days,
# and 0 for time based components. For year-month-weekday, we choose the
# 1st weekday in that month as the default value.

#' @export
calendar_cast <- function(x, precision) {
  UseMethod("calendar_cast")
}

#' @export
calendar_cast.clock_calendar <- function(x, precision) {
  stop_clock_unsupported_calendar_op("calendar_cast")
}

calendar_time_upcast <- function(fields, x_precision_value, to_precision_value, zeros) {
  if (to_precision_value >= PRECISION_HOUR && x_precision_value < PRECISION_HOUR) {
    fields[["hour"]] <- zeros
  }
  if (to_precision_value >= PRECISION_MINUTE && x_precision_value < PRECISION_MINUTE) {
    fields[["minute"]] <- zeros
  }
  if (to_precision_value >= PRECISION_SECOND && x_precision_value < PRECISION_SECOND) {
    fields[["second"]] <- zeros
  }
  if (to_precision_value == PRECISION_MILLISECOND) {
    fields[["subsecond"]] <- zeros
  }
  if (to_precision_value == PRECISION_MICROSECOND) {
    if (x_precision_value == PRECISION_MILLISECOND) {
      fields[["subsecond"]] <- fields[["subsecond"]] * 1e3L
    } else {
      fields[["subsecond"]] <- zeros
    }
  }
  if (to_precision_value == PRECISION_NANOSECOND) {
    if (x_precision_value == PRECISION_MILLISECOND) {
      fields[["subsecond"]] <- fields[["subsecond"]] * 1e6L
    } else if (x_precision_value == PRECISION_MICROSECOND) {
      fields[["subsecond"]] <- fields[["subsecond"]] * 1e3L
    } else {
      fields[["subsecond"]] <- zeros
    }
  }
  fields
}

calendar_time_downcast <- function(out, fields, x_precision_value, to_precision_value) {
  if (to_precision_value >= PRECISION_HOUR) {
    out[["hour"]] <- fields[["hour"]]
  }
  if (to_precision_value >= PRECISION_MINUTE) {
    out[["minute"]] <- fields[["minute"]]
  }
  if (to_precision_value >= PRECISION_SECOND) {
    out[["second"]] <- fields[["second"]]
  }
  if (to_precision_value == PRECISION_MILLISECOND) {
    if (x_precision_value == PRECISION_MILLISECOND) {
      out[["subsecond"]] <- fields[["subsecond"]]
    } else if (x_precision_value == PRECISION_MICROSECOND) {
      out[["subsecond"]] <- fields[["subsecond"]] %/% 1e3L
    } else if (x_precision_value == PRECISION_NANOSECOND) {
      out[["subsecond"]] <- fields[["subsecond"]] %/% 1e6L
    }
  }
  if (to_precision_value == PRECISION_MICROSECOND) {
    if (x_precision_value == PRECISION_MICROSECOND) {
      out[["subsecond"]] <- fields[["subsecond"]]
    } else if (x_precision_value == PRECISION_NANOSECOND) {
      out[["subsecond"]] <- fields[["subsecond"]] %/% 1e3L
    }
  }
  if (to_precision_value == PRECISION_NANOSECOND) {
    abort("Internal error: Should have early returned.")
  }
  out
}

na_to_ones <- function(na, any_na) {
  out <- rep(1L, length(na))
  if (any_na) {
    out[na] <- NA_integer_
  }
  out
}

na_to_zeros <- function(na, any_na) {
  out <- vector("integer", length(na))
  if (any_na) {
    out[na] <- NA_integer_
  }
  out
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_get_component <- function(x, component) {
  if (!calendar_is_valid_component(x, component)) {
    abort("`component` must be a valid component for a '", calendar_name(x), "'.")
  }
  UseMethod("calendar_get_component")
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_set_component <- function(x, value, component, ...) {
  if (!calendar_is_valid_component(x, component)) {
    abort("`component` must be a valid component for a '", calendar_name(x), "'.")
  }
  UseMethod("calendar_set_component")
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_check_component_range <- function(x, value, component, value_arg) {
  if (!calendar_is_valid_component(x, component)) {
    abort("`component` must be a valid component for a '", calendar_name(x), "'.")
  }
  UseMethod("calendar_check_component_range")
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_name <- function(x) {
  UseMethod("calendar_name")
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_component_to_precision <- function(x, component) {
  UseMethod("calendar_component_to_precision")
}

# Internal generic
calendar_component_to_field <- function(x, component) {
  UseMethod("calendar_component_to_field")
}

# Internal generic
calendar_precision_to_component <- function(x, precision) {
  UseMethod("calendar_precision_to_component")
}

# Internal generic
calendar_precision_to_field <- function(x, precision) {
  UseMethod("calendar_precision_to_field")
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_is_valid_precision <- function(x, precision) {
  UseMethod("calendar_is_valid_precision")
}

calendar_standard_precisions <- function() {
  c("day", "hour", "minute", "second", "millisecond", "microsecond", "nanosecond")
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_is_valid_component <- function(x, component) {
  UseMethod("calendar_is_valid_component")
}

calendar_standard_components <- function() {
  c("hour", "minute", "second", "millisecond", "microsecond", "nanosecond")
}

# ------------------------------------------------------------------------------

#' @export
get_precision.clock_calendar <- function(x) {
  calendar_precision(x)
}

# ------------------------------------------------------------------------------

calendar_precision <- function(x) {
  attr(x, "precision", exact = TRUE)
}

calendar_require_minimum_precision <- function(x, precision, fn) {
  if (!calendar_has_minimum_precision(x, precision)) {
    msg <- paste0("`", fn, "()` requires a minimum precision of '", precision, "'.")
    abort(msg)
  }
  invisible(x)
}

calendar_has_minimum_precision <- function(x, precision) {
  x_precision <- calendar_precision(x)
  precision_value(x_precision) >= precision_value(precision)
}

calendar_require_precision <- function(x, precision, fn) {
  if (!calendar_has_precision(x, precision)) {
    msg <- paste0("`", fn, "()` requires a precision of '", precision, "'.")
    abort(msg)
  }
  invisible(x)
}

calendar_require_any_of_precisions <- function(x, precisions, fn) {
  results <- vapply(precisions, calendar_has_precision, FUN.VALUE = logical(1), x = x)
  if (!any(results)) {
    msg <- paste0("`", fn, "()` does not support a precision of '", calendar_precision(x), "'.")
    abort(msg)
  }
  invisible(x)
}

calendar_has_precision <- function(x, precision) {
  calendar_precision(x) == precision
}

# ------------------------------------------------------------------------------

calendar_require_all_valid <- function(x, fn) {
  if (invalid_any(x)) {
    message <- paste0(
      "`", fn, "()` requires that all calendar dates are valid. ",
      "Resolve invalid dates by calling `invalid_resolve()`."
    )
    abort(message)
  }

  invisible(x)
}

# ------------------------------------------------------------------------------

calendar_fields <- function(x) {
  fields <- unclass(x)
  field_names <- names(fields)
  fields <- unstructure(fields)
  names(fields) <- field_names
  fields
}

# ------------------------------------------------------------------------------

calendar_ptype_full <- function(x, class) {
  count <- invalid_count(x)
  precision <- calendar_precision(x)
  paste0(class, "<", precision, ">[invalid=", count, "]")
}

calendar_ptype_abbr <- function(x, abbr) {
  count <- invalid_count(x)
  precision <- calendar_precision(x)
  precision <- precision_abbr(precision)
  paste0(abbr, "<", precision, ">[i=", count, "]")
}

# ------------------------------------------------------------------------------

arith_calendar_and_missing <- function(op, x, y, ...) {
  switch (
    op,
    "+" = x,
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_calendar_and_duration <- function(op, x, y, ...) {
  switch (
    op,
    "+" = add_duration(x, y),
    "-" = add_duration(x, -y),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_duration_and_calendar <- function(op, x, y, ...) {
  switch (
    op,
    "+" = add_duration(y, x, swapped = TRUE),
    "-" = stop_incompatible_op(op, x, y, details = "Can't subtract a calendar from a duration.", ...),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_calendar_and_numeric <- function(op, x, y, ...) {
  switch (
    op,
    "+" = add_duration(x, duration_helper(y, calendar_precision(x), retain_names = TRUE)),
    "-" = add_duration(x, duration_helper(-y, calendar_precision(x), retain_names = TRUE)),
    stop_incompatible_op(op, x, y, ...)
  )
}

arith_numeric_and_calendar <- function(op, x, y, ...) {
  switch (
    op,
    "+" = add_duration(y, duration_helper(x, calendar_precision(y), retain_names = TRUE), swapped = TRUE),
    "-" = stop_incompatible_op(op, x, y, details = "Can't subtract a calendar from a duration.", ...),
    stop_incompatible_op(op, x, y, ...)
  )
}


