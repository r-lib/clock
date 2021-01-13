#ifndef CLOCK_ISO_YEAR_WEEKNUM_WEEKDAY_H
#define CLOCK_ISO_YEAR_WEEKNUM_WEEKDAY_H

#include "clock.h"
#include "integers.h"
#include "enums.h"
#include "utils.h"
#include "stream.h"
#include "resolve.h"
#include "check.h"

namespace rclock {

namespace iso {

namespace detail {

inline
std::ostringstream&
stream_weeknum(std::ostringstream& os, int weeknum) NOEXCEPT
{
  os << 'W';
  os.fill('0');
  os.flags(std::ios::dec | std::ios::right);
  os.width(2);
  os << weeknum;
  return os;
}
inline
std::ostringstream&
stream_weekday(std::ostringstream& os, int weekday) NOEXCEPT
{
  os << weekday;
  return os;
}

inline
iso_week::year_weeknum_weekday
resolve_first_day_yww(const iso_week::year_weeknum_weekday& x) {
  // Only invalid on nonexistent week 53 day, rolls to first day of next iso year
  return (x.year() + iso_week::years{1}) / iso_week::weeknum{1} / iso_week::mon;
}
inline
iso_week::year_weeknum_weekday
resolve_last_day_yww(const iso_week::year_weeknum_weekday& x) {
  // Only invalid on nonexistent week 53 day, rolls to last day of current iso year
  return x.year() / iso_week::last / iso_week::sun;
}

inline
iso_week::year_weeknum
resolve_first_day_yw(const iso_week::year_weeknum& x) {
  // Only invalid on nonexistent week 53 day, rolls to first week of next iso year
  return (x.year() + iso_week::years{1}) / iso_week::weeknum{1};
}
inline
iso_week::year_weeknum
resolve_last_day_yw(const iso_week::year_weeknum& x) {
  // Only invalid on nonexistent week 53 day, rolls to last week of current iso year
  const iso_week::year_lastweek ylw{x.year()};
  return iso_week::year_weeknum{ylw.year(), ylw.weeknum()};
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
inline void check_range<component::weeknum>(const int& value, const char* arg) {
  check_range_iso_weeknum(value, arg);
}
template <>
inline void check_range<component::weekday>(const int& value, const char* arg) {
  check_range_iso_weekday(value, arg);
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
  CONSTCD11 r_ssize size() const NOEXCEPT;

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  template <typename Duration>
  void add(const Duration& x, r_ssize i) NOEXCEPT;

  void assign_year(const iso_week::year& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  template <component Component>
  void check_range(const int& value, const char* arg) const;

  iso_week::year to_year(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ywn : public y
{
protected:
  rclock::integers weeknum_;

public:
  ywn(r_ssize size);
  ywn(const cpp11::integers& year,
      const cpp11::integers& weeknum);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  template <typename Duration>
  void add(const Duration& x, r_ssize i) NOEXCEPT;

  void assign_weeknum(const iso_week::weeknum& x, r_ssize i) NOEXCEPT;
  void assign_year_weeknum(const iso_week::year_weeknum& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  iso_week::year_weeknum to_year_weeknum(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ywnwd : public ywn
{
protected:
  rclock::integers weekday_;

public:
  ywnwd(r_ssize size);
  ywnwd(const cpp11::integers& year,
        const cpp11::integers& weeknum,
        const cpp11::integers& weekday);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  void assign_weekday(const iso_week::weekday& x, r_ssize i) NOEXCEPT;
  void assign_year_weeknum_weekday(const iso_week::year_weeknum_weekday& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<date::days> to_sys_time(r_ssize i) const NOEXCEPT;
  iso_week::year_weeknum_weekday to_year_weeknum_weekday(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ywnwdh : public ywnwd
{
protected:
  rclock::integers hour_;

public:
  ywnwdh(r_ssize size);
  ywnwdh(const cpp11::integers& year,
         const cpp11::integers& weeknum,
         const cpp11::integers& weekday,
         const cpp11::integers& hour);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<std::chrono::hours> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ywnwdhm : public ywnwdh
{
protected:
  rclock::integers minute_;

public:
  ywnwdhm(r_ssize size);
  ywnwdhm(const cpp11::integers& year,
          const cpp11::integers& weeknum,
          const cpp11::integers& weekday,
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

class ywnwdhms : public ywnwdhm
{
protected:
  rclock::integers second_;

public:
  ywnwdhms(r_ssize size);
  ywnwdhms(const cpp11::integers& year,
           const cpp11::integers& weeknum,
           const cpp11::integers& weekday,
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
class ywnwdhmss : public ywnwdhms
{
protected:
  rclock::integers subsecond_;

public:
  ywnwdhmss(r_ssize size);
  ywnwdhmss(const cpp11::integers& year,
            const cpp11::integers& weeknum,
            const cpp11::integers& weekday,
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
  rclock::detail::stream_year(os, year_[i]);
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
y::assign_year(const iso_week::year& x, r_ssize i) NOEXCEPT
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
iso_week::year
y::to_year(r_ssize i) const NOEXCEPT
{
  return iso_week::year{year_[i]};
}

inline
cpp11::writable::list
y::to_list() const
{
  cpp11::writable::list out({year_.sexp()});
  out.names() = {"year"};
  return out;
}

// ywn

inline
ywn::ywn(r_ssize size)
  : y(size),
    weeknum_(size)
  {}


inline
ywn::ywn(const cpp11::integers& year,
         const cpp11::integers& weeknum)
  : y(year),
    weeknum_(weeknum)
  {}

inline
std::ostringstream&
ywn::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  y::stream(os, i);
  os << '-';
  detail::stream_weeknum(os, weeknum_[i]);
  return os;
}

inline
bool
ywn::ok(r_ssize i) const NOEXCEPT
{
  return to_year_weeknum(i).ok();
}

template <>
inline
void
ywn::add(const date::years& x, r_ssize i) NOEXCEPT
{
  assign_year(to_year(i) + x, i);
}

inline
void
ywn::assign_weeknum(const iso_week::weeknum& x, r_ssize i) NOEXCEPT
{
  weeknum_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

inline
void
ywn::assign_year_weeknum(const iso_week::year_weeknum& x, r_ssize i) NOEXCEPT
{
  assign_year(x.year(), i);
  assign_weeknum(x.weeknum(), i);
}

inline
void
ywn::assign_na(r_ssize i) NOEXCEPT
{
  y::assign_na(i);
  weeknum_.assign_na(i);
}

inline
void
ywn::resolve(r_ssize i, const enum invalid type)
{
  const iso_week::year_weeknum elt = to_year_weeknum(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::first_day:
  case invalid::first_time: {
    assign_year_weeknum(detail::resolve_first_day_yw(elt), i);
    break;
  }
  case invalid::last_day:
  case invalid::last_time: {
    assign_year_weeknum(detail::resolve_last_day_yw(elt), i);
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
iso_week::year_weeknum
ywn::to_year_weeknum(r_ssize i) const NOEXCEPT
{
  return iso_week::year{year_[i]} / static_cast<unsigned>(weeknum_[i]);
}

inline
cpp11::writable::list
ywn::to_list() const
{
  cpp11::writable::list out({year_.sexp(), weeknum_.sexp()});
  out.names() = {"year", "weeknum"};
  return out;
}

// ywnwd

inline
ywnwd::ywnwd(r_ssize size)
  : ywn(size),
    weekday_(size)
  {}

inline
ywnwd::ywnwd(const cpp11::integers& year,
             const cpp11::integers& weeknum,
             const cpp11::integers& weekday)
  : ywn(year, weeknum),
    weekday_(weekday)
  {}

inline
std::ostringstream&
ywnwd::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ywn::stream(os, i);
  os << '-';
  detail::stream_weekday(os, weekday_[i]);
  return os;
}

inline
bool
ywnwd::ok(r_ssize i) const NOEXCEPT
{
  return to_year_weeknum_weekday(i).ok();
}

inline
void
ywnwd::assign_weekday(const iso_week::weekday& x, r_ssize i) NOEXCEPT
{
  weekday_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

inline
void
ywnwd::assign_year_weeknum_weekday(const iso_week::year_weeknum_weekday& x, r_ssize i) NOEXCEPT
{
  assign_year(x.year(), i);
  assign_weeknum(x.weeknum(), i);
  assign_weekday(x.weekday(), i);
}

inline
void
ywnwd::assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT
{
  iso_week::year_weeknum_weekday ywnwd{x};
  assign_year_weeknum_weekday(ywnwd, i);
}

inline
void
ywnwd::assign_na(r_ssize i) NOEXCEPT
{
  ywn::assign_na(i);
  weekday_.assign_na(i);
}

inline
void
ywnwd::resolve(r_ssize i, const enum invalid type)
{
  const iso_week::year_weeknum_weekday elt = to_year_weeknum_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::first_day:
  case invalid::first_time: {
    assign_year_weeknum_weekday(detail::resolve_first_day_yww(elt), i);
    break;
  }
  case invalid::last_day:
  case invalid::last_time: {
    assign_year_weeknum_weekday(detail::resolve_last_day_yww(elt), i);
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
ywnwd::to_sys_time(r_ssize i) const NOEXCEPT
{
  return date::sys_time<date::days>{to_year_weeknum_weekday(i)};
}

inline
iso_week::year_weeknum_weekday
ywnwd::to_year_weeknum_weekday(r_ssize i) const NOEXCEPT
{
  return iso_week::year{year_[i]} /
    iso_week::weeknum{static_cast<unsigned>(weeknum_[i])} /
    iso_week::weekday{static_cast<unsigned>(weekday_[i])};
}

inline
cpp11::writable::list
ywnwd::to_list() const
{
  cpp11::writable::list out({year_.sexp(), weeknum_.sexp(), weekday_.sexp()});
  out.names() = {"year", "weeknum", "weekday"};
  return out;
}

// ywnwdh

inline
ywnwdh::ywnwdh(r_ssize size)
  : ywnwd(size),
    hour_(size)
  {}

inline
ywnwdh::ywnwdh(const cpp11::integers& year,
               const cpp11::integers& weeknum,
               const cpp11::integers& weekday,
               const cpp11::integers& hour)
  : ywnwd(year, weeknum, weekday),
    hour_(hour)
{}

inline
std::ostringstream&
ywnwdh::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ywnwd::stream(os, i);
  os << ' ';
  rclock::detail::stream_hour(os, hour_[i]);
  return os;
}

inline
void
ywnwdh::assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT
{
  hour_.assign(x.count(), i);
}

inline
void
ywnwdh::assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<date::days> day_point = date::floor<date::days>(x);
  const std::chrono::hours hours = x - day_point;
  ywnwd::assign_sys_time(day_point, i);
  assign_hour(hours, i);
}

inline
void
ywnwdh::assign_na(r_ssize i) NOEXCEPT
{
  ywnwd::assign_na(i);
  hour_.assign_na(i);
}

inline
void
ywnwdh::resolve(r_ssize i, const enum invalid type)
{
  const iso_week::year_weeknum_weekday elt = to_year_weeknum_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::first_day:
    assign_year_weeknum_weekday(detail::resolve_first_day_yww(elt), i);
    break;
  case invalid::first_time: {
    assign_year_weeknum_weekday(detail::resolve_first_day_yww(elt), i);
    assign_hour(rclock::detail::resolve_first_day_hour(), i);
    break;
  }
  case invalid::last_day:
    assign_year_weeknum_weekday(detail::resolve_last_day_yww(elt), i);
    break;
  case invalid::last_time: {
    assign_year_weeknum_weekday(detail::resolve_last_day_yww(elt), i);
    assign_hour(rclock::detail::resolve_last_day_hour(), i);
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
ywnwdh::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ywnwd::to_sys_time(i) + std::chrono::hours{hour_[i]};
}

inline
cpp11::writable::list
ywnwdh::to_list() const
{
  cpp11::writable::list out({year_.sexp(), weeknum_.sexp(), weekday_.sexp(), hour_.sexp()});
  out.names() = {"year", "weeknum", "weekday", "hour"};
  return out;
}

// ywnwdhm

inline
ywnwdhm::ywnwdhm(r_ssize size)
  : ywnwdh(size),
    minute_(size)
  {}

inline
ywnwdhm::ywnwdhm(const cpp11::integers& year,
                 const cpp11::integers& weeknum,
                 const cpp11::integers& weekday,
                 const cpp11::integers& hour,
                 const cpp11::integers& minute)
  : ywnwdh(year, weeknum, weekday, hour),
    minute_(minute)
  {}

inline
std::ostringstream&
ywnwdhm::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ywnwdh::stream(os, i);
  os << ':';
  rclock::detail::stream_minute(os, minute_[i]);
  return os;
}

inline
void
ywnwdhm::assign_minute(const std::chrono::minutes& x, r_ssize i) NOEXCEPT
{
  minute_.assign(x.count(), i);
}

inline
void
ywnwdhm::assign_sys_time(const date::sys_time<std::chrono::minutes>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::hours> hour_point = date::floor<std::chrono::hours>(x);
  const std::chrono::minutes minutes = x - hour_point;
  ywnwdh::assign_sys_time(hour_point, i);
  assign_minute(minutes, i);
}

inline
void
ywnwdhm::assign_na(r_ssize i) NOEXCEPT
{
  ywnwdh::assign_na(i);
  minute_.assign_na(i);
}

inline
void
ywnwdhm::resolve(r_ssize i, const enum invalid type)
{
  const iso_week::year_weeknum_weekday elt = to_year_weeknum_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::first_day:
    assign_year_weeknum_weekday(detail::resolve_first_day_yww(elt), i);
    break;
  case invalid::first_time: {
    assign_year_weeknum_weekday(detail::resolve_first_day_yww(elt), i);
    assign_hour(rclock::detail::resolve_first_day_hour(), i);
    assign_minute(rclock::detail::resolve_first_day_minute(), i);
    break;
  }
  case invalid::last_day:
    assign_year_weeknum_weekday(detail::resolve_last_day_yww(elt), i);
    break;
  case invalid::last_time: {
    assign_year_weeknum_weekday(detail::resolve_last_day_yww(elt), i);
    assign_hour(rclock::detail::resolve_last_day_hour(), i);
    assign_minute(rclock::detail::resolve_last_day_minute(), i);
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
ywnwdhm::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ywnwdh::to_sys_time(i) + std::chrono::minutes{minute_[i]};
}

inline
cpp11::writable::list
ywnwdhm::to_list() const
{
  cpp11::writable::list out({year_.sexp(), weeknum_.sexp(), weekday_.sexp(), hour_.sexp(), minute_.sexp()});
  out.names() = {"year", "weeknum", "weekday", "hour", "minute"};
  return out;
}

// ywnwdhms

inline
ywnwdhms::ywnwdhms(r_ssize size)
  : ywnwdhm(size),
    second_(size)
  {}

inline
ywnwdhms::ywnwdhms(const cpp11::integers& year,
                   const cpp11::integers& weeknum,
                   const cpp11::integers& weekday,
                   const cpp11::integers& hour,
                   const cpp11::integers& minute,
                   const cpp11::integers& second)
  : ywnwdhm(year, weeknum, weekday, hour, minute),
    second_(second)
  {}

inline
std::ostringstream&
ywnwdhms::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ywnwdhm::stream(os, i);
  os << ':';
  rclock::detail::stream_second(os, second_[i]);
  return os;
}

inline
void
ywnwdhms::assign_second(const std::chrono::seconds& x, r_ssize i) NOEXCEPT
{
  second_.assign(x.count(), i);
}

inline
void
ywnwdhms::assign_sys_time(const date::sys_time<std::chrono::seconds>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::minutes> minute_point = date::floor<std::chrono::minutes>(x);
  const std::chrono::seconds seconds = x - minute_point;
  ywnwdhm::assign_sys_time(minute_point, i);
  assign_second(seconds, i);
}

inline
void
ywnwdhms::assign_na(r_ssize i) NOEXCEPT
{
  ywnwdhm::assign_na(i);
  second_.assign_na(i);
}

inline
void
ywnwdhms::resolve(r_ssize i, const enum invalid type)
{
  const iso_week::year_weeknum_weekday elt = to_year_weeknum_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::first_day:
    assign_year_weeknum_weekday(detail::resolve_first_day_yww(elt), i);
    break;
  case invalid::first_time: {
    assign_year_weeknum_weekday(detail::resolve_first_day_yww(elt), i);
    assign_hour(rclock::detail::resolve_first_day_hour(), i);
    assign_minute(rclock::detail::resolve_first_day_minute(), i);
    assign_second(rclock::detail::resolve_first_day_second(), i);
    break;
  }
  case invalid::last_day:
    assign_year_weeknum_weekday(detail::resolve_last_day_yww(elt), i);
    break;
  case invalid::last_time: {
    assign_year_weeknum_weekday(detail::resolve_last_day_yww(elt), i);
    assign_hour(rclock::detail::resolve_last_day_hour(), i);
    assign_minute(rclock::detail::resolve_last_day_minute(), i);
    assign_second(rclock::detail::resolve_last_day_second(), i);
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
ywnwdhms::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ywnwdhm::to_sys_time(i) + std::chrono::seconds{second_[i]};
}

inline
cpp11::writable::list
ywnwdhms::to_list() const
{
  cpp11::writable::list out({year_.sexp(), weeknum_.sexp(), weekday_.sexp(), hour_.sexp(), minute_.sexp(), second_.sexp()});
  out.names() = {"year", "weeknum", "weekday", "hour", "minute", "second"};
  return out;
}

// ywnwdhmss

template <typename Duration>
inline
ywnwdhmss<Duration>::ywnwdhmss(r_ssize size)
  : ywnwdhms(size),
    subsecond_(size)
  {}

template <typename Duration>
inline
ywnwdhmss<Duration>::ywnwdhmss(const cpp11::integers& year,
                               const cpp11::integers& weeknum,
                               const cpp11::integers& weekday,
                               const cpp11::integers& hour,
                               const cpp11::integers& minute,
                               const cpp11::integers& second,
                               const cpp11::integers& subsecond)
  : ywnwdhms(year, weeknum, weekday, hour, minute, second),
    subsecond_(subsecond)
  {}

template <typename Duration>
inline
std::ostringstream&
ywnwdhmss<Duration>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ywnwdhm::stream(os, i);
  os << ':';
  rclock::detail::stream_second_and_subsecond<Duration>(os, second_[i], subsecond_[i]);
  return os;
}

template <typename Duration>
inline
void
ywnwdhmss<Duration>::assign_subsecond(const Duration& x, r_ssize i) NOEXCEPT
{
  subsecond_.assign(x.count(), i);
}

template <typename Duration>
inline
void
ywnwdhmss<Duration>::assign_sys_time(const date::sys_time<Duration>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::seconds> second_point = date::floor<std::chrono::seconds>(x);
  const Duration subseconds = x - second_point;
  ywnwdhms::assign_sys_time(second_point, i);
  assign_subsecond(subseconds, i);
}

template <typename Duration>
inline
void
ywnwdhmss<Duration>::assign_na(r_ssize i) NOEXCEPT
{
  ywnwdhms::assign_na(i);
  subsecond_.assign_na(i);
}

template <typename Duration>
inline
void
ywnwdhmss<Duration>::resolve(r_ssize i, const enum invalid type)
{
  const iso_week::year_weeknum_weekday elt = to_year_weeknum_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::first_day:
    assign_year_weeknum_weekday(detail::resolve_first_day_yww(elt), i);
    break;
  case invalid::first_time: {
    assign_year_weeknum_weekday(detail::resolve_first_day_yww(elt), i);
    assign_hour(rclock::detail::resolve_first_day_hour(), i);
    assign_minute(rclock::detail::resolve_first_day_minute(), i);
    assign_second(rclock::detail::resolve_first_day_second(), i);
    assign_subsecond(rclock::detail::resolve_first_day_subsecond<Duration>(), i);
    break;
  }
  case invalid::last_day:
    assign_year_weeknum_weekday(detail::resolve_last_day_yww(elt), i);
    break;
  case invalid::last_time: {
    assign_year_weeknum_weekday(detail::resolve_last_day_yww(elt), i);
    assign_hour(rclock::detail::resolve_last_day_hour(), i);
    assign_minute(rclock::detail::resolve_last_day_minute(), i);
    assign_second(rclock::detail::resolve_last_day_second(), i);
    assign_subsecond(rclock::detail::resolve_last_day_subsecond<Duration>(), i);
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
ywnwdhmss<Duration>::to_sys_time(r_ssize i) const NOEXCEPT
{
  return ywnwdhms::to_sys_time(i) + Duration{subsecond_[i]};
}

template <typename Duration>
inline
cpp11::writable::list
ywnwdhmss<Duration>::to_list() const
{
  cpp11::writable::list out({year_.sexp(), weeknum_.sexp(), weekday_.sexp(), hour_.sexp(), minute_.sexp(), second_.sexp(), subsecond_.sexp()});
  out.names() = {"year", "weeknum", "weekday", "hour", "minute", "second", "subsecond"};
  return out;
}

} // namespace iso

} // namespace rclock

#endif
