#ifndef CIVIL_CONVERSION_H
#define CIVIL_CONVERSION_H

#include "r.h"
#include "enums.h"
#include <date/tz.h>

double convert_local_seconds_to_posixt(const date::local_seconds& lsec,
                                       const date::time_zone* p_zone,
                                       r_ssize i,
                                       const enum dst_direction& dst_direction,
                                       const enum dst_nonexistant& dst_nonexistant,
                                       const enum dst_ambiguous& dst_ambiguous);

#endif
