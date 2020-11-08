#ifndef CIVIL_RESOLVE_H
#define CIVIL_RESOLVE_H

#include "r.h"
#include "enums.h"
#include <date/date.h>

// -----------------------------------------------------------------------------

static inline void resolve_day_nonexistent_first_day(date::year_month_day& ymd);
static inline void resolve_day_nonexistent_first_time(std::chrono::seconds& tod);
static inline void resolve_day_nonexistent_last_day(date::year_month_day& ymd);
static inline void resolve_day_nonexistent_last_time(std::chrono::seconds& tod);
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
  case day_nonexistent::first_day: {
    return resolve_day_nonexistent_first_day(ymd);
  }
  case day_nonexistent::first_time: {
    resolve_day_nonexistent_first_day(ymd);
    resolve_day_nonexistent_first_time(tod);
    return;
  }
  case day_nonexistent::last_day: {
    return resolve_day_nonexistent_last_day(ymd);
  }
  case day_nonexistent::last_time: {
    resolve_day_nonexistent_last_day(ymd);
    resolve_day_nonexistent_last_time(tod);
    return;
  }
  case day_nonexistent::na: {
    return resolve_day_nonexistent_na(na);
  }
  case day_nonexistent::error: {
    resolve_day_nonexistent_error(i);
  }
  }
}

static inline void resolve_day_nonexistent_ymd(r_ssize i,
                                               const enum day_nonexistent& day_nonexistent,
                                               date::year_month_day& ymd,
                                               bool& na) {
  switch (day_nonexistent) {
  case day_nonexistent::first_day: {
    return resolve_day_nonexistent_first_day(ymd);
  }
  case day_nonexistent::first_time: {
    return resolve_day_nonexistent_first_day(ymd);
  }
  case day_nonexistent::last_day: {
    return resolve_day_nonexistent_last_day(ymd);
  }
  case day_nonexistent::last_time: {
    return resolve_day_nonexistent_last_day(ymd);
  }
  case day_nonexistent::na: {
    return resolve_day_nonexistent_na(na);
  }
  case day_nonexistent::error: {
    resolve_day_nonexistent_error(i);
  }
  }
}

static inline void resolve_day_nonexistent_tod(const enum day_nonexistent& day_nonexistent,
                                               std::chrono::seconds& tod) {
  switch (day_nonexistent) {
  case day_nonexistent::first_time: {
    return resolve_day_nonexistent_first_time(tod);
  }
  case day_nonexistent::last_time: {
    return resolve_day_nonexistent_last_time(tod);
  }
  case day_nonexistent::first_day:
  case day_nonexistent::last_day:
  case day_nonexistent::na:
  case day_nonexistent::error: {
    return;
  }
  }
}

// First day of next month - keep time of day
static inline void resolve_day_nonexistent_first_day(date::year_month_day& ymd) {
  ymd = ymd.year() / (ymd.month() + date::months(1)) / date::day(1);
}
// First time of next month - cancels out time of day
static inline void resolve_day_nonexistent_first_time(std::chrono::seconds& tod) {
  tod = std::chrono::seconds{0};
}

// Last day of current month - keep time of day
static inline void resolve_day_nonexistent_last_day(date::year_month_day& ymd) {
  ymd = ymd.year() / ymd.month() / date::last;
}
// Last time of current month - shifts time of day to last second
static inline void resolve_day_nonexistent_last_time(std::chrono::seconds& tod) {
  tod = std::chrono::seconds{86400 - 1};
}

static inline void resolve_day_nonexistent_na(bool& na) {
  na = true;
}

static inline void resolve_day_nonexistent_error(r_ssize i) {
  r_abort("Nonexistent day found at location %i.", (int) i + 1);
}

// -----------------------------------------------------------------------------

#endif
