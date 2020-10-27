#include "enums.h"
#include "utils.h"
#include <cstring>
#include <string>

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum day_nonexistant parse_day_nonexistant(sexp x) {
  if (!r_is_string(x)) {
    r_abort("`day_nonexistant` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  if (!strcmp(string, "month-start")) return day_nonexistant::month_start;
  if (!strcmp(string, "month-end")) return day_nonexistant::month_end;
  if (!strcmp(string, "NA")) return day_nonexistant::na;
  if (!strcmp(string, "error")) return day_nonexistant::error;

  r_abort("'%s' is not a recognized `day_nonexistant` option.", string);
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum dst_nonexistant parse_dst_nonexistant(sexp x) {
  if (!r_is_string(x)) {
    r_abort("`dst_nonexistant` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  if (!strcmp(string, "directional")) return dst_nonexistant::directional;
  if (!strcmp(string, "next")) return dst_nonexistant::next;
  if (!strcmp(string, "previous")) return dst_nonexistant::previous;
  if (!strcmp(string, "NA")) return dst_nonexistant::na;
  if (!strcmp(string, "error")) return dst_nonexistant::error;

  r_abort("'%s' is not a recognized `dst_nonexistant` option.", string);
}

// [[ include("enums.h") ]]
enum dst_nonexistant parse_dst_nonexistant_no_directional(sexp x) {
  enum dst_nonexistant out = parse_dst_nonexistant(x);

  if (out == dst_nonexistant::directional) {
    r_abort("'directional' is not allowed for `dst_nonexistant` here.");
  }

  return out;
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum dst_ambiguous parse_dst_ambiguous(sexp x) {
  if (!r_is_string(x)) {
    r_abort("`dst_ambiguous` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  if (!strcmp(string, "directional")) return dst_ambiguous::directional;
  if (!strcmp(string, "earliest")) return dst_ambiguous::earliest;
  if (!strcmp(string, "latest")) return dst_ambiguous::latest;
  if (!strcmp(string, "NA")) return dst_ambiguous::na;
  if (!strcmp(string, "error")) return dst_ambiguous::error;

  r_abort("'%s' is not a recognized `dst_ambiguous` option.", string);
}

// [[ include("enums.h") ]]
enum dst_ambiguous parse_dst_ambiguous_no_directional(sexp x) {
  enum dst_ambiguous out = parse_dst_ambiguous(x);

  if (out == dst_ambiguous::directional) {
    r_abort("'directional' is not allowed for `dst_ambiguous` here.");
  }

  return out;
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum unit parse_unit(sexp x) {
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

  r_abort("'%s' is not a recognized `unit` option.", string);
}
