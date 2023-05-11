#include "gregorian-year-day.h"
#include "calendar.h"
#include "duration.h"
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
invalid_detect_year_day_cpp(const cpp11::integers& year,
                            const cpp11::integers& day) {
  rclock::yearday::yyd x{year, day};

  const r_ssize size = x.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = false;
    } else {
      out[i] = !x.to_year_yearday(i).ok();
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_year_day_cpp(const cpp11::integers& year,
                         const cpp11::integers& day) {
  rclock::yearday::yyd x{year, day};

  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (!x.is_na(i) && !x.to_year_yearday(i).ok()) {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_year_day_cpp(const cpp11::integers& year,
                           const cpp11::integers& day) {
  rclock::yearday::yyd x{year, day};

  const r_ssize size = x.size();

  int count = 0;

  for (r_ssize i = 0; i < size; ++i) {
    count += !x.is_na(i) && !x.to_year_yearday(i).ok();
  }

  return count;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_day_cpp(cpp11::list_of<cpp11::integers> fields,
                             const cpp11::integers& precision_int,
                             const cpp11::strings& invalid_string,
                             const cpp11::sexp& call) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid_string);

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
  case precision::day: return invalid_resolve_calendar_impl(yyd, invalid_val, call);
  case precision::hour: return invalid_resolve_calendar_impl(yydh, invalid_val, call);
  case precision::minute: return invalid_resolve_calendar_impl(yydhm, invalid_val, call);
  case precision::second: return invalid_resolve_calendar_impl(yydhms, invalid_val, call);
  case precision::millisecond: return invalid_resolve_calendar_impl(yydhmss1, invalid_val, call);
  case precision::microsecond: return invalid_resolve_calendar_impl(yydhmss2, invalid_val, call);
  case precision::nanosecond: return invalid_resolve_calendar_impl(yydhmss3, invalid_val, call);
  default: never_reached("invalid_resolve_year_day_cpp");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::integers
get_year_day_last_cpp(const cpp11::integers& year) {
  rclock::yearday::y x{year};

  const r_ssize size = x.size();
  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_int_na;
    } else {
      ordinal::year_yearday_last elt = x.to_year(i) / ordinal::last;
      out[i] = static_cast<int>(static_cast<unsigned>(elt.yearday()));
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
year_day_plus_years_cpp(const cpp11::integers& year,
                        cpp11::list_of<cpp11::doubles> fields_n) {
  rclock::yearday::y x{year};
  rclock::duration::years n{fields_n};
  return calendar_plus_duration_impl(x, n);
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
as_year_day_from_sys_time_cpp(cpp11::list_of<cpp11::doubles> fields,
                              const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::day: return as_calendar_from_sys_time_impl<duration::days, yearday::yyd>(fields);
  case precision::hour: return as_calendar_from_sys_time_impl<duration::hours, yearday::yydh>(fields);
  case precision::minute: return as_calendar_from_sys_time_impl<duration::minutes, yearday::yydhm>(fields);
  case precision::second: return as_calendar_from_sys_time_impl<duration::seconds, yearday::yydhms>(fields);
  case precision::millisecond: return as_calendar_from_sys_time_impl<duration::milliseconds, yearday::yydhmss<std::chrono::milliseconds>>(fields);
  case precision::microsecond: return as_calendar_from_sys_time_impl<duration::microseconds, yearday::yydhmss<std::chrono::microseconds>>(fields);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<duration::nanoseconds, yearday::yydhmss<std::chrono::nanoseconds>>(fields);
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
