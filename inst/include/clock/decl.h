#ifndef CLOCK_DECL_H
#define CLOCK_DECL_H

#include <chrono>
#include <stdbool.h>

namespace rclock {

struct time_zone {
  const void* p_time_zone;
};

struct sys_result {
  std::chrono::seconds sys_time;
  bool ok;
};

} // namespace rclock

#endif
