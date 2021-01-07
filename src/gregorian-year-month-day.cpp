#include "gregorian-year-month-day.h"
#include "duration.h"
#include "check.h"
#include "enums.h"
#include "get.h"

[[cpp11::register]]
cpp11::writable::list
collect_year_month_day_fields(cpp11::list_of<cpp11::integers> fields, bool last) {
  const cpp11::integers& year = fields["year"];
  const cpp11::integers& month = fields["month"];
  const cpp11::integers& day = fields["day"];

  rclock::gregorian::ymd x(year, month, day);
  r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    const int elt_year = year[i];
    const int elt_month = month[i];
    const int elt_day = day[i];

    if (elt_year == r_int_na ||
        elt_month == r_int_na ||
        elt_day == r_int_na) {
      x.assign_na(i);
      continue;
    }

    check_range_year(elt_year, "year");
    check_range_month(elt_month, "month");

    if (last) {
      date::year_month_day_last ymdl = x.to_year_month(i) / date::last;
      x.assign_day(ymdl.day(), i);
    } else {
      check_range_day(elt_day, "day");
    }
  }

  return x.to_list();
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
    case precision2::year: return set_field_calendar<date::year>(y, value2);
    case precision2::month: return set_field_calendar<date::month>(y, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::month: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(ym, value2);
    case precision2::month: return set_field_calendar<date::month>(ym, value2);
    case precision2::day: return set_field_calendar<date::day>(ym, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::day: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(ymd, value2);
    case precision2::month: return set_field_calendar<date::month>(ymd, value2);
    case precision2::day: return set_field_calendar<date::day>(ymd, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymd, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::hour: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(ymdh, value2);
    case precision2::month: return set_field_calendar<date::month>(ymdh, value2);
    case precision2::day: return set_field_calendar<date::day>(ymdh, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymdh, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(ymdh, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::minute: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(ymdhm, value2);
    case precision2::month: return set_field_calendar<date::month>(ymdhm, value2);
    case precision2::day: return set_field_calendar<date::day>(ymdhm, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymdhm, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(ymdhm, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(ymdhm, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::second: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(ymdhms, value2);
    case precision2::month: return set_field_calendar<date::month>(ymdhms, value2);
    case precision2::day: return set_field_calendar<date::day>(ymdhms, value2);
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
    case precision2::year: return set_field_calendar<date::year>(ymdhmss1, value2);
    case precision2::month: return set_field_calendar<date::month>(ymdhmss1, value2);
    case precision2::day: return set_field_calendar<date::day>(ymdhmss1, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymdhmss1, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(ymdhmss1, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(ymdhmss1, value2);
    case precision2::millisecond: return set_field_calendar<std::chrono::milliseconds>(ymdhmss1, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::microsecond: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(ymdhmss2, value2);
    case precision2::month: return set_field_calendar<date::month>(ymdhmss2, value2);
    case precision2::day: return set_field_calendar<date::day>(ymdhmss2, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(ymdhmss2, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(ymdhmss2, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(ymdhmss2, value2);
    case precision2::microsecond: return set_field_calendar<std::chrono::microseconds>(ymdhmss2, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::nanosecond: {
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(ymdhmss3, value2);
    case precision2::month: return set_field_calendar<date::month>(ymdhmss3, value2);
    case precision2::day: return set_field_calendar<date::day>(ymdhmss3, value2);
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
