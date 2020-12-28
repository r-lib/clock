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

template <typename Duration>
cpp11::writable::strings
format_duration_impl(const rclock::duration<Duration>& cd) {
  r_ssize size = cd.size();
  std::ostringstream stream;
  cpp11::writable::strings out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (cd.is_na(i)) {
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    Duration duration = cd[i];

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
cpp11::writable::strings format_duration_cpp(const cpp11::integers& ticks,
                                             const cpp11::integers& ticks_of_day,
                                             const cpp11::integers& ticks_of_second,
                                             const cpp11::strings& precision) {
  switch (parse_precision2(precision)) {
  case precision2::year: return format_duration_impl(rclock::duration<date::years>(ticks, ticks_of_day, ticks_of_second));
  case precision2::quarter: return format_duration_impl(rclock::duration<quarterly::quarters>(ticks, ticks_of_day, ticks_of_second));
  case precision2::month: return format_duration_impl(rclock::duration<date::months>(ticks, ticks_of_day, ticks_of_second));
  case precision2::week: return format_duration_impl(rclock::duration<date::weeks>(ticks, ticks_of_day, ticks_of_second));
  case precision2::day: return format_duration_impl(rclock::duration<date::days>(ticks, ticks_of_day, ticks_of_second));
  case precision2::hour: return format_duration_impl(rclock::duration<std::chrono::hours>(ticks, ticks_of_day, ticks_of_second));
  case precision2::minute: return format_duration_impl(rclock::duration<std::chrono::minutes>(ticks, ticks_of_day, ticks_of_second));
  case precision2::second: return format_duration_impl(rclock::duration<std::chrono::seconds>(ticks, ticks_of_day, ticks_of_second));
  case precision2::millisecond: return format_duration_impl(rclock::duration<std::chrono::milliseconds>(ticks, ticks_of_day, ticks_of_second));
  case precision2::microsecond: return format_duration_impl(rclock::duration<std::chrono::microseconds>(ticks, ticks_of_day, ticks_of_second));
  case precision2::nanosecond: return format_duration_impl(rclock::duration<std::chrono::nanoseconds>(ticks, ticks_of_day, ticks_of_second));
  default: clock_abort("Internal error: Should never be called.");
  }
}

// -----------------------------------------------------------------------------

template <typename Duration>
inline
cpp11::writable::list_of<cpp11::writable::integers>
duration_helper_impl(const cpp11::integers& n) {
  r_ssize size = n.size();
  rclock::writable::duration<Duration> cd(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_n = n[i];

    if (elt_n == r_int_na) {
      cd.assign_na(i);
      continue;
    }

    Duration duration{elt_n};
    cd.assign(duration, i);
  }

  return cd.to_list();
}

[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
duration_helper_cpp(const cpp11::integers& n,
                    const cpp11::strings& precision) {
  switch (parse_precision2(precision)) {
  case precision2::year: return duration_helper_impl<date::years>(n);
  case precision2::quarter: return duration_helper_impl<quarterly::quarters>(n);
  case precision2::month: return duration_helper_impl<date::months>(n);
  case precision2::week: return duration_helper_impl<date::weeks>(n);
  case precision2::day: return duration_helper_impl<date::days>(n);
  case precision2::hour: return duration_helper_impl<std::chrono::hours>(n);
  case precision2::minute: return duration_helper_impl<std::chrono::minutes>(n);
  case precision2::second: return duration_helper_impl<std::chrono::seconds>(n);
  case precision2::millisecond: return duration_helper_impl<std::chrono::milliseconds>(n);
  case precision2::microsecond: return duration_helper_impl<std::chrono::microseconds>(n);
  case precision2::nanosecond: return duration_helper_impl<std::chrono::nanoseconds>(n);
  default: clock_abort("Internal error: Should never be called.");
  }
}

// -----------------------------------------------------------------------------

template <typename Duration1, typename Duration2>
cpp11::writable::list_of<cpp11::writable::integers>
duration_cast_impl(const rclock::duration<Duration1>& cd) {
  r_ssize size = cd.size();
  rclock::writable::duration<Duration2> out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (cd.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    const Duration1 from = cd[i];
    const Duration2 to = std::chrono::duration_cast<Duration2>(from);

    out.assign(to, i);
  }

  return out.to_list();
}

template <typename Duration1>
inline
cpp11::writable::list_of<cpp11::writable::integers>
duration_cast_switch2(const rclock::duration<Duration1>& cd,
                      const enum precision2 precision_to_val) {
  switch (precision_to_val) {
  case precision2::year: return duration_cast_impl<Duration1, date::years>(cd);
  case precision2::quarter: return duration_cast_impl<Duration1, quarterly::quarters>(cd);
  case precision2::month: return duration_cast_impl<Duration1, date::months>(cd);
  case precision2::week: return duration_cast_impl<Duration1, date::weeks>(cd);
  case precision2::day: return duration_cast_impl<Duration1, date::days>(cd);
  case precision2::hour: return duration_cast_impl<Duration1, std::chrono::hours>(cd);
  case precision2::minute: return duration_cast_impl<Duration1, std::chrono::minutes>(cd);
  case precision2::second: return duration_cast_impl<Duration1, std::chrono::seconds>(cd);
  case precision2::millisecond: return duration_cast_impl<Duration1, std::chrono::milliseconds>(cd);
  case precision2::microsecond: return duration_cast_impl<Duration1, std::chrono::microseconds>(cd);
  case precision2::nanosecond: return duration_cast_impl<Duration1, std::chrono::nanoseconds>(cd);
  }
}

inline
cpp11::writable::list_of<cpp11::writable::integers>
duration_cast_switch(const cpp11::integers& ticks,
                     const cpp11::integers& ticks_of_day,
                     const cpp11::integers& ticks_of_second,
                     const enum precision2 precision_from_val,
                     const enum precision2 precision_to_val) {
  switch (precision_from_val) {
  case precision2::year: return duration_cast_switch2(rclock::duration<date::years>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  case precision2::quarter: return duration_cast_switch2(rclock::duration<quarterly::quarters>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  case precision2::month: return duration_cast_switch2(rclock::duration<date::months>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  case precision2::week: return duration_cast_switch2(rclock::duration<date::weeks>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  case precision2::day: return duration_cast_switch2(rclock::duration<date::days>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  case precision2::hour: return duration_cast_switch2(rclock::duration<std::chrono::hours>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  case precision2::minute: return duration_cast_switch2(rclock::duration<std::chrono::minutes>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  case precision2::second: return duration_cast_switch2(rclock::duration<std::chrono::seconds>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  case precision2::millisecond: return duration_cast_switch2(rclock::duration<std::chrono::milliseconds>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  case precision2::microsecond: return duration_cast_switch2(rclock::duration<std::chrono::microseconds>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  case precision2::nanosecond: return duration_cast_switch2(rclock::duration<std::chrono::nanoseconds>(ticks, ticks_of_day, ticks_of_second), precision_to_val);
  }
}

[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::integers>
duration_cast_cpp(const cpp11::integers& ticks,
                  const cpp11::integers& ticks_of_day,
                  const cpp11::integers& ticks_of_second,
                  const cpp11::strings& precision_from,
                  const cpp11::strings& precision_to) {
  const enum precision2 precision_from_val = parse_precision2(precision_from);
  const enum precision2 precision_to_val = parse_precision2(precision_to);

  return duration_cast_switch(
    ticks,
    ticks_of_day,
    ticks_of_second,
    precision_from_val,
    precision_to_val
  );
}
