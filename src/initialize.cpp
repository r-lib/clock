#include "civil.h"

extern void r_init_utils();
extern void civil_init_utils();

[[cpp11::register]]
cpp11::sexp civil_init() {
  r_init_utils();
  civil_init_utils();
  return r_null;
}
