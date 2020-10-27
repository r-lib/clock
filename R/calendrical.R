civil_add_calendrical <- function(x,
                                  n,
                                  unit,
                                  ...,
                                  day_nonexistant = "month-end",
                                  dst_nonexistant = "directional",
                                  dst_ambiguous = "directional") {
  check_dots_empty()

  size <- vec_size_common(
    x = x,
    n = n
  )

  x_ct <- cast_posixct(x)
  n <- vec_cast(n, integer(), x_arg = "n")

  out <- civil_add_calendrical_cpp(
    x_ct,
    n,
    unit,
    day_nonexistant,
    dst_nonexistant,
    dst_ambiguous,
    size
  )

  if (is_time_based_unit(unit)) {
    out
  } else {
    restore(out, x)
  }
}


