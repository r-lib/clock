#include "enums.h"
// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum day_nonexistant parse_day_nonexistant(cpp11::strings x) {
  if (Rf_length(x) != 1) {
    cpp11::stop("`day_nonexistant` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  if (!strcmp(string, "month-start")) return day_nonexistant::month_start;
  if (!strcmp(string, "month-end")) return day_nonexistant::month_end;
  if (!strcmp(string, "NA")) return day_nonexistant::na;
  if (!strcmp(string, "error")) return day_nonexistant::error;

  std::string cpp_string = string;

  cpp11::stop("'" + cpp_string + "' is not a recognized `day_nonexistant` option.");
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum dst_nonexistant parse_dst_nonexistant(cpp11::strings x) {
  if (Rf_length(x) != 1) {
    cpp11::stop("`dst_nonexistant` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  if (!strcmp(string, "next")) return dst_nonexistant::next;
  if (!strcmp(string, "previous")) return dst_nonexistant::previous;
  if (!strcmp(string, "NA")) return dst_nonexistant::na;
  if (!strcmp(string, "error")) return dst_nonexistant::error;

  std::string cpp_string = string;

  cpp11::stop("'" + cpp_string + "' is not a recognized `dst_nonexistant` option.");
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum dst_ambiguous parse_dst_ambiguous(cpp11::strings x) {
  if (Rf_length(x) != 1) {
    cpp11::stop("`dst_ambiguous` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  if (!strcmp(string, "earliest")) return dst_ambiguous::earliest;
  if (!strcmp(string, "latest")) return dst_ambiguous::latest;
  if (!strcmp(string, "NA")) return dst_ambiguous::na;
  if (!strcmp(string, "error")) return dst_ambiguous::error;

  std::string cpp_string = string;

  cpp11::stop("'" + cpp_string + "' is not a recognized `dst_ambiguous` option.");
}

// -----------------------------------------------------------------------------

// [[ include("enums.h") ]]
enum unit parse_unit(cpp11::strings x) {
  if (Rf_length(x) != 1) {
    Rf_errorcall(R_NilValue, "`unit` must be a string with length 1.");
  }

  const char* string = CHAR(STRING_ELT(x, 0));

  if (!strcmp(string, "year")) return unit::year;
  if (!strcmp(string, "month")) return unit::month;
  if (!strcmp(string, "week")) return unit::week;
  if (!strcmp(string, "yday")) return unit::yday;
  if (!strcmp(string, "day")) return unit::day;
  if (!strcmp(string, "hour")) return unit::hour;
  if (!strcmp(string, "minute")) return unit::minute;
  if (!strcmp(string, "second")) return unit::second;

  std::string cpp_string = string;

  cpp11::stop("'" + cpp_string + "' is not a recognized `unit` option.");
}
