#ifndef CLOCK_RESOLVE_H
#define CLOCK_RESOLVE_H

#include "clock.h"
#include "utils.h"

namespace rclock {

namespace detail {

inline
std::chrono::hours
resolve_next_hour() {
  return std::chrono::hours{0};
}
inline
std::chrono::minutes
resolve_next_minute() {
  return std::chrono::minutes{0};
}
inline
std::chrono::seconds
resolve_next_second() {
  return std::chrono::seconds{0};
}
template <typename Duration>
inline
Duration
resolve_next_subsecond() {
  return Duration{0};
}

inline
std::chrono::hours
resolve_previous_hour() {
  return std::chrono::hours{23};
}
inline
std::chrono::minutes
resolve_previous_minute() {
  return std::chrono::minutes{59};
}
inline
std::chrono::seconds
resolve_previous_second() {
  return std::chrono::seconds{59};
}
template <typename Duration>
inline
Duration
resolve_previous_subsecond() {
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
