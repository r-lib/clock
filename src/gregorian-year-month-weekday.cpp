#include "gregorian-year-month-weekday.h"
#include "calendar.h"
#include "duration.h"
#include "check.h"
#include "enums.h"
#include "get.h"
#include "rcrd.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
new_year_month_weekday_from_fields(SEXP fields,
                                   const cpp11::integers& precision_int,
                                   SEXP names) {
  const enum precision precision_val = parse_precision(precision_int);

  const r_ssize n_fields = Rf_xlength(fields);

  r_ssize n;
  switch (precision_val) {
  case precision::year: n = 1; break;
  case precision::month: n = 2; break;
  case precision::day: n = 4; break;
  case precision::hour: n = 5; break;
  case precision::minute: n = 6; break;
  case precision::second: n = 7; break;
  case precision::millisecond: n = 8; break;
  case precision::microsecond: n = 8; break;
  case precision::nanosecond: n = 8; break;
  default: never_reached("new_year_month_weekday_from_fields");
  }

  if (n != n_fields) {
    clock_abort("With the given precision, `fields` must have length %i, not %i.", n, n_fields);
  }

  SEXP out = PROTECT(new_clock_rcrd_from_fields(fields, names, classes_year_month_weekday));

  Rf_setAttrib(out, syms_precision, precision_int);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
year_month_weekday_restore(SEXP x, SEXP to) {
  SEXP precision = Rf_getAttrib(to, syms_precision);

  SEXP out = PROTECT(clock_rcrd_restore(x, to, classes_year_month_weekday));

  Rf_setAttrib(out, syms_precision, precision);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
collect_year_month_weekday_fields(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers day = weekday::get_day(fields);
  cpp11::integers index = weekday::get_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

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
    collect_field<component::year>(ymwd, year, "year");
    collect_field<component::month>(ymwd, month, "month");
    collect_field<component::day>(ymwd, day, "day");
    collect_field<component::index>(ymwd, index, "index");
    return ymwd.to_list();
  }
  case precision::hour: {
    collect_field<component::year>(ymwdh, year, "year");
    collect_field<component::month>(ymwdh, month, "month");
    collect_field<component::day>(ymwdh, day, "day");
    collect_field<component::index>(ymwdh, index, "index");
    collect_field<component::hour>(ymwdh, hour, "hour");
    return ymwdh.to_list();
  }
  case precision::minute: {
    collect_field<component::year>(ymwdhm, year, "year");
    collect_field<component::month>(ymwdhm, month, "month");
    collect_field<component::day>(ymwdhm, day, "day");
    collect_field<component::index>(ymwdhm, index, "index");
    collect_field<component::hour>(ymwdhm, hour, "hour");
    collect_field<component::minute>(ymwdhm, minute, "minute");
    return ymwdhm.to_list();
  }
  case precision::second: {
    collect_field<component::year>(ymwdhms, year, "year");
    collect_field<component::month>(ymwdhms, month, "month");
    collect_field<component::day>(ymwdhms, day, "day");
    collect_field<component::index>(ymwdhms, index, "index");
    collect_field<component::hour>(ymwdhms, hour, "hour");
    collect_field<component::minute>(ymwdhms, minute, "minute");
    collect_field<component::second>(ymwdhms, second, "second");
    return ymwdhms.to_list();
  }
  case precision::millisecond: {
    collect_field<component::year>(ymwdhmss1, year, "year");
    collect_field<component::month>(ymwdhmss1, month, "month");
    collect_field<component::day>(ymwdhmss1, day, "day");
    collect_field<component::index>(ymwdhmss1, index, "index");
    collect_field<component::hour>(ymwdhmss1, hour, "hour");
    collect_field<component::minute>(ymwdhmss1, minute, "minute");
    collect_field<component::second>(ymwdhmss1, second, "second");
    collect_field<component::millisecond>(ymwdhmss1, subsecond, "subsecond");
    return ymwdhmss1.to_list();
  }
  case precision::microsecond: {
    collect_field<component::year>(ymwdhmss2, year, "year");
    collect_field<component::month>(ymwdhmss2, month, "month");
    collect_field<component::day>(ymwdhmss2, day, "day");
    collect_field<component::index>(ymwdhmss2, index, "index");
    collect_field<component::hour>(ymwdhmss2, hour, "hour");
    collect_field<component::minute>(ymwdhmss2, minute, "minute");
    collect_field<component::second>(ymwdhmss2, second, "second");
    collect_field<component::microsecond>(ymwdhmss2, subsecond, "subsecond");
    return ymwdhmss2.to_list();
  }
  case precision::nanosecond: {
    collect_field<component::year>(ymwdhmss3, year, "year");
    collect_field<component::month>(ymwdhmss3, month, "month");
    collect_field<component::day>(ymwdhmss3, day, "day");
    collect_field<component::index>(ymwdhmss3, index, "index");
    collect_field<component::hour>(ymwdhmss3, hour, "hour");
    collect_field<component::minute>(ymwdhmss3, minute, "minute");
    collect_field<component::second>(ymwdhmss3, second, "second");
    collect_field<component::nanosecond>(ymwdhmss3, subsecond, "subsecond");
    return ymwdhmss3.to_list();
  }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("collect_year_month_weekday_fields");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
format_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                              const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers day = weekday::get_day(fields);
  cpp11::integers index = weekday::get_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return format_calendar_impl(y);
  case precision::month: return format_calendar_impl(ym);
  case precision::day: return format_calendar_impl(ymwd);
  case precision::hour: return format_calendar_impl(ymwdh);
  case precision::minute: return format_calendar_impl(ymwdhm);
  case precision::second: return format_calendar_impl(ymwdhms);
  case precision::millisecond: return format_calendar_impl(ymwdhmss1);
  case precision::microsecond: return format_calendar_impl(ymwdhmss2);
  case precision::nanosecond: return format_calendar_impl(ymwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("format_year_month_weekday_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                      const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers day = weekday::get_day(fields);
  cpp11::integers index = weekday::get_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_detect_calendar_impl(y);
  case precision::month: return invalid_detect_calendar_impl(ym);
  case precision::day: return invalid_detect_calendar_impl(ymwd);
  case precision::hour: return invalid_detect_calendar_impl(ymwdh);
  case precision::minute: return invalid_detect_calendar_impl(ymwdhm);
  case precision::second: return invalid_detect_calendar_impl(ymwdhms);
  case precision::millisecond: return invalid_detect_calendar_impl(ymwdhmss1);
  case precision::microsecond: return invalid_detect_calendar_impl(ymwdhmss2);
  case precision::nanosecond: return invalid_detect_calendar_impl(ymwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_detect_year_month_weekday_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                   const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers day = weekday::get_day(fields);
  cpp11::integers index = weekday::get_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_any_calendar_impl(y);
  case precision::month: return invalid_any_calendar_impl(ym);
  case precision::day: return invalid_any_calendar_impl(ymwd);
  case precision::hour: return invalid_any_calendar_impl(ymwdh);
  case precision::minute: return invalid_any_calendar_impl(ymwdhm);
  case precision::second: return invalid_any_calendar_impl(ymwdhms);
  case precision::millisecond: return invalid_any_calendar_impl(ymwdhmss1);
  case precision::microsecond: return invalid_any_calendar_impl(ymwdhmss2);
  case precision::nanosecond: return invalid_any_calendar_impl(ymwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_any_year_month_weekday_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                     const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers day = weekday::get_day(fields);
  cpp11::integers index = weekday::get_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_count_calendar_impl(y);
  case precision::month: return invalid_count_calendar_impl(ym);
  case precision::day: return invalid_count_calendar_impl(ymwd);
  case precision::hour: return invalid_count_calendar_impl(ymwdh);
  case precision::minute: return invalid_count_calendar_impl(ymwdhm);
  case precision::second: return invalid_count_calendar_impl(ymwdhms);
  case precision::millisecond: return invalid_count_calendar_impl(ymwdhmss1);
  case precision::microsecond: return invalid_count_calendar_impl(ymwdhmss2);
  case precision::nanosecond: return invalid_count_calendar_impl(ymwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_count_year_month_weekday_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                       const cpp11::integers& precision_int,
                                       const cpp11::strings& invalid_string) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid_string);

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers day = weekday::get_day(fields);
  cpp11::integers index = weekday::get_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_resolve_calendar_impl(y, invalid_val);
  case precision::month: return invalid_resolve_calendar_impl(ym, invalid_val);
  case precision::day: return invalid_resolve_calendar_impl(ymwd, invalid_val);
  case precision::hour: return invalid_resolve_calendar_impl(ymwdh, invalid_val);
  case precision::minute: return invalid_resolve_calendar_impl(ymwdhm, invalid_val);
  case precision::second: return invalid_resolve_calendar_impl(ymwdhms, invalid_val);
  case precision::millisecond: return invalid_resolve_calendar_impl(ymwdhmss1, invalid_val);
  case precision::microsecond: return invalid_resolve_calendar_impl(ymwdhmss2, invalid_val);
  case precision::nanosecond: return invalid_resolve_calendar_impl(ymwdhmss3, invalid_val);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_resolve_year_month_weekday_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
set_field_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::integers& value,
                                 const cpp11::integers& precision_fields,
                                 const cpp11::strings& component_string) {
  using namespace rclock;
  rclock::integers value2(value);

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers day = weekday::get_day(fields);
  cpp11::integers index = weekday::get_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::year: {
    switch (parse_component(component_string)) {
    case component::year: return set_field_calendar<component::year>(y, value2);
    case component::month: return set_field_calendar<component::month>(y, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision::month: {
    switch (parse_component(component_string)) {
    case component::year: return set_field_calendar<component::year>(ym, value2);
    case component::month: return set_field_calendar<component::month>(ym, value2);
    // Note: month precision will be promoted up to day before setting weekday/index
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision::day: {
    switch (parse_component(component_string)) {
    case component::year: return set_field_calendar<component::year>(ymwd, value2);
    case component::month: return set_field_calendar<component::month>(ymwd, value2);
    case component::day: return set_field_calendar<component::day>(ymwd, value2);
    case component::index: return set_field_calendar<component::index>(ymwd, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwd, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision::hour: {
    switch (parse_component(component_string)) {
    case component::year: return set_field_calendar<component::year>(ymwdh, value2);
    case component::month: return set_field_calendar<component::month>(ymwdh, value2);
    case component::day: return set_field_calendar<component::day>(ymwdh, value2);
    case component::index: return set_field_calendar<component::index>(ymwdh, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdh, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdh, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision::minute: {
    switch (parse_component(component_string)) {
    case component::year: return set_field_calendar<component::year>(ymwdhm, value2);
    case component::month: return set_field_calendar<component::month>(ymwdhm, value2);
    case component::day: return set_field_calendar<component::day>(ymwdhm, value2);
    case component::index: return set_field_calendar<component::index>(ymwdhm, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdhm, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdhm, value2);
    case component::second: return set_field_calendar<component::second>(ymwdhm, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision::second: {
    switch (parse_component(component_string)) {
    case component::year: return set_field_calendar<component::year>(ymwdhms, value2);
    case component::month: return set_field_calendar<component::month>(ymwdhms, value2);
    case component::day: return set_field_calendar<component::day>(ymwdhms, value2);
    case component::index: return set_field_calendar<component::index>(ymwdhms, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdhms, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdhms, value2);
    case component::second: return set_field_calendar<component::second>(ymwdhms, value2);
    case component::millisecond: return set_field_calendar<component::millisecond>(ymwdhms, value2);
    case component::microsecond: return set_field_calendar<component::microsecond>(ymwdhms, value2);
    case component::nanosecond: return set_field_calendar<component::nanosecond>(ymwdhms, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision::millisecond: {
    switch (parse_component(component_string)) {
    case component::year: return set_field_calendar<component::year>(ymwdhmss1, value2);
    case component::month: return set_field_calendar<component::month>(ymwdhmss1, value2);
    case component::day: return set_field_calendar<component::day>(ymwdhmss1, value2);
    case component::index: return set_field_calendar<component::index>(ymwdhmss1, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdhmss1, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdhmss1, value2);
    case component::second: return set_field_calendar<component::second>(ymwdhmss1, value2);
    case component::millisecond: return set_field_calendar<component::millisecond>(ymwdhmss1, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision::microsecond: {
    switch (parse_component(component_string)) {
    case component::year: return set_field_calendar<component::year>(ymwdhmss2, value2);
    case component::month: return set_field_calendar<component::month>(ymwdhmss2, value2);
    case component::day: return set_field_calendar<component::day>(ymwdhmss2, value2);
    case component::index: return set_field_calendar<component::index>(ymwdhmss2, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdhmss2, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdhmss2, value2);
    case component::second: return set_field_calendar<component::second>(ymwdhmss2, value2);
    case component::microsecond: return set_field_calendar<component::microsecond>(ymwdhmss2, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  case precision::nanosecond: {
    switch (parse_component(component_string)) {
    case component::year: return set_field_calendar<component::year>(ymwdhmss3, value2);
    case component::month: return set_field_calendar<component::month>(ymwdhmss3, value2);
    case component::day: return set_field_calendar<component::day>(ymwdhmss3, value2);
    case component::index: return set_field_calendar<component::index>(ymwdhmss3, value2);
    case component::hour: return set_field_calendar<component::hour>(ymwdhmss3, value2);
    case component::minute: return set_field_calendar<component::minute>(ymwdhmss3, value2);
    case component::second: return set_field_calendar<component::second>(ymwdhmss3, value2);
    case component::nanosecond: return set_field_calendar<component::nanosecond>(ymwdhmss3, value2);
    default: clock_abort("Internal error: Invalid component.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("set_field_year_month_weekday_cpp");
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
                                  const cpp11::integers& precision_fields) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers day = weekday::get_day(fields);
  cpp11::integers index = weekday::get_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::day: return set_field_year_month_weekday_last_impl(ymwd);
  case precision::hour: return set_field_year_month_weekday_last_impl(ymwdh);
  case precision::minute: return set_field_year_month_weekday_last_impl(ymwdhm);
  case precision::second: return set_field_year_month_weekday_last_impl(ymwdhms);
  case precision::millisecond: return set_field_year_month_weekday_last_impl(ymwdhmss1);
  case precision::microsecond: return set_field_year_month_weekday_last_impl(ymwdhmss2);
  case precision::nanosecond: return set_field_year_month_weekday_last_impl(ymwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("set_field_year_month_weekday_last_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
year_month_weekday_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                     cpp11::list_of<cpp11::integers> fields_n,
                                     const cpp11::integers& precision_fields,
                                     const cpp11::integers& precision_n) {
  using namespace rclock;

  const enum precision precision_fields_val = parse_precision(precision_fields);
  const enum precision precision_n_val = parse_precision(precision_n);

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers day = weekday::get_day(fields);
  cpp11::integers index = weekday::get_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::y y{year};
  weekday::ym ym{year, month};
  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

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
    case precision::year: return calendar_plus_duration_impl(ymwd, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymwd, dq);
    case precision::month: return calendar_plus_duration_impl(ymwd, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::hour:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymwdh, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymwdh, dq);
    case precision::month: return calendar_plus_duration_impl(ymwdh, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::minute:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymwdhm, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymwdhm, dq);
    case precision::month: return calendar_plus_duration_impl(ymwdhm, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::second:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymwdhms, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymwdhms, dq);
    case precision::month: return calendar_plus_duration_impl(ymwdhms, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::millisecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymwdhmss1, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymwdhmss1, dq);
    case precision::month: return calendar_plus_duration_impl(ymwdhmss1, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::microsecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymwdhmss2, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymwdhmss2, dq);
    case precision::month: return calendar_plus_duration_impl(ymwdhmss2, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::nanosecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ymwdhmss3, dy);
    case precision::quarter: return calendar_plus_duration_impl(ymwdhmss3, dq);
    case precision::month: return calendar_plus_duration_impl(ymwdhmss3, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_month_weekday_plus_duration_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = weekday::get_year(fields);
  cpp11::integers month = weekday::get_month(fields);
  cpp11::integers day = weekday::get_day(fields);
  cpp11::integers index = weekday::get_index(fields);
  cpp11::integers hour = weekday::get_hour(fields);
  cpp11::integers minute = weekday::get_minute(fields);
  cpp11::integers second = weekday::get_second(fields);
  cpp11::integers subsecond = weekday::get_subsecond(fields);

  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::day: return as_sys_time_from_calendar_impl<duration::days>(ymwd);
  case precision::hour: return as_sys_time_from_calendar_impl<duration::hours>(ymwdh);
  case precision::minute: return as_sys_time_from_calendar_impl<duration::minutes>(ymwdhm);
  case precision::second: return as_sys_time_from_calendar_impl<duration::seconds>(ymwdhms);
  case precision::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(ymwdhmss1);
  case precision::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(ymwdhmss2);
  case precision::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(ymwdhmss3);
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

  never_reached("as_sys_time_year_month_weekday_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_year_month_weekday_from_sys_time_cpp(cpp11::list_of<cpp11::integers> fields,
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
  case precision::day: return as_calendar_from_sys_time_impl<weekday::ymwd>(dd);
  case precision::hour: return as_calendar_from_sys_time_impl<weekday::ymwdh>(dh);
  case precision::minute: return as_calendar_from_sys_time_impl<weekday::ymwdhm>(dmin);
  case precision::second: return as_calendar_from_sys_time_impl<weekday::ymwdhms>(ds);
  case precision::millisecond: return as_calendar_from_sys_time_impl<weekday::ymwdhmss<std::chrono::milliseconds>>(dmilli);
  case precision::microsecond: return as_calendar_from_sys_time_impl<weekday::ymwdhmss<std::chrono::microseconds>>(dmicro);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<weekday::ymwdhmss<std::chrono::nanoseconds>>(dnano);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("as_year_month_weekday_from_sys_time_cpp");
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
                                                const cpp11::integers& precision_int) {
  const cpp11::integers x_year = rclock::weekday::get_year(x);
  const cpp11::integers x_month = rclock::weekday::get_month(x);

  const cpp11::integers y_year = rclock::weekday::get_year(y);
  const cpp11::integers y_month = rclock::weekday::get_month(y);

  const rclock::weekday::y x_y{x_year};
  const rclock::weekday::ym x_ym{x_year, x_month};

  const rclock::weekday::y y_y{y_year};
  const rclock::weekday::ym y_ym{y_year, y_month};

  switch (parse_precision(precision_int)) {
  case precision::year: return year_minus_year_impl(x_y, y_y);
  case precision::month: return year_month_minus_year_month_impl(x_ym, y_ym);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_month_weekday_minus_year_month_weekday_cpp");
}
