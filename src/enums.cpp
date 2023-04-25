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
enum week::start parse_week_start(const cpp11::integers& x) {
  if (x.size() != 1) {
    clock_abort("`start` must be an integer with length 1.");
  }

  const int s = x[0];

  if (s == 1) return week::start::sunday;
  else if (s == 2) return week::start::monday;
  else if (s == 3) return week::start::tuesday;
  else if (s == 4) return week::start::wednesday;
  else if (s == 5) return week::start::thursday;
  else if (s == 6) return week::start::friday;
  else if (s == 7) return week::start::saturday;
  else clock_abort("'%i' is not a recognized `start` option.", s);
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum quarterly::start parse_quarterly_start(const cpp11::integers& x) {
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
enum precision
parse_precision(const cpp11::integers& x) {
  if (x.size() != 1) {
    clock_abort("`precision` must be an integer with length 1.");
  }

  const int elt = x[0];

  if (elt > 10 || elt < 0) {
    clock_abort("`%i` is not a recognized `precision` option.", elt);
  }

  return static_cast<enum precision>(static_cast<unsigned char>(elt));
}

static const std::string chr_year{"year"};
static const std::string chr_quarter{"quarter"};
static const std::string chr_month{"month"};
static const std::string chr_week{"week"};
static const std::string chr_day{"day"};
static const std::string chr_hour{"hour"};
static const std::string chr_minute{"minute"};
static const std::string chr_second{"second"};
static const std::string chr_millisecond{"millisecond"};
static const std::string chr_microsecond{"microsecond"};
static const std::string chr_nanosecond{"nanosecond"};

const std::string&
precision_to_cpp_string(const enum precision& x) {
  switch (x) {
  case precision::year: return chr_year;
  case precision::quarter: return chr_quarter;
  case precision::month: return chr_month;
  case precision::week: return chr_week;
  case precision::day: return chr_day;
  case precision::hour: return chr_hour;
  case precision::minute: return chr_minute;
  case precision::second: return chr_second;
  case precision::millisecond: return chr_millisecond;
  case precision::microsecond: return chr_microsecond;
  case precision::nanosecond: return chr_nanosecond;
  default: never_reached("precision_to_cpp_string");
  }
}

[[cpp11::register]]
cpp11::writable::strings
precision_to_string(const cpp11::integers& precision_int) {
  const enum precision precision_val = parse_precision(precision_int);
  const std::string precision_string = precision_to_cpp_string(precision_val);
  cpp11::writable::strings out{precision_string};
  return out;
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum clock_name parse_clock_name(const cpp11::integers& x) {
  if (x.size() != 1) {
    clock_abort("`clock_name` must be an integer with length 1.");
  }

  const int elt = x[0];

  if (elt > 1 || elt < 0) {
    clock_abort("`%i` is not a recognized `clock_name` option.", elt);
  }

  return static_cast<enum clock_name>(static_cast<unsigned char>(elt));
}

static const std::string chr_sys{"sys"};
static const std::string chr_naive{"naive"};

const std::string&
clock_name_to_cpp_string(const enum clock_name& x) {
  switch (x) {
  case clock_name::sys: return chr_sys;
  case clock_name::naive: return chr_naive;
  default: never_reached("clock_name_to_cpp_string");
  }
}

[[cpp11::register]]
cpp11::writable::strings
clock_to_string(const cpp11::integers& clock_int) {
  const enum clock_name clock_val = parse_clock_name(clock_int);
  const std::string clock_string = clock_name_to_cpp_string(clock_val);
  cpp11::writable::strings out{clock_string};
  return out;
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum decimal_mark parse_decimal_mark(const cpp11::strings& x) {
  if (x.size() != 1) {
    clock_abort("`decimal_mark` must be a string with length 1.");
  }

  std::string string = x[0];

  if (string == ".") return decimal_mark::period;
  if (string == ",") return decimal_mark::comma;

  clock_abort("'%s' is not a recognized `decimal_mark` option.", string.c_str());
}
