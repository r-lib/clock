#include "iso-year-week-day.h"
#include "calendar.h"
#include "duration.h"
#include "check.h"
#include "enums.h"
#include "get.h"
#include "rcrd.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
new_iso_year_week_day_from_fields(SEXP fields,
                                  const cpp11::integers& precision_int,
                                  SEXP names) {
  const enum precision precision_val = parse_precision(precision_int);

  const r_ssize n_fields = Rf_xlength(fields);

  r_ssize n;
  switch (precision_val) {
  case precision::year: n = 1; break;
  case precision::week: n = 2; break;
  case precision::day: n = 3; break;
  case precision::hour: n = 4; break;
  case precision::minute: n = 5; break;
  case precision::second: n = 6; break;
  case precision::millisecond: n = 7; break;
  case precision::microsecond: n = 7; break;
  case precision::nanosecond: n = 7; break;
  default: never_reached("new_iso_year_week_day_from_fields");
  }

  if (n != n_fields) {
    clock_abort("With the given precision, `fields` must have length %i, not %i.", n, n_fields);
  }

  SEXP out = PROTECT(new_clock_rcrd_from_fields(fields, names, classes_iso_year_week_day));

  Rf_setAttrib(out, syms_precision, precision_int);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
iso_year_week_day_restore(SEXP x, SEXP to) {
  SEXP precision = Rf_getAttrib(to, syms_precision);

  SEXP out = PROTECT(clock_rcrd_restore(x, to, classes_iso_year_week_day));

  Rf_setAttrib(out, syms_precision, precision);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
collect_iso_year_week_day_fields(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, week};
  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: {
    collect_field<component::year>(y, year, "year");
    return y.to_list();
  }
  case precision::week: {
    collect_field<component::year>(ywn, year, "year");
    collect_field<component::week>(ywn, week, "week");
    return ywn.to_list();
  }
  case precision::day: {
    collect_field<component::year>(ywnwd, year, "year");
    collect_field<component::week>(ywnwd, week, "week");
    collect_field<component::day>(ywnwd, day, "day");
    return ywnwd.to_list();
  }
  case precision::hour: {
    collect_field<component::year>(ywnwdh, year, "year");
    collect_field<component::week>(ywnwdh, week, "week");
    collect_field<component::day>(ywnwdh, day, "day");
    collect_field<component::hour>(ywnwdh, hour, "hour");
    return ywnwdh.to_list();
  }
  case precision::minute: {
    collect_field<component::year>(ywnwdhm, year, "year");
    collect_field<component::week>(ywnwdhm, week, "week");
    collect_field<component::day>(ywnwdhm, day, "day");
    collect_field<component::hour>(ywnwdhm, hour, "hour");
    collect_field<component::minute>(ywnwdhm, minute, "minute");
    return ywnwdhm.to_list();
  }
  case precision::second: {
    collect_field<component::year>(ywnwdhms, year, "year");
    collect_field<component::week>(ywnwdhms, week, "week");
    collect_field<component::day>(ywnwdhms, day, "day");
    collect_field<component::hour>(ywnwdhms, hour, "hour");
    collect_field<component::minute>(ywnwdhms, minute, "minute");
    collect_field<component::second>(ywnwdhms, second, "second");
    return ywnwdhms.to_list();
  }
  case precision::millisecond: {
    collect_field<component::year>(ywnwdhmss1, year, "year");
    collect_field<component::week>(ywnwdhmss1, week, "week");
    collect_field<component::day>(ywnwdhmss1, day, "day");
    collect_field<component::hour>(ywnwdhmss1, hour, "hour");
    collect_field<component::minute>(ywnwdhmss1, minute, "minute");
    collect_field<component::second>(ywnwdhmss1, second, "second");
    collect_field<component::millisecond>(ywnwdhmss1, subsecond, "subsecond");
    return ywnwdhmss1.to_list();
  }
  case precision::microsecond: {
    collect_field<component::year>(ywnwdhmss2, year, "year");
    collect_field<component::week>(ywnwdhmss2, week, "week");
    collect_field<component::day>(ywnwdhmss2, day, "day");
    collect_field<component::hour>(ywnwdhmss2, hour, "hour");
    collect_field<component::minute>(ywnwdhmss2, minute, "minute");
    collect_field<component::second>(ywnwdhmss2, second, "second");
    collect_field<component::microsecond>(ywnwdhmss2, subsecond, "subsecond");
    return ywnwdhmss2.to_list();
  }
  case precision::nanosecond: {
    collect_field<component::year>(ywnwdhmss3, year, "year");
    collect_field<component::week>(ywnwdhmss3, week, "week");
    collect_field<component::day>(ywnwdhmss3, day, "day");
    collect_field<component::hour>(ywnwdhmss3, hour, "hour");
    collect_field<component::minute>(ywnwdhmss3, minute, "minute");
    collect_field<component::second>(ywnwdhmss3, second, "second");
    collect_field<component::nanosecond>(ywnwdhmss3, subsecond, "subsecond");
    return ywnwdhmss3.to_list();
  }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("collect_iso_year_week_day_fields");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
format_iso_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                             const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, week};
  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
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

  never_reached("format_iso_year_week_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_iso_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                     const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, week};
  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
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

  never_reached("invalid_detect_iso_year_week_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_iso_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, week};
  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
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

  never_reached("invalid_any_iso_year_week_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_iso_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                    const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, week};
  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
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

  never_reached("invalid_count_iso_year_week_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_iso_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                      const cpp11::integers& precision_int,
                                      const cpp11::strings& invalid_string) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid_string);

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, week};
  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
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

  never_reached("invalid_resolve_iso_year_week_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
set_field_iso_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                const cpp11::integers& value,
                                const cpp11::integers& precision_fields,
                                const cpp11::integers& precision_value) {
  using namespace rclock;
  rclock::integers value2(value);

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, week};
  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::year: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(y, value2);
    case precision::week: return set_field_calendar<component::week>(y, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::week: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywn, value2);
    case precision::week: return set_field_calendar<component::week>(ywn, value2);
    case precision::day: return set_field_calendar<component::day>(ywn, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::day: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwd, value2);
    case precision::week: return set_field_calendar<component::week>(ywnwd, value2);
    case precision::day: return set_field_calendar<component::day>(ywnwd, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwd, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::hour: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwdh, value2);
    case precision::week: return set_field_calendar<component::week>(ywnwdh, value2);
    case precision::day: return set_field_calendar<component::day>(ywnwdh, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwdh, value2);
    case precision::minute: return set_field_calendar<component::minute>(ywnwdh, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::minute: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwdhm, value2);
    case precision::week: return set_field_calendar<component::week>(ywnwdhm, value2);
    case precision::day: return set_field_calendar<component::day>(ywnwdhm, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwdhm, value2);
    case precision::minute: return set_field_calendar<component::minute>(ywnwdhm, value2);
    case precision::second: return set_field_calendar<component::second>(ywnwdhm, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::second: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(ywnwdhms, value2);
    case precision::week: return set_field_calendar<component::week>(ywnwdhms, value2);
    case precision::day: return set_field_calendar<component::day>(ywnwdhms, value2);
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
    case precision::week: return set_field_calendar<component::week>(ywnwdhmss1, value2);
    case precision::day: return set_field_calendar<component::day>(ywnwdhmss1, value2);
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
    case precision::week: return set_field_calendar<component::week>(ywnwdhmss2, value2);
    case precision::day: return set_field_calendar<component::day>(ywnwdhmss2, value2);
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
    case precision::week: return set_field_calendar<component::week>(ywnwdhmss3, value2);
    case precision::day: return set_field_calendar<component::day>(ywnwdhmss3, value2);
    case precision::hour: return set_field_calendar<component::hour>(ywnwdhmss3, value2);
    case precision::minute: return set_field_calendar<component::minute>(ywnwdhmss3, value2);
    case precision::second: return set_field_calendar<component::second>(ywnwdhmss3, value2);
    case precision::nanosecond: return set_field_calendar<component::nanosecond>(ywnwdhmss3, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("set_field_iso_year_week_day_cpp");
}

template <class Calendar>
cpp11::writable::list
set_field_iso_year_week_day_last_impl(const Calendar& x) {
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
set_field_iso_year_week_day_last_cpp(cpp11::list_of<cpp11::integers> fields,
                                     const cpp11::integers& precision_fields) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, week};
  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::year: return set_field_iso_year_week_day_last_impl(y);
  case precision::week: return set_field_iso_year_week_day_last_impl(ywn);
  case precision::day: return set_field_iso_year_week_day_last_impl(ywnwd);
  case precision::hour: return set_field_iso_year_week_day_last_impl(ywnwdh);
  case precision::minute: return set_field_iso_year_week_day_last_impl(ywnwdhm);
  case precision::second: return set_field_iso_year_week_day_last_impl(ywnwdhms);
  case precision::millisecond: return set_field_iso_year_week_day_last_impl(ywnwdhmss1);
  case precision::microsecond: return set_field_iso_year_week_day_last_impl(ywnwdhmss2);
  case precision::nanosecond: return set_field_iso_year_week_day_last_impl(ywnwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("set_field_iso_year_week_day_last_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
iso_year_week_day_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                    cpp11::list_of<cpp11::integers> fields_n,
                                    const cpp11::integers& precision_fields,
                                    const cpp11::integers& precision_n) {
  using namespace rclock;

  const enum precision precision_fields_val = parse_precision(precision_fields);
  const enum precision precision_n_val = parse_precision(precision_n);

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::y y{year};
  iso::ywn ywn{year, week};
  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

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

  never_reached("iso_year_week_day_plus_duration_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_sys_time_iso_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::day: return as_sys_time_from_calendar_impl<duration::days>(ywnwd);
  case precision::hour: return as_sys_time_from_calendar_impl<duration::hours>(ywnwdh);
  case precision::minute: return as_sys_time_from_calendar_impl<duration::minutes>(ywnwdhm);
  case precision::second: return as_sys_time_from_calendar_impl<duration::seconds>(ywnwdhms);
  case precision::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(ywnwdhmss1);
  case precision::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(ywnwdhmss2);
  case precision::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(ywnwdhmss3);
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

  never_reached("as_sys_time_iso_year_week_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_iso_year_week_day_from_sys_time_cpp(cpp11::list_of<cpp11::integers> fields,
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
  case precision::day: return as_calendar_from_sys_time_impl<iso::ywnwd>(dd);
  case precision::hour: return as_calendar_from_sys_time_impl<iso::ywnwdh>(dh);
  case precision::minute: return as_calendar_from_sys_time_impl<iso::ywnwdhm>(dmin);
  case precision::second: return as_calendar_from_sys_time_impl<iso::ywnwdhms>(ds);
  case precision::millisecond: return as_calendar_from_sys_time_impl<iso::ywnwdhmss<std::chrono::milliseconds>>(dmilli);
  case precision::microsecond: return as_calendar_from_sys_time_impl<iso::ywnwdhmss<std::chrono::microseconds>>(dmicro);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<iso::ywnwdhmss<std::chrono::nanoseconds>>(dnano);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("as_iso_year_week_day_from_sys_time_cpp");
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
iso_year_week_day_minus_iso_year_week_day_cpp(cpp11::list_of<cpp11::integers> x,
                                              cpp11::list_of<cpp11::integers> y,
                                              const cpp11::integers& precision_int) {
  const cpp11::integers x_year = rclock::iso::get_year(x);
  const cpp11::integers y_year = rclock::iso::get_year(y);

  const rclock::iso::y x_y{x_year};
  const rclock::iso::y y_y{y_year};

  switch (parse_precision(precision_int)) {
  case precision::year: return year_minus_year_impl(x_y, y_y);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("iso_year_week_day_minus_iso_year_week_day_cpp");
}
