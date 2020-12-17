#ifndef CLOCK_CLOCK_RCRD_H
#define CLOCK_CLOCK_RCRD_H

#include "clock.h"
#include "utils.h"

// -----------------------------------------------------------------------------

// FIXME: https://github.com/r-lib/cpp11/issues/131
//
// Should be able to remove this after that issue is fixed, in favor of
// assigning directly with `x[i] = value`

static inline void clock_writable_rcrd_set(clock_writable_rcrd& x,
                                           const r_ssize& i,
                                           const clock_writable_field& value) {
  SET_VECTOR_ELT(x, i, value);
}

// -----------------------------------------------------------------------------

static inline int* clock_rcrd_days_deref(clock_writable_rcrd& x) {
  return clock_int_deref(x[0].data());
}
static inline int* clock_rcrd_time_of_day_deref(clock_writable_rcrd& x) {
  return x.size() < 2 ? NULL : clock_int_deref(x[1].data());
}
static inline int* clock_rcrd_nanos_of_second_deref(clock_writable_rcrd& x) {
  return x.size() < 3 ? NULL : clock_int_deref(x[2].data());
}

static inline void clock_rcrd_assign_missing(const r_ssize& i,
                                             int* p_days,
                                             int* p_time_of_day,
                                             int* p_nanos_of_second) {
  // Always exists
  p_days[i] = r_int_na;

  if (p_time_of_day != NULL) {
    p_time_of_day[i] = r_int_na;
  }
  if (p_nanos_of_second != NULL) {
    p_nanos_of_second[i] = r_int_na;
  }
}

/*
 * - Shallow duplicate the list (cpp11 does this for us), including all
 *   attributes
 * - Shallow duplicate each element, which shallow duplicating the list doesn't
 *   do.
 */
static inline clock_writable_rcrd clock_rcrd_clone(const clock_rcrd& x) {
  // Shallow duplicate with attributes
  clock_writable_rcrd out(x);

  r_ssize n = out.size();

  for (r_ssize i = 0; i < n; ++i) {
    // FIXME: out[i] = cpp11::safe[Rf_shallow_duplicate](out[i]);
    clock_writable_rcrd_set(out, i, cpp11::safe[Rf_shallow_duplicate](out[i]));
  }

  return out;
}

// -----------------------------------------------------------------------------

#endif
