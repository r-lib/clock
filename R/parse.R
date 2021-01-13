# parse_zoned_datetime <- function(x,
#                                  ...,
#                                  format = "%Y-%m-%d %H:%M:%S",
#                                  zone = "UTC",
#                                  dst_nonexistent = "roll-forward",
#                                  dst_ambiguous = "earliest") {
#   check_dots_empty()
#
#   size <- vec_size_common(
#     x = x,
#     dst_nonexistent = dst_nonexistent,
#     dst_ambiguous = dst_ambiguous
#   )
#
#   zone <- zone_standardize(zone)
#
#   fields <- parse_zoned_datetime_cpp(
#     x = x,
#     format = format,
#     zone = zone,
#     dst_nonexistent = dst_nonexistent,
#     dst_ambiguous = dst_ambiguous,
#     size = size
#   )
#
#   days <- fields$days
#   time_of_day <- fields$time_of_day
#
#   calendar <- new_year_month_day(days)
#   seconds_of_day <- time_of_day
#
#   new_zoned_time_point(calendar, seconds_of_day, precision = "second", zone = zone)
# }
#
# parse_naive_datetime <- function(x,
#                                  ...,
#                                  format = "%Y-%m-%d %H:%M:%S") {
#   check_dots_empty()
#
#   fields <- parse_naive_datetime_cpp(
#     x = x,
#     format = format
#   )
#
#   days <- fields$days
#   time_of_day <- fields$time_of_day
#
#   calendar <- new_year_month_day(days)
#   seconds_of_day <- time_of_day
#
#   new_naive_time_point(calendar, seconds_of_day, precision = "second")
# }
