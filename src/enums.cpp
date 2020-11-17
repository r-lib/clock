#include "enums.h"
#include <cstring>
#include <string>

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum day_nonexistent parse_day_nonexistent(SEXP x) {
  if (!r_is_string(x)) {
    r_abort("`day_nonexistent` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  if (!strcmp(string, "last-time")) return day_nonexistent::last_time;
  if (!strcmp(string, "first-time")) return day_nonexistent::first_time;
  if (!strcmp(string, "last-day")) return day_nonexistent::last_day;
  if (!strcmp(string, "first-day")) return day_nonexistent::first_day;
  if (!strcmp(string, "NA")) return day_nonexistent::na;
  if (!strcmp(string, "error")) return day_nonexistent::error;

  r_abort("'%s' is not a recognized `day_nonexistent` option.", string);
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum dst_nonexistent parse_dst_nonexistent(SEXP x) {
  if (!r_is_string(x)) {
    r_abort("`dst_nonexistent` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  return parse_dst_nonexistent_one(string);
}

// [[ include("enums.h") ]]
enum dst_nonexistent parse_dst_nonexistent_one(const char* x) {
  if (!strcmp(x, "roll-forward")) return dst_nonexistent::roll_forward;
  if (!strcmp(x, "roll-backward")) return dst_nonexistent::roll_backward;
  if (!strcmp(x, "shift-forward")) return dst_nonexistent::shift_forward;
  if (!strcmp(x, "shift-backward")) return dst_nonexistent::shift_backward;
  if (!strcmp(x, "NA")) return dst_nonexistent::na;
  if (!strcmp(x, "error")) return dst_nonexistent::error;

  r_abort("'%s' is not a recognized `dst_nonexistent` option.", x);
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum dst_ambiguous parse_dst_ambiguous(SEXP x) {
  if (!r_is_string(x)) {
    r_abort("`dst_ambiguous` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  return parse_dst_ambiguous_one(string);
}

// [[ include("enums.h") ]]
enum dst_ambiguous parse_dst_ambiguous_one(const char* x) {
  if (!strcmp(x, "earliest")) return dst_ambiguous::earliest;
  if (!strcmp(x, "latest")) return dst_ambiguous::latest;
  if (!strcmp(x, "NA")) return dst_ambiguous::na;
  if (!strcmp(x, "error")) return dst_ambiguous::error;

  r_abort("'%s' is not a recognized `dst_ambiguous` option.", x);
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum unit parse_unit(SEXP x) {
  if (!r_is_string(x)) {
    r_abort("`unit` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  if (!strcmp(string, "year")) return unit::year;
  if (!strcmp(string, "month")) return unit::month;
  if (!strcmp(string, "week")) return unit::week;
  if (!strcmp(string, "day")) return unit::day;
  if (!strcmp(string, "hour")) return unit::hour;
  if (!strcmp(string, "minute")) return unit::minute;
  if (!strcmp(string, "second")) return unit::second;
  if (!strcmp(string, "millisecond")) return unit::millisecond;
  if (!strcmp(string, "microsecond")) return unit::microsecond;
  if (!strcmp(string, "nanosecond")) return unit::nanosecond;

  r_abort("'%s' is not a recognized `unit` option.", string);
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum adjuster parse_adjuster(SEXP x) {
  if (!r_is_string(x)) {
    r_abort("`adjuster` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  if (!strcmp(string, "year")) return adjuster::year;
  if (!strcmp(string, "month")) return adjuster::month;
  if (!strcmp(string, "day")) return adjuster::day;
  if (!strcmp(string, "hour")) return adjuster::hour;
  if (!strcmp(string, "minute")) return adjuster::minute;
  if (!strcmp(string, "second")) return adjuster::second;
  if (!strcmp(string, "nanosecond")) return adjuster::nanosecond;
  if (!strcmp(string, "last_day_of_month")) return adjuster::last_day_of_month;

  r_abort("'%s' is not a recognized `adjuster` option.", string);
}
