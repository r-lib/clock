#ifndef CLOCK_GREGORIAN_YEAR_MONTH_WEEKDAY_H
#define CLOCK_GREGORIAN_YEAR_MONTH_WEEKDAY_H

#include "clock.h"
#include "integers.h"
#include "enums.h"
#include "utils.h"
#include "stream.h"
#include "resolve.h"
#include "check.h"

namespace rclock {

namespace weekday {

namespace detail {

inline
std::ostringstream&
stream_day(std::ostringstream& os, int day) NOEXCEPT
{
  os << date::weekday{static_cast<unsigned>(day - 1)};
  return os;
}
inline
std::ostringstream&
stream_index(std::ostringstream& os, int index) NOEXCEPT
{
  os << index;
  return os;
}

inline
date::year_month_weekday
resolve_next_day_ymw(const date::year_month_weekday& x) {
  // First day in next month
  return date::year_month_weekday{((x.year() / x.month()) + date::months(1)) / date::day(1)};
}
inline
date::year_month_weekday
resolve_previous_day_ymw(const date::year_month_weekday& x) {
  // Last day in current month
  return date::year_month_weekday{x.year() / x.month() / date::last};
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
  check_range_weekday_day(value, arg);
}
template <>
inline void check_range<component::index>(const int& value, const char* arg) {
  check_range_weekday_index(value, arg);
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

class ymwd : public ym
{
protected:
  rclock::integers day_;
  rclock::integers index_;

public:
  ymwd(r_ssize size);
  ymwd(const cpp11::integers& year,
       const cpp11::integers& month,
       const cpp11::integers& day,
       const cpp11::integers& index);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  void assign_weekday(const date::weekday& x, r_ssize i) NOEXCEPT;
  void assign_index(const unsigned& x, r_ssize i) NOEXCEPT;
  void assign_year_month_weekday(const date::year_month_weekday& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<date::days> to_sys_time(r_ssize i) const NOEXCEPT;
  date::year_month_weekday to_year_month_weekday(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ymwdh : public ymwd
{
protected:
  rclock::integers hour_;

public:
  ymwdh(r_ssize size);
  ymwdh(const cpp11::integers& year,
        const cpp11::integers& month,
        const cpp11::integers& day,
        const cpp11::integers& index,
        const cpp11::integers& hour);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<std::chrono::hours> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ymwdhm : public ymwdh
{
protected:
  rclock::integers minute_;

public:
  ymwdhm(r_ssize size);
  ymwdhm(const cpp11::integers& year,
         const cpp11::integers& month,
         const cpp11::integers& day,
         const cpp11::integers& index,
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

class ymwdhms : public ymwdhm
{
protected:
  rclock::integers second_;

public:
  ymwdhms(r_ssize size);
  ymwdhms(const cpp11::integers& year,
          const cpp11::integers& month,
          const cpp11::integers& day,
          const cpp11::integers& index,
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
class ymwdhmss : public ymwdhms
{
protected:
  rclock::integers subsecond_;

public:
  ymwdhmss(r_ssize size);
  ymwdhmss(const cpp11::integers& year,
           const cpp11::integers& month,
           const cpp11::integers& day,
           const cpp11::integers& index,
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

// ymwd

inline
ymwd::ymwd(r_ssize size)
  : ym(size),
    day_(size),
    index_(size)
  {}

inline
ymwd::ymwd(const cpp11::integers& year,
           const cpp11::integers& month,
           const cpp11::integers& day,
           const cpp11::integers& index)
  : ym(year, month),
    day_(day),
    index_(index)
  {}

inline
std::ostringstream&
ymwd::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ym::stream(os, i);
  os << '-';
  detail::stream_day(os, day_[i]);
  os << '[';
  detail::stream_index(os, index_[i]);
  os << ']';
  return os;
}

inline
bool
ymwd::ok(r_ssize i) const NOEXCEPT
{
  return to_year_month_weekday(i).ok();
}

inline
void
ymwd::assign_weekday(const date::weekday& x, r_ssize i) NOEXCEPT
{
  day_.assign(static_cast<int>(x.c_encoding() + 1), i);
}

inline
void
ymwd::assign_index(const unsigned& x, r_ssize i) NOEXCEPT
{
  index_.assign(static_cast<int>(x), i);
}

inline
void
ymwd::assign_year_month_weekday(const date::year_month_weekday& x, r_ssize i) NOEXCEPT
{
  assign_year(x.year(), i);
  assign_month(x.month(), i);
  assign_weekday(x.weekday(), i);
  assign_index(x.index(), i);
}

inline
void
ymwd::assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT
{
  date::year_month_weekday ymwd{x};
  assign_year_month_weekday(ymwd, i);
}

inline
void
ymwd::assign_na(r_ssize i) NOEXCEPT
{
  ym::assign_na(i);
  day_.assign_na(i);
  index_.assign_na(i);
}

inline
void
ymwd::resolve(r_ssize i, const enum invalid type)
{
  const date::year_month_weekday elt = to_year_month_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
  case invalid::next: {
    assign_year_month_weekday(detail::resolve_next_day_ymw(elt), i);
    break;
  }
  case invalid::previous_day:
  case invalid::previous: {
    assign_year_month_weekday(detail::resolve_previous_day_ymw(elt), i);
    break;
  }
  case invalid::overflow_day:
  case invalid::overflow: {
    assign_year_month_weekday(date::sys_days{elt}, i);
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
ymwd::to_sys_time(r_ssize i) const NOEXCEPT
{
  return date::sys_time<date::days>{to_year_month_weekday(i)};
}

inline
date::year_month_weekday
ymwd::to_year_month_weekday(r_ssize i) const NOEXCEPT
{
  return date::year{year_[i]} /
    date::month{static_cast<unsigned>(month_[i])} /
    date::weekday{static_cast<unsigned>(day_[i] - 1)}[static_cast<unsigned>(index_[i])];
}

inline
cpp11::writable::list
ymwd::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp(), day_.sexp(), index_.sexp()});
  out.names() = {"year", "month", "day", "index"};
  return out;
}

// ymwdh

inline
ymwdh::ymwdh(r_ssize size)
  : ymwd(size),
    hour_(size)
  {}

inline
ymwdh::ymwdh(const cpp11::integers& year,
             const cpp11::integers& month,
             const cpp11::integers& day,
             const cpp11::integers& index,
             const cpp11::integers& hour)
  : ymwd(year, month, day, index),
    hour_(hour)
{}

inline
std::ostringstream&
ymwdh::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ymwd::stream(os, i);
  os << ' ';
  rclock::detail::stream_hour(os, hour_[i]);
  return os;
}

inline
void
ymwdh::assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT
{
  hour_.assign(x.count(), i);
}

inline
void
ymwdh::assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<date::days> day_point = date::floor<date::days>(x);
  const std::chrono::hours hours = x - day_point;
  ymwd::assign_sys_time(day_point, i);
  assign_hour(hours, i);
}

inline
void
ymwdh::assign_na(r_ssize i) NOEXCEPT
{
  ymwd::assign_na(i);
  hour_.assign_na(i);
}

inline
void
ymwdh::resolve(r_ssize i, const enum invalid type)
{
  const date::year_month_weekday elt = to_year_month_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_month_weekday(detail::resolve_next_day_ymw(elt), i);
    break;
  case invalid::next: {
    assign_year_month_weekday(detail::resolve_next_day_ymw(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    break;
  }
  case invalid::previous_day:
    assign_year_month_weekday(detail::resolve_previous_day_ymw(elt), i);
    break;
  case invalid::previous: {
    assign_year_month_weekday(detail::resolve_previous_day_ymw(elt), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_month_weekday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_month_weekday(date::sys_days{elt}, i);
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
ymwdh::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ymwd::to_sys_time(i) + std::chrono::hours{hour_[i]};
}

inline
cpp11::writable::list
ymwdh::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp(), day_.sexp(), index_.sexp(), hour_.sexp()});
  out.names() = {"year", "month", "day", "index", "hour"};
  return out;
}

// ymwdhm

inline
ymwdhm::ymwdhm(r_ssize size)
  : ymwdh(size),
    minute_(size)
  {}

inline
ymwdhm::ymwdhm(const cpp11::integers& year,
               const cpp11::integers& month,
               const cpp11::integers& day,
               const cpp11::integers& index,
               const cpp11::integers& hour,
               const cpp11::integers& minute)
  : ymwdh(year, month, day, index, hour),
    minute_(minute)
  {}

inline
std::ostringstream&
ymwdhm::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ymwdh::stream(os, i);
  os << ':';
  rclock::detail::stream_minute(os, minute_[i]);
  return os;
}

inline
void
ymwdhm::assign_minute(const std::chrono::minutes& x, r_ssize i) NOEXCEPT
{
  minute_.assign(x.count(), i);
}

inline
void
ymwdhm::assign_sys_time(const date::sys_time<std::chrono::minutes>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::hours> hour_point = date::floor<std::chrono::hours>(x);
  const std::chrono::minutes minutes = x - hour_point;
  ymwdh::assign_sys_time(hour_point, i);
  assign_minute(minutes, i);
}

inline
void
ymwdhm::assign_na(r_ssize i) NOEXCEPT
{
  ymwdh::assign_na(i);
  minute_.assign_na(i);
}

inline
void
ymwdhm::resolve(r_ssize i, const enum invalid type)
{
  const date::year_month_weekday elt = to_year_month_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_month_weekday(detail::resolve_next_day_ymw(elt), i);
    break;
  case invalid::next: {
    assign_year_month_weekday(detail::resolve_next_day_ymw(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    break;
  }
  case invalid::previous_day:
    assign_year_month_weekday(detail::resolve_previous_day_ymw(elt), i);
    break;
  case invalid::previous: {
    assign_year_month_weekday(detail::resolve_previous_day_ymw(elt), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_month_weekday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_month_weekday(date::sys_days{elt}, i);
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
ymwdhm::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ymwdh::to_sys_time(i) + std::chrono::minutes{minute_[i]};
}

inline
cpp11::writable::list
ymwdhm::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp(), day_.sexp(), index_.sexp(), hour_.sexp(), minute_.sexp()});
  out.names() = {"year", "month", "day", "index", "hour", "minute"};
  return out;
}

// ymwdhms

inline
ymwdhms::ymwdhms(r_ssize size)
  : ymwdhm(size),
    second_(size)
  {}

inline
ymwdhms::ymwdhms(const cpp11::integers& year,
                 const cpp11::integers& month,
                 const cpp11::integers& day,
                 const cpp11::integers& index,
                 const cpp11::integers& hour,
                 const cpp11::integers& minute,
                 const cpp11::integers& second)
  : ymwdhm(year, month, day, index, hour, minute),
    second_(second)
  {}

inline
std::ostringstream&
ymwdhms::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ymwdhm::stream(os, i);
  os << ':';
  rclock::detail::stream_second(os, second_[i]);
  return os;
}

inline
void
ymwdhms::assign_second(const std::chrono::seconds& x, r_ssize i) NOEXCEPT
{
  second_.assign(x.count(), i);
}

inline
void
ymwdhms::assign_sys_time(const date::sys_time<std::chrono::seconds>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::minutes> minute_point = date::floor<std::chrono::minutes>(x);
  const std::chrono::seconds seconds = x - minute_point;
  ymwdhm::assign_sys_time(minute_point, i);
  assign_second(seconds, i);
}

inline
void
ymwdhms::assign_na(r_ssize i) NOEXCEPT
{
  ymwdhm::assign_na(i);
  second_.assign_na(i);
}

inline
void
ymwdhms::resolve(r_ssize i, const enum invalid type)
{
  const date::year_month_weekday elt = to_year_month_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_month_weekday(detail::resolve_next_day_ymw(elt), i);
    break;
  case invalid::next: {
    assign_year_month_weekday(detail::resolve_next_day_ymw(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    break;
  }
  case invalid::previous_day:
    assign_year_month_weekday(detail::resolve_previous_day_ymw(elt), i);
    break;
  case invalid::previous: {
    assign_year_month_weekday(detail::resolve_previous_day_ymw(elt), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    assign_second(rclock::detail::resolve_previous_second(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_month_weekday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_month_weekday(date::sys_days{elt}, i);
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
ymwdhms::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ymwdhm::to_sys_time(i) + std::chrono::seconds{second_[i]};
}

inline
cpp11::writable::list
ymwdhms::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp(), day_.sexp(), index_.sexp(), hour_.sexp(), minute_.sexp(), second_.sexp()});
  out.names() = {"year", "month", "day", "index", "hour", "minute", "second"};
  return out;
}

// ymwdhmss

template <typename Duration>
inline
ymwdhmss<Duration>::ymwdhmss(r_ssize size)
  : ymwdhms(size),
    subsecond_(size)
  {}

template <typename Duration>
inline
ymwdhmss<Duration>::ymwdhmss(const cpp11::integers& year,
                             const cpp11::integers& month,
                             const cpp11::integers& day,
                             const cpp11::integers& index,
                             const cpp11::integers& hour,
                             const cpp11::integers& minute,
                             const cpp11::integers& second,
                             const cpp11::integers& subsecond)
  : ymwdhms(year, month, day, index, hour, minute, second),
    subsecond_(subsecond)
  {}

template <typename Duration>
inline
std::ostringstream&
ymwdhmss<Duration>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ymwdhm::stream(os, i);
  os << ':';
  rclock::detail::stream_second_and_subsecond<Duration>(os, second_[i], subsecond_[i]);
  return os;
}

template <typename Duration>
inline
void
ymwdhmss<Duration>::assign_subsecond(const Duration& x, r_ssize i) NOEXCEPT
{
  subsecond_.assign(x.count(), i);
}

template <typename Duration>
inline
void
ymwdhmss<Duration>::assign_sys_time(const date::sys_time<Duration>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::seconds> second_point = date::floor<std::chrono::seconds>(x);
  const Duration subseconds = x - second_point;
  ymwdhms::assign_sys_time(second_point, i);
  assign_subsecond(subseconds, i);
}

template <typename Duration>
inline
void
ymwdhmss<Duration>::assign_na(r_ssize i) NOEXCEPT
{
  ymwdhms::assign_na(i);
  subsecond_.assign_na(i);
}

template <typename Duration>
inline
void
ymwdhmss<Duration>::resolve(r_ssize i, const enum invalid type)
{
  const date::year_month_weekday elt = to_year_month_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_month_weekday(detail::resolve_next_day_ymw(elt), i);
    break;
  case invalid::next: {
    assign_year_month_weekday(detail::resolve_next_day_ymw(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    assign_subsecond(rclock::detail::resolve_next_subsecond<Duration>(), i);
    break;
  }
  case invalid::previous_day:
    assign_year_month_weekday(detail::resolve_previous_day_ymw(elt), i);
    break;
  case invalid::previous: {
    assign_year_month_weekday(detail::resolve_previous_day_ymw(elt), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    assign_second(rclock::detail::resolve_previous_second(), i);
    assign_subsecond(rclock::detail::resolve_previous_subsecond<Duration>(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_month_weekday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    assign_year_month_weekday(date::sys_days{elt}, i);
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
ymwdhmss<Duration>::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ymwdhms::to_sys_time(i) + Duration{subsecond_[i]};
}

template <typename Duration>
inline
cpp11::writable::list
ymwdhmss<Duration>::to_list() const
{
  cpp11::writable::list out({year_.sexp(), month_.sexp(), day_.sexp(), index_.sexp(), hour_.sexp(), minute_.sexp(), second_.sexp(), subsecond_.sexp()});
  out.names() = {"year", "month", "day", "index", "hour", "minute", "second", "subsecond"};
  return out;
}

} // namespace weekday

} // namespace rclock

#endif
