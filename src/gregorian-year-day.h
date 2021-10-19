#ifndef CLOCK_GREGORIAN_YEAR_DAY_H
#define CLOCK_GREGORIAN_YEAR_DAY_H

#include "clock.h"
#include "integers.h"
#include "enums.h"
#include "utils.h"
#include "stream.h"
#include "resolve.h"
#include "check.h"

namespace rclock {

namespace yearday {

namespace detail {

inline
ordinal::year_yearday
resolve_next_day_yyd(const ordinal::year_yearday& x) {
  return {x.year() + ordinal::years{1}, ordinal::yearday{1}};
}
inline
ordinal::year_yearday
resolve_previous_day_yyd(const ordinal::year_yearday& x) {
  return {x.year(), ordinal::yearday{365}};
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
inline void check_range<component::day>(const int& value, const char* arg) {
  check_range_ordinal_day(value, arg);
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

inline
std::ostringstream&
stream_year(std::ostringstream& os, int year) NOEXCEPT
{
  os << ordinal::year{year};
  return os;
}

inline
std::ostringstream&
stream_day(std::ostringstream& os, int day) NOEXCEPT
{
  os << ordinal::yearday{static_cast<unsigned>(day)};
  return os;
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

  void assign_year(const ordinal::year& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  template <component Component>
  void check_range(const int& value, const char* arg) const;

  ordinal::year to_year(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class yyd : public y
{
protected:
  rclock::integers day_;

public:
  yyd(r_ssize size);
  yyd(const cpp11::integers& year,
      const cpp11::integers& day);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  void assign_day(const ordinal::yearday& x, r_ssize i) NOEXCEPT;
  void assign_year_yearday(const ordinal::year_yearday& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<date::days> to_sys_time(r_ssize i) const NOEXCEPT;
  ordinal::year_yearday to_year_yearday(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class yydh : public yyd
{
protected:
  rclock::integers hour_;

public:
  yydh(r_ssize size);
  yydh(const cpp11::integers& year,
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

class yydhm : public yydh
{
protected:
  rclock::integers minute_;

public:
  yydhm(r_ssize size);
  yydhm(const cpp11::integers& year,
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

class yydhms : public yydhm
{
protected:
  rclock::integers second_;

public:
  yydhms(r_ssize size);
  yydhms(const cpp11::integers& year,
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
class yydhmss : public yydhms
{
protected:
  rclock::integers subsecond_;

public:
  yydhmss(r_ssize size);
  yydhmss(const cpp11::integers& year,
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
y::assign_year(const ordinal::year& x, r_ssize i) NOEXCEPT
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
ordinal::year
y::to_year(r_ssize i) const NOEXCEPT
{
  return ordinal::year{year_[i]};
}

inline
cpp11::writable::list
y::to_list() const
{
  cpp11::writable::list out({year_.sexp()});
  out.names() = {"year"};
  return out;
}

// yyd

inline
yyd::yyd(r_ssize size)
  : y(size),
    day_(size)
  {}

inline
yyd::yyd(const cpp11::integers& year, const cpp11::integers& day)
  : y(year),
    day_(day)
  {}

inline
std::ostringstream&
yyd::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  y::stream(os, i);
  os << '-';
  detail::stream_day(os, day_[i]);
  return os;
}

inline
bool
yyd::ok(r_ssize i) const NOEXCEPT
{
  return to_year_yearday(i).ok();
}

inline
void
yyd::assign_day(const ordinal::yearday& x, r_ssize i) NOEXCEPT
{
  day_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

inline
void
yyd::assign_year_yearday(const ordinal::year_yearday& x, r_ssize i) NOEXCEPT
{
  assign_year(x.year(), i);
  assign_day(x.yearday(), i);
}

inline
void
yyd::assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT
{
  ordinal::year_yearday yyd{x};
  assign_year_yearday(yyd, i);
}

inline
void
yyd::assign_na(r_ssize i) NOEXCEPT
{
  y::assign_na(i);
  day_.assign_na(i);
}

inline
void
yyd::resolve(r_ssize i, const enum invalid type)
{
  const ordinal::year_yearday elt = to_year_yearday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
  case invalid::next: {
    assign_year_yearday(detail::resolve_next_day_yyd(elt), i);
    break;
  }
  case invalid::previous_day:
  case invalid::previous: {
    assign_day(detail::resolve_previous_day_yyd(elt).yearday(), i);
    break;
  }
  case invalid::overflow_day:
  case invalid::overflow: {
    assign_year_yearday(date::sys_days{elt}, i);
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
yyd::to_sys_time(r_ssize i) const NOEXCEPT
{
  return date::sys_time<date::days>{to_year_yearday(i)};
}

inline
ordinal::year_yearday
yyd::to_year_yearday(r_ssize i) const NOEXCEPT
{
  return ordinal::year{year_[i]} / static_cast<unsigned>(day_[i]);
}

inline
cpp11::writable::list
yyd::to_list() const
{
  cpp11::writable::list out({year_.sexp(), day_.sexp()});
  out.names() = {"year", "day"};
  return out;
}

// yydh

inline
yydh::yydh(r_ssize size)
  : yyd(size),
    hour_(size)
  {}

inline
yydh::yydh(const cpp11::integers& year,
           const cpp11::integers& day,
           const cpp11::integers& hour)
  : yyd(year, day),
    hour_(hour)
{}

inline
std::ostringstream&
yydh::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yyd::stream(os, i);
  os << 'T';
  rclock::detail::stream_hour(os, hour_[i]);
  return os;
}

inline
void
yydh::assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT
{
  hour_.assign(x.count(), i);
}

inline
void
yydh::assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<date::days> day_point = date::floor<date::days>(x);
  const std::chrono::hours hours = x - day_point;
  yyd::assign_sys_time(day_point, i);
  assign_hour(hours, i);
}

inline
void
yydh::assign_na(r_ssize i) NOEXCEPT
{
  yyd::assign_na(i);
  hour_.assign_na(i);
}

inline
void
yydh::resolve(r_ssize i, const enum invalid type)
{
  const ordinal::year_yearday elt = to_year_yearday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_yearday(detail::resolve_next_day_yyd(elt), i);
    break;
  case invalid::next: {
    assign_year_yearday(detail::resolve_next_day_yyd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    break;
  }
  case invalid::previous_day:
    assign_day(detail::resolve_previous_day_yyd(elt).yearday(), i);
    break;
  case invalid::previous: {
    assign_day(detail::resolve_previous_day_yyd(elt).yearday(), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_yearday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_yearday(date::sys_days{elt}, i);
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
yydh::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yyd::to_sys_time(i) + std::chrono::hours{hour_[i]};
}

inline
cpp11::writable::list
yydh::to_list() const
{
  cpp11::writable::list out({year_.sexp(), day_.sexp(), hour_.sexp()});
  out.names() = {"year", "day", "hour"};
  return out;
}

// yydhm

inline
yydhm::yydhm(r_ssize size)
  : yydh(size),
    minute_(size)
  {}

inline
yydhm::yydhm(const cpp11::integers& year,
             const cpp11::integers& day,
             const cpp11::integers& hour,
             const cpp11::integers& minute)
  : yydh(year, day, hour),
    minute_(minute)
  {}

inline
std::ostringstream&
yydhm::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yydh::stream(os, i);
  os << ':';
  rclock::detail::stream_minute(os, minute_[i]);
  return os;
}

inline
void
yydhm::assign_minute(const std::chrono::minutes& x, r_ssize i) NOEXCEPT
{
  minute_.assign(x.count(), i);
}

inline
void
yydhm::assign_sys_time(const date::sys_time<std::chrono::minutes>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::hours> hour_point = date::floor<std::chrono::hours>(x);
  const std::chrono::minutes minutes = x - hour_point;
  yydh::assign_sys_time(hour_point, i);
  assign_minute(minutes, i);
}

inline
void
yydhm::assign_na(r_ssize i) NOEXCEPT
{
  yydh::assign_na(i);
  minute_.assign_na(i);
}

inline
void
yydhm::resolve(r_ssize i, const enum invalid type)
{
  const ordinal::year_yearday elt = to_year_yearday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_yearday(detail::resolve_next_day_yyd(elt), i);
    break;
  case invalid::next: {
    assign_year_yearday(detail::resolve_next_day_yyd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    break;
  }
  case invalid::previous_day:
    assign_day(detail::resolve_previous_day_yyd(elt).yearday(), i);
    break;
  case invalid::previous: {
    assign_day(detail::resolve_previous_day_yyd(elt).yearday(), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_yearday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_yearday(date::sys_days{elt}, i);
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
yydhm::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yydh::to_sys_time(i) + std::chrono::minutes{minute_[i]};
}

inline
cpp11::writable::list
yydhm::to_list() const
{
  cpp11::writable::list out({year_.sexp(), day_.sexp(), hour_.sexp(), minute_.sexp()});
  out.names() = {"year", "day", "hour", "minute"};
  return out;
}

// yydhms

inline
yydhms::yydhms(r_ssize size)
  : yydhm(size),
    second_(size)
  {}

inline
yydhms::yydhms(const cpp11::integers& year,
               const cpp11::integers& day,
               const cpp11::integers& hour,
               const cpp11::integers& minute,
               const cpp11::integers& second)
  : yydhm(year, day, hour, minute),
    second_(second)
  {}

inline
std::ostringstream&
yydhms::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yydhm::stream(os, i);
  os << ':';
  rclock::detail::stream_second(os, second_[i]);
  return os;
}

inline
void
yydhms::assign_second(const std::chrono::seconds& x, r_ssize i) NOEXCEPT
{
  second_.assign(x.count(), i);
}

inline
void
yydhms::assign_sys_time(const date::sys_time<std::chrono::seconds>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::minutes> minute_point = date::floor<std::chrono::minutes>(x);
  const std::chrono::seconds seconds = x - minute_point;
  yydhm::assign_sys_time(minute_point, i);
  assign_second(seconds, i);
}

inline
void
yydhms::assign_na(r_ssize i) NOEXCEPT
{
  yydhm::assign_na(i);
  second_.assign_na(i);
}

inline
void
yydhms::resolve(r_ssize i, const enum invalid type)
{
  const ordinal::year_yearday elt = to_year_yearday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_yearday(detail::resolve_next_day_yyd(elt), i);
    break;
  case invalid::next: {
    assign_year_yearday(detail::resolve_next_day_yyd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    break;
  }
  case invalid::previous_day:
    assign_day(detail::resolve_previous_day_yyd(elt).yearday(), i);
    break;
  case invalid::previous: {
    assign_day(detail::resolve_previous_day_yyd(elt).yearday(), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    assign_second(rclock::detail::resolve_previous_second(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_yearday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_yearday(date::sys_days{elt}, i);
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
yydhms::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yydhm::to_sys_time(i) + std::chrono::seconds{second_[i]};
}

inline
cpp11::writable::list
yydhms::to_list() const
{
  cpp11::writable::list out({year_.sexp(), day_.sexp(), hour_.sexp(), minute_.sexp(), second_.sexp()});
  out.names() = {"year", "day", "hour", "minute", "second"};
  return out;
}

// yydhmss

template <typename Duration>
inline
yydhmss<Duration>::yydhmss(r_ssize size)
  : yydhms(size),
    subsecond_(size)
  {}

template <typename Duration>
inline
yydhmss<Duration>::yydhmss(const cpp11::integers& year,
                           const cpp11::integers& day,
                           const cpp11::integers& hour,
                           const cpp11::integers& minute,
                           const cpp11::integers& second,
                           const cpp11::integers& subsecond)
  : yydhms(year, day, hour, minute, second),
    subsecond_(subsecond)
  {}

template <typename Duration>
inline
std::ostringstream&
yydhmss<Duration>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yydhm::stream(os, i);
  os << ':';
  rclock::detail::stream_second_and_subsecond<Duration>(os, second_[i], subsecond_[i]);
  return os;
}

template <typename Duration>
inline
void
yydhmss<Duration>::assign_subsecond(const Duration& x, r_ssize i) NOEXCEPT
{
  subsecond_.assign(x.count(), i);
}

template <typename Duration>
inline
void
yydhmss<Duration>::assign_sys_time(const date::sys_time<Duration>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::seconds> second_point = date::floor<std::chrono::seconds>(x);
  const Duration subseconds = x - second_point;
  yydhms::assign_sys_time(second_point, i);
  assign_subsecond(subseconds, i);
}

template <typename Duration>
inline
void
yydhmss<Duration>::assign_na(r_ssize i) NOEXCEPT
{
  yydhms::assign_na(i);
  subsecond_.assign_na(i);
}

template <typename Duration>
inline
void
yydhmss<Duration>::resolve(r_ssize i, const enum invalid type)
{
  const ordinal::year_yearday elt = to_year_yearday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_yearday(detail::resolve_next_day_yyd(elt), i);
    break;
  case invalid::next: {
    assign_year_yearday(detail::resolve_next_day_yyd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    assign_subsecond(rclock::detail::resolve_next_subsecond<Duration>(), i);
    break;
  }
  case invalid::previous_day:
    assign_day(detail::resolve_previous_day_yyd(elt).yearday(), i);
    break;
  case invalid::previous: {
    assign_day(detail::resolve_previous_day_yyd(elt).yearday(), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    assign_second(rclock::detail::resolve_previous_second(), i);
    assign_subsecond(rclock::detail::resolve_previous_subsecond<Duration>(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_yearday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_yearday(date::sys_days{elt}, i);
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
yydhmss<Duration>::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yydhms::to_sys_time(i) + Duration{subsecond_[i]};
}

template <typename Duration>
inline
cpp11::writable::list
yydhmss<Duration>::to_list() const
{
  cpp11::writable::list out({year_.sexp(), day_.sexp(), hour_.sexp(), minute_.sexp(), second_.sexp(), subsecond_.sexp()});
  out.names() = {"year", "day", "hour", "minute", "second", "subsecond"};
  return out;
}

} // namespace yearday

} // namespace rclock

#endif
