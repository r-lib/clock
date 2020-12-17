#ifndef CLOCK_UTILS_H
#define CLOCK_UTILS_H

#include "clock.h"
#include <cstdint>
#include <cmath>
#include <cstdarg> // For `va_start()` and `va_end()`
#include <cstdio> // For `vsnprintf()`

// -----------------------------------------------------------------------------
// "Safe" variants on rlib functions

static inline bool r_dbl_is_missing(double x) {
  return ISNAN(x);
}
static inline bool clock_dbl_is_missing(double x) {
  return cpp11::safe[r_dbl_is_missing](x);
}

static inline int* r_int_deref(SEXP x) {
  return INTEGER(x);
}
static inline int* clock_int_deref(SEXP x) {
  return cpp11::safe[r_int_deref](x);
}

static inline const int* r_int_deref_const(SEXP x) {
  return (const int*) INTEGER(x);
}
static inline const int* clock_int_deref_const(SEXP x) {
  return cpp11::safe[r_int_deref_const](x);
}

static inline r_ssize r_length(SEXP x) {
  return Rf_xlength(x);
}
static inline r_ssize clock_length(SEXP x) {
  return cpp11::safe[r_length](x);
}

static inline bool r_is_scalar(SEXP x) {
  return Rf_xlength(x) == 1;
}
static inline bool clock_is_scalar(const cpp11::sexp& x) {
  return cpp11::safe[r_is_scalar](x);
}

static inline bool r_is_string(SEXP x) {
  return (TYPEOF(x) == STRSXP) && (Rf_length(x) == 1) && (STRING_ELT(x, 0) != r_chr_na);
}
static inline bool clock_is_string(const cpp11::sexp& x) {
  return cpp11::safe[r_is_string](x);
}

// -----------------------------------------------------------------------------

static int64_t r_int64_na = INT64_MIN;

static inline int64_t as_int64(double x) {
  if (clock_dbl_is_missing(x)) {
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
 * clock_abort() calls back to `rlang::abort()` to actually throw the error,
 * with unwind protection.
 */

#define BUFSIZE 8192

static inline void fill_buffer(char* buf, const char* fmt, ...) {
  std::va_list dots;
  va_start(dots, fmt);
  std::vsnprintf(buf, BUFSIZE, fmt, dots);
  va_end(dots);

  buf[BUFSIZE - 1] = '\0';
}

template <typename... Args>
void clock_abort [[noreturn]] (const char* fmt, Args... args) {
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
  clock_abort("Internal error: Reached the unreachable in `%s()`.", fn);
}

// -----------------------------------------------------------------------------

#endif
