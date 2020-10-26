#ifndef CIVIL_ENUMS_H
#define CIVIL_ENUMS_H

#include <cpp11.hpp>
// -----------------------------------------------------------------------------

enum class day_nonexistant {
  month_start,
  month_end,
  na,
  error
};

enum day_nonexistant parse_day_nonexistant(cpp11::strings x);

// -----------------------------------------------------------------------------

enum class dst_nonexistant {
  next,
  previous,
  na,
  error
};

enum dst_nonexistant parse_dst_nonexistant(cpp11::strings x);

// -----------------------------------------------------------------------------

enum class dst_ambiguous {
  earliest,
  latest,
  na,
  error
};

enum dst_ambiguous parse_dst_ambiguous(cpp11::strings x);

// -----------------------------------------------------------------------------

enum class unit {
  year,
  month,
  week,
  yday,
  day,
  hour,
  minute,
  second
};

enum unit parse_unit(cpp11::strings x);

// -----------------------------------------------------------------------------
#endif
