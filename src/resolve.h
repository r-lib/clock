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

static inline void resolve_day_nonexistent_yww_first_day(iso_week::year_weeknum_weekday& yww) {
  yww = (yww.year() + iso_week::years{1}) / iso_week::weeknum{1} / iso_week::mon;
}
static inline void resolve_day_nonexistent_yw_first_day(iso_week::year_weeknum& yw) {
  yw = (yw.year() + iso_week::years{1}) / iso_week::weeknum{1};
}

static inline void resolve_day_nonexistent_yww_last_day(iso_week::year_weeknum_weekday& yww) {
  yww = yww.year() / iso_week::last / iso_week::sun;
}
static inline void resolve_day_nonexistent_yw_last_day(iso_week::year_weeknum& yw) {
  const iso_week::year_lastweek ylw{yw.year()};
  yw = iso_week::year_weeknum{ylw.year(), ylw.weeknum()};
}

#endif
