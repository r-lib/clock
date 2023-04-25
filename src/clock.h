#ifndef CLOCK_CLOCK_H
#define CLOCK_CLOCK_H

// Include date tooling first to avoid hitting R's `length()` macro
#include <tzdb/tzdb.h>
#include <tzdb/date.h>
#include <tzdb/tz.h>
#include <tzdb/iso_week.h>

// Include date extensions next
#include "quarterly.h"
#include "ordinal.h"
#include "week.h"

// Include cpp11 next to avoid <Rinternals.h> being included before cpp11
#include <cpp11.hpp>

// Then include common utility headers
#include <R.h>
#include <Rinternals.h>
#include <stdint.h>
#include <stdbool.h>

#define r_null R_NilValue
#define r_ssize R_xlen_t

#define r_dbl_na NA_REAL
#define r_int_na NA_INTEGER
#define r_chr_na NA_STRING
#define r_lgl_na NA_LOGICAL

#endif
