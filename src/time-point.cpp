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

  if (precision_val < precision::day) {
    clock_abort("`precision` must be at least 'day' precision.");
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
