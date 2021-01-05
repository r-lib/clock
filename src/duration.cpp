#include "clock.h"
#include "utils.h"
#include "duration.h"
#include "enums.h"
#include <sstream>

// -----------------------------------------------------------------------------

/*
 * This is operator<< for durations in `date.h`, but without the unit
 */
template <class CharT, class Traits, class Rep, class Period>
inline
std::basic_ostream<CharT, Traits>&
duration_stream(std::basic_ostream<CharT, Traits>& os,
                const std::chrono::duration<Rep, Period>& d)
{
  return os << date::detail::make_string<CharT, Traits>::from(d.count());
}

// -----------------------------------------------------------------------------

template <typename ClockDuration>
cpp11::writable::strings
format_duration_impl(const ClockDuration& cd) {
  r_ssize size = cd.size();
  std::ostringstream stream;
  cpp11::writable::strings out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (cd.is_na(i)) {
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    typename ClockDuration::duration duration = cd[i];

    stream.str(std::string());
    stream.clear();

    duration_stream(stream, duration);
    std::string string = stream.str();

    SEXP r_string = Rf_mkCharLenCE(string.c_str(), string.size(), CE_UTF8);
    SET_STRING_ELT(out, i, r_string);
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::strings format_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                             const cpp11::strings& precision) {
  switch (parse_precision2(precision)) {
  case precision2::year: return format_duration_impl(rclock::duration::duration1<date::years>(fields[0]));
  case precision2::quarter: return format_duration_impl(rclock::duration::duration1<quarterly::quarters>(fields[0]));
  case precision2::month: return format_duration_impl(rclock::duration::duration1<date::months>(fields[0]));
  case precision2::week: return format_duration_impl(rclock::duration::duration1<date::weeks>(fields[0]));
  case precision2::day: return format_duration_impl(rclock::duration::duration1<date::days>(fields[0]));
  case precision2::hour: return format_duration_impl(rclock::duration::duration2<std::chrono::hours>(fields[0], fields[1]));
  case precision2::minute: return format_duration_impl(rclock::duration::duration2<std::chrono::minutes>(fields[0], fields[1]));
  case precision2::second: return format_duration_impl(rclock::duration::duration2<std::chrono::seconds>(fields[0], fields[1]));
  case precision2::millisecond: return format_duration_impl(rclock::duration::duration3<std::chrono::milliseconds>(fields[0], fields[1], fields[2]));
  case precision2::microsecond: return format_duration_impl(rclock::duration::duration3<std::chrono::microseconds>(fields[0], fields[1], fields[2]));
  case precision2::nanosecond: return format_duration_impl(rclock::duration::duration3<std::chrono::nanoseconds>(fields[0], fields[1], fields[2]));
  default: clock_abort("Internal error: Should never be called.");
  }
}

// -----------------------------------------------------------------------------

template <typename ClockDuration>
inline
cpp11::writable::list_of<cpp11::writable::integers>
duration_helper_impl(const cpp11::integers& n) {
  r_ssize size = n.size();
  ClockDuration cd(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_n = n[i];

    if (elt_n == r_int_na) {
      cd.assign_na(i);
      continue;
    }

    typename ClockDuration::duration duration{elt_n};
    cd.assign(duration, i);
  }

  return cd.to_list();
}

[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
duration_helper_cpp(const cpp11::integers& n,
                    const cpp11::strings& precision) {
  switch (parse_precision2(precision)) {
  case precision2::year: return duration_helper_impl<rclock::duration::duration1<date::years>>(n);
  case precision2::quarter: return duration_helper_impl<rclock::duration::duration1<quarterly::quarters>>(n);
  case precision2::month: return duration_helper_impl<rclock::duration::duration1<date::months>>(n);
  case precision2::week: return duration_helper_impl<rclock::duration::duration1<date::weeks>>(n);
  case precision2::day: return duration_helper_impl<rclock::duration::duration1<date::days>>(n);
  case precision2::hour: return duration_helper_impl<rclock::duration::duration2<std::chrono::hours>>(n);
  case precision2::minute: return duration_helper_impl<rclock::duration::duration2<std::chrono::minutes>>(n);
  case precision2::second: return duration_helper_impl<rclock::duration::duration2<std::chrono::seconds>>(n);
  case precision2::millisecond: return duration_helper_impl<rclock::duration::duration3<std::chrono::milliseconds>>(n);
  case precision2::microsecond: return duration_helper_impl<rclock::duration::duration3<std::chrono::microseconds>>(n);
  case precision2::nanosecond: return duration_helper_impl<rclock::duration::duration3<std::chrono::nanoseconds>>(n);
  default: clock_abort("Internal error: Should never be called.");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDurationTo, class ClockDurationFrom>
cpp11::writable::list
duration_cast_impl(const ClockDurationFrom& cd) {
  using DurationFrom = typename ClockDurationFrom::duration;
  using DurationTo = typename ClockDurationTo::duration;

  if (std::is_same<DurationFrom, DurationTo>::value) {
    return(cd.to_list());
  }

  r_ssize size = cd.size();
  ClockDurationTo out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (cd.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    const DurationFrom from = cd[i];
    const DurationTo to = std::chrono::duration_cast<DurationTo>(from);

    out.assign(to, i);
  }

  return out.to_list();
}

template <class ClockDurationFrom>
inline
cpp11::writable::list
duration_cast_switch2(const ClockDurationFrom& cd,
                      const enum precision2 precision_to_val) {
  switch (precision_to_val) {
  case precision2::year: return duration_cast_impl<rclock::duration::duration1<date::years>>(cd);
  case precision2::quarter: return duration_cast_impl<rclock::duration::duration1<quarterly::quarters>>(cd);
  case precision2::month: return duration_cast_impl<rclock::duration::duration1<date::months>>(cd);
  case precision2::week: return duration_cast_impl<rclock::duration::duration1<date::weeks>>(cd);
  case precision2::day: return duration_cast_impl<rclock::duration::duration1<date::days>>(cd);
  case precision2::hour: return duration_cast_impl<rclock::duration::duration2<std::chrono::hours>>(cd);
  case precision2::minute: return duration_cast_impl<rclock::duration::duration2<std::chrono::minutes>>(cd);
  case precision2::second: return duration_cast_impl<rclock::duration::duration2<std::chrono::seconds>>(cd);
  case precision2::millisecond: return duration_cast_impl<rclock::duration::duration3<std::chrono::milliseconds>>(cd);
  case precision2::microsecond: return duration_cast_impl<rclock::duration::duration3<std::chrono::microseconds>>(cd);
  case precision2::nanosecond: return duration_cast_impl<rclock::duration::duration3<std::chrono::nanoseconds>>(cd);
  }
}

inline
cpp11::writable::list
duration_cast_switch(cpp11::list_of<cpp11::integers>& fields,
                     const enum precision2 precision_from_val,
                     const enum precision2 precision_to_val) {
  switch (precision_from_val) {
  case precision2::year: return duration_cast_switch2(rclock::duration::duration1<date::years>(fields[0]), precision_to_val);
  case precision2::quarter: return duration_cast_switch2(rclock::duration::duration1<quarterly::quarters>(fields[0]), precision_to_val);
  case precision2::month: return duration_cast_switch2(rclock::duration::duration1<date::months>(fields[0]), precision_to_val);
  case precision2::week: return duration_cast_switch2(rclock::duration::duration1<date::weeks>(fields[0]), precision_to_val);
  case precision2::day: return duration_cast_switch2(rclock::duration::duration1<date::days>(fields[0]), precision_to_val);
  case precision2::hour: return duration_cast_switch2(rclock::duration::duration2<std::chrono::hours>(fields[0], fields[1]), precision_to_val);
  case precision2::minute: return duration_cast_switch2(rclock::duration::duration2<std::chrono::minutes>(fields[0], fields[1]), precision_to_val);
  case precision2::second: return duration_cast_switch2(rclock::duration::duration2<std::chrono::seconds>(fields[0], fields[1]), precision_to_val);
  case precision2::millisecond: return duration_cast_switch2(rclock::duration::duration3<std::chrono::milliseconds>(fields[0], fields[1], fields[2]), precision_to_val);
  case precision2::microsecond: return duration_cast_switch2(rclock::duration::duration3<std::chrono::microseconds>(fields[0], fields[1], fields[2]), precision_to_val);
  case precision2::nanosecond: return duration_cast_switch2(rclock::duration::duration3<std::chrono::nanoseconds>(fields[0], fields[1], fields[2]), precision_to_val);
  }
}

[[cpp11::register]]
cpp11::writable::list
duration_cast_cpp(cpp11::list_of<cpp11::integers> fields,
                  const cpp11::strings& precision_from,
                  const cpp11::strings& precision_to) {
  const enum precision2 precision_from_val = parse_precision2(precision_from);
  const enum precision2 precision_to_val = parse_precision2(precision_to);

  return duration_cast_switch(
    fields,
    precision_from_val,
    precision_to_val
  );
}

// -----------------------------------------------------------------------------

enum class rounding {
  round,
  floor,
  ceil,
};

template <class ClockDurationTo, class ClockDurationFrom>
cpp11::writable::list
duration_rounding_impl(const ClockDurationFrom& cd, const enum rounding& type) {
  using DurationFrom = typename ClockDurationFrom::duration;
  using DurationTo = typename ClockDurationTo::duration;

  if (std::is_same<DurationFrom, DurationTo>::value) {
    return(cd.to_list());
  }

  r_ssize size = cd.size();
  ClockDurationTo out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (cd.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    const DurationFrom from = cd[i];

    switch (type) {
    case rounding::round: out.assign(date::round<DurationTo>(from), i); break;
    case rounding::floor: out.assign(date::floor<DurationTo>(from), i); break;
    case rounding::ceil: out.assign(date::ceil<DurationTo>(from), i); break;
    }
  }

  return out.to_list();
}

inline
cpp11::writable::list
duration_rounding_switch(cpp11::list_of<cpp11::integers>& fields,
                         const enum precision2& precision_from_val,
                         const enum precision2& precision_to_val,
                         const enum rounding& type) {
  using namespace rclock;

  switch (precision_from_val) {
  case precision2::year: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::years(fields[0]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision2::quarter: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::quarters(fields[0]), type);
    case precision2::quarter: return duration_rounding_impl<duration::quarters>(duration::quarters(fields[0]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision2::month: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::months(fields[0]), type);
    case precision2::quarter: return duration_rounding_impl<duration::quarters>(duration::months(fields[0]), type);
    case precision2::month: return duration_rounding_impl<duration::months>(duration::months(fields[0]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision2::week: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::weeks(fields[0]), type);
    case precision2::quarter: return duration_rounding_impl<duration::quarters>(duration::weeks(fields[0]), type);
    case precision2::month: return duration_rounding_impl<duration::months>(duration::weeks(fields[0]), type);
    case precision2::week: return duration_rounding_impl<duration::weeks>(duration::weeks(fields[0]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision2::day: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::days(fields[0]), type);
    case precision2::quarter: return duration_rounding_impl<duration::quarters>(duration::days(fields[0]), type);
    case precision2::month: return duration_rounding_impl<duration::months>(duration::days(fields[0]), type);
    case precision2::week: return duration_rounding_impl<duration::weeks>(duration::days(fields[0]), type);
    case precision2::day: return duration_rounding_impl<duration::days>(duration::days(fields[0]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision2::hour: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::hours(fields[0], fields[1]), type);
    case precision2::quarter: return duration_rounding_impl<duration::quarters>(duration::hours(fields[0], fields[1]), type);
    case precision2::month: return duration_rounding_impl<duration::months>(duration::hours(fields[0], fields[1]), type);
    case precision2::week: return duration_rounding_impl<duration::weeks>(duration::hours(fields[0], fields[1]), type);
    case precision2::day: return duration_rounding_impl<duration::days>(duration::hours(fields[0], fields[1]), type);
    case precision2::hour: return duration_rounding_impl<duration::hours>(duration::hours(fields[0], fields[1]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision2::minute: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::minutes(fields[0], fields[1]), type);
    case precision2::quarter: return duration_rounding_impl<duration::quarters>(duration::minutes(fields[0], fields[1]), type);
    case precision2::month: return duration_rounding_impl<duration::months>(duration::minutes(fields[0], fields[1]), type);
    case precision2::week: return duration_rounding_impl<duration::weeks>(duration::minutes(fields[0], fields[1]), type);
    case precision2::day: return duration_rounding_impl<duration::days>(duration::minutes(fields[0], fields[1]), type);
    case precision2::hour: return duration_rounding_impl<duration::hours>(duration::minutes(fields[0], fields[1]), type);
    case precision2::minute: return duration_rounding_impl<duration::minutes>(duration::minutes(fields[0], fields[1]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision2::second: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::seconds(fields[0], fields[1]), type);
    case precision2::quarter: return duration_rounding_impl<duration::quarters>(duration::seconds(fields[0], fields[1]), type);
    case precision2::month: return duration_rounding_impl<duration::months>(duration::seconds(fields[0], fields[1]), type);
    case precision2::week: return duration_rounding_impl<duration::weeks>(duration::seconds(fields[0], fields[1]), type);
    case precision2::day: return duration_rounding_impl<duration::days>(duration::seconds(fields[0], fields[1]), type);
    case precision2::hour: return duration_rounding_impl<duration::hours>(duration::seconds(fields[0], fields[1]), type);
    case precision2::minute: return duration_rounding_impl<duration::minutes>(duration::seconds(fields[0], fields[1]), type);
    case precision2::second: return duration_rounding_impl<duration::seconds>(duration::seconds(fields[0], fields[1]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision2::millisecond: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::milliseconds(fields[0], fields[1], fields[2]), type);
    case precision2::quarter: return duration_rounding_impl<duration::quarters>(duration::milliseconds(fields[0], fields[1], fields[2]), type);
    case precision2::month: return duration_rounding_impl<duration::months>(duration::milliseconds(fields[0], fields[1], fields[2]), type);
    case precision2::week: return duration_rounding_impl<duration::weeks>(duration::milliseconds(fields[0], fields[1], fields[2]), type);
    case precision2::day: return duration_rounding_impl<duration::days>(duration::milliseconds(fields[0], fields[1], fields[2]), type);
    case precision2::hour: return duration_rounding_impl<duration::hours>(duration::milliseconds(fields[0], fields[1], fields[2]), type);
    case precision2::minute: return duration_rounding_impl<duration::minutes>(duration::milliseconds(fields[0], fields[1], fields[2]), type);
    case precision2::second: return duration_rounding_impl<duration::seconds>(duration::milliseconds(fields[0], fields[1], fields[2]), type);
    case precision2::millisecond: return duration_rounding_impl<duration::milliseconds>(duration::milliseconds(fields[0], fields[1], fields[2]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision2::microsecond: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::microseconds(fields[0], fields[1], fields[2]), type);
    case precision2::quarter: return duration_rounding_impl<duration::quarters>(duration::microseconds(fields[0], fields[1], fields[2]), type);
    case precision2::month: return duration_rounding_impl<duration::months>(duration::microseconds(fields[0], fields[1], fields[2]), type);
    case precision2::week: return duration_rounding_impl<duration::weeks>(duration::microseconds(fields[0], fields[1], fields[2]), type);
    case precision2::day: return duration_rounding_impl<duration::days>(duration::microseconds(fields[0], fields[1], fields[2]), type);
    case precision2::hour: return duration_rounding_impl<duration::hours>(duration::microseconds(fields[0], fields[1], fields[2]), type);
    case precision2::minute: return duration_rounding_impl<duration::minutes>(duration::microseconds(fields[0], fields[1], fields[2]), type);
    case precision2::second: return duration_rounding_impl<duration::seconds>(duration::microseconds(fields[0], fields[1], fields[2]), type);
    case precision2::millisecond: return duration_rounding_impl<duration::milliseconds>(duration::microseconds(fields[0], fields[1], fields[2]), type);
    case precision2::microsecond: return duration_rounding_impl<duration::microseconds>(duration::microseconds(fields[0], fields[1], fields[2]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision2::nanosecond: {
    switch (precision_to_val) {
    case precision2::year: return duration_rounding_impl<duration::years>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    case precision2::quarter: return duration_rounding_impl<duration::quarters>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    case precision2::month: return duration_rounding_impl<duration::months>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    case precision2::week: return duration_rounding_impl<duration::weeks>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    case precision2::day: return duration_rounding_impl<duration::days>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    case precision2::hour: return duration_rounding_impl<duration::hours>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    case precision2::minute: return duration_rounding_impl<duration::minutes>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    case precision2::second: return duration_rounding_impl<duration::seconds>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    case precision2::millisecond: return duration_rounding_impl<duration::milliseconds>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    case precision2::microsecond: return duration_rounding_impl<duration::microseconds>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    case precision2::nanosecond: return duration_rounding_impl<duration::nanoseconds>(duration::nanoseconds(fields[0], fields[1], fields[2]), type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  }
}

[[cpp11::register]]
cpp11::writable::list
duration_floor_cpp(cpp11::list_of<cpp11::integers> fields,
                   const cpp11::strings& precision_from,
                   const cpp11::strings& precision_to) {
  const enum precision2 precision_from_val = parse_precision2(precision_from);
  const enum precision2 precision_to_val = parse_precision2(precision_to);

  return duration_rounding_switch(
    fields,
    precision_from_val,
    precision_to_val,
    rounding::floor
  );
}

[[cpp11::register]]
cpp11::writable::list
duration_ceil_cpp(cpp11::list_of<cpp11::integers> fields,
                  const cpp11::strings& precision_from,
                  const cpp11::strings& precision_to) {
  const enum precision2 precision_from_val = parse_precision2(precision_from);
  const enum precision2 precision_to_val = parse_precision2(precision_to);

  return duration_rounding_switch(
    fields,
    precision_from_val,
    precision_to_val,
    rounding::ceil
  );
}

[[cpp11::register]]
cpp11::writable::list
duration_round_cpp(cpp11::list_of<cpp11::integers> fields,
                   const cpp11::strings& precision_from,
                   const cpp11::strings& precision_to) {
  const enum precision2 precision_from_val = parse_precision2(precision_from);
  const enum precision2 precision_to_val = parse_precision2(precision_to);

  return duration_rounding_switch(
    fields,
    precision_from_val,
    precision_to_val,
    rounding::round
  );
}
