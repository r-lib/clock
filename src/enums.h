#ifndef CIVIL_ENUMS_H
#define CIVIL_ENUMS_H

#include "r.h"
#include "utils.h"
// -----------------------------------------------------------------------------

enum class day_nonexistent {
  end,
  start,
  end_keep,
  start_keep,
  na,
  error
};

enum day_nonexistent parse_day_nonexistent(sexp x);

// -----------------------------------------------------------------------------

enum class dst_direction {
  positive,
  negative
};

// -----------------------------------------------------------------------------

enum class dst_nonexistent {
  directional,
  next,
  previous,
  directional_shift,
  next_shift,
  previous_shift,
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
