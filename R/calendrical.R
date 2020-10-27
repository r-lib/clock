civil_add_calendrical <- function(x,
                                  n,
                                  unit,
                                  ...,
                                  day_nonexistant = "month-end",
                                  dst_nonexistant = "directional",
                                  dst_ambiguous = "directional") {
  check_dots_empty()

  x <- cast_posixct(x)
  n <- vec_cast(n, integer(), x_arg = "n")

  size <- vec_size_common(
    x = x,
    n = n
  )

  civil_add_calendrical_cpp(
    x,
    n,
    unit,
    day_nonexistant,
    dst_nonexistant,
    dst_ambiguous,
    size
  )
}


