#ifndef CIVIL_ZONE_H
#define CIVIL_ZONE_H

#include <cpp11.hpp>
#include <date/tz.h>

/*
 * Time zone name from `tzone` attribute
 */
std::string civil_zone_name_from_tzone(cpp11::sexp tzone);

/*
 * Time zone name from POSIXct
 */
std::string civil_zone_name_from_posixt(cpp11::sexp x);

/*
 * Load a time zone name, or throw an R error if it can't be loaded
 */
const date::time_zone* civil_zone_name_load(const std::string& zone_name);

#endif
