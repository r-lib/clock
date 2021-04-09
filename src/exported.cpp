#include "clock.h"
#include <R_ext/Rdynload.h> // For DllInfo on R 3.3

// -----------------------------------------------------------------------------

/*
 * Look up a time zone by name
 *
 * Returns a `time_zone` pointer for use in `clock_get_local_info()`.
 *
 * `""` should not be passed through here. If you need to pass through the
 * system time zone, materialize its value with `Sys.timezone()` first.
 *
 * Throws a `std::runtime_error()` with an informative message if the
 * `zone_name` does not exist in the database.
 */
const date::time_zone*
clock_locate_zone(const std::string& zone_name) {
  return date::locate_zone(zone_name);
}

// -----------------------------------------------------------------------------

/*
 * Pair a local time with a time zone to compute all local time information
 */
date::local_info
clock_get_local_info(const date::local_seconds& lt,
                     const date::time_zone* p_time_zone) {
  return p_time_zone->get_info(lt);
}

// -----------------------------------------------------------------------------

[[cpp11::init]]
void export_clock_callables(DllInfo* dll){
  R_RegisterCCallable("clock", "clock_locate_zone",    (DL_FUNC)clock_locate_zone);
  R_RegisterCCallable("clock", "clock_get_local_info", (DL_FUNC)clock_get_local_info);
}
