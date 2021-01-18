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

# Note: Cannot cast between calendar precisions. Casting to a more precise
# precision is undefined because we consider things like year-month to be
# a range of days over the whole month, and it would be impossible to map
# that to just one day.

ptype2_calendar_and_calendar <- function(x, y, ...) {
  if (calendar_precision(x) == calendar_precision(y)) {
    x
  } else {
    stop_incompatible_type(x, y, ..., details = "Can't combine calendars with different precisions.")
  }
}

cast_calendar_to_calendar <- function(x, to, ...) {
  if (calendar_precision(x) == calendar_precision(to)) {
    x
  } else {
    stop_incompatible_cast(x, to, ..., details = "Can't cast between calendars with different precisions.")
  }
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

  x <- calendar_narrow(x, precision)

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

#' @export
calendar_narrow <- function(x, precision) {
  if (!calendar_is_valid_precision(x, precision)) {
    abort("`precision` must be a valid precision for a '", calendar_name(x), "'.")
  }

  x_precision <- calendar_precision(x)
  if (precision_value(x_precision) < precision_value(precision)) {
    abort("Can't narrow to a precision that is wider than `x`.")
  }

  UseMethod("calendar_narrow")
}

#' @export
calendar_narrow.clock_calendar <- function(x, precision) {
  stop_clock_unsupported_calendar_op("calendar_narrow")
}

calendar_narrow_time <- function(out_fields,
                                 out_precision_value,
                                 x_fields,
                                 x_precision_value) {
  if (out_precision_value >= PRECISION_HOUR) {
    out_fields[["hour"]] <- x_fields[["hour"]]
  }
  if (out_precision_value >= PRECISION_MINUTE) {
    out_fields[["minute"]] <- x_fields[["minute"]]
  }
  if (out_precision_value >= PRECISION_SECOND) {
    out_fields[["second"]] <- x_fields[["second"]]
  }
  if (out_precision_value > PRECISION_SECOND) {
    factor <- precision_subsecond_factor(out_precision_value, x_precision_value)
    out_fields[["subsecond"]] <- x_fields[["subsecond"]] %/% factor
  }

  out_fields
}

precision_subsecond_factor <- function(narrow_precision_value, wide_precision_value) {
  if (narrow_precision_value == PRECISION_MILLISECOND) {
    if (wide_precision_value == PRECISION_MILLISECOND) {
      1L
    } else if (wide_precision_value == PRECISION_MICROSECOND) {
      1e3L
    } else if (wide_precision_value == PRECISION_NANOSECOND) {
      1e6L
    } else {
      abort("Internal error: Invalid precision combination.")
    }
  } else if (narrow_precision_value == PRECISION_MICROSECOND) {
    if (wide_precision_value == PRECISION_MICROSECOND) {
      1L
    } else if (wide_precision_value == PRECISION_NANOSECOND) {
      1e3L
    } else {
      abort("Internal error: Invalid precision combination.")
    }
  } else if (narrow_precision_value == PRECISION_NANOSECOND) {
    if (wide_precision_value == PRECISION_NANOSECOND) {
      1L
    } else {
      abort("Internal error: Invalid precision combination.")
    }
  } else {
    abort("Internal error: Invalid precision combination.")
  }
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_get_component <- function(x, component) {
  if (!calendar_is_valid_component(x, component)) {
    abort(paste0("`component` must be a valid component for a '", calendar_name(x), "'."))
  }
  UseMethod("calendar_get_component")
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_set_component <- function(x, value, component, ...) {
  if (!calendar_is_valid_component(x, component)) {
    abort(paste0("`component` must be a valid component for a '", calendar_name(x), "'."))
  }
  UseMethod("calendar_set_component")
}

# ------------------------------------------------------------------------------

# Internal generic
calendar_check_component_range <- function(x, value, component, value_arg) {
  if (!calendar_is_valid_component(x, component)) {
    abort(paste0("`component` must be a valid component for a '", calendar_name(x), "'."))
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

calendar_precision <- function(x) {
  attr(x, "precision", exact = TRUE)
}

# ------------------------------------------------------------------------------

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

# For use in calendar constructor helpers
calendar_validate_subsecond_precision <- function(subsecond_precision) {
  if (is_null(subsecond_precision)) {
    abort("If `subsecond` is provided, `subsecond_precision` must be specified.")
  }
  if (!is_valid_subsecond_precision(subsecond_precision)) {
    abort("`subsecond_precision` must be a valid subsecond precision.")
  }
  subsecond_precision
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

arith_calendar_and_calendar <- function(op, x, y, ..., calendar_minus_calendar_fn) {
  switch (
    op,
    "-" = calendar_minus_calendar_fn(op, x, y, ...),
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

# ------------------------------------------------------------------------------

field_year <- function(x) {
  field(x, "year")
}
field_quarter <- function(x) {
  field(x, "quarter")
}
field_month <- function(x) {
  field(x, "month")
}
field_week <- function(x) {
  field(x, "week")
}
field_day <- function(x) {
  field(x, "day")
}
field_hour <- function(x) {
  field(x, "hour")
}
field_minute <- function(x) {
  field(x, "minute")
}
field_second <- function(x) {
  field(x, "second")
}
field_subsecond <- function(x) {
  field(x, "subsecond")
}
field_index <- function(x) {
  field(x, "index")
}
