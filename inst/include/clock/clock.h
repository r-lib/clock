#ifndef CLOCK_CLOCK_H
#define CLOCK_CLOCK_H

#include <R_ext/Rdynload.h>

#include <chrono>
#include <string>

#include <date/date.h>
#include <date/tz.h>

#include "decl.h"

namespace rclock {

static
inline
const date::time_zone*
zone_name_load(const std::string& zone_name) {
  typedef const date::time_zone* fn_t(const std::string&);
  static fn_t *fn = (fn_t*) R_GetCCallable("clock", "clock_zone_name_load");
  return fn(zone_name);
}

static
inline
struct sys_result
local_to_sys(const date::local_seconds& lt,
             const date::time_zone* p_time_zone) {
  typedef struct sys_result fn_t(const date::local_seconds&, const date::time_zone*);
  static fn_t *fn = (fn_t*) R_GetCCallable("clock", "clock_local_to_sys");
  return fn(lt, p_time_zone);
}

} // namespace rclock

#endif
