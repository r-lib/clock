#ifndef CLOCK_GREGORIAN_YEAR_MONTH_DAY_H
#define CLOCK_GREGORIAN_YEAR_MONTH_DAY_H

#include "clock.h"
#include "integers.h"
#include "enums.h"
#include "utils.h"
#include "stream.h"
#include "resolve.h"
#include "check.h"

namespace rclock {

namespace gregorian {

namespace detail {

inline
date::year_month_day
resolve_next_day_ymd(const date::year_month_day& x) {
  return ((x.year() / x.month()) + date::months(1)) / date::day(1);
}
inline
date::year_month_day
resolve_previous_day_ymd(const date::year_month_day& x) {
  return x.year() / x.month() / date::last;
}

template <enum component Component>
inline void check_range(const int& value, const char* arg) {
  clock_abort("Unimplemented range check");
}
template <>
inline void check_range<component::year>(const int& value, const char* arg) {
  check_range_year(value, arg);
}
template <>
inline void check_range<component::month>(const int& value, const char* arg) {
  check_range_month(value, arg);
}
template <>
inline void check_range<component::day>(const int& value, const char* arg) {
  check_range_day(value, arg);
}
template <>
inline void check_range<component::hour>(const int& value, const char* arg) {
  check_range_hour(value, arg);
}
template <>
inline void check_range<component::minute>(const int& value, const char* arg) {
  check_range_minute(value, arg);
}
template <>
inline void check_range<component::second>(const int& value, const char* arg) {
  check_range_second(value, arg);
}
template <>
inline void check_range<component::millisecond>(const int& value, const char* arg) {
  check_range_millisecond(value, arg);
}
template <>
inline void check_range<component::microsecond>(const int& value, const char* arg) {
  check_range_microsecond(value, arg);
}
template <>
inline void check_range<component::nanosecond>(const int& value, const char* arg) {
  check_range_nanosecond(value, arg);
}

} // namespace detail

class y
{
protected:
  rclock::integers year_;

public:
  y(r_ssize size);
  y(const cpp11::integers& year);

  bool is_na(r_ssize i) const NOEXCEPT;
  r_ssize size() const NOEXCEPT;

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  void add(const date::years& x, r_ssize i) NOEXCEPT;

  void assign_year(const date::year& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  template <component Component>
  void check_range(const int& value, const char* arg) const;

  date::year to_year(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ym : public y
{
protected:
  rclock::integers month_;

public:
  ym(r_ssize size);
  ym(const cpp11::integers& year,
     const cpp11::integers& month);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  void add(const date::months& x, r_ssize i) NOEXCEPT;

  void assign_month(const date::month& x, r_ssize i) NOEXCEPT;
  void assign_year_month(const date::year_month& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::year_month to_year_month(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ymd : public ym
{
protected:
  rclock::integers day_;

public:
  ymd(r_ssize size);
  ymd(const cpp11::integers& year,
      const cpp11::integers& month,
      const cpp11::integers& day);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  void assign_day(const date::day& x, r_ssize i) NOEXCEPT;
  void assign_year_month_day(const date::year_month_day& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<date::days> to_sys_time(r_ssize i) const NOEXCEPT;
  date::year_month_day to_year_month_day(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ymdh : public ymd
{
protected:
  rclock::integers hour_;

public:
  ymdh(r_ssize size);
  ymdh(const cpp11::integers& year,
       const cpp11::integers& month,
       const cpp11::integers& day,
       const cpp11::integers& hour);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<std::chrono::hours> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ymdhm : public ymdh
{
protected:
  rclock::integers minute_;

public:
  ymdhm(r_ssize size);
  ymdhm(const cpp11::integers& year,
        const cpp11::integers& month,
        const cpp11::integers& day,
        const cpp11::integers& hour,
        const cpp11::integers& minute);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_minute(const std::chrono::minutes& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::minutes>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<std::chrono::minutes> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ymdhms : public ymdhm
{
protected:
  rclock::integers second_;

public:
  ymdhms(r_ssize size);
  ymdhms(const cpp11::integers& year,
         const cpp11::integers& month,
         const cpp11::integers& day,
         const cpp11::integers& hour,
         const cpp11::integers& minute,
         const cpp11::integers& second);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_second(const std::chrono::seconds& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::seconds>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<std::chrono::seconds> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

template <typename Duration>
class ymdhmss : public ymdhms
{
protected:
  rclock::integers subsecond_;

public:
  ymdhmss(r_ssize size);
  ymdhmss(const cpp11::integers& year,
          const cpp11::integers& month,
          const cpp11::integers& day,
          const cpp11::integers& hour,
          const cpp11::integers& minute,
          const cpp11::integers& second,
          const cpp11::integers& subsecond);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_subsecond(const Duration& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<Duration>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<Duration> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;

  using duration = Duration;
};

// Implementation

// y

inline
y::y(r_ssize size)
  : year_(size)
  {}

inline
y::y(const cpp11::integers& year)
  : year_(rclock::integers(year))
  {}

inline
bool
y::is_na(r_ssize i) const NOEXCEPT
{
  return year_.is_na(i);
}

inline
r_ssize
y::size() const NOEXCEPT
{
  return year_.size();
}

inline
std::ostringstream&
y::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  rclock::detail::stream_year(os, year_[i]);
  return os;
}

inline
bool
y::ok(r_ssize i) const NOEXCEPT
{
  return true;
}

inline
void
y::add(const date::years& x, r_ssize i) NOEXCEPT
{
  assign_year(to_year(i) + x, i);
}

inline
void
y::assign_year(const date::year& x, r_ssize i) NOEXCEPT
{
  year_.assign(static_cast<int>(x), i);
}

inline
void
y::assign_na(r_ssize i) NOEXCEPT
{
  year_.assign_na(i);
}

inline
void
y::resolve(r_ssize i, const enum invalid type)
{
  // Never invalid
}

template <component Component>
inline
void
y::check_range(const int& value, const char* arg) const
{
  detail::check_range<Component>(value, arg);
}

inline
date::year
y::to_year(r_ssize i) const NOEXCEPT
{
  return date::year{year_[i]};
}

inline
cpp11::writable::list
y::to_list() const
{
  cpp11::writable::list out({year_.sexp()});
  out.names() = {"year"};
  return out;
}

// ym

inline
ym::ym(r_ssize size)
  : y(size),
    month_(size)
  {}


inline
ym::ym(const cpp11::integers& year,
       const cpp11::integers& month)
  : y(year),
    month_(month)
  {}

inline
std::ostringstream&
ym::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  y::stream(os, i);
  os << '-';
  rclock::detail::stream_month(os, month_[i]);
  return os;
}

inline
bool
ym::ok(r_ssize i) const NOEXCEPT
{
  return true;
}

inline
void
ym::add(const date::months& x, r_ssize i) NOEXCEPT
{
  assign_year_month(to_year_month(i) + x, i);
}

inline
void
ym::assign_month(const date::month& x, r_ssize i) NOEXCEPT
{
  month_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

inline
void
ym::assign_year_month(const date::year_month& x, r_ssize i) NOEXCEPT
{
  assign_year(x.year(), i);
  assign_month(x.month(), i);
}

inline
void
ym::assign_na(r_ssize i) NOEXCEPT
{
  y::assign_na(i);
  month_.assign_na(i);
}

inline
void
ym::resolve(r_ssize i, const enum invalid type)
{
  // Never invalid
}

inline
date::year_month
ym::to_year_month(r_ssize i) const NOEXCEPT
{
  return date::year{year_[i]} / static_cast<unsigned>(month_[i]);
}

inline
cpp11::writable::list
ym::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp()});
  out.names() = {"year", "month"};
  return out;
}

// ymd

inline
ymd::ymd(r_ssize size)
  : ym(size),
    day_(size)
  {}

inline
ymd::ymd(const cpp11::integers& year,
         const cpp11::integers& month,
         const cpp11::integers& day)
  : ym(year, month),
    day_(day)
  {}

inline
std::ostringstream&
ymd::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ym::stream(os, i);
  os << '-';
  rclock::detail::stream_day(os, day_[i]);
  return os;
}

inline
bool
ymd::ok(r_ssize i) const NOEXCEPT
{
  return to_year_month_day(i).ok();
}

inline
void
ymd::assign_day(const date::day& x, r_ssize i) NOEXCEPT
{
  day_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

inline
void
ymd::assign_year_month_day(const date::year_month_day& x, r_ssize i) NOEXCEPT
{
  assign_year(x.year(), i);
  assign_month(x.month(), i);
  assign_day(x.day(), i);
}

inline
void
ymd::assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT
{
  date::year_month_day ymd{x};
  assign_year_month_day(ymd, i);
}

inline
void
ymd::assign_na(r_ssize i) NOEXCEPT
{
  ym::assign_na(i);
  day_.assign_na(i);
}

inline
void
ymd::resolve(r_ssize i, const enum invalid type)
{
  const date::year_month_day elt = to_year_month_day(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
  case invalid::next: {
    assign_year_month_day(detail::resolve_next_day_ymd(elt), i);
    break;
  }
  case invalid::previous_day:
  case invalid::previous: {
    assign_day(detail::resolve_previous_day_ymd(elt).day(), i);
    break;
  }
  case invalid::overflow_day:
  case invalid::overflow: {
    assign_year_month_day(date::sys_days{elt}, i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    rclock::detail::resolve_error(i);
  }
  }
}

inline
date::sys_time<date::days>
ymd::to_sys_time(r_ssize i) const NOEXCEPT
{
  return date::sys_time<date::days>{to_year_month_day(i)};
}

inline
date::year_month_day
ymd::to_year_month_day(r_ssize i) const NOEXCEPT
{
  return date::year{year_[i]} / static_cast<unsigned>(month_[i]) / static_cast<unsigned>(day_[i]);
}

inline
cpp11::writable::list
ymd::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp(), day_.sexp()});
  out.names() = {"year", "month", "day"};
  return out;
}

// ymdh

inline
ymdh::ymdh(r_ssize size)
  : ymd(size),
    hour_(size)
  {}

inline
ymdh::ymdh(const cpp11::integers& year,
           const cpp11::integers& month,
           const cpp11::integers& day,
           const cpp11::integers& hour)
  : ymd(year, month, day),
    hour_(hour)
{}

inline
std::ostringstream&
ymdh::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ymd::stream(os, i);
  os << 'T';
  rclock::detail::stream_hour(os, hour_[i]);
  return os;
}

inline
void
ymdh::assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT
{
  hour_.assign(x.count(), i);
}

inline
void
ymdh::assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<date::days> day_point = date::floor<date::days>(x);
  const std::chrono::hours hours = x - day_point;
  ymd::assign_sys_time(day_point, i);
  assign_hour(hours, i);
}

inline
void
ymdh::assign_na(r_ssize i) NOEXCEPT
{
  ymd::assign_na(i);
  hour_.assign_na(i);
}

inline
void
ymdh::resolve(r_ssize i, const enum invalid type)
{
  const date::year_month_day elt = to_year_month_day(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_month_day(detail::resolve_next_day_ymd(elt), i);
    break;
  case invalid::next: {
    assign_year_month_day(detail::resolve_next_day_ymd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    break;
  }
  case invalid::previous_day:
    assign_day(detail::resolve_previous_day_ymd(elt).day(), i);
    break;
  case invalid::previous: {
    assign_day(detail::resolve_previous_day_ymd(elt).day(), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_month_day(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_month_day(date::sys_days{elt}, i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    rclock::detail::resolve_error(i);
  }
  }
}

inline
date::sys_time<std::chrono::hours>
ymdh::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ymd::to_sys_time(i) + std::chrono::hours{hour_[i]};
}

inline
cpp11::writable::list
ymdh::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp(), day_.sexp(), hour_.sexp()});
  out.names() = {"year", "month", "day", "hour"};
  return out;
}

// ymdhm

inline
ymdhm::ymdhm(r_ssize size)
  : ymdh(size),
    minute_(size)
  {}

inline
ymdhm::ymdhm(const cpp11::integers& year,
             const cpp11::integers& month,
             const cpp11::integers& day,
             const cpp11::integers& hour,
             const cpp11::integers& minute)
  : ymdh(year, month, day, hour),
    minute_(minute)
  {}

inline
std::ostringstream&
ymdhm::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ymdh::stream(os, i);
  os << ':';
  rclock::detail::stream_minute(os, minute_[i]);
  return os;
}

inline
void
ymdhm::assign_minute(const std::chrono::minutes& x, r_ssize i) NOEXCEPT
{
  minute_.assign(x.count(), i);
}

inline
void
ymdhm::assign_sys_time(const date::sys_time<std::chrono::minutes>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::hours> hour_point = date::floor<std::chrono::hours>(x);
  const std::chrono::minutes minutes = x - hour_point;
  ymdh::assign_sys_time(hour_point, i);
  assign_minute(minutes, i);
}

inline
void
ymdhm::assign_na(r_ssize i) NOEXCEPT
{
  ymdh::assign_na(i);
  minute_.assign_na(i);
}

inline
void
ymdhm::resolve(r_ssize i, const enum invalid type)
{
  const date::year_month_day elt = to_year_month_day(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_month_day(detail::resolve_next_day_ymd(elt), i);
    break;
  case invalid::next: {
    assign_year_month_day(detail::resolve_next_day_ymd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    break;
  }
  case invalid::previous_day:
    assign_day(detail::resolve_previous_day_ymd(elt).day(), i);
    break;
  case invalid::previous: {
    assign_day(detail::resolve_previous_day_ymd(elt).day(), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_month_day(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_month_day(date::sys_days{elt}, i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    rclock::detail::resolve_error(i);
  }
  }
}

inline
date::sys_time<std::chrono::minutes>
ymdhm::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ymdh::to_sys_time(i) + std::chrono::minutes{minute_[i]};
}

inline
cpp11::writable::list
ymdhm::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp(), day_.sexp(), hour_.sexp(), minute_.sexp()});
  out.names() = {"year", "month", "day", "hour", "minute"};
  return out;
}

// ymdhms

inline
ymdhms::ymdhms(r_ssize size)
  : ymdhm(size),
    second_(size)
  {}

inline
ymdhms::ymdhms(const cpp11::integers& year,
               const cpp11::integers& month,
               const cpp11::integers& day,
               const cpp11::integers& hour,
               const cpp11::integers& minute,
               const cpp11::integers& second)
  : ymdhm(year, month, day, hour, minute),
    second_(second)
  {}

inline
std::ostringstream&
ymdhms::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ymdhm::stream(os, i);
  os << ':';
  rclock::detail::stream_second(os, second_[i]);
  return os;
}

inline
void
ymdhms::assign_second(const std::chrono::seconds& x, r_ssize i) NOEXCEPT
{
  second_.assign(x.count(), i);
}

inline
void
ymdhms::assign_sys_time(const date::sys_time<std::chrono::seconds>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::minutes> minute_point = date::floor<std::chrono::minutes>(x);
  const std::chrono::seconds seconds = x - minute_point;
  ymdhm::assign_sys_time(minute_point, i);
  assign_second(seconds, i);
}

inline
void
ymdhms::assign_na(r_ssize i) NOEXCEPT
{
  ymdhm::assign_na(i);
  second_.assign_na(i);
}

inline
void
ymdhms::resolve(r_ssize i, const enum invalid type)
{
  const date::year_month_day elt = to_year_month_day(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_month_day(detail::resolve_next_day_ymd(elt), i);
    break;
  case invalid::next: {
    assign_year_month_day(detail::resolve_next_day_ymd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    break;
  }
  case invalid::previous_day:
    assign_day(detail::resolve_previous_day_ymd(elt).day(), i);
    break;
  case invalid::previous: {
    assign_day(detail::resolve_previous_day_ymd(elt).day(), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    assign_second(rclock::detail::resolve_previous_second(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_month_day(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_month_day(date::sys_days{elt}, i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    rclock::detail::resolve_error(i);
  }
  }
}

inline
date::sys_time<std::chrono::seconds>
ymdhms::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ymdhm::to_sys_time(i) + std::chrono::seconds{second_[i]};
}

inline
cpp11::writable::list
ymdhms::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp(), day_.sexp(), hour_.sexp(), minute_.sexp(), second_.sexp()});
  out.names() = {"year", "month", "day", "hour", "minute", "second"};
  return out;
}

// ymdhmss

template <typename Duration>
inline
ymdhmss<Duration>::ymdhmss(r_ssize size)
  : ymdhms(size),
    subsecond_(size)
  {}

template <typename Duration>
inline
ymdhmss<Duration>::ymdhmss(const cpp11::integers& year,
                           const cpp11::integers& month,
                           const cpp11::integers& day,
                           const cpp11::integers& hour,
                           const cpp11::integers& minute,
                           const cpp11::integers& second,
                           const cpp11::integers& subsecond)
  : ymdhms(year, month, day, hour, minute, second),
    subsecond_(subsecond)
  {}

template <typename Duration>
inline
std::ostringstream&
ymdhmss<Duration>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ymdhm::stream(os, i);
  os << ':';
  rclock::detail::stream_second_and_subsecond<Duration>(os, second_[i], subsecond_[i]);
  return os;
}

template <typename Duration>
inline
void
ymdhmss<Duration>::assign_subsecond(const Duration& x, r_ssize i) NOEXCEPT
{
  subsecond_.assign(x.count(), i);
}

template <typename Duration>
inline
void
ymdhmss<Duration>::assign_sys_time(const date::sys_time<Duration>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::seconds> second_point = date::floor<std::chrono::seconds>(x);
  const Duration subseconds = x - second_point;
  ymdhms::assign_sys_time(second_point, i);
  assign_subsecond(subseconds, i);
}

template <typename Duration>
inline
void
ymdhmss<Duration>::assign_na(r_ssize i) NOEXCEPT
{
  ymdhms::assign_na(i);
  subsecond_.assign_na(i);
}

template <typename Duration>
inline
void
ymdhmss<Duration>::resolve(r_ssize i, const enum invalid type)
{
  const date::year_month_day elt = to_year_month_day(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_month_day(detail::resolve_next_day_ymd(elt), i);
    break;
  case invalid::next: {
    assign_year_month_day(detail::resolve_next_day_ymd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    assign_subsecond(rclock::detail::resolve_next_subsecond<Duration>(), i);
    break;
  }
  case invalid::previous_day:
    assign_day(detail::resolve_previous_day_ymd(elt).day(), i);
    break;
  case invalid::previous: {
    assign_day(detail::resolve_previous_day_ymd(elt).day(), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    assign_second(rclock::detail::resolve_previous_second(), i);
    assign_subsecond(rclock::detail::resolve_previous_subsecond<Duration>(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_month_day(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_month_day(date::sys_days{elt}, i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    assign_subsecond(rclock::detail::resolve_next_subsecond<Duration>(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    rclock::detail::resolve_error(i);
  }
  }
}

template <typename Duration>
inline
date::sys_time<Duration>
ymdhmss<Duration>::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ymdhms::to_sys_time(i) + Duration{subsecond_[i]};
}

template <typename Duration>
inline
cpp11::writable::list
ymdhmss<Duration>::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp(), day_.sexp(), hour_.sexp(), minute_.sexp(), second_.sexp(), subsecond_.sexp()});
  out.names() = {"year", "month", "day", "hour", "minute", "second", "subsecond"};
  return out;
}

} // namespace gregorian

} // namespace rclock

#endif
