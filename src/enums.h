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

enum class dst_direction {
  forward,
  backward
};

// -----------------------------------------------------------------------------

enum class dst_nonexistent {
  roll_directional,
  roll_forward,
  roll_backward,
  shift_directional,
  shift_forward,
  shift_backward,
  na,
  error
};

enum dst_nonexistent parse_dst_nonexistent(sexp x);
enum dst_nonexistent parse_dst_nonexistent_arithmetic(sexp x);

// -----------------------------------------------------------------------------

enum class dst_ambiguous {
  directional,
  earliest,
  latest,
  na,
  error
};

enum dst_ambiguous parse_dst_ambiguous(sexp x);
enum dst_ambiguous parse_dst_ambiguous_arithmetic(sexp x);

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

enum class update_unit {
  year,
  month,
  yday,
  day,
  hour,
  minute,
  second
};

enum update_unit parse_update_unit(sexp x);

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
