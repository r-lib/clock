#ifndef CIVIL_CONVERSION_H
#define CIVIL_CONVERSION_H

#include "civil.h"
#include "enums.h"

date::sys_seconds convert_local_to_sys(const date::local_seconds& lsec,
                                       const date::time_zone* p_zone,
                                       const r_ssize& i,
                                       const enum dst_nonexistent& dst_nonexistent_val,
                                       const enum dst_ambiguous& dst_ambiguous_val,
                                       bool& na);

date::sys_seconds convert_local_to_sys(const date::local_seconds& lsec,
                                       const date::time_zone* p_zone,
                                       const r_ssize& i,
                                       const enum dst_nonexistent& dst_nonexistent_val,
                                       const enum dst_ambiguous& dst_ambiguous_val,
                                       const enum precision& precision_val,
                                       bool& na,
                                       std::chrono::nanoseconds& nanos);

#endif
