#include "gregorian-year-month-weekday.h"
#include "calendar.h"
#include "duration.h"
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
invalid_detect_year_month_weekday_cpp(const cpp11::integers& year,
                                      const cpp11::integers& month,
                                      const cpp11::integers& day,
                                      const cpp11::integers& index) {
  rclock::weekday::ymwd x{year, month, day, index};

  const r_ssize size = x.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = false;
    } else {
      out[i] = !x.to_year_month_weekday(i).ok();
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_year_month_weekday_cpp(const cpp11::integers& year,
                                   const cpp11::integers& month,
                                   const cpp11::integers& day,
                                   const cpp11::integers& index) {
  rclock::weekday::ymwd x{year, month, day, index};

  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (!x.is_na(i) && !x.to_year_month_weekday(i).ok()) {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_year_month_weekday_cpp(const cpp11::integers& year,
                                     const cpp11::integers& month,
                                     const cpp11::integers& day,
                                     const cpp11::integers& index) {
  rclock::weekday::ymwd x{year, month, day, index};

  const r_ssize size = x.size();

  int count = 0;

  for (r_ssize i = 0; i < size; ++i) {
    count += !x.is_na(i) && !x.to_year_month_weekday(i).ok();
  }

  return count;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_month_weekday_cpp(cpp11::list_of<cpp11::integers> fields,
                                       const cpp11::integers& precision_int,
                                       const cpp11::strings& invalid_string,
                                       const cpp11::sexp& call) {
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

  weekday::ymwd ymwd{year, month, day, index};
  weekday::ymwdh ymwdh{year, month, day, index, hour};
  weekday::ymwdhm ymwdhm{year, month, day, index, hour, minute};
  weekday::ymwdhms ymwdhms{year, month, day, index, hour, minute, second};
  weekday::ymwdhmss<std::chrono::milliseconds> ymwdhmss1{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::microseconds> ymwdhmss2{year, month, day, index, hour, minute, second, subsecond};
  weekday::ymwdhmss<std::chrono::nanoseconds> ymwdhmss3{year, month, day, index, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::day: return invalid_resolve_calendar_impl(ymwd, invalid_val, call);
  case precision::hour: return invalid_resolve_calendar_impl(ymwdh, invalid_val, call);
  case precision::minute: return invalid_resolve_calendar_impl(ymwdhm, invalid_val, call);
  case precision::second: return invalid_resolve_calendar_impl(ymwdhms, invalid_val, call);
  case precision::millisecond: return invalid_resolve_calendar_impl(ymwdhmss1, invalid_val, call);
  case precision::microsecond: return invalid_resolve_calendar_impl(ymwdhmss2, invalid_val, call);
  case precision::nanosecond: return invalid_resolve_calendar_impl(ymwdhmss3, invalid_val, call);
  default: never_reached("invalid_resolve_year_month_weekday_cpp");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::integers
get_year_month_weekday_last_cpp(const cpp11::integers& year,
                                const cpp11::integers& month,
                                const cpp11::integers& day,
                                const cpp11::integers& index) {
  rclock::weekday::ymwd x{year, month, day, index};

  const r_ssize size = x.size();
  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_int_na;
    } else {
      // We require day precision to set the index to last, so this is allowed
      date::year_month_weekday elt = x.to_year_month_weekday(i);
      date::year_month_weekday elt_last{elt.year() / elt.month() / elt.weekday()[date::last]};
      out[i] = static_cast<int>(static_cast<unsigned>(elt_last.index()));
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
year_month_weekday_plus_years_cpp(const cpp11::integers& year,
                                  cpp11::list_of<cpp11::doubles> fields_n) {
  rclock::weekday::y x{year};
  rclock::duration::years n{fields_n};
  return calendar_plus_duration_impl(x, n);
}

[[cpp11::register]]
cpp11::writable::list
year_month_weekday_plus_months_cpp(const cpp11::integers& year,
                                   const cpp11::integers& month,
                                   cpp11::list_of<cpp11::doubles> fields_n) {
  rclock::weekday::ym x{year, month};
  rclock::duration::months n{fields_n};
  return calendar_plus_duration_impl(x, n);
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
as_year_month_weekday_from_sys_time_cpp(cpp11::list_of<cpp11::doubles> fields,
                                        const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::day: return as_calendar_from_sys_time_impl<duration::days, weekday::ymwd>(fields);
  case precision::hour: return as_calendar_from_sys_time_impl<duration::hours, weekday::ymwdh>(fields);
  case precision::minute: return as_calendar_from_sys_time_impl<duration::minutes, weekday::ymwdhm>(fields);
  case precision::second: return as_calendar_from_sys_time_impl<duration::seconds, weekday::ymwdhms>(fields);
  case precision::millisecond: return as_calendar_from_sys_time_impl<duration::milliseconds, weekday::ymwdhmss<std::chrono::milliseconds>>(fields);
  case precision::microsecond: return as_calendar_from_sys_time_impl<duration::microseconds, weekday::ymwdhmss<std::chrono::microseconds>>(fields);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<duration::nanoseconds, weekday::ymwdhmss<std::chrono::nanoseconds>>(fields);
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
