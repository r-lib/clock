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

# ------------------------------------------------------------------------------

convert_year_month_day_to_fields <- function(year, month, day, day_nonexistent) {
  convert_year_month_day_to_fields_cpp(
    year = year,
    month = month,
    day = day,
    day_nonexistent = day_nonexistent
  )
}

convert_year_month_day_hour_minute_second_to_fields <- function(year,
                                                                month,
                                                                day,
                                                                hour,
                                                                minute,
                                                                second,
                                                                day_nonexistent) {
  convert_year_month_day_hour_minute_second_to_fields_cpp(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second,
    day_nonexistent = day_nonexistent
  )
}

convert_year_month_day_hour_minute_second_nanos_to_fields <- function(year,
                                                                      month,
                                                                      day,
                                                                      hour,
                                                                      minute,
                                                                      second,
                                                                      nanos,
                                                                      day_nonexistent) {
  convert_year_month_day_hour_minute_second_nanos_to_fields_cpp(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second,
    nanos = nanos,
    day_nonexistent = day_nonexistent
  )
}

# ------------------------------------------------------------------------------

convert_days_to_year_month_day <- function(days) {
  convert_days_to_year_month_day_cpp(days)
}

convert_time_of_day_to_hour_minute_second <- function(time_of_day) {
  convert_time_of_day_to_hour_minute_second_cpp(time_of_day)
}


