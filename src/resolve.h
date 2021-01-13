#ifndef CLOCK_RESOLVE_H
#define CLOCK_RESOLVE_H

#include "clock.h"
#include "utils.h"

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

#endif
