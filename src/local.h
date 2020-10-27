#ifndef CIVIL_LOCAL_H
#define CIVIL_LOCAL_H

#include "r.h"
#include "enums.h"

sexp civil_add_local_impl(sexp x,
                          sexp years,
                          sexp months,
                          sexp weeks,
                          sexp days,
                          sexp hours,
                          sexp minutes,
                          sexp seconds,
                          dst_nonexistant dst_nonexistant,
                          dst_ambiguous dst_ambiguous,
                          r_ssize size);

#endif
