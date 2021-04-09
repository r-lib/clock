#ifndef CLOCK_DECL_H
#define CLOCK_DECL_H

#include <stdbool.h>

#include <date/date.h>

namespace rclock {

struct time_zone {
  const void* p_time_zone;
};

struct sys_result {
  date::sys_seconds st;
  bool ok;
};

} // namespace rclock

#endif
