#include "enums.h"
#include "utils.h"
#include <cstring>
#include <string>

// -----------------------------------------------------------------------------

enum invalid parse_invalid(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`invalid` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == "last-time") return invalid::last_time;
  if (string == "first-time") return invalid::first_time;
  if (string == "last-day") return invalid::last_day;
  if (string == "first-day") return invalid::first_day;
  if (string == "NA") return invalid::na;
  if (string == "error") return invalid::error;

  clock_abort("'%s' is not a recognized `invalid` option.", string.c_str());
}

// [[ include("enums.h") ]]
enum day_nonexistent parse_day_nonexistent(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`day_nonexistent` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == "last-time") return day_nonexistent::last_time;
  if (string == "first-time") return day_nonexistent::first_time;
  if (string == "last-day") return day_nonexistent::last_day;
  if (string == "first-day") return day_nonexistent::first_day;
  if (string == "NA") return day_nonexistent::na;
  if (string == "error") return day_nonexistent::error;

  clock_abort("'%s' is not a recognized `day_nonexistent` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum dst_nonexistent parse_dst_nonexistent(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`dst_nonexistent` must be a string with length 1.");
  }

  return parse_dst_nonexistent_one(x[0]);
}

// [[ include("enums.h") ]]
enum dst_nonexistent parse_dst_nonexistent_one(const cpp11::r_string& x) {
  std::string string(x);

  if (string == "roll-forward") return dst_nonexistent::roll_forward;
  if (string == "roll-backward") return dst_nonexistent::roll_backward;
  if (string == "shift-forward") return dst_nonexistent::shift_forward;
  if (string == "shift-backward") return dst_nonexistent::shift_backward;
  if (string == "NA") return dst_nonexistent::na;
  if (string == "error") return dst_nonexistent::error;

  clock_abort("'%s' is not a recognized `dst_nonexistent` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum dst_ambiguous parse_dst_ambiguous(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`dst_ambiguous` must be a string with length 1.");
  }

  return parse_dst_ambiguous_one(x[0]);
}

// [[ include("enums.h") ]]
enum dst_ambiguous parse_dst_ambiguous_one(const cpp11::r_string& x) {
  std::string string(x);

  if (string == "earliest") return dst_ambiguous::earliest;
  if (string == "latest") return dst_ambiguous::latest;
  if (string == "NA") return dst_ambiguous::na;
  if (string == "error") return dst_ambiguous::error;

  clock_abort("'%s' is not a recognized `dst_ambiguous` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum unit parse_unit(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`unit` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == "year") return unit::year;
  if (string == "quarter") return unit::quarter;
  if (string == "month") return unit::month;
  if (string == "week") return unit::week;
  if (string == "day") return unit::day;
  if (string == "hour") return unit::hour;
  if (string == "minute") return unit::minute;
  if (string == "second") return unit::second;
  if (string == "millisecond") return unit::millisecond;
  if (string == "microsecond") return unit::microsecond;
  if (string == "nanosecond") return unit::nanosecond;

  clock_abort("'%s' is not a recognized `unit` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum adjuster parse_adjuster(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`adjuster` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == "year") return adjuster::year;
  if (string == "month") return adjuster::month;
  if (string == "day") return adjuster::day;
  if (string == "hour") return adjuster::hour;
  if (string == "minute") return adjuster::minute;
  if (string == "second") return adjuster::second;
  if (string == "millisecond") return adjuster::millisecond;
  if (string == "microsecond") return adjuster::microsecond;
  if (string == "nanosecond") return adjuster::nanosecond;
  if (string == "weeknum") return adjuster::weeknum;
  if (string == "weekday") return adjuster::weekday;
  if (string == "weekday_index") return adjuster::weekday_index;
  if (string == "quarternum") return adjuster::quarternum;
  if (string == "quarterday") return adjuster::quarterday;
  if (string == "last_weekday_index_of_month") return adjuster::last_weekday_index_of_month;
  if (string == "last_weeknum_of_year") return adjuster::last_weeknum_of_year;
  if (string == "last_day_of_quarter") return adjuster::last_day_of_quarter;
  if (string == "last_day_of_month") return adjuster::last_day_of_month;

  clock_abort("'%s' is not a recognized `adjuster` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum precision parse_precision(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`precision` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == "second") return precision::second;
  if (string == "millisecond") return precision::millisecond;
  if (string == "microsecond") return precision::microsecond;
  if (string == "nanosecond") return precision::nanosecond;

  clock_abort("'%s' is not a recognized `precision` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum precision2 parse_precision2(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`precision` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == "year") return precision2::year;
  if (string == "quarter") return precision2::quarter;
  if (string == "month") return precision2::month;
  if (string == "week") return precision2::week;
  if (string == "day") return precision2::day;
  if (string == "hour") return precision2::hour;
  if (string == "minute") return precision2::minute;
  if (string == "second") return precision2::second;
  if (string == "millisecond") return precision2::millisecond;
  if (string == "microsecond") return precision2::microsecond;
  if (string == "nanosecond") return precision2::nanosecond;

  clock_abort("'%s' is not a recognized `precision` option.", string.c_str());
}
