#ifndef CLOCK_PARSE_H
#define CLOCK_PARSE_H

#include "clock.h"

// -----------------------------------------------------------------------------

namespace rclock {

/*
 * Extra template variations of `from_stream()`
 *
 * Both allow "invalid" dates to be parsed, like 2020-02-30. This ensures we
 * can parse() what we produce from format().
 *
 * The first `from_stream()` variant accepts and ymd and tod directly, which
 * no <date> `from_stream()` variants do.
 */

template <class Duration, class CharT, class Traits, class Alloc = std::allocator<CharT>>
std::basic_istream<CharT, Traits>&
from_stream(std::basic_istream<CharT, Traits>& is,
            const CharT* fmt,
            date::year_month_day& ymd,
            date::hh_mm_ss<Duration>& tod,
            std::basic_string<CharT, Traits, Alloc>* abbrev = nullptr,
            std::chrono::minutes* offset = nullptr)
{
  using CT = typename std::common_type<Duration, std::chrono::seconds>::type;
  std::chrono::minutes offset_local{};
  std::chrono::minutes* offptr = offset ? offset : &offset_local;
  date::fields<CT> fds{};
  fds.has_tod = true;
  date::from_stream(is, fmt, fds, abbrev, offptr);
  // Fields must be `ok()` independently, not jointly. i.e. invalid dates are allowed.
  if (!fds.ymd.year().ok() || !fds.ymd.month().ok() || !fds.ymd.day().ok() || !fds.tod.in_conventional_range())
    is.setstate(std::ios::failbit);
  if (!is.fail()) {
    ymd = fds.ymd;
    tod = fds.tod;
  }
  return is;
}

template <class CharT, class Traits, class Alloc = std::allocator<CharT>>
std::basic_istream<CharT, Traits>&
from_stream(std::basic_istream<CharT, Traits>& is,
            const CharT* fmt,
            date::year_month_day& ymd,
            std::basic_string<CharT, Traits, Alloc>* abbrev = nullptr,
            std::chrono::minutes* offset = nullptr)
{
  using CT = std::chrono::seconds;
  date::fields<CT> fds{};
  date::from_stream(is, fmt, fds, abbrev, offset);
  // Fields must be `ok()` independently, not jointly. i.e. invalid dates are allowed.
  if (!fds.ymd.year().ok() || !fds.ymd.month().ok() || !fds.ymd.day().ok())
    is.setstate(std::ios::failbit);
  if (!is.fail())
    ymd = fds.ymd;
  return is;
}

} // namespace rclock

#endif
