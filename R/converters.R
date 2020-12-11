convert_sys_seconds_to_naive_days_and_time_of_day <- function(seconds, zone) {
  convert_sys_seconds_to_naive_days_and_time_of_day_cpp(seconds, zone)
}

convert_naive_days_and_time_of_day_to_sys_seconds <- function(days,
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

  convert_naive_days_and_time_of_day_to_sys_seconds_cpp(
    days = days,
    time_of_day = time_of_day,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size
  )
}

# ------------------------------------------------------------------------------

convert_sys_seconds_to_sys_days_and_time_of_day <- function(seconds) {
  convert_sys_seconds_to_sys_days_and_time_of_day_cpp(seconds)
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

convert_year_month_day_hour_minute_second_nanos_to_naive_fields <- function(year,
                                                                            month,
                                                                            day,
                                                                            hour,
                                                                            minute,
                                                                            second,
                                                                            nanos,
                                                                            day_nonexistent) {
  convert_year_month_day_hour_minute_second_nanos_to_naive_fields_cpp(
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

convert_datetime_fields_from_zoned_to_naive <- function(days, time_of_day, zone) {
  convert_datetime_fields_from_zoned_to_naive_cpp(days, time_of_day, zone)
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

convert_quarterly_year_quarternum_quarterday_to_naive_days <- function(year,
                                                                       quarternum,
                                                                       quarterday,
                                                                       start,
                                                                       day_nonexistent) {
  convert_quarterly_year_quarternum_quarterday_to_naive_days_cpp(year, quarternum, quarterday, start, day_nonexistent)
}

convert_naive_days_to_quarterly_year_quarternum_quarterday <- function(days, start) {
  convert_naive_days_to_quarterly_year_quarternum_quarterday_cpp(days, start)
}
