#include "quarterly-year-quarter-day.h"
#include "quarterly-shim.h"
#include "calendar.h"
#include "duration.h"
#include "enums.h"
#include "get.h"
#include "rcrd.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
new_year_quarter_day_from_fields(SEXP fields,
                                 const cpp11::integers& precision_int,
                                 SEXP start,
                                 SEXP names) {
  const enum precision precision_val = parse_precision(precision_int);

  const r_ssize n_fields = Rf_xlength(fields);

  r_ssize n;
  switch (precision_val) {
  case precision::year: n = 1; break;
  case precision::quarter: n = 2; break;
  case precision::day: n = 3; break;
  case precision::hour: n = 4; break;
  case precision::minute: n = 5; break;
  case precision::second: n = 6; break;
  case precision::millisecond: n = 7; break;
  case precision::microsecond: n = 7; break;
  case precision::nanosecond: n = 7; break;
  default: never_reached("new_year_quarter_day_from_fields");
  }

  if (n != n_fields) {
    clock_abort("With the given precision, `fields` must have length %i, not %i.", n, n_fields);
  }

  SEXP out = PROTECT(new_clock_rcrd_from_fields(fields, names, classes_year_quarter_day));

  Rf_setAttrib(out, syms_precision, precision_int);
  Rf_setAttrib(out, syms_start, start);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
year_quarter_day_restore(SEXP x, SEXP to) {
  SEXP precision = Rf_getAttrib(to, syms_precision);
  SEXP start = Rf_getAttrib(to, syms_start);

  SEXP out = PROTECT(clock_rcrd_restore(x, to, classes_year_quarter_day));

  Rf_setAttrib(out, syms_precision, precision);
  Rf_setAttrib(out, syms_start, start);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
format_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> fields,
                            const cpp11::integers& precision_int,
                            const cpp11::integers& start_int) {
  using namespace rclock;

  const quarterly::start start = parse_start(start_int);

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y y{year, start};
  rquarterly::yqn yqn{year, quarter, start};
  rquarterly::yqnqd yqnqd{year, quarter, day, start};
  rquarterly::yqnqdh yqnqdh{year, quarter, day, hour, start};
  rquarterly::yqnqdhm yqnqdhm{year, quarter, day, hour, minute, start};
  rquarterly::yqnqdhms yqnqdhms{year, quarter, day, hour, minute, second, start};
  rquarterly::yqnqdhmss<std::chrono::milliseconds> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond, start};
  rquarterly::yqnqdhmss<std::chrono::microseconds> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond, start};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond, start};

  switch (parse_precision(precision_int)) {
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

  never_reached("format_year_quarter_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_quarter_day_cpp(const cpp11::integers& year,
                                    const cpp11::integers& quarter,
                                    const cpp11::integers& day,
                                    const cpp11::integers& start_int) {
  const quarterly::start start = parse_start(start_int);

  rclock::rquarterly::yqnqd x{year, quarter, day, start};

  const r_ssize size = x.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = false;
    } else {
      out[i] = !x.to_year_quarternum_quarterday(i).ok();
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_year_quarter_day_cpp(const cpp11::integers& year,
                                 const cpp11::integers& quarter,
                                 const cpp11::integers& day,
                                 const cpp11::integers& start_int) {
  const quarterly::start start = parse_start(start_int);

  rclock::rquarterly::yqnqd x{year, quarter, day, start};

  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (!x.is_na(i) && !x.to_year_quarternum_quarterday(i).ok()) {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_year_quarter_day_cpp(const cpp11::integers& year,
                                   const cpp11::integers& quarter,
                                   const cpp11::integers& day,
                                   const cpp11::integers& start_int) {
  const quarterly::start start = parse_start(start_int);

  rclock::rquarterly::yqnqd x{year, quarter, day, start};

  const r_ssize size = x.size();

  int count = 0;

  for (r_ssize i = 0; i < size; ++i) {
    count += !x.is_na(i) && !x.to_year_quarternum_quarterday(i).ok();
  }

  return count;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                     const cpp11::integers& precision_int,
                                     const cpp11::integers& start_int,
                                     const cpp11::strings& invalid_string) {
  using namespace rclock;

  const quarterly::start start = parse_start(start_int);
  const enum invalid invalid_val = parse_invalid(invalid_string);

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::yqnqd yqnqd{year, quarter, day, start};
  rquarterly::yqnqdh yqnqdh{year, quarter, day, hour, start};
  rquarterly::yqnqdhm yqnqdhm{year, quarter, day, hour, minute, start};
  rquarterly::yqnqdhms yqnqdhms{year, quarter, day, hour, minute, second, start};
  rquarterly::yqnqdhmss<std::chrono::milliseconds> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond, start};
  rquarterly::yqnqdhmss<std::chrono::microseconds> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond, start};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond, start};

  switch (parse_precision(precision_int)) {
  case precision::day: return invalid_resolve_calendar_impl(yqnqd, invalid_val);
  case precision::hour: return invalid_resolve_calendar_impl(yqnqdh, invalid_val);
  case precision::minute: return invalid_resolve_calendar_impl(yqnqdhm, invalid_val);
  case precision::second: return invalid_resolve_calendar_impl(yqnqdhms, invalid_val);
  case precision::millisecond: return invalid_resolve_calendar_impl(yqnqdhmss1, invalid_val);
  case precision::microsecond: return invalid_resolve_calendar_impl(yqnqdhmss2, invalid_val);
  case precision::nanosecond: return invalid_resolve_calendar_impl(yqnqdhmss3, invalid_val);
  default: never_reached("invalid_resolve_year_quarter_day_cpp");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::integers
get_year_quarter_day_last_cpp(const cpp11::integers& year,
                              const cpp11::integers& quarter,
                              const cpp11::integers& start_int) {
  const quarterly::start start = parse_start(start_int);

  rclock::rquarterly::yqn x{year, quarter, start};

  const r_ssize size = x.size();
  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_int_na;
    } else {
      rclock::rquarterly::quarterly_shim::year_quarternum_quarterday_last elt{x.to_year_quarternum(i)};
      out[i] = static_cast<int>(static_cast<unsigned>(elt.quarterday()));
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
year_quarter_day_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                   cpp11::list_of<cpp11::doubles> fields_n,
                                   const cpp11::integers& precision_fields,
                                   const cpp11::integers& precision_n,
                                   const cpp11::integers& start_int) {
  using namespace rclock;

  const quarterly::start start = parse_start(start_int);

  const enum precision precision_fields_val = parse_precision(precision_fields);
  const enum precision precision_n_val = parse_precision(precision_n);

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y y{year, start};
  rquarterly::yqn yqn{year, quarter, start};
  rquarterly::yqnqd yqnqd{year, quarter, day, start};
  rquarterly::yqnqdh yqnqdh{year, quarter, day, hour, start};
  rquarterly::yqnqdhm yqnqdhm{year, quarter, day, hour, minute, start};
  rquarterly::yqnqdhms yqnqdhms{year, quarter, day, hour, minute, second, start};
  rquarterly::yqnqdhmss<std::chrono::milliseconds> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond, start};
  rquarterly::yqnqdhmss<std::chrono::microseconds> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond, start};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond, start};

  duration::years dy{fields_n};
  duration::quarters dq{fields_n};

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

  never_reached("year_quarter_day_plus_duration_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::integers& precision_int,
                                 const cpp11::integers& start_int) {
  using namespace rclock;

  const quarterly::start start = parse_start(start_int);

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::yqnqd yqnqd{year, quarter, day, start};
  rquarterly::yqnqdh yqnqdh{year, quarter, day, hour, start};
  rquarterly::yqnqdhm yqnqdhm{year, quarter, day, hour, minute, start};
  rquarterly::yqnqdhms yqnqdhms{year, quarter, day, hour, minute, second, start};
  rquarterly::yqnqdhmss<std::chrono::milliseconds> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond, start};
  rquarterly::yqnqdhmss<std::chrono::microseconds> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond, start};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond, start};

  switch (parse_precision(precision_int)) {
  case precision::day: return as_sys_time_from_calendar_impl<duration::days>(yqnqd);
  case precision::hour: return as_sys_time_from_calendar_impl<duration::hours>(yqnqdh);
  case precision::minute: return as_sys_time_from_calendar_impl<duration::minutes>(yqnqdhm);
  case precision::second: return as_sys_time_from_calendar_impl<duration::seconds>(yqnqdhms);
  case precision::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(yqnqdhmss1);
  case precision::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(yqnqdhmss2);
  case precision::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(yqnqdhmss3);
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

  never_reached("as_sys_time_year_quarter_day_cpp");
}

// -----------------------------------------------------------------------------

template <class ClockDuration, class Calendar>
cpp11::writable::list
as_year_quarter_day_from_sys_time_impl(cpp11::list_of<cpp11::doubles>& fields,
                                       quarterly::start start) {
  using Duration = typename ClockDuration::chrono_duration;

  const ClockDuration x{fields};
  const r_ssize size = x.size();

  Calendar out(size, start);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
    } else {
      const Duration elt = x[i];
      const date::sys_time<Duration> elt_st{elt};
      out.assign_sys_time(elt_st, i);
    }
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
as_year_quarter_day_from_sys_time_cpp(cpp11::list_of<cpp11::doubles> fields,
                                      const cpp11::integers& precision_int,
                                      const cpp11::integers& start_int) {
  using namespace rclock;

  const quarterly::start start = parse_start(start_int);

  switch (parse_precision(precision_int)) {
  case precision::day: return as_year_quarter_day_from_sys_time_impl<duration::days, rquarterly::yqnqd>(fields, start);
  case precision::hour: return as_year_quarter_day_from_sys_time_impl<duration::hours, rquarterly::yqnqdh>(fields, start);
  case precision::minute: return as_year_quarter_day_from_sys_time_impl<duration::minutes, rquarterly::yqnqdhm>(fields, start);
  case precision::second: return as_year_quarter_day_from_sys_time_impl<duration::seconds, rquarterly::yqnqdhms>(fields, start);
  case precision::millisecond: return as_year_quarter_day_from_sys_time_impl<duration::milliseconds, rquarterly::yqnqdhmss<std::chrono::milliseconds>>(fields, start);
  case precision::microsecond: return as_year_quarter_day_from_sys_time_impl<duration::microseconds, rquarterly::yqnqdhmss<std::chrono::microseconds>>(fields, start);
  case precision::nanosecond: return as_year_quarter_day_from_sys_time_impl<duration::nanoseconds, rquarterly::yqnqdhmss<std::chrono::nanoseconds>>(fields, start);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("as_year_quarter_day_from_sys_time_cpp");
}

// -----------------------------------------------------------------------------

static
inline
cpp11::writable::list
year_minus_year_impl(const rclock::rquarterly::y& x,
                     const rclock::rquarterly::y& y) {
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
year_quarter_minus_year_quarter_impl(const rclock::rquarterly::yqn& x,
                                     const rclock::rquarterly::yqn& y) {
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

[[cpp11::register]]
cpp11::writable::list
year_quarter_day_minus_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> x,
                                            cpp11::list_of<cpp11::integers> y,
                                            const cpp11::integers& precision_int,
                                            const cpp11::integers& start_int) {
  quarterly::start start = parse_start(start_int);

  const cpp11::integers x_year = rclock::rquarterly::get_year(x);
  const cpp11::integers x_quarter = rclock::rquarterly::get_quarter(x);

  const cpp11::integers y_year = rclock::rquarterly::get_year(y);
  const cpp11::integers y_quarter = rclock::rquarterly::get_quarter(y);

  const rclock::rquarterly::y x_y{x_year, start};
  const rclock::rquarterly::yqn x_yqn{x_year, x_quarter, start};

  const rclock::rquarterly::y y_y{y_year, start};
  const rclock::rquarterly::yqn y_yqn{y_year, y_quarter, start};

  switch (parse_precision(precision_int)) {
  case precision::year: return year_minus_year_impl(x_y, y_y);
  case precision::quarter: return year_quarter_minus_year_quarter_impl(x_yqn, y_yqn);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_quarter_day_minus_year_quarter_day_cpp");
}
