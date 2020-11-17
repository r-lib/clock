#ifndef CIVIL_CONVERSION_H
#define CIVIL_CONVERSION_H

#include "civil.h"
#include "enums.h"

date::sys_seconds convert_local_to_sys(const date::local_seconds& lsec,
                                       const date::time_zone* p_zone,
                                       r_ssize i,
                                       const enum dst_nonexistent& dst_nonexistent,
                                       const enum dst_ambiguous& dst_ambiguous,
                                       bool& na);

date::sys_seconds convert_local_to_sys(const date::local_seconds& lsec,
                                       const date::time_zone* p_zone,
                                       r_ssize i,
                                       const enum dst_nonexistent& dst_nonexistent,
                                       const enum dst_ambiguous& dst_ambiguous,
                                       bool& na,
                                       std::chrono::nanoseconds& nanos);

#endif
