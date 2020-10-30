#ifndef CIVIL_RESOLVE_H
#define CIVIL_RESOLVE_H

#include "r.h"
#include "enums.h"
#include <date/date.h>

// -----------------------------------------------------------------------------

static inline void resolve_day_nonexistent_start_keep(date::year_month_day& ymd);
static inline void resolve_day_nonexistent_start(date::year_month_day& ymd, std::chrono::seconds& tod);
static inline void resolve_day_nonexistent_end_keep(date::year_month_day& ymd);
static inline void resolve_day_nonexistent_end(date::year_month_day& ymd, std::chrono::seconds& tod);
static inline void resolve_day_nonexistent_na(bool& na);
static inline void resolve_day_nonexistent_error(r_ssize i);

/*
 * Resolve a `!ymd.ok()` issue using `day_nonexistent`.
 * Directly updates `ymd` and `tod` as required.
 */
static inline void resolve_day_nonexistent(r_ssize i,
                                           const enum day_nonexistent& day_nonexistent,
                                           date::year_month_day& ymd,
                                           std::chrono::seconds& tod,
                                           bool& na) {
  switch (day_nonexistent) {
  case day_nonexistent::start_keep: {
    return resolve_day_nonexistent_start_keep(ymd);
  }
  case day_nonexistent::start: {
    return resolve_day_nonexistent_start(ymd, tod);
  }
  case day_nonexistent::end_keep: {
    return resolve_day_nonexistent_end_keep(ymd);
  }
  case day_nonexistent::end: {
    return resolve_day_nonexistent_end(ymd, tod);
  }
  case day_nonexistent::na: {
    return resolve_day_nonexistent_na(na);
  }
  case day_nonexistent::error: {
    resolve_day_nonexistent_error(i);
  }
  }
}

// First day of next month - keep time of day
static inline void resolve_day_nonexistent_start_keep(date::year_month_day& ymd) {
  ymd = ymd.year() / (ymd.month() + date::months(1)) / date::day(1);
}

// First day of next month - cancels out time of day
static inline void resolve_day_nonexistent_start(date::year_month_day& ymd, std::chrono::seconds& tod) {
  resolve_day_nonexistent_start_keep(ymd);
  tod = std::chrono::seconds{0};
}

// Last day of current month - keep time of day
static inline void resolve_day_nonexistent_end_keep(date::year_month_day& ymd) {
  ymd = ymd.year() / ymd.month() / date::last;
}

// First day of next month - shifts time of day to last second
static inline void resolve_day_nonexistent_end(date::year_month_day& ymd, std::chrono::seconds& tod) {
  resolve_day_nonexistent_end_keep(ymd);
  tod = std::chrono::seconds{86400 - 1};
}

static inline void resolve_day_nonexistent_na(bool& na) {
  na = true;
}

static inline void resolve_day_nonexistent_error(r_ssize i) {
  r_abort("nonexistent day found at location %i.", (int) i + 1);
}

#endif
