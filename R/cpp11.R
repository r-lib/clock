# Generated by cpp11: do not edit by hand

new_duration_from_fields <- function(fields, precision_int, names) {
  .Call(`_clock_new_duration_from_fields`, fields, precision_int, names)
}

duration_restore <- function(x, to) {
  .Call(`_clock_duration_restore`, x, to)
}

format_duration_cpp <- function(fields, precision_int) {
  .Call(`_clock_format_duration_cpp`, fields, precision_int)
}

duration_helper_cpp <- function(n, precision_int) {
  .Call(`_clock_duration_helper_cpp`, n, precision_int)
}

duration_cast_cpp <- function(fields, precision_from, precision_to) {
  .Call(`_clock_duration_cast_cpp`, fields, precision_from, precision_to)
}

duration_plus_cpp <- function(x, y, precision_int) {
  .Call(`_clock_duration_plus_cpp`, x, y, precision_int)
}

duration_minus_cpp <- function(x, y, precision_int) {
  .Call(`_clock_duration_minus_cpp`, x, y, precision_int)
}

duration_modulus_cpp <- function(x, y, precision_int) {
  .Call(`_clock_duration_modulus_cpp`, x, y, precision_int)
}

duration_scalar_multiply_cpp <- function(x, y, precision_int) {
  .Call(`_clock_duration_scalar_multiply_cpp`, x, y, precision_int)
}

duration_scalar_divide_cpp <- function(x, y, precision_int) {
  .Call(`_clock_duration_scalar_divide_cpp`, x, y, precision_int)
}

duration_precision_common_cpp <- function(x_precision, y_precision) {
  .Call(`_clock_duration_precision_common_cpp`, x_precision, y_precision)
}

duration_has_common_precision_cpp <- function(x_precision, y_precision) {
  .Call(`_clock_duration_has_common_precision_cpp`, x_precision, y_precision)
}

duration_floor_cpp <- function(fields, precision_from, precision_to, n) {
  .Call(`_clock_duration_floor_cpp`, fields, precision_from, precision_to, n)
}

duration_ceiling_cpp <- function(fields, precision_from, precision_to, n) {
  .Call(`_clock_duration_ceiling_cpp`, fields, precision_from, precision_to, n)
}

duration_round_cpp <- function(fields, precision_from, precision_to, n) {
  .Call(`_clock_duration_round_cpp`, fields, precision_from, precision_to, n)
}

duration_unary_minus_cpp <- function(fields, precision_int) {
  .Call(`_clock_duration_unary_minus_cpp`, fields, precision_int)
}

duration_as_integer_cpp <- function(fields, precision_int) {
  .Call(`_clock_duration_as_integer_cpp`, fields, precision_int)
}

duration_as_double_cpp <- function(fields, precision_int) {
  .Call(`_clock_duration_as_double_cpp`, fields, precision_int)
}

duration_seq_by_lo_cpp <- function(from, precision_int, by, length_out) {
  .Call(`_clock_duration_seq_by_lo_cpp`, from, precision_int, by, length_out)
}

duration_seq_to_by_cpp <- function(from, precision_int, to, by) {
  .Call(`_clock_duration_seq_to_by_cpp`, from, precision_int, to, by)
}

duration_seq_to_lo_cpp <- function(from, precision_int, to, length_out) {
  .Call(`_clock_duration_seq_to_lo_cpp`, from, precision_int, to, length_out)
}

precision_to_string <- function(precision_int) {
  .Call(`_clock_precision_to_string`, precision_int)
}

clock_to_string <- function(clock_int) {
  .Call(`_clock_clock_to_string`, clock_int)
}

format_time_point_cpp <- function(fields, clock, format, precision_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, decimal_mark) {
  .Call(`_clock_format_time_point_cpp`, fields, clock, format, precision_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, decimal_mark)
}

format_zoned_time_cpp <- function(fields, zone, abbreviate_zone, format, precision_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, decimal_mark) {
  .Call(`_clock_format_zoned_time_cpp`, fields, zone, abbreviate_zone, format, precision_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, decimal_mark)
}

new_year_day_from_fields <- function(fields, precision_int, names) {
  .Call(`_clock_new_year_day_from_fields`, fields, precision_int, names)
}

year_day_restore <- function(x, to) {
  .Call(`_clock_year_day_restore`, x, to)
}

collect_year_day_fields <- function(fields, precision_int) {
  .Call(`_clock_collect_year_day_fields`, fields, precision_int)
}

format_year_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_format_year_day_cpp`, fields, precision_int)
}

invalid_detect_year_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_detect_year_day_cpp`, fields, precision_int)
}

invalid_any_year_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_any_year_day_cpp`, fields, precision_int)
}

invalid_count_year_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_count_year_day_cpp`, fields, precision_int)
}

invalid_resolve_year_day_cpp <- function(fields, precision_int, invalid_string) {
  .Call(`_clock_invalid_resolve_year_day_cpp`, fields, precision_int, invalid_string)
}

set_field_year_day_cpp <- function(fields, value, precision_fields, precision_value) {
  .Call(`_clock_set_field_year_day_cpp`, fields, value, precision_fields, precision_value)
}

set_field_year_day_last_cpp <- function(fields, precision_fields) {
  .Call(`_clock_set_field_year_day_last_cpp`, fields, precision_fields)
}

year_day_plus_duration_cpp <- function(fields, fields_n, precision_fields, precision_n) {
  .Call(`_clock_year_day_plus_duration_cpp`, fields, fields_n, precision_fields, precision_n)
}

as_sys_time_year_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_as_sys_time_year_day_cpp`, fields, precision_int)
}

as_year_day_from_sys_time_cpp <- function(fields, precision_int) {
  .Call(`_clock_as_year_day_from_sys_time_cpp`, fields, precision_int)
}

year_day_minus_year_day_cpp <- function(x, y, precision_int) {
  .Call(`_clock_year_day_minus_year_day_cpp`, x, y, precision_int)
}

new_year_month_day_from_fields <- function(fields, precision_int, names) {
  .Call(`_clock_new_year_month_day_from_fields`, fields, precision_int, names)
}

year_month_day_restore <- function(x, to) {
  .Call(`_clock_year_month_day_restore`, x, to)
}

collect_year_month_day_fields <- function(fields, precision_int) {
  .Call(`_clock_collect_year_month_day_fields`, fields, precision_int)
}

format_year_month_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_format_year_month_day_cpp`, fields, precision_int)
}

invalid_detect_year_month_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_detect_year_month_day_cpp`, fields, precision_int)
}

invalid_any_year_month_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_any_year_month_day_cpp`, fields, precision_int)
}

invalid_count_year_month_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_count_year_month_day_cpp`, fields, precision_int)
}

invalid_resolve_year_month_day_cpp <- function(fields, precision_int, invalid_string) {
  .Call(`_clock_invalid_resolve_year_month_day_cpp`, fields, precision_int, invalid_string)
}

set_field_year_month_day_cpp <- function(fields, value, precision_fields, precision_value) {
  .Call(`_clock_set_field_year_month_day_cpp`, fields, value, precision_fields, precision_value)
}

set_field_year_month_day_last_cpp <- function(fields, precision_fields) {
  .Call(`_clock_set_field_year_month_day_last_cpp`, fields, precision_fields)
}

year_month_day_plus_duration_cpp <- function(fields, fields_n, precision_fields, precision_n) {
  .Call(`_clock_year_month_day_plus_duration_cpp`, fields, fields_n, precision_fields, precision_n)
}

as_sys_time_year_month_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_as_sys_time_year_month_day_cpp`, fields, precision_int)
}

as_year_month_day_from_sys_time_cpp <- function(fields, precision_int) {
  .Call(`_clock_as_year_month_day_from_sys_time_cpp`, fields, precision_int)
}

year_month_day_minus_year_month_day_cpp <- function(x, y, precision_int) {
  .Call(`_clock_year_month_day_minus_year_month_day_cpp`, x, y, precision_int)
}

year_month_day_parse_cpp <- function(x, format, precision_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark) {
  .Call(`_clock_year_month_day_parse_cpp`, x, format, precision_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark)
}

gregorian_leap_year_cpp <- function(year) {
  .Call(`_clock_gregorian_leap_year_cpp`, year)
}

new_year_month_weekday_from_fields <- function(fields, precision_int, names) {
  .Call(`_clock_new_year_month_weekday_from_fields`, fields, precision_int, names)
}

year_month_weekday_restore <- function(x, to) {
  .Call(`_clock_year_month_weekday_restore`, x, to)
}

collect_year_month_weekday_fields <- function(fields, precision_int) {
  .Call(`_clock_collect_year_month_weekday_fields`, fields, precision_int)
}

format_year_month_weekday_cpp <- function(fields, precision_int) {
  .Call(`_clock_format_year_month_weekday_cpp`, fields, precision_int)
}

invalid_detect_year_month_weekday_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_detect_year_month_weekday_cpp`, fields, precision_int)
}

invalid_any_year_month_weekday_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_any_year_month_weekday_cpp`, fields, precision_int)
}

invalid_count_year_month_weekday_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_count_year_month_weekday_cpp`, fields, precision_int)
}

invalid_resolve_year_month_weekday_cpp <- function(fields, precision_int, invalid_string) {
  .Call(`_clock_invalid_resolve_year_month_weekday_cpp`, fields, precision_int, invalid_string)
}

set_field_year_month_weekday_cpp <- function(fields, value, precision_fields, component_string) {
  .Call(`_clock_set_field_year_month_weekday_cpp`, fields, value, precision_fields, component_string)
}

set_field_year_month_weekday_last_cpp <- function(fields, precision_fields) {
  .Call(`_clock_set_field_year_month_weekday_last_cpp`, fields, precision_fields)
}

year_month_weekday_plus_duration_cpp <- function(fields, fields_n, precision_fields, precision_n) {
  .Call(`_clock_year_month_weekday_plus_duration_cpp`, fields, fields_n, precision_fields, precision_n)
}

as_sys_time_year_month_weekday_cpp <- function(fields, precision_int) {
  .Call(`_clock_as_sys_time_year_month_weekday_cpp`, fields, precision_int)
}

as_year_month_weekday_from_sys_time_cpp <- function(fields, precision_int) {
  .Call(`_clock_as_year_month_weekday_from_sys_time_cpp`, fields, precision_int)
}

year_month_weekday_minus_year_month_weekday_cpp <- function(x, y, precision_int) {
  .Call(`_clock_year_month_weekday_minus_year_month_weekday_cpp`, x, y, precision_int)
}

new_iso_year_week_day_from_fields <- function(fields, precision_int, names) {
  .Call(`_clock_new_iso_year_week_day_from_fields`, fields, precision_int, names)
}

iso_year_week_day_restore <- function(x, to) {
  .Call(`_clock_iso_year_week_day_restore`, x, to)
}

collect_iso_year_week_day_fields <- function(fields, precision_int) {
  .Call(`_clock_collect_iso_year_week_day_fields`, fields, precision_int)
}

format_iso_year_week_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_format_iso_year_week_day_cpp`, fields, precision_int)
}

invalid_detect_iso_year_week_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_detect_iso_year_week_day_cpp`, fields, precision_int)
}

invalid_any_iso_year_week_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_any_iso_year_week_day_cpp`, fields, precision_int)
}

invalid_count_iso_year_week_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_invalid_count_iso_year_week_day_cpp`, fields, precision_int)
}

invalid_resolve_iso_year_week_day_cpp <- function(fields, precision_int, invalid_string) {
  .Call(`_clock_invalid_resolve_iso_year_week_day_cpp`, fields, precision_int, invalid_string)
}

set_field_iso_year_week_day_cpp <- function(fields, value, precision_fields, precision_value) {
  .Call(`_clock_set_field_iso_year_week_day_cpp`, fields, value, precision_fields, precision_value)
}

set_field_iso_year_week_day_last_cpp <- function(fields, precision_fields) {
  .Call(`_clock_set_field_iso_year_week_day_last_cpp`, fields, precision_fields)
}

iso_year_week_day_plus_duration_cpp <- function(fields, fields_n, precision_fields, precision_n) {
  .Call(`_clock_iso_year_week_day_plus_duration_cpp`, fields, fields_n, precision_fields, precision_n)
}

as_sys_time_iso_year_week_day_cpp <- function(fields, precision_int) {
  .Call(`_clock_as_sys_time_iso_year_week_day_cpp`, fields, precision_int)
}

as_iso_year_week_day_from_sys_time_cpp <- function(fields, precision_int) {
  .Call(`_clock_as_iso_year_week_day_from_sys_time_cpp`, fields, precision_int)
}

iso_year_week_day_minus_iso_year_week_day_cpp <- function(x, y, precision_int) {
  .Call(`_clock_iso_year_week_day_minus_iso_year_week_day_cpp`, x, y, precision_int)
}

naive_time_info_cpp <- function(fields, precision_int, zone) {
  .Call(`_clock_naive_time_info_cpp`, fields, precision_int, zone)
}

new_year_quarter_day_from_fields <- function(fields, precision_int, start, names) {
  .Call(`_clock_new_year_quarter_day_from_fields`, fields, precision_int, start, names)
}

year_quarter_day_restore <- function(x, to) {
  .Call(`_clock_year_quarter_day_restore`, x, to)
}

collect_year_quarter_day_fields <- function(fields, precision_int, start_int) {
  .Call(`_clock_collect_year_quarter_day_fields`, fields, precision_int, start_int)
}

format_year_quarter_day_cpp <- function(fields, precision_int, start_int) {
  .Call(`_clock_format_year_quarter_day_cpp`, fields, precision_int, start_int)
}

invalid_detect_year_quarter_day_cpp <- function(fields, precision_int, start_int) {
  .Call(`_clock_invalid_detect_year_quarter_day_cpp`, fields, precision_int, start_int)
}

invalid_any_year_quarter_day_cpp <- function(fields, precision_int, start_int) {
  .Call(`_clock_invalid_any_year_quarter_day_cpp`, fields, precision_int, start_int)
}

invalid_count_year_quarter_day_cpp <- function(fields, precision_int, start_int) {
  .Call(`_clock_invalid_count_year_quarter_day_cpp`, fields, precision_int, start_int)
}

invalid_resolve_year_quarter_day_cpp <- function(fields, precision_int, start_int, invalid_string) {
  .Call(`_clock_invalid_resolve_year_quarter_day_cpp`, fields, precision_int, start_int, invalid_string)
}

set_field_year_quarter_day_cpp <- function(fields, value, precision_fields, precision_value, start_int) {
  .Call(`_clock_set_field_year_quarter_day_cpp`, fields, value, precision_fields, precision_value, start_int)
}

set_field_year_quarter_day_last_cpp <- function(fields, precision_fields, start_int) {
  .Call(`_clock_set_field_year_quarter_day_last_cpp`, fields, precision_fields, start_int)
}

year_quarter_day_plus_duration_cpp <- function(fields, fields_n, precision_fields, precision_n, start_int) {
  .Call(`_clock_year_quarter_day_plus_duration_cpp`, fields, fields_n, precision_fields, precision_n, start_int)
}

as_sys_time_year_quarter_day_cpp <- function(fields, precision_int, start_int) {
  .Call(`_clock_as_sys_time_year_quarter_day_cpp`, fields, precision_int, start_int)
}

as_year_quarter_day_from_sys_time_cpp <- function(fields, precision_int, start_int) {
  .Call(`_clock_as_year_quarter_day_from_sys_time_cpp`, fields, precision_int, start_int)
}

year_quarter_day_minus_year_quarter_day_cpp <- function(x, y, precision_int, start_int) {
  .Call(`_clock_year_quarter_day_minus_year_quarter_day_cpp`, x, y, precision_int, start_int)
}

clock_rcrd_proxy <- function(x) {
  .Call(`_clock_clock_rcrd_proxy`, x)
}

clock_rcrd_names <- function(x) {
  .Call(`_clock_clock_rcrd_names`, x)
}

clock_rcrd_set_names <- function(x, names) {
  .Call(`_clock_clock_rcrd_set_names`, x, names)
}

sys_time_now_cpp <- function() {
  .Call(`_clock_sys_time_now_cpp`)
}

sys_time_info_cpp <- function(fields, precision_int, zone) {
  .Call(`_clock_sys_time_info_cpp`, fields, precision_int, zone)
}

new_time_point_from_fields <- function(fields, precision_int, clock_int, names) {
  .Call(`_clock_new_time_point_from_fields`, fields, precision_int, clock_int, names)
}

time_point_restore <- function(x, to) {
  .Call(`_clock_time_point_restore`, x, to)
}

time_point_parse_cpp <- function(x, format, precision_int, clock_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark) {
  .Call(`_clock_time_point_parse_cpp`, x, format, precision_int, clock_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark)
}

clock_init_utils <- function() {
  .Call(`_clock_clock_init_utils`)
}

weekday_add_days_cpp <- function(x, n) {
  .Call(`_clock_weekday_add_days_cpp`, x, n)
}

weekday_minus_weekday_cpp <- function(x, y) {
  .Call(`_clock_weekday_minus_weekday_cpp`, x, y)
}

weekday_from_time_point_cpp <- function(x) {
  .Call(`_clock_weekday_from_time_point_cpp`, x)
}

format_weekday_cpp <- function(x, labels) {
  .Call(`_clock_format_weekday_cpp`, x, labels)
}

zone_is_valid <- function(zone) {
  .Call(`_clock_zone_is_valid`, zone)
}

zone_current <- function() {
  .Call(`_clock_zone_current`)
}

new_zoned_time_from_fields <- function(fields, precision_int, zone, names) {
  .Call(`_clock_new_zoned_time_from_fields`, fields, precision_int, zone, names)
}

zoned_time_restore <- function(x, to) {
  .Call(`_clock_zoned_time_restore`, x, to)
}

get_naive_time_cpp <- function(fields, precision_int, zone) {
  .Call(`_clock_get_naive_time_cpp`, fields, precision_int, zone)
}

as_zoned_sys_time_from_naive_time_cpp <- function(fields, precision_int, zone, nonexistent_string, ambiguous_string) {
  .Call(`_clock_as_zoned_sys_time_from_naive_time_cpp`, fields, precision_int, zone, nonexistent_string, ambiguous_string)
}

as_zoned_sys_time_from_naive_time_with_reference_cpp <- function(fields, precision_int, zone, nonexistent_string, ambiguous_string, reference) {
  .Call(`_clock_as_zoned_sys_time_from_naive_time_with_reference_cpp`, fields, precision_int, zone, nonexistent_string, ambiguous_string, reference)
}

to_sys_duration_fields_from_sys_seconds_cpp <- function(seconds) {
  .Call(`_clock_to_sys_duration_fields_from_sys_seconds_cpp`, seconds)
}

to_sys_seconds_from_sys_duration_fields_cpp <- function(fields) {
  .Call(`_clock_to_sys_seconds_from_sys_duration_fields_cpp`, fields)
}

zoned_time_parse_complete_cpp <- function(x, format, precision_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark) {
  .Call(`_clock_zoned_time_parse_complete_cpp`, x, format, precision_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark)
}

zoned_time_parse_abbrev_cpp <- function(x, zone, format, precision_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark) {
  .Call(`_clock_zoned_time_parse_abbrev_cpp`, x, zone, format, precision_int, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark)
}
