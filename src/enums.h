#ifndef CLOCK_ENUMS_H
#define CLOCK_ENUMS_H

#include "clock.h"
#include "utils.h"
// -----------------------------------------------------------------------------

enum class invalid {
  previous,
  next,
  overflow,
  previous_day,
  next_day,
  overflow_day,
  na,
  error
};

enum invalid parse_invalid(const cpp11::strings& x);

// -----------------------------------------------------------------------------

enum class nonexistent {
  roll_forward,
  roll_backward,
  shift_forward,
  shift_backward,
  na,
  error
};

enum nonexistent parse_nonexistent_one(const cpp11::r_string& x);

// -----------------------------------------------------------------------------

enum class ambiguous {
  earliest,
  latest,
  na,
  error
};

enum ambiguous parse_ambiguous_one(const cpp11::r_string& x);

// -----------------------------------------------------------------------------

enum class component {
  year,
  quarter,
  month,
  week,
  day,
  hour,
  minute,
  second,
  millisecond,
  microsecond,
  nanosecond,
  index
};

enum component parse_component(const cpp11::strings& x);

// -----------------------------------------------------------------------------

enum week::start parse_week_start(const cpp11::integers& x);

// -----------------------------------------------------------------------------

enum quarterly::start parse_quarterly_start(const cpp11::integers& x);

// -----------------------------------------------------------------------------

enum class precision: unsigned char {
  year = 0u,
  quarter = 1u,
  month = 2u,
  week = 3u,
  day = 4u,
  hour = 5u,
  minute = 6u,
  second = 7u,
  millisecond = 8u,
  microsecond = 9u,
  nanosecond = 10u
};

enum precision parse_precision(const cpp11::integers& x);

const std::string& precision_to_cpp_string(const enum precision& x);

// -----------------------------------------------------------------------------

enum class clock_name: unsigned char {
  sys = 0u,
  naive = 1u
};

enum clock_name parse_clock_name(const cpp11::integers& x);

// -----------------------------------------------------------------------------

enum class decimal_mark {
  period,
  comma
};

enum decimal_mark parse_decimal_mark(const cpp11::strings& x);

// -----------------------------------------------------------------------------
#endif
