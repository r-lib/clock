#include "clock.h"
#include "utils.h"
#include "duration.h"
#include "enums.h"
#include "get.h"
#include "rcrd.h"
#include <sstream>
#include <cfloat>
#include <algorithm>
#include <limits>

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
new_duration_from_fields(SEXP fields,
                         const cpp11::integers& precision_int,
                         SEXP names) {
  const r_ssize n_fields = Rf_xlength(fields);
  if (n_fields != 2) {
    clock_abort("`fields` must be length 2.");
  }

  SEXP out = PROTECT(new_clock_rcrd_from_fields(fields, names, classes_duration));

  Rf_setAttrib(out, syms_precision, precision_int);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
duration_restore(SEXP x, SEXP to) {
  SEXP out = PROTECT(clock_rcrd_restore(x, to, classes_duration));

  SEXP precision = Rf_getAttrib(to, syms_precision);
  Rf_setAttrib(out, syms_precision, precision);

  UNPROTECT(1);
  return out;
}

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
format_duration_impl(cpp11::list_of<cpp11::doubles>& fields) {
  const ClockDuration x{fields};

  const r_ssize size = x.size();

  std::ostringstream stream;
  cpp11::writable::strings out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      SET_STRING_ELT(out, i, r_chr_na);
      continue;
    }

    typename ClockDuration::chrono_duration duration = x[i];

    stream.str(std::string());
    stream.clear();

    duration_stream(stream, duration);
    std::string string = stream.str();

    SET_STRING_ELT(out, i, Rf_mkCharLenCE(string.c_str(), string.size(), CE_UTF8));
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::strings format_duration_cpp(cpp11::list_of<cpp11::doubles> fields,
                                             const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return format_duration_impl<duration::years>(fields);
  case precision::quarter: return format_duration_impl<duration::quarters>(fields);
  case precision::month: return format_duration_impl<duration::months>(fields);
  case precision::week: return format_duration_impl<duration::weeks>(fields);
  case precision::day: return format_duration_impl<duration::days>(fields);
  case precision::hour: return format_duration_impl<duration::hours>(fields);
  case precision::minute: return format_duration_impl<duration::minutes>(fields);
  case precision::second: return format_duration_impl<duration::seconds>(fields);
  case precision::millisecond: return format_duration_impl<duration::milliseconds>(fields);
  case precision::microsecond: return format_duration_impl<duration::microseconds>(fields);
  case precision::nanosecond: return format_duration_impl<duration::nanoseconds>(fields);
  default: never_reached("format_duration_cpp");
  }
}

// -----------------------------------------------------------------------------

template <typename ClockDuration>
inline
cpp11::writable::list_of<cpp11::writable::doubles>
duration_helper_impl(const cpp11::integers& n) {
  const r_ssize size = n.size();
  ClockDuration out(size);

  for (r_ssize i = 0; i < size; ++i) {
    const int n_elt = n[i];

    if (n_elt == r_int_na) {
      out.assign_na(i);
      continue;
    }

    const typename ClockDuration::chrono_duration elt{n_elt};

    out.assign(elt, i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list_of<cpp11::writable::doubles>
duration_helper_cpp(const cpp11::integers& n,
                    const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
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
  default: never_reached("duration_helper_cpp");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDurationFrom, class ClockDurationTo>
inline
cpp11::writable::list
duration_cast_impl(cpp11::list_of<cpp11::doubles>& fields) {
  using DurationFrom = typename ClockDurationFrom::chrono_duration;
  using DurationTo = typename ClockDurationTo::chrono_duration;

  const ClockDurationFrom x{fields};

  if (std::is_same<DurationFrom, DurationTo>::value) {
    return(x.to_list());
  }

  const r_ssize size = x.size();
  ClockDurationTo out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    const DurationFrom x_elt = x[i];
    const DurationTo out_elt = std::chrono::duration_cast<DurationTo>(x_elt);

    out.assign(out_elt, i);
  }

  return out.to_list();
}

template <class ClockDurationFrom>
inline
cpp11::writable::list
duration_cast_switch2(cpp11::list_of<cpp11::doubles>& fields,
                      const enum precision precision_to_val) {
  using namespace rclock;

  switch (precision_to_val) {
  case precision::year: return duration_cast_impl<ClockDurationFrom, duration::years>(fields);
  case precision::quarter: return duration_cast_impl<ClockDurationFrom, duration::quarters>(fields);
  case precision::month: return duration_cast_impl<ClockDurationFrom, duration::months>(fields);
  case precision::week: return duration_cast_impl<ClockDurationFrom, duration::weeks>(fields);
  case precision::day: return duration_cast_impl<ClockDurationFrom, duration::days>(fields);
  case precision::hour: return duration_cast_impl<ClockDurationFrom, duration::hours>(fields);
  case precision::minute: return duration_cast_impl<ClockDurationFrom, duration::minutes>(fields);
  case precision::second: return duration_cast_impl<ClockDurationFrom, duration::seconds>(fields);
  case precision::millisecond: return duration_cast_impl<ClockDurationFrom, duration::milliseconds>(fields);
  case precision::microsecond: return duration_cast_impl<ClockDurationFrom, duration::microseconds>(fields);
  case precision::nanosecond: return duration_cast_impl<ClockDurationFrom, duration::nanoseconds>(fields);
  default: never_reached("duration_cast_switch2");
  }
}

inline
cpp11::writable::list
duration_cast_switch(cpp11::list_of<cpp11::doubles>& fields,
                     const enum precision precision_from_val,
                     const enum precision precision_to_val) {
  using namespace rclock;

  switch (precision_from_val) {
  case precision::year: return duration_cast_switch2<duration::years>(fields, precision_to_val);
  case precision::quarter: return duration_cast_switch2<duration::quarters>(fields, precision_to_val);
  case precision::month: return duration_cast_switch2<duration::months>(fields, precision_to_val);
  case precision::week: return duration_cast_switch2<duration::weeks>(fields, precision_to_val);
  case precision::day: return duration_cast_switch2<duration::days>(fields, precision_to_val);
  case precision::hour: return duration_cast_switch2<duration::hours>(fields, precision_to_val);
  case precision::minute: return duration_cast_switch2<duration::minutes>(fields, precision_to_val);
  case precision::second: return duration_cast_switch2<duration::seconds>(fields, precision_to_val);
  case precision::millisecond: return duration_cast_switch2<duration::milliseconds>(fields, precision_to_val);
  case precision::microsecond: return duration_cast_switch2<duration::microseconds>(fields, precision_to_val);
  case precision::nanosecond: return duration_cast_switch2<duration::nanoseconds>(fields, precision_to_val);
  default: never_reached("duration_cast_switch");
  }
}

[[cpp11::register]]
cpp11::writable::list
duration_cast_cpp(cpp11::list_of<cpp11::doubles> fields,
                  const cpp11::integers& precision_from,
                  const cpp11::integers& precision_to) {
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
duration_arith_impl(cpp11::list_of<cpp11::doubles>& x_fields,
                    cpp11::list_of<cpp11::doubles>& y_fields,
                    const enum arith_op& op) {
  const ClockDuration x{x_fields};
  const ClockDuration y{y_fields};

  const r_ssize size = x.size();
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
    using Duration = typename ClockDuration::chrono_duration;

    for (r_ssize i = 0; i < size; ++i) {
      if (x.is_na(i) || y.is_na(i)) {
        out.assign_na(i);
        continue;
      }

      const Duration x_elt = x[i];
      const Duration y_elt = y[i];

      if (y_elt == Duration::zero()) {
        out.assign_na(i);
        continue;
      }

      out.assign(x_elt % y_elt, i);
    }
    break;
  }
  }

  return out.to_list();
}

static
inline
cpp11::writable::list
duration_arith(cpp11::list_of<cpp11::doubles>& x,
               cpp11::list_of<cpp11::doubles>& y,
               const cpp11::integers& precision_int,
               const enum arith_op& op) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_arith_impl<duration::years>(x, y, op);
  case precision::quarter: return duration_arith_impl<duration::quarters>(x, y, op);
  case precision::month: return duration_arith_impl<duration::months>(x, y, op);
  case precision::week: return duration_arith_impl<duration::weeks>(x, y, op);
  case precision::day: return duration_arith_impl<duration::days>(x, y, op);
  case precision::hour: return duration_arith_impl<duration::hours>(x, y, op);
  case precision::minute: return duration_arith_impl<duration::minutes>(x, y, op);
  case precision::second: return duration_arith_impl<duration::seconds>(x, y, op);
  case precision::millisecond: return duration_arith_impl<duration::milliseconds>(x, y, op);
  case precision::microsecond: return duration_arith_impl<duration::microseconds>(x, y, op);
  case precision::nanosecond: return duration_arith_impl<duration::nanoseconds>(x, y, op);
  default: never_reached("duration_arith");
  }
}

[[cpp11::register]]
cpp11::writable::list
duration_plus_cpp(cpp11::list_of<cpp11::doubles> x,
                  cpp11::list_of<cpp11::doubles> y,
                  const cpp11::integers& precision_int) {
  return duration_arith(x, y, precision_int, arith_op::plus);
}

[[cpp11::register]]
cpp11::writable::list
duration_minus_cpp(cpp11::list_of<cpp11::doubles> x,
                   cpp11::list_of<cpp11::doubles> y,
                   const cpp11::integers& precision_int) {
  return duration_arith(x, y, precision_int, arith_op::minus);
}

[[cpp11::register]]
cpp11::writable::list
duration_modulus_cpp(cpp11::list_of<cpp11::doubles> x,
                     cpp11::list_of<cpp11::doubles> y,
                     const cpp11::integers& precision_int) {
  return duration_arith(x, y, precision_int, arith_op::modulus);
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::integers
duration_integer_divide_impl(cpp11::list_of<cpp11::doubles>& x_fields,
                             cpp11::list_of<cpp11::doubles>& y_fields) {
  using Duration = typename ClockDuration::chrono_duration;
  using Rep = typename Duration::rep;

  const Rep REP_INT_MAX = static_cast<Rep>(std::numeric_limits<int>::max());
  const Rep REP_INT_MIN = static_cast<Rep>(std::numeric_limits<int>::min());

  const ClockDuration x{x_fields};
  const ClockDuration y{y_fields};

  const r_ssize size = x.size();

  cpp11::writable::integers out(size);

  bool warn = false;
  r_ssize loc = 0;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || y.is_na(i)) {
      out[i] = r_int_na;
      continue;
    }

    const Duration x_elt = x[i];
    const Duration y_elt = y[i];

    if (y_elt == Duration::zero()) {
      // Consistent with `2L %/% 0L` rather than `2 %/% 0` since infinite
      // durations aren't supported
      out[i] = r_int_na;
      continue;
    }

    const Rep elt = x_elt / y_elt;

    if (elt > REP_INT_MAX || elt <= REP_INT_MIN) {
      out[i] = r_int_na;

      if (!warn) {
        warn = true;
        loc = i + 1;
      }

      continue;
    }

    out[i] = static_cast<int>(elt);
  }

  if (warn) {
    cpp11::warning(
      "Conversion to integer is outside the range of an integer. "
      "`NA` values have been introduced, beginning at location %td.",
      (ptrdiff_t) loc
    );
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::integers
duration_integer_divide_cpp(cpp11::list_of<cpp11::doubles> x,
                            cpp11::list_of<cpp11::doubles> y,
                            const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_integer_divide_impl<duration::years>(x, y);
  case precision::quarter: return duration_integer_divide_impl<duration::quarters>(x, y);
  case precision::month: return duration_integer_divide_impl<duration::months>(x, y);
  case precision::week: return duration_integer_divide_impl<duration::weeks>(x, y);
  case precision::day: return duration_integer_divide_impl<duration::days>(x, y);
  case precision::hour: return duration_integer_divide_impl<duration::hours>(x, y);
  case precision::minute: return duration_integer_divide_impl<duration::minutes>(x, y);
  case precision::second: return duration_integer_divide_impl<duration::seconds>(x, y);
  case precision::millisecond: return duration_integer_divide_impl<duration::milliseconds>(x, y);
  case precision::microsecond: return duration_integer_divide_impl<duration::microseconds>(x, y);
  case precision::nanosecond: return duration_integer_divide_impl<duration::nanoseconds>(x, y);
  default: never_reached("duration_integer_divide_cpp");
  }
}

// -----------------------------------------------------------------------------

enum class arith_scalar_op {
  multiply,
  modulus,
  divide
};

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_scalar_arith_impl(cpp11::list_of<cpp11::doubles>& x_fields,
                           const cpp11::integers& y,
                           const enum arith_scalar_op& op) {
  const ClockDuration x{x_fields};

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
  case arith_scalar_op::modulus: {
    for (r_ssize i = 0; i < size; ++i) {
      const int elt_y = y[i];
      if (x.is_na(i) || elt_y == r_int_na || elt_y == 0) {
        out.assign_na(i);
        continue;
      }
      out.assign(x[i] % elt_y, i);
    }
    break;
  }
  case arith_scalar_op::divide: {
    for (r_ssize i = 0; i < size; ++i) {
      const int elt_y = y[i];
      if (x.is_na(i) || elt_y == r_int_na || elt_y == 0) {
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
duration_scalar_arith(cpp11::list_of<cpp11::doubles>& x,
                      const cpp11::integers& y,
                      const cpp11::integers& precision_int,
                      const enum arith_scalar_op& op) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_scalar_arith_impl<duration::years>(x, y, op);
  case precision::quarter: return duration_scalar_arith_impl<duration::quarters>(x, y, op);
  case precision::month: return duration_scalar_arith_impl<duration::months>(x, y, op);
  case precision::week: return duration_scalar_arith_impl<duration::weeks>(x, y, op);
  case precision::day: return duration_scalar_arith_impl<duration::days>(x, y, op);
  case precision::hour: return duration_scalar_arith_impl<duration::hours>(x, y, op);
  case precision::minute: return duration_scalar_arith_impl<duration::minutes>(x, y, op);
  case precision::second: return duration_scalar_arith_impl<duration::seconds>(x, y, op);
  case precision::millisecond: return duration_scalar_arith_impl<duration::milliseconds>(x, y, op);
  case precision::microsecond: return duration_scalar_arith_impl<duration::microseconds>(x, y, op);
  case precision::nanosecond: return duration_scalar_arith_impl<duration::nanoseconds>(x, y, op);
  default: never_reached("duration_scalar_arith");
  }
}

[[cpp11::register]]
cpp11::writable::list
duration_scalar_multiply_cpp(cpp11::list_of<cpp11::doubles> x,
                             const cpp11::integers& y,
                             const cpp11::integers& precision_int) {
  return duration_scalar_arith(x, y, precision_int, arith_scalar_op::multiply);
}

[[cpp11::register]]
cpp11::writable::list
duration_scalar_modulus_cpp(cpp11::list_of<cpp11::doubles> x,
                            const cpp11::integers& y,
                            const cpp11::integers& precision_int) {
  return duration_scalar_arith(x, y, precision_int, arith_scalar_op::modulus);
}

[[cpp11::register]]
cpp11::writable::list
duration_scalar_divide_cpp(cpp11::list_of<cpp11::doubles> x,
                           const cpp11::integers& y,
                           const cpp11::integers& precision_int) {
  return duration_scalar_arith(x, y, precision_int, arith_scalar_op::divide);
}

// -----------------------------------------------------------------------------

/*
 * Restricts normal result returned by `std::common_type()` to only allow
 * combinations of:
 *
 * Calendrical durations:
 * - year, quarter, month
 *
 * Chronological durations:
 * - week, day, hour, minute, second, microsecond, millisecond, nanosecond
 *
 * These two groups consist of durations that are intuitively defined relative
 * to each other. They also separate how durations are interpreted in clock,
 * the granular ones are calendrical (and are used with calendars), the precise
 * ones are chronological (and are used with time points).
 */
template <typename Duration1, typename Duration2>
inline
std::pair<enum precision, bool>
duration_common_precision_impl() {
  using CT = typename std::common_type<Duration1, Duration2>::type;

  const bool duration1_calendrical =
    std::is_same<Duration1, date::years>::value ||
    std::is_same<Duration1, quarterly::quarters>::value ||
    std::is_same<Duration1, date::months>::value;

  const bool duration2_calendrical =
    std::is_same<Duration2, date::years>::value ||
    std::is_same<Duration2, quarterly::quarters>::value ||
    std::is_same<Duration2, date::months>::value;

  // Duration combinations that cross the
  // calendrical/chronological boundary are invalid
  if (duration1_calendrical && !duration2_calendrical) {
    return std::make_pair(precision::year, false);
  }
  if (!duration1_calendrical && duration2_calendrical) {
    return std::make_pair(precision::year, false);
  }

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
    clock_abort("Internal error: Invalid combination of duration precisions.");
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
int
duration_precision_common_cpp(const cpp11::integers& x_precision,
                              const cpp11::integers& y_precision) {
  const enum precision x_precision_val = parse_precision(x_precision);
  const enum precision y_precision_val = parse_precision(y_precision);
  const std::pair<enum precision, bool> pair = duration_common_precision_pair(x_precision_val, y_precision_val);

  if (pair.second) {
    return static_cast<int>(pair.first);
  } else {
    return r_int_na;
  }
}

[[cpp11::register]]
bool
duration_has_common_precision_cpp(const cpp11::integers& x_precision,
                                  const cpp11::integers& y_precision) {
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

template <class ClockDurationFrom, class ClockDurationTo>
cpp11::writable::list
duration_rounding_impl(cpp11::list_of<cpp11::doubles>& fields,
                       const int& n,
                       const enum rounding& type) {
  using DurationFrom = typename ClockDurationFrom::chrono_duration;
  using DurationTo = typename ClockDurationTo::chrono_duration;

  const ClockDurationFrom x{fields};

  const r_ssize size = x.size();
  ClockDurationTo out(size);

  if (type == rounding::floor) {
    for (r_ssize i = 0; i < size; ++i) {
      if (x.is_na(i)) {
        out.assign_na(i);
        continue;
      }
      const DurationFrom from = x[i];
      const DurationTo to = clock_floor<DurationTo>(from, n);
      out.assign(to, i);
    }
  } else if (type == rounding::ceil) {
    for (r_ssize i = 0; i < size; ++i) {
      if (x.is_na(i)) {
        out.assign_na(i);
        continue;
      }
      const DurationFrom from = x[i];
      const DurationTo to = clock_ceil<DurationTo>(from, n);
      out.assign(to, i);
    }
  } else {
    for (r_ssize i = 0; i < size; ++i) {
      if (x.is_na(i)) {
        out.assign_na(i);
        continue;
      }
      const DurationFrom from = x[i];
      const DurationTo to = clock_round<DurationTo>(from, n);
      out.assign(to, i);
    }
  }

  return out.to_list();
}

inline
cpp11::writable::list
duration_rounding_switch(cpp11::list_of<cpp11::doubles>& fields,
                         const enum precision& precision_from_val,
                         const enum precision& precision_to_val,
                         const int& n,
                         const enum rounding& type) {
  using namespace rclock;

  switch (precision_from_val) {
  case precision::year: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::years, duration::years>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::quarter: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::quarters, duration::years>(fields, n, type);
    case precision::quarter: return duration_rounding_impl<duration::quarters, duration::quarters>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::month: {
    switch (precision_to_val) {
    case precision::year: return duration_rounding_impl<duration::months, duration::years>(fields, n, type);
    case precision::quarter: return duration_rounding_impl<duration::months, duration::quarters>(fields, n, type);
    case precision::month: return duration_rounding_impl<duration::months, duration::months>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::week: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::weeks, duration::weeks>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::day: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::days, duration::weeks>(fields, n, type);
    case precision::day: return duration_rounding_impl<duration::days, duration::days>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::hour: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::hours, duration::weeks>(fields, n, type);
    case precision::day: return duration_rounding_impl<duration::hours, duration::days>(fields, n, type);
    case precision::hour: return duration_rounding_impl<duration::hours, duration::hours>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::minute: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::minutes, duration::weeks>(fields, n, type);
    case precision::day: return duration_rounding_impl<duration::minutes, duration::days>(fields, n, type);
    case precision::hour: return duration_rounding_impl<duration::minutes, duration::hours>(fields, n, type);
    case precision::minute: return duration_rounding_impl<duration::minutes, duration::minutes>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::second: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::seconds, duration::weeks>(fields, n, type);
    case precision::day: return duration_rounding_impl<duration::seconds, duration::days>(fields, n, type);
    case precision::hour: return duration_rounding_impl<duration::seconds, duration::hours>(fields, n, type);
    case precision::minute: return duration_rounding_impl<duration::seconds, duration::minutes>(fields, n, type);
    case precision::second: return duration_rounding_impl<duration::seconds, duration::seconds>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::millisecond: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::milliseconds, duration::weeks>(fields, n, type);
    case precision::day: return duration_rounding_impl<duration::milliseconds, duration::days>(fields, n, type);
    case precision::hour: return duration_rounding_impl<duration::milliseconds, duration::hours>(fields, n, type);
    case precision::minute: return duration_rounding_impl<duration::milliseconds, duration::minutes>(fields, n, type);
    case precision::second: return duration_rounding_impl<duration::milliseconds, duration::seconds>(fields, n, type);
    case precision::millisecond: return duration_rounding_impl<duration::milliseconds, duration::milliseconds>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::microsecond: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::microseconds, duration::weeks>(fields, n, type);
    case precision::day: return duration_rounding_impl<duration::microseconds, duration::days>(fields, n, type);
    case precision::hour: return duration_rounding_impl<duration::microseconds, duration::hours>(fields, n, type);
    case precision::minute: return duration_rounding_impl<duration::microseconds, duration::minutes>(fields, n, type);
    case precision::second: return duration_rounding_impl<duration::microseconds, duration::seconds>(fields, n, type);
    case precision::millisecond: return duration_rounding_impl<duration::microseconds, duration::milliseconds>(fields, n, type);
    case precision::microsecond: return duration_rounding_impl<duration::microseconds, duration::microseconds>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::nanosecond: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::nanoseconds, duration::weeks>(fields, n, type);
    case precision::day: return duration_rounding_impl<duration::nanoseconds, duration::days>(fields, n, type);
    case precision::hour: return duration_rounding_impl<duration::nanoseconds, duration::hours>(fields, n, type);
    case precision::minute: return duration_rounding_impl<duration::nanoseconds, duration::minutes>(fields, n, type);
    case precision::second: return duration_rounding_impl<duration::nanoseconds, duration::seconds>(fields, n, type);
    case precision::millisecond: return duration_rounding_impl<duration::nanoseconds, duration::milliseconds>(fields, n, type);
    case precision::microsecond: return duration_rounding_impl<duration::nanoseconds, duration::microseconds>(fields, n, type);
    case precision::nanosecond: return duration_rounding_impl<duration::nanoseconds, duration::nanoseconds>(fields, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  }

  never_reached("duration_rounding_switch");
}

[[cpp11::register]]
cpp11::writable::list
duration_floor_cpp(cpp11::list_of<cpp11::doubles> fields,
                   const cpp11::integers& precision_from,
                   const cpp11::integers& precision_to,
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
duration_ceiling_cpp(cpp11::list_of<cpp11::doubles> fields,
                     const cpp11::integers& precision_from,
                     const cpp11::integers& precision_to,
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
duration_round_cpp(cpp11::list_of<cpp11::doubles> fields,
                   const cpp11::integers& precision_from,
                   const cpp11::integers& precision_to,
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
duration_unary_minus_impl(cpp11::list_of<cpp11::doubles>& fields) {
  const ClockDuration x{fields};

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
duration_unary_minus_cpp(cpp11::list_of<cpp11::doubles> fields,
                         const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_unary_minus_impl<duration::years>(fields);
  case precision::quarter: return duration_unary_minus_impl<duration::quarters>(fields);
  case precision::month: return duration_unary_minus_impl<duration::months>(fields);
  case precision::week: return duration_unary_minus_impl<duration::weeks>(fields);
  case precision::day: return duration_unary_minus_impl<duration::days>(fields);
  case precision::hour: return duration_unary_minus_impl<duration::hours>(fields);
  case precision::minute: return duration_unary_minus_impl<duration::minutes>(fields);
  case precision::second: return duration_unary_minus_impl<duration::seconds>(fields);
  case precision::millisecond: return duration_unary_minus_impl<duration::milliseconds>(fields);
  case precision::microsecond: return duration_unary_minus_impl<duration::microseconds>(fields);
  case precision::nanosecond: return duration_unary_minus_impl<duration::nanoseconds>(fields);
  default: never_reached("duration_unary_minus_cpp");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::integers
duration_as_integer_impl(cpp11::list_of<cpp11::doubles>& fields) {
  using Duration = typename ClockDuration::chrono_duration;
  using Rep = typename Duration::rep;

  const ClockDuration x{fields};
  const r_ssize size = x.size();

  cpp11::writable::integers out(size);

  bool warn = false;
  r_ssize loc = 0;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_int_na;
      continue;
    }

    const Duration elt = x[i];
    const Rep elt_rep = elt.count();

    if (elt_rep > INT32_MAX || elt_rep <= INT32_MIN) {
      out[i] = r_int_na;
      if (!warn) {
        loc = i + 1;
      }
      warn = true;
      continue;
    }

    out[i] = static_cast<int>(elt_rep);
  }

  if (warn) {
    cpp11::warning(
      "Conversion from duration to integer is outside the range of an integer. "
      "`NA` values have been introduced, beginning at location %td.",
      (ptrdiff_t) loc
    );
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::integers
duration_as_integer_cpp(cpp11::list_of<cpp11::doubles> fields,
                        const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_as_integer_impl<duration::years>(fields);
  case precision::quarter: return duration_as_integer_impl<duration::quarters>(fields);
  case precision::month: return duration_as_integer_impl<duration::months>(fields);
  case precision::week: return duration_as_integer_impl<duration::weeks>(fields);
  case precision::day: return duration_as_integer_impl<duration::days>(fields);
  case precision::hour: return duration_as_integer_impl<duration::hours>(fields);
  case precision::minute: return duration_as_integer_impl<duration::minutes>(fields);
  case precision::second: return duration_as_integer_impl<duration::seconds>(fields);
  case precision::millisecond: return duration_as_integer_impl<duration::milliseconds>(fields);
  case precision::microsecond: return duration_as_integer_impl<duration::microseconds>(fields);
  case precision::nanosecond: return duration_as_integer_impl<duration::nanoseconds>(fields);
  default: never_reached("duration_as_integer_cpp");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::doubles
duration_as_double_impl(cpp11::list_of<cpp11::doubles>& fields) {
  using Duration = typename ClockDuration::chrono_duration;
  using Rep = typename Duration::rep;

  // Usually 2^53 - 1
  // Pass `double`s to `pow()` for Solaris, where `pow(int, int)` is undefined
  static double DOUBLE_FLT_RADIX = static_cast<double>(FLT_RADIX);
  static double DOUBLE_DBL_MANT_DIG = static_cast<double>(DBL_MANT_DIG);
  static int64_t DOUBLE_MAX_NO_LOSS = static_cast<int64_t>(std::pow(DOUBLE_FLT_RADIX, DOUBLE_DBL_MANT_DIG) - 1);
  static int64_t DOUBLE_MIN_NO_LOSS = -DOUBLE_MAX_NO_LOSS;

  const ClockDuration x{fields};
  const r_ssize size = x.size();
  cpp11::writable::doubles out(size);

  bool warn = false;
  r_ssize loc = 0;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_dbl_na;
      continue;
    }

    const Duration elt = x[i];
    const Rep elt_rep = elt.count();

    if (elt_rep > DOUBLE_MAX_NO_LOSS || elt_rep < DOUBLE_MIN_NO_LOSS) {
      if (!warn) {
        loc = i + 1;
      }
      warn = true;
    }

    out[i] = static_cast<double>(elt_rep);
  }

  if (warn) {
    cpp11::warning(
      "Conversion from duration to double is outside the range of lossless conversion. "
      "Precision may have been lost, beginning at location %td.",
      (ptrdiff_t) loc
    );
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::doubles
duration_as_double_cpp(cpp11::list_of<cpp11::doubles> fields,
                       const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_as_double_impl<duration::years>(fields);
  case precision::quarter: return duration_as_double_impl<duration::quarters>(fields);
  case precision::month: return duration_as_double_impl<duration::months>(fields);
  case precision::week: return duration_as_double_impl<duration::weeks>(fields);
  case precision::day: return duration_as_double_impl<duration::days>(fields);
  case precision::hour: return duration_as_double_impl<duration::hours>(fields);
  case precision::minute: return duration_as_double_impl<duration::minutes>(fields);
  case precision::second: return duration_as_double_impl<duration::seconds>(fields);
  case precision::millisecond: return duration_as_double_impl<duration::milliseconds>(fields);
  case precision::microsecond: return duration_as_double_impl<duration::microseconds>(fields);
  case precision::nanosecond: return duration_as_double_impl<duration::nanoseconds>(fields);
  default: never_reached("duration_as_double_cpp");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_abs_impl(cpp11::list_of<cpp11::doubles>& fields) {
  using Duration = typename ClockDuration::chrono_duration;
  using Rep = typename Duration::rep;

  const ClockDuration x{fields};
  const r_ssize size = x.size();
  ClockDuration out(size);

  const Rep zero{0};

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
      continue;
    }

    const Duration elt = x[i];
    const Rep elt_rep = elt.count();
    const Rep out_rep = (elt_rep < zero) ? std::abs(elt_rep) : elt_rep;
    const Duration out_elt{out_rep};

    out.assign(out_elt, i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
duration_abs_cpp(cpp11::list_of<cpp11::doubles> fields,
                 const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_abs_impl<duration::years>(fields);
  case precision::quarter: return duration_abs_impl<duration::quarters>(fields);
  case precision::month: return duration_abs_impl<duration::months>(fields);
  case precision::week: return duration_abs_impl<duration::weeks>(fields);
  case precision::day: return duration_abs_impl<duration::days>(fields);
  case precision::hour: return duration_abs_impl<duration::hours>(fields);
  case precision::minute: return duration_abs_impl<duration::minutes>(fields);
  case precision::second: return duration_abs_impl<duration::seconds>(fields);
  case precision::millisecond: return duration_abs_impl<duration::milliseconds>(fields);
  case precision::microsecond: return duration_abs_impl<duration::microseconds>(fields);
  case precision::nanosecond: return duration_abs_impl<duration::nanoseconds>(fields);
  default: never_reached("duration_abs_cpp");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::integers
duration_sign_impl(cpp11::list_of<cpp11::doubles>& fields) {
  using Duration = typename ClockDuration::chrono_duration;
  using Rep = typename Duration::rep;

  const ClockDuration x{fields};
  const r_ssize size = x.size();
  cpp11::writable::integers out(size);

  const Rep zero{0};

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_int_na;
      continue;
    }

    const Duration elt = x[i];
    const Rep elt_rep = elt.count();

    if (elt_rep == zero) {
      out[i] = 0;
    } else if (elt_rep > zero) {
      out[i] = 1;
    } else {
      out[i] = -1;
    }
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::integers
duration_sign_cpp(cpp11::list_of<cpp11::doubles> fields,
                  const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_sign_impl<duration::years>(fields);
  case precision::quarter: return duration_sign_impl<duration::quarters>(fields);
  case precision::month: return duration_sign_impl<duration::months>(fields);
  case precision::week: return duration_sign_impl<duration::weeks>(fields);
  case precision::day: return duration_sign_impl<duration::days>(fields);
  case precision::hour: return duration_sign_impl<duration::hours>(fields);
  case precision::minute: return duration_sign_impl<duration::minutes>(fields);
  case precision::second: return duration_sign_impl<duration::seconds>(fields);
  case precision::millisecond: return duration_sign_impl<duration::milliseconds>(fields);
  case precision::microsecond: return duration_sign_impl<duration::microseconds>(fields);
  case precision::nanosecond: return duration_sign_impl<duration::nanoseconds>(fields);
  default: never_reached("duration_sign_cpp");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_seq_by_lo_impl(cpp11::list_of<cpp11::doubles>& from_fields,
                        cpp11::list_of<cpp11::doubles>& by_fields,
                        const r_ssize size) {
  using Duration = typename ClockDuration::chrono_duration;

  const ClockDuration from{from_fields};
  const ClockDuration by{by_fields};

  ClockDuration out(size);

  const Duration start = from[0];
  const Duration step = by[0];

  for (r_ssize i = 0; i < size; ++i) {
    const Duration elt = start + step * i;
    out.assign(elt, i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
duration_seq_by_lo_cpp(cpp11::list_of<cpp11::doubles> from,
                       const cpp11::integers& precision_int,
                       cpp11::list_of<cpp11::doubles> by,
                       const cpp11::integers& length_out) {
  using namespace rclock;

  if (length_out.size() != 1) {
    clock_abort("Internal error: `length_out` should have size 1.");
  }
  const r_ssize size = length_out[0];

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_seq_by_lo_impl<duration::years>(from, by, size);
  case precision::quarter: return duration_seq_by_lo_impl<duration::quarters>(from, by, size);
  case precision::month: return duration_seq_by_lo_impl<duration::months>(from, by, size);
  case precision::week: return duration_seq_by_lo_impl<duration::weeks>(from, by, size);
  case precision::day: return duration_seq_by_lo_impl<duration::days>(from, by, size);
  case precision::hour: return duration_seq_by_lo_impl<duration::hours>(from, by, size);
  case precision::minute: return duration_seq_by_lo_impl<duration::minutes>(from, by, size);
  case precision::second: return duration_seq_by_lo_impl<duration::seconds>(from, by, size);
  case precision::millisecond: return duration_seq_by_lo_impl<duration::milliseconds>(from, by, size);
  case precision::microsecond: return duration_seq_by_lo_impl<duration::microseconds>(from, by, size);
  case precision::nanosecond: return duration_seq_by_lo_impl<duration::nanoseconds>(from, by, size);
  default: never_reached("duration_seq_by_lo_cpp");
  }
}

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_seq_to_by_impl(cpp11::list_of<cpp11::doubles>& from_fields,
                        cpp11::list_of<cpp11::doubles>& to_fields,
                        cpp11::list_of<cpp11::doubles>& by_fields) {
  using Duration = typename ClockDuration::chrono_duration;
  using Rep = typename Duration::rep;

  const ClockDuration from{from_fields};
  const ClockDuration to{to_fields};
  const ClockDuration by{by_fields};

  const Duration start = from[0];
  const Duration end = to[0];
  const Duration step = by[0];

  // To match `rlang::seq2()`, which has nice mathematical properties of
  // returning an empty sequence when `start > end`
  const bool is_empty =
    (step > Duration::zero() && start > end) ||
    (step < Duration::zero() && start < end);

  r_ssize size;

  if (is_empty) {
    size = 0;
  } else {
    const Rep num = clock_safe_subtract(end.count(), start.count());
    const Rep den = step.count();
    size = static_cast<r_ssize>(num / den + 1);
  }

  ClockDuration out(size);

  for (r_ssize i = 0; i < size; ++i) {
    const Duration elt = start + step * i;
    out.assign(elt, i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
duration_seq_to_by_cpp(cpp11::list_of<cpp11::doubles> from,
                       const cpp11::integers& precision_int,
                       cpp11::list_of<cpp11::doubles> to,
                       cpp11::list_of<cpp11::doubles> by) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_seq_to_by_impl<duration::years>(from, to, by);
  case precision::quarter: return duration_seq_to_by_impl<duration::quarters>(from, to, by);
  case precision::month: return duration_seq_to_by_impl<duration::months>(from, to, by);
  case precision::week: return duration_seq_to_by_impl<duration::weeks>(from, to, by);
  case precision::day: return duration_seq_to_by_impl<duration::days>(from, to, by);
  case precision::hour: return duration_seq_to_by_impl<duration::hours>(from, to, by);
  case precision::minute: return duration_seq_to_by_impl<duration::minutes>(from, to, by);
  case precision::second: return duration_seq_to_by_impl<duration::seconds>(from, to, by);
  case precision::millisecond: return duration_seq_to_by_impl<duration::milliseconds>(from, to, by);
  case precision::microsecond: return duration_seq_to_by_impl<duration::microseconds>(from, to, by);
  case precision::nanosecond: return duration_seq_to_by_impl<duration::nanoseconds>(from, to, by);
  default: never_reached("duration_seq_to_by_cpp");
  }
}

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_seq_to_lo_impl(cpp11::list_of<cpp11::doubles>& from_fields,
                        cpp11::list_of<cpp11::doubles>& to_fields,
                        const r_ssize& size) {
  using Duration = typename ClockDuration::chrono_duration;
  using Rep = typename Duration::rep;

  const ClockDuration from{from_fields};
  const ClockDuration to{to_fields};

  ClockDuration out(size);

  const Duration start = from[0];
  const Duration end = to[0];

  if (size == 1) {
    // Avoid division by zero
    out.assign(start, 0);
    return out.to_list();
  }

  const Rep num = end.count() - start.count();
  const Rep den = static_cast<Rep>(size - 1);

  const Rep by = num / den;
  const Rep rem = num % den;

  if (rem != Rep{0}) {
    clock_abort(
      "The supplied output size does not result in a non-fractional "
      "sequence between `from` and `to`."
    );
  }

  const Duration step{by};

  for (r_ssize i = 0; i < size; ++i) {
    const Duration elt = start + step * i;
    out.assign(elt, i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
duration_seq_to_lo_cpp(cpp11::list_of<cpp11::doubles> from,
                       const cpp11::integers& precision_int,
                       cpp11::list_of<cpp11::doubles> to,
                       const cpp11::integers& length_out) {
  using namespace rclock;

  if (length_out.size() != 1) {
    clock_abort("Internal error: `length_out` should have size 1.");
  }
  const r_ssize size = length_out[0];

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_seq_to_lo_impl<duration::years>(from, to, size);
  case precision::quarter: return duration_seq_to_lo_impl<duration::quarters>(from, to, size);
  case precision::month: return duration_seq_to_lo_impl<duration::months>(from, to, size);
  case precision::week: return duration_seq_to_lo_impl<duration::weeks>(from, to, size);
  case precision::day: return duration_seq_to_lo_impl<duration::days>(from, to, size);
  case precision::hour: return duration_seq_to_lo_impl<duration::hours>(from, to, size);
  case precision::minute: return duration_seq_to_lo_impl<duration::minutes>(from, to, size);
  case precision::second: return duration_seq_to_lo_impl<duration::seconds>(from, to, size);
  case precision::millisecond: return duration_seq_to_lo_impl<duration::milliseconds>(from, to, size);
  case precision::microsecond: return duration_seq_to_lo_impl<duration::microseconds>(from, to, size);
  case precision::nanosecond: return duration_seq_to_lo_impl<duration::nanoseconds>(from, to, size);
  default: never_reached("duration_seq_to_lo_cpp");
  }
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_minimum_impl() {
  using Duration = typename ClockDuration::chrono_duration;

  ClockDuration out(1);
  out.assign(Duration::min(), 0);

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
duration_minimum_cpp(const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_minimum_impl<duration::years>();
  case precision::quarter: return duration_minimum_impl<duration::quarters>();
  case precision::month: return duration_minimum_impl<duration::months>();
  case precision::week: return duration_minimum_impl<duration::weeks>();
  case precision::day: return duration_minimum_impl<duration::days>();
  case precision::hour: return duration_minimum_impl<duration::hours>();
  case precision::minute: return duration_minimum_impl<duration::minutes>();
  case precision::second: return duration_minimum_impl<duration::seconds>();
  case precision::millisecond: return duration_minimum_impl<duration::milliseconds>();
  case precision::microsecond: return duration_minimum_impl<duration::microseconds>();
  case precision::nanosecond: return duration_minimum_impl<duration::nanoseconds>();
  default: never_reached("duration_minimum_cpp");
  }
}

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_maximum_impl() {
  using Duration = typename ClockDuration::chrono_duration;

  ClockDuration out(1);
  out.assign(Duration::max(), 0);

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
duration_maximum_cpp(const cpp11::integers& precision_int) {
  using namespace rclock;

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_maximum_impl<duration::years>();
  case precision::quarter: return duration_maximum_impl<duration::quarters>();
  case precision::month: return duration_maximum_impl<duration::months>();
  case precision::week: return duration_maximum_impl<duration::weeks>();
  case precision::day: return duration_maximum_impl<duration::days>();
  case precision::hour: return duration_maximum_impl<duration::hours>();
  case precision::minute: return duration_maximum_impl<duration::minutes>();
  case precision::second: return duration_maximum_impl<duration::seconds>();
  case precision::millisecond: return duration_maximum_impl<duration::milliseconds>();
  case precision::microsecond: return duration_maximum_impl<duration::microseconds>();
  case precision::nanosecond: return duration_maximum_impl<duration::nanoseconds>();
  default: never_reached("duration_maximum_cpp");
  }
}
