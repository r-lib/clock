convert_days_to_year_month_day <- function(days) {
  convert_days_to_year_month_day_cpp(days)
}

convert_year_month_day_to_days <- function(year,
                                           month,
                                           day,
                                           day_nonexistent) {
  convert_year_month_day_to_days_cpp(year, month, day, day_nonexistent)
}

convert_hour_minute_second_to_time_of_day <- function(hour, minute, second) {
  convert_hour_minute_second_to_time_of_day_cpp(hour, minute, second)
}

convert_time_of_day_to_hour_minute_second <- function(time_of_day) {
  convert_time_of_day_to_hour_minute_second_cpp(time_of_day)
}

convert_seconds_to_days_and_time_of_day <- function(seconds, zone) {
  convert_seconds_to_days_and_time_of_day_cpp(seconds, zone)
}

convert_days_and_time_of_day_to_seconds <- function(days,
                                                    time_of_day,
                                                    zone,
                                                    dst_nonexistent,
                                                    dst_ambiguous) {
  size <- vec_size_common(
    days = days,
    time_of_day = time_of_day,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  convert_days_and_time_of_day_to_seconds_cpp(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size
  )
}
