#ifndef CLOCK_RCRD_H
#define CLOCK_RCRD_H

#include "clock.h"

SEXP new_clock_rcrd_from_fields(SEXP fields, SEXP names, SEXP classes);

SEXP clock_rcrd_restore(SEXP x, SEXP to, SEXP classes);

#endif
