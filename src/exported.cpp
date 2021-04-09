#include "clock.h"
#include "zone.h"
#include "clock/decl.h"
#include <R_ext/Rdynload.h> // For DllInfo on R 3.3

// -----------------------------------------------------------------------------

std::chrono::seconds
clock_build(short year,
            unsigned month,
            unsigned day,
            unsigned hour,
            unsigned minute,
            unsigned second) {
  const date::local_days ld{date::year{year} / month / day};

  return std::chrono::seconds{second} +
    std::chrono::minutes{minute} +
    std::chrono::hours{hour} +
    ld.time_since_epoch();
}

// -----------------------------------------------------------------------------

struct rclock::time_zone
clock_zone_name_load(const std::string& zone_name) {
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  const struct rclock::time_zone zone {
    static_cast<const void*>(p_time_zone)
  };

  return zone;
}

// -----------------------------------------------------------------------------

struct rclock::sys_result
clock_naive_to_sys(const std::chrono::seconds& naive,
                   const struct rclock::time_zone& zone) {
  const date::local_seconds lt{naive};
  const date::time_zone* p_time_zone = static_cast<const date::time_zone*>(zone.p_time_zone);

  const date::local_info info = p_time_zone->get_info(lt);

  struct rclock::sys_result out;
  out.ok = true;

  switch (info.result) {
  case date::local_info::unique: {
    out.sys_time = lt.time_since_epoch() - info.first.offset;
    break;
  }
  case date::local_info::ambiguous: {
    // Choose `earliest` of the two ambiguous times
    out.sys_time = lt.time_since_epoch() - info.first.offset;
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
  R_RegisterCCallable("clock", "clock_build",          (DL_FUNC)clock_build);
  R_RegisterCCallable("clock", "clock_zone_name_load", (DL_FUNC)clock_zone_name_load);
  R_RegisterCCallable("clock", "clock_naive_to_sys",   (DL_FUNC)clock_naive_to_sys);
}
