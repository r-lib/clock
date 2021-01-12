#include "gregorian-year-month-weekday.h"
#include "calendar.h"
#include "duration.h"
#include "check.h"
#include "enums.h"
#include "get.h"

[[cpp11::register]]
cpp11::writable::list
collect_year_month_weekday_fields(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers weekday = weekday::get_weekday(fields);
  cpp11::integers weekday_index = weekday::get_weekday_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, weekday, weekday_index};
  weekday::ymwdh ymwdh{year, month, weekday, weekday_index, hour};
  weekday::ymwdhm ymwdhm{year, month, weekday, weekday_index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, weekday, weekday_index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, weekday, weekday_index, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: {
    collect_field<component::year>(y, year, "year");
    return y.to_list();
  }
  case precision2::month: {
    collect_field<component::year>(ym, year, "year");
    collect_field<component::month>(ym, month, "month");
    return ym.to_list();
  }
  case precision2::day: {
    collect_field<component::year>(ymwd, year, "year");
    collect_field<component::month>(ymwd, month, "month");
    collect_field<component::weekday>(ymwd, weekday, "weekday");
    collect_field<component::weekday_index>(ymwd, weekday_index, "weekday_index");
    return ymwd.to_list();
  }
  case precision2::hour: {
    collect_field<component::year>(ymwdh, year, "year");
    collect_field<component::month>(ymwdh, month, "month");
    collect_field<component::weekday>(ymwdh, weekday, "weekday");
    collect_field<component::weekday_index>(ymwdh, weekday_index, "weekday_index");
    collect_field<component::hour>(ymwdh, hour, "hour");
    return ymwdh.to_list();
  }
  case precision2::minute: {
    collect_field<component::year>(ymwdhm, year, "year");
    collect_field<component::month>(ymwdhm, month, "month");
    collect_field<component::weekday>(ymwdhm, weekday, "weekday");
    collect_field<component::weekday_index>(ymwdhm, weekday_index, "weekday_index");
    collect_field<component::hour>(ymwdhm, hour, "hour");
    collect_field<component::minute>(ymwdhm, minute, "minute");
    return ymwdhm.to_list();
  }
  case precision2::second: {
    collect_field<component::year>(ymwdhms, year, "year");
    collect_field<component::month>(ymwdhms, month, "month");
    collect_field<component::weekday>(ymwdhms, weekday, "weekday");
    collect_field<component::weekday_index>(ymwdhms, weekday_index, "weekday_index");
    collect_field<component::hour>(ymwdhms, hour, "hour");
    collect_field<component::minute>(ymwdhms, minute, "minute");
    collect_field<component::second>(ymwdhms, second, "second");
    return ymwdhms.to_list();
  }
  case precision2::millisecond: {
    collect_field<component::year>(ymwdhmss1, year, "year");
    collect_field<component::month>(ymwdhmss1, month, "month");
    collect_field<component::weekday>(ymwdhmss1, weekday, "weekday");
    collect_field<component::weekday_index>(ymwdhmss1, weekday_index, "weekday_index");
    collect_field<component::hour>(ymwdhmss1, hour, "hour");
    collect_field<component::minute>(ymwdhmss1, minute, "minute");
    collect_field<component::second>(ymwdhmss1, second, "second");
    collect_field<component::millisecond>(ymwdhmss1, subsecond, "subsecond");
    return ymwdhmss1.to_list();
  }
  case precision2::microsecond: {
    collect_field<component::year>(ymwdhmss2, year, "year");
    collect_field<component::month>(ymwdhmss2, month, "month");
    collect_field<component::weekday>(ymwdhmss2, weekday, "weekday");
    collect_field<component::weekday_index>(ymwdhmss2, weekday_index, "weekday_index");
    collect_field<component::hour>(ymwdhmss2, hour, "hour");
    collect_field<component::minute>(ymwdhmss2, minute, "minute");
    collect_field<component::second>(ymwdhmss2, second, "second");
    collect_field<component::microsecond>(ymwdhmss2, subsecond, "subsecond");
    return ymwdhmss2.to_list();
  }
  case precision2::nanosecond: {
    collect_field<component::year>(ymwdhmss3, year, "year");
    collect_field<component::month>(ymwdhmss3, month, "month");
    collect_field<component::weekday>(ymwdhmss3, weekday, "weekday");
    collect_field<component::weekday_index>(ymwdhmss3, weekday_index, "weekday_index");
    collect_field<component::hour>(ymwdhmss3, hour, "hour");
    collect_field<component::minute>(ymwdhmss3, minute, "minute");
    collect_field<component::second>(ymwdhmss3, second, "second");
    collect_field<component::nanosecond>(ymwdhmss3, subsecond, "subsecond");
    return ymwdhmss3.to_list();
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
format_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields, const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers weekday = weekday::get_weekday(fields);
  cpp11::integers weekday_index = weekday::get_weekday_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, weekday, weekday_index};
  weekday::ymwdh ymwdh{year, month, weekday, weekday_index, hour};
  weekday::ymwdhm ymwdhm{year, month, weekday, weekday_index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, weekday, weekday_index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, weekday, weekday_index, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: return format_calendar_impl(y);
  case precision2::month: return format_calendar_impl(ym);
  case precision2::day: return format_calendar_impl(ymwd);
  case precision2::hour: return format_calendar_impl(ymwdh);
  case precision2::minute: return format_calendar_impl(ymwdhm);
  case precision2::second: return format_calendar_impl(ymwdhms);
  case precision2::millisecond: return format_calendar_impl(ymwdhmss1);
  case precision2::microsecond: return format_calendar_impl(ymwdhmss2);
  case precision2::nanosecond: return format_calendar_impl(ymwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields, const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers weekday = weekday::get_weekday(fields);
  cpp11::integers weekday_index = weekday::get_weekday_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, weekday, weekday_index};
  weekday::ymwdh ymwdh{year, month, weekday, weekday_index, hour};
  weekday::ymwdhm ymwdhm{year, month, weekday, weekday_index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, weekday, weekday_index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, weekday, weekday_index, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_detect_calendar_impl(y);
  case precision2::month: return invalid_detect_calendar_impl(ym);
  case precision2::day: return invalid_detect_calendar_impl(ymwd);
  case precision2::hour: return invalid_detect_calendar_impl(ymwdh);
  case precision2::minute: return invalid_detect_calendar_impl(ymwdhm);
  case precision2::second: return invalid_detect_calendar_impl(ymwdhms);
  case precision2::millisecond: return invalid_detect_calendar_impl(ymwdhmss1);
  case precision2::microsecond: return invalid_detect_calendar_impl(ymwdhmss2);
  case precision2::nanosecond: return invalid_detect_calendar_impl(ymwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields, cpp11::strings precision) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers weekday = weekday::get_weekday(fields);
  cpp11::integers weekday_index = weekday::get_weekday_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, weekday, weekday_index};
  weekday::ymwdh ymwdh{year, month, weekday, weekday_index, hour};
  weekday::ymwdhm ymwdhm{year, month, weekday, weekday_index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, weekday, weekday_index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, weekday, weekday_index, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_any_calendar_impl(y);
  case precision2::month: return invalid_any_calendar_impl(ym);
  case precision2::day: return invalid_any_calendar_impl(ymwd);
  case precision2::hour: return invalid_any_calendar_impl(ymwdh);
  case precision2::minute: return invalid_any_calendar_impl(ymwdhm);
  case precision2::second: return invalid_any_calendar_impl(ymwdhms);
  case precision2::millisecond: return invalid_any_calendar_impl(ymwdhmss1);
  case precision2::microsecond: return invalid_any_calendar_impl(ymwdhmss2);
  case precision2::nanosecond: return invalid_any_calendar_impl(ymwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields, const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers weekday = weekday::get_weekday(fields);
  cpp11::integers weekday_index = weekday::get_weekday_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, weekday, weekday_index};
  weekday::ymwdh ymwdh{year, month, weekday, weekday_index, hour};
  weekday::ymwdhm ymwdhm{year, month, weekday, weekday_index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, weekday, weekday_index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, weekday, weekday_index, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_count_calendar_impl(y);
  case precision2::month: return invalid_count_calendar_impl(ym);
  case precision2::day: return invalid_count_calendar_impl(ymwd);
  case precision2::hour: return invalid_count_calendar_impl(ymwdh);
  case precision2::minute: return invalid_count_calendar_impl(ymwdhm);
  case precision2::second: return invalid_count_calendar_impl(ymwdhms);
  case precision2::millisecond: return invalid_count_calendar_impl(ymwdhmss1);
  case precision2::microsecond: return invalid_count_calendar_impl(ymwdhmss2);
  case precision2::nanosecond: return invalid_count_calendar_impl(ymwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                       const cpp11::strings& precision,
                                       const cpp11::strings& invalid) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid);

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers weekday = weekday::get_weekday(fields);
  cpp11::integers weekday_index = weekday::get_weekday_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, weekday, weekday_index};
  weekday::ymwdh ymwdh{year, month, weekday, weekday_index, hour};
  weekday::ymwdhm ymwdhm{year, month, weekday, weekday_index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, weekday, weekday_index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, weekday, weekday_index, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_resolve_calendar_impl(y, invalid_val);
  case precision2::month: return invalid_resolve_calendar_impl(ym, invalid_val);
  case precision2::day: return invalid_resolve_calendar_impl(ymwd, invalid_val);
  case precision2::hour: return invalid_resolve_calendar_impl(ymwdh, invalid_val);
  case precision2::minute: return invalid_resolve_calendar_impl(ymwdhm, invalid_val);
  case precision2::second: return invalid_resolve_calendar_impl(ymwdhms, invalid_val);
  case precision2::millisecond: return invalid_resolve_calendar_impl(ymwdhmss1, invalid_val);
  case precision2::microsecond: return invalid_resolve_calendar_impl(ymwdhmss2, invalid_val);
  case precision2::nanosecond: return invalid_resolve_calendar_impl(ymwdhmss3, invalid_val);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
set_field_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::integers& value,
                                 const cpp11::strings& precision_fields,
                                 const cpp11::strings& component) {
  using namespace rclock;
  rclock::integers value2(value);

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers weekday = weekday::get_weekday(fields);
  cpp11::integers weekday_index = weekday::get_weekday_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, weekday, weekday_index};
  weekday::ymwdh ymwdh{year, month, weekday, weekday_index, hour};
  weekday::ymwdhm ymwdhm{year, month, weekday, weekday_index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, weekday, weekday_index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, weekday, weekday_index, hour, minute, second, subsecond};

  switch (parse_precision2(precision_fields)) {
  case precision2::year: {
    switch (parse_component(component)) {
    case component::year: return set_field_calendar<component::year>(y, value2);
    case component::month: return set_field_calendar<component::month>(y, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision2::month: {
    switch (parse_component(component)) {
    case component::year: return set_field_calendar<component::year>(ym, value2);
    case component::month: return set_field_calendar<component::month>(ym, value2);
    // Note: month precision will be promoted up to day before setting weekday/index
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision2::day: {
    switch (parse_component(component)) {
    case component::year: return set_field_calendar<component::year>(ymwd, value2);
    case component::month: return set_field_calendar<component::month>(ymwd, value2);
    case component::weekday: return set_field_calendar<component::weekday>(ymwd, value2);
    case component::weekday_index: return set_field_calendar<component::weekday_index>(ymwd, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwd, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision2::hour: {
    switch (parse_component(component)) {
    case component::year: return set_field_calendar<component::year>(ymwdh, value2);
    case component::month: return set_field_calendar<component::month>(ymwdh, value2);
    case component::weekday: return set_field_calendar<component::weekday>(ymwdh, value2);
    case component::weekday_index: return set_field_calendar<component::weekday_index>(ymwdh, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdh, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdh, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision2::minute: {
    switch (parse_component(component)) {
    case component::year: return set_field_calendar<component::year>(ymwdhm, value2);
    case component::month: return set_field_calendar<component::month>(ymwdhm, value2);
    case component::weekday: return set_field_calendar<component::weekday>(ymwdhm, value2);
    case component::weekday_index: return set_field_calendar<component::weekday_index>(ymwdhm, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdhm, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdhm, value2);
    case component::second: return set_field_calendar<component::second>(ymwdhm, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision2::second: {
    switch (parse_component(component)) {
    case component::year: return set_field_calendar<component::year>(ymwdhms, value2);
    case component::month: return set_field_calendar<component::month>(ymwdhms, value2);
    case component::weekday: return set_field_calendar<component::weekday>(ymwdhms, value2);
    case component::weekday_index: return set_field_calendar<component::weekday_index>(ymwdhms, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdhms, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdhms, value2);
    case component::second: return set_field_calendar<component::second>(ymwdhms, value2);
    case component::millisecond: return set_field_calendar<component::millisecond>(ymwdhms, value2);
    case component::microsecond: return set_field_calendar<component::microsecond>(ymwdhms, value2);
    case component::nanosecond: return set_field_calendar<component::nanosecond>(ymwdhms, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision2::millisecond: {
    switch (parse_component(component)) {
    case component::year: return set_field_calendar<component::year>(ymwdhmss1, value2);
    case component::month: return set_field_calendar<component::month>(ymwdhmss1, value2);
    case component::weekday: return set_field_calendar<component::weekday>(ymwdhmss1, value2);
    case component::weekday_index: return set_field_calendar<component::weekday_index>(ymwdhmss1, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdhmss1, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdhmss1, value2);
    case component::second: return set_field_calendar<component::second>(ymwdhmss1, value2);
    case component::millisecond: return set_field_calendar<component::millisecond>(ymwdhmss1, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision2::microsecond: {
    switch (parse_component(component)) {
    case component::year: return set_field_calendar<component::year>(ymwdhmss2, value2);
    case component::month: return set_field_calendar<component::month>(ymwdhmss2, value2);
    case component::weekday: return set_field_calendar<component::weekday>(ymwdhmss2, value2);
    case component::weekday_index: return set_field_calendar<component::weekday_index>(ymwdhmss2, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdhmss2, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdhmss2, value2);
    case component::second: return set_field_calendar<component::second>(ymwdhmss2, value2);
    case component::microsecond: return set_field_calendar<component::microsecond>(ymwdhmss2, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision2::nanosecond: {
    switch (parse_component(component)) {
    case component::year: return set_field_calendar<component::year>(ymwdhmss3, value2);
    case component::month: return set_field_calendar<component::month>(ymwdhmss3, value2);
    case component::weekday: return set_field_calendar<component::weekday>(ymwdhmss3, value2);
    case component::weekday_index: return set_field_calendar<component::weekday_index>(ymwdhmss3, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdhmss3, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdhmss3, value2);
    case component::second: return set_field_calendar<component::second>(ymwdhmss3, value2);
    case component::nanosecond: return set_field_calendar<component::nanosecond>(ymwdhmss3, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

template <class Calendar>
cpp11::writable::list
set_field_year_month_weekday_last_impl(const Calendar& x) {
  const r_ssize size = x.size();
  cpp11::writable::integers day(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      day[i] = r_int_na;
    } else {
      // We require day precision to set the index to last, so this is allowed
      date::year_month_weekday elt = x.to_year_month_weekday(i);
      date::year_month_weekday ymwd{elt.year() / elt.month() / elt.weekday()[date::last]};
      day[i] = static_cast<int>(static_cast<unsigned>(ymwd.index()));
    }
  }

  cpp11::writable::list out({x.to_list(), day});
  out.names() = {"fields", "value"};

  return out;
}

[[cpp11::register]]
cpp11::writable::list
set_field_year_month_weekday_last_cpp(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::strings& precision_fields) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers weekday = weekday::get_weekday(fields);
  cpp11::integers weekday_index = weekday::get_weekday_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::ymwd ymwd{year, month, weekday, weekday_index};
  weekday::ymwdh ymwdh{year, month, weekday, weekday_index, hour};
  weekday::ymwdhm ymwdhm{year, month, weekday, weekday_index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, weekday, weekday_index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, weekday, weekday_index, hour, minute, second, subsecond};

  switch (parse_precision2(precision_fields)) {
  case precision2::day: return set_field_year_month_weekday_last_impl(ymwd);
  case precision2::hour: return set_field_year_month_weekday_last_impl(ymwdh);
  case precision2::minute: return set_field_year_month_weekday_last_impl(ymwdhm);
  case precision2::second: return set_field_year_month_weekday_last_impl(ymwdhms);
  case precision2::millisecond: return set_field_year_month_weekday_last_impl(ymwdhmss1);
  case precision2::microsecond: return set_field_year_month_weekday_last_impl(ymwdhmss2);
  case precision2::nanosecond: return set_field_year_month_weekday_last_impl(ymwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
year_month_weekday_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                     cpp11::list_of<cpp11::integers> fields_n,
                                     const cpp11::strings& precision_fields,
                                     const cpp11::strings& precision_n) {
  using namespace rclock;

  const enum precision2 precision_fields_val = parse_precision2(precision_fields);
  const enum precision2 precision_n_val = parse_precision2(precision_n);

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers weekday = weekday::get_weekday(fields);
  cpp11::integers weekday_index = weekday::get_weekday_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, weekday, weekday_index};
  weekday::ymwdh ymwdh{year, month, weekday, weekday_index, hour};
  weekday::ymwdhm ymwdhm{year, month, weekday, weekday_index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, weekday, weekday_index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, weekday, weekday_index, hour, minute, second, subsecond};

  cpp11::integers ticks = duration::get_ticks(fields_n);

  duration::years dy{ticks};
  duration::quarters dq{ticks};
  duration::months dm{ticks};

  switch (precision_fields_val) {
  case precision2::year:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(y, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::month:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ym, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ym, dq);
    case precision2::month: return calendar_plus_duration_impl(ym, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::day:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymwd, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymwd, dq);
    case precision2::month: return calendar_plus_duration_impl(ymwd, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::hour:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymwdh, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymwdh, dq);
    case precision2::month: return calendar_plus_duration_impl(ymwdh, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::minute:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymwdhm, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymwdhm, dq);
    case precision2::month: return calendar_plus_duration_impl(ymwdhm, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::second:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymwdhms, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymwdhms, dq);
    case precision2::month: return calendar_plus_duration_impl(ymwdhms, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::millisecond:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymwdhmss1, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymwdhmss1, dq);
    case precision2::month: return calendar_plus_duration_impl(ymwdhmss1, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::microsecond:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymwdhmss2, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymwdhmss2, dq);
    case precision2::month: return calendar_plus_duration_impl(ymwdhmss2, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::nanosecond:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymwdhmss3, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymwdhmss3, dq);
    case precision2::month: return calendar_plus_duration_impl(ymwdhmss3, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers weekday = weekday::get_weekday(fields);
  cpp11::integers weekday_index = weekday::get_weekday_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::ymwd ymwd{year, month, weekday, weekday_index};
  weekday::ymwdh ymwdh{year, month, weekday, weekday_index, hour};
  weekday::ymwdhm ymwdhm{year, month, weekday, weekday_index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, weekday, weekday_index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, weekday, weekday_index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, weekday, weekday_index, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::day: return as_sys_time_from_calendar_impl<duration::days>(ymwd);
  case precision2::hour: return as_sys_time_from_calendar_impl<duration::hours>(ymwdh);
  case precision2::minute: return as_sys_time_from_calendar_impl<duration::minutes>(ymwdhm);
  case precision2::second: return as_sys_time_from_calendar_impl<duration::seconds>(ymwdhms);
  case precision2::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(ymwdhmss1);
  case precision2::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(ymwdhmss2);
  case precision2::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(ymwdhmss3);
  default: {
    std::string precision_string = precision[0];
    std::string message =
      "Can't convert to a time point from a calendar with '" +
      precision_string +
      "' precision. " +
      "A minimum of 'day' precision is required.";
    clock_abort(message.c_str());
  }
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_year_month_weekday_from_sys_time_cpp(cpp11::list_of<cpp11::integers> fields,
                                        const cpp11::strings& precision) {
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

  switch (parse_precision2(precision)) {
  case precision2::day: return as_calendar_from_sys_time_impl<weekday::ymwd>(dd);
  case precision2::hour: return as_calendar_from_sys_time_impl<weekday::ymwdh>(dh);
  case precision2::minute: return as_calendar_from_sys_time_impl<weekday::ymwdhm>(dmin);
  case precision2::second: return as_calendar_from_sys_time_impl<weekday::ymwdhms>(ds);
  case precision2::millisecond: return as_calendar_from_sys_time_impl<weekday::ymwdhmss<std::chrono::milliseconds>>(dmilli);
  case precision2::microsecond: return as_calendar_from_sys_time_impl<weekday::ymwdhmss<std::chrono::microseconds>>(dmicro);
  case precision2::nanosecond: return as_calendar_from_sys_time_impl<weekday::ymwdhmss<std::chrono::nanoseconds>>(dnano);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

static
inline
cpp11::writable::list
year_minus_year_impl(const rclock::weekday::y& x,
                     const rclock::weekday::y& y) {
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
year_month_minus_year_month_impl(const rclock::weekday::ym& x,
                                 const rclock::weekday::ym& y) {
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
year_month_weekday_minus_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> x,
                                                cpp11::list_of<cpp11::integers> y,
                                                const cpp11::strings& precision) {
  const cpp11::integers x_year = rclock::weekday::get_year(x);
  const cpp11::integers x_month = rclock::weekday::get_month(x);

  const cpp11::integers y_year = rclock::weekday::get_year(y);
  const cpp11::integers y_month = rclock::weekday::get_month(y);

  const rclock::weekday::y x_y{x_year};
  const rclock::weekday::ym x_ym{x_year, x_month};

  const rclock::weekday::y y_y{y_year};
  const rclock::weekday::ym y_ym{y_year, y_month};

  switch (parse_precision2(precision)) {
  case precision2::year: return year_minus_year_impl(x_y, y_y);
  case precision2::month: return year_month_minus_year_month_impl(x_ym, y_ym);
  default: clock_abort("Internal error: Invalid precision.");
  }
}
