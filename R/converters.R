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
