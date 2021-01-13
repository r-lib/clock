#ifndef CLOCK_RESOLVE_H
#define CLOCK_RESOLVE_H

#include "clock.h"
#include "utils.h"
#include "enums.h"

namespace rclock {

namespace detail {

inline
std::chrono::hours
resolve_first_day_hour() {
  return std::chrono::hours{0};
}
inline
std::chrono::minutes
resolve_first_day_minute() {
  return std::chrono::minutes{0};
}
inline
std::chrono::seconds
resolve_first_day_second() {
  return std::chrono::seconds{0};
}
template <typename Duration>
inline
Duration
resolve_first_day_subsecond() {
  return Duration{0};
}

inline
std::chrono::hours
resolve_last_day_hour() {
  return std::chrono::hours{23};
}
inline
std::chrono::minutes
resolve_last_day_minute() {
  return std::chrono::minutes{59};
}
inline
std::chrono::seconds
resolve_last_day_second() {
  return std::chrono::seconds{59};
}
template <typename Duration>
inline
Duration
resolve_last_day_subsecond() {
  return std::chrono::seconds{1} - Duration{1};
}

inline
void
resolve_error(r_ssize i) {
  std::string message =
    std::string{"Invalid day found at location %td. "} +
    "Resolve invalid day issues by specifying the `invalid` argument.";

  clock_abort(message.c_str(), (ptrdiff_t) i + 1);
}

} // namespace detail

} // namespace rclock

// -----------------------------------------------------------------------------


static inline void resolve_day_nonexistent_yww_first_day(iso_week::year_weeknum_weekday& yww);
static inline void resolve_day_nonexistent_tod_first_time(std::chrono::seconds& tod);
static inline void resolve_day_nonexistent_nanos_first_time(std::chrono::nanoseconds& nanos_of_second);
static inline void resolve_day_nonexistent_yww_last_day(iso_week::year_weeknum_weekday& yww);
static inline void resolve_day_nonexistent_tod_last_time(std::chrono::seconds& tod);
static inline void resolve_day_nonexistent_nanos_last_time(std::chrono::nanoseconds& nanos_of_second);
static inline void resolve_day_nonexistent_na(bool& na);
static inline void resolve_day_nonexistent_error(const r_ssize& i);

// -----------------------------------------------------------------------------

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

static inline void resolve_day_nonexistent_yww_first_day(iso_week::year_weeknum_weekday& yww) {
  yww = (yww.year() + iso_week::years{1}) / iso_week::weeknum{1} / iso_week::mon;
}
static inline void resolve_day_nonexistent_yw_first_day(iso_week::year_weeknum& yw) {
  yw = (yw.year() + iso_week::years{1}) / iso_week::weeknum{1};
}
static inline void resolve_day_nonexistent_tod_first_time(std::chrono::seconds& tod) {
  tod = std::chrono::seconds{0};
}
static inline void resolve_day_nonexistent_nanos_first_time(std::chrono::nanoseconds& nanos_of_second) {
  nanos_of_second = std::chrono::nanoseconds{0};
}

static inline void resolve_day_nonexistent_yww_last_day(iso_week::year_weeknum_weekday& yww) {
  yww = yww.year() / iso_week::last / iso_week::sun;
}
static inline void resolve_day_nonexistent_yw_last_day(iso_week::year_weeknum& yw) {
  const iso_week::year_lastweek ylw{yw.year()};
  yw = iso_week::year_weeknum{ylw.year(), ylw.weeknum()};
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
