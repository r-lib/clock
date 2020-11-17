#include "civil.h"

/*
 * This function won't do anything if `USE_OS_TZDB=1`. In that case, the date
 * library will auto find the binary version of the TZDB on your computer,
 * but this doesn't work on Windows and the binary parser has some issues.
 * So instead we set `USE_OS_TZDB=0` in the Makevars so `set_install()` is
 * always run.
 */
[[cpp11::register]]
void civil_set_install(SEXP path) {
  if (!r_is_string(path)) {
    r_abort("Internal error: Time zone database installation path should have size 1.");
  }

  const char* c_path = CHAR(STRING_PTR(path)[0]);
  std::string string_path(c_path);

#if !USE_OS_TZDB
  date::set_install(string_path);
#endif
}
