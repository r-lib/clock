#ifndef CIVIL_RESOLVE_H
#define CIVIL_RESOLVE_H

#include "civil.h"
#include "utils.h"
#include "enums.h"
#include "civil-rcrd.h"

// -----------------------------------------------------------------------------

static inline void resolve_day_nonexistent_ymd_first_day(date::year_month_day& ymd);
template <fiscal_year::start S>
static inline void resolve_day_nonexistent_yqnqd_first_day(fiscal_year::year_quarternum_quarterday<S>& yqnqd);
static inline void resolve_day_nonexistent_yww_first_day(iso_week::year_weeknum_weekday& yww);
static inline void resolve_day_nonexistent_tod_first_time(std::chrono::seconds& tod);
static inline void resolve_day_nonexistent_nanos_first_time(std::chrono::nanoseconds& nanos_of_second);
static inline void resolve_day_nonexistent_ymd_last_day(date::year_month_day& ymd);
template <fiscal_year::start S>
static inline void resolve_day_nonexistent_yqnqd_last_day(fiscal_year::year_quarternum_quarterday<S>& yqnqd);
static inline void resolve_day_nonexistent_yww_last_day(iso_week::year_weeknum_weekday& yww);
static inline void resolve_day_nonexistent_tod_last_time(std::chrono::seconds& tod);
static inline void resolve_day_nonexistent_nanos_last_time(std::chrono::nanoseconds& nanos_of_second);
static inline void resolve_day_nonexistent_na(bool& na);
static inline void resolve_day_nonexistent_error(const r_ssize& i);

static inline void resolve_day_nonexistent_ymd(const r_ssize& i,
                                               const enum day_nonexistent& day_nonexistent_val,
                                               date::year_month_day& ymd,
                                               bool& na) {
  switch (day_nonexistent_val) {
  case day_nonexistent::first_day: {
    return resolve_day_nonexistent_ymd_first_day(ymd);
  }
  case day_nonexistent::first_time: {
    return resolve_day_nonexistent_ymd_first_day(ymd);
  }
  case day_nonexistent::last_day: {
    return resolve_day_nonexistent_ymd_last_day(ymd);
  }
  case day_nonexistent::last_time: {
    return resolve_day_nonexistent_ymd_last_day(ymd);
  }
  case day_nonexistent::na: {
    return resolve_day_nonexistent_na(na);
  }
  case day_nonexistent::error: {
    resolve_day_nonexistent_error(i);
  }
  }
}

template <fiscal_year::start S>
static inline void resolve_day_nonexistent_yqnqd(const r_ssize& i,
                                                 const enum day_nonexistent& day_nonexistent_val,
                                                 fiscal_year::year_quarternum_quarterday<S>& yqnqd,
                                                 bool& na) {
  switch (day_nonexistent_val) {
  case day_nonexistent::first_day: {
    return resolve_day_nonexistent_yqnqd_first_day(yqnqd);
  }
  case day_nonexistent::first_time: {
    return resolve_day_nonexistent_yqnqd_first_day(yqnqd);
  }
  case day_nonexistent::last_day: {
    return resolve_day_nonexistent_yqnqd_last_day(yqnqd);
  }
  case day_nonexistent::last_time: {
    return resolve_day_nonexistent_yqnqd_last_day(yqnqd);
  }
  case day_nonexistent::na: {
    return resolve_day_nonexistent_na(na);
  }
  case day_nonexistent::error: {
    resolve_day_nonexistent_error(i);
  }
  }
}

static inline void resolve_day_nonexistent_yww(const r_ssize& i,
                                               const enum day_nonexistent& day_nonexistent_val,
                                               iso_week::year_weeknum_weekday& yww,
                                               bool& na) {
  switch (day_nonexistent_val) {
  case day_nonexistent::first_day: {
    return resolve_day_nonexistent_yww_first_day(yww);
  }
  case day_nonexistent::first_time: {
    return resolve_day_nonexistent_yww_first_day(yww);
  }
  case day_nonexistent::last_day: {
    return resolve_day_nonexistent_yww_last_day(yww);
  }
  case day_nonexistent::last_time: {
    return resolve_day_nonexistent_yww_last_day(yww);
  }
  case day_nonexistent::na: {
    return resolve_day_nonexistent_na(na);
  }
  case day_nonexistent::error: {
    resolve_day_nonexistent_error(i);
  }
  }
}

static inline void resolve_day_nonexistent_tod(const enum day_nonexistent& day_nonexistent_val,
                                               std::chrono::seconds& tod) {
  switch (day_nonexistent_val) {
  case day_nonexistent::first_time: {
    return resolve_day_nonexistent_tod_first_time(tod);
  }
  case day_nonexistent::last_time: {
    return resolve_day_nonexistent_tod_last_time(tod);
  }
  case day_nonexistent::first_day:
  case day_nonexistent::last_day:
  case day_nonexistent::na:
  case day_nonexistent::error: {
    return;
  }
  }
}

static inline void resolve_day_nonexistent_nanos_of_second(const enum day_nonexistent& day_nonexistent_val,
                                                           std::chrono::nanoseconds& nanos_of_second) {
  switch (day_nonexistent_val) {
  case day_nonexistent::first_time: {
    return resolve_day_nonexistent_nanos_first_time(nanos_of_second);
  }
  case day_nonexistent::last_time: {
    return resolve_day_nonexistent_nanos_last_time(nanos_of_second);
  }
  case day_nonexistent::first_day:
  case day_nonexistent::last_day:
  case day_nonexistent::na:
  case day_nonexistent::error: {
    return;
  }
  }
}

static inline void resolve_day_nonexistent_ymd_first_day(date::year_month_day& ymd) {
  ymd = ((ymd.year() / ymd.month()) + date::months(1)) / date::day(1);
}
template <fiscal_year::start S>
static inline void resolve_day_nonexistent_yqnqd_first_day(fiscal_year::year_quarternum_quarterday<S>& yqnqd) {
  yqnqd = ((yqnqd.year() / yqnqd.quarternum()) + fiscal_year::quarters(1)) / fiscal_year::quarterday{1u};
}
static inline void resolve_day_nonexistent_yww_first_day(iso_week::year_weeknum_weekday& yww) {
  yww = (yww.year() + iso_week::years{1}) / iso_week::weeknum{1} / iso_week::mon;
}
static inline void resolve_day_nonexistent_tod_first_time(std::chrono::seconds& tod) {
  tod = std::chrono::seconds{0};
}
static inline void resolve_day_nonexistent_nanos_first_time(std::chrono::nanoseconds& nanos_of_second) {
  nanos_of_second = std::chrono::nanoseconds{0};
}

static inline void resolve_day_nonexistent_ymd_last_day(date::year_month_day& ymd) {
  ymd = ymd.year() / ymd.month() / date::last;
}
template <fiscal_year::start S>
static inline void resolve_day_nonexistent_yqnqd_last_day(fiscal_year::year_quarternum_quarterday<S>& yqnqd) {
  yqnqd = yqnqd.year() / yqnqd.quarternum() / fiscal_year::last;
}
static inline void resolve_day_nonexistent_yww_last_day(iso_week::year_weeknum_weekday& yww) {
  yww = yww.year() / iso_week::last / iso_week::sun;
}
static inline void resolve_day_nonexistent_tod_last_time(std::chrono::seconds& tod) {
  tod = std::chrono::seconds{86400 - 1};
}
static inline void resolve_day_nonexistent_nanos_last_time(std::chrono::nanoseconds& nanos_of_second) {
  nanos_of_second = std::chrono::nanoseconds{999999999};
}

static inline void resolve_day_nonexistent_na(bool& na) {
  na = true;
}

static inline void resolve_day_nonexistent_error(const r_ssize& i) {
  civil_abort("Nonexistent day found at location %i.", (int) i + 1);
}

// -----------------------------------------------------------------------------

static inline void convert_year_month_day_to_days_one(const r_ssize& i,
                                                      const enum day_nonexistent& day_nonexistent_val,
                                                      date::year_month_day& ymd,
                                                      int* p_days,
                                                      int* p_time_of_day,
                                                      int* p_nanos_of_second) {
  // Simple case - convert to local_days, no changes to time-of-day
  if (ymd.ok()) {
    date::local_days out_lday{ymd};
    p_days[i] = out_lday.time_since_epoch().count();
    return;
  }

  bool na = false;
  resolve_day_nonexistent_ymd(i, day_nonexistent_val, ymd, na);

  if (na) {
    civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
    return;
  }

  if (p_time_of_day != NULL) {
    std::chrono::seconds elt_tod{p_time_of_day[i]};
    resolve_day_nonexistent_tod(day_nonexistent_val, elt_tod);
    p_time_of_day[i] = elt_tod.count();
  }

  if (p_nanos_of_second != NULL) {
    std::chrono::nanoseconds elt_nanos_of_second{p_nanos_of_second[i]};
    resolve_day_nonexistent_nanos_of_second(day_nonexistent_val, elt_nanos_of_second);
    p_nanos_of_second[i] = elt_nanos_of_second.count();
  }

  date::local_days out_lday{ymd};
  p_days[i] = out_lday.time_since_epoch().count();
}

// -----------------------------------------------------------------------------

template <fiscal_year::start S>
static inline void convert_year_quarternum_quarterday_to_days_one(const r_ssize& i,
                                                                  const enum day_nonexistent& day_nonexistent_val,
                                                                  fiscal_year::year_quarternum_quarterday<S>& yqnqd,
                                                                  int* p_days,
                                                                  int* p_time_of_day,
                                                                  int* p_nanos_of_second) {
  // Simple case - convert to local_days, no changes to time-of-day
  if (yqnqd.ok()) {
    date::local_days out_lday{yqnqd};
    p_days[i] = out_lday.time_since_epoch().count();
    return;
  }

  bool na = false;
  resolve_day_nonexistent_yqnqd(i, day_nonexistent_val, yqnqd, na);

  if (na) {
    civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
    return;
  }

  if (p_time_of_day != NULL) {
    std::chrono::seconds elt_tod{p_time_of_day[i]};
    resolve_day_nonexistent_tod(day_nonexistent_val, elt_tod);
    p_time_of_day[i] = elt_tod.count();
  }

  if (p_nanos_of_second != NULL) {
    std::chrono::nanoseconds elt_nanos_of_second{p_nanos_of_second[i]};
    resolve_day_nonexistent_nanos_of_second(day_nonexistent_val, elt_nanos_of_second);
    p_nanos_of_second[i] = elt_nanos_of_second.count();
  }

  date::local_days out_lday{yqnqd};
  p_days[i] = out_lday.time_since_epoch().count();
}

// -----------------------------------------------------------------------------

static inline void convert_year_weeknum_weekday_to_days_one(const r_ssize& i,
                                                            const enum day_nonexistent& day_nonexistent_val,
                                                            iso_week::year_weeknum_weekday& yww,
                                                            int* p_days,
                                                            int* p_time_of_day,
                                                            int* p_nanos_of_second) {
  // Simple case - convert to local_days, no changes to time-of-day
  if (yww.ok()) {
    date::local_days out_lday{yww};
    p_days[i] = out_lday.time_since_epoch().count();
    return;
  }

  bool na = false;
  resolve_day_nonexistent_yww(i, day_nonexistent_val, yww, na);

  if (na) {
    civil_rcrd_assign_missing(i, p_days, p_time_of_day, p_nanos_of_second);
    return;
  }

  if (p_time_of_day != NULL) {
    std::chrono::seconds elt_tod{p_time_of_day[i]};
    resolve_day_nonexistent_tod(day_nonexistent_val, elt_tod);
    p_time_of_day[i] = elt_tod.count();
  }

  if (p_nanos_of_second != NULL) {
    std::chrono::nanoseconds elt_nanos_of_second{p_nanos_of_second[i]};
    resolve_day_nonexistent_nanos_of_second(day_nonexistent_val, elt_nanos_of_second);
    p_nanos_of_second[i] = elt_nanos_of_second.count();
  }

  date::local_days out_lday{yww};
  p_days[i] = out_lday.time_since_epoch().count();
}

// -----------------------------------------------------------------------------

#endif
