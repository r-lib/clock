#include "gregorian-year-month-day.h"
#include "check.h"
#include "enums.h"

[[cpp11::register]]
cpp11::writable::list
collect_year_month_day_fields(cpp11::list_of<cpp11::integers> fields, bool last) {
  const cpp11::integers& year = fields[0];
  const cpp11::integers& month = fields[1];
  const cpp11::integers& day = fields[2];

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
                          cpp11::strings precision) {
  switch (parse_precision2(precision)) {
  case precision2::year: return format_year_month_day_impl(rclock::gregorian::y{fields[0]});
  case precision2::month: return format_year_month_day_impl(rclock::gregorian::ym{fields[0], fields[1]});
  case precision2::day: return format_year_month_day_impl(rclock::gregorian::ymd{fields[0], fields[1], fields[2]});
  case precision2::hour: return format_year_month_day_impl(rclock::gregorian::ymdh{fields[0], fields[1], fields[2], fields[3]});
  case precision2::minute: return format_year_month_day_impl(rclock::gregorian::ymdhm{fields[0], fields[1], fields[2], fields[3], fields[4]});
  case precision2::second: return format_year_month_day_impl(rclock::gregorian::ymdhms{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]});
  case precision2::millisecond: return format_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::milliseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  case precision2::microsecond: return format_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::microseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  case precision2::nanosecond: return format_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::nanoseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  default: clock_abort("Internal error: Unknown precision.");
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
invalid_detect_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields, cpp11::strings precision) {
  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_detect_year_month_day_impl(rclock::gregorian::y{fields[0]});
  case precision2::month: return invalid_detect_year_month_day_impl(rclock::gregorian::ym{fields[0], fields[1]});
  case precision2::day: return invalid_detect_year_month_day_impl(rclock::gregorian::ymd{fields[0], fields[1], fields[2]});
  case precision2::hour: return invalid_detect_year_month_day_impl(rclock::gregorian::ymdh{fields[0], fields[1], fields[2], fields[3]});
  case precision2::minute: return invalid_detect_year_month_day_impl(rclock::gregorian::ymdhm{fields[0], fields[1], fields[2], fields[3], fields[4]});
  case precision2::second: return invalid_detect_year_month_day_impl(rclock::gregorian::ymdhms{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]});
  case precision2::millisecond: return invalid_detect_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::milliseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  case precision2::microsecond: return invalid_detect_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::microseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  case precision2::nanosecond: return invalid_detect_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::nanoseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  default: clock_abort("Internal error: Unknown precision.");
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
  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_any_year_month_day_impl(rclock::gregorian::y{fields[0]});
  case precision2::month: return invalid_any_year_month_day_impl(rclock::gregorian::ym{fields[0], fields[1]});
  case precision2::day: return invalid_any_year_month_day_impl(rclock::gregorian::ymd{fields[0], fields[1], fields[2]});
  case precision2::hour: return invalid_any_year_month_day_impl(rclock::gregorian::ymdh{fields[0], fields[1], fields[2], fields[3]});
  case precision2::minute: return invalid_any_year_month_day_impl(rclock::gregorian::ymdhm{fields[0], fields[1], fields[2], fields[3], fields[4]});
  case precision2::second: return invalid_any_year_month_day_impl(rclock::gregorian::ymdhms{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]});
  case precision2::millisecond: return invalid_any_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::milliseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  case precision2::microsecond: return invalid_any_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::microseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  case precision2::nanosecond: return invalid_any_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::nanoseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  default: clock_abort("Internal error: Unknown precision.");
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
invalid_count_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields, cpp11::strings precision) {
  switch (parse_precision2(precision)) {
  case precision2::year: return invalid_count_year_month_day_impl(rclock::gregorian::y{fields[0]});
  case precision2::month: return invalid_count_year_month_day_impl(rclock::gregorian::ym{fields[0], fields[1]});
  case precision2::day: return invalid_count_year_month_day_impl(rclock::gregorian::ymd{fields[0], fields[1], fields[2]});
  case precision2::hour: return invalid_count_year_month_day_impl(rclock::gregorian::ymdh{fields[0], fields[1], fields[2], fields[3]});
  case precision2::minute: return invalid_count_year_month_day_impl(rclock::gregorian::ymdhm{fields[0], fields[1], fields[2], fields[3], fields[4]});
  case precision2::second: return invalid_count_year_month_day_impl(rclock::gregorian::ymdhms{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]});
  case precision2::millisecond: return invalid_count_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::milliseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  case precision2::microsecond: return invalid_count_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::microseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  case precision2::nanosecond: return invalid_count_year_month_day_impl(rclock::gregorian::ymdhmss<std::chrono::nanoseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  default: clock_abort("Internal error: Unknown precision.");
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
  const enum invalid invalid_val = parse_invalid(invalid);

  switch (parse_precision2(precision)) {
  case precision2::year: {
    rclock::gregorian::y x{fields[0]};
    return invalid_resolve_year_month_day_impl(x, invalid_val);
  }
  case precision2::month: {
    rclock::gregorian::ym x{fields[0], fields[1]};
    return invalid_resolve_year_month_day_impl(x, invalid_val);
  }
  case precision2::day: {
    rclock::gregorian::ymd x{fields[0], fields[1], fields[2]};
    return invalid_resolve_year_month_day_impl(x, invalid_val);
  }
  case precision2::hour: {
    rclock::gregorian::ymdh x{fields[0], fields[1], fields[2], fields[3]};
    return invalid_resolve_year_month_day_impl(x, invalid_val);
  }
  case precision2::minute: {
    rclock::gregorian::ymdhm x{fields[0], fields[1], fields[2], fields[3], fields[4]};
    return invalid_resolve_year_month_day_impl(x, invalid_val);
  }
  case precision2::second: {
    rclock::gregorian::ymdhms x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]};
    return invalid_resolve_year_month_day_impl(x, invalid_val);
  }
  case precision2::millisecond: {
    rclock::gregorian::ymdhmss<std::chrono::milliseconds> x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]};
    return invalid_resolve_year_month_day_impl(x, invalid_val);
  }
  case precision2::microsecond: {
    rclock::gregorian::ymdhmss<std::chrono::microseconds> x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]};
    return invalid_resolve_year_month_day_impl(x, invalid_val);
  }
  case precision2::nanosecond: {
    rclock::gregorian::ymdhmss<std::chrono::nanoseconds> x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]};
    return invalid_resolve_year_month_day_impl(x, invalid_val);
  }
  default: clock_abort("Internal error: Unknown precision.");
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
  rclock::integers value2(value);

  switch (parse_precision2(precision_fields)) {
  case precision2::year: {
    rclock::gregorian::y x{fields[0]};
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(x, value2);
    case precision2::month: return set_field_calendar<date::month>(x, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::month: {
    rclock::gregorian::ym x{fields[0], fields[1]};
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(x, value2);
    case precision2::month: return set_field_calendar<date::month>(x, value2);
    case precision2::day: set_field_calendar<date::day>(x, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::day: {
    rclock::gregorian::ymd x{fields[0], fields[1], fields[2]};
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(x, value2);
    case precision2::month: return set_field_calendar<date::month>(x, value2);
    case precision2::day: return set_field_calendar<date::day>(x, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(x, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::hour: {
    rclock::gregorian::ymdh x{fields[0], fields[1], fields[2], fields[3]};
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(x, value2);
    case precision2::month: return set_field_calendar<date::month>(x, value2);
    case precision2::day: return set_field_calendar<date::day>(x, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(x, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(x, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::minute: {
    rclock::gregorian::ymdhm x{fields[0], fields[1], fields[2], fields[3], fields[4]};
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(x, value2);
    case precision2::month: return set_field_calendar<date::month>(x, value2);
    case precision2::day: return set_field_calendar<date::day>(x, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(x, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(x, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(x, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::second: {
    rclock::gregorian::ymdhms x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]};
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(x, value2);
    case precision2::month: return set_field_calendar<date::month>(x, value2);
    case precision2::day: return set_field_calendar<date::day>(x, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(x, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(x, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(x, value2);
    case precision2::millisecond: return set_field_calendar<std::chrono::milliseconds>(x, value2);
    case precision2::microsecond: return set_field_calendar<std::chrono::microseconds>(x, value2);
    case precision2::nanosecond: return set_field_calendar<std::chrono::nanoseconds>(x, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::millisecond: {
    rclock::gregorian::ymdhmss<std::chrono::milliseconds> x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]};
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(x, value2);
    case precision2::month: return set_field_calendar<date::month>(x, value2);
    case precision2::day: return set_field_calendar<date::day>(x, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(x, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(x, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(x, value2);
    case precision2::millisecond: return set_field_calendar<std::chrono::milliseconds>(x, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::microsecond: {
    rclock::gregorian::ymdhmss<std::chrono::microseconds> x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]};
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(x, value2);
    case precision2::month: return set_field_calendar<date::month>(x, value2);
    case precision2::day: return set_field_calendar<date::day>(x, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(x, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(x, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(x, value2);
    case precision2::microsecond: return set_field_calendar<std::chrono::microseconds>(x, value2);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::nanosecond: {
    rclock::gregorian::ymdhmss<std::chrono::nanoseconds> x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]};
    switch (parse_precision2(precision_value)) {
    case precision2::year: return set_field_calendar<date::year>(x, value2);
    case precision2::month: return set_field_calendar<date::month>(x, value2);
    case precision2::day: return set_field_calendar<date::day>(x, value2);
    case precision2::hour: return set_field_calendar<std::chrono::hours>(x, value2);
    case precision2::minute: return set_field_calendar<std::chrono::minutes>(x, value2);
    case precision2::second: return set_field_calendar<std::chrono::seconds>(x, value2);
    case precision2::nanosecond: return set_field_calendar<std::chrono::nanoseconds>(x, value2);
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
  switch (parse_precision2(precision_fields)) {
  case precision2::month: return set_field_year_month_day_last_impl(rclock::gregorian::ym{fields[0], fields[1]});
  case precision2::day: return set_field_year_month_day_last_impl(rclock::gregorian::ymd{fields[0], fields[1], fields[2]});
  case precision2::hour: return set_field_year_month_day_last_impl(rclock::gregorian::ymdh{fields[0], fields[1], fields[2], fields[3]});
  case precision2::minute: return set_field_year_month_day_last_impl(rclock::gregorian::ymdhm{fields[0], fields[1], fields[2], fields[3], fields[4]});
  case precision2::second: return set_field_year_month_day_last_impl(rclock::gregorian::ymdhms{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]});
  case precision2::millisecond: return set_field_year_month_day_last_impl(rclock::gregorian::ymdhmss<std::chrono::milliseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  case precision2::microsecond: return set_field_year_month_day_last_impl(rclock::gregorian::ymdhmss<std::chrono::microseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  case precision2::nanosecond: return set_field_year_month_day_last_impl(rclock::gregorian::ymdhmss<std::chrono::nanoseconds>{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]});
  default: clock_abort("Internal error: Unknown precision.");
  }
}

// -----------------------------------------------------------------------------

template <typename Duration, class Calendar>
cpp11::writable::list
add_field_year_month_day(Calendar& x, const cpp11::integers& n) {
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      continue;
    }

    int elt_n = n[i];

    if (elt_n == r_int_na) {
      x.assign_na(i);
      continue;
    }

    x.add(Duration{elt_n}, i);
  }

  return x.to_list();
}

[[cpp11::register]]
cpp11::writable::list
add_field_year_month_day_cpp(cpp11::list_of<cpp11::integers> fields,
                             const cpp11::integers& n,
                             const cpp11::strings& precision_fields,
                             const cpp11::strings& precision_n) {
  switch (parse_precision2(precision_fields)) {
  case precision2::year: {
    rclock::gregorian::y x{fields[0]};
    switch (parse_precision2(precision_n)) {
    case precision2::year: return add_field_year_month_day<date::years>(x, n);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::month: {
    rclock::gregorian::ym x{fields[0], fields[1]};
    switch (parse_precision2(precision_n)) {
    case precision2::year: return add_field_year_month_day<date::years>(x, n);
    case precision2::month: return add_field_year_month_day<date::months>(x, n);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::day: {
    rclock::gregorian::ymd x{fields[0], fields[1], fields[2]};
    switch (parse_precision2(precision_n)) {
    case precision2::year: return add_field_year_month_day<date::years>(x, n);
    case precision2::month: return add_field_year_month_day<date::months>(x, n);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::hour: {
    rclock::gregorian::ymdh x{fields[0], fields[1], fields[2], fields[3]};
    switch (parse_precision2(precision_n)) {
    case precision2::year: return add_field_year_month_day<date::years>(x, n);
    case precision2::month: return add_field_year_month_day<date::months>(x, n);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::minute: {
    rclock::gregorian::ymdhm x{fields[0], fields[1], fields[2], fields[3], fields[4]};
    switch (parse_precision2(precision_n)) {
    case precision2::year: return add_field_year_month_day<date::years>(x, n);
    case precision2::month: return add_field_year_month_day<date::months>(x, n);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::second: {
    rclock::gregorian::ymdhms x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]};
    switch (parse_precision2(precision_n)) {
    case precision2::year: return add_field_year_month_day<date::years>(x, n);
    case precision2::month: return add_field_year_month_day<date::months>(x, n);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::millisecond: {
    rclock::gregorian::ymdhmss<std::chrono::milliseconds> x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]};
    switch (parse_precision2(precision_n)) {
    case precision2::year: return add_field_year_month_day<date::years>(x, n);
    case precision2::month: return add_field_year_month_day<date::months>(x, n);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::microsecond: {
    rclock::gregorian::ymdhmss<std::chrono::microseconds> x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]};
    switch (parse_precision2(precision_n)) {
    case precision2::year: return add_field_year_month_day<date::years>(x, n);
    case precision2::month: return add_field_year_month_day<date::months>(x, n);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  case precision2::nanosecond: {
    rclock::gregorian::ymdhmss<std::chrono::nanoseconds> x{fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]};
    switch (parse_precision2(precision_n)) {
    case precision2::year: return add_field_year_month_day<date::years>(x, n);
    case precision2::month: return add_field_year_month_day<date::months>(x, n);
    default: clock_abort("Internal error: Invalid precision.");
    }
  }
  default: clock_abort("Internal error: Invalid precision.");
  }
}
