#ifndef CIVIL_CIVIL_RCRD_H
#define CIVIL_CIVIL_RCRD_H

#include "r.h"
#include "utils-rlib.h"
#include <date/date.h>

using sys_nanoseconds = date::sys_time<std::chrono::nanoseconds>;
using local_nanoseconds = date::local_time<std::chrono::nanoseconds>;

static inline sexp civil_rcrd_recycle(sexp x, r_ssize size) {
  x = PROTECT(r_maybe_clone(x));

  r_ssize n = r_length(x);

  for (r_ssize i = 0; i < n; ++i) {
    r_list_poke(x, i, r_int_recycle(r_list_get(x, i), size));
  }

  UNPROTECT(1);
  return x;
}

static inline sexp civil_rcrd_maybe_clone(sexp x) {
  x = PROTECT(r_maybe_clone(x));

  r_ssize n = r_length(x);

  for (r_ssize i = 0; i < n; ++i) {
    r_list_poke(x, i, r_maybe_clone(r_list_get(x, i)));
  }

  UNPROTECT(1);
  return x;
}

static inline sexp civil_rcrd_days(sexp x) {
  return r_list_get(x, 0);
}
static inline sexp civil_rcrd_time_of_day(sexp x) {
  return r_length(x) < 2 ? NULL : r_list_get(x, 1);
}
static inline sexp civil_rcrd_nanos_of_second(sexp x) {
  return r_length(x) < 3 ? NULL : r_list_get(x, 2);
}

static inline int* civil_rcrd_days_deref(sexp x) {
  return r_int_deref(civil_rcrd_days(x));
}
static inline int* civil_rcrd_time_of_day_deref(sexp x) {
  sexp time_of_day = civil_rcrd_time_of_day(x);
  return time_of_day == NULL ? NULL : r_int_deref(time_of_day);
}
static inline int* civil_rcrd_nanos_of_second_deref(sexp x) {
  sexp nanos_of_second = civil_rcrd_nanos_of_second(x);
  return nanos_of_second == NULL ? NULL : r_int_deref(nanos_of_second);
}

static inline void civil_rcrd_assign_missing(r_ssize i,
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

#endif
