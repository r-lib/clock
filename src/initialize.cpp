#include "r.h"

extern void r_init_utils_rlib();
extern void civil_init_utils();

[[cpp11::register]]
SEXP civil_init() {
  r_init_utils_rlib();
  civil_init_utils();
  return r_null;
}
