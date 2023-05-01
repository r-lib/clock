#ifndef CLOCK_CALENDAR_H
#define CLOCK_CALENDAR_H

#include "clock.h"
#include "enums.h"
#include "integers.h"

// -----------------------------------------------------------------------------

template <class Calendar>
static
inline
cpp11::writable::strings
format_calendar_impl(const Calendar& x) {
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
    SET_STRING_ELT(out, i, Rf_mkCharLenCE(string.c_str(), string.size(), CE_UTF8));
  }

  return out;
}

// -----------------------------------------------------------------------------

template <class Calendar>
static
inline
cpp11::writable::list
invalid_resolve_calendar_impl(Calendar& x,
                              const enum invalid& invalid_val,
                              const cpp11::sexp& call) {
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      continue;
    }
    x.resolve(i, invalid_val, call);
  }

  return x.to_list();
}

// -----------------------------------------------------------------------------

template <class Calendar, class ClockDuration>
static
inline
cpp11::writable::list
calendar_plus_duration_impl(Calendar& x, const ClockDuration& n) {
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

// -----------------------------------------------------------------------------

template <class ClockDuration, class Calendar>
static
inline
cpp11::writable::list
as_sys_time_from_calendar_impl(const Calendar& x) {
  using Duration = typename ClockDuration::chrono_duration;

  const r_ssize size = x.size();
  ClockDuration out(size);

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

template <class ClockDuration, class Calendar>
cpp11::writable::list
as_calendar_from_sys_time_impl(cpp11::list_of<cpp11::doubles>& fields) {
  using Duration = typename ClockDuration::chrono_duration;

  const ClockDuration x{fields};
  const r_ssize size = x.size();

  Calendar out(size);

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

#endif
