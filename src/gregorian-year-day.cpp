#include "gregorian-year-day.h"
#include "calendar.h"
#include "duration.h"
#include "check.h"
#include "enums.h"
#include "get.h"
#include "rcrd.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
new_year_day_from_fields(SEXP fields,
                         const cpp11::integers& precision_int,
                         SEXP names) {
  const enum precision precision_val = parse_precision(precision_int);

  const r_ssize n_fields = Rf_xlength(fields);

  r_ssize n;
  switch (precision_val) {
  case precision::year: n = 1; break;
  case precision::day: n = 2; break;
  case precision::hour: n = 3; break;
  case precision::minute: n = 4; break;
  case precision::second: n = 5; break;
  case precision::millisecond: n = 6; break;
  case precision::microsecond: n = 6; break;
  case precision::nanosecond: n = 6; break;
  default: never_reached("new_year_day_from_fields");
  }

  if (n != n_fields) {
    clock_abort("With the given precision, `fields` must have length %i, not %i.", n, n_fields);
  }

  SEXP out = PROTECT(new_clock_rcrd_from_fields(fields, names, classes_year_day));

  Rf_setAttrib(out, syms_precision, precision_int);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
year_day_restore(SEXP x, SEXP to) {
  SEXP precision = Rf_getAttrib(to, syms_precision);

  SEXP out = PROTECT(clock_rcrd_restore(x, to, classes_year_day));

  Rf_setAttrib(out, syms_precision, precision);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
collect_year_day_fields(cpp11::list_of<cpp11::integers> fields,
                        const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = yearday::get_year(fields);
  cpp11::integers day = yearday::get_day(fields);
  cpp11::integers hour = yearday::get_hour(fields);
  cpp11::integers minute = yearday::get_minute(fields);
  cpp11::integers second = yearday::get_second(fields);
  cpp11::integers subsecond = yearday::get_subsecond(fields);

  yearday::y y{year};
  yearday::yyd yyd{year, day};
  yearday::yydh yydh{year, day, hour};
  yearday::yydhm yydhm{year, day, hour, minute};
  yearday::yydhms yydhms{year, day, hour, minute, second};
  yearday::yydhmss<std::chrono::milliseconds> yydhmss1{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::microseconds> yydhmss2{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::nanoseconds> yydhmss3{year, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: {
    collect_field<component::year>(y, year, "year");
    return y.to_list();
  }
  case precision::day: {
    collect_field<component::year>(yyd, year, "year");
    collect_field<component::day>(yyd, day, "day");
    return yyd.to_list();
  }
  case precision::hour: {
    collect_field<component::year>(yydh, year, "year");
    collect_field<component::day>(yydh, day, "day");
    collect_field<component::hour>(yydh, hour, "hour");
    return yydh.to_list();
  }
  case precision::minute: {
    collect_field<component::year>(yydhm, year, "year");
    collect_field<component::day>(yydhm, day, "day");
    collect_field<component::hour>(yydhm, hour, "hour");
    collect_field<component::minute>(yydhm, minute, "minute");
    return yydhm.to_list();
  }
  case precision::second: {
    collect_field<component::year>(yydhms, year, "year");
    collect_field<component::day>(yydhms, day, "day");
    collect_field<component::hour>(yydhms, hour, "hour");
    collect_field<component::minute>(yydhms, minute, "minute");
    collect_field<component::second>(yydhms, second, "second");
    return yydhms.to_list();
  }
  case precision::millisecond: {
    collect_field<component::year>(yydhmss1, year, "year");
    collect_field<component::day>(yydhmss1, day, "day");
    collect_field<component::hour>(yydhmss1, hour, "hour");
    collect_field<component::minute>(yydhmss1, minute, "minute");
    collect_field<component::second>(yydhmss1, second, "second");
    collect_field<component::millisecond>(yydhmss1, subsecond, "subsecond");
    return yydhmss1.to_list();
  }
  case precision::microsecond: {
    collect_field<component::year>(yydhmss2, year, "year");
    collect_field<component::day>(yydhmss2, day, "day");
    collect_field<component::hour>(yydhmss2, hour, "hour");
    collect_field<component::minute>(yydhmss2, minute, "minute");
    collect_field<component::second>(yydhmss2, second, "second");
    collect_field<component::microsecond>(yydhmss2, subsecond, "subsecond");
    return yydhmss2.to_list();
  }
  case precision::nanosecond: {
    collect_field<component::year>(yydhmss3, year, "year");
    collect_field<component::day>(yydhmss3, day, "day");
    collect_field<component::hour>(yydhmss3, hour, "hour");
    collect_field<component::minute>(yydhmss3, minute, "minute");
    collect_field<component::second>(yydhmss3, second, "second");
    collect_field<component::nanosecond>(yydhmss3, subsecond, "subsecond");
    return yydhmss3.to_list();
  }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("collect_year_day_fields");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
format_year_day_cpp(cpp11::list_of<cpp11::integers> fields,
                    const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = yearday::get_year(fields);
  cpp11::integers day = yearday::get_day(fields);
  cpp11::integers hour = yearday::get_hour(fields);
  cpp11::integers minute = yearday::get_minute(fields);
  cpp11::integers second = yearday::get_second(fields);
  cpp11::integers subsecond = yearday::get_subsecond(fields);

  yearday::y y{year};
  yearday::yyd yyd{year, day};
  yearday::yydh yydh{year, day, hour};
  yearday::yydhm yydhm{year, day, hour, minute};
  yearday::yydhms yydhms{year, day, hour, minute, second};
  yearday::yydhmss<std::chrono::milliseconds> yydhmss1{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::microseconds> yydhmss2{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::nanoseconds> yydhmss3{year, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return format_calendar_impl(y);
  case precision::day: return format_calendar_impl(yyd);
  case precision::hour: return format_calendar_impl(yydh);
  case precision::minute: return format_calendar_impl(yydhm);
  case precision::second: return format_calendar_impl(yydhms);
  case precision::millisecond: return format_calendar_impl(yydhmss1);
  case precision::microsecond: return format_calendar_impl(yydhmss2);
  case precision::nanosecond: return format_calendar_impl(yydhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("format_year_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_day_cpp(cpp11::list_of<cpp11::integers> fields,
                            const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = yearday::get_year(fields);
  cpp11::integers day = yearday::get_day(fields);
  cpp11::integers hour = yearday::get_hour(fields);
  cpp11::integers minute = yearday::get_minute(fields);
  cpp11::integers second = yearday::get_second(fields);
  cpp11::integers subsecond = yearday::get_subsecond(fields);

  yearday::y y{year};
  yearday::yyd yyd{year, day};
  yearday::yydh yydh{year, day, hour};
  yearday::yydhm yydhm{year, day, hour, minute};
  yearday::yydhms yydhms{year, day, hour, minute, second};
  yearday::yydhmss<std::chrono::milliseconds> yydhmss1{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::microseconds> yydhmss2{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::nanoseconds> yydhmss3{year, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_detect_calendar_impl(y);
  case precision::day: return invalid_detect_calendar_impl(yyd);
  case precision::hour: return invalid_detect_calendar_impl(yydh);
  case precision::minute: return invalid_detect_calendar_impl(yydhm);
  case precision::second: return invalid_detect_calendar_impl(yydhms);
  case precision::millisecond: return invalid_detect_calendar_impl(yydhmss1);
  case precision::microsecond: return invalid_detect_calendar_impl(yydhmss2);
  case precision::nanosecond: return invalid_detect_calendar_impl(yydhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_detect_year_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_year_day_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = yearday::get_year(fields);
  cpp11::integers day = yearday::get_day(fields);
  cpp11::integers hour = yearday::get_hour(fields);
  cpp11::integers minute = yearday::get_minute(fields);
  cpp11::integers second = yearday::get_second(fields);
  cpp11::integers subsecond = yearday::get_subsecond(fields);

  yearday::y y{year};
  yearday::yyd yyd{year, day};
  yearday::yydh yydh{year, day, hour};
  yearday::yydhm yydhm{year, day, hour, minute};
  yearday::yydhms yydhms{year, day, hour, minute, second};
  yearday::yydhmss<std::chrono::milliseconds> yydhmss1{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::microseconds> yydhmss2{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::nanoseconds> yydhmss3{year, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_any_calendar_impl(y);
  case precision::day: return invalid_any_calendar_impl(yyd);
  case precision::hour: return invalid_any_calendar_impl(yydh);
  case precision::minute: return invalid_any_calendar_impl(yydhm);
  case precision::second: return invalid_any_calendar_impl(yydhms);
  case precision::millisecond: return invalid_any_calendar_impl(yydhmss1);
  case precision::microsecond: return invalid_any_calendar_impl(yydhmss2);
  case precision::nanosecond: return invalid_any_calendar_impl(yydhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_any_year_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_year_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = yearday::get_year(fields);
  cpp11::integers day = yearday::get_day(fields);
  cpp11::integers hour = yearday::get_hour(fields);
  cpp11::integers minute = yearday::get_minute(fields);
  cpp11::integers second = yearday::get_second(fields);
  cpp11::integers subsecond = yearday::get_subsecond(fields);

  yearday::y y{year};
  yearday::yyd yyd{year, day};
  yearday::yydh yydh{year, day, hour};
  yearday::yydhm yydhm{year, day, hour, minute};
  yearday::yydhms yydhms{year, day, hour, minute, second};
  yearday::yydhmss<std::chrono::milliseconds> yydhmss1{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::microseconds> yydhmss2{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::nanoseconds> yydhmss3{year, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_count_calendar_impl(y);
  case precision::day: return invalid_count_calendar_impl(yyd);
  case precision::hour: return invalid_count_calendar_impl(yydh);
  case precision::minute: return invalid_count_calendar_impl(yydhm);
  case precision::second: return invalid_count_calendar_impl(yydhms);
  case precision::millisecond: return invalid_count_calendar_impl(yydhmss1);
  case precision::microsecond: return invalid_count_calendar_impl(yydhmss2);
  case precision::nanosecond: return invalid_count_calendar_impl(yydhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_count_year_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                   const cpp11::integers& precision_int,
                                   const cpp11::strings& invalid_string) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid_string);

  cpp11::integers year = yearday::get_year(fields);
  cpp11::integers day = yearday::get_day(fields);
  cpp11::integers hour = yearday::get_hour(fields);
  cpp11::integers minute = yearday::get_minute(fields);
  cpp11::integers second = yearday::get_second(fields);
  cpp11::integers subsecond = yearday::get_subsecond(fields);

  yearday::y y{year};
  yearday::yyd yyd{year, day};
  yearday::yydh yydh{year, day, hour};
  yearday::yydhm yydhm{year, day, hour, minute};
  yearday::yydhms yydhms{year, day, hour, minute, second};
  yearday::yydhmss<std::chrono::milliseconds> yydhmss1{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::microseconds> yydhmss2{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::nanoseconds> yydhmss3{year, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: return invalid_resolve_calendar_impl(y, invalid_val);
  case precision::day: return invalid_resolve_calendar_impl(yyd, invalid_val);
  case precision::hour: return invalid_resolve_calendar_impl(yydh, invalid_val);
  case precision::minute: return invalid_resolve_calendar_impl(yydhm, invalid_val);
  case precision::second: return invalid_resolve_calendar_impl(yydhms, invalid_val);
  case precision::millisecond: return invalid_resolve_calendar_impl(yydhmss1, invalid_val);
  case precision::microsecond: return invalid_resolve_calendar_impl(yydhmss2, invalid_val);
  case precision::nanosecond: return invalid_resolve_calendar_impl(yydhmss3, invalid_val);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("invalid_resolve_year_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
set_field_year_day_cpp(cpp11::list_of<cpp11::integers> fields,
                       const cpp11::integers& value,
                       const cpp11::integers& precision_fields,
                       const cpp11::integers& precision_value) {
  using namespace rclock;
  rclock::integers value2(value);

  cpp11::integers year = yearday::get_year(fields);
  cpp11::integers day = yearday::get_day(fields);
  cpp11::integers hour = yearday::get_hour(fields);
  cpp11::integers minute = yearday::get_minute(fields);
  cpp11::integers second = yearday::get_second(fields);
  cpp11::integers subsecond = yearday::get_subsecond(fields);

  yearday::y y{year};
  yearday::yyd yyd{year, day};
  yearday::yydh yydh{year, day, hour};
  yearday::yydhm yydhm{year, day, hour, minute};
  yearday::yydhms yydhms{year, day, hour, minute, second};
  yearday::yydhmss<std::chrono::milliseconds> yydhmss1{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::microseconds> yydhmss2{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::nanoseconds> yydhmss3{year, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::year: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(y, value2);
    case precision::day: return set_field_calendar<component::day>(y, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::day: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yyd, value2);
    case precision::day: return set_field_calendar<component::day>(yyd, value2);
    case precision::hour: return set_field_calendar<component::hour>(yyd, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::hour: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yydh, value2);
    case precision::day: return set_field_calendar<component::day>(yydh, value2);
    case precision::hour: return set_field_calendar<component::hour>(yydh, value2);
    case precision::minute: return set_field_calendar<component::minute>(yydh, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::minute: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yydhm, value2);
    case precision::day: return set_field_calendar<component::day>(yydhm, value2);
    case precision::hour: return set_field_calendar<component::hour>(yydhm, value2);
    case precision::minute: return set_field_calendar<component::minute>(yydhm, value2);
    case precision::second: return set_field_calendar<component::second>(yydhm, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::second: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yydhms, value2);
    case precision::day: return set_field_calendar<component::day>(yydhms, value2);
    case precision::hour: return set_field_calendar<component::hour>(yydhms, value2);
    case precision::minute: return set_field_calendar<component::minute>(yydhms, value2);
    case precision::second: return set_field_calendar<component::second>(yydhms, value2);
    case precision::millisecond: return set_field_calendar<component::millisecond>(yydhms, value2);
    case precision::microsecond: return set_field_calendar<component::microsecond>(yydhms, value2);
    case precision::nanosecond: return set_field_calendar<component::nanosecond>(yydhms, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::millisecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yydhmss1, value2);
    case precision::day: return set_field_calendar<component::day>(yydhmss1, value2);
    case precision::hour: return set_field_calendar<component::hour>(yydhmss1, value2);
    case precision::minute: return set_field_calendar<component::minute>(yydhmss1, value2);
    case precision::second: return set_field_calendar<component::second>(yydhmss1, value2);
    case precision::millisecond: return set_field_calendar<component::millisecond>(yydhmss1, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::microsecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yydhmss2, value2);
    case precision::day: return set_field_calendar<component::day>(yydhmss2, value2);
    case precision::hour: return set_field_calendar<component::hour>(yydhmss2, value2);
    case precision::minute: return set_field_calendar<component::minute>(yydhmss2, value2);
    case precision::second: return set_field_calendar<component::second>(yydhmss2, value2);
    case precision::microsecond: return set_field_calendar<component::microsecond>(yydhmss2, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::nanosecond: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yydhmss3, value2);
    case precision::day: return set_field_calendar<component::day>(yydhmss3, value2);
    case precision::hour: return set_field_calendar<component::hour>(yydhmss3, value2);
    case precision::minute: return set_field_calendar<component::minute>(yydhmss3, value2);
    case precision::second: return set_field_calendar<component::second>(yydhmss3, value2);
    case precision::nanosecond: return set_field_calendar<component::nanosecond>(yydhmss3, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("set_field_year_day_cpp");
}

template <class Calendar>
cpp11::writable::list
set_field_year_day_last_impl(const Calendar& x) {
  const r_ssize size = x.size();
  cpp11::writable::integers day(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      day[i] = r_int_na;
    } else {
      ordinal::year_yearday_last yydl = x.to_year(i) / ordinal::last;
      day[i] = static_cast<int>(static_cast<unsigned>(yydl.yearday()));
    }
  }

  cpp11::writable::list out({x.to_list(), day});
  out.names() = {"fields", "value"};

  return out;
}

[[cpp11::register]]
cpp11::writable::list
set_field_year_day_last_cpp(cpp11::list_of<cpp11::integers> fields,
                            const cpp11::integers& precision_fields) {
  using namespace rclock;

  cpp11::integers year = yearday::get_year(fields);
  cpp11::integers day = yearday::get_day(fields);
  cpp11::integers hour = yearday::get_hour(fields);
  cpp11::integers minute = yearday::get_minute(fields);
  cpp11::integers second = yearday::get_second(fields);
  cpp11::integers subsecond = yearday::get_subsecond(fields);

  yearday::y y{year};
  yearday::yyd yyd{year, day};
  yearday::yydh yydh{year, day, hour};
  yearday::yydhm yydhm{year, day, hour, minute};
  yearday::yydhms yydhms{year, day, hour, minute, second};
  yearday::yydhmss<std::chrono::milliseconds> yydhmss1{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::microseconds> yydhmss2{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::nanoseconds> yydhmss3{year, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::year: return set_field_year_day_last_impl(y);
  case precision::day: return set_field_year_day_last_impl(yyd);
  case precision::hour: return set_field_year_day_last_impl(yydh);
  case precision::minute: return set_field_year_day_last_impl(yydhm);
  case precision::second: return set_field_year_day_last_impl(yydhms);
  case precision::millisecond: return set_field_year_day_last_impl(yydhmss1);
  case precision::microsecond: return set_field_year_day_last_impl(yydhmss2);
  case precision::nanosecond: return set_field_year_day_last_impl(yydhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("set_field_year_day_last_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
year_day_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                           cpp11::list_of<cpp11::integers> fields_n,
                           const cpp11::integers& precision_fields,
                           const cpp11::integers& precision_n) {
  using namespace rclock;

  const enum precision precision_fields_val = parse_precision(precision_fields);
  const enum precision precision_n_val = parse_precision(precision_n);

  cpp11::integers year = yearday::get_year(fields);
  cpp11::integers day = yearday::get_day(fields);
  cpp11::integers hour = yearday::get_hour(fields);
  cpp11::integers minute = yearday::get_minute(fields);
  cpp11::integers second = yearday::get_second(fields);
  cpp11::integers subsecond = yearday::get_subsecond(fields);

  yearday::y y{year};
  yearday::yyd yyd{year, day};
  yearday::yydh yydh{year, day, hour};
  yearday::yydhm yydhm{year, day, hour, minute};
  yearday::yydhms yydhms{year, day, hour, minute, second};
  yearday::yydhmss<std::chrono::milliseconds> yydhmss1{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::microseconds> yydhmss2{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::nanoseconds> yydhmss3{year, day, hour, minute, second, subsecond};

  cpp11::integers ticks = duration::get_ticks(fields_n);

  duration::years dy{ticks};

  switch (precision_fields_val) {
  case precision::year:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(y, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::day:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yyd, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::hour:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yydh, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::minute:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yydhm, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::second:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yydhms, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::millisecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yydhmss1, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::microsecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yydhmss2, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::nanosecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(yydhmss3, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_day_plus_duration_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_day_cpp(cpp11::list_of<cpp11::integers> fields,
                         const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = yearday::get_year(fields);
  cpp11::integers day = yearday::get_day(fields);
  cpp11::integers hour = yearday::get_hour(fields);
  cpp11::integers minute = yearday::get_minute(fields);
  cpp11::integers second = yearday::get_second(fields);
  cpp11::integers subsecond = yearday::get_subsecond(fields);

  yearday::yyd yyd{year, day};
  yearday::yydh yydh{year, day, hour};
  yearday::yydhm yydhm{year, day, hour, minute};
  yearday::yydhms yydhms{year, day, hour, minute, second};
  yearday::yydhmss<std::chrono::milliseconds> yydhmss1{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::microseconds> yydhmss2{year, day, hour, minute, second, subsecond};
  yearday::yydhmss<std::chrono::nanoseconds> yydhmss3{year, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::day: return as_sys_time_from_calendar_impl<duration::days>(yyd);
  case precision::hour: return as_sys_time_from_calendar_impl<duration::hours>(yydh);
  case precision::minute: return as_sys_time_from_calendar_impl<duration::minutes>(yydhm);
  case precision::second: return as_sys_time_from_calendar_impl<duration::seconds>(yydhms);
  case precision::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(yydhmss1);
  case precision::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(yydhmss2);
  case precision::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(yydhmss3);
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

  never_reached("as_sys_time_year_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_year_day_from_sys_time_cpp(cpp11::list_of<cpp11::integers> fields,
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
  case precision::day: return as_calendar_from_sys_time_impl<yearday::yyd>(dd);
  case precision::hour: return as_calendar_from_sys_time_impl<yearday::yydh>(dh);
  case precision::minute: return as_calendar_from_sys_time_impl<yearday::yydhm>(dmin);
  case precision::second: return as_calendar_from_sys_time_impl<yearday::yydhms>(ds);
  case precision::millisecond: return as_calendar_from_sys_time_impl<yearday::yydhmss<std::chrono::milliseconds>>(dmilli);
  case precision::microsecond: return as_calendar_from_sys_time_impl<yearday::yydhmss<std::chrono::microseconds>>(dmicro);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<yearday::yydhmss<std::chrono::nanoseconds>>(dnano);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("as_year_day_from_sys_time_cpp");
}

// -----------------------------------------------------------------------------

static
inline
cpp11::writable::list
year_minus_year_impl(const rclock::yearday::y& x,
                     const rclock::yearday::y& y) {
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
year_day_minus_year_day_cpp(cpp11::list_of<cpp11::integers> x,
                            cpp11::list_of<cpp11::integers> y,
                            const cpp11::integers& precision_int) {
  const cpp11::integers x_year = rclock::yearday::get_year(x);

  const cpp11::integers y_year = rclock::yearday::get_year(y);

  const rclock::yearday::y x_y{x_year};

  const rclock::yearday::y y_y{y_year};

  switch (parse_precision(precision_int)) {
  case precision::year: return year_minus_year_impl(x_y, y_y);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_day_minus_year_day_cpp");
}
