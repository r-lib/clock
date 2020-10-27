civil_update <- function(x,
                         value,
                         unit,
                         ...,
                         day_nonexistant = "month-end",
                         dst_nonexistant = "next",
                         dst_ambiguous = "earliest") {
  check_dots_empty()

  x_ct <- to_posixct(x)
  value <- vec_cast(value, integer(), x_arg = "value")

  # Guarantee that we return something with same size as `x`.
  # `value` can be size 1 or the same size of `x`, and this checks that.
  check_value_size(value, x)

  out <- civil_update_cpp(
    x_ct,
    value,
    unit,
    day_nonexistant,
    dst_nonexistant,
    dst_ambiguous
  )

  if (is_time_based_unit(unit)) {
    out
  } else {
    from_posixct(out, x)
  }
}

check_value_size <- function(value, x) {
  value_size <- vec_size(value)
  x_size <- vec_size(x)

  if (x_size == value_size || value_size == 1L) {
    return(invisible(NULL))
  }

  msg <- paste0(
    "Can't recycle `value` (size ",
    value_size,
    ") to match `x` (size ",
    x_size,
    ")."
  )

  abort(msg)
}
