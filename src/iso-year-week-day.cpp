#include "iso-year-week-day.h"
#include "calendar.h"
#include "duration.h"
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
invalid_detect_iso_year_week_day_cpp(const cpp11::integers& year,
                                     const cpp11::integers& week) {
  rclock::iso::ywn x{year, week};

  const r_ssize size = x.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = false;
    } else {
      out[i] = !x.to_year_weeknum(i).ok();
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_iso_year_week_day_cpp(const cpp11::integers& year,
                                  const cpp11::integers& week) {
  rclock::iso::ywn x{year, week};

  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (!x.is_na(i) && !x.to_year_weeknum(i).ok()) {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_iso_year_week_day_cpp(const cpp11::integers& year,
                                    const cpp11::integers& week) {
  rclock::iso::ywn x{year, week};

  const r_ssize size = x.size();

  int count = 0;

  for (r_ssize i = 0; i < size; ++i) {
    count += !x.is_na(i) && !x.to_year_weeknum(i).ok();
  }

  return count;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_iso_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                      const cpp11::integers& precision_int,
                                      const cpp11::strings& invalid_string,
                                      const cpp11::sexp& call) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid_string);

  cpp11::integers year = iso::get_year(fields);
  cpp11::integers week = iso::get_week(fields);
  cpp11::integers day = iso::get_day(fields);
  cpp11::integers hour = iso::get_hour(fields);
  cpp11::integers minute = iso::get_minute(fields);
  cpp11::integers second = iso::get_second(fields);
  cpp11::integers subsecond = iso::get_subsecond(fields);

  iso::ywn ywn{year, week};
  iso::ywnwd ywnwd{year, week, day};
  iso::ywnwdh ywnwdh{year, week, day, hour};
  iso::ywnwdhm ywnwdhm{year, week, day, hour, minute};
  iso::ywnwdhms ywnwdhms{year, week, day, hour, minute, second};
  iso::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond};
  iso::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::week: return invalid_resolve_calendar_impl(ywn, invalid_val, call);
  case precision::day: return invalid_resolve_calendar_impl(ywnwd, invalid_val, call);
  case precision::hour: return invalid_resolve_calendar_impl(ywnwdh, invalid_val, call);
  case precision::minute: return invalid_resolve_calendar_impl(ywnwdhm, invalid_val, call);
  case precision::second: return invalid_resolve_calendar_impl(ywnwdhms, invalid_val, call);
  case precision::millisecond: return invalid_resolve_calendar_impl(ywnwdhmss1, invalid_val, call);
  case precision::microsecond: return invalid_resolve_calendar_impl(ywnwdhmss2, invalid_val, call);
  case precision::nanosecond: return invalid_resolve_calendar_impl(ywnwdhmss3, invalid_val, call);
  default: never_reached("invalid_resolve_iso_year_week_day_cpp");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::integers
get_iso_year_week_day_last_cpp(const cpp11::integers& year) {
  rclock::iso::y x{year};

  const r_ssize size = x.size();
  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_int_na;
    } else {
      iso_week::year_lastweek elt = x.to_year(i) / iso_week::last;
      out[i] = static_cast<int>(static_cast<unsigned>(elt.weeknum()));
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
iso_year_week_day_plus_years_cpp(const cpp11::integers& year,
                                 cpp11::list_of<cpp11::doubles> fields_n) {
  rclock::iso::y x{year};
  rclock::duration::years n{fields_n};
  return calendar_plus_duration_impl(x, n);
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
as_iso_year_week_day_from_sys_time_cpp(cpp11::list_of<cpp11::doubles> fields,
                                       const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::day: return as_calendar_from_sys_time_impl<duration::days, iso::ywnwd>(fields);
  case precision::hour: return as_calendar_from_sys_time_impl<duration::hours, iso::ywnwdh>(fields);
  case precision::minute: return as_calendar_from_sys_time_impl<duration::minutes, iso::ywnwdhm>(fields);
  case precision::second: return as_calendar_from_sys_time_impl<duration::seconds, iso::ywnwdhms>(fields);
  case precision::millisecond: return as_calendar_from_sys_time_impl<duration::milliseconds, iso::ywnwdhmss<std::chrono::milliseconds>>(fields);
  case precision::microsecond: return as_calendar_from_sys_time_impl<duration::microseconds, iso::ywnwdhmss<std::chrono::microseconds>>(fields);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<duration::nanoseconds, iso::ywnwdhmss<std::chrono::nanoseconds>>(fields);
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

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
iso_year_week_day_leap_year_cpp(const cpp11::integers& year) {
  const r_ssize size = year.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    const int elt = year[i];

    if (elt == r_int_na) {
      out[i] = r_lgl_na;
    } else {
      out[i] = iso_week::year{elt}.is_leap();
    }
  }

  return out;
}
