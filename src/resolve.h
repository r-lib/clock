#ifndef CLOCK_RESOLVE_H
#define CLOCK_RESOLVE_H

#include "clock.h"
#include "utils.h"
#include "enums.h"
#include "clock-rcrd.h"

// -----------------------------------------------------------------------------

static inline void resolve_day_nonexistent_ymd_first_day(date::year_month_day& ymd);
static inline void resolve_day_nonexistent_ymw_first_day(date::year_month_weekday& ymw);
template <quarterly::start S>
static inline void resolve_day_nonexistent_yqnqd_first_day(quarterly::year_quarternum_quarterday<S>& yqnqd);
static inline void resolve_day_nonexistent_yww_first_day(iso_week::year_weeknum_weekday& yww);
static inline void resolve_day_nonexistent_tod_first_time(std::chrono::seconds& tod);
static inline void resolve_day_nonexistent_nanos_first_time(std::chrono::nanoseconds& nanos_of_second);
static inline void resolve_day_nonexistent_ymd_last_day(date::year_month_day& ymd);
static inline void resolve_day_nonexistent_ymw_last_day(date::year_month_weekday& ymw);
template <quarterly::start S>
static inline void resolve_day_nonexistent_yqnqd_last_day(quarterly::year_quarternum_quarterday<S>& yqnqd);
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

static inline void resolve_day_nonexistent_ymw(const r_ssize& i,
                                               const enum day_nonexistent& day_nonexistent_val,
                                               date::year_month_weekday& ymw,
                                               bool& na) {
  switch (day_nonexistent_val) {
  case day_nonexistent::first_day: {
    return resolve_day_nonexistent_ymw_first_day(ymw);
  }
  case day_nonexistent::first_time: {
    return resolve_day_nonexistent_ymw_first_day(ymw);
  }
  case day_nonexistent::last_day: {
    return resolve_day_nonexistent_ymw_last_day(ymw);
  }
  case day_nonexistent::last_time: {
    return resolve_day_nonexistent_ymw_last_day(ymw);
  }
  case day_nonexistent::na: {
    return resolve_day_nonexistent_na(na);
  }
  case day_nonexistent::error: {
    resolve_day_nonexistent_error(i);
  }
  }
}

template <quarterly::start S>
static inline void resolve_day_nonexistent_yqnqd(const r_ssize& i,
                                                 const enum day_nonexistent& day_nonexistent_val,
                                                 quarterly::year_quarternum_quarterday<S>& yqnqd,
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
static inline void resolve_day_nonexistent_ymw_first_day(date::year_month_weekday& ymw) {
  ymw = date::year_month_weekday{((ymw.year() / ymw.month()) + date::months(1)) / date::day(1)};
}
template <quarterly::start S>
static inline void resolve_day_nonexistent_yqnqd_first_day(quarterly::year_quarternum_quarterday<S>& yqnqd) {
  yqnqd = ((yqnqd.year() / yqnqd.quarternum()) + quarterly::quarters(1)) / quarterly::quarterday{1u};
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
static inline void resolve_day_nonexistent_ymw_last_day(date::year_month_weekday& ymw) {
  ymw = date::year_month_weekday{ymw.year() / ymw.month() / date::last};
}
template <quarterly::start S>
static inline void resolve_day_nonexistent_yqnqd_last_day(quarterly::year_quarternum_quarterday<S>& yqnqd) {
  yqnqd = yqnqd.year() / yqnqd.quarternum() / quarterly::last;
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
  clock_abort("Nonexistent day found at location %i.", (int) i + 1);
}

// -----------------------------------------------------------------------------

static inline void convert_ymd_to_calendar_one(const r_ssize& i,
                                               const enum day_nonexistent& day_nonexistent_val,
                                               date::year_month_day& ymd,
                                               clock_writable_field& calendar,
                                               cpp11::writable::logicals& ok,
                                               cpp11::writable::logicals& any) {
  if (ymd.ok()) {
    date::local_days out_lday{ymd};
    calendar[i] = out_lday.time_since_epoch().count();
    return;
  }

  any[0] = true;
  ok[i] = false;

  bool na = false;
  resolve_day_nonexistent_ymd(i, day_nonexistent_val, ymd, na);

  if (na) {
    calendar[i] = r_int_na;
    return;
  }

  date::local_days out_lday{ymd};
  calendar[i] = out_lday.time_since_epoch().count();
}

// -----------------------------------------------------------------------------

static inline void convert_ymw_to_calendar_one(const r_ssize& i,
                                               const enum day_nonexistent& day_nonexistent_val,
                                               date::year_month_weekday& ymw,
                                               clock_writable_field& calendar,
                                               cpp11::writable::logicals& ok,
                                               cpp11::writable::logicals& any) {
  if (ymw.ok()) {
    date::local_days out_lday{ymw};
    calendar[i] = out_lday.time_since_epoch().count();
    return;
  }

  any[0] = true;
  ok[i] = false;

  bool na = false;
  resolve_day_nonexistent_ymw(i, day_nonexistent_val, ymw, na);

  if (na) {
    calendar[i] = r_int_na;
    return;
  }

  date::local_days out_lday{ymw};
  calendar[i] = out_lday.time_since_epoch().count();
}

// -----------------------------------------------------------------------------

template <quarterly::start S>
static inline void convert_yqnqd_to_calendar_one(const r_ssize& i,
                                                 const enum day_nonexistent& day_nonexistent_val,
                                                 quarterly::year_quarternum_quarterday<S>& yqnqd,
                                                 clock_writable_field& calendar,
                                                 cpp11::writable::logicals& ok,
                                                 cpp11::writable::logicals& any) {
  if (yqnqd.ok()) {
    date::local_days out_lday{yqnqd};
    calendar[i] = out_lday.time_since_epoch().count();
    return;
  }

  any[0] = true;
  ok[i] = false;

  bool na = false;
  resolve_day_nonexistent_yqnqd(i, day_nonexistent_val, yqnqd, na);

  if (na) {
    calendar[i] = r_int_na;
    return;
  }

  date::local_days out_lday{yqnqd};
  calendar[i] = out_lday.time_since_epoch().count();
}

// -----------------------------------------------------------------------------

static inline void convert_iso_yww_to_calendar_one(const r_ssize& i,
                                                   const enum day_nonexistent& day_nonexistent_val,
                                                   iso_week::year_weeknum_weekday& yww,
                                                   clock_writable_field& calendar,
                                                   cpp11::writable::logicals& ok,
                                                   cpp11::writable::logicals& any) {
  if (yww.ok()) {
    date::local_days out_lday{yww};
    calendar[i] = out_lday.time_since_epoch().count();
    return;
  }

  any[0] = true;
  ok[i] = false;

  bool na = false;
  resolve_day_nonexistent_yww(i, day_nonexistent_val, yww, na);

  if (na) {
    calendar[i] = r_int_na;
    return;
  }

  date::local_days out_lday{yww};
  calendar[i] = out_lday.time_since_epoch().count();
}

// -----------------------------------------------------------------------------

#endif
