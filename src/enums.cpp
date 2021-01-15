#include "enums.h"
#include "utils.h"
#include <cstring>
#include <string>

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum invalid parse_invalid(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`invalid` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == "previous") return invalid::previous;
  if (string == "next") return invalid::next;
  if (string == "overflow") return invalid::overflow;
  if (string == "previous-day") return invalid::previous_day;
  if (string == "next-day") return invalid::next_day;
  if (string == "overflow-day") return invalid::overflow_day;
  if (string == "NA") return invalid::na;
  if (string == "error") return invalid::error;

  clock_abort("'%s' is not a recognized `invalid` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum nonexistent parse_nonexistent_one(const cpp11::r_string& x) {
  std::string string(x);

  if (string == "roll-forward") return nonexistent::roll_forward;
  if (string == "roll-backward") return nonexistent::roll_backward;
  if (string == "shift-forward") return nonexistent::shift_forward;
  if (string == "shift-backward") return nonexistent::shift_backward;
  if (string == "NA") return nonexistent::na;
  if (string == "error") return nonexistent::error;

  clock_abort("'%s' is not a recognized `nonexistent` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum ambiguous parse_ambiguous_one(const cpp11::r_string& x) {
  std::string string(x);

  if (string == "earliest") return ambiguous::earliest;
  if (string == "latest") return ambiguous::latest;
  if (string == "NA") return ambiguous::na;
  if (string == "error") return ambiguous::error;

  clock_abort("'%s' is not a recognized `ambiguous` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum component parse_component(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`component` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == "year") return component::year;
  if (string == "quarter") return component::quarter;
  if (string == "month") return component::month;
  if (string == "week") return component::week;
  if (string == "day") return component::day;
  if (string == "hour") return component::hour;
  if (string == "minute") return component::minute;
  if (string == "second") return component::second;
  if (string == "millisecond") return component::millisecond;
  if (string == "microsecond") return component::microsecond;
  if (string == "nanosecond") return component::nanosecond;
  if (string == "index") return component::index;

  clock_abort("'%s' is not a recognized `component` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum quarterly::start parse_start(const cpp11::integers& x) {
  if (x.size() != 1) {
    clock_abort("`start` must be an integer with length 1.");
  }

  const int s = x[0];

  if (s == 1) return quarterly::start::january;
  else if (s == 2) return quarterly::start::february;
  else if (s == 3) return quarterly::start::march;
  else if (s == 4) return quarterly::start::april;
  else if (s == 5) return quarterly::start::may;
  else if (s == 6) return quarterly::start::june;
  else if (s == 7) return quarterly::start::july;
  else if (s == 8) return quarterly::start::august;
  else if (s == 9) return quarterly::start::september;
  else if (s == 10) return quarterly::start::october;
  else if (s == 11) return quarterly::start::november;
  else if (s == 12) return quarterly::start::december;
  else clock_abort("'%i' is not a recognized `start` option.", s);
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum precision parse_precision(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`precision` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == "year") return precision::year;
  if (string == "quarter") return precision::quarter;
  if (string == "month") return precision::month;
  if (string == "week") return precision::week;
  if (string == "day") return precision::day;
  if (string == "hour") return precision::hour;
  if (string == "minute") return precision::minute;
  if (string == "second") return precision::second;
  if (string == "millisecond") return precision::millisecond;
  if (string == "microsecond") return precision::microsecond;
  if (string == "nanosecond") return precision::nanosecond;

  clock_abort("'%s' is not a recognized `precision` option.", string.c_str());
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum clock_name parse_clock_name(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`clock` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == "sys") return clock_name::sys;
  if (string == "naive") return clock_name::naive;

  clock_abort("'%s' is not a recognized `clock` option.", string.c_str());
}
