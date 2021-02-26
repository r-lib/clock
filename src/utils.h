#ifndef CLOCK_UTILS_H
#define CLOCK_UTILS_H

#include "clock.h"
#include <cstdint>
#include <cmath>
#include <cstdarg> // For `va_start()` and `va_end()`
#include <cstdio> // For `vsnprintf()`

// -----------------------------------------------------------------------------

extern SEXP strings_empty;

extern SEXP syms_precision;
extern SEXP syms_start;
extern SEXP syms_clock;
extern SEXP syms_zone;
extern SEXP syms_set_names;

extern SEXP classes_duration;
extern SEXP classes_sys_time;
extern SEXP classes_naive_time;
extern SEXP classes_zoned_time;
extern SEXP classes_year_month_day;
extern SEXP classes_year_month_weekday;
extern SEXP classes_year_day;
extern SEXP classes_iso_year_week_day;
extern SEXP classes_year_quarter_day;
extern SEXP classes_data_frame;

extern SEXP ints_empty;

// -----------------------------------------------------------------------------

static
inline
SEXP
r_clone_referenced(SEXP x) {
  if (MAYBE_REFERENCED(x)) {
    return Rf_shallow_duplicate(x);
  } else {
    return x;
  }
}

static
inline
const SEXP*
r_chr_deref_const(SEXP x) {
  return (const SEXP*) STRING_PTR(x);
}

static
inline
const SEXP*
r_list_deref_const(SEXP x) {
#if (R_VERSION < R_Version(3, 5, 0))
  return ((const SEXP*) STRING_PTR(x));
#else
  return ((const SEXP*) DATAPTR_RO(x));
#endif
}

// -----------------------------------------------------------------------------

static
inline
SEXP new_compact_rownames(r_ssize n_rows) {
  if (n_rows <= 0) {
    return ints_empty;
  }

  SEXP out = Rf_allocVector(INTSXP, 2);
  int* p_out = INTEGER(out);
  p_out[0] = r_int_na;
  p_out[1] = -n_rows;

  return out;
}

static
inline
void init_compact_rownames(SEXP x, r_ssize n_rows) {
  SEXP rn = PROTECT(new_compact_rownames(n_rows));
  Rf_setAttrib(x, R_RowNamesSymbol, rn);
  UNPROTECT(1);
}

static
inline
void
r_init_data_frame(SEXP x, r_ssize n_rows) {
  init_compact_rownames(x, n_rows);
  Rf_setAttrib(x, R_ClassSymbol, classes_data_frame);
}

// -----------------------------------------------------------------------------

/*
 * `names<-()` can take advantage of shallow duplication using an ALTREP wrapper
 * which is otherwise not exposed in the R API
 */
static
inline
SEXP set_names_dispatch(SEXP x, SEXP value) {
  SEXP call = PROTECT(Rf_lang3(syms_set_names, x, value));
  SEXP out = Rf_eval(call, R_GlobalEnv);
  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------
// "Safe" variants on rlib functions

static inline bool r_dbl_is_missing(double x) {
  return ISNAN(x);
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

// Max - If int64_t is a long long, this converts `9223372036854775807` ->
// `9223372036854775808` as that is the next possible value representable as a
// double.
//
// Min - This should let `-9223372036854775808` remain as is, as it is
// directly representable as a double.
//
// Practically this just forces us to use `x >= INT64_MAX_AS_DOUBLE` rather
// than just `>`, as you can't ever have a double value right on `INT64_MAX`.
static const double INT64_MAX_AS_DOUBLE = static_cast<double>(INT64_MAX);
static const double INT64_MIN_AS_DOUBLE = static_cast<double>(INT64_MIN);

static
inline
bool
clock_dbl_is_oob_for_int64(double x) {
  return x >= INT64_MAX_AS_DOUBLE || x < INT64_MIN_AS_DOUBLE;
}

/*
 * Floor to get rid of fractional seconds. This is the most consistent way to
 * drop them to pretend like they don't exist. Using `floor()` to round towards
 * negative infinity is the correct thing to do with pre 1970 (i.e. negative)
 * times.
 *
 * For example:
 *
 * unclass(as.POSIXct("1969-12-31 23:59:59.9999", "UTC"))
 * [1] -0.0001000000000033196556615
 *
 * Truncating to 0 gives 1970-01-01.
 *
 * Flooring to -1 gives 1969-12-31 23:59:59, i.e. the "correct" result if we are
 * ignoring fractional seconds.
 */
static
inline
int64_t
clock_dbl_as_int64(double x) {
  x = std::floor(x);
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
