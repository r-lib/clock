parse_naive_time <- function(x,
                             format,
                             ...,
                             precision = "second") {
  check_dots_empty()
  precision <- validate_time_point_precision(precision)
  fields <- parse_time_point_cpp(x, format, precision, CLOCK_NAIVE)
  new_naive_time_from_fields(fields, precision, names(x))
}

parse_sys_time <- function(x,
                           format,
                           ...,
                           precision = "second") {
  check_dots_empty()
  precision <- validate_time_point_precision(precision)
  fields <- parse_time_point_cpp(x, format, precision, CLOCK_SYS)
  new_sys_time_from_fields(fields, precision, names(x))
}


# TODO: Remove this or keep?
#
# Current thinking:
# Parsing into sys / naive time is enough. The user should know what time
# zone to use, and we can assume there is only 1 time zone for the entire
# character vector. The only difference between parsing into naive/sys
# time (besides how they are interpreted later) is that parsing into sys
# time while using %z will treat the input as local time, then offset it
# by %z to get the sys time.
#
# parse_zoned_time <- function(x,
#                              format,
#                              zone,
#                              ...,
#                              precision = "second",
#                              nonexistent = "error",
#                              ambiguous = "error") {
#   check_dots_empty()
#
#   # `nonexistent` and `ambiguous` are allowed to be
#   # size 1 or the same size as `x`
#   size <- vec_size(x)
#   validate_nonexistent(nonexistent, size)
#   validate_ambiguous(nonexistent, size)
#
#   zone <- zone_validate(zone)
#   precision <- validate_zoned_time_precision(precision)
#
#   fields <- parse_zoned_time_cpp(x, format, precision, zone, nonexistent, ambiguous)
#
#   new_zoned_time_from_fields(fields, precision, zone, names(x))
# }
