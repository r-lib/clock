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
calendar_floor <- function(x, precision, ..., n = 1L, drop = TRUE) {
  UseMethod("calendar_floor")
}

#' @export
calendar_floor.clock_calendar <- function(x, precision, ..., n = 1L, drop = TRUE) {
  stop_clock_unsupported_calendar_op("calendar_floor")
}

# Year starting from 0. Returns `year` value for use in constructor.
calendar_year_floor <- function(x, n) {
  check_range(n, "year", "n")
  field <- field_year(x)
  compute_count(field, n)
}

# Returns calendar ready to be returned to the caller
calendar_month_floor <- function(x, n) {
  # Month of year
  check_range(n, "month", "n")
  field <- field_month(x)
  x <- calendar_cast(x, "year")
  x <- calendar_cast(x, "month")
  count <- compute_count1(field, n)
  x + duration_months(count)
}

# Returns sys_time to be converted back to calendar
calendar_day_floor <- function(x, n, field_fn, component) {
  check_range(n, component, "n")
  field <- field_fn(x)
  x <- calendar_cast(x, "month")
  x <- calendar_cast(x, "day")
  x <- as_sys_time(x)
  count <- compute_count1(field, n)
  x + duration_days(count)
}

# Returns sys_time to be converted back to calendar
calendar_time_floor <- function(x, precision, n) {
  if (precision == "hour") {
    # Hour of day
    precision_downcast <- "day"
    field_name <- "hour"
  } else if (precision == "minute") {
    # Minute of hour
    precision_downcast <- "hour"
    field_name <- "minute"
  } else if (precision == "second") {
    # Second of minute
    precision_downcast <- "minute"
    field_name <- "second"
  } else if (precision %in% c("millisecond", "microsecond", "nanosecond")) {
    # Millisecond/Microsecond/Nanosecond of second
    # Preemptive cast to ensure we extract the correct component
    x <- calendar_cast(x, precision)
    precision_downcast <- "second"
    field_name <- "subsecond"
  } else {
    abort("Internal error: `precision` is not a time based precision")
  }

  # Time based precision maps directly to component
  component <- precision
  check_range(n, component, "n")

  field <- field(x, field_name)
  count <- compute_count(field, n)

  x <- calendar_cast(x, precision_downcast)
  x <- calendar_cast(x, precision)

  x <- as_sys_time(x)

  x + duration_helper(count, precision)
}

compute_count <- function(field, n) {
  (field %/% n) * n
}
compute_count1 <- function(field, n) {
  ((field - 1L) %/% n) * n
}

check_rounding_n <- function(n) {
  n <- vec_cast(n, integer(), x_arg = "n")
  if (!is_number(n)) {
    abort("`n` must be a single number")
  }
  n
}
check_rounding_drop <- function(drop) {
  if (!is_bool(drop)) {
    abort("`drop` must be a single TRUE or FALSE.")
  }
  drop
}
check_rounding_precision <- function(x_precision, precision, verb) {
  if (precision_value(x_precision) < precision_value(precision)) {
    msg <- paste0("Can't ", verb, " to a precision that is more precise than `x`.")
    abort(msg)
  }
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

calendar_require_all_valid <- function(x) {
  if (invalid_any(x)) {
    message <- paste0(
      "Can't convert to a time point when there are invalid dates. ",
      "Resolve them before converting by calling `invalid_resolve()`."
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


