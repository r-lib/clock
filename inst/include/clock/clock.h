#ifndef CLOCK_CLOCK_H
#define CLOCK_CLOCK_H

#include <R_ext/Rdynload.h>

#include <chrono>
#include <string>

#include "decl.h"

namespace rclock {

static
inline
std::chrono::seconds
build(short year,
      unsigned month,
      unsigned day,
      unsigned hour,
      unsigned minute,
      unsigned second) {
  typedef std::chrono::seconds fn_t(short, unsigned, unsigned, unsigned, unsigned, unsigned);
  static fn_t *fn = (fn_t*) R_GetCCallable("clock", "clock_build");
  return fn(year, month, day, hour, minute, second);
}

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
naive_to_sys(const std::chrono::seconds& naive,
             const struct time_zone& zone) {
  typedef struct sys_result fn_t(const std::chrono::seconds&, const struct time_zone&);
  static fn_t *fn = (fn_t*) R_GetCCallable("clock", "clock_naive_to_sys");
  return fn(naive, zone);
}

} // namespace rclock

#endif
