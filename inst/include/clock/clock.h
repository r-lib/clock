#ifndef CLOCK_CLOCK_H
#define CLOCK_CLOCK_H

#include <R_ext/Rdynload.h>

#include <chrono>
#include <string>

#include <date/date.h>

#include "decl.h"

namespace rclock {

static
inline
struct time_zone
zone_name_load(const std::string& zone_name) {
  typedef struct time_zone fn_t(const std::string&);
  static fn_t *fn = (fn_t*) R_GetCCallable("clock", "clock_zone_name_load");
  return fn(zone_name);
}

static
inline
struct sys_result
local_to_sys(const date::local_seconds& lt,
             const struct time_zone& zone) {
  typedef struct sys_result fn_t(const date::local_seconds&, const struct time_zone&);
  static fn_t *fn = (fn_t*) R_GetCCallable("clock", "clock_local_to_sys");
  return fn(lt, zone);
}

} // namespace rclock

#endif
