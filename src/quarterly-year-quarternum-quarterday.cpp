#include "quarterly-year-quarternum-quarterday.h"
#include "calendar.h"
#include "duration.h"
#include "check.h"
#include "enums.h"
#include "get.h"

[[cpp11::register]]
void
year_quarternum_quarterday_check_range_cpp(const cpp11::integers& x,
                                           const cpp11::strings& component,
                                           const cpp11::strings& arg) {
  std::string x_arg_string = arg[0];
  const char* x_arg = x_arg_string.c_str();

  // The `quarterly::start` doesn't affect the range of the individual fields
  static const enum quarterly::start start_val = quarterly::start::january;

  // Used to access the non-static method
  rclock::rquarterly::y<start_val> dummy(0);

  switch (parse_component(component)) {
  case component::year: return calendar_check_range_impl<component::year>(dummy, x, x_arg);
  case component::quarternum: return calendar_check_range_impl<component::quarternum>(dummy, x, x_arg);
  case component::quarterday: return calendar_check_range_impl<component::quarterday>(dummy, x, x_arg);
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

template <quarterly::start S>
static
inline
cpp11::writable::list
collect_year_quarternum_quarterday_fields_impl(cpp11::list_of<cpp11::integers> fields,
                                               const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarternum = rquarterly::get_quarternum(fields);
  cpp11::integers quarterday = rquarterly::get_quarterday(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarternum};
  rquarterly::yqnqd<S> yqnqd{year, quarternum, quarterday};
  rquarterly::yqnqdh<S> yqnqdh{year, quarternum, quarterday, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarternum, quarterday, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarternum, quarterday, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarternum, quarterday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: {
    collect_field<component::year>(y, year, "year");
    return y.to_list();
  }
  case precision::quarter: {
    collect_field<component::year>(yqn, year, "year");
    collect_field<component::quarternum>(yqn, quarternum, "quarternum");
    return yqn.to_list();
  }
  case precision::day: {
    collect_field<component::year>(yqnqd, year, "year");
    collect_field<component::quarternum>(yqnqd, quarternum, "quarternum");
    collect_field<component::quarterday>(yqnqd, quarterday, "quarterday");
    return yqnqd.to_list();
  }
  case precision::hour: {
    collect_field<component::year>(yqnqdh, year, "year");
    collect_field<component::quarternum>(yqnqdh, quarternum, "quarternum");
    collect_field<component::quarterday>(yqnqdh, quarterday, "quarterday");
    collect_field<component::hour>(yqnqdh, hour, "hour");
    return yqnqdh.to_list();
  }
  case precision::minute: {
    collect_field<component::year>(yqnqdhm, year, "year");
    collect_field<component::quarternum>(yqnqdhm, quarternum, "quarternum");
    collect_field<component::quarterday>(yqnqdhm, quarterday, "quarterday");
    collect_field<component::hour>(yqnqdhm, hour, "hour");
    collect_field<component::minute>(yqnqdhm, minute, "minute");
    return yqnqdhm.to_list();
  }
  case precision::second: {
    collect_field<component::year>(yqnqdhms, year, "year");
    collect_field<component::quarternum>(yqnqdhms, quarternum, "quarternum");
    collect_field<component::quarterday>(yqnqdhms, quarterday, "quarterday");
    collect_field<component::hour>(yqnqdhms, hour, "hour");
    collect_field<component::minute>(yqnqdhms, minute, "minute");
    collect_field<component::second>(yqnqdhms, second, "second");
    return yqnqdhms.to_list();
  }
  case precision::millisecond: {
    collect_field<component::year>(yqnqdhmss1, year, "year");
    collect_field<component::quarternum>(yqnqdhmss1, quarternum, "quarternum");
    collect_field<component::quarterday>(yqnqdhmss1, quarterday, "quarterday");
    collect_field<component::hour>(yqnqdhmss1, hour, "hour");
    collect_field<component::minute>(yqnqdhmss1, minute, "minute");
    collect_field<component::second>(yqnqdhmss1, second, "second");
    collect_field<component::millisecond>(yqnqdhmss1, subsecond, "subsecond");
    return yqnqdhmss1.to_list();
  }
  case precision::microsecond: {
    collect_field<component::year>(yqnqdhmss2, year, "year");
    collect_field<component::quarternum>(yqnqdhmss2, quarternum, "quarternum");
    collect_field<component::quarterday>(yqnqdhmss2, quarterday, "quarterday");
    collect_field<component::hour>(yqnqdhmss2, hour, "hour");
    collect_field<component::minute>(yqnqdhmss2, minute, "minute");
    collect_field<component::second>(yqnqdhmss2, second, "second");
    collect_field<component::microsecond>(yqnqdhmss2, subsecond, "subsecond");
    return yqnqdhmss2.to_list();
  }
  case precision::nanosecond: {
    collect_field<component::year>(yqnqdhmss3, year, "year");
    collect_field<component::quarternum>(yqnqdhmss3, quarternum, "quarternum");
    collect_field<component::quarterday>(yqnqdhmss3, quarterday, "quarterday");
    collect_field<component::hour>(yqnqdhmss3, hour, "hour");
    collect_field<component::minute>(yqnqdhmss3, minute, "minute");
    collect_field<component::second>(yqnqdhmss3, second, "second");
    collect_field<component::nanosecond>(yqnqdhmss3, subsecond, "subsecond");
    return yqnqdhmss3.to_list();
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
cpp11::writable::list
collect_year_quarternum_quarterday_fields(cpp11::list_of<cpp11::integers> fields,
                                          const cpp11::strings& precision,
                                          const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return collect_year_quarternum_quarterday_fields_impl<start::january>(fields, precision);
  case start::february: return collect_year_quarternum_quarterday_fields_impl<start::february>(fields, precision);
  case start::march: return collect_year_quarternum_quarterday_fields_impl<start::march>(fields, precision);
  case start::april: return collect_year_quarternum_quarterday_fields_impl<start::april>(fields, precision);
  case start::may: return collect_year_quarternum_quarterday_fields_impl<start::may>(fields, precision);
  case start::june: return collect_year_quarternum_quarterday_fields_impl<start::june>(fields, precision);
  case start::july: return collect_year_quarternum_quarterday_fields_impl<start::july>(fields, precision);
  case start::august: return collect_year_quarternum_quarterday_fields_impl<start::august>(fields, precision);
  case start::september: return collect_year_quarternum_quarterday_fields_impl<start::september>(fields, precision);
  case start::october: return collect_year_quarternum_quarterday_fields_impl<start::october>(fields, precision);
  case start::november: return collect_year_quarternum_quarterday_fields_impl<start::november>(fields, precision);
  case start::december: return collect_year_quarternum_quarterday_fields_impl<start::december>(fields, precision);
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
inline
cpp11::writable::strings
format_quarternum_quarterday_impl(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarternum = rquarterly::get_quarternum(fields);
  cpp11::integers quarterday = rquarterly::get_quarterday(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarternum};
  rquarterly::yqnqd<S> yqnqd{year, quarternum, quarterday};
  rquarterly::yqnqdh<S> yqnqdh{year, quarternum, quarterday, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarternum, quarterday, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarternum, quarterday, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarternum, quarterday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: return format_calendar_impl(y);
  case precision::quarter: return format_calendar_impl(yqn);
  case precision::day: return format_calendar_impl(yqnqd);
  case precision::hour: return format_calendar_impl(yqnqdh);
  case precision::minute: return format_calendar_impl(yqnqdhm);
  case precision::second: return format_calendar_impl(yqnqdhms);
  case precision::millisecond: return format_calendar_impl(yqnqdhmss1);
  case precision::microsecond: return format_calendar_impl(yqnqdhmss2);
  case precision::nanosecond: return format_calendar_impl(yqnqdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
cpp11::writable::strings
format_year_quarternum_quarterday_cpp(cpp11::list_of<cpp11::integers> fields,
                                      const cpp11::strings& precision,
                                      const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return format_quarternum_quarterday_impl<start::january>(fields, precision);
  case start::february: return format_quarternum_quarterday_impl<start::february>(fields, precision);
  case start::march: return format_quarternum_quarterday_impl<start::march>(fields, precision);
  case start::april: return format_quarternum_quarterday_impl<start::april>(fields, precision);
  case start::may: return format_quarternum_quarterday_impl<start::may>(fields, precision);
  case start::june: return format_quarternum_quarterday_impl<start::june>(fields, precision);
  case start::july: return format_quarternum_quarterday_impl<start::july>(fields, precision);
  case start::august: return format_quarternum_quarterday_impl<start::august>(fields, precision);
  case start::september: return format_quarternum_quarterday_impl<start::september>(fields, precision);
  case start::october: return format_quarternum_quarterday_impl<start::october>(fields, precision);
  case start::november: return format_quarternum_quarterday_impl<start::november>(fields, precision);
  case start::december: return format_quarternum_quarterday_impl<start::december>(fields, precision);
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
cpp11::writable::logicals
invalid_detect_year_quarternum_quarterday_impl(cpp11::list_of<cpp11::integers> fields,
                                               const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarternum = rquarterly::get_quarternum(fields);
  cpp11::integers quarterday = rquarterly::get_quarterday(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarternum};
  rquarterly::yqnqd<S> yqnqd{year, quarternum, quarterday};
  rquarterly::yqnqdh<S> yqnqdh{year, quarternum, quarterday, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarternum, quarterday, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarternum, quarterday, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarternum, quarterday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: return invalid_detect_calendar_impl(y);
  case precision::quarter: return invalid_detect_calendar_impl(yqn);
  case precision::day: return invalid_detect_calendar_impl(yqnqd);
  case precision::hour: return invalid_detect_calendar_impl(yqnqdh);
  case precision::minute: return invalid_detect_calendar_impl(yqnqdhm);
  case precision::second: return invalid_detect_calendar_impl(yqnqdhms);
  case precision::millisecond: return invalid_detect_calendar_impl(yqnqdhmss1);
  case precision::microsecond: return invalid_detect_calendar_impl(yqnqdhmss2);
  case precision::nanosecond: return invalid_detect_calendar_impl(yqnqdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_quarternum_quarterday_cpp(cpp11::list_of<cpp11::integers> fields,
                                              const cpp11::strings& precision,
                                              const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return invalid_detect_year_quarternum_quarterday_impl<start::january>(fields, precision);
  case start::february: return invalid_detect_year_quarternum_quarterday_impl<start::february>(fields, precision);
  case start::march: return invalid_detect_year_quarternum_quarterday_impl<start::march>(fields, precision);
  case start::april: return invalid_detect_year_quarternum_quarterday_impl<start::april>(fields, precision);
  case start::may: return invalid_detect_year_quarternum_quarterday_impl<start::may>(fields, precision);
  case start::june: return invalid_detect_year_quarternum_quarterday_impl<start::june>(fields, precision);
  case start::july: return invalid_detect_year_quarternum_quarterday_impl<start::july>(fields, precision);
  case start::august: return invalid_detect_year_quarternum_quarterday_impl<start::august>(fields, precision);
  case start::september: return invalid_detect_year_quarternum_quarterday_impl<start::september>(fields, precision);
  case start::october: return invalid_detect_year_quarternum_quarterday_impl<start::october>(fields, precision);
  case start::november: return invalid_detect_year_quarternum_quarterday_impl<start::november>(fields, precision);
  case start::december: return invalid_detect_year_quarternum_quarterday_impl<start::december>(fields, precision);
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
inline
bool
invalid_any_year_quarternum_quarterday_impl(cpp11::list_of<cpp11::integers> fields,
                                            const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarternum = rquarterly::get_quarternum(fields);
  cpp11::integers quarterday = rquarterly::get_quarterday(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarternum};
  rquarterly::yqnqd<S> yqnqd{year, quarternum, quarterday};
  rquarterly::yqnqdh<S> yqnqdh{year, quarternum, quarterday, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarternum, quarterday, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarternum, quarterday, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarternum, quarterday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: return invalid_any_calendar_impl(y);
  case precision::quarter: return invalid_any_calendar_impl(yqn);
  case precision::day: return invalid_any_calendar_impl(yqnqd);
  case precision::hour: return invalid_any_calendar_impl(yqnqdh);
  case precision::minute: return invalid_any_calendar_impl(yqnqdhm);
  case precision::second: return invalid_any_calendar_impl(yqnqdhms);
  case precision::millisecond: return invalid_any_calendar_impl(yqnqdhmss1);
  case precision::microsecond: return invalid_any_calendar_impl(yqnqdhmss2);
  case precision::nanosecond: return invalid_any_calendar_impl(yqnqdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
bool
invalid_any_year_quarternum_quarterday_cpp(cpp11::list_of<cpp11::integers> fields,
                                           const cpp11::strings& precision,
                                           const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return invalid_any_year_quarternum_quarterday_impl<start::january>(fields, precision);
  case start::february: return invalid_any_year_quarternum_quarterday_impl<start::february>(fields, precision);
  case start::march: return invalid_any_year_quarternum_quarterday_impl<start::march>(fields, precision);
  case start::april: return invalid_any_year_quarternum_quarterday_impl<start::april>(fields, precision);
  case start::may: return invalid_any_year_quarternum_quarterday_impl<start::may>(fields, precision);
  case start::june: return invalid_any_year_quarternum_quarterday_impl<start::june>(fields, precision);
  case start::july: return invalid_any_year_quarternum_quarterday_impl<start::july>(fields, precision);
  case start::august: return invalid_any_year_quarternum_quarterday_impl<start::august>(fields, precision);
  case start::september: return invalid_any_year_quarternum_quarterday_impl<start::september>(fields, precision);
  case start::october: return invalid_any_year_quarternum_quarterday_impl<start::october>(fields, precision);
  case start::november: return invalid_any_year_quarternum_quarterday_impl<start::november>(fields, precision);
  case start::december: return invalid_any_year_quarternum_quarterday_impl<start::december>(fields, precision);
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
inline
int
invalid_count_year_quarternum_quarterday_impl(cpp11::list_of<cpp11::integers> fields,
                                              const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarternum = rquarterly::get_quarternum(fields);
  cpp11::integers quarterday = rquarterly::get_quarterday(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarternum};
  rquarterly::yqnqd<S> yqnqd{year, quarternum, quarterday};
  rquarterly::yqnqdh<S> yqnqdh{year, quarternum, quarterday, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarternum, quarterday, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarternum, quarterday, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarternum, quarterday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: return invalid_count_calendar_impl(y);
  case precision::quarter: return invalid_count_calendar_impl(yqn);
  case precision::day: return invalid_count_calendar_impl(yqnqd);
  case precision::hour: return invalid_count_calendar_impl(yqnqdh);
  case precision::minute: return invalid_count_calendar_impl(yqnqdhm);
  case precision::second: return invalid_count_calendar_impl(yqnqdhms);
  case precision::millisecond: return invalid_count_calendar_impl(yqnqdhmss1);
  case precision::microsecond: return invalid_count_calendar_impl(yqnqdhmss2);
  case precision::nanosecond: return invalid_count_calendar_impl(yqnqdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
int
invalid_count_year_quarternum_quarterday_cpp(cpp11::list_of<cpp11::integers> fields,
                                             const cpp11::strings& precision,
                                             const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return invalid_count_year_quarternum_quarterday_impl<start::january>(fields, precision);
  case start::february: return invalid_count_year_quarternum_quarterday_impl<start::february>(fields, precision);
  case start::march: return invalid_count_year_quarternum_quarterday_impl<start::march>(fields, precision);
  case start::april: return invalid_count_year_quarternum_quarterday_impl<start::april>(fields, precision);
  case start::may: return invalid_count_year_quarternum_quarterday_impl<start::may>(fields, precision);
  case start::june: return invalid_count_year_quarternum_quarterday_impl<start::june>(fields, precision);
  case start::july: return invalid_count_year_quarternum_quarterday_impl<start::july>(fields, precision);
  case start::august: return invalid_count_year_quarternum_quarterday_impl<start::august>(fields, precision);
  case start::september: return invalid_count_year_quarternum_quarterday_impl<start::september>(fields, precision);
  case start::october: return invalid_count_year_quarternum_quarterday_impl<start::october>(fields, precision);
  case start::november: return invalid_count_year_quarternum_quarterday_impl<start::november>(fields, precision);
  case start::december: return invalid_count_year_quarternum_quarterday_impl<start::december>(fields, precision);
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
inline
cpp11::writable::list
invalid_resolve_year_quarternum_quarterday_impl(cpp11::list_of<cpp11::integers> fields,
                                                const cpp11::strings& precision,
                                                const cpp11::strings& invalid) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid);

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarternum = rquarterly::get_quarternum(fields);
  cpp11::integers quarterday = rquarterly::get_quarterday(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarternum};
  rquarterly::yqnqd<S> yqnqd{year, quarternum, quarterday};
  rquarterly::yqnqdh<S> yqnqdh{year, quarternum, quarterday, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarternum, quarterday, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarternum, quarterday, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarternum, quarterday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::year: return invalid_resolve_calendar_impl(y, invalid_val);
  case precision::quarter: return invalid_resolve_calendar_impl(yqn, invalid_val);
  case precision::day: return invalid_resolve_calendar_impl(yqnqd, invalid_val);
  case precision::hour: return invalid_resolve_calendar_impl(yqnqdh, invalid_val);
  case precision::minute: return invalid_resolve_calendar_impl(yqnqdhm, invalid_val);
  case precision::second: return invalid_resolve_calendar_impl(yqnqdhms, invalid_val);
  case precision::millisecond: return invalid_resolve_calendar_impl(yqnqdhmss1, invalid_val);
  case precision::microsecond: return invalid_resolve_calendar_impl(yqnqdhmss2, invalid_val);
  case precision::nanosecond: return invalid_resolve_calendar_impl(yqnqdhmss3, invalid_val);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_quarternum_quarterday_cpp(cpp11::list_of<cpp11::integers> fields,
                                               const cpp11::strings& precision,
                                               const cpp11::integers& start,
                                               const cpp11::strings& invalid) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return invalid_resolve_year_quarternum_quarterday_impl<start::january>(fields, precision, invalid);
  case start::february: return invalid_resolve_year_quarternum_quarterday_impl<start::february>(fields, precision, invalid);
  case start::march: return invalid_resolve_year_quarternum_quarterday_impl<start::march>(fields, precision, invalid);
  case start::april: return invalid_resolve_year_quarternum_quarterday_impl<start::april>(fields, precision, invalid);
  case start::may: return invalid_resolve_year_quarternum_quarterday_impl<start::may>(fields, precision, invalid);
  case start::june: return invalid_resolve_year_quarternum_quarterday_impl<start::june>(fields, precision, invalid);
  case start::july: return invalid_resolve_year_quarternum_quarterday_impl<start::july>(fields, precision, invalid);
  case start::august: return invalid_resolve_year_quarternum_quarterday_impl<start::august>(fields, precision, invalid);
  case start::september: return invalid_resolve_year_quarternum_quarterday_impl<start::september>(fields, precision, invalid);
  case start::october: return invalid_resolve_year_quarternum_quarterday_impl<start::october>(fields, precision, invalid);
  case start::november: return invalid_resolve_year_quarternum_quarterday_impl<start::november>(fields, precision, invalid);
  case start::december: return invalid_resolve_year_quarternum_quarterday_impl<start::december>(fields, precision, invalid);
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
cpp11::writable::list
set_field_year_quarternum_quarterday_impl(cpp11::list_of<cpp11::integers> fields,
                                          const cpp11::integers& value,
                                          const cpp11::strings& precision_fields,
                                          const cpp11::strings& precision_value) {
  using namespace rclock;
  rclock::integers value2(value);

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarternum = rquarterly::get_quarternum(fields);
  cpp11::integers quarterday = rquarterly::get_quarterday(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarternum};
  rquarterly::yqnqd<S> yqnqd{year, quarternum, quarterday};
  rquarterly::yqnqdh<S> yqnqdh{year, quarternum, quarterday, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarternum, quarterday, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarternum, quarterday, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarternum, quarterday, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::year: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(y, value2);
    case precision::quarter: return set_field_calendar<component::quarternum>(y, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::quarter: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqn, value2);
    case precision::quarter: return set_field_calendar<component::quarternum>(yqn, value2);
    case precision::day: return set_field_calendar<component::quarterday>(yqn, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::day: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqd, value2);
    case precision::quarter: return set_field_calendar<component::quarternum>(yqnqd, value2);
    case precision::day: return set_field_calendar<component::quarterday>(yqnqd, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqd, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::hour: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqdh, value2);
    case precision::quarter: return set_field_calendar<component::quarternum>(yqnqdh, value2);
    case precision::day: return set_field_calendar<component::quarterday>(yqnqdh, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqdh, value2);
    case precision::minute: return set_field_calendar<component::minute>(yqnqdh, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::minute: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqdhm, value2);
    case precision::quarter: return set_field_calendar<component::quarternum>(yqnqdhm, value2);
    case precision::day: return set_field_calendar<component::quarterday>(yqnqdhm, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqdhm, value2);
    case precision::minute: return set_field_calendar<component::minute>(yqnqdhm, value2);
    case precision::second: return set_field_calendar<component::second>(yqnqdhm, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::second: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqdhms, value2);
    case precision::quarter: return set_field_calendar<component::quarternum>(yqnqdhms, value2);
    case precision::day: return set_field_calendar<component::quarterday>(yqnqdhms, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqdhms, value2);
    case precision::minute: return set_field_calendar<component::minute>(yqnqdhms, value2);
    case precision::second: return set_field_calendar<component::second>(yqnqdhms, value2);
    case precision::millisecond: return set_field_calendar<component::millisecond>(yqnqdhms, value2);
    case precision::microsecond: return set_field_calendar<component::microsecond>(yqnqdhms, value2);
    case precision::nanosecond: return set_field_calendar<component::nanosecond>(yqnqdhms, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::millisecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqdhmss1, value2);
    case precision::quarter: return set_field_calendar<component::quarternum>(yqnqdhmss1, value2);
    case precision::day: return set_field_calendar<component::quarterday>(yqnqdhmss1, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqdhmss1, value2);
    case precision::minute: return set_field_calendar<component::minute>(yqnqdhmss1, value2);
    case precision::second: return set_field_calendar<component::second>(yqnqdhmss1, value2);
    case precision::millisecond: return set_field_calendar<component::millisecond>(yqnqdhmss1, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::microsecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqdhmss2, value2);
    case precision::quarter: return set_field_calendar<component::quarternum>(yqnqdhmss2, value2);
    case precision::day: return set_field_calendar<component::quarterday>(yqnqdhmss2, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqdhmss2, value2);
    case precision::minute: return set_field_calendar<component::minute>(yqnqdhmss2, value2);
    case precision::second: return set_field_calendar<component::second>(yqnqdhmss2, value2);
    case precision::microsecond: return set_field_calendar<component::microsecond>(yqnqdhmss2, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::nanosecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqdhmss3, value2);
    case precision::quarter: return set_field_calendar<component::quarternum>(yqnqdhmss3, value2);
    case precision::day: return set_field_calendar<component::quarterday>(yqnqdhmss3, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqdhmss3, value2);
    case precision::minute: return set_field_calendar<component::minute>(yqnqdhmss3, value2);
    case precision::second: return set_field_calendar<component::second>(yqnqdhmss3, value2);
    case precision::nanosecond: return set_field_calendar<component::nanosecond>(yqnqdhmss3, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
cpp11::writable::list
set_field_year_quarternum_quarterday_cpp(cpp11::list_of<cpp11::integers> fields,
                                         const cpp11::integers& value,
                                         const cpp11::strings& precision_fields,
                                         const cpp11::strings& precision_value,
                                         const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return set_field_year_quarternum_quarterday_impl<start::january>(fields, value, precision_fields, precision_value);
  case start::february: return set_field_year_quarternum_quarterday_impl<start::february>(fields, value, precision_fields, precision_value);
  case start::march: return set_field_year_quarternum_quarterday_impl<start::march>(fields, value, precision_fields, precision_value);
  case start::april: return set_field_year_quarternum_quarterday_impl<start::april>(fields, value, precision_fields, precision_value);
  case start::may: return set_field_year_quarternum_quarterday_impl<start::may>(fields, value, precision_fields, precision_value);
  case start::june: return set_field_year_quarternum_quarterday_impl<start::june>(fields, value, precision_fields, precision_value);
  case start::july: return set_field_year_quarternum_quarterday_impl<start::july>(fields, value, precision_fields, precision_value);
  case start::august: return set_field_year_quarternum_quarterday_impl<start::august>(fields, value, precision_fields, precision_value);
  case start::september: return set_field_year_quarternum_quarterday_impl<start::september>(fields, value, precision_fields, precision_value);
  case start::october: return set_field_year_quarternum_quarterday_impl<start::october>(fields, value, precision_fields, precision_value);
  case start::november: return set_field_year_quarternum_quarterday_impl<start::november>(fields, value, precision_fields, precision_value);
  case start::december: return set_field_year_quarternum_quarterday_impl<start::december>(fields, value, precision_fields, precision_value);
  }
}

template <quarterly::start S, class Calendar>
cpp11::writable::list
set_field_year_quarternum_quarterday_last_impl(const Calendar& x) {
  const r_ssize size = x.size();
  cpp11::writable::integers quarterday(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      quarterday[i] = r_int_na;
    } else {
      quarterly::year_quarternum_quarterday_last<S> yqnqdl = x.to_year_quarternum(i) / quarterly::last;
      quarterday[i] = static_cast<int>(static_cast<unsigned>(yqnqdl.quarterday()));
    }
  }

  cpp11::writable::list out({x.to_list(), quarterday});
  out.names() = {"fields", "value"};

  return out;
}

template <quarterly::start S>
cpp11::writable::list
set_field_year_quarternum_quarterday_last_switch(cpp11::list_of<cpp11::integers> fields,
                                                 const cpp11::strings& precision_fields) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarternum = rquarterly::get_quarternum(fields);
  cpp11::integers quarterday = rquarterly::get_quarterday(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarternum};
  rquarterly::yqnqd<S> yqnqd{year, quarternum, quarterday};
  rquarterly::yqnqdh<S> yqnqdh{year, quarternum, quarterday, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarternum, quarterday, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarternum, quarterday, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarternum, quarterday, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::quarter: return set_field_year_quarternum_quarterday_last_impl<S>(yqn);
  case precision::day: return set_field_year_quarternum_quarterday_last_impl<S>(yqnqd);
  case precision::hour: return set_field_year_quarternum_quarterday_last_impl<S>(yqnqdh);
  case precision::minute: return set_field_year_quarternum_quarterday_last_impl<S>(yqnqdhm);
  case precision::second: return set_field_year_quarternum_quarterday_last_impl<S>(yqnqdhms);
  case precision::millisecond: return set_field_year_quarternum_quarterday_last_impl<S>(yqnqdhmss1);
  case precision::microsecond: return set_field_year_quarternum_quarterday_last_impl<S>(yqnqdhmss2);
  case precision::nanosecond: return set_field_year_quarternum_quarterday_last_impl<S>(yqnqdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
cpp11::writable::list
set_field_year_quarternum_quarterday_last_cpp(cpp11::list_of<cpp11::integers> fields,
                                              const cpp11::strings& precision_fields,
                                              const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return set_field_year_quarternum_quarterday_last_switch<start::january>(fields, precision_fields);
  case start::february: return set_field_year_quarternum_quarterday_last_switch<start::february>(fields, precision_fields);
  case start::march: return set_field_year_quarternum_quarterday_last_switch<start::march>(fields, precision_fields);
  case start::april: return set_field_year_quarternum_quarterday_last_switch<start::april>(fields, precision_fields);
  case start::may: return set_field_year_quarternum_quarterday_last_switch<start::may>(fields, precision_fields);
  case start::june: return set_field_year_quarternum_quarterday_last_switch<start::june>(fields, precision_fields);
  case start::july: return set_field_year_quarternum_quarterday_last_switch<start::july>(fields, precision_fields);
  case start::august: return set_field_year_quarternum_quarterday_last_switch<start::august>(fields, precision_fields);
  case start::september: return set_field_year_quarternum_quarterday_last_switch<start::september>(fields, precision_fields);
  case start::october: return set_field_year_quarternum_quarterday_last_switch<start::october>(fields, precision_fields);
  case start::november: return set_field_year_quarternum_quarterday_last_switch<start::november>(fields, precision_fields);
  case start::december: return set_field_year_quarternum_quarterday_last_switch<start::december>(fields, precision_fields);
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
cpp11::writable::list
year_quarternum_quarterday_plus_duration_impl(cpp11::list_of<cpp11::integers> fields,
                                              cpp11::list_of<cpp11::integers> fields_n,
                                              const cpp11::strings& precision_fields,
                                              const cpp11::strings& precision_n) {
  using namespace rclock;

  const enum precision precision_fields_val = parse_precision(precision_fields);
  const enum precision precision_n_val = parse_precision(precision_n);

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarternum = rquarterly::get_quarternum(fields);
  cpp11::integers quarterday = rquarterly::get_quarterday(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarternum};
  rquarterly::yqnqd<S> yqnqd{year, quarternum, quarterday};
  rquarterly::yqnqdh<S> yqnqdh{year, quarternum, quarterday, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarternum, quarterday, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarternum, quarterday, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarternum, quarterday, hour, minute, second, subsecond};

  cpp11::integers ticks = duration::get_ticks(fields_n);

  duration::years dy{ticks};
  duration::quarters dq{ticks};

  switch (precision_fields_val) {
  case precision::year:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(y, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::quarter:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yqn, dy);
    case precision::quarter: return calendar_plus_duration_impl(yqn, dq);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::day:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yqnqd, dy);
    case precision::quarter: return calendar_plus_duration_impl(yqnqd, dq);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::hour:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yqnqdh, dy);
    case precision::quarter: return calendar_plus_duration_impl(yqnqdh, dq);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::minute:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yqnqdhm, dy);
    case precision::quarter: return calendar_plus_duration_impl(yqnqdhm, dq);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::second:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yqnqdhms, dy);
    case precision::quarter: return calendar_plus_duration_impl(yqnqdhms, dq);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::millisecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yqnqdhmss1, dy);
    case precision::quarter: return calendar_plus_duration_impl(yqnqdhmss1, dq);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::microsecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yqnqdhmss2, dy);
    case precision::quarter: return calendar_plus_duration_impl(yqnqdhmss2, dq);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::nanosecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yqnqdhmss3, dy);
    case precision::quarter: return calendar_plus_duration_impl(yqnqdhmss3, dq);
    default: clock_abort("Internal error: Invalid precision.");
    }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
cpp11::writable::list
year_quarternum_quarterday_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                             cpp11::list_of<cpp11::integers> fields_n,
                                             const cpp11::strings& precision_fields,
                                             const cpp11::strings& precision_n,
                                             const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return year_quarternum_quarterday_plus_duration_impl<start::january>(fields, fields_n, precision_fields, precision_n);
  case start::february: return year_quarternum_quarterday_plus_duration_impl<start::february>(fields, fields_n, precision_fields, precision_n);
  case start::march: return year_quarternum_quarterday_plus_duration_impl<start::march>(fields, fields_n, precision_fields, precision_n);
  case start::april: return year_quarternum_quarterday_plus_duration_impl<start::april>(fields, fields_n, precision_fields, precision_n);
  case start::may: return year_quarternum_quarterday_plus_duration_impl<start::may>(fields, fields_n, precision_fields, precision_n);
  case start::june: return year_quarternum_quarterday_plus_duration_impl<start::june>(fields, fields_n, precision_fields, precision_n);
  case start::july: return year_quarternum_quarterday_plus_duration_impl<start::july>(fields, fields_n, precision_fields, precision_n);
  case start::august: return year_quarternum_quarterday_plus_duration_impl<start::august>(fields, fields_n, precision_fields, precision_n);
  case start::september: return year_quarternum_quarterday_plus_duration_impl<start::september>(fields, fields_n, precision_fields, precision_n);
  case start::october: return year_quarternum_quarterday_plus_duration_impl<start::october>(fields, fields_n, precision_fields, precision_n);
  case start::november: return year_quarternum_quarterday_plus_duration_impl<start::november>(fields, fields_n, precision_fields, precision_n);
  case start::december: return year_quarternum_quarterday_plus_duration_impl<start::december>(fields, fields_n, precision_fields, precision_n);
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
cpp11::writable::list
as_sys_time_year_quarternum_quarterday_impl(cpp11::list_of<cpp11::integers> fields,
                                            const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarternum = rquarterly::get_quarternum(fields);
  cpp11::integers quarterday = rquarterly::get_quarterday(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarternum};
  rquarterly::yqnqd<S> yqnqd{year, quarternum, quarterday};
  rquarterly::yqnqdh<S> yqnqdh{year, quarternum, quarterday, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarternum, quarterday, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarternum, quarterday, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarternum, quarterday, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarternum, quarterday, hour, minute, second, subsecond};

  switch (parse_precision(precision)) {
  case precision::day: return as_sys_time_from_calendar_impl<duration::days>(yqnqd);
  case precision::hour: return as_sys_time_from_calendar_impl<duration::hours>(yqnqdh);
  case precision::minute: return as_sys_time_from_calendar_impl<duration::minutes>(yqnqdhm);
  case precision::second: return as_sys_time_from_calendar_impl<duration::seconds>(yqnqdhms);
  case precision::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(yqnqdhmss1);
  case precision::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(yqnqdhmss2);
  case precision::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(yqnqdhmss3);
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

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_quarternum_quarterday_cpp(cpp11::list_of<cpp11::integers> fields,
                                           const cpp11::strings& precision,
                                           const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return as_sys_time_year_quarternum_quarterday_impl<start::january>(fields, precision);
  case start::february: return as_sys_time_year_quarternum_quarterday_impl<start::february>(fields, precision);
  case start::march: return as_sys_time_year_quarternum_quarterday_impl<start::march>(fields, precision);
  case start::april: return as_sys_time_year_quarternum_quarterday_impl<start::april>(fields, precision);
  case start::may: return as_sys_time_year_quarternum_quarterday_impl<start::may>(fields, precision);
  case start::june: return as_sys_time_year_quarternum_quarterday_impl<start::june>(fields, precision);
  case start::july: return as_sys_time_year_quarternum_quarterday_impl<start::july>(fields, precision);
  case start::august: return as_sys_time_year_quarternum_quarterday_impl<start::august>(fields, precision);
  case start::september: return as_sys_time_year_quarternum_quarterday_impl<start::september>(fields, precision);
  case start::october: return as_sys_time_year_quarternum_quarterday_impl<start::october>(fields, precision);
  case start::november: return as_sys_time_year_quarternum_quarterday_impl<start::november>(fields, precision);
  case start::december: return as_sys_time_year_quarternum_quarterday_impl<start::december>(fields, precision);
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
cpp11::writable::list
as_year_quarternum_quarterday_from_sys_time_impl(cpp11::list_of<cpp11::integers> fields,
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
  case precision::day: return as_calendar_from_sys_time_impl<rquarterly::yqnqd<S>>(dd);
  case precision::hour: return as_calendar_from_sys_time_impl<rquarterly::yqnqdh<S>>(dh);
  case precision::minute: return as_calendar_from_sys_time_impl<rquarterly::yqnqdhm<S>>(dmin);
  case precision::second: return as_calendar_from_sys_time_impl<rquarterly::yqnqdhms<S>>(ds);
  case precision::millisecond: return as_calendar_from_sys_time_impl<rquarterly::yqnqdhmss<std::chrono::milliseconds, S>>(dmilli);
  case precision::microsecond: return as_calendar_from_sys_time_impl<rquarterly::yqnqdhmss<std::chrono::microseconds, S>>(dmicro);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<rquarterly::yqnqdhmss<std::chrono::nanoseconds, S>>(dnano);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
cpp11::writable::list
as_year_quarternum_quarterday_from_sys_time_cpp(cpp11::list_of<cpp11::integers> fields,
                                                const cpp11::strings& precision,
                                                const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return as_year_quarternum_quarterday_from_sys_time_impl<start::january>(fields, precision);
  case start::february: return as_year_quarternum_quarterday_from_sys_time_impl<start::february>(fields, precision);
  case start::march: return as_year_quarternum_quarterday_from_sys_time_impl<start::march>(fields, precision);
  case start::april: return as_year_quarternum_quarterday_from_sys_time_impl<start::april>(fields, precision);
  case start::may: return as_year_quarternum_quarterday_from_sys_time_impl<start::may>(fields, precision);
  case start::june: return as_year_quarternum_quarterday_from_sys_time_impl<start::june>(fields, precision);
  case start::july: return as_year_quarternum_quarterday_from_sys_time_impl<start::july>(fields, precision);
  case start::august: return as_year_quarternum_quarterday_from_sys_time_impl<start::august>(fields, precision);
  case start::september: return as_year_quarternum_quarterday_from_sys_time_impl<start::september>(fields, precision);
  case start::october: return as_year_quarternum_quarterday_from_sys_time_impl<start::october>(fields, precision);
  case start::november: return as_year_quarternum_quarterday_from_sys_time_impl<start::november>(fields, precision);
  case start::december: return as_year_quarternum_quarterday_from_sys_time_impl<start::december>(fields, precision);
  }
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
inline
cpp11::writable::list
year_minus_year_impl(const rclock::rquarterly::y<S>& x,
                     const rclock::rquarterly::y<S>& y) {
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

template <quarterly::start S>
static
inline
cpp11::writable::list
year_quarternum_minus_year_quarternum_impl(const rclock::rquarterly::yqn<S>& x,
                                           const rclock::rquarterly::yqn<S>& y) {
  const r_ssize size = x.size();
  rclock::duration::quarters out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || y.is_na(i)) {
      out.assign_na(i);
      continue;
    }
    out.assign(x.to_year_quarternum(i) - y.to_year_quarternum(i), i);
  }

  return out.to_list();
}

template <quarterly::start S>
cpp11::writable::list
year_quarternum_quarterday_minus_year_quarternum_quarterday_impl(cpp11::list_of<cpp11::integers> x,
                                                                 cpp11::list_of<cpp11::integers> y,
                                                                 const cpp11::strings& precision) {
  const cpp11::integers x_year = rclock::rquarterly::get_year(x);
  const cpp11::integers x_quarternum = rclock::rquarterly::get_quarternum(x);

  const cpp11::integers y_year = rclock::rquarterly::get_year(y);
  const cpp11::integers y_quarternum = rclock::rquarterly::get_quarternum(y);

  const rclock::rquarterly::y<S> x_y{x_year};
  const rclock::rquarterly::yqn<S> x_yqn{x_year, x_quarternum};

  const rclock::rquarterly::y<S> y_y{y_year};
  const rclock::rquarterly::yqn<S> y_yqn{y_year, y_quarternum};

  switch (parse_precision(precision)) {
  case precision::year: return year_minus_year_impl(x_y, y_y);
  case precision::quarter: return year_quarternum_minus_year_quarternum_impl(x_yqn, y_yqn);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

[[cpp11::register]]
cpp11::writable::list
year_quarternum_quarterday_minus_year_quarternum_quarterday_cpp(cpp11::list_of<cpp11::integers> x,
                                                                cpp11::list_of<cpp11::integers> y,
                                                                const cpp11::strings& precision,
                                                                const cpp11::integers& start) {
  using namespace quarterly;

  switch (parse_start(start)) {
  case start::january: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::january>(x, y, precision);
  case start::february: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::february>(x, y, precision);
  case start::march: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::march>(x, y, precision);
  case start::april: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::april>(x, y, precision);
  case start::may: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::may>(x, y, precision);
  case start::june: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::june>(x, y, precision);
  case start::july: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::july>(x, y, precision);
  case start::august: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::august>(x, y, precision);
  case start::september: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::september>(x, y, precision);
  case start::october: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::october>(x, y, precision);
  case start::november: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::november>(x, y, precision);
  case start::december: return year_quarternum_quarterday_minus_year_quarternum_quarterday_impl<start::december>(x, y, precision);
  }
}
