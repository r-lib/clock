civil_add_chrono <- function(x,
                             ...,
                             years = NULL,
                             months = NULL,
                             weeks = NULL,
                             days = NULL,
                             hours = NULL,
                             minutes = NULL,
                             seconds = NULL) {
  check_dots_empty()

  x_ct <- cast_posixct(x)

  years <- vec_cast(years, integer(), x_arg = "years")
  months <- vec_cast(months, integer(), x_arg = "months")
  weeks <- vec_cast(weeks, integer(), x_arg = "weeks")
  days <- vec_cast(days, integer(), x_arg = "days")
  hours <- vec_cast(hours, integer(), x_arg = "hours")
  minutes <- vec_cast(minutes, integer(), x_arg = "minutes")
  seconds <- vec_cast(seconds, integer(), x_arg = "seconds")

  out <- civil_add_chrono_cpp(
    x_ct,
    years,
    months,
    weeks,
    days,
    hours,
    minutes,
    seconds
  )

  out
}


