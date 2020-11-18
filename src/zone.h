#ifndef CIVIL_ZONE_H
#define CIVIL_ZONE_H

#include "civil.h"

cpp11::writable::strings zone_standardize(const cpp11::strings& zone);

/*
 * Load a time zone name, or throw an R error if it can't be loaded
 */
const date::time_zone* zone_name_load(const std::string& zone_name);

#endif
