civil_add_local <- function(x,
                            ...,
                            years = NULL,
                            months = NULL,
                            weeks = NULL,
                            days = NULL,
                            hours = NULL,
                            minutes = NULL,
                            seconds = NULL,
                            dst_nonexistant = "directional",
                            dst_ambiguous = "directional") {
  check_dots_empty()

  x <- to_posixct(x)

  years <- vec_cast(years, integer(), x_arg = "years")
  months <- vec_cast(months, integer(), x_arg = "months")
  weeks <- vec_cast(weeks, integer(), x_arg = "weeks")
  days <- vec_cast(days, integer(), x_arg = "days")
  hours <- vec_cast(hours, integer(), x_arg = "hours")
  minutes <- vec_cast(minutes, integer(), x_arg = "minutes")
  seconds <- vec_cast(seconds, integer(), x_arg = "seconds")

  size <- vec_size_common(
    x = x,
    years = years,
    months = months,
    weeks = weeks,
    days = days,
    hours = hours,
    minutes = minutes,
    seconds = seconds
  )

  civil_add_local_cpp(
    x,
    years,
    months,
    weeks,
    days,
    hours,
    minutes,
    seconds,
    dst_nonexistant,
    dst_ambiguous,
    size
  )
}


