#include "gregorian-year-month-day.h"
#include "calendar.h"
#include "duration.h"
#include "check.h"
#include "enums.h"
#include "get.h"
#include "parse.h"
#include "locale.h"

[[cpp11::register]]
void
year_month_day_check_range_cpp(const cpp11::integers& x,
                               const cpp11::strings& component_string,
                               const cpp11::strings& arg) {
  std::string x_arg_string = arg[0];
  const char* x_arg = x_arg_string.c_str();

  // Used to access the non-static method
  rclock::gregorian::y dummy(0);

  switch (parse_component(component_string)) {
  case component::year: return calendar_check_range_impl<component::year>(dummy, x, x_arg);
  case component::month: return calendar_check_range_impl<component::month>(dummy, x, x_arg);
  case component::day: return calendar_check_range_impl<component::day>(dummy, x, x_arg);
  case component::hour: return calendar_check_range_impl<component::hour>(dummy, x, x_arg);
  case component::minute: return calendar_check_range_impl<component::minute>(dummy, x, x_arg);
  case component::second: return calendar_check_range_impl<component::second>(dummy, x, x_arg);
  case component::millisecond: return calendar_check_range_impl<component::millisecond>(dummy, x, x_arg);
  case component::microsecond: return calendar_check_range_impl<component::microsecond>(dummy, x, x_arg);
  case component::nanosecond: return calendar_check_range_impl<component::nanosecond>(dummy, x, x_arg);
  default: clock_abort("Internal error: Unknown component");
  }

  never_reached("year_month_day_check_range_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
collect_year_month_day_fields(cpp11::list_of<cpp11::integers> fields,
                              const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);
  cpp11::integers subsecond = gregorian::get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: {
    collect_field<component::year>(y, year, "year");
    return y.to_list();
  }
  case precision::month: {
    collect_field<component::year>(ym, year, "year");
    collect_field<component::month>(ym, month, "month");
    return ym.to_list();
  }
  case precision::day: {
    collect_field<component::year>(ymd, year, "year");
    collect_field<component::month>(ymd, month, "month");
    collect_field<component::day>(ymd, day, "day");
    return ymd.to_list();
  }
  case precision::hour: {
    collect_field<component::year>(ymdh, year, "year");
    collect_field<component::month>(ymdh, month, "month");
    collect_field<component::day>(ymdh, day, "day");
    collect_field<component::hour>(ymdh, hour, "hour");
    return ymdh.to_list();
  }
  case precision::minute: {
    collect_field<component::year>(ymdhm, year, "year");
    collect_field<component::month>(ymdhm, month, "month");
    collect_field<component::day>(ymdhm, day, "day");
    collect_field<component::hour>(ymdhm, hour, "hour");
    collect_field<component::minute>(ymdhm, minute, "minute");
    return ymdhm.to_list();
  }
  case precision::second: {
    collect_field<component::year>(ymdhms, year, "year");
    collect_field<component::month>(ymdhms, month, "month");
    collect_field<component::day>(ymdhms, day, "day");
    collect_field<component::hour>(ymdhms, hour, "hour");
    collect_field<component::minute>(ymdhms, minute, "minute");
    collect_field<component::second>(ymdhms, second, "second");
    return ymdhms.to_list();
  }
  case precision::millisecond: {
    collect_field<component::year>(ymdhmss1, year, "year");
    collect_field<component::month>(ymdhmss1, month, "month");
    collect_field<component::day>(ymdhmss1, day, "day");
    collect_field<component::hour>(ymdhmss1, hour, "hour");
    collect_field<component::minute>(ymdhmss1, minute, "minute");
    collect_field<component::second>(ymdhmss1, second, "second");
    collect_field<component::millisecond>(ymdhmss1, subsecond, "subsecond");
    return ymdhmss1.to_list();
  }
  case precision::microsecond: {
    collect_field<component::year>(ymdhmss2, year, "year");
    collect_field<component::month>(ymdhmss2, month, "month");
    collect_field<component::day>(ymdhmss2, day, "day");
    collect_field<component::hour>(ymdhmss2, hour, "hour");
    collect_field<component::minute>(ymdhmss2, minute, "minute");
    collect_field<component::second>(ymdhmss2, second, "second");
    collect_field<component::microsecond>(ymdhmss2, subsecond, "subsecond");
    return ymdhmss2.to_list();
  }
  case precision::nanosecond: {
    collect_field<component::year>(ymdhmss3, year, "year");
    collect_field<component::month>(ymdhmss3, month, "month");
    collect_field<component::day>(ymdhmss3, day, "day");
    collect_field<component::hour>(ymdhmss3, hour, "hour");
    collect_field<component::minute>(ymdhmss3, minute, "minute");
    collect_field<component::second>(ymdhmss3, second, "second");
    collect_field<component::nanosecond>(ymdhmss3, subsecond, "subsecond");
    return ymdhmss3.to_list();
  }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("collect_year_month_day_fields");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
format_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                          const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);
  cpp11::integers subsecond = gregorian::get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return format_calendar_impl(y);
  case precision::month: return format_calendar_impl(ym);
  case precision::day: return format_calendar_impl(ymd);
  case precision::hour: return format_calendar_impl(ymdh);
  case precision::minute: return format_calendar_impl(ymdhm);
  case precision::second: return format_calendar_impl(ymdhms);
  case precision::millisecond: return format_calendar_impl(ymdhmss1);
  case precision::microsecond: return format_calendar_impl(ymdhmss2);
  case precision::nanosecond: return format_calendar_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("format_year_month_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);
  cpp11::integers subsecond = gregorian::get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_detect_calendar_impl(y);
  case precision::month: return invalid_detect_calendar_impl(ym);
  case precision::day: return invalid_detect_calendar_impl(ymd);
  case precision::hour: return invalid_detect_calendar_impl(ymdh);
  case precision::minute: return invalid_detect_calendar_impl(ymdhm);
  case precision::second: return invalid_detect_calendar_impl(ymdhms);
  case precision::millisecond: return invalid_detect_calendar_impl(ymdhmss1);
  case precision::microsecond: return invalid_detect_calendar_impl(ymdhmss2);
  case precision::nanosecond: return invalid_detect_calendar_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_detect_year_month_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);
  cpp11::integers subsecond = gregorian::get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_any_calendar_impl(y);
  case precision::month: return invalid_any_calendar_impl(ym);
  case precision::day: return invalid_any_calendar_impl(ymd);
  case precision::hour: return invalid_any_calendar_impl(ymdh);
  case precision::minute: return invalid_any_calendar_impl(ymdhm);
  case precision::second: return invalid_any_calendar_impl(ymdhms);
  case precision::millisecond: return invalid_any_calendar_impl(ymdhmss1);
  case precision::microsecond: return invalid_any_calendar_impl(ymdhmss2);
  case precision::nanosecond: return invalid_any_calendar_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_any_year_month_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);
  cpp11::integers subsecond = gregorian::get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_count_calendar_impl(y);
  case precision::month: return invalid_count_calendar_impl(ym);
  case precision::day: return invalid_count_calendar_impl(ymd);
  case precision::hour: return invalid_count_calendar_impl(ymdh);
  case precision::minute: return invalid_count_calendar_impl(ymdhm);
  case precision::second: return invalid_count_calendar_impl(ymdhms);
  case precision::millisecond: return invalid_count_calendar_impl(ymdhmss1);
  case precision::microsecond: return invalid_count_calendar_impl(ymdhmss2);
  case precision::nanosecond: return invalid_count_calendar_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_count_year_month_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                   const cpp11::integers& precision_int,
                                   const cpp11::strings& invalid_string) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid_string);

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);
  cpp11::integers subsecond = gregorian::get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_resolve_calendar_impl(y, invalid_val);
  case precision::month: return invalid_resolve_calendar_impl(ym, invalid_val);
  case precision::day: return invalid_resolve_calendar_impl(ymd, invalid_val);
  case precision::hour: return invalid_resolve_calendar_impl(ymdh, invalid_val);
  case precision::minute: return invalid_resolve_calendar_impl(ymdhm, invalid_val);
  case precision::second: return invalid_resolve_calendar_impl(ymdhms, invalid_val);
  case precision::millisecond: return invalid_resolve_calendar_impl(ymdhmss1, invalid_val);
  case precision::microsecond: return invalid_resolve_calendar_impl(ymdhmss2, invalid_val);
  case precision::nanosecond: return invalid_resolve_calendar_impl(ymdhmss3, invalid_val);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_resolve_year_month_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
set_field_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                             const cpp11::integers& value,
                             const cpp11::integers& precision_fields,
                             const cpp11::integers& precision_value) {
  using namespace rclock;
  rclock::integers value2(value);

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);
  cpp11::integers subsecond = gregorian::get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::year: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(y, value2);
    case precision::month: return set_field_calendar<component::month>(y, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::month: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ym, value2);
    case precision::month: return set_field_calendar<component::month>(ym, value2);
    case precision::day: return set_field_calendar<component::day>(ym, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::day: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ymd, value2);
    case precision::month: return set_field_calendar<component::month>(ymd, value2);
    case precision::day: return set_field_calendar<component::day>(ymd, value2);
    case precision::hour: return set_field_calendar<component::hour>(ymd, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::hour: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ymdh, value2);
    case precision::month: return set_field_calendar<component::month>(ymdh, value2);
    case precision::day: return set_field_calendar<component::day>(ymdh, value2);
    case precision::hour: return set_field_calendar<component::hour>(ymdh, value2);
    case precision::minute: return set_field_calendar<component::minute>(ymdh, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::minute: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ymdhm, value2);
    case precision::month: return set_field_calendar<component::month>(ymdhm, value2);
    case precision::day: return set_field_calendar<component::day>(ymdhm, value2);
    case precision::hour: return set_field_calendar<component::hour>(ymdhm, value2);
    case precision::minute: return set_field_calendar<component::minute>(ymdhm, value2);
    case precision::second: return set_field_calendar<component::second>(ymdhm, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::second: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ymdhms, value2);
    case precision::month: return set_field_calendar<component::month>(ymdhms, value2);
    case precision::day: return set_field_calendar<component::day>(ymdhms, value2);
    case precision::hour: return set_field_calendar<component::hour>(ymdhms, value2);
    case precision::minute: return set_field_calendar<component::minute>(ymdhms, value2);
    case precision::second: return set_field_calendar<component::second>(ymdhms, value2);
    case precision::millisecond: return set_field_calendar<component::millisecond>(ymdhms, value2);
    case precision::microsecond: return set_field_calendar<component::microsecond>(ymdhms, value2);
    case precision::nanosecond: return set_field_calendar<component::nanosecond>(ymdhms, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::millisecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ymdhmss1, value2);
    case precision::month: return set_field_calendar<component::month>(ymdhmss1, value2);
    case precision::day: return set_field_calendar<component::day>(ymdhmss1, value2);
    case precision::hour: return set_field_calendar<component::hour>(ymdhmss1, value2);
    case precision::minute: return set_field_calendar<component::minute>(ymdhmss1, value2);
    case precision::second: return set_field_calendar<component::second>(ymdhmss1, value2);
    case precision::millisecond: return set_field_calendar<component::millisecond>(ymdhmss1, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::microsecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ymdhmss2, value2);
    case precision::month: return set_field_calendar<component::month>(ymdhmss2, value2);
    case precision::day: return set_field_calendar<component::day>(ymdhmss2, value2);
    case precision::hour: return set_field_calendar<component::hour>(ymdhmss2, value2);
    case precision::minute: return set_field_calendar<component::minute>(ymdhmss2, value2);
    case precision::second: return set_field_calendar<component::second>(ymdhmss2, value2);
    case precision::microsecond: return set_field_calendar<component::microsecond>(ymdhmss2, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::nanosecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ymdhmss3, value2);
    case precision::month: return set_field_calendar<component::month>(ymdhmss3, value2);
    case precision::day: return set_field_calendar<component::day>(ymdhmss3, value2);
    case precision::hour: return set_field_calendar<component::hour>(ymdhmss3, value2);
    case precision::minute: return set_field_calendar<component::minute>(ymdhmss3, value2);
    case precision::second: return set_field_calendar<component::second>(ymdhmss3, value2);
    case precision::nanosecond: return set_field_calendar<component::nanosecond>(ymdhmss3, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("set_field_year_month_day_cpp");
}

template <class Calendar>
cpp11::writable::list
set_field_year_month_day_last_impl(const Calendar& x) {
  const r_ssize size = x.size();
  cpp11::writable::integers day(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      day[i] = r_int_na;
    } else {
      date::year_month_day_last ymdl = x.to_year_month(i) / date::last;
      day[i] = static_cast<int>(static_cast<unsigned>(ymdl.day()));
    }
  }

  cpp11::writable::list out({x.to_list(), day});
  out.names() = {"fields", "value"};

  return out;
}

[[cpp11::register]]
cpp11::writable::list
set_field_year_month_day_last_cpp(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::integers& precision_fields) {
  using namespace rclock;

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);
  cpp11::integers subsecond = gregorian::get_subsecond(fields);

  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::month: return set_field_year_month_day_last_impl(ym);
  case precision::day: return set_field_year_month_day_last_impl(ymd);
  case precision::hour: return set_field_year_month_day_last_impl(ymdh);
  case precision::minute: return set_field_year_month_day_last_impl(ymdhm);
  case precision::second: return set_field_year_month_day_last_impl(ymdhms);
  case precision::millisecond: return set_field_year_month_day_last_impl(ymdhmss1);
  case precision::microsecond: return set_field_year_month_day_last_impl(ymdhmss2);
  case precision::nanosecond: return set_field_year_month_day_last_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("set_field_year_month_day_last_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
year_month_day_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                 cpp11::list_of<cpp11::integers> fields_n,
                                 const cpp11::integers& precision_fields,
                                 const cpp11::integers& precision_n) {
  using namespace rclock;

  const enum precision precision_fields_val = parse_precision(precision_fields);
  const enum precision precision_n_val = parse_precision(precision_n);

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);
  cpp11::integers subsecond = gregorian::get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  cpp11::integers ticks = duration::get_ticks(fields_n);

  duration::years dy{ticks};
  duration::quarters dq{ticks};
  duration::months dm{ticks};

  switch (precision_fields_val) {
  case precision::year:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(y, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::month:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ym, dy);
    case precision::quarter: return calendar_plus_duration_impl(ym, dq);
    case precision::month: return calendar_plus_duration_impl(ym, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::day:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymd, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymd, dq);
    case precision::month: return calendar_plus_duration_impl(ymd, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::hour:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymdh, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymdh, dq);
    case precision::month: return calendar_plus_duration_impl(ymdh, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::minute:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymdhm, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymdhm, dq);
    case precision::month: return calendar_plus_duration_impl(ymdhm, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::second:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymdhms, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymdhms, dq);
    case precision::month: return calendar_plus_duration_impl(ymdhms, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::millisecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymdhmss1, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymdhmss1, dq);
    case precision::month: return calendar_plus_duration_impl(ymdhmss1, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::microsecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymdhmss2, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymdhmss2, dq);
    case precision::month: return calendar_plus_duration_impl(ymdhmss2, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::nanosecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymdhmss3, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymdhmss3, dq);
    case precision::month: return calendar_plus_duration_impl(ymdhmss3, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_month_day_plus_duration_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);
  cpp11::integers subsecond = gregorian::get_subsecond(fields);

  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::day: return as_sys_time_from_calendar_impl<duration::days>(ymd);
  case precision::hour: return as_sys_time_from_calendar_impl<duration::hours>(ymdh);
  case precision::minute: return as_sys_time_from_calendar_impl<duration::minutes>(ymdhm);
  case precision::second: return as_sys_time_from_calendar_impl<duration::seconds>(ymdhms);
  case precision::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(ymdhmss1);
  case precision::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(ymdhmss2);
  case precision::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(ymdhmss3);
  default: {
    const enum precision precision_val = parse_precision(precision_int);
    const std::string precision_string = precision_to_cpp_string(precision_val);
    std::string message =
      "Can't convert to a time point from a calendar with '" +
      precision_string +
      "' precision. " +
      "A minimum of 'day' precision is required.";
    clock_abort(message.c_str());
  }
  }

  never_reached("as_sys_time_year_month_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_year_month_day_from_sys_time_cpp(cpp11::list_of<cpp11::integers> fields,
                                    const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers ticks = duration::get_ticks(fields);
  cpp11::integers ticks_of_day = duration::get_ticks_of_day(fields);
  cpp11::integers ticks_of_second = duration::get_ticks_of_second(fields);

  duration::days dd{ticks};
  duration::hours dh{ticks, ticks_of_day};
  duration::minutes dmin{ticks, ticks_of_day};
  duration::seconds ds{ticks, ticks_of_day};
  duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision(precision_int)) {
  case precision::day: return as_calendar_from_sys_time_impl<gregorian::ymd>(dd);
  case precision::hour: return as_calendar_from_sys_time_impl<gregorian::ymdh>(dh);
  case precision::minute: return as_calendar_from_sys_time_impl<gregorian::ymdhm>(dmin);
  case precision::second: return as_calendar_from_sys_time_impl<gregorian::ymdhms>(ds);
  case precision::millisecond: return as_calendar_from_sys_time_impl<gregorian::ymdhmss<std::chrono::milliseconds>>(dmilli);
  case precision::microsecond: return as_calendar_from_sys_time_impl<gregorian::ymdhmss<std::chrono::microseconds>>(dmicro);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<gregorian::ymdhmss<std::chrono::nanoseconds>>(dnano);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("as_year_month_day_from_sys_time_cpp");
}

// -----------------------------------------------------------------------------

static
inline
cpp11::writable::list
year_minus_year_impl(const rclock::gregorian::y& x,
                     const rclock::gregorian::y& y) {
  const r_ssize size = x.size();
  rclock::duration::years out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || y.is_na(i)) {
      out.assign_na(i);
      continue;
    }
    out.assign(x.to_year(i) - y.to_year(i), i);
  }

  return out.to_list();
}

static
inline
cpp11::writable::list
year_month_minus_year_month_impl(const rclock::gregorian::ym& x,
                                 const rclock::gregorian::ym& y) {
  const r_ssize size = x.size();
  rclock::duration::months out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || y.is_na(i)) {
      out.assign_na(i);
      continue;
    }
    out.assign(x.to_year_month(i) - y.to_year_month(i), i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
year_month_day_minus_year_month_day_cpp(cpp11::list_of<cpp11::integers> x,
                                        cpp11::list_of<cpp11::integers> y,
                                        const cpp11::integers& precision_int) {
  const cpp11::integers x_year = rclock::gregorian::get_year(x);
  const cpp11::integers x_month = rclock::gregorian::get_month(x);

  const cpp11::integers y_year = rclock::gregorian::get_year(y);
  const cpp11::integers y_month = rclock::gregorian::get_month(y);

  const rclock::gregorian::y x_y{x_year};
  const rclock::gregorian::ym x_ym{x_year, x_month};

  const rclock::gregorian::y y_y{y_year};
  const rclock::gregorian::ym y_ym{y_year, y_month};

  switch (parse_precision(precision_int)) {
  case precision::year: return year_minus_year_impl(x_y, y_y);
  case precision::month: return year_month_minus_year_month_impl(x_ym, y_ym);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_month_day_minus_year_month_day_cpp");
}

// -----------------------------------------------------------------------------

// Default impl applies to millisecond/microsecond/nanosecond parsers
template <class Calendar>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const char* format,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const r_ssize& i,
                           Calendar& out) {
  using Duration = typename Calendar::duration;
  date::year_month_day ymd{};
  date::hh_mm_ss<Duration> hms{};
  rclock::from_stream(stream, format, month_names_pair, weekday_names_pair, ampm_names_pair, ymd, hms);
  if (stream.fail()) {
    out.assign_na(i);
  } else {
    out.assign_year_month_day(ymd, i);
    out.assign_hour(hms.hours(), i);
    out.assign_minute(hms.minutes(), i);
    out.assign_second(hms.seconds(), i);
    out.assign_subsecond(hms.subseconds(), i);
  }
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const char* format,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const r_ssize& i,
                           rclock::gregorian::y& out) {
  date::year x{};
  rclock::from_stream(stream, format, month_names_pair, weekday_names_pair, ampm_names_pair, x);
  if (stream.fail()) {
    out.assign_na(i);
  } else {
    out.assign_year(x, i);
  }
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const char* format,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const r_ssize& i,
                           rclock::gregorian::ym& out) {
  date::year_month x{};
  rclock::from_stream(stream, format, month_names_pair, weekday_names_pair, ampm_names_pair, x);
  if (stream.fail()) {
    out.assign_na(i);
  } else {
    out.assign_year_month(x, i);
  }
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const char* format,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const r_ssize& i,
                           rclock::gregorian::ymd& out) {
  date::year_month_day x{};
  rclock::from_stream(stream, format, month_names_pair, weekday_names_pair, ampm_names_pair, x);
  if (stream.fail()) {
    out.assign_na(i);
  } else {
    out.assign_year_month_day(x, i);
  }
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const char* format,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const r_ssize& i,
                           rclock::gregorian::ymdh& out) {
  date::year_month_day ymd{};
  date::hh_mm_ss<std::chrono::seconds> hms{};
  rclock::from_stream(stream, format, month_names_pair, weekday_names_pair, ampm_names_pair, ymd, hms);
  if (stream.fail()) {
    out.assign_na(i);
  } else {
    out.assign_year_month_day(ymd, i);
    out.assign_hour(hms.hours(), i);
  }
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const char* format,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const r_ssize& i,
                           rclock::gregorian::ymdhm& out) {
  date::year_month_day ymd{};
  date::hh_mm_ss<std::chrono::seconds> hms{};
  rclock::from_stream(stream, format, month_names_pair, weekday_names_pair, ampm_names_pair, ymd, hms);
  if (stream.fail()) {
    out.assign_na(i);
  } else {
    out.assign_year_month_day(ymd, i);
    out.assign_hour(hms.hours(), i);
    out.assign_minute(hms.minutes(), i);
  }
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const char* format,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const r_ssize& i,
                           rclock::gregorian::ymdhms& out) {
  date::year_month_day ymd{};
  date::hh_mm_ss<std::chrono::seconds> hms{};
  rclock::from_stream(stream, format, month_names_pair, weekday_names_pair, ampm_names_pair, ymd, hms);
  if (stream.fail()) {
    out.assign_na(i);
  } else {
    out.assign_year_month_day(ymd, i);
    out.assign_hour(hms.hours(), i);
    out.assign_minute(hms.minutes(), i);
    out.assign_second(hms.seconds(), i);
  }
}

template <class Calendar>
static
cpp11::writable::list
parse_year_month_day_impl(const cpp11::strings& x,
                          const cpp11::strings& format,
                          const cpp11::strings& mon,
                          const cpp11::strings& mon_ab,
                          const cpp11::strings& day,
                          const cpp11::strings& day_ab,
                          const cpp11::strings& am_pm,
                          const cpp11::strings& mark) {
  const r_ssize size = x.size();
  Calendar out(size);

  if (!r_is_string(format)) {
    clock_abort("`format` must be a single string.");
  }
  const SEXP format_sexp = format[0];
  const char* format_char = CHAR(format_sexp);

  const enum decimal_mark dmark = parse_decimal_mark(mark);
  std::locale loc(std::locale::classic(), new clock_numpunct(dmark));

  std::string month_names[24];
  const std::pair<const std::string*, const std::string*>& month_names_pair = fill_month_names(
    mon,
    mon_ab,
    month_names
  );

  std::string weekday_names[14];
  const std::pair<const std::string*, const std::string*>& weekday_names_pair = fill_weekday_names(
    day,
    day_ab,
    weekday_names
  );

  std::string ampm_names[2];
  const std::pair<const std::string*, const std::string*>& ampm_names_pair = fill_ampm_names(
    am_pm,
    ampm_names
  );

  std::istringstream stream;
  stream.imbue(loc);

  for (r_ssize i = 0; i < size; ++i) {
    const SEXP elt = x[i];

    if (elt == r_chr_na) {
      out.assign_na(i);
      continue;
    }

    const char* elt_char = CHAR(elt);

    stream.clear();
    stream.str(elt_char);

    year_month_day_from_stream(
      stream,
      format_char,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      i,
      out
    );
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
parse_year_month_day_cpp(const cpp11::strings& x,
                         const cpp11::strings& format,
                         const cpp11::integers& precision_int,
                         const cpp11::strings& mon,
                         const cpp11::strings& mon_ab,
                         const cpp11::strings& day,
                         const cpp11::strings& day_ab,
                         const cpp11::strings& am_pm,
                         const cpp11::strings& mark) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return parse_year_month_day_impl<gregorian::y>(x, format, mon, mon_ab, day, day_ab, am_pm, mark);
  case precision::month: return parse_year_month_day_impl<gregorian::ym>(x, format, mon, mon_ab, day, day_ab, am_pm, mark);
  case precision::day: return parse_year_month_day_impl<gregorian::ymd>(x, format, mon, mon_ab, day, day_ab, am_pm, mark);
  case precision::hour: return parse_year_month_day_impl<gregorian::ymdh>(x, format, mon, mon_ab, day, day_ab, am_pm, mark);
  case precision::minute: return parse_year_month_day_impl<gregorian::ymdhm>(x, format, mon, mon_ab, day, day_ab, am_pm, mark);
  case precision::second: return parse_year_month_day_impl<gregorian::ymdhms>(x, format, mon, mon_ab, day, day_ab, am_pm, mark);
  case precision::millisecond: return parse_year_month_day_impl<gregorian::ymdhmss<std::chrono::milliseconds>>(x, format, mon, mon_ab, day, day_ab, am_pm, mark);
  case precision::microsecond: return parse_year_month_day_impl<gregorian::ymdhmss<std::chrono::microseconds>>(x, format, mon, mon_ab, day, day_ab, am_pm, mark);
  case precision::nanosecond: return parse_year_month_day_impl<gregorian::ymdhmss<std::chrono::nanoseconds>>(x, format, mon, mon_ab, day, day_ab, am_pm, mark);
  default: never_reached("parse_year_month_day_cpp");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
gregorian_leap_year_cpp(const cpp11::integers& year) {
  const r_ssize size = year.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    const int elt = year[i];

    if (elt == r_int_na) {
      out[i] = r_lgl_na;
    } else {
      out[i] = date::year{elt}.is_leap();
    }
  }

  return out;
}
