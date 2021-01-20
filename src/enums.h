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

enum quarterly::start parse_start(const cpp11::integers& x);

// -----------------------------------------------------------------------------

enum class precision: unsigned char {
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
  nanosecond
};

enum precision parse_precision(const cpp11::integers& x);

const std::string& precision_to_cpp_string(const enum precision& x);

// -----------------------------------------------------------------------------

enum class clock_name {
  sys,
  naive
};

enum clock_name parse_clock_name(const cpp11::strings& x);

// -----------------------------------------------------------------------------
#endif
