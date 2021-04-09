#include "clock.h"
#include "zone.h"
#include "clock/decl.h"
#include <R_ext/Rdynload.h> // For DllInfo on R 3.3

// -----------------------------------------------------------------------------

const date::time_zone*
clock_zone_name_load(const std::string& zone_name) {
  return zone_name_load(zone_name);
}

// -----------------------------------------------------------------------------

struct rclock::sys_result
clock_local_to_sys(const date::local_seconds& lt,
                   const date::time_zone* p_time_zone) {
  const date::local_info info = p_time_zone->get_info(lt);

  struct rclock::sys_result out;

  switch (info.result) {
  case date::local_info::unique: {
    out.st = date::sys_seconds{lt.time_since_epoch() - info.first.offset};
    out.ok = true;
    break;
  }
  case date::local_info::ambiguous: {
    // Choose `earliest` of the two ambiguous times
    out.st = date::sys_seconds{lt.time_since_epoch() - info.first.offset};
    out.ok = true;
    break;
  }
  case date::local_info::nonexistent: {
    // Client should return `NA`
    out.ok = false;
    break;
  }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::init]]
void export_clock_callables(DllInfo* dll){
  R_RegisterCCallable("clock", "clock_zone_name_load", (DL_FUNC)clock_zone_name_load);
  R_RegisterCCallable("clock", "clock_local_to_sys",   (DL_FUNC)clock_local_to_sys);
}
