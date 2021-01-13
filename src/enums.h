#ifndef CLOCK_ENUMS_H
#define CLOCK_ENUMS_H

#include "clock.h"
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
  month,
  day,
  hour,
  minute,
  second,
  millisecond,
  microsecond,
  nanosecond,
  quarternum,
  quarterday,
  weeknum,
  weekday,
  weekday_index
};

enum component parse_component(const cpp11::strings& x);

// -----------------------------------------------------------------------------

enum quarterly::start parse_start(const cpp11::integers& x);

// -----------------------------------------------------------------------------

enum class precision2 {
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

enum precision2 parse_precision2(const cpp11::strings& x);

static
inline
std::string
precision_to_string(const enum precision2 precision) {
  switch (precision) {
  case precision2::year: return "year";
  case precision2::quarter: return "quarter";
  case precision2::month: return "month";
  case precision2::week: return "week";
  case precision2::day: return "day";
  case precision2::hour: return "hour";
  case precision2::minute: return "minute";
  case precision2::second: return "second";
  case precision2::millisecond: return "millisecond";
  case precision2::microsecond: return "microsecond";
  case precision2::nanosecond: return "nanosecond";
  }
}

// -----------------------------------------------------------------------------

enum class clock {
  sys,
  naive
};

enum clock parse_clock(const cpp11::strings& x);

// -----------------------------------------------------------------------------
#endif
