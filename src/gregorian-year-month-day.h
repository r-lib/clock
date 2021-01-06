#ifndef CLOCK_GREGORIAN_YEAR_MONTH_DAY_H
#define CLOCK_GREGORIAN_YEAR_MONTH_DAY_H

#include "clock.h"
#include "integers.h"
#include "enums.h"
#include "utils.h"

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

inline
date::year_month_day
resolve_first_day_ymd(const date::year_month_day& x) {
  return ((x.year() / x.month()) + date::months(1)) / date::day(1);
}
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
date::year_month_day
resolve_last_day_ymd(const date::year_month_day& x) {
  return x.year() / x.month() / date::last;
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
  clock_abort("Invalid day found at location %i.", (int) i + 1);
}

} // namespace detail

namespace gregorian {

class y
{
protected:
  rclock::integers year_;

public:
  y(const cpp11::integers& year);

  bool is_na(r_ssize i) const NOEXCEPT;
  CONSTCD11 r_ssize size() const NOEXCEPT;

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  template <typename Duration>
  void add(const Duration& x, r_ssize i) NOEXCEPT;

  void assign_year(const date::year& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::year to_year(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ym : public y
{
protected:
  rclock::integers month_;

public:
  ym(const cpp11::integers& year,
     const cpp11::integers& month);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  template <typename Duration>
  void add(const Duration& x, r_ssize i) NOEXCEPT;

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
  ymd(const cpp11::integers& year,
      const cpp11::integers& month,
      const cpp11::integers& day);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  void assign_day(const date::day& x, r_ssize i) NOEXCEPT;
  void assign_year_month_day(const date::year_month_day& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::year_month_day to_year_month_day(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ymdh : public ymd
{
protected:
  rclock::integers hour_;

public:
  ymdh(const cpp11::integers& year,
       const cpp11::integers& month,
       const cpp11::integers& day,
       const cpp11::integers& hour);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  cpp11::writable::list to_list() const;
};

class ymdhm : public ymdh
{
protected:
  rclock::integers minute_;

public:
  ymdhm(const cpp11::integers& year,
        const cpp11::integers& month,
        const cpp11::integers& day,
        const cpp11::integers& hour,
        const cpp11::integers& minute);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_minute(const std::chrono::minutes& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  cpp11::writable::list to_list() const;
};

class ymdhms : public ymdhm
{
protected:
  rclock::integers second_;

public:
  ymdhms(const cpp11::integers& year,
         const cpp11::integers& month,
         const cpp11::integers& day,
         const cpp11::integers& hour,
         const cpp11::integers& minute,
         const cpp11::integers& second);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_second(const std::chrono::seconds& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  cpp11::writable::list to_list() const;
};

template <typename Duration>
class ymdhmss : public ymdhms
{
protected:
  rclock::integers subsecond_;

public:
  ymdhmss(const cpp11::integers& year,
          const cpp11::integers& month,
          const cpp11::integers& day,
          const cpp11::integers& hour,
          const cpp11::integers& minute,
          const cpp11::integers& second,
          const cpp11::integers& subsecond);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_subsecond(const Duration& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  cpp11::writable::list to_list() const;
};

// Implementation

// y

inline y::y(const cpp11::integers& year)
  : year_(rclock::integers(year))
  {}

inline
bool
y::is_na(r_ssize i) const NOEXCEPT
{
  return year_.is_na(i);
}

CONSTCD11
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
  detail::stream_year(os, year_[i]);
  return os;
}

inline
bool
y::ok(r_ssize i) const NOEXCEPT
{
  return true;
}

template <>
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
ym::ym(const cpp11::integers& year,
       const cpp11::integers& month)
  : y(year),
    month_(month)
  {}

inline
std::ostringstream&
ym::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  detail::stream_year(os, year_[i]);
  os << '-';
  detail::stream_month(os, month_[i]);
  return os;
}

inline
bool
ym::ok(r_ssize i) const NOEXCEPT
{
  return true;
}

template <>
inline
void
ym::add(const date::years& x, r_ssize i) NOEXCEPT
{
  assign_year(to_year(i) + x, i);
}

template <>
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
  year_.assign_na(i);
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
  detail::stream_year(os, year_[i]);
  os << '-';
  detail::stream_month(os, month_[i]);
  os << '-';
  detail::stream_day(os, day_[i]);
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
ymd::assign_na(r_ssize i) NOEXCEPT
{
  year_.assign_na(i);
  month_.assign_na(i);
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
  case invalid::first_day:
  case invalid::first_time: {
    assign_year_month_day(detail::resolve_first_day_ymd(elt), i);
    break;
  }
  case invalid::last_day:
  case invalid::last_time: {
    assign_day(detail::resolve_last_day_ymd(elt).day(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    detail::resolve_error(i);
  }
  }
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
  detail::stream_year(os, year_[i]);
  os << '-';
  detail::stream_month(os, month_[i]);
  os << '-';
  detail::stream_day(os, day_[i]);
  os << ' ';
  detail::stream_hour(os, hour_[i]);
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
ymdh::assign_na(r_ssize i) NOEXCEPT
{
  year_.assign_na(i);
  month_.assign_na(i);
  day_.assign_na(i);
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
  case invalid::first_day:
  case invalid::first_time: {
    assign_year_month_day(detail::resolve_first_day_ymd(elt), i);
    assign_hour(detail::resolve_first_day_hour(), i);
    break;
  }
  case invalid::last_day:
  case invalid::last_time: {
    assign_day(detail::resolve_last_day_ymd(elt).day(), i);
    assign_hour(detail::resolve_last_day_hour(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    detail::resolve_error(i);
  }
  }
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
  detail::stream_year(os, year_[i]);
  os << '-';
  detail::stream_month(os, month_[i]);
  os << '-';
  detail::stream_day(os, day_[i]);
  os << ' ';
  detail::stream_hour(os, hour_[i]);
  os << ':';
  detail::stream_minute(os, minute_[i]);
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
ymdhm::assign_na(r_ssize i) NOEXCEPT
{
  year_.assign_na(i);
  month_.assign_na(i);
  day_.assign_na(i);
  hour_.assign_na(i);
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
  case invalid::first_day:
  case invalid::first_time: {
    assign_year_month_day(detail::resolve_first_day_ymd(elt), i);
    assign_hour(detail::resolve_first_day_hour(), i);
    assign_minute(detail::resolve_first_day_minute(), i);
    break;
  }
  case invalid::last_day:
  case invalid::last_time: {
    assign_day(detail::resolve_last_day_ymd(elt).day(), i);
    assign_hour(detail::resolve_last_day_hour(), i);
    assign_minute(detail::resolve_last_day_minute(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    detail::resolve_error(i);
  }
  }
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
  detail::stream_year(os, year_[i]);
  os << '-';
  detail::stream_month(os, month_[i]);
  os << '-';
  detail::stream_day(os, day_[i]);
  os << ' ';
  detail::stream_hour(os, hour_[i]);
  os << ':';
  detail::stream_minute(os, minute_[i]);
  os << ':';
  detail::stream_second(os, second_[i]);
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
ymdhms::assign_na(r_ssize i) NOEXCEPT
{
  year_.assign_na(i);
  month_.assign_na(i);
  day_.assign_na(i);
  hour_.assign_na(i);
  minute_.assign_na(i);
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
  case invalid::first_day:
  case invalid::first_time: {
    assign_year_month_day(detail::resolve_first_day_ymd(elt), i);
    assign_hour(detail::resolve_first_day_hour(), i);
    assign_minute(detail::resolve_first_day_minute(), i);
    assign_second(detail::resolve_first_day_second(), i);
    break;
  }
  case invalid::last_day:
  case invalid::last_time: {
    assign_day(detail::resolve_last_day_ymd(elt).day(), i);
    assign_hour(detail::resolve_last_day_hour(), i);
    assign_minute(detail::resolve_last_day_minute(), i);
    assign_second(detail::resolve_last_day_second(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    detail::resolve_error(i);
  }
  }
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
  detail::stream_year(os, year_[i]);
  os << '-';
  detail::stream_month(os, month_[i]);
  os << '-';
  detail::stream_day(os, day_[i]);
  os << ' ';
  detail::stream_hour(os, hour_[i]);
  os << ':';
  detail::stream_minute(os, minute_[i]);
  os << ':';
  detail::stream_second_and_subsecond<Duration>(os, second_[i], subsecond_[i]);
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
ymdhmss<Duration>::assign_na(r_ssize i) NOEXCEPT
{
  year_.assign_na(i);
  month_.assign_na(i);
  day_.assign_na(i);
  hour_.assign_na(i);
  minute_.assign_na(i);
  second_.assign_na(i);
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
  case invalid::first_day:
  case invalid::first_time: {
    assign_year_month_day(detail::resolve_first_day_ymd(elt), i);
    assign_hour(detail::resolve_first_day_hour(), i);
    assign_minute(detail::resolve_first_day_minute(), i);
    assign_second(detail::resolve_first_day_second(), i);
    assign_subsecond(detail::resolve_first_day_subsecond<Duration>(), i);
    break;
  }
  case invalid::last_day:
  case invalid::last_time: {
    assign_day(detail::resolve_last_day_ymd(elt).day(), i);
    assign_hour(detail::resolve_last_day_hour(), i);
    assign_minute(detail::resolve_last_day_minute(), i);
    assign_second(detail::resolve_last_day_second(), i);
    assign_subsecond(detail::resolve_last_day_subsecond<Duration>(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    detail::resolve_error(i);
  }
  }
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
