#ifndef CLOCK_DURATION_H
#define CLOCK_DURATION_H

#include "clock.h"
#include "enums.h"

namespace rclock {

template <typename Duration>
class duration
{
  const cpp11::integers& ticks_;
  const cpp11::integers& ticks_of_day_;
  const cpp11::integers& ticks_of_second_;

public:
  CONSTCD11 duration(const cpp11::integers& ticks,
                     const cpp11::integers& ticks_of_day,
                     const cpp11::integers& ticks_of_second) NOEXCEPT;

  CONSTCD11 bool is_na(r_ssize i) const NOEXCEPT;
  CONSTCD11 r_ssize size() const NOEXCEPT;

  CONSTCD11 Duration operator[](r_ssize i) const NOEXCEPT;
};

template <typename Duration>
CONSTCD11
inline
duration<Duration>::duration(const cpp11::integers& ticks,
                             const cpp11::integers& ticks_of_day,
                             const cpp11::integers& ticks_of_second) NOEXCEPT
  : ticks_(ticks),
    ticks_of_day_(ticks_of_day),
    ticks_of_second_(ticks_of_second)
  {}

template <typename Duration>
CONSTCD11
inline
bool
duration<Duration>::is_na(r_ssize i) const NOEXCEPT
{
  return ticks_[i] == r_int_na;
}

template <typename Duration>
CONSTCD11
inline
r_ssize
duration<Duration>::size() const NOEXCEPT
{
  return ticks_.size();
}

template <>
CONSTCD11
inline
date::years
duration<date::years>::operator[](r_ssize i) const NOEXCEPT
{
  return date::years{ticks_[i]};
}

template <>
CONSTCD11
inline
quarterly::quarters
duration<quarterly::quarters>::operator[](r_ssize i) const NOEXCEPT
{
  return quarterly::quarters{ticks_[i]};
}

template <>
CONSTCD11
inline
date::months
duration<date::months>::operator[](r_ssize i) const NOEXCEPT
{
  return date::months{ticks_[i]};
}

template <>
CONSTCD11
inline
date::weeks
duration<date::weeks>::operator[](r_ssize i) const NOEXCEPT
{
  return date::weeks{ticks_[i]};
}

template <>
CONSTCD11
inline
date::days
duration<date::days>::operator[](r_ssize i) const NOEXCEPT
{
  return date::days{ticks_[i]};
}

template <>
CONSTCD11
inline
std::chrono::hours
duration<std::chrono::hours>::operator[](r_ssize i) const NOEXCEPT
{
  return date::days{ticks_[i]} + std::chrono::hours{ticks_of_day_[i]};
}

template <>
CONSTCD11
inline
std::chrono::minutes
duration<std::chrono::minutes>::operator[](r_ssize i) const NOEXCEPT
{
  return date::days{ticks_[i]} + std::chrono::minutes{ticks_of_day_[i]};
}

template <>
CONSTCD11
inline
std::chrono::seconds
duration<std::chrono::seconds>::operator[](r_ssize i) const NOEXCEPT
{
  return date::days{ticks_[i]} + std::chrono::seconds{ticks_of_day_[i]};
}

template <>
CONSTCD11
inline
std::chrono::milliseconds
duration<std::chrono::milliseconds>::operator[](r_ssize i) const NOEXCEPT
{
  return date::days{ticks_[i]} +
    std::chrono::seconds{ticks_of_day_[i]} +
    std::chrono::milliseconds{ticks_of_second_[i]};
}

template <>
CONSTCD11
inline
std::chrono::microseconds
duration<std::chrono::microseconds>::operator[](r_ssize i) const NOEXCEPT
{
  return date::days{ticks_[i]} +
    std::chrono::seconds{ticks_of_day_[i]} +
    std::chrono::microseconds{ticks_of_second_[i]};
}

template <>
CONSTCD11
inline
std::chrono::nanoseconds
duration<std::chrono::nanoseconds>::operator[](r_ssize i) const NOEXCEPT
{
  return date::days{ticks_[i]} +
    std::chrono::seconds{ticks_of_day_[i]} +
    std::chrono::nanoseconds{ticks_of_second_[i]};
}

namespace writable {

template <typename Duration>
class duration
{
  cpp11::writable::integers ticks_;
  cpp11::writable::integers ticks_of_day_;
  cpp11::writable::integers ticks_of_second_;

public:
  duration(r_ssize size);

  cpp11::writable::list_of<cpp11::writable::integers> to_list() const;

  void assign_na(r_ssize i) NOEXCEPT;
  void assign(const Duration& d, r_ssize i) NOEXCEPT;
};

template <>
inline
duration<date::years>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers((r_ssize) 0);
  ticks_of_second_ = cpp11::writable::integers((r_ssize) 0);
}
template <>
inline
duration<quarterly::quarters>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers((r_ssize) 0);
  ticks_of_second_ = cpp11::writable::integers((r_ssize) 0);
}
template <>
inline
duration<date::months>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers((r_ssize) 0);
  ticks_of_second_ = cpp11::writable::integers((r_ssize) 0);
}
template <>
inline
duration<date::weeks>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers((r_ssize) 0);
  ticks_of_second_ = cpp11::writable::integers((r_ssize) 0);
}
template <>
inline
duration<date::days>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers((r_ssize) 0);
  ticks_of_second_ = cpp11::writable::integers((r_ssize) 0);
}
template <>
inline
duration<std::chrono::hours>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers(size);
  ticks_of_second_ = cpp11::writable::integers((r_ssize) 0);
}
template <>
inline
duration<std::chrono::minutes>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers(size);
  ticks_of_second_ = cpp11::writable::integers((r_ssize) 0);
}
template <>
inline
duration<std::chrono::seconds>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers(size);
  ticks_of_second_ = cpp11::writable::integers((r_ssize) 0);
}
template <>
inline
duration<std::chrono::milliseconds>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers(size);
  ticks_of_second_ = cpp11::writable::integers(size);
}
template <>
inline
duration<std::chrono::microseconds>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers(size);
  ticks_of_second_ = cpp11::writable::integers(size);
}
template <>
inline
duration<std::chrono::nanoseconds>::duration(r_ssize size) {
  ticks_ = cpp11::writable::integers(size);
  ticks_of_day_ = cpp11::writable::integers(size);
  ticks_of_second_ = cpp11::writable::integers(size);
}

template <typename Duration>
inline
cpp11::writable::list_of<cpp11::writable::integers>
duration<Duration>::to_list() const
{
  cpp11::writable::list_of<cpp11::writable::integers> out(
      {ticks_, ticks_of_day_, ticks_of_second_}
  );

  out.names() = {"ticks", "ticks_of_day", "ticks_of_second"};

  return out;
}

template <>
inline
void duration<date::years>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
}
template <>
inline
void duration<quarterly::quarters>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
}
template <>
inline
void duration<date::months>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
}
template <>
inline
void duration<date::weeks>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
}
template <>
inline
void duration<date::days>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
}
template <>
inline
void duration<std::chrono::hours>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
  ticks_of_day_[i] = r_int_na;
}
template <>
inline
void duration<std::chrono::minutes>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
  ticks_of_day_[i] = r_int_na;
}
template <>
inline
void duration<std::chrono::seconds>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
  ticks_of_day_[i] = r_int_na;
}
template <>
inline
void duration<std::chrono::milliseconds>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
  ticks_of_day_[i] = r_int_na;
  ticks_of_second_[i] = r_int_na;
}
template <>
inline
void duration<std::chrono::microseconds>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
  ticks_of_day_[i] = r_int_na;
  ticks_of_second_[i] = r_int_na;
}
template <>
inline
void duration<std::chrono::nanoseconds>::assign_na(r_ssize i) NOEXCEPT
{
  ticks_[i] = r_int_na;
  ticks_of_day_[i] = r_int_na;
  ticks_of_second_[i] = r_int_na;
}

template <>
inline
void duration<date::years>::assign(const date::years& d, r_ssize i) NOEXCEPT
{
  ticks_[i] = d.count();
}
template <>
inline
void duration<quarterly::quarters>::assign(const quarterly::quarters& d, r_ssize i) NOEXCEPT
{
  ticks_[i] = d.count();
}
template <>
inline
void duration<date::months>::assign(const date::months& d, r_ssize i) NOEXCEPT
{
  ticks_[i] = d.count();
}
template <>
inline
void duration<date::weeks>::assign(const date::weeks& d, r_ssize i) NOEXCEPT
{
  ticks_[i] = d.count();
}
template <>
inline
void duration<date::days>::assign(const date::days& d, r_ssize i) NOEXCEPT
{
  ticks_[i] = d.count();
}
template <>
inline
void duration<std::chrono::hours>::assign(const std::chrono::hours& d, r_ssize i) NOEXCEPT
{
  const date::days tick = date::floor<date::days>(d);
  ticks_[i] = tick.count();
  ticks_of_day_[i] = (d - tick).count();
}
template <>
inline
void duration<std::chrono::minutes>::assign(const std::chrono::minutes& d, r_ssize i) NOEXCEPT
{
  const date::days tick = date::floor<date::days>(d);
  ticks_[i] = tick.count();
  ticks_of_day_[i] = (d - tick).count();
}
template <>
inline
void duration<std::chrono::seconds>::assign(const std::chrono::seconds& d, r_ssize i) NOEXCEPT
{
  const date::days tick = date::floor<date::days>(d);
  ticks_[i] = tick.count();
  ticks_of_day_[i] = (d - tick).count();
}
template <>
inline
void duration<std::chrono::milliseconds>::assign(const std::chrono::milliseconds& d, r_ssize i) NOEXCEPT
{
  const date::days tick = date::floor<date::days>(d);
  const std::chrono::milliseconds d2 = d - tick;
  const std::chrono::seconds tick_of_day = date::floor<std::chrono::seconds>(d2);
  const std::chrono::milliseconds tick_of_second = d2 - tick_of_day;
  ticks_[i] = tick.count();
  ticks_of_day_[i] = tick_of_day.count();
  ticks_of_second_[i] = tick_of_second.count();
}
template <>
inline
void duration<std::chrono::microseconds>::assign(const std::chrono::microseconds& d, r_ssize i) NOEXCEPT
{
  const date::days tick = date::floor<date::days>(d);
  const std::chrono::microseconds d2 = d - tick;
  const std::chrono::seconds tick_of_day = date::floor<std::chrono::seconds>(d2);
  const std::chrono::microseconds tick_of_second = d2 - tick_of_day;
  ticks_[i] = tick.count();
  ticks_of_day_[i] = tick_of_day.count();
  ticks_of_second_[i] = tick_of_second.count();
}
template <>
inline
void duration<std::chrono::nanoseconds>::assign(const std::chrono::nanoseconds& d, r_ssize i) NOEXCEPT
{
  const date::days tick = date::floor<date::days>(d);
  const std::chrono::nanoseconds d2 = d - tick;
  const std::chrono::seconds tick_of_day = date::floor<std::chrono::seconds>(d2);
  const std::chrono::nanoseconds tick_of_second = d2 - tick_of_day;
  ticks_[i] = tick.count();
  ticks_of_day_[i] = tick_of_day.count();
  ticks_of_second_[i] = tick_of_second.count();
}

} // namespace writable

} // namespace rclock

#endif
