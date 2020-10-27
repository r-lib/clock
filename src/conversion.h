#ifndef CIVIL_CONVERSION_H
#define CIVIL_CONVERSION_H

#include "r.h"
#include "enums.h"
#include <date/tz.h>

double civil_local_seconds_to_posixt(const date::local_seconds& lsec,
                                     const date::time_zone* p_zone,
                                     const std::chrono::seconds& duration,
                                     r_ssize i,
                                     const enum dst_nonexistant& dst_nonexistant,
                                     const enum dst_ambiguous& dst_ambiguous);

#endif
