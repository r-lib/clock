#ifndef CIVIL_UTILS_H
#define CIVIL_UTILS_H

#include "civil.h"
#include <cstdint>
#include <cmath>

// -----------------------------------------------------------------------------

extern SEXP civil_syms_tzone;

extern SEXP civil_classes_date;
extern SEXP civil_classes_posixct;

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

static inline void civil_poke_tzone(SEXP x, SEXP value) {
  r_poke_attribute(x, civil_syms_tzone, value);
}

static inline SEXP civil_get_tzone(SEXP x) {
  return r_get_attribute(x, civil_syms_tzone);
}

// -----------------------------------------------------------------------------

template <typename... Args>
void civil_abort [[noreturn]] (const char* fmt, Args... args) {
  cpp11::safe.noreturn(r_abort)(fmt, args...);
}

// -----------------------------------------------------------------------------

static inline bool civil_is_scalar(const cpp11::sexp& x) {
  return cpp11::safe[r_is_scalar](x);
}

static inline bool civil_is_string(const cpp11::sexp& x) {
  return cpp11::safe[r_is_string](x);
}

// -----------------------------------------------------------------------------

static inline
cpp11::writable::integers civil_int_recycle(const cpp11::writable::integers& x,
                                            const r_ssize& size) {
  r_ssize x_size = x.size();

  if (x_size == size) {
    return x;
  }
  if (x_size != 1) {
    civil_abort("`x` must be size 1 or %i.", (int) size);
  }

  int val = x[0];
  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    out[i] = val;
  }

  return out;
}

// -----------------------------------------------------------------------------

#endif
