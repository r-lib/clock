#ifndef CIVIL_UTILS_H
#define CIVIL_UTILS_H

#include "civil.h"
#include <cstdint>
#include <cmath>

// -----------------------------------------------------------------------------
// "Safe" variants on rlib functions

static inline bool r_dbl_is_missing(double x) {
  return ISNAN(x);
}
static inline bool civil_dbl_is_missing(double x) {
  return cpp11::safe[r_dbl_is_missing](x);
}

static inline int* r_int_deref(SEXP x) {
  return INTEGER(x);
}
static inline int* civil_int_deref(SEXP x) {
  return cpp11::safe[r_int_deref](x);
}

static inline const int* r_int_deref_const(SEXP x) {
  return (const int*) INTEGER(x);
}
static inline const int* civil_int_deref_const(SEXP x) {
  return cpp11::safe[r_int_deref_const](x);
}

static inline r_ssize r_length(SEXP x) {
  return Rf_xlength(x);
}
static inline r_ssize civil_length(SEXP x) {
  return cpp11::safe[r_length](x);
}

static inline bool r_is_scalar(SEXP x) {
  return Rf_xlength(x) == 1;
}
static inline bool civil_is_scalar(const cpp11::sexp& x) {
  return cpp11::safe[r_is_scalar](x);
}

static inline bool r_is_string(SEXP x) {
  return (TYPEOF(x) == STRSXP) && (Rf_length(x) == 1) && (STRING_ELT(x, 0) != r_chr_na);
}
static inline bool civil_is_string(const cpp11::sexp& x) {
  return cpp11::safe[r_is_string](x);
}

// -----------------------------------------------------------------------------

static int64_t r_int64_na = INT64_MIN;

static inline int64_t as_int64(double x) {
  if (civil_dbl_is_missing(x)) {
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

/*
 * civil_abort() calls back to `rlang::abort()` to actually throw the error,
 * with unwind protection.
 */

#define BUFSIZE 8192

static inline void fill_buffer(char* buf, const char* fmt, ...) {
  va_list dots;
  va_start(dots, fmt);
  vsnprintf(buf, BUFSIZE, fmt, dots);
  va_end(dots);

  buf[BUFSIZE - 1] = '\0';
}

template <typename... Args>
void civil_abort [[noreturn]] (const char* fmt, Args... args) {
  char buf[BUFSIZE];
  fill_buffer(buf, fmt, args...);

  cpp11::r_string string{buf};
  cpp11::writable::strings arg({string});

  auto abort = cpp11::package("rlang")["abort"];
  abort(arg);

  cpp11::stop("Internal error: Got past an rlang::abort()!");
}

#undef BUFSIZE


static inline void never_reached [[noreturn]] (const char* fn) {
  civil_abort("Internal error: Reached the unreachable in `%s()`.", fn);
}

// -----------------------------------------------------------------------------

static inline cpp11::writable::integers civil_int_recycle(const cpp11::writable::integers& x,
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

static inline cpp11::strings civil_chr_recycle(const cpp11::strings& x,
                                               const r_ssize& size) {
  r_ssize x_size = x.size();

  if (x_size == size) {
    return x;
  }
  if (x_size != 1) {
    civil_abort("`x` must be size 1 or %i.", (int) size);
  }

  cpp11::r_string val = x[0];
  cpp11::writable::strings out(size);

  for (r_ssize i = 0; i < size; ++i) {
    out[i] = val;
  }

  return out;
}

static inline cpp11::sexp civil_names_recycle(const cpp11::sexp& names,
                                              const r_ssize& size) {
  if (names == r_null) {
    return names;
  }

  if (TYPEOF(names) != STRSXP) {
    civil_abort("`names` must be a character vector or `NULL`.");
  }

  cpp11::strings typed_names = cpp11::as_cpp<cpp11::strings>(names);

  return civil_chr_recycle(typed_names, size);
}

// -----------------------------------------------------------------------------

#endif
