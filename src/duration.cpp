#include "clock.h"
#include "utils.h"
#include "duration.h"
#include "enums.h"
#include "get.h"
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
                                             const cpp11::strings& precision_string) {
  using namespace rclock;

  cpp11::integers ticks = duration::get_ticks(fields);
  cpp11::integers ticks_of_day = duration::get_ticks_of_day(fields);
  cpp11::integers ticks_of_second = duration::get_ticks_of_second(fields);

  duration::years dy{ticks};
  duration::quarters dq{ticks};
  duration::months dm{ticks};
  duration::weeks dw{ticks};
  duration::days dd{ticks};
  duration::hours dh{ticks, ticks_of_day};
  duration::minutes dmin{ticks, ticks_of_day};
  duration::seconds ds{ticks, ticks_of_day};
  duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision(precision_string)) {
  case precision::year: return format_duration_impl(dy);
  case precision::quarter: return format_duration_impl(dq);
  case precision::month: return format_duration_impl(dm);
  case precision::week: return format_duration_impl(dw);
  case precision::day: return format_duration_impl(dd);
  case precision::hour: return format_duration_impl(dh);
  case precision::minute: return format_duration_impl(dmin);
  case precision::second: return format_duration_impl(ds);
  case precision::millisecond: return format_duration_impl(dmilli);
  case precision::microsecond: return format_duration_impl(dmicro);
  case precision::nanosecond: return format_duration_impl(dnano);
  }

  never_reached("format_duration_cpp");
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
                    const cpp11::strings& precision_string) {
  using namespace rclock;

  switch (parse_precision(precision_string)) {
  case precision::year: return duration_helper_impl<duration::years>(n);
  case precision::quarter: return duration_helper_impl<duration::quarters>(n);
  case precision::month: return duration_helper_impl<duration::months>(n);
  case precision::week: return duration_helper_impl<duration::weeks>(n);
  case precision::day: return duration_helper_impl<duration::days>(n);
  case precision::hour: return duration_helper_impl<duration::hours>(n);
  case precision::minute: return duration_helper_impl<duration::minutes>(n);
  case precision::second: return duration_helper_impl<duration::seconds>(n);
  case precision::millisecond: return duration_helper_impl<duration::milliseconds>(n);
  case precision::microsecond: return duration_helper_impl<duration::microseconds>(n);
  case precision::nanosecond: return duration_helper_impl<duration::nanoseconds>(n);
  }

  never_reached("duration_helper_cpp");
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
                      const enum precision precision_to_val) {
  using namespace rclock;

  switch (precision_to_val) {
  case precision::year: return duration_cast_impl<duration::years>(cd);
  case precision::quarter: return duration_cast_impl<duration::quarters>(cd);
  case precision::month: return duration_cast_impl<duration::months>(cd);
  case precision::week: return duration_cast_impl<duration::weeks>(cd);
  case precision::day: return duration_cast_impl<duration::days>(cd);
  case precision::hour: return duration_cast_impl<duration::hours>(cd);
  case precision::minute: return duration_cast_impl<duration::minutes>(cd);
  case precision::second: return duration_cast_impl<duration::seconds>(cd);
  case precision::millisecond: return duration_cast_impl<duration::milliseconds>(cd);
  case precision::microsecond: return duration_cast_impl<duration::microseconds>(cd);
  case precision::nanosecond: return duration_cast_impl<duration::nanoseconds>(cd);
  }

  never_reached("duration_cast_switch2");
}

inline
cpp11::writable::list
duration_cast_switch(cpp11::list_of<cpp11::integers>& fields,
                     const enum precision precision_from_val,
                     const enum precision precision_to_val) {
  using namespace rclock;

  cpp11::integers ticks = duration::get_ticks(fields);
  cpp11::integers ticks_of_day = duration::get_ticks_of_day(fields);
  cpp11::integers ticks_of_second = duration::get_ticks_of_second(fields);

  duration::years dy{ticks};
  duration::quarters dq{ticks};
  duration::months dm{ticks};
  duration::weeks dw{ticks};
  duration::days dd{ticks};
  duration::hours dh{ticks, ticks_of_day};
  duration::minutes dmin{ticks, ticks_of_day};
  duration::seconds ds{ticks, ticks_of_day};
  duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (precision_from_val) {
  case precision::year: return duration_cast_switch2(dy, precision_to_val);
  case precision::quarter: return duration_cast_switch2(dq, precision_to_val);
  case precision::month: return duration_cast_switch2(dm, precision_to_val);
  case precision::week: return duration_cast_switch2(dw, precision_to_val);
  case precision::day: return duration_cast_switch2(dd, precision_to_val);
  case precision::hour: return duration_cast_switch2(dh, precision_to_val);
  case precision::minute: return duration_cast_switch2(dmin, precision_to_val);
  case precision::second: return duration_cast_switch2(ds, precision_to_val);
  case precision::millisecond: return duration_cast_switch2(dmilli, precision_to_val);
  case precision::microsecond: return duration_cast_switch2(dmicro, precision_to_val);
  case precision::nanosecond: return duration_cast_switch2(dnano, precision_to_val);
  }

  never_reached("duration_cast_switch");
}

[[cpp11::register]]
cpp11::writable::list
duration_cast_cpp(cpp11::list_of<cpp11::integers> fields,
                  const cpp11::strings& precision_from,
                  const cpp11::strings& precision_to) {
  const enum precision precision_from_val = parse_precision(precision_from);
  const enum precision precision_to_val = parse_precision(precision_to);

  return duration_cast_switch(
    fields,
    precision_from_val,
    precision_to_val
  );
}

// -----------------------------------------------------------------------------

enum class arith_op {
  plus,
  minus,
  modulus
};

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_arith_impl(const ClockDuration& x,
                    const ClockDuration& y,
                    const enum arith_op& op) {
  r_ssize size = x.size();
  ClockDuration out(size);

  switch (op) {
  case arith_op::plus: {
    for (r_ssize i = 0; i < size; ++i) {
      if (x.is_na(i) || y.is_na(i)) {
        out.assign_na(i);
        continue;
      }
      out.assign(x[i] + y[i], i);
    }
    break;
  }
  case arith_op::minus: {
    for (r_ssize i = 0; i < size; ++i) {
      if (x.is_na(i) || y.is_na(i)) {
        out.assign_na(i);
        continue;
      }
      out.assign(x[i] - y[i], i);
    }
    break;
  }
  case arith_op::modulus: {
    for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || y.is_na(i)) {
      out.assign_na(i);
      continue;
    }
    out.assign(x[i] % y[i], i);
  }
    break;
  }
  }

  return out.to_list();
}

static
inline
cpp11::writable::list
duration_arith_switch(cpp11::list_of<cpp11::integers>& x,
                      cpp11::list_of<cpp11::integers>& y,
                      const enum precision& precision_val,
                      const enum arith_op& op) {
  using namespace rclock;

  const cpp11::integers x_ticks = duration::get_ticks(x);
  const cpp11::integers x_ticks_of_day = duration::get_ticks_of_day(x);
  const cpp11::integers x_ticks_of_second = duration::get_ticks_of_second(x);

  const cpp11::integers y_ticks = duration::get_ticks(y);
  const cpp11::integers y_ticks_of_day = duration::get_ticks_of_day(y);
  const cpp11::integers y_ticks_of_second = duration::get_ticks_of_second(y);

  const duration::years x_dy{x_ticks};
  const duration::quarters x_dq{x_ticks};
  const duration::months x_dm{x_ticks};
  const duration::weeks x_dw{x_ticks};
  const duration::days x_dd{x_ticks};
  const duration::hours x_dh{x_ticks, x_ticks_of_day};
  const duration::minutes x_dmin{x_ticks, x_ticks_of_day};
  const duration::seconds x_ds{x_ticks, x_ticks_of_day};
  const duration::milliseconds x_dmilli{x_ticks, x_ticks_of_day, x_ticks_of_second};
  const duration::microseconds x_dmicro{x_ticks, x_ticks_of_day, x_ticks_of_second};
  const duration::nanoseconds x_dnano{x_ticks, x_ticks_of_day, x_ticks_of_second};

  const duration::years y_dy{y_ticks};
  const duration::quarters y_dq{y_ticks};
  const duration::months y_dm{y_ticks};
  const duration::weeks y_dw{y_ticks};
  const duration::days y_dd{y_ticks};
  const duration::hours y_dh{y_ticks, y_ticks_of_day};
  const duration::minutes y_dmin{y_ticks, y_ticks_of_day};
  const duration::seconds y_ds{y_ticks, y_ticks_of_day};
  const duration::milliseconds y_dmilli{y_ticks, y_ticks_of_day, y_ticks_of_second};
  const duration::microseconds y_dmicro{y_ticks, y_ticks_of_day, y_ticks_of_second};
  const duration::nanoseconds y_dnano{y_ticks, y_ticks_of_day, y_ticks_of_second};

  switch (precision_val) {
  case precision::year: return duration_arith_impl(x_dy, y_dy, op);
  case precision::quarter: return duration_arith_impl(x_dq, y_dq, op);
  case precision::month: return duration_arith_impl(x_dm, y_dm, op);
  case precision::week: return duration_arith_impl(x_dw, y_dw, op);
  case precision::day: return duration_arith_impl(x_dd, y_dd, op);
  case precision::hour: return duration_arith_impl(x_dh, y_dh, op);
  case precision::minute: return duration_arith_impl(x_dmin, y_dmin, op);
  case precision::second: return duration_arith_impl(x_ds, y_ds, op);
  case precision::millisecond: return duration_arith_impl(x_dmilli, y_dmilli, op);
  case precision::microsecond: return duration_arith_impl(x_dmicro, y_dmicro, op);
  case precision::nanosecond: return duration_arith_impl(x_dnano, y_dnano, op);
  }

  never_reached("duration_arith_switch");
}

static
inline
cpp11::writable::list
duration_arith(cpp11::list_of<cpp11::integers>& x,
               cpp11::list_of<cpp11::integers>& y,
               const cpp11::strings& precision_string,
               const enum arith_op& op) {
  const enum precision precision_val = parse_precision(precision_string);

  return duration_arith_switch(
    x,
    y,
    precision_val,
    op
  );
}

[[cpp11::register]]
cpp11::writable::list
duration_plus_cpp(cpp11::list_of<cpp11::integers> x,
                  cpp11::list_of<cpp11::integers> y,
                  const cpp11::strings& precision_string) {
  return duration_arith(x, y, precision_string, arith_op::plus);
}

[[cpp11::register]]
cpp11::writable::list
duration_minus_cpp(cpp11::list_of<cpp11::integers> x,
                   cpp11::list_of<cpp11::integers> y,
                   const cpp11::strings& precision_string) {
  return duration_arith(x, y, precision_string, arith_op::minus);
}

[[cpp11::register]]
cpp11::writable::list
duration_modulus_cpp(cpp11::list_of<cpp11::integers> x,
                     cpp11::list_of<cpp11::integers> y,
                     const cpp11::strings& precision_string) {
  return duration_arith(x, y, precision_string, arith_op::modulus);
}

// -----------------------------------------------------------------------------

enum class arith_scalar_op {
  multiply,
  divide
};

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_scalar_arith_impl(const ClockDuration& x,
                           const cpp11::integers& y,
                           const enum arith_scalar_op& op) {
  r_ssize size = x.size();
  ClockDuration out(size);

  switch (op) {
  case arith_scalar_op::multiply: {
    for (r_ssize i = 0; i < size; ++i) {
      const int elt_y = y[i];
      if (x.is_na(i) || elt_y == r_int_na) {
        out.assign_na(i);
        continue;
      }
      out.assign(x[i] * elt_y, i);
  }
    break;
  }
  case arith_scalar_op::divide: {
    for (r_ssize i = 0; i < size; ++i) {
      const int elt_y = y[i];
      if (x.is_na(i) || elt_y == r_int_na) {
        out.assign_na(i);
        continue;
      }
      out.assign(x[i] / elt_y, i);
  }
    break;
  }
  }

  return out.to_list();
}


static
inline
cpp11::writable::list
duration_scalar_arith(cpp11::list_of<cpp11::integers>& x,
                      const cpp11::integers& y,
                      const cpp11::strings& precision_string,
                      const enum arith_scalar_op& op) {
  using namespace rclock;

  cpp11::integers ticks = duration::get_ticks(x);
  cpp11::integers ticks_of_day = duration::get_ticks_of_day(x);
  cpp11::integers ticks_of_second = duration::get_ticks_of_second(x);

  duration::years dy{ticks};
  duration::quarters dq{ticks};
  duration::months dm{ticks};
  duration::weeks dw{ticks};
  duration::days dd{ticks};
  duration::hours dh{ticks, ticks_of_day};
  duration::minutes dmin{ticks, ticks_of_day};
  duration::seconds ds{ticks, ticks_of_day};
  duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision(precision_string)) {
  case precision::year: return duration_scalar_arith_impl(dy, y, op);
  case precision::quarter: return duration_scalar_arith_impl(dq, y, op);
  case precision::month: return duration_scalar_arith_impl(dm, y, op);
  case precision::week: return duration_scalar_arith_impl(dw, y, op);
  case precision::day: return duration_scalar_arith_impl(dd, y, op);
  case precision::hour: return duration_scalar_arith_impl(dh, y, op);
  case precision::minute: return duration_scalar_arith_impl(dmin, y, op);
  case precision::second: return duration_scalar_arith_impl(ds, y, op);
  case precision::millisecond: return duration_scalar_arith_impl(dmilli, y, op);
  case precision::microsecond: return duration_scalar_arith_impl(dmicro, y, op);
  case precision::nanosecond: return duration_scalar_arith_impl(dnano, y, op);
  }

  never_reached("duration_scalar_arith");
}

[[cpp11::register]]
cpp11::writable::list
duration_scalar_multiply_cpp(cpp11::list_of<cpp11::integers> x,
                             const cpp11::integers& y,
                             const cpp11::strings& precision_string) {
  return duration_scalar_arith(x, y, precision_string, arith_scalar_op::multiply);
}

[[cpp11::register]]
cpp11::writable::list
duration_scalar_divide_cpp(cpp11::list_of<cpp11::integers> x,
                           const cpp11::integers& y,
                           const cpp11::strings& precision_string) {
  return duration_scalar_arith(x, y, precision_string, arith_scalar_op::divide);
}

// -----------------------------------------------------------------------------

/*
 * Restricts normal result returned by `std::common_type()` to only allow
 * common durations that result in a named duration type (year, month, ...).
 * For example, duration_common("year", "month") is allowed because month is
 * defined as 1/12 of a year. duration_common("year", "second") is allowed
 * because years are defined in terms of seconds. But
 * duration_common("year", "day") is not allowed because the greatest common
 * divisor between their ratios is 216, which would create an unnamed duration.
 */
template <typename Duration1, typename Duration2>
inline
std::pair<enum precision, bool>
duration_common_precision_impl() {
  using CT = typename std::common_type<Duration1, Duration2>::type;

  if (std::is_same<CT, date::years>::value) {
    return std::make_pair(precision::year, true);
  } else if (std::is_same<CT, quarterly::quarters>::value) {
    return std::make_pair(precision::quarter, true);
  } else if (std::is_same<CT, date::months>::value) {
    return std::make_pair(precision::month, true);
  } else if (std::is_same<CT, date::weeks>::value) {
    return std::make_pair(precision::week, true);
  } else if (std::is_same<CT, date::days>::value) {
    return std::make_pair(precision::day, true);
  } else if (std::is_same<CT, std::chrono::hours>::value) {
    return std::make_pair(precision::hour, true);
  } else if (std::is_same<CT, std::chrono::minutes>::value) {
    return std::make_pair(precision::minute, true);
  } else if (std::is_same<CT, std::chrono::seconds>::value) {
    return std::make_pair(precision::second, true);
  } else if (std::is_same<CT, std::chrono::milliseconds>::value) {
    return std::make_pair(precision::millisecond, true);
  } else if (std::is_same<CT, std::chrono::microseconds>::value) {
    return std::make_pair(precision::microsecond, true);
  } else if (std::is_same<CT, std::chrono::nanoseconds>::value) {
    return std::make_pair(precision::nanosecond, true);
  } else {
    return std::make_pair(precision::year, false);
  }

  never_reached("duration_common_precision_impl");
}

template <typename Duration1>
static
inline
std::pair<enum precision, bool>
duration_common_precision_switch2(const enum precision& y_precision) {
  switch (y_precision) {
  case precision::year: return duration_common_precision_impl<Duration1, date::years>();
  case precision::quarter: return duration_common_precision_impl<Duration1, quarterly::quarters>();
  case precision::month: return duration_common_precision_impl<Duration1, date::months>();
  case precision::week: return duration_common_precision_impl<Duration1, date::weeks>();
  case precision::day: return duration_common_precision_impl<Duration1, date::days>();
  case precision::hour: return duration_common_precision_impl<Duration1, std::chrono::hours>();
  case precision::minute: return duration_common_precision_impl<Duration1, std::chrono::minutes>();
  case precision::second: return duration_common_precision_impl<Duration1, std::chrono::seconds>();
  case precision::millisecond: return duration_common_precision_impl<Duration1, std::chrono::milliseconds>();
  case precision::microsecond: return duration_common_precision_impl<Duration1, std::chrono::microseconds>();
  case precision::nanosecond: return duration_common_precision_impl<Duration1, std::chrono::nanoseconds>();
  }
  never_reached("duration_common_precision_switch2");
}

static
inline
std::pair<enum precision, bool>
duration_common_precision_pair(const enum precision& x_precision,
                               const enum precision& y_precision) {
  switch (x_precision) {
  case precision::year: return duration_common_precision_switch2<date::years>(y_precision);
  case precision::quarter: return duration_common_precision_switch2<quarterly::quarters>(y_precision);
  case precision::month: return duration_common_precision_switch2<date::months>(y_precision);
  case precision::week: return duration_common_precision_switch2<date::weeks>(y_precision);
  case precision::day: return duration_common_precision_switch2<date::days>(y_precision);
  case precision::hour: return duration_common_precision_switch2<std::chrono::hours>(y_precision);
  case precision::minute: return duration_common_precision_switch2<std::chrono::minutes>(y_precision);
  case precision::second: return duration_common_precision_switch2<std::chrono::seconds>(y_precision);
  case precision::millisecond: return duration_common_precision_switch2<std::chrono::milliseconds>(y_precision);
  case precision::microsecond: return duration_common_precision_switch2<std::chrono::microseconds>(y_precision);
  case precision::nanosecond: return duration_common_precision_switch2<std::chrono::nanoseconds>(y_precision);
  }
  never_reached("duration_common_precision_pair");
}

[[cpp11::register]]
cpp11::writable::strings
duration_precision_common_cpp(const cpp11::strings& x_precision,
                              const cpp11::strings& y_precision) {
  const enum precision x_precision_val = parse_precision(x_precision);
  const enum precision y_precision_val = parse_precision(y_precision);
  const std::pair<enum precision, bool> pair = duration_common_precision_pair(x_precision_val, y_precision_val);

  if (pair.second) {
    const std::string string = precision_to_string(pair.first);
    return cpp11::writable::strings({string});
  } else {
    return cpp11::writable::strings({r_chr_na});
  }
}

[[cpp11::register]]
bool
duration_has_common_precision_cpp(const cpp11::strings& x_precision,
                                  const cpp11::strings& y_precision) {
  const enum precision x_precision_val = parse_precision(x_precision);
  const enum precision y_precision_val = parse_precision(y_precision);
  return duration_common_precision_pair(x_precision_val, y_precision_val).second;
}

// -----------------------------------------------------------------------------

enum class rounding {
  round,
  floor,
  ceil,
};

template <class Duration>
static
inline
Duration
clock_multi_floor_impl(const Duration& x, const int& n) {
  const typename Duration::rep c = x.count();
  return Duration{(c >= 0 ? c : (c - n + 1)) / n * n};
}

template <class DurationTo, class DurationFrom>
static
inline
DurationTo
clock_floor(const DurationFrom& d, const int& n) {
  const DurationTo x = date::floor<DurationTo>(d);
  return n == 1 ? x : clock_multi_floor_impl(x, n);
}

template <class DurationTo, class DurationFrom>
static
inline
DurationTo
clock_ceil(const DurationFrom& d, const int& n) {
  DurationTo x = clock_floor<DurationTo>(d, n);
  if (x < d) {
    // Return input at new precision if on boundary, otherwise do ceiling
    x += DurationTo{n};
  }
  return x;
}

template <class DurationTo, class DurationFrom>
static
inline
DurationTo
clock_round(const DurationFrom& d, const int& n) {
  const DurationTo floor = clock_floor<DurationTo>(d, n);
  const DurationTo ceil = floor < d ? floor + DurationTo{n} : floor;

  if (ceil - d <= d - floor) {
    return ceil;
  } else {
    return floor;
  }
}

template <class ClockDurationTo, class ClockDurationFrom>
cpp11::writable::list
duration_rounding_impl(const ClockDurationFrom& cd,
                       const int& n,
                       const enum rounding& type) {
  using DurationFrom = typename ClockDurationFrom::duration;
  using DurationTo = typename ClockDurationTo::duration;

  r_ssize size = cd.size();
  ClockDurationTo out(size);

  if (type == rounding::floor) {
    for (r_ssize i = 0; i < size; ++i) {
      if (cd.is_na(i)) {
        out.assign_na(i);
        continue;
      }
      const DurationFrom from = cd[i];
      const DurationTo to = clock_floor<DurationTo>(from, n);
      out.assign(to, i);
    }
  } else if (type == rounding::ceil) {
    for (r_ssize i = 0; i < size; ++i) {
      if (cd.is_na(i)) {
        out.assign_na(i);
        continue;
      }
      const DurationFrom from = cd[i];
      const DurationTo to = clock_ceil<DurationTo>(from, n);
      out.assign(to, i);
    }
  } else {
    for (r_ssize i = 0; i < size; ++i) {
      if (cd.is_na(i)) {
        out.assign_na(i);
        continue;
      }
      const DurationFrom from = cd[i];
      const DurationTo to = clock_round<DurationTo>(from, n);
      out.assign(to, i);
    }
  }

  return out.to_list();
}

inline
cpp11::writable::list
duration_rounding_switch(cpp11::list_of<cpp11::integers>& fields,
                         const enum precision& precision_from_val,
                         const enum precision& precision_to_val,
                         const int& n,
                         const enum rounding& type) {
  using namespace rclock;

  cpp11::integers ticks = duration::get_ticks(fields);
  cpp11::integers ticks_of_day = duration::get_ticks_of_day(fields);
  cpp11::integers ticks_of_second = duration::get_ticks_of_second(fields);

  duration::years dy{ticks};
  duration::quarters dq{ticks};
  duration::months dm{ticks};
  duration::weeks dw{ticks};
  duration::days dd{ticks};
  duration::hours dh{ticks, ticks_of_day};
  duration::minutes dmin{ticks, ticks_of_day};
  duration::seconds ds{ticks, ticks_of_day};
  duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (precision_from_val) {
  case precision::year: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(dy, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::quarter: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(dq, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters>(dq, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::month: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(dm, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters>(dm, n, type);
    case precision::month: return duration_rounding_impl<duration::months>(dm, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::week: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(dw, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters>(dw, n, type);
    case precision::month: return duration_rounding_impl<duration::months>(dw, n, type);
    case precision::week: return duration_rounding_impl<duration::weeks>(dw, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::day: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(dd, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters>(dd, n, type);
    case precision::month: return duration_rounding_impl<duration::months>(dd, n, type);
    case precision::week: return duration_rounding_impl<duration::weeks>(dd, n, type);
    case precision::day: return duration_rounding_impl<duration::days>(dd, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::hour: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(dh, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters>(dh, n, type);
    case precision::month: return duration_rounding_impl<duration::months>(dh, n, type);
    case precision::week: return duration_rounding_impl<duration::weeks>(dh, n, type);
    case precision::day: return duration_rounding_impl<duration::days>(dh, n, type);
    case precision::hour: return duration_rounding_impl<duration::hours>(dh, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::minute: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(dmin, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters>(dmin, n, type);
    case precision::month: return duration_rounding_impl<duration::months>(dmin, n, type);
    case precision::week: return duration_rounding_impl<duration::weeks>(dmin, n, type);
    case precision::day: return duration_rounding_impl<duration::days>(dmin, n, type);
    case precision::hour: return duration_rounding_impl<duration::hours>(dmin, n, type);
    case precision::minute: return duration_rounding_impl<duration::minutes>(dmin, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::second: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(ds, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters>(ds, n, type);
    case precision::month: return duration_rounding_impl<duration::months>(ds, n, type);
    case precision::week: return duration_rounding_impl<duration::weeks>(ds, n, type);
    case precision::day: return duration_rounding_impl<duration::days>(ds, n, type);
    case precision::hour: return duration_rounding_impl<duration::hours>(ds, n, type);
    case precision::minute: return duration_rounding_impl<duration::minutes>(ds, n, type);
    case precision::second: return duration_rounding_impl<duration::seconds>(ds, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::millisecond: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(dmilli, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters>(dmilli, n, type);
    case precision::month: return duration_rounding_impl<duration::months>(dmilli, n, type);
    case precision::week: return duration_rounding_impl<duration::weeks>(dmilli, n, type);
    case precision::day: return duration_rounding_impl<duration::days>(dmilli, n, type);
    case precision::hour: return duration_rounding_impl<duration::hours>(dmilli, n, type);
    case precision::minute: return duration_rounding_impl<duration::minutes>(dmilli, n, type);
    case precision::second: return duration_rounding_impl<duration::seconds>(dmilli, n, type);
    case precision::millisecond: return duration_rounding_impl<duration::milliseconds>(dmilli, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::microsecond: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(dmicro, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters>(dmicro, n, type);
    case precision::month: return duration_rounding_impl<duration::months>(dmicro, n, type);
    case precision::week: return duration_rounding_impl<duration::weeks>(dmicro, n, type);
    case precision::day: return duration_rounding_impl<duration::days>(dmicro, n, type);
    case precision::hour: return duration_rounding_impl<duration::hours>(dmicro, n, type);
    case precision::minute: return duration_rounding_impl<duration::minutes>(dmicro, n, type);
    case precision::second: return duration_rounding_impl<duration::seconds>(dmicro, n, type);
    case precision::millisecond: return duration_rounding_impl<duration::milliseconds>(dmicro, n, type);
    case precision::microsecond: return duration_rounding_impl<duration::microseconds>(dmicro, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::nanosecond: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years>(dnano, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters>(dnano, n, type);
    case precision::month: return duration_rounding_impl<duration::months>(dnano, n, type);
    case precision::week: return duration_rounding_impl<duration::weeks>(dnano, n, type);
    case precision::day: return duration_rounding_impl<duration::days>(dnano, n, type);
    case precision::hour: return duration_rounding_impl<duration::hours>(dnano, n, type);
    case precision::minute: return duration_rounding_impl<duration::minutes>(dnano, n, type);
    case precision::second: return duration_rounding_impl<duration::seconds>(dnano, n, type);
    case precision::millisecond: return duration_rounding_impl<duration::milliseconds>(dnano, n, type);
    case precision::microsecond: return duration_rounding_impl<duration::microseconds>(dnano, n, type);
    case precision::nanosecond: return duration_rounding_impl<duration::nanoseconds>(dnano, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  }

  never_reached("duration_rounding_switch");
}

[[cpp11::register]]
cpp11::writable::list
duration_floor_cpp(cpp11::list_of<cpp11::integers> fields,
                   const cpp11::strings& precision_from,
                   const cpp11::strings& precision_to,
                   const int& n) {
  const enum precision precision_from_val = parse_precision(precision_from);
  const enum precision precision_to_val = parse_precision(precision_to);

  return duration_rounding_switch(
    fields,
    precision_from_val,
    precision_to_val,
    n,
    rounding::floor
  );
}

[[cpp11::register]]
cpp11::writable::list
duration_ceiling_cpp(cpp11::list_of<cpp11::integers> fields,
                     const cpp11::strings& precision_from,
                     const cpp11::strings& precision_to,
                     const int& n) {
  const enum precision precision_from_val = parse_precision(precision_from);
  const enum precision precision_to_val = parse_precision(precision_to);

  return duration_rounding_switch(
    fields,
    precision_from_val,
    precision_to_val,
    n,
    rounding::ceil
  );
}

[[cpp11::register]]
cpp11::writable::list
duration_round_cpp(cpp11::list_of<cpp11::integers> fields,
                   const cpp11::strings& precision_from,
                   const cpp11::strings& precision_to,
                   const int& n) {
  const enum precision precision_from_val = parse_precision(precision_from);
  const enum precision precision_to_val = parse_precision(precision_to);

  return duration_rounding_switch(
    fields,
    precision_from_val,
    precision_to_val,
    n,
    rounding::round
  );
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_unary_minus_impl(const ClockDuration& x) {
  const r_ssize size = x.size();
  ClockDuration out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
      continue;
    }
    out.assign(-x[i], i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
duration_unary_minus_cpp(cpp11::list_of<cpp11::integers> fields, const cpp11::strings& precision_string) {
  using namespace rclock;

  cpp11::integers ticks = duration::get_ticks(fields);
  cpp11::integers ticks_of_day = duration::get_ticks_of_day(fields);
  cpp11::integers ticks_of_second = duration::get_ticks_of_second(fields);

  duration::years dy{ticks};
  duration::quarters dq{ticks};
  duration::months dm{ticks};
  duration::weeks dw{ticks};
  duration::days dd{ticks};
  duration::hours dh{ticks, ticks_of_day};
  duration::minutes dmin{ticks, ticks_of_day};
  duration::seconds ds{ticks, ticks_of_day};
  duration::milliseconds dmilli{ticks, ticks_of_day, ticks_of_second};
  duration::microseconds dmicro{ticks, ticks_of_day, ticks_of_second};
  duration::nanoseconds dnano{ticks, ticks_of_day, ticks_of_second};

  switch (parse_precision(precision_string)) {
  case precision::year: return duration_unary_minus_impl(dy);
  case precision::quarter: return duration_unary_minus_impl(dq);
  case precision::month: return duration_unary_minus_impl(dm);
  case precision::week: return duration_unary_minus_impl(dw);
  case precision::day: return duration_unary_minus_impl(dd);
  case precision::hour: return duration_unary_minus_impl(dh);
  case precision::minute: return duration_unary_minus_impl(dmin);
  case precision::second: return duration_unary_minus_impl(ds);
  case precision::millisecond: return duration_unary_minus_impl(dmilli);
  case precision::microsecond: return duration_unary_minus_impl(dmicro);
  case precision::nanosecond: return duration_unary_minus_impl(dnano);
  }

  never_reached("duration_unary_minus_cpp");
}


