#ifndef CIVIL_RESOLVE_H
#define CIVIL_RESOLVE_H

#include "r.h"
#include "enums.h"
#include <date/date.h>

// -----------------------------------------------------------------------------

static inline void resolve_day_nonexistant_start_keep(date::year_month_day& ymd);
static inline void resolve_day_nonexistant_start(date::year_month_day& ymd, std::chrono::seconds& tod);
static inline void resolve_day_nonexistant_end_keep(date::year_month_day& ymd);
static inline void resolve_day_nonexistant_end(date::year_month_day& ymd, std::chrono::seconds& tod);
static inline void resolve_day_nonexistant_na(bool& na);
static inline void resolve_day_nonexistant_error(r_ssize i);

/*
 * Resolve a `!ymd.ok()` issue using `day_nonexistant`.
 * Directly updates `ymd` and `tod` as required.
 */
static inline void resolve_day_nonexistant(r_ssize i,
                                           const enum day_nonexistant& day_nonexistant,
                                           date::year_month_day& ymd,
                                           std::chrono::seconds& tod,
                                           bool& na) {
  switch (day_nonexistant) {
  case day_nonexistant::start_keep: {
    return resolve_day_nonexistant_start_keep(ymd);
  }
  case day_nonexistant::start: {
    return resolve_day_nonexistant_start(ymd, tod);
  }
  case day_nonexistant::end_keep: {
    return resolve_day_nonexistant_end_keep(ymd);
  }
  case day_nonexistant::end: {
    return resolve_day_nonexistant_end(ymd, tod);
  }
  case day_nonexistant::na: {
    return resolve_day_nonexistant_na(na);
  }
  case day_nonexistant::error: {
    resolve_day_nonexistant_error(i);
  }
  }
}

// First day of next month - keep time of day
static inline void resolve_day_nonexistant_start_keep(date::year_month_day& ymd) {
  ymd = ymd.year() / (ymd.month() + date::months(1)) / date::day(1);
}

// First day of next month - cancels out time of day
static inline void resolve_day_nonexistant_start(date::year_month_day& ymd, std::chrono::seconds& tod) {
  resolve_day_nonexistant_start_keep(ymd);
  tod = std::chrono::seconds{0};
}

// Last day of current month - keep time of day
static inline void resolve_day_nonexistant_end_keep(date::year_month_day& ymd) {
  ymd = ymd.year() / ymd.month() / date::last;
}

// First day of next month - shifts time of day to last second
static inline void resolve_day_nonexistant_end(date::year_month_day& ymd, std::chrono::seconds& tod) {
  resolve_day_nonexistant_end_keep(ymd);
  tod = std::chrono::seconds{86400 - 1};
}

static inline void resolve_day_nonexistant_na(bool& na) {
  na = true;
}

static inline void resolve_day_nonexistant_error(r_ssize i) {
  r_abort("Nonexistant day found at location %i.", (int) i + 1);
}

#endif
