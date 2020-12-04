#ifndef CIVIL_CIVIL_H
#define CIVIL_CIVIL_H

// Include <date/> first to avoid hitting R's `length()` macro
#include <date/date.h>
#include <date/fiscal_year.h>
#include <date/iso_week.h>
#include <date/tz.h>

// Include cpp11 next to avoid <Rinternals.h> being included before cpp11
#include <cpp11.hpp>

// Custom cpp11 typedefs for civil
#include "civil_types.h"

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

#endif
