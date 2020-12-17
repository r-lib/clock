#ifndef CIVIL_CIVIL_RCRD_H
#define CIVIL_CIVIL_RCRD_H

#include "civil.h"
#include "utils.h"

// -----------------------------------------------------------------------------

// FIXME: https://github.com/r-lib/cpp11/issues/131
//
// Should be able to remove this after that issue is fixed, in favor of
// assigning directly with `x[i] = value`

static inline void civil_writable_rcrd_set(civil_writable_rcrd& x,
                                           const r_ssize& i,
                                           const civil_writable_field& value) {
  SET_VECTOR_ELT(x, i, value);
}

// -----------------------------------------------------------------------------

static inline int* civil_rcrd_days_deref(civil_writable_rcrd& x) {
  return civil_int_deref(x[0].data());
}
static inline int* civil_rcrd_time_of_day_deref(civil_writable_rcrd& x) {
  return x.size() < 2 ? NULL : civil_int_deref(x[1].data());
}
static inline int* civil_rcrd_nanos_of_second_deref(civil_writable_rcrd& x) {
  return x.size() < 3 ? NULL : civil_int_deref(x[2].data());
}

static inline void civil_rcrd_assign_missing(const r_ssize& i,
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
static inline civil_writable_rcrd civil_rcrd_clone(const civil_rcrd& x) {
  // Shallow duplicate with attributes
  civil_writable_rcrd out(x);

  r_ssize n = out.size();

  for (r_ssize i = 0; i < n; ++i) {
    // FIXME: out[i] = cpp11::safe[Rf_shallow_duplicate](out[i]);
    civil_writable_rcrd_set(out, i, cpp11::safe[Rf_shallow_duplicate](out[i]));
  }

  return out;
}

// -----------------------------------------------------------------------------

#endif
