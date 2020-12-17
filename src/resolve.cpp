#include "clock.h"
#include "resolve.h"

[[cpp11::register]]
clock_writable_field resolve_seconds_of_day(const clock_field& seconds_of_day,
                                            const cpp11::logicals& ok,
                                            const cpp11::strings& day_nonexistent) {
  const r_ssize size = seconds_of_day.size();
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  clock_writable_field out(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_ok = ok[i];
    int elt_seconds_of_day = seconds_of_day[i];

    if (elt_ok == 1) {
      out[i] = elt_seconds_of_day;
      continue;
    }
    if (elt_ok == r_int_na) {
      out[i] = r_int_na;
      continue;
    }

    switch (day_nonexistent_val) {
    case day_nonexistent::first_time: {
      out[i] = 0;
      break;
    }
    case day_nonexistent::last_time: {
      out[i] = 86400 - 1;
      break;
    }
    case day_nonexistent::na: {
      out[i] = r_int_na;
      break;
    }
    case day_nonexistent::first_day:
    case day_nonexistent::last_day: {
      out[i] = elt_seconds_of_day;
      break;
    }
    case day_nonexistent::error: {
      clock_abort("Internal error: Should never get here.");
    }
    }
  }

  return out;
}

[[cpp11::register]]
clock_writable_field resolve_nanoseconds_of_second(const clock_field& nanoseconds_of_second,
                                                   const cpp11::logicals& ok,
                                                   const cpp11::strings& day_nonexistent,
                                                   const cpp11::strings& precision) {
  const r_ssize size = nanoseconds_of_second.size();
  enum day_nonexistent day_nonexistent_val = parse_day_nonexistent(day_nonexistent);
  enum precision precision_val = parse_precision(precision);
  clock_writable_field out(size);

  for (r_ssize i = 0; i < size; ++i) {
    int elt_ok = ok[i];
    int elt_nanoseconds_of_second = nanoseconds_of_second[i];

    if (elt_ok == 1) {
      out[i] = elt_nanoseconds_of_second;
      continue;
    }
    if (elt_ok == r_int_na) {
      out[i] = r_int_na;
      continue;
    }

    switch (day_nonexistent_val) {
    case day_nonexistent::first_time: {
      out[i] = 0;
      break;
    }
    case day_nonexistent::last_time: {
      switch (precision_val) {
      case precision::millisecond: out[i] = 999000000; break;
      case precision::microsecond: out[i] = 999999000; break;
      case precision::nanosecond:  out[i] = 999999999; break;
      default: clock_abort("Internal error: Should never get here.");
      }
      break;
    }
    case day_nonexistent::na: {
      out[i] = r_int_na;
      break;
    }
    case day_nonexistent::first_day:
    case day_nonexistent::last_day: {
      out[i] = elt_nanoseconds_of_second;
      break;
    }
    case day_nonexistent::error: {
      clock_abort("Internal error: Should never get here.");
    }
    }
  }

  return out;
}
