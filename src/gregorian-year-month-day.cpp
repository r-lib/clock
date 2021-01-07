#include "gregorian-year-month-day.h"
#include "duration.h"
#include "check.h"
#include "enums.h"
#include "get.h"

template <typename Duration, class Calendar>
void
collect_field(Calendar& x, const cpp11::integers& field, const char* arg) {
  r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    const int elt = field[i];

    if (elt == r_int_na) {
      x.assign_na(i);
      continue;
    }

    check_range<Duration>(elt, arg);
  }
}

[[cpp11::register]]
cpp11::writable::list
collect_year_month_day_fields(cpp11::list_of<cpp11::integers> fields,
                              const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = get_year(fields);
  cpp11::integers month = get_month(fields);
  cpp11::integers day = get_day(fields);
  cpp11::integers hour = get_hour(fields);
  cpp11::integers minute = get_minute(fields);
  cpp11::integers second = get_second(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};

  switch (parse_precision2(precision)) {
  case precision2::year: {
    collect_field<date::years>(y, year, "year");
    return y.to_list();
  }
  case precision2::month: {
    collect_field<date::years>(ym, year, "year");
    collect_field<date::months>(ym, month, "month");
    return ym.to_list();
  }
  case precision2::day: {
    collect_field<date::years>(ymd, year, "year");
    collect_field<date::months>(ymd, month, "month");
    collect_field<date::days>(ymd, day, "day");
    return ymd.to_list();
  }
  case precision2::hour: {
    collect_field<date::years>(ymdh, year, "year");
    collect_field<date::months>(ymdh, month, "month");
    collect_field<date::days>(ymdh, day, "day");
    collect_field<std::chrono::hours>(ymdh, hour, "hour");
    return ymdh.to_list();
  }
  case precision2::minute: {
    collect_field<date::years>(ymdhm, year, "year");
    collect_field<date::months>(ymdhm, month, "month");
    collect_field<date::days>(ymdhm, day, "day");
    collect_field<std::chrono::hours>(ymdhm, hour, "hour");
    collect_field<std::chrono::minutes>(ymdhm, minute, "minute");
    return ymdhm.to_list();
  }
  case precision2::second: {
    collect_field<date::years>(ymdhms, year, "year");
    collect_field<date::months>(ymdhms, month, "month");
    collect_field<date::days>(ymdhms, day, "day");
    collect_field<std::chrono::hours>(ymdhms, hour, "hour");
    collect_field<std::chrono::minutes>(ymdhms, minute, "minute");
    collect_field<std::chrono::seconds>(ymdhms, second, "second");
    return ymdhms.to_list();
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

template <class Calendar>
cpp11::writable::strings
format_year_month_day_impl(const Calendar& x) {
  const r_ssize size = x.size();
  cpp11::writable::strings out(size);

  std::ostringstream stream;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    stream.str(std::string());
    stream.clear();

    x.stream(stream, i);

    if (stream.fail()) {
      // Should never happen but...
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    const std::string string = stream.str();
    const SEXP r_string = Rf_mkCharLenCE(string.c_str(), string.size(), CE_UTF8);
    SET_STRING_ELT(out, i, r_string);
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::strings
format_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                          const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = get_year(fields);
  cpp11::integers month = get_month(fields);
  cpp11::integers day = get_day(fields);
  cpp11::integers hour = get_hour(fields);
  cpp11::integers minute = get_minute(fields);
  cpp11::integers second = get_second(fields);
  cpp11::integers subsecond = get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: return format_year_month_day_impl(y);
  case precision2::month: return format_year_month_day_impl(ym);
  case precision2::day: return format_year_month_day_impl(ymd);
  case precision2::hour: return format_year_month_day_impl(ymdh);
  case precision2::minute: return format_year_month_day_impl(ymdhm);
  case precision2::second: return format_year_month_day_impl(ymdhms);
  case precision2::millisecond: return format_year_month_day_impl(ymdhmss1);
  case precision2::microsecond: return format_year_month_day_impl(ymdhmss2);
  case precision2::nanosecond: return format_year_month_day_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

template <class Calendar>
cpp11::writable::logicals
invalid_detect_year_month_day_impl(const Calendar& x) {
  const r_ssize size = x.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = false;
    } else {
      out[i] = !x.ok(i);
    }
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = get_year(fields);
  cpp11::integers month = get_month(fields);
  cpp11::integers day = get_day(fields);
  cpp11::integers hour = get_hour(fields);
  cpp11::integers minute = get_minute(fields);
  cpp11::integers second = get_second(fields);
  cpp11::integers subsecond = get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_detect_year_month_day_impl(y);
  case precision2::month: return invalid_detect_year_month_day_impl(ym);
  case precision2::day: return invalid_detect_year_month_day_impl(ymd);
  case precision2::hour: return invalid_detect_year_month_day_impl(ymdh);
  case precision2::minute: return invalid_detect_year_month_day_impl(ymdhm);
  case precision2::second: return invalid_detect_year_month_day_impl(ymdhms);
  case precision2::millisecond: return invalid_detect_year_month_day_impl(ymdhmss1);
  case precision2::microsecond: return invalid_detect_year_month_day_impl(ymdhmss2);
  case precision2::nanosecond: return invalid_detect_year_month_day_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

template <class Calendar>
bool
invalid_any_year_month_day_impl(const Calendar& x) {
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || x.ok(i)) {
      continue;
    }
    return true;
  }

  return false;
}

[[cpp11::register]]
bool
invalid_any_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields, cpp11::strings precision) {
  using namespace rclock;

  cpp11::integers year = get_year(fields);
  cpp11::integers month = get_month(fields);
  cpp11::integers day = get_day(fields);
  cpp11::integers hour = get_hour(fields);
  cpp11::integers minute = get_minute(fields);
  cpp11::integers second = get_second(fields);
  cpp11::integers subsecond = get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_any_year_month_day_impl(y);
  case precision2::month: return invalid_any_year_month_day_impl(ym);
  case precision2::day: return invalid_any_year_month_day_impl(ymd);
  case precision2::hour: return invalid_any_year_month_day_impl(ymdh);
  case precision2::minute: return invalid_any_year_month_day_impl(ymdhm);
  case precision2::second: return invalid_any_year_month_day_impl(ymdhms);
  case precision2::millisecond: return invalid_any_year_month_day_impl(ymdhmss1);
  case precision2::microsecond: return invalid_any_year_month_day_impl(ymdhmss2);
  case precision2::nanosecond: return invalid_any_year_month_day_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

template <class Calendar>
int
invalid_count_year_month_day_impl(const Calendar& x) {
  int count = 0;
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      continue;
    }
    if (!x.ok(i)) {
      ++count;
    }
  }

  return count;
}

[[cpp11::register]]
int
invalid_count_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                 const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = get_year(fields);
  cpp11::integers month = get_month(fields);
  cpp11::integers day = get_day(fields);
  cpp11::integers hour = get_hour(fields);
  cpp11::integers minute = get_minute(fields);
  cpp11::integers second = get_second(fields);
  cpp11::integers subsecond = get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_count_year_month_day_impl(y);
  case precision2::month: return invalid_count_year_month_day_impl(ym);
  case precision2::day: return invalid_count_year_month_day_impl(ymd);
  case precision2::hour: return invalid_count_year_month_day_impl(ymdh);
  case precision2::minute: return invalid_count_year_month_day_impl(ymdhm);
  case precision2::second: return invalid_count_year_month_day_impl(ymdhms);
  case precision2::millisecond: return invalid_count_year_month_day_impl(ymdhmss1);
  case precision2::microsecond: return invalid_count_year_month_day_impl(ymdhmss2);
  case precision2::nanosecond: return invalid_count_year_month_day_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

template <class Calendar>
cpp11::writable::list
invalid_resolve_year_month_day_impl(Calendar& x,
                                    const enum invalid& invalid_val) {
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      continue;
    }
    x.resolve(i, invalid_val);
  }

  return x.to_list();
}

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                   const cpp11::strings& precision,
                                   const cpp11::strings& invalid) {
  using namespace rclock;
  const enum invalid invalid_val = parse_invalid(invalid);

  cpp11::integers year = get_year(fields);
  cpp11::integers month = get_month(fields);
  cpp11::integers day = get_day(fields);
  cpp11::integers hour = get_hour(fields);
  cpp11::integers minute = get_minute(fields);
  cpp11::integers second = get_second(fields);
  cpp11::integers subsecond = get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_resolve_year_month_day_impl(y, invalid_val);
  case precision2::month: return invalid_resolve_year_month_day_impl(ym, invalid_val);
  case precision2::day: return invalid_resolve_year_month_day_impl(ymd, invalid_val);
  case precision2::hour: return invalid_resolve_year_month_day_impl(ymdh, invalid_val);
  case precision2::minute: return invalid_resolve_year_month_day_impl(ymdhm, invalid_val);
  case precision2::second: return invalid_resolve_year_month_day_impl(ymdhms, invalid_val);
  case precision2::millisecond: return invalid_resolve_year_month_day_impl(ymdhmss1, invalid_val);
  case precision2::microsecond: return invalid_resolve_year_month_day_impl(ymdhmss2, invalid_val);
  case precision2::nanosecond: return invalid_resolve_year_month_day_impl(ymdhmss3, invalid_val);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

template <typename Duration, class Calendar>
cpp11::writable::list
set_field_calendar(Calendar& x, rclock::integers& value) {
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      if (!value.is_na(i)) {
        value.assign_na(i);
      }
    } else if (value.is_na(i)) {
      x.assign_na(i);
    } else {
      check_range<Duration>(value[i], "value");
    }
  }

  cpp11::writable::list out({x.to_list(), value.sexp()});
  out.names() = {"fields", "value"};

  return out;
}

[[cpp11::register]]
cpp11::writable::list
set_field_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                             const cpp11::integers& value,
                             const cpp11::strings& precision_fields,
                             const cpp11::strings& precision_value) {
  using namespace rclock;
  rclock::integers value2(value);

  cpp11::integers year = get_year(fields);
  cpp11::integers month = get_month(fields);
  cpp11::integers day = get_day(fields);
  cpp11::integers hour = get_hour(fields);
  cpp11::integers minute = get_minute(fields);
  cpp11::integers second = get_second(fields);
  cpp11::integers subsecond = get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision2(precision_fields)) {
  case precision2::year: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::years>(y, value2);
    case precision2::month: return set_field_calendar<date::months>(y, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::month: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::years>(ym, value2);
    case precision2::month: return set_field_calendar<date::months>(ym, value2);
    case precision2::day: return set_field_calendar<date::days>(ym, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::day: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::years>(ymd, value2);
    case precision2::month: return set_field_calendar<date::months>(ymd, value2);
    case precision2::day: return set_field_calendar<date::days>(ymd, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymd, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::hour: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::years>(ymdh, value2);
    case precision2::month: return set_field_calendar<date::months>(ymdh, value2);
    case precision2::day: return set_field_calendar<date::days>(ymdh, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymdh, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(ymdh, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::minute: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::years>(ymdhm, value2);
    case precision2::month: return set_field_calendar<date::months>(ymdhm, value2);
    case precision2::day: return set_field_calendar<date::days>(ymdhm, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymdhm, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(ymdhm, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(ymdhm, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::second: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::years>(ymdhms, value2);
    case precision2::month: return set_field_calendar<date::months>(ymdhms, value2);
    case precision2::day: return set_field_calendar<date::days>(ymdhms, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymdhms, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(ymdhms, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(ymdhms, value2);
    case precision2::millisecond: return set_field_calendar<std::chrono::milliseconds>(ymdhms, value2);
    case precision2::microsecond: return set_field_calendar<std::chrono::microseconds>(ymdhms, value2);
    case precision2::nanosecond: return set_field_calendar<std::chrono::nanoseconds>(ymdhms, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::millisecond: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::years>(ymdhmss1, value2);
    case precision2::month: return set_field_calendar<date::months>(ymdhmss1, value2);
    case precision2::day: return set_field_calendar<date::days>(ymdhmss1, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymdhmss1, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(ymdhmss1, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(ymdhmss1, value2);
    case precision2::millisecond: return set_field_calendar<std::chrono::milliseconds>(ymdhmss1, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::microsecond: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::years>(ymdhmss2, value2);
    case precision2::month: return set_field_calendar<date::months>(ymdhmss2, value2);
    case precision2::day: return set_field_calendar<date::days>(ymdhmss2, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymdhmss2, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(ymdhmss2, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(ymdhmss2, value2);
    case precision2::microsecond: return set_field_calendar<std::chrono::microseconds>(ymdhmss2, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::nanosecond: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::years>(ymdhmss3, value2);
    case precision2::month: return set_field_calendar<date::months>(ymdhmss3, value2);
    case precision2::day: return set_field_calendar<date::days>(ymdhmss3, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymdhmss3, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(ymdhmss3, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(ymdhmss3, value2);
    case precision2::nanosecond: return set_field_calendar<std::chrono::nanoseconds>(ymdhmss3, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

template <class Calendar>
cpp11::writable::list
set_field_year_month_day_last_impl(const Calendar& x) {
  const r_ssize size = x.size();
  cpp11::writable::integers day(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      day[i] = r_int_na;
    } else {
      date::year_month_day_last ymdl = x.to_year_month(i) / date::last;
      day[i] = static_cast<int>(static_cast<unsigned>(ymdl.day()));
    }
  }

  cpp11::writable::list out({x.to_list(), day});
  out.names() = {"fields", "value"};

  return out;
}

[[cpp11::register]]
cpp11::writable::list
set_field_year_month_day_last_cpp(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::strings& precision_fields) {
  using namespace rclock;

  cpp11::integers year = get_year(fields);
  cpp11::integers month = get_month(fields);
  cpp11::integers day = get_day(fields);
  cpp11::integers hour = get_hour(fields);
  cpp11::integers minute = get_minute(fields);
  cpp11::integers second = get_second(fields);
  cpp11::integers subsecond = get_subsecond(fields);

  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision2(precision_fields)) {
  case precision2::month: return set_field_year_month_day_last_impl(ym);
  case precision2::day: return set_field_year_month_day_last_impl(ymd);
  case precision2::hour: return set_field_year_month_day_last_impl(ymdh);
  case precision2::minute: return set_field_year_month_day_last_impl(ymdhm);
  case precision2::second: return set_field_year_month_day_last_impl(ymdhms);
  case precision2::millisecond: return set_field_year_month_day_last_impl(ymdhmss1);
  case precision2::microsecond: return set_field_year_month_day_last_impl(ymdhmss2);
  case precision2::nanosecond: return set_field_year_month_day_last_impl(ymdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

template <class Calendar, class ClockDuration>
cpp11::writable::list
add_field_year_month_day(Calendar& x, const ClockDuration& n) {
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      continue;
    }
    if (n.is_na(i)) {
      x.assign_na(i);
      continue;
    }

    x.add(n[i], i);
  }

  return x.to_list();
}

[[cpp11::register]]
cpp11::writable::list
add_field_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                             cpp11::list_of<cpp11::integers> fields_n,
                             const cpp11::strings& precision_fields,
                             const cpp11::strings& precision_n) {
  using namespace rclock;

  const enum precision2 precision_fields_val = parse_precision2(precision_fields);
  const enum precision2 precision_n_val = parse_precision2(precision_n);

  cpp11::integers year = get_year(fields);
  cpp11::integers month = get_month(fields);
  cpp11::integers day = get_day(fields);
  cpp11::integers hour = get_hour(fields);
  cpp11::integers minute = get_minute(fields);
  cpp11::integers second = get_second(fields);
  cpp11::integers subsecond = get_subsecond(fields);

  gregorian::y y{year};
  gregorian::ym ym{year, month};
  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  cpp11::integers ticks = get_ticks(fields_n);

  duration::years dy{ticks};
  duration::months dm{ticks};

  switch (precision_fields_val) {
  case precision2::year:
    switch (precision_n_val) {
    case precision2::year: return add_field_year_month_day(y, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::month:
    switch (precision_n_val) {
    case precision2::year: return add_field_year_month_day(ym, dy);
    case precision2::month: return add_field_year_month_day(ym, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::day:
    switch (precision_n_val) {
    case precision2::year: return add_field_year_month_day(ymd, dy);
    case precision2::month: return add_field_year_month_day(ymd, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::hour:
    switch (precision_n_val) {
    case precision2::year: return add_field_year_month_day(ymdh, dy);
    case precision2::month: return add_field_year_month_day(ymdh, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::minute:
    switch (precision_n_val) {
    case precision2::year: return add_field_year_month_day(ymdhm, dy);
    case precision2::month: return add_field_year_month_day(ymdhm, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::second:
    switch (precision_n_val) {
    case precision2::year: return add_field_year_month_day(ymdhms, dy);
    case precision2::month: return add_field_year_month_day(ymdhms, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::millisecond:
    switch (precision_n_val) {
    case precision2::year: return add_field_year_month_day(ymdhmss1, dy);
    case precision2::month: return add_field_year_month_day(ymdhmss1, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::microsecond:
    switch (precision_n_val) {
    case precision2::year: return add_field_year_month_day(ymdhmss2, dy);
    case precision2::month: return add_field_year_month_day(ymdhmss2, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision2::nanosecond:
    switch (precision_n_val) {
    case precision2::year: return add_field_year_month_day(ymdhmss3, dy);
    case precision2::month: return add_field_year_month_day(ymdhmss3, dm);
    default: clock_abort("Internal error: Invalid precision.");
    }
  default: clock_abort("Internal error: Invalid precision.");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration, class Calendar>
cpp11::writable::list
as_sys_time_year_month_day_impl(const Calendar& x) {
  const r_ssize size = x.size();
  ClockDuration out(size);
  using Duration = typename ClockDuration::duration;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
    } else {
      date::sys_time<Duration> elt_st = x.to_sys_time(i);
      Duration elt = elt_st.time_since_epoch();
      out.assign(elt, i);
    }
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers year = get_year(fields);
  cpp11::integers month = get_month(fields);
  cpp11::integers day = get_day(fields);
  cpp11::integers hour = get_hour(fields);
  cpp11::integers minute = get_minute(fields);
  cpp11::integers second = get_second(fields);
  cpp11::integers subsecond = get_subsecond(fields);

  gregorian::ymd ymd{year, month, day};
  gregorian::ymdh ymdh{year, month, day, hour};
  gregorian::ymdhm ymdhm{year, month, day, hour, minute};
  gregorian::ymdhms ymdhms{year, month, day, hour, minute, second};
  gregorian::ymdhmss<std::chrono::milliseconds> ymdhmss1{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::microseconds> ymdhmss2{year, month, day, hour, minute, second, subsecond};
  gregorian::ymdhmss<std::chrono::nanoseconds> ymdhmss3{year, month, day, hour, minute, second, subsecond};

  switch (parse_precision2(precision)) {
  case precision2::day: return as_sys_time_year_month_day_impl<duration::days>(ymd);
  case precision2::hour: return as_sys_time_year_month_day_impl<duration::hours>(ymdh);
  case precision2::minute: return as_sys_time_year_month_day_impl<duration::minutes>(ymdhm);
  case precision2::second: return as_sys_time_year_month_day_impl<duration::seconds>(ymdhms);
  case precision2::millisecond: return as_sys_time_year_month_day_impl<duration::milliseconds>(ymdhmss1);
  case precision2::microsecond: return as_sys_time_year_month_day_impl<duration::microseconds>(ymdhmss2);
  case precision2::nanosecond: return as_sys_time_year_month_day_impl<duration::nanoseconds>(ymdhmss3);
  default: {
    std::string precision_string = precision[0];
    std::string message = "Can't convert to a time point from a calendar with '" + precision_string + "' precision.";
    clock_abort(message.c_str());
  }
  }
}

// -----------------------------------------------------------------------------

template <class Calendar, class ClockDuration>
cpp11::writable::list
as_year_month_day_from_time_point_impl(const ClockDuration& x) {
  const r_ssize size = x.size();
  Calendar out(size);
  using Duration = typename ClockDuration::duration;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
    } else {
      Duration elt = x[i];
      date::sys_time<Duration> elt_st{elt};
      out.assign_sys_time(elt_st, i);
    }
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
as_year_month_day_from_time_point_cpp(cpp11::list_of<cpp11::integers> fields,
                               const cpp11::strings& precision) {
  using namespace rclock;

  cpp11::integers ticks = get_ticks(fields);
  cpp11::integers ticks_of_day = get_ticks_of_day(fields);
  cpp11::integers ticks_of_second = get_ticks_of_second(fields);

  duration::days dd{ticks};
  duration::hours dh{ticks, ticks_of_day};
  duration::minutes dmin{ticks, ticks_of_day};
  duration::seconds ds{ticks, ticks_of_day};
  duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision2(precision)) {
  case precision2::day: return as_year_month_day_from_time_point_impl<gregorian::ymd>(dd);
  case precision2::hour: return as_year_month_day_from_time_point_impl<gregorian::ymdh>(dh);
  case precision2::minute: return as_year_month_day_from_time_point_impl<gregorian::ymdhm>(dmin);
  case precision2::second: return as_year_month_day_from_time_point_impl<gregorian::ymdhms>(ds);
  case precision2::millisecond: return as_year_month_day_from_time_point_impl<gregorian::ymdhmss<std::chrono::milliseconds>>(dmilli);
  case precision2::microsecond: return as_year_month_day_from_time_point_impl<gregorian::ymdhmss<std::chrono::microseconds>>(dmicro);
  case precision2::nanosecond: return as_year_month_day_from_time_point_impl<gregorian::ymdhmss<std::chrono::nanoseconds>>(dnano);
  default: clock_abort("Internal error: Invalid precision.");
  }
}
