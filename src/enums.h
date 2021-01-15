#ifndef CLOCK_ENUMS_H
#define CLOCK_ENUMS_H

#include "clock.h"
#include "utils.h"
// -----------------------------------------------------------------------------

enum class invalid {
  last_time,
  first_time,
  last_day,
  first_day,
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

enum class precision {
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

enum precision parse_precision(const cpp11::strings& x);

static
inline
std::string
precision_to_string(const enum precision precision_val) {
  switch (precision_val) {
  case precision::year: return "year";
  case precision::quarter: return "quarter";
  case precision::month: return "month";
  case precision::week: return "week";
  case precision::day: return "day";
  case precision::hour: return "hour";
  case precision::minute: return "minute";
  case precision::second: return "second";
  case precision::millisecond: return "millisecond";
  case precision::microsecond: return "microsecond";
  case precision::nanosecond: return "nanosecond";
  }
  never_reached("precision_to_string");
}

// -----------------------------------------------------------------------------

enum class clock_name {
  sys,
  naive
};

enum clock_name parse_clock_name(const cpp11::strings& x);

// -----------------------------------------------------------------------------
#endif
