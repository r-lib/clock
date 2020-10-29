#ifndef CIVIL_ZONE_H
#define CIVIL_ZONE_H

#include "r.h"
#include <date/tz.h>

SEXP zone_standardize(SEXP zone);
std::string zone_unwrap(sexp zone);

/*
 * Load a time zone name, or throw an R error if it can't be loaded
 */
const date::time_zone* zone_name_load(const std::string& zone_name);

#endif
