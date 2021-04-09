#ifndef CLOCK_DECL_H
#define CLOCK_DECL_H

#include <stdbool.h>

#include <date/date.h>

namespace rclock {

struct sys_result {
  date::sys_seconds st;
  bool ok;
};

} // namespace rclock

#endif
