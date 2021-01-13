#include "iso-year-weeknum-weekday.h"
#include "calendar.h"
#include "duration.h"
#include "check.h"
#include "enums.h"
#include "get.h"

[[cpp11::register]]
void
iso_year_weeknum_weekday_check_range_cpp(const cpp11::integers& x,
                                         const cpp11::strings& component,
                                         const cpp11::strings& arg) {
  std::string x_arg_string = arg[0];
  const char* x_arg = x_arg_string.c_str();

  // Used to access the non-static method
  rclock::iso::y dummy(0);

  switch (parse_component(component)) {
  case component::year: return calendar_check_range_impl<component::year>(dummy, x, x_arg);
  case component::weeknum: return calendar_check_range_impl<component::weeknum>(dummy, x, x_arg);
  case component::weekday: return calendar_check_range_impl<component::weekday>(dummy, x, x_arg);
  case component::hour: return calendar_check_range_impl<component::hour>(dummy, x, x_arg);
  case component::minute: return calendar_check_range_impl<component::minute>(dummy, x, x_arg);
  case component::second: return calendar_check_range_impl<component::second>(dummy, x, x_arg);
  case component::millisecond: return calendar_check_range_impl<component::millisecond>(dummy, x, x_arg);
  case component::microsecond: return calendar_check_range_impl<component::microsecond>(dummy, x, x_arg);
  case component::nanosecond: return calendar_check_range_impl<component::nanosecond>(dummy, x, x_arg);
  default: clock_abort("Internal error: Unknown component");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
collect_iso_year_weeknum_weekday_fields(cpp11::list_of<cpp11::integers> fields,
                                        const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers weeknum = iso::get_weeknum(fields);
  cpp11::integers weekday = iso::get_weekday(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, weeknum};
  iso::ywnwd ywnwd{year, weeknum, weekday};
  iso::ywnwdh ywnwdh{year, weeknum, weekday, hour};
  iso::ywnwdhm ywnwdhm{year, weeknum, weekday, hour, minute};
  iso::ywnwdhms ywnwdhms{year, weeknum, weekday, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, weeknum, weekday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: {
    collect_field<component::year>(y, year, "year");
    return y.to_list();
  }
  case precision::week: {
    collect_field<component::year>(ywn, year, "year");
    collect_field<component::weeknum>(ywn, weeknum, "weeknum");
    return ywn.to_list();
  }
  case precision::day: {
    collect_field<component::year>(ywnwd, year, "year");
    collect_field<component::weeknum>(ywnwd, weeknum, "weeknum");
    collect_field<component::weekday>(ywnwd, weekday, "weekday");
    return ywnwd.to_list();
  }
  case precision::hour: {
    collect_field<component::year>(ywnwdh, year, "year");
    collect_field<component::weeknum>(ywnwdh, weeknum, "weeknum");
    collect_field<component::weekday>(ywnwdh, weekday, "weekday");
    collect_field<component::hour>(ywnwdh, hour, "hour");
    return ywnwdh.to_list();
  }
  case precision::minute: {
    collect_field<component::year>(ywnwdhm, year, "year");
    collect_field<component::weeknum>(ywnwdhm, weeknum, "weeknum");
    collect_field<component::weekday>(ywnwdhm, weekday, "weekday");
    collect_field<component::hour>(ywnwdhm, hour, "hour");
    collect_field<component::minute>(ywnwdhm, minute, "minute");
    return ywnwdhm.to_list();
  }
  case precision::second: {
    collect_field<component::year>(ywnwdhms, year, "year");
    collect_field<component::weeknum>(ywnwdhms, weeknum, "weeknum");
    collect_field<component::weekday>(ywnwdhms, weekday, "weekday");
    collect_field<component::hour>(ywnwdhms, hour, "hour");
    collect_field<component::minute>(ywnwdhms, minute, "minute");
    collect_field<component::second>(ywnwdhms, second, "second");
    return ywnwdhms.to_list();
  }
  case precision::millisecond: {
    collect_field<component::year>(ywnwdhmss1, year, "year");
    collect_field<component::weeknum>(ywnwdhmss1, weeknum, "weeknum");
    collect_field<component::weekday>(ywnwdhmss1, weekday, "weekday");
    collect_field<component::hour>(ywnwdhmss1, hour, "hour");
    collect_field<component::minute>(ywnwdhmss1, minute, "minute");
    collect_field<component::second>(ywnwdhmss1, second, "second");
    collect_field<component::millisecond>(ywnwdhmss1, subsecond, "subsecond");
    return ywnwdhmss1.to_list();
  }
  case precision::microsecond: {
    collect_field<component::year>(ywnwdhmss2, year, "year");
    collect_field<component::weeknum>(ywnwdhmss2, weeknum, "weeknum");
    collect_field<component::weekday>(ywnwdhmss2, weekday, "weekday");
    collect_field<component::hour>(ywnwdhmss2, hour, "hour");
    collect_field<component::minute>(ywnwdhmss2, minute, "minute");
    collect_field<component::second>(ywnwdhmss2, second, "second");
    collect_field<component::microsecond>(ywnwdhmss2, subsecond, "subsecond");
    return ywnwdhmss2.to_list();
  }
  case precision::nanosecond: {
    collect_field<component::year>(ywnwdhmss3, year, "year");
    collect_field<component::weeknum>(ywnwdhmss3, weeknum, "weeknum");
    collect_field<component::weekday>(ywnwdhmss3, weekday, "weekday");
    collect_field<component::hour>(ywnwdhmss3, hour, "hour");
    collect_field<component::minute>(ywnwdhmss3, minute, "minute");
    collect_field<component::second>(ywnwdhmss3, second, "second");
    collect_field<component::nanosecond>(ywnwdhmss3, subsecond, "subsecond");
    return ywnwdhmss3.to_list();
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
format_iso_year_weeknum_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                    const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers weeknum = iso::get_weeknum(fields);
  cpp11::integers weekday = iso::get_weekday(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, weeknum};
  iso::ywnwd ywnwd{year, weeknum, weekday};
  iso::ywnwdh ywnwdh{year, weeknum, weekday, hour};
  iso::ywnwdhm ywnwdhm{year, weeknum, weekday, hour, minute};
  iso::ywnwdhms ywnwdhms{year, weeknum, weekday, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, weeknum, weekday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: return format_calendar_impl(y);
  case precision::week: return format_calendar_impl(ywn);
  case precision::day: return format_calendar_impl(ywnwd);
  case precision::hour: return format_calendar_impl(ywnwdh);
  case precision::minute: return format_calendar_impl(ywnwdhm);
  case precision::second: return format_calendar_impl(ywnwdhms);
  case precision::millisecond: return format_calendar_impl(ywnwdhmss1);
  case precision::microsecond: return format_calendar_impl(ywnwdhmss2);
  case precision::nanosecond: return format_calendar_impl(ywnwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_iso_year_weeknum_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers weeknum = iso::get_weeknum(fields);
  cpp11::integers weekday = iso::get_weekday(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, weeknum};
  iso::ywnwd ywnwd{year, weeknum, weekday};
  iso::ywnwdh ywnwdh{year, weeknum, weekday, hour};
  iso::ywnwdhm ywnwdhm{year, weeknum, weekday, hour, minute};
  iso::ywnwdhms ywnwdhms{year, weeknum, weekday, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, weeknum, weekday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: return invalid_detect_calendar_impl(y);
  case precision::week: return invalid_detect_calendar_impl(ywn);
  case precision::day: return invalid_detect_calendar_impl(ywnwd);
  case precision::hour: return invalid_detect_calendar_impl(ywnwdh);
  case precision::minute: return invalid_detect_calendar_impl(ywnwdhm);
  case precision::second: return invalid_detect_calendar_impl(ywnwdhms);
  case precision::millisecond: return invalid_detect_calendar_impl(ywnwdhmss1);
  case precision::microsecond: return invalid_detect_calendar_impl(ywnwdhmss2);
  case precision::nanosecond: return invalid_detect_calendar_impl(ywnwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_iso_year_weeknum_weekday_cpp(cpp11::list_of<cpp11::integers> fields, cpp11::strings precision) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers weeknum = iso::get_weeknum(fields);
  cpp11::integers weekday = iso::get_weekday(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, weeknum};
  iso::ywnwd ywnwd{year, weeknum, weekday};
  iso::ywnwdh ywnwdh{year, weeknum, weekday, hour};
  iso::ywnwdhm ywnwdhm{year, weeknum, weekday, hour, minute};
  iso::ywnwdhms ywnwdhms{year, weeknum, weekday, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, weeknum, weekday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: return invalid_any_calendar_impl(y);
  case precision::week: return invalid_any_calendar_impl(ywn);
  case precision::day: return invalid_any_calendar_impl(ywnwd);
  case precision::hour: return invalid_any_calendar_impl(ywnwdh);
  case precision::minute: return invalid_any_calendar_impl(ywnwdhm);
  case precision::second: return invalid_any_calendar_impl(ywnwdhms);
  case precision::millisecond: return invalid_any_calendar_impl(ywnwdhmss1);
  case precision::microsecond: return invalid_any_calendar_impl(ywnwdhmss2);
  case precision::nanosecond: return invalid_any_calendar_impl(ywnwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_iso_year_weeknum_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers weeknum = iso::get_weeknum(fields);
  cpp11::integers weekday = iso::get_weekday(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, weeknum};
  iso::ywnwd ywnwd{year, weeknum, weekday};
  iso::ywnwdh ywnwdh{year, weeknum, weekday, hour};
  iso::ywnwdhm ywnwdhm{year, weeknum, weekday, hour, minute};
  iso::ywnwdhms ywnwdhms{year, weeknum, weekday, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, weeknum, weekday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: return invalid_count_calendar_impl(y);
  case precision::week: return invalid_count_calendar_impl(ywn);
  case precision::day: return invalid_count_calendar_impl(ywnwd);
  case precision::hour: return invalid_count_calendar_impl(ywnwdh);
  case precision::minute: return invalid_count_calendar_impl(ywnwdhm);
  case precision::second: return invalid_count_calendar_impl(ywnwdhms);
  case precision::millisecond: return invalid_count_calendar_impl(ywnwdhmss1);
  case precision::microsecond: return invalid_count_calendar_impl(ywnwdhmss2);
  case precision::nanosecond: return invalid_count_calendar_impl(ywnwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_iso_year_weeknum_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                   const cpp11::strings& precision,
                                   const cpp11::strings& invalid) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid);

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers weeknum = iso::get_weeknum(fields);
  cpp11::integers weekday = iso::get_weekday(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, weeknum};
  iso::ywnwd ywnwd{year, weeknum, weekday};
  iso::ywnwdh ywnwdh{year, weeknum, weekday, hour};
  iso::ywnwdhm ywnwdhm{year, weeknum, weekday, hour, minute};
  iso::ywnwdhms ywnwdhms{year, weeknum, weekday, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, weeknum, weekday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: return invalid_resolve_calendar_impl(y, invalid_val);
  case precision::week: return invalid_resolve_calendar_impl(ywn, invalid_val);
  case precision::day: return invalid_resolve_calendar_impl(ywnwd, invalid_val);
  case precision::hour: return invalid_resolve_calendar_impl(ywnwdh, invalid_val);
  case precision::minute: return invalid_resolve_calendar_impl(ywnwdhm, invalid_val);
  case precision::second: return invalid_resolve_calendar_impl(ywnwdhms, invalid_val);
  case precision::millisecond: return invalid_resolve_calendar_impl(ywnwdhmss1, invalid_val);
  case precision::microsecond: return invalid_resolve_calendar_impl(ywnwdhmss2, invalid_val);
  case precision::nanosecond: return invalid_resolve_calendar_impl(ywnwdhmss3, invalid_val);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
set_field_iso_year_weeknum_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                             const cpp11::integers& value,
                             const cpp11::strings& precision_fields,
                             const cpp11::strings& precision_value) {
  using namespace rclock;
  rclock::integers value2(value);

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers weeknum = iso::get_weeknum(fields);
  cpp11::integers weekday = iso::get_weekday(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, weeknum};
  iso::ywnwd ywnwd{year, weeknum, weekday};
  iso::ywnwdh ywnwdh{year, weeknum, weekday, hour};
  iso::ywnwdhm ywnwdhm{year, weeknum, weekday, hour, minute};
  iso::ywnwdhms ywnwdhms{year, weeknum, weekday, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, weeknum, weekday, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::year: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(y, value2);
    case precision::week: return set_field_calendar<component::weeknum>(y, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::week: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywn, value2);
    case precision::week: return set_field_calendar<component::weeknum>(ywn, value2);
    case precision::day: return set_field_calendar<component::weekday>(ywn, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::day: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwd, value2);
    case precision::week: return set_field_calendar<component::weeknum>(ywnwd, value2);
    case precision::day: return set_field_calendar<component::weekday>(ywnwd, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwd, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::hour: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwdh, value2);
    case precision::week: return set_field_calendar<component::weeknum>(ywnwdh, value2);
    case precision::day: return set_field_calendar<component::weekday>(ywnwdh, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwdh, value2);
    case precision::minute: return set_field_calendar<component::minute>(ywnwdh, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::minute: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwdhm, value2);
    case precision::week: return set_field_calendar<component::weeknum>(ywnwdhm, value2);
    case precision::day: return set_field_calendar<component::weekday>(ywnwdhm, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwdhm, value2);
    case precision::minute: return set_field_calendar<component::minute>(ywnwdhm, value2);
    case precision::second: return set_field_calendar<component::second>(ywnwdhm, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::second: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwdhms, value2);
    case precision::week: return set_field_calendar<component::weeknum>(ywnwdhms, value2);
    case precision::day: return set_field_calendar<component::weekday>(ywnwdhms, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwdhms, value2);
    case precision::minute: return set_field_calendar<component::minute>(ywnwdhms, value2);
    case precision::second: return set_field_calendar<component::second>(ywnwdhms, value2);
    case precision::millisecond: return set_field_calendar<component::millisecond>(ywnwdhms, value2);
    case precision::microsecond: return set_field_calendar<component::microsecond>(ywnwdhms, value2);
    case precision::nanosecond: return set_field_calendar<component::nanosecond>(ywnwdhms, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::millisecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwdhmss1, value2);
    case precision::week: return set_field_calendar<component::weeknum>(ywnwdhmss1, value2);
    case precision::day: return set_field_calendar<component::weekday>(ywnwdhmss1, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwdhmss1, value2);
    case precision::minute: return set_field_calendar<component::minute>(ywnwdhmss1, value2);
    case precision::second: return set_field_calendar<component::second>(ywnwdhmss1, value2);
    case precision::millisecond: return set_field_calendar<component::millisecond>(ywnwdhmss1, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::microsecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwdhmss2, value2);
    case precision::week: return set_field_calendar<component::weeknum>(ywnwdhmss2, value2);
    case precision::day: return set_field_calendar<component::weekday>(ywnwdhmss2, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwdhmss2, value2);
    case precision::minute: return set_field_calendar<component::minute>(ywnwdhmss2, value2);
    case precision::second: return set_field_calendar<component::second>(ywnwdhmss2, value2);
    case precision::microsecond: return set_field_calendar<component::microsecond>(ywnwdhmss2, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::nanosecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwdhmss3, value2);
    case precision::week: return set_field_calendar<component::weeknum>(ywnwdhmss3, value2);
    case precision::day: return set_field_calendar<component::weekday>(ywnwdhmss3, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwdhmss3, value2);
    case precision::minute: return set_field_calendar<component::minute>(ywnwdhmss3, value2);
    case precision::second: return set_field_calendar<component::second>(ywnwdhmss3, value2);
    case precision::nanosecond: return set_field_calendar<component::nanosecond>(ywnwdhmss3, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

template <class Calendar>
cpp11::writable::list
set_field_iso_year_weeknum_weekday_last_impl(const Calendar& x) {
  const r_ssize size = x.size();
  cpp11::writable::integers value(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      value[i] = r_int_na;
    } else {
      iso_week::year_lastweek ylw = x.to_year(i) / iso_week::last;
      value[i] = static_cast<int>(static_cast<unsigned>(ylw.weeknum()));
    }
  }

  cpp11::writable::list out({x.to_list(), value});
  out.names() = {"fields", "value"};

  return out;
}

[[cpp11::register]]
cpp11::writable::list
set_field_iso_year_weeknum_weekday_last_cpp(cpp11::list_of<cpp11::integers> fields,
                                            const cpp11::strings& precision_fields) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers weeknum = iso::get_weeknum(fields);
  cpp11::integers weekday = iso::get_weekday(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, weeknum};
  iso::ywnwd ywnwd{year, weeknum, weekday};
  iso::ywnwdh ywnwdh{year, weeknum, weekday, hour};
  iso::ywnwdhm ywnwdhm{year, weeknum, weekday, hour, minute};
  iso::ywnwdhms ywnwdhms{year, weeknum, weekday, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, weeknum, weekday, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::year: return set_field_iso_year_weeknum_weekday_last_impl(y);
  case precision::week: return set_field_iso_year_weeknum_weekday_last_impl(ywn);
  case precision::day: return set_field_iso_year_weeknum_weekday_last_impl(ywnwd);
  case precision::hour: return set_field_iso_year_weeknum_weekday_last_impl(ywnwdh);
  case precision::minute: return set_field_iso_year_weeknum_weekday_last_impl(ywnwdhm);
  case precision::second: return set_field_iso_year_weeknum_weekday_last_impl(ywnwdhms);
  case precision::millisecond: return set_field_iso_year_weeknum_weekday_last_impl(ywnwdhmss1);
  case precision::microsecond: return set_field_iso_year_weeknum_weekday_last_impl(ywnwdhmss2);
  case precision::nanosecond: return set_field_iso_year_weeknum_weekday_last_impl(ywnwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
iso_year_weeknum_weekday_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                           cpp11::list_of<cpp11::integers> fields_n,
                                           const cpp11::strings& precision_fields,
                                           const cpp11::strings& precision_n) {
  using namespace rclock;

  const enum precision precision_fields_val = parse_precision(precision_fields);
  const enum precision precision_n_val = parse_precision(precision_n);

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers weeknum = iso::get_weeknum(fields);
  cpp11::integers weekday = iso::get_weekday(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, weeknum};
  iso::ywnwd ywnwd{year, weeknum, weekday};
  iso::ywnwdh ywnwdh{year, weeknum, weekday, hour};
  iso::ywnwdhm ywnwdhm{year, weeknum, weekday, hour, minute};
  iso::ywnwdhms ywnwdhms{year, weeknum, weekday, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, weeknum, weekday, hour, minute, second, subsecond};

  cpp11::integers ticks = duration::get_ticks(fields_n);

  duration::years dy{ticks};

  switch (precision_fields_val) {
  case precision::year:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(y, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::week:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywn, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::day:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwd, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::hour:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdh, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::minute:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdhm, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::second:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdhms, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::millisecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdhmss1, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::microsecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdhmss2, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::nanosecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdhmss3, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_sys_time_iso_year_weeknum_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers weeknum = iso::get_weeknum(fields);
  cpp11::integers weekday = iso::get_weekday(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::ywnwd ywnwd{year, weeknum, weekday};
  iso::ywnwdh ywnwdh{year, weeknum, weekday, hour};
  iso::ywnwdhm ywnwdhm{year, weeknum, weekday, hour, minute};
  iso::ywnwdhms ywnwdhms{year, weeknum, weekday, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, weeknum, weekday, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, weeknum, weekday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::day: return as_sys_time_from_calendar_impl<duration::days>(ywnwd);
  case precision::hour: return as_sys_time_from_calendar_impl<duration::hours>(ywnwdh);
  case precision::minute: return as_sys_time_from_calendar_impl<duration::minutes>(ywnwdhm);
  case precision::second: return as_sys_time_from_calendar_impl<duration::seconds>(ywnwdhms);
  case precision::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(ywnwdhmss1);
  case precision::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(ywnwdhmss2);
  case precision::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(ywnwdhmss3);
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
as_iso_year_weeknum_weekday_from_sys_time_cpp(cpp11::list_of<cpp11::integers> fields,
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

  switch (parse_precision(precision)) {
  case precision::day: return as_calendar_from_sys_time_impl<iso::ywnwd>(dd);
  case precision::hour: return as_calendar_from_sys_time_impl<iso::ywnwdh>(dh);
  case precision::minute: return as_calendar_from_sys_time_impl<iso::ywnwdhm>(dmin);
  case precision::second: return as_calendar_from_sys_time_impl<iso::ywnwdhms>(ds);
  case precision::millisecond: return as_calendar_from_sys_time_impl<iso::ywnwdhmss<std::chrono::milliseconds>>(dmilli);
  case precision::microsecond: return as_calendar_from_sys_time_impl<iso::ywnwdhmss<std::chrono::microseconds>>(dmicro);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<iso::ywnwdhmss<std::chrono::nanoseconds>>(dnano);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

static
inline
cpp11::writable::list
year_minus_year_impl(const rclock::iso::y& x,
                     const rclock::iso::y& y) {
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

[[cpp11::register]]
cpp11::writable::list
iso_year_weeknum_weekday_minus_iso_year_weeknum_weekday_cpp(cpp11::list_of<cpp11::integers> x,
                                                            cpp11::list_of<cpp11::integers> y,
                                                            const cpp11::strings& precision) {
  const cpp11::integers x_year = rclock::iso::get_year(x);
  const cpp11::integers y_year = rclock::iso::get_year(y);

  const rclock::iso::y x_y{x_year};
  const rclock::iso::y y_y{y_year};

  switch (parse_precision(precision)) {
  case precision::year: return year_minus_year_impl(x_y, y_y);
  default: clock_abort("Internal error: Invalid precision.");
  }
}
