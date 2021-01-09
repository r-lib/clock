#include "gregorian-year-month-day.h"
#include "calendar.h"
#include "duration.h"
#include "check.h"
#include "enums.h"
#include "get.h"

[[cpp11::register]]
cpp11::writable::list
collect_year_month_day_fields(cpp11::list_of<cpp11::integers> fields,
                              const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = gregorian::get_year(fields);
  cpp11::integers month = gregorian::get_month(fields);
  cpp11::integers day = gregorian::get_day(fields);
  cpp11::integers hour = gregorian::get_hour(fields);
  cpp11::integers minute = gregorian::get_minute(fields);
  cpp11::integers second = gregorian::get_second(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};

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
    collect_field<component::year>(ymd, year, "year");
    collect_field<component::month>(ymd, month, "month");
    collect_field<component::day>(ymd, day, "day");
    return ymd.to_list();
  }
  case precision2::hour: {
    collect_field<component::year>(ymdh, year, "year");
    collect_field<component::month>(ymdh, month, "month");
    collect_field<component::day>(ymdh, day, "day");
    collect_field<component::hour>(ymdh, hour, "hour");
    return ymdh.to_list();
  }
  case precision2::minute: {
    collect_field<component::year>(ymdhm, year, "year");
    collect_field<component::month>(ymdhm, month, "month");
    collect_field<component::day>(ymdhm, day, "day");
    collect_field<component::hour>(ymdhm, hour, "hour");
    collect_field<component::minute>(ymdhm, minute, "minute");
    return ymdhm.to_list();
  }
  case precision2::second: {
    collect_field<component::year>(ymdhms, year, "year");
    collect_field<component::month>(ymdhms, month, "month");
    collect_field<component::day>(ymdhms, day, "day");
    collect_field<component::hour>(ymdhms, hour, "hour");
    collect_field<component::minute>(ymdhms, minute, "minute");
    collect_field<component::second>(ymdhms, second, "second");
    return ymdhms.to_list();
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
format_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                          const cpp11::strings& precision) {
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

  switch (parse_precision2(precision)) {
  case precision2::year: return format_calendar_impl(y);
  case precision2::month: return format_calendar_impl(ym);
  case precision2::day: return format_calendar_impl(ymd);
  case precision2::hour: return format_calendar_impl(ymdh);
  case precision2::minute: return format_calendar_impl(ymdhm);
  case precision2::second: return format_calendar_impl(ymdhms);
  case precision2::millisecond: return format_calendar_impl(ymdhmss1);
  case precision2::microsecond: return format_calendar_impl(ymdhmss2);
  case precision2::nanosecond: return format_calendar_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::strings& precision) {
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

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_detect_calendar_impl(y);
  case precision2::month: return invalid_detect_calendar_impl(ym);
  case precision2::day: return invalid_detect_calendar_impl(ymd);
  case precision2::hour: return invalid_detect_calendar_impl(ymdh);
  case precision2::minute: return invalid_detect_calendar_impl(ymdhm);
  case precision2::second: return invalid_detect_calendar_impl(ymdhms);
  case precision2::millisecond: return invalid_detect_calendar_impl(ymdhmss1);
  case precision2::microsecond: return invalid_detect_calendar_impl(ymdhmss2);
  case precision2::nanosecond: return invalid_detect_calendar_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields, cpp11::strings precision) {
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

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_any_calendar_impl(y);
  case precision2::month: return invalid_any_calendar_impl(ym);
  case precision2::day: return invalid_any_calendar_impl(ymd);
  case precision2::hour: return invalid_any_calendar_impl(ymdh);
  case precision2::minute: return invalid_any_calendar_impl(ymdhm);
  case precision2::second: return invalid_any_calendar_impl(ymdhms);
  case precision2::millisecond: return invalid_any_calendar_impl(ymdhmss1);
  case precision2::microsecond: return invalid_any_calendar_impl(ymdhmss2);
  case precision2::nanosecond: return invalid_any_calendar_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::strings& precision) {
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

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_count_calendar_impl(y);
  case precision2::month: return invalid_count_calendar_impl(ym);
  case precision2::day: return invalid_count_calendar_impl(ymd);
  case precision2::hour: return invalid_count_calendar_impl(ymdh);
  case precision2::minute: return invalid_count_calendar_impl(ymdhm);
  case precision2::second: return invalid_count_calendar_impl(ymdhms);
  case precision2::millisecond: return invalid_count_calendar_impl(ymdhmss1);
  case precision2::microsecond: return invalid_count_calendar_impl(ymdhmss2);
  case precision2::nanosecond: return invalid_count_calendar_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                   const cpp11::strings& precision,
                                   const cpp11::strings& invalid) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid);

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

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_resolve_calendar_impl(y, invalid_val);
  case precision2::month: return invalid_resolve_calendar_impl(ym, invalid_val);
  case precision2::day: return invalid_resolve_calendar_impl(ymd, invalid_val);
  case precision2::hour: return invalid_resolve_calendar_impl(ymdh, invalid_val);
  case precision2::minute: return invalid_resolve_calendar_impl(ymdhm, invalid_val);
  case precision2::second: return invalid_resolve_calendar_impl(ymdhms, invalid_val);
  case precision2::millisecond: return invalid_resolve_calendar_impl(ymdhmss1, invalid_val);
  case precision2::microsecond: return invalid_resolve_calendar_impl(ymdhmss2, invalid_val);
  case precision2::nanosecond: return invalid_resolve_calendar_impl(ymdhmss3, invalid_val);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
set_field_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                             const cpp11::integers& value,
                             const cpp11::strings& precision_fields,
                             const cpp11::strings& precision_value) {
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

  switch (parse_precision2(precision_fields)) {
  case precision2::year: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<component::year>(y, value2);
    case precision2::month: return set_field_calendar<component::month>(y, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::month: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<component::year>(ym, value2);
    case precision2::month: return set_field_calendar<component::month>(ym, value2);
    case precision2::day: return set_field_calendar<component::day>(ym, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::day: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<component::year>(ymd, value2);
    case precision2::month: return set_field_calendar<component::month>(ymd, value2);
    case precision2::day: return set_field_calendar<component::day>(ymd, value2);
    case precision2::hour: return set_field_calendar<component::hour>(ymd, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::hour: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<component::year>(ymdh, value2);
    case precision2::month: return set_field_calendar<component::month>(ymdh, value2);
    case precision2::day: return set_field_calendar<component::day>(ymdh, value2);
    case precision2::hour: return set_field_calendar<component::hour>(ymdh, value2);
    case precision2::minute: return set_field_calendar<component::minute>(ymdh, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::minute: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<component::year>(ymdhm, value2);
    case precision2::month: return set_field_calendar<component::month>(ymdhm, value2);
    case precision2::day: return set_field_calendar<component::day>(ymdhm, value2);
    case precision2::hour: return set_field_calendar<component::hour>(ymdhm, value2);
    case precision2::minute: return set_field_calendar<component::minute>(ymdhm, value2);
    case precision2::second: return set_field_calendar<component::second>(ymdhm, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::second: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<component::year>(ymdhms, value2);
    case precision2::month: return set_field_calendar<component::month>(ymdhms, value2);
    case precision2::day: return set_field_calendar<component::day>(ymdhms, value2);
    case precision2::hour: return set_field_calendar<component::hour>(ymdhms, value2);
    case precision2::minute: return set_field_calendar<component::minute>(ymdhms, value2);
    case precision2::second: return set_field_calendar<component::second>(ymdhms, value2);
    case precision2::millisecond: return set_field_calendar<component::millisecond>(ymdhms, value2);
    case precision2::microsecond: return set_field_calendar<component::microsecond>(ymdhms, value2);
    case precision2::nanosecond: return set_field_calendar<component::nanosecond>(ymdhms, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::millisecond: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<component::year>(ymdhmss1, value2);
    case precision2::month: return set_field_calendar<component::month>(ymdhmss1, value2);
    case precision2::day: return set_field_calendar<component::day>(ymdhmss1, value2);
    case precision2::hour: return set_field_calendar<component::hour>(ymdhmss1, value2);
    case precision2::minute: return set_field_calendar<component::minute>(ymdhmss1, value2);
    case precision2::second: return set_field_calendar<component::second>(ymdhmss1, value2);
    case precision2::millisecond: return set_field_calendar<component::millisecond>(ymdhmss1, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::microsecond: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<component::year>(ymdhmss2, value2);
    case precision2::month: return set_field_calendar<component::month>(ymdhmss2, value2);
    case precision2::day: return set_field_calendar<component::day>(ymdhmss2, value2);
    case precision2::hour: return set_field_calendar<component::hour>(ymdhmss2, value2);
    case precision2::minute: return set_field_calendar<component::minute>(ymdhmss2, value2);
    case precision2::second: return set_field_calendar<component::second>(ymdhmss2, value2);
    case precision2::microsecond: return set_field_calendar<component::microsecond>(ymdhmss2, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::nanosecond: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<component::year>(ymdhmss3, value2);
    case precision2::month: return set_field_calendar<component::month>(ymdhmss3, value2);
    case precision2::day: return set_field_calendar<component::day>(ymdhmss3, value2);
    case precision2::hour: return set_field_calendar<component::hour>(ymdhmss3, value2);
    case precision2::minute: return set_field_calendar<component::minute>(ymdhmss3, value2);
    case precision2::second: return set_field_calendar<component::second>(ymdhmss3, value2);
    case precision2::nanosecond: return set_field_calendar<component::nanosecond>(ymdhmss3, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
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
                                  const cpp11::strings& precision_fields) {
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

  switch (parse_precision2(precision_fields)) {
  case precision2::month: return set_field_year_month_day_last_impl(ym);
  case precision2::day: return set_field_year_month_day_last_impl(ymd);
  case precision2::hour: return set_field_year_month_day_last_impl(ymdh);
  case precision2::minute: return set_field_year_month_day_last_impl(ymdhm);
  case precision2::second: return set_field_year_month_day_last_impl(ymdhms);
  case precision2::millisecond: return set_field_year_month_day_last_impl(ymdhmss1);
  case precision2::microsecond: return set_field_year_month_day_last_impl(ymdhmss2);
  case precision2::nanosecond: return set_field_year_month_day_last_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
year_month_day_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                 cpp11::list_of<cpp11::integers> fields_n,
                                 const cpp11::strings& precision_fields,
                                 const cpp11::strings& precision_n) {
  using namespace rclock;

  const enum precision2 precision_fields_val = parse_precision2(precision_fields);
  const enum precision2 precision_n_val = parse_precision2(precision_n);

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
    case precision2::year: return calendar_plus_duration_impl(ymd, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymd, dq);
    case precision2::month: return calendar_plus_duration_impl(ymd, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::hour:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymdh, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymdh, dq);
    case precision2::month: return calendar_plus_duration_impl(ymdh, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::minute:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymdhm, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymdhm, dq);
    case precision2::month: return calendar_plus_duration_impl(ymdhm, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::second:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymdhms, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymdhms, dq);
    case precision2::month: return calendar_plus_duration_impl(ymdhms, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::millisecond:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymdhmss1, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymdhmss1, dq);
    case precision2::month: return calendar_plus_duration_impl(ymdhmss1, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::microsecond:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymdhmss2, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymdhmss2, dq);
    case precision2::month: return calendar_plus_duration_impl(ymdhmss2, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::nanosecond:
    switch (precision_n_val) {
    case precision2::year: return calendar_plus_duration_impl(ymdhmss3, dy);
    case precision2::quarter: return calendar_plus_duration_impl(ymdhmss3, dq);
    case precision2::month: return calendar_plus_duration_impl(ymdhmss3, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::strings& precision) {
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

  switch (parse_precision2(precision)) {
  case precision2::day: return as_sys_time_from_calendar_impl<duration::days>(ymd);
  case precision2::hour: return as_sys_time_from_calendar_impl<duration::hours>(ymdh);
  case precision2::minute: return as_sys_time_from_calendar_impl<duration::minutes>(ymdhm);
  case precision2::second: return as_sys_time_from_calendar_impl<duration::seconds>(ymdhms);
  case precision2::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(ymdhmss1);
  case precision2::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(ymdhmss2);
  case precision2::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(ymdhmss3);
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
as_year_month_day_from_sys_time_cpp(cpp11::list_of<cpp11::integers> fields,
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
  case precision2::day: return as_calendar_from_sys_time_impl<gregorian::ymd>(dd);
  case precision2::hour: return as_calendar_from_sys_time_impl<gregorian::ymdh>(dh);
  case precision2::minute: return as_calendar_from_sys_time_impl<gregorian::ymdhm>(dmin);
  case precision2::second: return as_calendar_from_sys_time_impl<gregorian::ymdhms>(ds);
  case precision2::millisecond: return as_calendar_from_sys_time_impl<gregorian::ymdhmss<std::chrono::milliseconds>>(dmilli);
  case precision2::microsecond: return as_calendar_from_sys_time_impl<gregorian::ymdhmss<std::chrono::microseconds>>(dmicro);
  case precision2::nanosecond: return as_calendar_from_sys_time_impl<gregorian::ymdhmss<std::chrono::nanoseconds>>(dnano);
  default: clock_abort("Internal error: Invalid precision.");
  }
}
