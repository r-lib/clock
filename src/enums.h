#ifndef CIVIL_ENUMS_H
#define CIVIL_ENUMS_H

#include "r.h"
#include "utils.h"
// -----------------------------------------------------------------------------

enum class day_nonexistant {
  month_start,
  month_end,
  na,
  error
};

enum day_nonexistant parse_day_nonexistant(sexp x);

// -----------------------------------------------------------------------------

enum class dst_direction {
  positive,
  negative
};

// -----------------------------------------------------------------------------

enum class dst_nonexistant {
  directional,
  next,
  previous,
  na,
  error
};

enum dst_nonexistant parse_dst_nonexistant(sexp x);
enum dst_nonexistant parse_dst_nonexistant_no_directional(sexp x);

// -----------------------------------------------------------------------------

enum class dst_ambiguous {
  directional,
  earliest,
  latest,
  na,
  error
};

enum dst_ambiguous parse_dst_ambiguous(sexp x);
enum dst_ambiguous parse_dst_ambiguous_no_directional(sexp x);

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
#endif
