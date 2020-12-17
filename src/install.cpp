#include "clock.h"
#include "utils.h"

/*
 * This function won't do anything if `USE_OS_TZDB=1`. In that case, the date
 * library will auto find the binary version of the TZDB on your computer,
 * but this doesn't work on Windows and the binary parser has some issues.
 * So instead we set `USE_OS_TZDB=0` in the Makevars so `set_install()` is
 * always run.
 */
[[cpp11::register]]
void clock_set_install(const cpp11::strings& path) {
  if (path.size() != 1) {
    clock_abort("Internal error: Time zone database installation path should have size 1.");
  }

  std::string string_path(path[0]);

#if !USE_OS_TZDB
  date::set_install(string_path);
#endif
}
