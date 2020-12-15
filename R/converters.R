convert_naive_second_point_fields_to_zoned_seconds <- function(calendar,
                                                               seconds_of_day,
                                                               zone,
                                                               dst_nonexistent,
                                                               dst_ambiguous) {
  size <- vec_size_common(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  convert_naive_second_point_fields_to_zoned_seconds_cpp(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size
  )
}

# ------------------------------------------------------------------------------

convert_year_month_day_to_naive_fields <- function(year, month, day, day_nonexistent) {
  convert_year_month_day_to_naive_fields_cpp(
    year = year,
    month = month,
    day = day,
    day_nonexistent = day_nonexistent
  )
}

convert_year_month_day_hour_minute_second_to_naive_fields <- function(year,
                                                                      month,
                                                                      day,
                                                                      hour,
                                                                      minute,
                                                                      second,
                                                                      day_nonexistent) {
  convert_year_month_day_hour_minute_second_to_naive_fields_cpp(
    year = year,
    month = month,
    day = day,
    hour = hour,
    minute = minute,
    second = second,
    day_nonexistent = day_nonexistent
  )
}

# ------------------------------------------------------------------------------

convert_naive_days_to_year_month_day <- function(days) {
  convert_naive_days_to_year_month_day_cpp(days)
}

convert_naive_time_of_day_to_hour_minute_second <- function(time_of_day) {
  convert_naive_time_of_day_to_hour_minute_second_cpp(time_of_day)
}

# ------------------------------------------------------------------------------

convert_datetime_fields_from_naive_to_zoned <- function(days,
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

  convert_datetime_fields_from_naive_to_zoned_cpp(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size
  )
}

# ------------------------------------------------------------------------------

convert_nano_datetime_fields_from_naive_to_zoned <- function(days,
                                                             time_of_day,
                                                             nanos_of_second,
                                                             zone,
                                                             dst_nonexistent,
                                                             dst_ambiguous) {
  size <- vec_size_common(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  convert_nano_datetime_fields_from_naive_to_zoned_cpp(
    days = days,
    time_of_day = time_of_day,
    nanos_of_second = nanos_of_second,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size
  )
}

# ------------------------------------------------------------------------------

convert_second_point_fields_from_naive_to_zoned <- function(calendar,
                                                            seconds_of_day,
                                                            zone,
                                                            dst_nonexistent,
                                                            dst_ambiguous) {
  size <- vec_size_common(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  convert_second_point_fields_from_naive_to_zoned_cpp(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size
  )
}

convert_subsecond_point_fields_from_naive_to_zoned <- function(calendar,
                                                               seconds_of_day,
                                                               nanoseconds_of_second,
                                                               zone,
                                                               dst_nonexistent,
                                                               dst_ambiguous) {
  size <- vec_size_common(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  convert_subsecond_point_fields_from_naive_to_zoned_cpp(
    calendar = calendar,
    seconds_of_day = seconds_of_day,
    nanoseconds_of_second = nanoseconds_of_second,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size
  )
}
