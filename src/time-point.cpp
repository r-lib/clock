#include "clock.h"
#include "enums.h"
#include "utils.h"
#include "rcrd.h"

[[cpp11::register]]
SEXP
new_time_point_from_fields(SEXP fields,
                           const cpp11::integers& precision_int,
                           const cpp11::integers& clock_int,
                           SEXP names) {
  const enum precision precision_val = parse_precision(precision_int);
  const enum clock_name clock_val = parse_clock_name(clock_int);

  const r_ssize n_fields = Rf_xlength(fields);

  switch (precision_val) {
  case precision::year:
  case precision::quarter:
  case precision::month:
  case precision::week: {
    clock_abort("`precision` must be at least 'day' precision.");
  }
  case precision::day: {
    if (n_fields != 1) {
      clock_abort("`fields` must have 1 field for day precision.");
    }
    break;
  }
  case precision::hour:
  case precision::minute:
  case precision::second: {
    if (n_fields != 2) {
      clock_abort("`fields` must have 2 fields for [hour, second] precision.");
    }
    break;
  }
  case precision::millisecond:
  case precision::microsecond:
  case precision::nanosecond: {
    if (n_fields != 3) {
      clock_abort("`fields` must have 3 fields for [millisecond, nanosecond] precision.");
    }
    break;
  }
  default: {
    never_reached("new_time_point_from_fields");
  }
  }

  SEXP classes;

  switch (clock_val) {
  case clock_name::naive: classes = classes_naive_time; break;
  case clock_name::sys: classes = classes_sys_time; break;
  default: clock_abort("Internal error: Unknown clock.");
  }

  SEXP out = PROTECT(new_clock_rcrd_from_fields(fields, names, classes));

  Rf_setAttrib(out, syms_precision, precision_int);
  Rf_setAttrib(out, syms_clock, clock_int);

  UNPROTECT(1);
  return out;
}
