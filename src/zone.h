#ifndef CLOCK_ZONE_H
#define CLOCK_ZONE_H

#include "clock.h"

cpp11::writable::strings zone_standardize(const cpp11::strings& zone);

std::string zone_name_current();

/*
 * Load a time zone name, or throw an R error if it can't be loaded
 */
const date::time_zone* zone_name_load(const std::string& zone_name);

#endif
