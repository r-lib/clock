#ifndef CLOCK_STREAM_H
#define CLOCK_STREAM_H

#include "clock.h"

namespace rclock {

namespace detail {

inline
std::ostringstream&
stream_year(std::ostringstream& os, int year) NOEXCEPT
{
  os << date::year{year};
  return os;
}

inline
std::ostringstream&
stream_month(std::ostringstream& os, int month) NOEXCEPT
{
  os.fill('0');
  os.flags(std::ios::dec | std::ios::right);
  os.width(2);
  os << month;
  return os;
}

inline
std::ostringstream&
stream_day(std::ostringstream& os, int day) NOEXCEPT
{
  os << date::day{static_cast<unsigned>(day)};
  return os;
}

inline
std::ostringstream&
stream_hour(std::ostringstream& os, int hour) NOEXCEPT
{
  os.fill('0');
  os.flags(std::ios::dec | std::ios::right);
  os.width(2);
  os << hour;
  return os;
}

inline
std::ostringstream&
stream_minute(std::ostringstream& os, int minute) NOEXCEPT
{
  os.fill('0');
  os.flags(std::ios::dec | std::ios::right);
  os.width(2);
  os << minute;
  return os;
}

inline
std::ostringstream&
stream_second(std::ostringstream& os, int second) NOEXCEPT
{
  os.fill('0');
  os.flags(std::ios::dec | std::ios::right);
  os.width(2);
  os << second;
  return os;
}

template <typename Duration>
inline
std::ostringstream&
stream_second_and_subsecond(std::ostringstream& os, int second, int subsecond) NOEXCEPT
{
  date::detail::decimal_format_seconds<Duration> dfs{std::chrono::seconds{second} + Duration{subsecond}};
  os << dfs;
  return os;
}

} // namespace detail

} // namespace rclock

#endif
