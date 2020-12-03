parse_zoned_datetime <- function(x,
                                 ...,
                                 format = "%Y-%m-%d %H:%M:%S",
                                 zone = "UTC",
                                 dst_nonexistent = "roll-forward",
                                 dst_ambiguous = "earliest") {
  check_dots_empty()

  size <- vec_size_common(
    x = x,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous
  )

  zone <- zone_standardize(zone)

  fields <- parse_zoned_datetime_cpp(
    x = x,
    format = format,
    zone = zone,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size
  )

  new_zoned_datetime_from_fields(fields, zone)
}

parse_naive_datetime <- function(x,
                                 ...,
                                 format = "%Y-%m-%d %H:%M:%S") {
  check_dots_empty()

  fields <- parse_naive_datetime_cpp(
    x = x,
    format = format
  )

  new_naive_datetime_from_fields(fields)
}
