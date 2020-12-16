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

static inline void civil_rcrd_recycle(civil_writable_rcrd& x,
                                      const r_ssize& size) {
  r_ssize x_size = x[0].size();

  // Avoiding a copy from `civil_int_recycle()` that happens even when the
  // sizes are the same.
  // TODO: Is there a better way to structure `civil_int_recycle()`?
  if (x_size == size) {
    return;
  }

  r_ssize n = x.size();

  for (r_ssize i = 0; i < n; ++i) {
    civil_writable_rcrd_set(x, i, civil_int_recycle(x[i], size));
  }

  // Ensure names get recycled
  // TODO: https://github.com/r-lib/cpp11/issues/132
  // Should be able to go straight into a `cpp11::sexp`
  SEXP names = x.attr("clock_time_point:::names");
  x.attr("clock_time_point:::names") = civil_names_recycle(names, size);
}

static inline int* civil_rcrd_days_deref(civil_writable_rcrd& x) {
  return civil_int_deref(x[0].data());
}
static inline int* civil_rcrd_time_of_day_deref(civil_writable_rcrd& x) {
  return x.size() < 2 ? NULL : civil_int_deref(x[1].data());
}
static inline int* civil_rcrd_nanos_of_second_deref(civil_writable_rcrd& x) {
  return x.size() < 3 ? NULL : civil_int_deref(x[2].data());
}

static inline const int* civil_rcrd_days_deref_const(civil_rcrd& x) {
  return civil_int_deref_const(x[0].data());
}
static inline const int* civil_rcrd_time_of_day_deref_const(civil_rcrd& x) {
  return x.size() < 2 ? NULL : civil_int_deref_const(x[1].data());
}
static inline const int* civil_rcrd_nanos_of_second_deref_const(civil_rcrd& x) {
  return x.size() < 3 ? NULL : civil_int_deref_const(x[2].data());
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

static inline civil_writable_rcrd new_days_list(const civil_writable_field& days) {
  civil_writable_rcrd out({days});
  out.names() = "days";
  return out;
}

static inline civil_writable_rcrd new_days_time_of_day_list(const civil_writable_field& days,
                                                            const civil_writable_field& time_of_day) {
  civil_writable_rcrd out({days, time_of_day});
  out.names() = {"days", "time_of_day"};
  return out;
}

static inline civil_writable_rcrd new_days_time_of_day_nanos_of_second_list(const civil_writable_field& days,
                                                                            const civil_writable_field& time_of_day,
                                                                            const civil_writable_field& nanos_of_second) {
  civil_writable_rcrd out({days, time_of_day, nanos_of_second});
  out.names() = {"days", "time_of_day", "nanos_of_second"};
  return out;
}

#endif
