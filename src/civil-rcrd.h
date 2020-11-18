#ifndef CIVIL_CIVIL_RCRD_H
#define CIVIL_CIVIL_RCRD_H

#include "civil.h"
#include "utils.h"

static inline void civil_rcrd_recycle_fields(civil_writable_rcrd& x,
                                             const r_ssize& size) {
  r_ssize n = x.size();

  for (r_ssize i = 0; i < n; ++i) {
    x[i] = civil_int_recycle(x[i], size);
  }
}

static inline int* civil_rcrd_days_deref(civil_writable_rcrd& x) {
  return r_int_deref(x[0].data());
}
static inline int* civil_rcrd_time_of_day_deref(civil_writable_rcrd& x) {
  return x.size() < 2 ? NULL : r_int_deref(x[1].data());
}
static inline int* civil_rcrd_nanos_of_second_deref(civil_writable_rcrd& x) {
  return x.size() < 3 ? NULL : r_int_deref(x[2].data());
}

static inline const int* civil_rcrd_days_deref_const(civil_rcrd& x) {
  return r_int_deref_const(x[0].data());
}
static inline const int* civil_rcrd_time_of_day_deref_const(civil_rcrd& x) {
  return x.size() < 2 ? NULL : r_int_deref_const(x[1].data());
}
static inline const int* civil_rcrd_nanos_of_second_deref_const(civil_rcrd& x) {
  return x.size() < 3 ? NULL : r_int_deref_const(x[2].data());
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
