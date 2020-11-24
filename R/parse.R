parse_zoned_datetime <- function(x,
                                 ...,
                                 format = "%Y-%m-%d %H:%M:%S",
                                 zone = "UTC",
                                 locale = Sys.getlocale(category = "LC_TIME"),
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
    locale = locale,
    dst_nonexistent = dst_nonexistent,
    dst_ambiguous = dst_ambiguous,
    size = size
  )

  new_zoned_datetime_from_fields(fields, zone)
}

parse_local_datetime <- function(x,
                                 ...,
                                 format = "%Y-%m-%d %H:%M:%S",
                                 locale = Sys.getlocale(category = "LC_TIME")) {
  check_dots_empty()

  fields <- parse_local_datetime_cpp(
    x = x,
    format = format,
    locale = locale
  )

  new_local_datetime_from_fields(fields)
}
