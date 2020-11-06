#ifndef CIVIL_UTILS_H
#define CIVIL_UTILS_H

#include "r.h"
#include "utils-rlib.h"
#include <date/date.h>
#include <cstdint>
#include <cmath>

// -----------------------------------------------------------------------------

extern sexp civil_syms_tzone;

extern sexp civil_classes_date;
extern sexp civil_classes_posixct;

// -----------------------------------------------------------------------------

static int64_t r_int64_na = INT64_MIN;

static inline int64_t as_int64(double x) {
  if (r_dbl_is_missing(x)) {
    return r_int64_na;
  }

  // Truncate towards zero to treat fractional seconds as if they never existed.
  x = std::trunc(x);

  if (x > INT64_MAX || x < INT64_MIN) {
    return r_int64_na;
  }

  return static_cast<int64_t>(x);
}

// -----------------------------------------------------------------------------

static inline void civil_poke_tzone(sexp x, sexp value) {
  r_poke_attribute(x, civil_syms_tzone, value);
}

static inline sexp civil_get_tzone(sexp x) {
  return r_get_attribute(x, civil_syms_tzone);
}

// -----------------------------------------------------------------------------

#endif
