#ifndef CIVIL_LOCAL_H
#define CIVIL_LOCAL_H

#include "r.h"
#include "utils-rlib.h"

static inline sexp local_datetime_year(sexp x) {
  return r_list_get(x, 0);
}
static inline sexp local_datetime_month(sexp x) {
  return r_list_get(x, 1);
}
static inline sexp local_datetime_day(sexp x) {
  return r_list_get(x, 2);
}
static inline sexp local_datetime_hour(sexp x) {
  return r_list_get(x, 3);
}
static inline sexp local_datetime_minute(sexp x) {
  return r_list_get(x, 4);
}
static inline sexp local_datetime_second(sexp x) {
  return r_list_get(x, 5);
}

static inline sexp local_datetime_poke_year(sexp x, sexp value) {
  return r_list_poke(x, 0, value);
}
static inline sexp local_datetime_poke_month(sexp x, sexp value) {
  return r_list_poke(x, 1, value);
}
static inline sexp local_datetime_poke_day(sexp x, sexp value) {
  return r_list_poke(x, 2, value);
}
static inline sexp local_datetime_poke_hour(sexp x, sexp value) {
  return r_list_poke(x, 3, value);
}
static inline sexp local_datetime_poke_minute(sexp x, sexp value) {
  return r_list_poke(x, 4, value);
}
static inline sexp local_datetime_poke_second(sexp x, sexp value) {
  return r_list_poke(x, 5, value);
}

static inline sexp local_names(sexp x) {
  return r_get_attribute(x, r_sym("civil_local:::names"));
}
static inline void local_poke_names(sexp x, sexp value) {
  return r_poke_attribute(x, r_sym("civil_local:::names"), value);
}

static inline sexp local_datetime_recycle(sexp x, r_ssize size) {
  x = PROTECT(r_maybe_clone(x));

  sexp year = local_datetime_year(x);
  local_datetime_poke_year(x, r_int_recycle(year, size));

  sexp month = local_datetime_month(x);
  local_datetime_poke_month(x, r_int_recycle(month, size));

  sexp day = local_datetime_day(x);
  local_datetime_poke_day(x, r_int_recycle(day, size));

  sexp hour = local_datetime_hour(x);
  local_datetime_poke_hour(x, r_int_recycle(hour, size));

  sexp minute = local_datetime_minute(x);
  local_datetime_poke_minute(x, r_int_recycle(minute, size));

  sexp second = local_datetime_second(x);
  local_datetime_poke_second(x, r_int_recycle(second, size));

  sexp names = local_names(x);
  if (names != r_null) {
    local_poke_names(x, r_chr_recycle(names, size));
  }

  UNPROTECT(1);
  return x;
}

static inline sexp local_datetime_maybe_clone(sexp x) {
  x = PROTECT(r_maybe_clone(x));

  local_datetime_poke_year(x, r_maybe_clone(local_datetime_year(x)));
  local_datetime_poke_month(x, r_maybe_clone(local_datetime_month(x)));
  local_datetime_poke_day(x, r_maybe_clone(local_datetime_day(x)));
  local_datetime_poke_hour(x, r_maybe_clone(local_datetime_hour(x)));
  local_datetime_poke_minute(x, r_maybe_clone(local_datetime_minute(x)));
  local_datetime_poke_second(x, r_maybe_clone(local_datetime_second(x)));

  UNPROTECT(1);
  return x;
}

#endif
