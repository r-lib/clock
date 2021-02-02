#include "clock.h"
#include "enums.h"
#include "utils.h"
#include "rcrd.h"
#include "duration.h"
#include "parse.h"
#include "locale.h"
#include <sstream>

[[cpp11::register]]
SEXP
new_time_point_from_fields(SEXP fields,
                           const cpp11::integers& precision_int,
                           const cpp11::integers& clock_int,
                           SEXP names) {
  const enum precision precision_val = parse_precision(precision_int);
  const enum clock_name clock_val = parse_clock_name(clock_int);

  const r_ssize n_fields = Rf_xlength(fields);

  switch (precision_val) {
  case precision::year:
  case precision::quarter:
  case precision::month:
  case precision::week: {
    clock_abort("`precision` must be at least 'day' precision.");
  }
  case precision::day: {
    if (n_fields != 1) {
      clock_abort("`fields` must have 1 field for day precision.");
    }
    break;
  }
  case precision::hour:
  case precision::minute:
  case precision::second: {
    if (n_fields != 2) {
      clock_abort("`fields` must have 2 fields for [hour, second] precision.");
    }
    break;
  }
  case precision::millisecond:
  case precision::microsecond:
  case precision::nanosecond: {
    if (n_fields != 3) {
      clock_abort("`fields` must have 3 fields for [millisecond, nanosecond] precision.");
    }
    break;
  }
  default: {
    never_reached("new_time_point_from_fields");
  }
  }

  SEXP classes;

  switch (clock_val) {
  case clock_name::naive: classes = classes_naive_time; break;
  case clock_name::sys: classes = classes_sys_time; break;
  default: clock_abort("Internal error: Unknown clock.");
  }

  SEXP out = PROTECT(new_clock_rcrd_from_fields(fields, names, classes));

  Rf_setAttrib(out, syms_precision, precision_int);
  Rf_setAttrib(out, syms_clock, clock_int);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
time_point_restore(SEXP x, SEXP to) {
  SEXP clock_int = Rf_getAttrib(to, syms_clock);
  SEXP precision_int = Rf_getAttrib(to, syms_precision);

  SEXP classes;

  switch (parse_clock_name(clock_int)) {
  case clock_name::naive: classes = classes_naive_time; break;
  case clock_name::sys: classes = classes_sys_time; break;
  default: clock_abort("Internal error: Unknown clock.");
  }

  SEXP out = PROTECT(clock_rcrd_restore(x, to, classes));

  Rf_setAttrib(out, syms_clock, clock_int);
  Rf_setAttrib(out, syms_precision, precision_int);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

template <class ClockDuration, class Clock>
static
cpp11::writable::list
parse_time_point_impl(const cpp11::strings& x,
                      const cpp11::strings& format,
                      const cpp11::strings& month,
                      const cpp11::strings& mon_ab,
                      const cpp11::strings& day,
                      const cpp11::strings& day_ab,
                      const cpp11::strings& am_pm,
                      const cpp11::strings& mark) {
  const r_ssize size = x.size();
  ClockDuration out(size);
  using Duration = typename ClockDuration::duration;

  if (!r_is_string(format)) {
    clock_abort("`format` must be a single string.");
  }
  const SEXP format_sexp = format[0];
  const char* format_char = CHAR(format_sexp);

  char dmark;
  switch (parse_decimal_mark(mark)) {
  case decimal_mark::comma: dmark = ','; break;
  case decimal_mark::period: dmark = '.'; break;
  default: clock_abort("Internal error: Unknown decimal mark.");
  }

  std::string month_names[24];
  const std::pair<const std::string*, const std::string*>& month_names_pair = fill_month_names(
    month,
    mon_ab,
    month_names
  );

  std::string weekday_names[14];
  const std::pair<const std::string*, const std::string*>& weekday_names_pair = fill_weekday_names(
    day,
    day_ab,
    weekday_names
  );

  std::string ampm_names[2];
  const std::pair<const std::string*, const std::string*>& ampm_names_pair = fill_ampm_names(
    am_pm,
    ampm_names
  );

  std::istringstream stream;

  for (r_ssize i = 0; i < size; ++i) {
    const SEXP elt = x[i];

    if (elt == r_chr_na) {
      out.assign_na(i);
      continue;
    }

    const char* elt_char = CHAR(elt);

    stream.clear();
    stream.str(elt_char);

    std::chrono::time_point<Clock, Duration> tp;

    rclock::from_stream(
      stream,
      format_char,
      month_names_pair,
      weekday_names_pair,
      ampm_names_pair,
      dmark,
      tp
    );

    if (stream.fail()) {
      out.assign_na(i);
      continue;
    }

    out.assign(tp.time_since_epoch(), i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
parse_time_point_cpp(const cpp11::strings& x,
                     const cpp11::strings& format,
                     const cpp11::integers& precision_int,
                     const cpp11::integers& clock_int,
                     const cpp11::strings& month,
                     const cpp11::strings& mon_ab,
                     const cpp11::strings& day,
                     const cpp11::strings& day_ab,
                     const cpp11::strings& am_pm,
                     const cpp11::strings& mark) {
  using namespace rclock;

  switch (parse_clock_name(clock_int)) {
  case clock_name::naive: {
    switch (parse_precision(precision_int)) {
    case precision::day: return parse_time_point_impl<duration::days, date::local_t>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::hour: return parse_time_point_impl<duration::hours, date::local_t>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::minute: return parse_time_point_impl<duration::minutes, date::local_t>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::second: return parse_time_point_impl<duration::seconds, date::local_t>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::millisecond: return parse_time_point_impl<duration::milliseconds, date::local_t>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::microsecond: return parse_time_point_impl<duration::microseconds, date::local_t>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::nanosecond: return parse_time_point_impl<duration::nanoseconds, date::local_t>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    default: never_reached("parse_time_point_cpp");
    }
  }
  case clock_name::sys: {
    switch (parse_precision(precision_int)) {
    case precision::day: return parse_time_point_impl<duration::days, std::chrono::system_clock>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::hour: return parse_time_point_impl<duration::hours, std::chrono::system_clock>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::minute: return parse_time_point_impl<duration::minutes, std::chrono::system_clock>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::second: return parse_time_point_impl<duration::seconds, std::chrono::system_clock>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::millisecond: return parse_time_point_impl<duration::milliseconds, std::chrono::system_clock>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::microsecond: return parse_time_point_impl<duration::microseconds, std::chrono::system_clock>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    case precision::nanosecond: return parse_time_point_impl<duration::nanoseconds, std::chrono::system_clock>(x, format, month, mon_ab, day, day_ab, am_pm, mark);
    default: never_reached("parse_time_point_cpp");
    }
  }
  default: never_reached("parse_time_point_cpp");
  }
}
