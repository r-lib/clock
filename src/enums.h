#ifndef CIVIL_ENUMS_H
#define CIVIL_ENUMS_H

#include "r.h"
#include "utils.h"
// -----------------------------------------------------------------------------

enum class day_nonexistent {
  last_time,
  first_time,
  last_day,
  first_day,
  na,
  error
};

enum day_nonexistent parse_day_nonexistent(sexp x);

// -----------------------------------------------------------------------------

enum class dst_nonexistent {
  roll_forward,
  roll_backward,
  shift_forward,
  shift_backward,
  na,
  error
};

enum dst_nonexistent parse_dst_nonexistent(sexp x);
enum dst_nonexistent parse_dst_nonexistent_one(const char* x);

// -----------------------------------------------------------------------------

enum class dst_ambiguous {
  earliest,
  latest,
  na,
  error
};

enum dst_ambiguous parse_dst_ambiguous(sexp x);
enum dst_ambiguous parse_dst_ambiguous_one(const char* x);

// -----------------------------------------------------------------------------

enum class unit {
  year,
  month,
  week,
  day,
  hour,
  minute,
  second
};

enum unit parse_unit(sexp x);

// -----------------------------------------------------------------------------

enum class adjuster {
  year,
  month,
  day,
  hour,
  minute,
  second,
  last_day_of_month
};

enum adjuster parse_adjuster(sexp x);

// -----------------------------------------------------------------------------
#endif
