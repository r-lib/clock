#ifndef CLOCK_ZONE_H
#define CLOCK_ZONE_H

#include "clock.h"
#include "utils.h"

static
inline
void
zone_size_validate(const cpp11::strings& zone) {
  if (zone.size() != 1) {
    clock_abort("`zone` must be a single string.");
  }
}

std::string zone_name_current();

/*
 * Load a time zone name, or throw an R error if it can't be loaded
 */
const date::time_zone* zone_name_load(const std::string& zone_name);

#endif
