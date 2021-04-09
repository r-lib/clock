#ifndef CLOCK_CLOCK_H
#define CLOCK_CLOCK_H

#include <R_ext/Rdynload.h>

#include <chrono>
#include <string>

#include <date/date.h>
#include <date/tz.h>

namespace rclock {

static
inline
const date::time_zone*
locate_zone(const std::string& zone_name) {
  typedef const date::time_zone* fn_t(const std::string&);
  static fn_t *fn = (fn_t*) R_GetCCallable("clock", "clock_locate_zone");
  return fn(zone_name);
}

static
inline
date::local_info
get_local_info(const date::local_seconds& lt,
               const date::time_zone* p_time_zone) {
  typedef date::local_info fn_t(const date::local_seconds&, const date::time_zone*);
  static fn_t *fn = (fn_t*) R_GetCCallable("clock", "clock_get_local_info");
  return fn(lt, p_time_zone);
}

} // namespace rclock

#endif
