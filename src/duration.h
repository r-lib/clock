#ifndef CLOCK_DURATION_H
#define CLOCK_DURATION_H

#include "clock.h"
#include "integers.h"
#include "enums.h"

namespace rclock {

namespace duration {

template <typename Duration>
class duration1
{
protected:
  rclock::integers ticks_;

public:
  CONSTCD11 duration1(const cpp11::integers& ticks) NOEXCEPT;
  duration1(r_ssize size);

  CONSTCD11 bool is_na(r_ssize i) const NOEXCEPT;
  CONSTCD11 r_ssize size() const NOEXCEPT;

  void assign_na(r_ssize i);
  void assign(const Duration& x, r_ssize i);

  CONSTCD11 Duration operator[](r_ssize i) const NOEXCEPT;

  cpp11::writable::list to_list() const;

  using duration = Duration;
};

using years = duration1<date::years>;
using quarters = duration1<quarterly::quarters>;
using months = duration1<date::months>;
using weeks = duration1<date::weeks>;
using days = duration1<date::days>;

template <typename Duration>
class duration2 : public duration1<date::days>
{
protected:
  rclock::integers ticks_of_day_;

public:
  CONSTCD11 duration2(const cpp11::integers& ticks,
                      const cpp11::integers& ticks_of_day) NOEXCEPT;
  duration2(r_ssize size);

  void assign_na(r_ssize i);
  void assign(const Duration& x, r_ssize i);

  CONSTCD11 Duration operator[](r_ssize i) const NOEXCEPT;

  cpp11::writable::list to_list() const;

  using duration = Duration;
};

using hours = duration2<std::chrono::hours>;
using minutes = duration2<std::chrono::minutes>;
using seconds = duration2<std::chrono::seconds>;

template <typename Duration>
class duration3 : public duration2<std::chrono::seconds>
{
protected:
  rclock::integers ticks_of_second_;

public:
  CONSTCD11 duration3(const cpp11::integers& ticks,
                      const cpp11::integers& ticks_of_day,
                      const cpp11::integers& ticks_of_second) NOEXCEPT;
  duration3(r_ssize size);

  void assign_na(r_ssize i);
  void assign(const Duration& x, r_ssize i);

  CONSTCD11 Duration operator[](r_ssize i) const NOEXCEPT;

  cpp11::writable::list to_list() const;

  using duration = Duration;
};

using milliseconds = duration3<std::chrono::milliseconds>;
using microseconds = duration3<std::chrono::microseconds>;
using nanoseconds = duration3<std::chrono::nanoseconds>;

// Implementation

// duration1

template <typename Duration>
CONSTCD11
inline
duration1<Duration>::duration1(const cpp11::integers& ticks) NOEXCEPT
  : ticks_(ticks)
  {}

template <typename Duration>
inline
duration1<Duration>::duration1(r_ssize size)
  : ticks_(size)
  {}

template <typename Duration>
CONSTCD11
inline
bool
duration1<Duration>::is_na(r_ssize i) const NOEXCEPT
{
  return ticks_[i] == r_int_na;
}

template <typename Duration>
CONSTCD11
inline
r_ssize
duration1<Duration>::size() const NOEXCEPT
{
  return ticks_.size();
}

template <typename Duration>
inline
void
duration1<Duration>::assign_na(r_ssize i)
{
  ticks_.assign_na(i);
}

template <typename Duration>
inline
void
duration1<Duration>::assign(const Duration& x, r_ssize i)
{
  ticks_.assign(x.count(), i);
}

template <typename Duration>
CONSTCD11
inline
Duration
duration1<Duration>::operator[](r_ssize i) const NOEXCEPT
{
  return Duration{ticks_[i]};
}

template <typename Duration>
inline
cpp11::writable::list
duration1<Duration>::to_list() const
{
  cpp11::writable::list out({ticks_.sexp()});
  out.names() = {"ticks"};
  return out;
}

// duration2

template <typename Duration>
CONSTCD11
inline
duration2<Duration>::duration2(const cpp11::integers& ticks,
                               const cpp11::integers& ticks_of_day) NOEXCEPT
  : duration1(ticks),
    ticks_of_day_(ticks_of_day)
  {}

template <typename Duration>
inline
duration2<Duration>::duration2(r_ssize size)
  : duration1(size),
    ticks_of_day_(size)
  {}

template <typename Duration>
inline
void
duration2<Duration>::assign_na(r_ssize i)
{
  duration1::assign_na(i);
  ticks_of_day_.assign_na(i);
}

template <typename Duration>
inline
void
duration2<Duration>::assign(const Duration& x, r_ssize i)
{
  const date::days tick = date::floor<date::days>(x);
  ticks_.assign(tick.count(), i);
  ticks_of_day_.assign((x - tick).count(), i);
}

template <typename Duration>
CONSTCD11
inline
Duration
duration2<Duration>::operator[](r_ssize i) const NOEXCEPT
{
  return duration1::operator[](i) + Duration{ticks_of_day_[i]};
}

template <typename Duration>
inline
cpp11::writable::list
duration2<Duration>::to_list() const
{
  cpp11::writable::list out({ticks_.sexp(), ticks_of_day_.sexp()});
  out.names() = {"ticks", "ticks_of_day"};
  return out;
}

// duration3

template <typename Duration>
CONSTCD11
inline
duration3<Duration>::duration3(const cpp11::integers& ticks,
                               const cpp11::integers& ticks_of_day,
                               const cpp11::integers& ticks_of_second) NOEXCEPT
  : duration2(ticks, ticks_of_day),
    ticks_of_second_(ticks_of_second)
  {}

template <typename Duration>
inline
duration3<Duration>::duration3(r_ssize size)
  : duration2(size),
    ticks_of_second_(size)
  {}

template <typename Duration>
inline
void
duration3<Duration>::assign_na(r_ssize i)
{
  duration2::assign_na(i);
  ticks_of_second_.assign_na(i);
}

template <typename Duration>
inline
void
duration3<Duration>::assign(const Duration& x, r_ssize i)
{
  const date::days tick = date::floor<date::days>(x);
  const Duration x_of_day = x - tick;
  const std::chrono::seconds tick_of_day = date::floor<std::chrono::seconds>(x_of_day);
  const Duration tick_of_second = x_of_day - tick_of_day;
  ticks_.assign(tick.count(), i);
  ticks_of_day_.assign(tick_of_day.count(), i);
  ticks_of_second_.assign(tick_of_second.count(), i);
}

template <typename Duration>
CONSTCD11
inline
Duration
duration3<Duration>::operator[](r_ssize i) const NOEXCEPT
{
  return duration2::operator[](i) + Duration{ticks_of_second_[i]};
}

template <typename Duration>
inline
cpp11::writable::list
duration3<Duration>::to_list() const
{
  cpp11::writable::list out({ticks_.sexp(), ticks_of_day_.sexp(), ticks_of_second_.sexp()});
  out.names() = {"ticks", "ticks_of_day", "ticks_of_second"};
  return out;
}

} // namespace duration

} // namespace rclock

#endif
