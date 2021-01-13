#ifndef CLOCK_CALENDAR_H
#define CLOCK_CALENDAR_H

#include "clock.h"
#include "enums.h"
#include "check.h"
#include "integers.h"

// -----------------------------------------------------------------------------

template <enum component Component, class Calendar>
void
calendar_check_range_impl(const Calendar& dummy, const cpp11::integers& x, const char* arg) {
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    // Should never be `NA`, so don't allow that to pass through
    dummy.template check_range<Component>(x[i], arg);
  }
}

// -----------------------------------------------------------------------------

template <enum component Component, class Calendar>
static
inline
void
collect_field(Calendar& x, const cpp11::integers& field, const char* arg) {
  r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    const int elt = field[i];

    if (elt == r_int_na) {
      x.assign_na(i);
      continue;
    }

    x.template check_range<Component>(elt, arg);
  }
}

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
    const SEXP r_string = Rf_mkCharLenCE(string.c_str(), string.size(), CE_UTF8);
    SET_STRING_ELT(out, i, r_string);
  }

  return out;
}

// -----------------------------------------------------------------------------

template <class Calendar>
static
inline
cpp11::writable::logicals
invalid_detect_calendar_impl(const Calendar& x) {
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

template <class Calendar>
static
inline
bool
invalid_any_calendar_impl(const Calendar& x) {
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || x.ok(i)) {
      continue;
    }
    return true;
  }

  return false;
}

template <class Calendar>
static
inline
int
invalid_count_calendar_impl(const Calendar& x) {
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

template <class Calendar>
static
inline
cpp11::writable::list
invalid_resolve_calendar_impl(Calendar& x, const enum invalid& invalid_val) {
  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      continue;
    }
    x.resolve(i, invalid_val);
  }

  return x.to_list();
}


// -----------------------------------------------------------------------------

template <enum component Component, class Calendar>
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
      x.template check_range<Component>(value[i], "value");
    }
  }

  cpp11::writable::list out({x.to_list(), value.sexp()});
  out.names() = {"fields", "value"};

  return out;
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

template <class Calendar, class ClockDuration>
cpp11::writable::list
as_calendar_from_sys_time_impl(const ClockDuration& x) {
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

#endif
