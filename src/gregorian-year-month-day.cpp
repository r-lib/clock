#include "gregorian-year-month-day.h"
#include "calendar.h"
#include "duration.h"
#include "enums.h"
#include "get.h"
#include "parse.h"
#include "failure.h"
#include "fill.h"
#include "rcrd.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
new_year_month_day_from_fields(SEXP fields,
                               const cpp11::integers& precision_int,
                               SEXP names) {
  const enum precision precision_val = parse_precision(precision_int);

  const r_ssize n_fields = Rf_xlength(fields);

  r_ssize n;
  switch (precision_val) {
  case precision::year: n = 1; break;
  case precision::month: n = 2; break;
  case precision::day: n = 3; break;
  case precision::hour: n = 4; break;
  case precision::minute: n = 5; break;
  case precision::second: n = 6; break;
  case precision::millisecond: n = 7; break;
  case precision::microsecond: n = 7; break;
  case precision::nanosecond: n = 7; break;
  default: never_reached("new_year_month_day_from_fields");
  }

  if (n != n_fields) {
    clock_abort("With the given precision, `fields` must have length %i, not %i.", n, n_fields);
  }

  SEXP out = PROTECT(new_clock_rcrd_from_fields(fields, names, classes_year_month_day));

  Rf_setAttrib(out, syms_precision, precision_int);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
year_month_day_restore(SEXP x, SEXP to) {
  SEXP precision = Rf_getAttrib(to, syms_precision);

  SEXP out = PROTECT(clock_rcrd_restore(x, to, classes_year_month_day));

  Rf_setAttrib(out, syms_precision, precision);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
format_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                          const cpp11::integers& precision_int) {
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

  switch (parse_precision(precision_int)) {
  case precision::year: return format_calendar_impl(y);
  case precision::month: return format_calendar_impl(ym);
  case precision::day: return format_calendar_impl(ymd);
  case precision::hour: return format_calendar_impl(ymdh);
  case precision::minute: return format_calendar_impl(ymdhm);
  case precision::second: return format_calendar_impl(ymdhms);
  case precision::millisecond: return format_calendar_impl(ymdhmss1);
  case precision::microsecond: return format_calendar_impl(ymdhmss2);
  case precision::nanosecond: return format_calendar_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("format_year_month_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_month_day_cpp(const cpp11::integers& year,
                                  const cpp11::integers& month,
                                  const cpp11::integers& day) {
  rclock::gregorian::ymd x{year, month, day};

  const r_ssize size = x.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = false;
    } else {
      out[i] = !x.to_year_month_day(i).ok();
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_year_month_day_cpp(const cpp11::integers& year,
                               const cpp11::integers& month,
                               const cpp11::integers& day) {
  rclock::gregorian::ymd x{year, month, day};

  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (!x.is_na(i) && !x.to_year_month_day(i).ok()) {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_year_month_day_cpp(const cpp11::integers& year,
                                 const cpp11::integers& month,
                                 const cpp11::integers& day) {
  rclock::gregorian::ymd x{year, month, day};

  const r_ssize size = x.size();

  int count = 0;

  for (r_ssize i = 0; i < size; ++i) {
    count += !x.is_na(i) && !x.to_year_month_day(i).ok();
  }

  return count;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                   const cpp11::integers& precision_int,
                                   const cpp11::strings& invalid_string,
                                   const cpp11::sexp& call) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid_string);

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

  switch (parse_precision(precision_int)) {
  case precision::day: return invalid_resolve_calendar_impl(ymd, invalid_val, call);
  case precision::hour: return invalid_resolve_calendar_impl(ymdh, invalid_val, call);
  case precision::minute: return invalid_resolve_calendar_impl(ymdhm, invalid_val, call);
  case precision::second: return invalid_resolve_calendar_impl(ymdhms, invalid_val, call);
  case precision::millisecond: return invalid_resolve_calendar_impl(ymdhmss1, invalid_val, call);
  case precision::microsecond: return invalid_resolve_calendar_impl(ymdhmss2, invalid_val, call);
  case precision::nanosecond: return invalid_resolve_calendar_impl(ymdhmss3, invalid_val, call);
  default: never_reached("invalid_resolve_year_month_day_cpp");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::integers
get_year_month_day_last_cpp(const cpp11::integers& year,
                            const cpp11::integers& month) {
  rclock::gregorian::ym x{year, month};

  const r_ssize size = x.size();
  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_int_na;
    } else {
      date::year_month_day_last elt = x.to_year_month(i) / date::last;
      out[i] = static_cast<int>(static_cast<unsigned>(elt.day()));
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
year_month_day_plus_years_cpp(const cpp11::integers& year,
                              cpp11::list_of<cpp11::doubles> fields_n) {
  rclock::gregorian::y x{year};
  rclock::duration::years n{fields_n};
  return calendar_plus_duration_impl(x, n);
}

[[cpp11::register]]
cpp11::writable::list
year_month_day_plus_months_cpp(const cpp11::integers& year,
                               const cpp11::integers& month,
                               cpp11::list_of<cpp11::doubles> fields_n) {
  rclock::gregorian::ym x{year, month};
  rclock::duration::months n{fields_n};
  return calendar_plus_duration_impl(x, n);
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::integers& precision_int) {
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

  switch (parse_precision(precision_int)) {
  case precision::day: return as_sys_time_from_calendar_impl<duration::days>(ymd);
  case precision::hour: return as_sys_time_from_calendar_impl<duration::hours>(ymdh);
  case precision::minute: return as_sys_time_from_calendar_impl<duration::minutes>(ymdhm);
  case precision::second: return as_sys_time_from_calendar_impl<duration::seconds>(ymdhms);
  case precision::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(ymdhmss1);
  case precision::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(ymdhmss2);
  case precision::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(ymdhmss3);
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

  never_reached("as_sys_time_year_month_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_year_month_day_from_sys_time_cpp(cpp11::list_of<cpp11::doubles> fields,
                                    const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::day: return as_calendar_from_sys_time_impl<duration::days, gregorian::ymd>(fields);
  case precision::hour: return as_calendar_from_sys_time_impl<duration::hours, gregorian::ymdh>(fields);
  case precision::minute: return as_calendar_from_sys_time_impl<duration::minutes, gregorian::ymdhm>(fields);
  case precision::second: return as_calendar_from_sys_time_impl<duration::seconds, gregorian::ymdhms>(fields);
  case precision::millisecond: return as_calendar_from_sys_time_impl<duration::milliseconds, gregorian::ymdhmss<std::chrono::milliseconds>>(fields);
  case precision::microsecond: return as_calendar_from_sys_time_impl<duration::microseconds, gregorian::ymdhmss<std::chrono::microseconds>>(fields);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<duration::nanoseconds, gregorian::ymdhmss<std::chrono::nanoseconds>>(fields);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("as_year_month_day_from_sys_time_cpp");
}

// -----------------------------------------------------------------------------

static
inline
cpp11::writable::list
year_minus_year_impl(const rclock::gregorian::y& x,
                     const rclock::gregorian::y& y) {
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
year_month_minus_year_month_impl(const rclock::gregorian::ym& x,
                                 const rclock::gregorian::ym& y) {
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
year_month_day_minus_year_month_day_cpp(cpp11::list_of<cpp11::integers> x,
                                        cpp11::list_of<cpp11::integers> y,
                                        const cpp11::integers& precision_int) {
  const cpp11::integers x_year = rclock::gregorian::get_year(x);
  const cpp11::integers x_month = rclock::gregorian::get_month(x);

  const cpp11::integers y_year = rclock::gregorian::get_year(y);
  const cpp11::integers y_month = rclock::gregorian::get_month(y);

  const rclock::gregorian::y x_y{x_year};
  const rclock::gregorian::ym x_ym{x_year, x_month};

  const rclock::gregorian::y y_y{y_year};
  const rclock::gregorian::ym y_ym{y_year, y_month};

  switch (parse_precision(precision_int)) {
  case precision::year: return year_minus_year_impl(x_y, y_y);
  case precision::month: return year_month_minus_year_month_impl(x_ym, y_ym);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_month_day_minus_year_month_day_cpp");
}

// -----------------------------------------------------------------------------

// Default impl applies to millisecond/microsecond/nanosecond parsers
template <class Calendar>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const std::vector<std::string>& fmts,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const char& decimal_mark,
                           const r_ssize& i,
                           rclock::failures& fail,
                           Calendar& out) {
  using Duration = typename Calendar::duration;
  const r_ssize size = fmts.size();

  for (r_ssize j = 0; j < size; ++j) {
    stream.clear();
    stream.seekg(0);

    const char* fmt = fmts[j].c_str();
    date::year_month_day ymd{};
    date::hh_mm_ss<Duration> hms{};

    rclock::from_stream(
      stream,
      fmt,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      decimal_mark,
      ymd,
      hms
    );

    if (!stream.fail()) {
      out.assign_year_month_day(ymd, i);
      out.assign_hour(hms.hours(), i);
      out.assign_minute(hms.minutes(), i);
      out.assign_second(hms.seconds(), i);
      out.assign_subsecond(hms.subseconds(), i);
      return;
    }
  }

  fail.write(i);
  out.assign_na(i);
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const std::vector<std::string>& fmts,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const char& decimal_mark,
                           const r_ssize& i,
                           rclock::failures& fail,
                           rclock::gregorian::y& out) {
  const r_ssize size = fmts.size();

  for (r_ssize j = 0; j < size; ++j) {
    stream.clear();
    stream.seekg(0);

    const char* fmt = fmts[j].c_str();
    date::year x{};

    rclock::from_stream(
      stream,
      fmt,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      decimal_mark,
      x
    );

    if (!stream.fail()) {
      out.assign_year(x, i);
      return;
    }
  }

  fail.write(i);
  out.assign_na(i);
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const std::vector<std::string>& fmts,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const char& decimal_mark,
                           const r_ssize& i,
                           rclock::failures& fail,
                           rclock::gregorian::ym& out) {
  const r_ssize size = fmts.size();

  for (r_ssize j = 0; j < size; ++j) {
    stream.clear();
    stream.seekg(0);

    const char* fmt = fmts[j].c_str();
    date::year_month x{};

    rclock::from_stream(
      stream,
      fmt,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      decimal_mark,
      x
    );

    if (!stream.fail()) {
      out.assign_year_month(x, i);
      return;
    }
  }

  fail.write(i);
  out.assign_na(i);
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const std::vector<std::string>& fmts,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const char& decimal_mark,
                           const r_ssize& i,
                           rclock::failures& fail,
                           rclock::gregorian::ymd& out) {
  const r_ssize size = fmts.size();

  for (r_ssize j = 0; j < size; ++j) {
    stream.clear();
    stream.seekg(0);

    const char* fmt = fmts[j].c_str();
    date::year_month_day x{};

    rclock::from_stream(
      stream,
      fmt,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      decimal_mark,
      x
    );

    if (!stream.fail()) {
      out.assign_year_month_day(x, i);
      return;
    }
  }

  fail.write(i);
  out.assign_na(i);
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const std::vector<std::string>& fmts,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const char& decimal_mark,
                           const r_ssize& i,
                           rclock::failures& fail,
                           rclock::gregorian::ymdh& out) {
  const r_ssize size = fmts.size();

  for (r_ssize j = 0; j < size; ++j) {
    stream.clear();
    stream.seekg(0);

    const char* fmt = fmts[j].c_str();
    date::year_month_day ymd{};
    date::hh_mm_ss<std::chrono::seconds> hms{};

    rclock::from_stream(
      stream,
      fmt,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      decimal_mark,
      ymd,
      hms
    );

    if (!stream.fail()) {
      out.assign_year_month_day(ymd, i);
      out.assign_hour(hms.hours(), i);
      return;
    }
  }

  fail.write(i);
  out.assign_na(i);
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const std::vector<std::string>& fmts,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const char& decimal_mark,
                           const r_ssize& i,
                           rclock::failures& fail,
                           rclock::gregorian::ymdhm& out) {
  const r_ssize size = fmts.size();

  for (r_ssize j = 0; j < size; ++j) {
    stream.clear();
    stream.seekg(0);

    const char* fmt = fmts[j].c_str();
    date::year_month_day ymd{};
    date::hh_mm_ss<std::chrono::seconds> hms{};

    rclock::from_stream(
      stream,
      fmt,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      decimal_mark,
      ymd,
      hms
    );

    if (!stream.fail()) {
      out.assign_year_month_day(ymd, i);
      out.assign_hour(hms.hours(), i);
      out.assign_minute(hms.minutes(), i);
      return;
    }
  }

  fail.write(i);
  out.assign_na(i);
}

template <>
inline
void
year_month_day_from_stream(std::istringstream& stream,
                           const std::vector<std::string>& fmts,
                           const std::pair<const std::string*, const std::string*>& month_names_pair,
                           const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                           const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                           const char& decimal_mark,
                           const r_ssize& i,
                           rclock::failures& fail,
                           rclock::gregorian::ymdhms& out) {
  const r_ssize size = fmts.size();

  for (r_ssize j = 0; j < size; ++j) {
    stream.clear();
    stream.seekg(0);

    const char* fmt = fmts[j].c_str();
    date::year_month_day ymd{};
    date::hh_mm_ss<std::chrono::seconds> hms{};

    rclock::from_stream(
      stream,
      fmt,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      decimal_mark,
      ymd,
      hms
    );

    if (!stream.fail()) {
      out.assign_year_month_day(ymd, i);
      out.assign_hour(hms.hours(), i);
      out.assign_minute(hms.minutes(), i);
      out.assign_second(hms.seconds(), i);
      return;
    }
  }

  fail.write(i);
  out.assign_na(i);
}

template <class Calendar>
static
cpp11::writable::list
year_month_day_parse_impl(const cpp11::strings& x,
                          const cpp11::strings& format,
                          const cpp11::strings& month,
                          const cpp11::strings& month_abbrev,
                          const cpp11::strings& weekday,
                          const cpp11::strings& weekday_abbrev,
                          const cpp11::strings& am_pm,
                          const cpp11::strings& mark) {
  const r_ssize size = x.size();
  Calendar out(size);

  std::vector<std::string> fmts(format.size());
  rclock::fill_formats(format, fmts);

  char dmark;
  switch (parse_decimal_mark(mark)) {
  case decimal_mark::comma: dmark = ','; break;
  case decimal_mark::period: dmark = '.'; break;
  default: clock_abort("Internal error: Unknown decimal mark.");
  }

  std::string month_names[24];
  const std::pair<const std::string*, const std::string*>& month_names_pair = fill_month_names(
    month,
    month_abbrev,
    month_names
  );

  std::string weekday_names[14];
  const std::pair<const std::string*, const std::string*>& weekday_names_pair = fill_weekday_names(
    weekday,
    weekday_abbrev,
    weekday_names
  );

  std::string ampm_names[2];
  const std::pair<const std::string*, const std::string*>& ampm_names_pair = fill_ampm_names(
    am_pm,
    ampm_names
  );

  rclock::failures fail{};

  std::istringstream stream;

  void* vmax = vmaxget();

  for (r_ssize i = 0; i < size; ++i) {
    const SEXP elt = x[i];

    if (elt == r_chr_na) {
      out.assign_na(i);
      continue;
    }

    const char* p_elt = Rf_translateCharUTF8(elt);

    stream.str(p_elt);

    year_month_day_from_stream(
      stream,
      fmts,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      dmark,
      i,
      fail,
      out
    );
  }

  vmaxset(vmax);

  if (fail.any_failures()) {
    fail.warn_parse();
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
year_month_day_parse_cpp(const cpp11::strings& x,
                         const cpp11::strings& format,
                         const cpp11::integers& precision_int,
                         const cpp11::strings& month,
                         const cpp11::strings& month_abbrev,
                         const cpp11::strings& weekday,
                         const cpp11::strings& weekday_abbrev,
                         const cpp11::strings& am_pm,
                         const cpp11::strings& mark) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return year_month_day_parse_impl<gregorian::y>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::month: return year_month_day_parse_impl<gregorian::ym>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::day: return year_month_day_parse_impl<gregorian::ymd>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::hour: return year_month_day_parse_impl<gregorian::ymdh>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::minute: return year_month_day_parse_impl<gregorian::ymdhm>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::second: return year_month_day_parse_impl<gregorian::ymdhms>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::millisecond: return year_month_day_parse_impl<gregorian::ymdhmss<std::chrono::milliseconds>>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::microsecond: return year_month_day_parse_impl<gregorian::ymdhmss<std::chrono::microseconds>>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  case precision::nanosecond: return year_month_day_parse_impl<gregorian::ymdhmss<std::chrono::nanoseconds>>(x, format, month, month_abbrev, weekday, weekday_abbrev, am_pm, mark);
  default: never_reached("year_month_day_parse_cpp");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
gregorian_leap_year_cpp(const cpp11::integers& year) {
  const r_ssize size = year.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    const int elt = year[i];

    if (elt == r_int_na) {
      out[i] = r_lgl_na;
    } else {
      out[i] = date::year{elt}.is_leap();
    }
  }

  return out;
}
