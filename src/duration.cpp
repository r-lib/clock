#include "clock.h"
#include "utils.h"
#include "duration.h"
#include "enums.h"
#include "get.h"
#include "rcrd.h"
#include <sstream>
#include <cfloat>
#include <algorithm>

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
new_duration_from_fields(SEXP fields,
                         const cpp11::integers& precision_int,
                         SEXP names) {
  const r_ssize n_fields = Rf_xlength(fields);
  const enum precision& precision_val = parse_precision(precision_int);

  switch (precision_val) {
  case precision::year:
  case precision::quarter:
  case precision::month:
  case precision::week:
  case precision::day: {
    if (n_fields != 1) {
      clock_abort("`fields` must have 1 field for [year, day] precision.");
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
    never_reached("new_duration_from_fields");
  }
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

    SET_STRING_ELT(out, i, Rf_mkCharLenCE(string.c_str(), string.size(), CE_UTF8));
  }

  return out;
}

[[cpp11::register]]
cpp11::writable::strings format_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                             const cpp11::integers& precision_int) {
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

  switch (parse_precision(precision_int)) {
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
               const cpp11::integers& precision_int,
               const enum arith_op& op) {
  const enum precision precision_val = parse_precision(precision_int);

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
                  const cpp11::integers& precision_int) {
  return duration_arith(x, y, precision_int, arith_op::plus);
}

[[cpp11::register]]
cpp11::writable::list
duration_minus_cpp(cpp11::list_of<cpp11::integers> x,
                   cpp11::list_of<cpp11::integers> y,
                   const cpp11::integers& precision_int) {
  return duration_arith(x, y, precision_int, arith_op::minus);
}

[[cpp11::register]]
cpp11::writable::list
duration_modulus_cpp(cpp11::list_of<cpp11::integers> x,
                     cpp11::list_of<cpp11::integers> y,
                     const cpp11::integers& precision_int) {
  return duration_arith(x, y, precision_int, arith_op::modulus);
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::integers
duration_integer_divide_impl(const ClockDuration& x, const ClockDuration& y) {
  using Duration = typename ClockDuration::duration;
  using Rep = typename Duration::rep;

  const Rep REP_INT_MAX = static_cast<Rep>(INT_MAX);
  const Rep REP_INT_MIN = static_cast<Rep>(INT_MIN);

  const r_ssize size = x.size();

  cpp11::writable::integers out(size);

  bool warn = false;
  r_ssize loc = 0;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || y.is_na(i)) {
      out[i] = r_int_na;
      continue;
    }

    const Rep elt = x[i] / y[i];

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
duration_integer_divide_cpp(cpp11::list_of<cpp11::integers> x,
                            cpp11::list_of<cpp11::integers> y,
                            const cpp11::integers& precision_int) {
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

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_integer_divide_impl(x_dy, y_dy);
  case precision::quarter: return duration_integer_divide_impl(x_dq, y_dq);
  case precision::month: return duration_integer_divide_impl(x_dm, y_dm);
  case precision::week: return duration_integer_divide_impl(x_dw, y_dw);
  case precision::day: return duration_integer_divide_impl(x_dd, y_dd);
  case precision::hour: return duration_integer_divide_impl(x_dh, y_dh);
  case precision::minute: return duration_integer_divide_impl(x_dmin, y_dmin);
  case precision::second: return duration_integer_divide_impl(x_ds, y_ds);
  case precision::millisecond: return duration_integer_divide_impl(x_dmilli, y_dmilli);
  case precision::microsecond: return duration_integer_divide_impl(x_dmicro, y_dmicro);
  case precision::nanosecond: return duration_integer_divide_impl(x_dnano, y_dnano);
  }

  never_reached("duration_integer_divide_cpp");
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
  case arith_scalar_op::modulus: {
    for (r_ssize i = 0; i < size; ++i) {
      const int elt_y = y[i];
      if (x.is_na(i) || elt_y == r_int_na) {
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
                      const cpp11::integers& precision_int,
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

  switch (parse_precision(precision_int)) {
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
                             const cpp11::integers& precision_int) {
  return duration_scalar_arith(x, y, precision_int, arith_scalar_op::multiply);
}

[[cpp11::register]]
cpp11::writable::list
duration_scalar_modulus_cpp(cpp11::list_of<cpp11::integers> x,
                            const cpp11::integers& y,
                            const cpp11::integers& precision_int) {
  return duration_scalar_arith(x, y, precision_int, arith_scalar_op::modulus);
}

[[cpp11::register]]
cpp11::writable::list
duration_scalar_divide_cpp(cpp11::list_of<cpp11::integers> x,
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
    case precision::week: return duration_rounding_impl<duration::weeks>(dw, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::day: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::weeks>(dd, n, type);
    case precision::day: return duration_rounding_impl<duration::days>(dd, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::hour: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::weeks>(dh, n, type);
    case precision::day: return duration_rounding_impl<duration::days>(dh, n, type);
    case precision::hour: return duration_rounding_impl<duration::hours>(dh, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::minute: {
    switch (precision_to_val) {
    case precision::week: return duration_rounding_impl<duration::weeks>(dmin, n, type);
    case precision::day: return duration_rounding_impl<duration::days>(dmin, n, type);
    case precision::hour: return duration_rounding_impl<duration::hours>(dmin, n, type);
    case precision::minute: return duration_rounding_impl<duration::minutes>(dmin, n, type);
    default: clock_abort("Internal error: Invalid precision combination.");
    }
  }
  case precision::second: {
    switch (precision_to_val) {
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
duration_ceiling_cpp(cpp11::list_of<cpp11::integers> fields,
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
duration_round_cpp(cpp11::list_of<cpp11::integers> fields,
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
duration_unary_minus_cpp(cpp11::list_of<cpp11::integers> fields,
                         const cpp11::integers& precision_int) {
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

  switch (parse_precision(precision_int)) {
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

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::integers
duration_as_integer_impl(const ClockDuration& x) {
  const r_ssize size = x.size();
  cpp11::writable::integers out(size);

  using Duration = typename ClockDuration::duration;
  using Rep = typename Duration::rep;

  bool warn = false;
  r_ssize loc = 0;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_int_na;
      continue;
    }

    Duration elt = x[i];
    Rep elt_rep = elt.count();

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
duration_as_integer_cpp(cpp11::list_of<cpp11::integers> fields,
                        const cpp11::integers& precision_int) {
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

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_as_integer_impl(dy);
  case precision::quarter: return duration_as_integer_impl(dq);
  case precision::month: return duration_as_integer_impl(dm);
  case precision::week: return duration_as_integer_impl(dw);
  case precision::day: return duration_as_integer_impl(dd);
  case precision::hour: return duration_as_integer_impl(dh);
  case precision::minute: return duration_as_integer_impl(dmin);
  case precision::second: return duration_as_integer_impl(ds);
  case precision::millisecond: return duration_as_integer_impl(dmilli);
  case precision::microsecond: return duration_as_integer_impl(dmicro);
  case precision::nanosecond: return duration_as_integer_impl(dnano);
  }

  never_reached("duration_as_integer_cpp");
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::doubles
duration_as_double_impl(const ClockDuration& x) {
  // Usually 2^53 - 1
  // Pass `double`s to `pow()` for Solaris, where `pow(int, int)` is undefined
  static double DOUBLE_FLT_RADIX = static_cast<double>(FLT_RADIX);
  static double DOUBLE_DBL_MANT_DIG = static_cast<double>(DBL_MANT_DIG);
  static int64_t DOUBLE_MAX_NO_LOSS = static_cast<int64_t>(std::pow(DOUBLE_FLT_RADIX, DOUBLE_DBL_MANT_DIG) - 1);
  static int64_t DOUBLE_MIN_NO_LOSS = -DOUBLE_MAX_NO_LOSS;

  const r_ssize size = x.size();
  cpp11::writable::doubles out(size);

  using Duration = typename ClockDuration::duration;
  using Rep = typename Duration::rep;

  bool warn = false;
  r_ssize loc = 0;

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_dbl_na;
      continue;
    }

    Duration elt = x[i];
    Rep elt_rep = elt.count();

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
duration_as_double_cpp(cpp11::list_of<cpp11::integers> fields,
                       const cpp11::integers& precision_int) {
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

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_as_double_impl(dy);
  case precision::quarter: return duration_as_double_impl(dq);
  case precision::month: return duration_as_double_impl(dm);
  case precision::week: return duration_as_double_impl(dw);
  case precision::day: return duration_as_double_impl(dd);
  case precision::hour: return duration_as_double_impl(dh);
  case precision::minute: return duration_as_double_impl(dmin);
  case precision::second: return duration_as_double_impl(ds);
  case precision::millisecond: return duration_as_double_impl(dmilli);
  case precision::microsecond: return duration_as_double_impl(dmicro);
  case precision::nanosecond: return duration_as_double_impl(dnano);
  }

  never_reached("duration_as_double_cpp");
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_abs_impl(const ClockDuration& x) {
  using Duration = typename ClockDuration::duration;
  using Rep = typename Duration::rep;

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
duration_abs_cpp(cpp11::list_of<cpp11::integers> fields,
                 const cpp11::integers& precision_int) {
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

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_abs_impl(dy);
  case precision::quarter: return duration_abs_impl(dq);
  case precision::month: return duration_abs_impl(dm);
  case precision::week: return duration_abs_impl(dw);
  case precision::day: return duration_abs_impl(dd);
  case precision::hour: return duration_abs_impl(dh);
  case precision::minute: return duration_abs_impl(dmin);
  case precision::second: return duration_abs_impl(ds);
  case precision::millisecond: return duration_abs_impl(dmilli);
  case precision::microsecond: return duration_abs_impl(dmicro);
  case precision::nanosecond: return duration_abs_impl(dnano);
  }

  never_reached("duration_abs_cpp");
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::integers
duration_sign_impl(const ClockDuration& x) {
  using Duration = typename ClockDuration::duration;
  using Rep = typename Duration::rep;

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
duration_sign_cpp(cpp11::list_of<cpp11::integers> fields,
                  const cpp11::integers& precision_int) {
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

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_sign_impl(dy);
  case precision::quarter: return duration_sign_impl(dq);
  case precision::month: return duration_sign_impl(dm);
  case precision::week: return duration_sign_impl(dw);
  case precision::day: return duration_sign_impl(dd);
  case precision::hour: return duration_sign_impl(dh);
  case precision::minute: return duration_sign_impl(dmin);
  case precision::second: return duration_sign_impl(ds);
  case precision::millisecond: return duration_sign_impl(dmilli);
  case precision::microsecond: return duration_sign_impl(dmicro);
  case precision::nanosecond: return duration_sign_impl(dnano);
  }

  never_reached("duration_sign_impl");
}

// -----------------------------------------------------------------------------

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_seq_by_lo_impl(const ClockDuration& from,
                        const ClockDuration& by,
                        const r_ssize size) {
  ClockDuration out(size);

  using Duration = typename ClockDuration::duration;

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
duration_seq_by_lo_cpp(cpp11::list_of<cpp11::integers> from,
                       const cpp11::integers& precision_int,
                       cpp11::list_of<cpp11::integers> by,
                       const cpp11::integers& length_out) {
  using namespace rclock;

  const cpp11::integers from_ticks = duration::get_ticks(from);
  const cpp11::integers from_ticks_of_day = duration::get_ticks_of_day(from);
  const cpp11::integers from_ticks_of_second = duration::get_ticks_of_second(from);

  const duration::years from_dy{from_ticks};
  const duration::quarters from_dq{from_ticks};
  const duration::months from_dm{from_ticks};
  const duration::weeks from_dw{from_ticks};
  const duration::days from_dd{from_ticks};
  const duration::hours from_dh{from_ticks, from_ticks_of_day};
  const duration::minutes from_dmin{from_ticks, from_ticks_of_day};
  const duration::seconds from_ds{from_ticks, from_ticks_of_day};
  const duration::milliseconds from_dmilli{from_ticks, from_ticks_of_day, from_ticks_of_second};
  const duration::microseconds from_dmicro{from_ticks, from_ticks_of_day, from_ticks_of_second};
  const duration::nanoseconds from_dnano{from_ticks, from_ticks_of_day, from_ticks_of_second};

  const cpp11::integers by_ticks = duration::get_ticks(by);
  const cpp11::integers by_ticks_of_day = duration::get_ticks_of_day(by);
  const cpp11::integers by_ticks_of_second = duration::get_ticks_of_second(by);

  const duration::years by_dy{by_ticks};
  const duration::quarters by_dq{by_ticks};
  const duration::months by_dm{by_ticks};
  const duration::weeks by_dw{by_ticks};
  const duration::days by_dd{by_ticks};
  const duration::hours by_dh{by_ticks, by_ticks_of_day};
  const duration::minutes by_dmin{by_ticks, by_ticks_of_day};
  const duration::seconds by_ds{by_ticks, by_ticks_of_day};
  const duration::milliseconds by_dmilli{by_ticks, by_ticks_of_day, by_ticks_of_second};
  const duration::microseconds by_dmicro{by_ticks, by_ticks_of_day, by_ticks_of_second};
  const duration::nanoseconds by_dnano{by_ticks, by_ticks_of_day, by_ticks_of_second};

  if (length_out.size() != 1) {
    clock_abort("Internal error: `length_out` should have size 1.");
  }

  const r_ssize size = length_out[0];

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_seq_by_lo_impl(from_dy, by_dy, size);
  case precision::quarter: return duration_seq_by_lo_impl(from_dq, by_dq, size);
  case precision::month: return duration_seq_by_lo_impl(from_dm, by_dm, size);
  case precision::week: return duration_seq_by_lo_impl(from_dw, by_dw, size);
  case precision::day: return duration_seq_by_lo_impl(from_dd, by_dd, size);
  case precision::hour: return duration_seq_by_lo_impl(from_dh, by_dh, size);
  case precision::minute: return duration_seq_by_lo_impl(from_dmin, by_dmin, size);
  case precision::second: return duration_seq_by_lo_impl(from_ds, by_ds, size);
  case precision::millisecond: return duration_seq_by_lo_impl(from_dmilli, by_dmilli, size);
  case precision::microsecond: return duration_seq_by_lo_impl(from_dmicro, by_dmicro, size);
  case precision::nanosecond: return duration_seq_by_lo_impl(from_dnano, by_dnano, size);
  }

  never_reached("duration_seq_by_lo_cpp");
}

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_seq_to_by_impl(const ClockDuration& from,
                        const ClockDuration& to,
                        const ClockDuration& by) {
  using Duration = typename ClockDuration::duration;
  using Rep = typename Duration::rep;

  const Duration start = from[0];
  const Duration end = to[0];
  const Duration step = by[0];

  const Rep num = clock_safe_subtract(end.count(), start.count());
  const Rep den = step.count();
  const Rep length_out = num / den + 1;

  // To match `rlang::seq2()`, which has nicer properties.
  // i.e., when `start > end && step > 0` or `start < end && step < 0`.
  const Rep length_out2 = std::max(length_out, Rep{0});

  const r_ssize size = static_cast<r_ssize>(length_out2);

  ClockDuration out(size);

  for (r_ssize i = 0; i < size; ++i) {
    const Duration elt = start + step * i;
    out.assign(elt, i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
duration_seq_to_by_cpp(cpp11::list_of<cpp11::integers> from,
                       const cpp11::integers& precision_int,
                       cpp11::list_of<cpp11::integers> to,
                       cpp11::list_of<cpp11::integers> by) {
  using namespace rclock;

  const cpp11::integers from_ticks = duration::get_ticks(from);
  const cpp11::integers from_ticks_of_day = duration::get_ticks_of_day(from);
  const cpp11::integers from_ticks_of_second = duration::get_ticks_of_second(from);

  const duration::years from_dy{from_ticks};
  const duration::quarters from_dq{from_ticks};
  const duration::months from_dm{from_ticks};
  const duration::weeks from_dw{from_ticks};
  const duration::days from_dd{from_ticks};
  const duration::hours from_dh{from_ticks, from_ticks_of_day};
  const duration::minutes from_dmin{from_ticks, from_ticks_of_day};
  const duration::seconds from_ds{from_ticks, from_ticks_of_day};
  const duration::milliseconds from_dmilli{from_ticks, from_ticks_of_day, from_ticks_of_second};
  const duration::microseconds from_dmicro{from_ticks, from_ticks_of_day, from_ticks_of_second};
  const duration::nanoseconds from_dnano{from_ticks, from_ticks_of_day, from_ticks_of_second};

  const cpp11::integers to_ticks = duration::get_ticks(to);
  const cpp11::integers to_ticks_of_day = duration::get_ticks_of_day(to);
  const cpp11::integers to_ticks_of_second = duration::get_ticks_of_second(to);

  const duration::years to_dy{to_ticks};
  const duration::quarters to_dq{to_ticks};
  const duration::months to_dm{to_ticks};
  const duration::weeks to_dw{to_ticks};
  const duration::days to_dd{to_ticks};
  const duration::hours to_dh{to_ticks, to_ticks_of_day};
  const duration::minutes to_dmin{to_ticks, to_ticks_of_day};
  const duration::seconds to_ds{to_ticks, to_ticks_of_day};
  const duration::milliseconds to_dmilli{to_ticks, to_ticks_of_day, to_ticks_of_second};
  const duration::microseconds to_dmicro{to_ticks, to_ticks_of_day, to_ticks_of_second};
  const duration::nanoseconds to_dnano{to_ticks, to_ticks_of_day, to_ticks_of_second};

  const cpp11::integers by_ticks = duration::get_ticks(by);
  const cpp11::integers by_ticks_of_day = duration::get_ticks_of_day(by);
  const cpp11::integers by_ticks_of_second = duration::get_ticks_of_second(by);

  const duration::years by_dy{by_ticks};
  const duration::quarters by_dq{by_ticks};
  const duration::months by_dm{by_ticks};
  const duration::weeks by_dw{by_ticks};
  const duration::days by_dd{by_ticks};
  const duration::hours by_dh{by_ticks, by_ticks_of_day};
  const duration::minutes by_dmin{by_ticks, by_ticks_of_day};
  const duration::seconds by_ds{by_ticks, by_ticks_of_day};
  const duration::milliseconds by_dmilli{by_ticks, by_ticks_of_day, by_ticks_of_second};
  const duration::microseconds by_dmicro{by_ticks, by_ticks_of_day, by_ticks_of_second};
  const duration::nanoseconds by_dnano{by_ticks, by_ticks_of_day, by_ticks_of_second};

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_seq_to_by_impl(from_dy, to_dy, by_dy);
  case precision::quarter: return duration_seq_to_by_impl(from_dq, to_dq, by_dq);
  case precision::month: return duration_seq_to_by_impl(from_dm, to_dm, by_dm);
  case precision::week: return duration_seq_to_by_impl(from_dw, to_dw, by_dw);
  case precision::day: return duration_seq_to_by_impl(from_dd, to_dd, by_dd);
  case precision::hour: return duration_seq_to_by_impl(from_dh, to_dh, by_dh);
  case precision::minute: return duration_seq_to_by_impl(from_dmin, to_dmin, by_dmin);
  case precision::second: return duration_seq_to_by_impl(from_ds, to_ds, by_ds);
  case precision::millisecond: return duration_seq_to_by_impl(from_dmilli, to_dmilli, by_dmilli);
  case precision::microsecond: return duration_seq_to_by_impl(from_dmicro, to_dmicro, by_dmicro);
  case precision::nanosecond: return duration_seq_to_by_impl(from_dnano, to_dnano, by_dnano);
  }

  never_reached("duration_seq_to_by_cpp");
}

template <class ClockDuration>
static
inline
cpp11::writable::list
duration_seq_to_lo_impl(const ClockDuration& from,
                        const ClockDuration& to,
                        const r_ssize& size) {
  using Duration = typename ClockDuration::duration;
  using Rep = typename Duration::rep;

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
duration_seq_to_lo_cpp(cpp11::list_of<cpp11::integers> from,
                       const cpp11::integers& precision_int,
                       cpp11::list_of<cpp11::integers> to,
                       const cpp11::integers& length_out) {
  using namespace rclock;

  const cpp11::integers from_ticks = duration::get_ticks(from);
  const cpp11::integers from_ticks_of_day = duration::get_ticks_of_day(from);
  const cpp11::integers from_ticks_of_second = duration::get_ticks_of_second(from);

  const duration::years from_dy{from_ticks};
  const duration::quarters from_dq{from_ticks};
  const duration::months from_dm{from_ticks};
  const duration::weeks from_dw{from_ticks};
  const duration::days from_dd{from_ticks};
  const duration::hours from_dh{from_ticks, from_ticks_of_day};
  const duration::minutes from_dmin{from_ticks, from_ticks_of_day};
  const duration::seconds from_ds{from_ticks, from_ticks_of_day};
  const duration::milliseconds from_dmilli{from_ticks, from_ticks_of_day, from_ticks_of_second};
  const duration::microseconds from_dmicro{from_ticks, from_ticks_of_day, from_ticks_of_second};
  const duration::nanoseconds from_dnano{from_ticks, from_ticks_of_day, from_ticks_of_second};

  const cpp11::integers to_ticks = duration::get_ticks(to);
  const cpp11::integers to_ticks_of_day = duration::get_ticks_of_day(to);
  const cpp11::integers to_ticks_of_second = duration::get_ticks_of_second(to);

  const duration::years to_dy{to_ticks};
  const duration::quarters to_dq{to_ticks};
  const duration::months to_dm{to_ticks};
  const duration::weeks to_dw{to_ticks};
  const duration::days to_dd{to_ticks};
  const duration::hours to_dh{to_ticks, to_ticks_of_day};
  const duration::minutes to_dmin{to_ticks, to_ticks_of_day};
  const duration::seconds to_ds{to_ticks, to_ticks_of_day};
  const duration::milliseconds to_dmilli{to_ticks, to_ticks_of_day, to_ticks_of_second};
  const duration::microseconds to_dmicro{to_ticks, to_ticks_of_day, to_ticks_of_second};
  const duration::nanoseconds to_dnano{to_ticks, to_ticks_of_day, to_ticks_of_second};

  if (length_out.size() != 1) {
    clock_abort("Internal error: `length_out` should have size 1.");
  }

  const r_ssize size = length_out[0];

  switch (parse_precision(precision_int)) {
  case precision::year: return duration_seq_to_lo_impl(from_dy, to_dy, size);
  case precision::quarter: return duration_seq_to_lo_impl(from_dq, to_dq, size);
  case precision::month: return duration_seq_to_lo_impl(from_dm, to_dm, size);
  case precision::week: return duration_seq_to_lo_impl(from_dw, to_dw, size);
  case precision::day: return duration_seq_to_lo_impl(from_dd, to_dd, size);
  case precision::hour: return duration_seq_to_lo_impl(from_dh, to_dh, size);
  case precision::minute: return duration_seq_to_lo_impl(from_dmin, to_dmin, size);
  case precision::second: return duration_seq_to_lo_impl(from_ds, to_ds, size);
  case precision::millisecond: return duration_seq_to_lo_impl(from_dmilli, to_dmilli, size);
  case precision::microsecond: return duration_seq_to_lo_impl(from_dmicro, to_dmicro, size);
  case precision::nanosecond: return duration_seq_to_lo_impl(from_dnano, to_dnano, size);
  }

  never_reached("duration_seq_to_lo_cpp");
}
