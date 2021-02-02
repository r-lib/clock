#include "quarterly-year-quarter-day.h"
#include "calendar.h"
#include "duration.h"
#include "check.h"
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

template <quarterly::start S>
static
inline
cpp11::writable::list
collect_year_quarter_day_fields_impl(cpp11::list_of<cpp11::integers> fields,
                                     const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarter};
  rquarterly::yqnqd<S> yqnqd{year, quarter, day};
  rquarterly::yqnqdh<S> yqnqdh{year, quarter, day, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarter, day, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarter, day, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
  case precision::year: {
    collect_field<component::year>(y, year, "year");
    return y.to_list();
  }
  case precision::quarter: {
    collect_field<component::year>(yqn, year, "year");
    collect_field<component::quarter>(yqn, quarter, "quarter");
    return yqn.to_list();
  }
  case precision::day: {
    collect_field<component::year>(yqnqd, year, "year");
    collect_field<component::quarter>(yqnqd, quarter, "quarter");
    collect_field<component::day>(yqnqd, day, "day");
    return yqnqd.to_list();
  }
  case precision::hour: {
    collect_field<component::year>(yqnqdh, year, "year");
    collect_field<component::quarter>(yqnqdh, quarter, "quarter");
    collect_field<component::day>(yqnqdh, day, "day");
    collect_field<component::hour>(yqnqdh, hour, "hour");
    return yqnqdh.to_list();
  }
  case precision::minute: {
    collect_field<component::year>(yqnqdhm, year, "year");
    collect_field<component::quarter>(yqnqdhm, quarter, "quarter");
    collect_field<component::day>(yqnqdhm, day, "day");
    collect_field<component::hour>(yqnqdhm, hour, "hour");
    collect_field<component::minute>(yqnqdhm, minute, "minute");
    return yqnqdhm.to_list();
  }
  case precision::second: {
    collect_field<component::year>(yqnqdhms, year, "year");
    collect_field<component::quarter>(yqnqdhms, quarter, "quarter");
    collect_field<component::day>(yqnqdhms, day, "day");
    collect_field<component::hour>(yqnqdhms, hour, "hour");
    collect_field<component::minute>(yqnqdhms, minute, "minute");
    collect_field<component::second>(yqnqdhms, second, "second");
    return yqnqdhms.to_list();
  }
  case precision::millisecond: {
    collect_field<component::year>(yqnqdhmss1, year, "year");
    collect_field<component::quarter>(yqnqdhmss1, quarter, "quarter");
    collect_field<component::day>(yqnqdhmss1, day, "day");
    collect_field<component::hour>(yqnqdhmss1, hour, "hour");
    collect_field<component::minute>(yqnqdhmss1, minute, "minute");
    collect_field<component::second>(yqnqdhmss1, second, "second");
    collect_field<component::millisecond>(yqnqdhmss1, subsecond, "subsecond");
    return yqnqdhmss1.to_list();
  }
  case precision::microsecond: {
    collect_field<component::year>(yqnqdhmss2, year, "year");
    collect_field<component::quarter>(yqnqdhmss2, quarter, "quarter");
    collect_field<component::day>(yqnqdhmss2, day, "day");
    collect_field<component::hour>(yqnqdhmss2, hour, "hour");
    collect_field<component::minute>(yqnqdhmss2, minute, "minute");
    collect_field<component::second>(yqnqdhmss2, second, "second");
    collect_field<component::microsecond>(yqnqdhmss2, subsecond, "subsecond");
    return yqnqdhmss2.to_list();
  }
  case precision::nanosecond: {
    collect_field<component::year>(yqnqdhmss3, year, "year");
    collect_field<component::quarter>(yqnqdhmss3, quarter, "quarter");
    collect_field<component::day>(yqnqdhmss3, day, "day");
    collect_field<component::hour>(yqnqdhmss3, hour, "hour");
    collect_field<component::minute>(yqnqdhmss3, minute, "minute");
    collect_field<component::second>(yqnqdhmss3, second, "second");
    collect_field<component::nanosecond>(yqnqdhmss3, subsecond, "subsecond");
    return yqnqdhmss3.to_list();
  }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("collect_year_quarter_day_fields_impl");
}

[[cpp11::register]]
cpp11::writable::list
collect_year_quarter_day_fields(cpp11::list_of<cpp11::integers> fields,
                                const cpp11::integers& precision_int,
                                const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return collect_year_quarter_day_fields_impl<start::january>(fields, precision_int);
  case start::february: return collect_year_quarter_day_fields_impl<start::february>(fields, precision_int);
  case start::march: return collect_year_quarter_day_fields_impl<start::march>(fields, precision_int);
  case start::april: return collect_year_quarter_day_fields_impl<start::april>(fields, precision_int);
  case start::may: return collect_year_quarter_day_fields_impl<start::may>(fields, precision_int);
  case start::june: return collect_year_quarter_day_fields_impl<start::june>(fields, precision_int);
  case start::july: return collect_year_quarter_day_fields_impl<start::july>(fields, precision_int);
  case start::august: return collect_year_quarter_day_fields_impl<start::august>(fields, precision_int);
  case start::september: return collect_year_quarter_day_fields_impl<start::september>(fields, precision_int);
  case start::october: return collect_year_quarter_day_fields_impl<start::october>(fields, precision_int);
  case start::november: return collect_year_quarter_day_fields_impl<start::november>(fields, precision_int);
  case start::december: return collect_year_quarter_day_fields_impl<start::december>(fields, precision_int);
  }

  never_reached("collect_year_quarter_day_fields");
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
inline
cpp11::writable::strings
format_year_quarter_day_impl(cpp11::list_of<cpp11::integers> fields,
                             const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarter};
  rquarterly::yqnqd<S> yqnqd{year, quarter, day};
  rquarterly::yqnqdh<S> yqnqdh{year, quarter, day, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarter, day, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarter, day, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond};

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

  never_reached("format_year_quarter_day_impl");
}

[[cpp11::register]]
cpp11::writable::strings
format_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> fields,
                            const cpp11::integers& precision_int,
                            const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return format_year_quarter_day_impl<start::january>(fields, precision_int);
  case start::february: return format_year_quarter_day_impl<start::february>(fields, precision_int);
  case start::march: return format_year_quarter_day_impl<start::march>(fields, precision_int);
  case start::april: return format_year_quarter_day_impl<start::april>(fields, precision_int);
  case start::may: return format_year_quarter_day_impl<start::may>(fields, precision_int);
  case start::june: return format_year_quarter_day_impl<start::june>(fields, precision_int);
  case start::july: return format_year_quarter_day_impl<start::july>(fields, precision_int);
  case start::august: return format_year_quarter_day_impl<start::august>(fields, precision_int);
  case start::september: return format_year_quarter_day_impl<start::september>(fields, precision_int);
  case start::october: return format_year_quarter_day_impl<start::october>(fields, precision_int);
  case start::november: return format_year_quarter_day_impl<start::november>(fields, precision_int);
  case start::december: return format_year_quarter_day_impl<start::december>(fields, precision_int);
  }

  never_reached("format_year_quarter_day_cpp");
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
cpp11::writable::logicals
invalid_detect_year_quarter_day_impl(cpp11::list_of<cpp11::integers> fields,
                                     const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarter};
  rquarterly::yqnqd<S> yqnqd{year, quarter, day};
  rquarterly::yqnqdh<S> yqnqdh{year, quarter, day, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarter, day, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarter, day, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
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

  never_reached("invalid_detect_year_quarter_day_impl");
}

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                    const cpp11::integers& precision_int,
                                    const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return invalid_detect_year_quarter_day_impl<start::january>(fields, precision_int);
  case start::february: return invalid_detect_year_quarter_day_impl<start::february>(fields, precision_int);
  case start::march: return invalid_detect_year_quarter_day_impl<start::march>(fields, precision_int);
  case start::april: return invalid_detect_year_quarter_day_impl<start::april>(fields, precision_int);
  case start::may: return invalid_detect_year_quarter_day_impl<start::may>(fields, precision_int);
  case start::june: return invalid_detect_year_quarter_day_impl<start::june>(fields, precision_int);
  case start::july: return invalid_detect_year_quarter_day_impl<start::july>(fields, precision_int);
  case start::august: return invalid_detect_year_quarter_day_impl<start::august>(fields, precision_int);
  case start::september: return invalid_detect_year_quarter_day_impl<start::september>(fields, precision_int);
  case start::october: return invalid_detect_year_quarter_day_impl<start::october>(fields, precision_int);
  case start::november: return invalid_detect_year_quarter_day_impl<start::november>(fields, precision_int);
  case start::december: return invalid_detect_year_quarter_day_impl<start::december>(fields, precision_int);
  }

  never_reached("invalid_detect_year_quarter_day_cpp");
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
inline
bool
invalid_any_year_quarter_day_impl(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarter};
  rquarterly::yqnqd<S> yqnqd{year, quarter, day};
  rquarterly::yqnqdh<S> yqnqdh{year, quarter, day, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarter, day, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarter, day, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
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

  never_reached("invalid_any_year_quarter_day_impl");
}

[[cpp11::register]]
bool
invalid_any_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::integers& precision_int,
                                 const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return invalid_any_year_quarter_day_impl<start::january>(fields, precision_int);
  case start::february: return invalid_any_year_quarter_day_impl<start::february>(fields, precision_int);
  case start::march: return invalid_any_year_quarter_day_impl<start::march>(fields, precision_int);
  case start::april: return invalid_any_year_quarter_day_impl<start::april>(fields, precision_int);
  case start::may: return invalid_any_year_quarter_day_impl<start::may>(fields, precision_int);
  case start::june: return invalid_any_year_quarter_day_impl<start::june>(fields, precision_int);
  case start::july: return invalid_any_year_quarter_day_impl<start::july>(fields, precision_int);
  case start::august: return invalid_any_year_quarter_day_impl<start::august>(fields, precision_int);
  case start::september: return invalid_any_year_quarter_day_impl<start::september>(fields, precision_int);
  case start::october: return invalid_any_year_quarter_day_impl<start::october>(fields, precision_int);
  case start::november: return invalid_any_year_quarter_day_impl<start::november>(fields, precision_int);
  case start::december: return invalid_any_year_quarter_day_impl<start::december>(fields, precision_int);
  }

  never_reached("invalid_any_year_quarter_day_cpp");
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
inline
int
invalid_count_year_quarter_day_impl(cpp11::list_of<cpp11::integers> fields,
                                    const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarter};
  rquarterly::yqnqd<S> yqnqd{year, quarter, day};
  rquarterly::yqnqdh<S> yqnqdh{year, quarter, day, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarter, day, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarter, day, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
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

  never_reached("invalid_count_year_quarter_day_impl");
}

[[cpp11::register]]
int
invalid_count_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                   const cpp11::integers& precision_int,
                                   const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return invalid_count_year_quarter_day_impl<start::january>(fields, precision_int);
  case start::february: return invalid_count_year_quarter_day_impl<start::february>(fields, precision_int);
  case start::march: return invalid_count_year_quarter_day_impl<start::march>(fields, precision_int);
  case start::april: return invalid_count_year_quarter_day_impl<start::april>(fields, precision_int);
  case start::may: return invalid_count_year_quarter_day_impl<start::may>(fields, precision_int);
  case start::june: return invalid_count_year_quarter_day_impl<start::june>(fields, precision_int);
  case start::july: return invalid_count_year_quarter_day_impl<start::july>(fields, precision_int);
  case start::august: return invalid_count_year_quarter_day_impl<start::august>(fields, precision_int);
  case start::september: return invalid_count_year_quarter_day_impl<start::september>(fields, precision_int);
  case start::october: return invalid_count_year_quarter_day_impl<start::october>(fields, precision_int);
  case start::november: return invalid_count_year_quarter_day_impl<start::november>(fields, precision_int);
  case start::december: return invalid_count_year_quarter_day_impl<start::december>(fields, precision_int);
  }

  never_reached("invalid_count_year_quarter_day_cpp");
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
inline
cpp11::writable::list
invalid_resolve_year_quarter_day_impl(cpp11::list_of<cpp11::integers> fields,
                                      const cpp11::integers& precision_int,
                                      const cpp11::strings& invalid_string) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid_string);

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarter};
  rquarterly::yqnqd<S> yqnqd{year, quarter, day};
  rquarterly::yqnqdh<S> yqnqdh{year, quarter, day, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarter, day, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarter, day, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_int)) {
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

  never_reached("invalid_resolve_year_quarter_day_impl");
}

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                     const cpp11::integers& precision_int,
                                     const cpp11::integers& start_int,
                                     const cpp11::strings& invalid_string) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return invalid_resolve_year_quarter_day_impl<start::january>(fields, precision_int, invalid_string);
  case start::february: return invalid_resolve_year_quarter_day_impl<start::february>(fields, precision_int, invalid_string);
  case start::march: return invalid_resolve_year_quarter_day_impl<start::march>(fields, precision_int, invalid_string);
  case start::april: return invalid_resolve_year_quarter_day_impl<start::april>(fields, precision_int, invalid_string);
  case start::may: return invalid_resolve_year_quarter_day_impl<start::may>(fields, precision_int, invalid_string);
  case start::june: return invalid_resolve_year_quarter_day_impl<start::june>(fields, precision_int, invalid_string);
  case start::july: return invalid_resolve_year_quarter_day_impl<start::july>(fields, precision_int, invalid_string);
  case start::august: return invalid_resolve_year_quarter_day_impl<start::august>(fields, precision_int, invalid_string);
  case start::september: return invalid_resolve_year_quarter_day_impl<start::september>(fields, precision_int, invalid_string);
  case start::october: return invalid_resolve_year_quarter_day_impl<start::october>(fields, precision_int, invalid_string);
  case start::november: return invalid_resolve_year_quarter_day_impl<start::november>(fields, precision_int, invalid_string);
  case start::december: return invalid_resolve_year_quarter_day_impl<start::december>(fields, precision_int, invalid_string);
  }

  never_reached("invalid_resolve_year_quarter_day_cpp");
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static
cpp11::writable::list
set_field_year_quarter_day_impl(cpp11::list_of<cpp11::integers> fields,
                                const cpp11::integers& value,
                                const cpp11::integers& precision_fields,
                                const cpp11::integers& precision_value) {
  using namespace rclock;
  rclock::integers value2(value);

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarter};
  rquarterly::yqnqd<S> yqnqd{year, quarter, day};
  rquarterly::yqnqdh<S> yqnqdh{year, quarter, day, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarter, day, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarter, day, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::year: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(y, value2);
    case precision::quarter: return set_field_calendar<component::quarter>(y, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::quarter: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqn, value2);
    case precision::quarter: return set_field_calendar<component::quarter>(yqn, value2);
    case precision::day: return set_field_calendar<component::day>(yqn, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::day: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqd, value2);
    case precision::quarter: return set_field_calendar<component::quarter>(yqnqd, value2);
    case precision::day: return set_field_calendar<component::day>(yqnqd, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqd, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::hour: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqdh, value2);
    case precision::quarter: return set_field_calendar<component::quarter>(yqnqdh, value2);
    case precision::day: return set_field_calendar<component::day>(yqnqdh, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqdh, value2);
    case precision::minute: return set_field_calendar<component::minute>(yqnqdh, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::minute: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqdhm, value2);
    case precision::quarter: return set_field_calendar<component::quarter>(yqnqdhm, value2);
    case precision::day: return set_field_calendar<component::day>(yqnqdhm, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqdhm, value2);
    case precision::minute: return set_field_calendar<component::minute>(yqnqdhm, value2);
    case precision::second: return set_field_calendar<component::second>(yqnqdhm, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision::second: {
    switch (parse_precision(precision_value)) {
    case precision::year: return set_field_calendar<component::year>(yqnqdhms, value2);
    case precision::quarter: return set_field_calendar<component::quarter>(yqnqdhms, value2);
    case precision::day: return set_field_calendar<component::day>(yqnqdhms, value2);
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
    case precision::quarter: return set_field_calendar<component::quarter>(yqnqdhmss1, value2);
    case precision::day: return set_field_calendar<component::day>(yqnqdhmss1, value2);
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
    case precision::quarter: return set_field_calendar<component::quarter>(yqnqdhmss2, value2);
    case precision::day: return set_field_calendar<component::day>(yqnqdhmss2, value2);
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
    case precision::quarter: return set_field_calendar<component::quarter>(yqnqdhmss3, value2);
    case precision::day: return set_field_calendar<component::day>(yqnqdhmss3, value2);
    case precision::hour: return set_field_calendar<component::hour>(yqnqdhmss3, value2);
    case precision::minute: return set_field_calendar<component::minute>(yqnqdhmss3, value2);
    case precision::second: return set_field_calendar<component::second>(yqnqdhmss3, value2);
    case precision::nanosecond: return set_field_calendar<component::nanosecond>(yqnqdhmss3, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("set_field_year_quarter_day_impl");
}

[[cpp11::register]]
cpp11::writable::list
set_field_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::integers& value,
                               const cpp11::integers& precision_fields,
                               const cpp11::integers& precision_value,
                               const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return set_field_year_quarter_day_impl<start::january>(fields, value, precision_fields, precision_value);
  case start::february: return set_field_year_quarter_day_impl<start::february>(fields, value, precision_fields, precision_value);
  case start::march: return set_field_year_quarter_day_impl<start::march>(fields, value, precision_fields, precision_value);
  case start::april: return set_field_year_quarter_day_impl<start::april>(fields, value, precision_fields, precision_value);
  case start::may: return set_field_year_quarter_day_impl<start::may>(fields, value, precision_fields, precision_value);
  case start::june: return set_field_year_quarter_day_impl<start::june>(fields, value, precision_fields, precision_value);
  case start::july: return set_field_year_quarter_day_impl<start::july>(fields, value, precision_fields, precision_value);
  case start::august: return set_field_year_quarter_day_impl<start::august>(fields, value, precision_fields, precision_value);
  case start::september: return set_field_year_quarter_day_impl<start::september>(fields, value, precision_fields, precision_value);
  case start::october: return set_field_year_quarter_day_impl<start::october>(fields, value, precision_fields, precision_value);
  case start::november: return set_field_year_quarter_day_impl<start::november>(fields, value, precision_fields, precision_value);
  case start::december: return set_field_year_quarter_day_impl<start::december>(fields, value, precision_fields, precision_value);
  }

  never_reached("set_field_year_quarter_day_cpp");
}

template <quarterly::start S, class Calendar>
cpp11::writable::list
set_field_year_quarter_day_last_impl(const Calendar& x) {
  const r_ssize size = x.size();
  cpp11::writable::integers value(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      value[i] = r_int_na;
    } else {
      quarterly::year_quarternum_quarterday<S> yqnqdl = x.to_year_quarternum(i) / quarterly::last;
      value[i] = static_cast<int>(static_cast<unsigned>(yqnqdl.quarterday()));
    }
  }

  cpp11::writable::list out({x.to_list(), value});
  out.names() = {"fields", "value"};

  return out;
}

template <quarterly::start S>
cpp11::writable::list
set_field_year_quarter_day_last_switch(cpp11::list_of<cpp11::integers> fields,
                                       const cpp11::integers& precision_fields) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarter};
  rquarterly::yqnqd<S> yqnqd{year, quarter, day};
  rquarterly::yqnqdh<S> yqnqdh{year, quarter, day, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarter, day, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarter, day, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond};

  switch (parse_precision(precision_fields)) {
  case precision::quarter: return set_field_year_quarter_day_last_impl<S>(yqn);
  case precision::day: return set_field_year_quarter_day_last_impl<S>(yqnqd);
  case precision::hour: return set_field_year_quarter_day_last_impl<S>(yqnqdh);
  case precision::minute: return set_field_year_quarter_day_last_impl<S>(yqnqdhm);
  case precision::second: return set_field_year_quarter_day_last_impl<S>(yqnqdhms);
  case precision::millisecond: return set_field_year_quarter_day_last_impl<S>(yqnqdhmss1);
  case precision::microsecond: return set_field_year_quarter_day_last_impl<S>(yqnqdhmss2);
  case precision::nanosecond: return set_field_year_quarter_day_last_impl<S>(yqnqdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("set_field_year_quarter_day_last_switch");
}

[[cpp11::register]]
cpp11::writable::list
set_field_year_quarter_day_last_cpp(cpp11::list_of<cpp11::integers> fields,
                                    const cpp11::integers& precision_fields,
                                    const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return set_field_year_quarter_day_last_switch<start::january>(fields, precision_fields);
  case start::february: return set_field_year_quarter_day_last_switch<start::february>(fields, precision_fields);
  case start::march: return set_field_year_quarter_day_last_switch<start::march>(fields, precision_fields);
  case start::april: return set_field_year_quarter_day_last_switch<start::april>(fields, precision_fields);
  case start::may: return set_field_year_quarter_day_last_switch<start::may>(fields, precision_fields);
  case start::june: return set_field_year_quarter_day_last_switch<start::june>(fields, precision_fields);
  case start::july: return set_field_year_quarter_day_last_switch<start::july>(fields, precision_fields);
  case start::august: return set_field_year_quarter_day_last_switch<start::august>(fields, precision_fields);
  case start::september: return set_field_year_quarter_day_last_switch<start::september>(fields, precision_fields);
  case start::october: return set_field_year_quarter_day_last_switch<start::october>(fields, precision_fields);
  case start::november: return set_field_year_quarter_day_last_switch<start::november>(fields, precision_fields);
  case start::december: return set_field_year_quarter_day_last_switch<start::december>(fields, precision_fields);
  }

  never_reached("set_field_year_quarter_day_last_cpp");
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
cpp11::writable::list
year_quarter_day_plus_duration_impl(cpp11::list_of<cpp11::integers> fields,
                                    cpp11::list_of<cpp11::integers> fields_n,
                                    const cpp11::integers& precision_fields,
                                    const cpp11::integers& precision_n) {
  using namespace rclock;

  const enum precision precision_fields_val = parse_precision(precision_fields);
  const enum precision precision_n_val = parse_precision(precision_n);

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarter};
  rquarterly::yqnqd<S> yqnqd{year, quarter, day};
  rquarterly::yqnqdh<S> yqnqdh{year, quarter, day, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarter, day, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarter, day, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond};

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

  never_reached("year_quarter_day_plus_duration_impl");
}

[[cpp11::register]]
cpp11::writable::list
year_quarter_day_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                   cpp11::list_of<cpp11::integers> fields_n,
                                   const cpp11::integers& precision_fields,
                                   const cpp11::integers& precision_n,
                                   const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return year_quarter_day_plus_duration_impl<start::january>(fields, fields_n, precision_fields, precision_n);
  case start::february: return year_quarter_day_plus_duration_impl<start::february>(fields, fields_n, precision_fields, precision_n);
  case start::march: return year_quarter_day_plus_duration_impl<start::march>(fields, fields_n, precision_fields, precision_n);
  case start::april: return year_quarter_day_plus_duration_impl<start::april>(fields, fields_n, precision_fields, precision_n);
  case start::may: return year_quarter_day_plus_duration_impl<start::may>(fields, fields_n, precision_fields, precision_n);
  case start::june: return year_quarter_day_plus_duration_impl<start::june>(fields, fields_n, precision_fields, precision_n);
  case start::july: return year_quarter_day_plus_duration_impl<start::july>(fields, fields_n, precision_fields, precision_n);
  case start::august: return year_quarter_day_plus_duration_impl<start::august>(fields, fields_n, precision_fields, precision_n);
  case start::september: return year_quarter_day_plus_duration_impl<start::september>(fields, fields_n, precision_fields, precision_n);
  case start::october: return year_quarter_day_plus_duration_impl<start::october>(fields, fields_n, precision_fields, precision_n);
  case start::november: return year_quarter_day_plus_duration_impl<start::november>(fields, fields_n, precision_fields, precision_n);
  case start::december: return year_quarter_day_plus_duration_impl<start::december>(fields, fields_n, precision_fields, precision_n);
  }

  never_reached("year_quarter_day_plus_duration_cpp");
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
cpp11::writable::list
as_sys_time_year_quarter_day_impl(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::integers& precision_int) {
  using namespace rclock;

  cpp11::integers year = rquarterly::get_year(fields);
  cpp11::integers quarter = rquarterly::get_quarter(fields);
  cpp11::integers day = rquarterly::get_day(fields);
  cpp11::integers hour = rquarterly::get_hour(fields);
  cpp11::integers minute = rquarterly::get_minute(fields);
  cpp11::integers second = rquarterly::get_second(fields);
  cpp11::integers subsecond = rquarterly::get_subsecond(fields);

  rquarterly::y<S> y{year};
  rquarterly::yqn<S> yqn{year, quarter};
  rquarterly::yqnqd<S> yqnqd{year, quarter, day};
  rquarterly::yqnqdh<S> yqnqdh{year, quarter, day, hour};
  rquarterly::yqnqdhm<S> yqnqdhm{year, quarter, day, hour, minute};
  rquarterly::yqnqdhms<S> yqnqdhms{year, quarter, day, hour, minute, second};
  rquarterly::yqnqdhmss<std::chrono::milliseconds, S> yqnqdhmss1{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::microseconds, S> yqnqdhmss2{year, quarter, day, hour, minute, second, subsecond};
  rquarterly::yqnqdhmss<std::chrono::nanoseconds, S> yqnqdhmss3{year, quarter, day, hour, minute, second, subsecond};

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

  never_reached("as_sys_time_year_quarter_day_impl");
}

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::integers& precision_int,
                                 const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return as_sys_time_year_quarter_day_impl<start::january>(fields, precision_int);
  case start::february: return as_sys_time_year_quarter_day_impl<start::february>(fields, precision_int);
  case start::march: return as_sys_time_year_quarter_day_impl<start::march>(fields, precision_int);
  case start::april: return as_sys_time_year_quarter_day_impl<start::april>(fields, precision_int);
  case start::may: return as_sys_time_year_quarter_day_impl<start::may>(fields, precision_int);
  case start::june: return as_sys_time_year_quarter_day_impl<start::june>(fields, precision_int);
  case start::july: return as_sys_time_year_quarter_day_impl<start::july>(fields, precision_int);
  case start::august: return as_sys_time_year_quarter_day_impl<start::august>(fields, precision_int);
  case start::september: return as_sys_time_year_quarter_day_impl<start::september>(fields, precision_int);
  case start::october: return as_sys_time_year_quarter_day_impl<start::october>(fields, precision_int);
  case start::november: return as_sys_time_year_quarter_day_impl<start::november>(fields, precision_int);
  case start::december: return as_sys_time_year_quarter_day_impl<start::december>(fields, precision_int);
  }

  never_reached("as_sys_time_year_quarter_day_cpp");
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
cpp11::writable::list
as_year_quarter_day_from_sys_time_impl(cpp11::list_of<cpp11::integers> fields,
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
  case precision::day: return as_calendar_from_sys_time_impl<rquarterly::yqnqd<S>>(dd);
  case precision::hour: return as_calendar_from_sys_time_impl<rquarterly::yqnqdh<S>>(dh);
  case precision::minute: return as_calendar_from_sys_time_impl<rquarterly::yqnqdhm<S>>(dmin);
  case precision::second: return as_calendar_from_sys_time_impl<rquarterly::yqnqdhms<S>>(ds);
  case precision::millisecond: return as_calendar_from_sys_time_impl<rquarterly::yqnqdhmss<std::chrono::milliseconds, S>>(dmilli);
  case precision::microsecond: return as_calendar_from_sys_time_impl<rquarterly::yqnqdhmss<std::chrono::microseconds, S>>(dmicro);
  case precision::nanosecond: return as_calendar_from_sys_time_impl<rquarterly::yqnqdhmss<std::chrono::nanoseconds, S>>(dnano);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("as_year_quarter_day_from_sys_time_impl");
}

[[cpp11::register]]
cpp11::writable::list
as_year_quarter_day_from_sys_time_cpp(cpp11::list_of<cpp11::integers> fields,
                                      const cpp11::integers& precision_int,
                                      const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return as_year_quarter_day_from_sys_time_impl<start::january>(fields, precision_int);
  case start::february: return as_year_quarter_day_from_sys_time_impl<start::february>(fields, precision_int);
  case start::march: return as_year_quarter_day_from_sys_time_impl<start::march>(fields, precision_int);
  case start::april: return as_year_quarter_day_from_sys_time_impl<start::april>(fields, precision_int);
  case start::may: return as_year_quarter_day_from_sys_time_impl<start::may>(fields, precision_int);
  case start::june: return as_year_quarter_day_from_sys_time_impl<start::june>(fields, precision_int);
  case start::july: return as_year_quarter_day_from_sys_time_impl<start::july>(fields, precision_int);
  case start::august: return as_year_quarter_day_from_sys_time_impl<start::august>(fields, precision_int);
  case start::september: return as_year_quarter_day_from_sys_time_impl<start::september>(fields, precision_int);
  case start::october: return as_year_quarter_day_from_sys_time_impl<start::october>(fields, precision_int);
  case start::november: return as_year_quarter_day_from_sys_time_impl<start::november>(fields, precision_int);
  case start::december: return as_year_quarter_day_from_sys_time_impl<start::december>(fields, precision_int);
  }

  never_reached("as_year_quarter_day_from_sys_time_cpp");
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
year_quarter_minus_year_quarter_impl(const rclock::rquarterly::yqn<S>& x,
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
year_quarter_day_minus_year_quarter_day_impl(cpp11::list_of<cpp11::integers> x,
                                             cpp11::list_of<cpp11::integers> y,
                                             const cpp11::integers& precision_int) {
  const cpp11::integers x_year = rclock::rquarterly::get_year(x);
  const cpp11::integers x_quarter = rclock::rquarterly::get_quarter(x);

  const cpp11::integers y_year = rclock::rquarterly::get_year(y);
  const cpp11::integers y_quarter = rclock::rquarterly::get_quarter(y);

  const rclock::rquarterly::y<S> x_y{x_year};
  const rclock::rquarterly::yqn<S> x_yqn{x_year, x_quarter};

  const rclock::rquarterly::y<S> y_y{y_year};
  const rclock::rquarterly::yqn<S> y_yqn{y_year, y_quarter};

  switch (parse_precision(precision_int)) {
  case precision::year: return year_minus_year_impl(x_y, y_y);
  case precision::quarter: return year_quarter_minus_year_quarter_impl(x_yqn, y_yqn);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_quarter_day_minus_year_quarter_day_impl");
}

[[cpp11::register]]
cpp11::writable::list
year_quarter_day_minus_year_quarter_day_cpp(cpp11::list_of<cpp11::integers> x,
                                            cpp11::list_of<cpp11::integers> y,
                                            const cpp11::integers& precision_int,
                                            const cpp11::integers& start_int) {
  using namespace quarterly;

  switch (parse_start(start_int)) {
  case start::january: return year_quarter_day_minus_year_quarter_day_impl<start::january>(x, y, precision_int);
  case start::february: return year_quarter_day_minus_year_quarter_day_impl<start::february>(x, y, precision_int);
  case start::march: return year_quarter_day_minus_year_quarter_day_impl<start::march>(x, y, precision_int);
  case start::april: return year_quarter_day_minus_year_quarter_day_impl<start::april>(x, y, precision_int);
  case start::may: return year_quarter_day_minus_year_quarter_day_impl<start::may>(x, y, precision_int);
  case start::june: return year_quarter_day_minus_year_quarter_day_impl<start::june>(x, y, precision_int);
  case start::july: return year_quarter_day_minus_year_quarter_day_impl<start::july>(x, y, precision_int);
  case start::august: return year_quarter_day_minus_year_quarter_day_impl<start::august>(x, y, precision_int);
  case start::september: return year_quarter_day_minus_year_quarter_day_impl<start::september>(x, y, precision_int);
  case start::october: return year_quarter_day_minus_year_quarter_day_impl<start::october>(x, y, precision_int);
  case start::november: return year_quarter_day_minus_year_quarter_day_impl<start::november>(x, y, precision_int);
  case start::december: return year_quarter_day_minus_year_quarter_day_impl<start::december>(x, y, precision_int);
  }

  never_reached("year_quarter_day_minus_year_quarter_day_cpp");
}
