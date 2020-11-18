#ifndef CIVIL_CIVIL_H
#define CIVIL_CIVIL_H

// Include <date/> first to avoid hitting R's `length()` macro
#include <date/date.h>
#include <date/tz.h>

// Include cpp11 next to avoid <Rinternals.h> being included before cpp11
#include <cpp11.hpp>

// Custom cpp11 typedefs for civil
#include "civil_types.h"

// Then include common utility headers
#include "r.h"
#include "utils-rlib.h"

#endif
