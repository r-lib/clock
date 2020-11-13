convert_seconds_to_days_and_time_of_day <- function(seconds, zone) {
  convert_seconds_to_days_and_time_of_day_cpp(seconds, zone)
}

convert_local_days_and_time_of_day_to_sys_seconds <- function(days,
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

  convert_local_days_and_time_of_day_to_sys_seconds_cpp(
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

# ------------------------------------------------------------------------------

convert_datetime_fields_from_local_to_zoned <- function(days,
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

  convert_datetime_fields_from_local_to_zoned_cpp(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size
  )
}

convert_nano_datetime_fields_from_local_to_zoned <- function(days,
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

  convert_nano_datetime_fields_from_local_to_zoned_cpp(
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

convert_datetime_fields_from_zoned_to_local <- function(days, time_of_day, zone) {
  convert_datetime_fields_from_zoned_to_local_cpp(days, time_of_day, zone)
}

# ------------------------------------------------------------------------------

convert_sys_seconds_to_sys_days_and_time_of_day <- function(seconds) {
  convert_sys_seconds_to_sys_days_and_time_of_day_cpp(seconds)
}
