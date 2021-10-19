#ifndef CLOCK_QUARTERLY_YEAR_QUARTER_DAY_H
#define CLOCK_QUARTERLY_YEAR_QUARTER_DAY_H

#include "clock.h"
#include "integers.h"
#include "enums.h"
#include "utils.h"
#include "stream.h"
#include "resolve.h"
#include "check.h"

namespace rclock {

namespace rquarterly {

namespace detail {

inline
std::ostringstream&
stream_quarter(std::ostringstream& os, int quarter) NOEXCEPT
{
  os << quarterly::quarternum{static_cast<unsigned>(quarter)};
  return os;
}
inline
std::ostringstream&
stream_day(std::ostringstream& os, int day) NOEXCEPT
{
  os.fill('0');
  os.flags(std::ios::dec | std::ios::right);
  os.width(2);
  os << day;
  return os;
}

template <quarterly::start S>
inline
quarterly::year_quarternum_quarterday<S>
resolve_next_day_yqd(const quarterly::year_quarternum_quarterday<S>& x) {
  return ((x.year() / x.quarternum()) + quarterly::quarters(1)) / quarterly::quarterday{1u};
}
template <quarterly::start S>
inline
quarterly::year_quarternum_quarterday<S>
resolve_previous_day_yqd(const quarterly::year_quarternum_quarterday<S>& x) {
  return x.year() / x.quarternum() / quarterly::last;
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
inline void check_range<component::quarter>(const int& value, const char* arg) {
  check_range_quarterly_quarter(value, arg);
}
template <>
inline void check_range<component::day>(const int& value, const char* arg) {
  check_range_quarterly_day(value, arg);
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

template <quarterly::start S>
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

  void assign_year(const quarterly::year<S>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  template <component Component>
  void check_range(const int& value, const char* arg) const;

  void resolve(r_ssize i, const enum invalid type);

  quarterly::year<S> to_year(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

template <quarterly::start S>
class yqn : public y<S>
{
protected:
  rclock::integers quarter_;

public:
  yqn(r_ssize size);
  yqn(const cpp11::integers& year,
      const cpp11::integers& quarter);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  void add(const quarterly::quarters& x, r_ssize i) NOEXCEPT;

  void assign_quarternum(const quarterly::quarternum& x, r_ssize i) NOEXCEPT;
  void assign_year_quarternum(const quarterly::year_quarternum<S>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  quarterly::year_quarternum<S> to_year_quarternum(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

template <quarterly::start S>
class yqnqd : public yqn<S>
{
protected:
  rclock::integers day_;

public:
  yqnqd(r_ssize size);
  yqnqd(const cpp11::integers& year,
        const cpp11::integers& quarter,
        const cpp11::integers& day);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  bool ok(r_ssize i) const NOEXCEPT;

  void assign_quarterday(const quarterly::quarterday& x, r_ssize i) NOEXCEPT;
  void assign_year_quarternum_quarterday(const quarterly::year_quarternum_quarterday<S>& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type);

  date::sys_time<date::days> to_sys_time(r_ssize i) const NOEXCEPT;
  quarterly::year_quarternum_quarterday<S> to_year_quarternum_quarterday(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

template <quarterly::start S>
class yqnqdh : public yqnqd<S>
{
protected:
  rclock::integers hour_;

public:
  yqnqdh(r_ssize size);
  yqnqdh(const cpp11::integers& year,
         const cpp11::integers& quarter,
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

template <quarterly::start S>
class yqnqdhm : public yqnqdh<S>
{
protected:
  rclock::integers minute_;

public:
  yqnqdhm(r_ssize size);
  yqnqdhm(const cpp11::integers& year,
          const cpp11::integers& quarter,
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

template <quarterly::start S>
class yqnqdhms : public yqnqdhm<S>
{
protected:
  rclock::integers second_;

public:
  yqnqdhms(r_ssize size);
  yqnqdhms(const cpp11::integers& year,
           const cpp11::integers& quarter,
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

template <typename Duration, quarterly::start S>
class yqnqdhmss : public yqnqdhms<S>
{
protected:
  rclock::integers subsecond_;

public:
  yqnqdhmss(r_ssize size);
  yqnqdhmss(const cpp11::integers& year,
            const cpp11::integers& quarter,
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
};

// Implementation

// y

template <quarterly::start S>
inline
y<S>::y(r_ssize size)
  : year_(size)
  {}

template <quarterly::start S>
inline
y<S>::y(const cpp11::integers& year)
  : year_(rclock::integers(year))
  {}

template <quarterly::start S>
inline
bool
y<S>::is_na(r_ssize i) const NOEXCEPT
{
  return year_.is_na(i);
}

template <quarterly::start S>
inline
r_ssize
y<S>::size() const NOEXCEPT
{
  return year_.size();
}

template <quarterly::start S>
inline
std::ostringstream&
y<S>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  rclock::detail::stream_year(os, year_[i]);
  return os;
}

template <quarterly::start S>
inline
bool
y<S>::ok(r_ssize i) const NOEXCEPT
{
  return true;
}

template <quarterly::start S>
inline
void
y<S>::add(const date::years& x, r_ssize i) NOEXCEPT
{
  assign_year(to_year(i) + x, i);
}

template <quarterly::start S>
inline
void
y<S>::assign_year(const quarterly::year<S>& x, r_ssize i) NOEXCEPT
{
  year_.assign(static_cast<int>(x), i);
}

template <quarterly::start S>
inline
void
y<S>::assign_na(r_ssize i) NOEXCEPT
{
  year_.assign_na(i);
}

template <quarterly::start S>
inline
void
y<S>::resolve(r_ssize i, const enum invalid type)
{
  // Never invalid
}

template <quarterly::start S>
template <component Component>
inline
void
y<S>::check_range(const int& value, const char* arg) const
{
  detail::check_range<Component>(value, arg);
}

template <quarterly::start S>
inline
quarterly::year<S>
y<S>::to_year(r_ssize i) const NOEXCEPT
{
  return quarterly::year<S>{year_[i]};
}

template <quarterly::start S>
inline
cpp11::writable::list
y<S>::to_list() const
{
  cpp11::writable::list out({year_.sexp()});
  out.names() = {"year"};
  return out;
}

// yqn

template <quarterly::start S>
inline
yqn<S>::yqn(r_ssize size)
  : y<S>(size),
    quarter_(size)
  {}

template <quarterly::start S>
inline
yqn<S>::yqn(const cpp11::integers& year,
            const cpp11::integers& quarter)
  : y<S>(year),
    quarter_(quarter)
  {}

template <quarterly::start S>
inline
std::ostringstream&
yqn<S>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  y<S>::stream(os, i);
  os << '-';
  detail::stream_quarter(os, quarter_[i]);
  return os;
}

template <quarterly::start S>
inline
bool
yqn<S>::ok(r_ssize i) const NOEXCEPT
{
  return true;
}

template <quarterly::start S>
inline
void
yqn<S>::add(const quarterly::quarters& x, r_ssize i) NOEXCEPT
{
  assign_year_quarternum(to_year_quarternum(i) + x, i);
}

template <quarterly::start S>
inline
void
yqn<S>::assign_quarternum(const quarterly::quarternum& x, r_ssize i) NOEXCEPT
{
  quarter_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

template <quarterly::start S>
inline
void
yqn<S>::assign_year_quarternum(const quarterly::year_quarternum<S>& x, r_ssize i) NOEXCEPT
{
  y<S>::assign_year(x.year(), i);
  assign_quarternum(x.quarternum(), i);
}

template <quarterly::start S>
inline
void
yqn<S>::assign_na(r_ssize i) NOEXCEPT
{
  y<S>::assign_na(i);
  quarter_.assign_na(i);
}

template <quarterly::start S>
inline
void
yqn<S>::resolve(r_ssize i, const enum invalid type)
{
  // Never invalid
}

template <quarterly::start S>
inline
quarterly::year_quarternum<S>
yqn<S>::to_year_quarternum(r_ssize i) const NOEXCEPT
{
  return quarterly::year<S>{y<S>::year_[i]} / static_cast<unsigned>(quarter_[i]);
}

template <quarterly::start S>
inline
cpp11::writable::list
yqn<S>::to_list() const
{
  cpp11::writable::list out({y<S>::year_.sexp(), quarter_.sexp()});
  out.names() = {"year", "quarter"};
  return out;
}

// yqnqd

template <quarterly::start S>
inline
yqnqd<S>::yqnqd(r_ssize size)
  : yqn<S>(size),
    day_(size)
  {}

template <quarterly::start S>
inline
yqnqd<S>::yqnqd(const cpp11::integers& year,
                const cpp11::integers& quarter,
                const cpp11::integers& day)
  : yqn<S>(year, quarter),
    day_(day)
  {}

template <quarterly::start S>
inline
std::ostringstream&
yqnqd<S>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yqn<S>::stream(os, i);
  os << '-';
  detail::stream_day(os, day_[i]);
  return os;
}

template <quarterly::start S>
inline
bool
yqnqd<S>::ok(r_ssize i) const NOEXCEPT
{
  return to_year_quarternum_quarterday(i).ok();
}

template <quarterly::start S>
inline
void
yqnqd<S>::assign_quarterday(const quarterly::quarterday& x, r_ssize i) NOEXCEPT
{
  day_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

template <quarterly::start S>
inline
void
yqnqd<S>::assign_year_quarternum_quarterday(const quarterly::year_quarternum_quarterday<S>& x, r_ssize i) NOEXCEPT
{
  yqn<S>::assign_year(x.year(), i);
  yqn<S>::assign_quarternum(x.quarternum(), i);
  assign_quarterday(x.quarterday(), i);
}

template <quarterly::start S>
inline
void
yqnqd<S>::assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT
{
  quarterly::year_quarternum_quarterday<S> yqnqd{x};
  assign_year_quarternum_quarterday(yqnqd, i);
}

template <quarterly::start S>
inline
void
yqnqd<S>::assign_na(r_ssize i) NOEXCEPT
{
  yqn<S>::assign_na(i);
  day_.assign_na(i);
}

template <quarterly::start S>
inline
void
yqnqd<S>::resolve(r_ssize i, const enum invalid type)
{
  const quarterly::year_quarternum_quarterday<S> elt = to_year_quarternum_quarterday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
  case invalid::next: {
    assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    break;
  }
  case invalid::previous_day:
  case invalid::previous: {
    assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    break;
  }
  case invalid::overflow_day:
  case invalid::overflow: {
    assign_year_quarternum_quarterday(date::sys_days{elt}, i);
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

template <quarterly::start S>
inline
date::sys_time<date::days>
yqnqd<S>::to_sys_time(r_ssize i) const NOEXCEPT
{
  return date::sys_time<date::days>{to_year_quarternum_quarterday(i)};
}

template <quarterly::start S>
inline
quarterly::year_quarternum_quarterday<S>
yqnqd<S>::to_year_quarternum_quarterday(r_ssize i) const NOEXCEPT
{
  return yqn<S>::to_year_quarternum(i) /
    static_cast<unsigned>(day_[i]);
}

template <quarterly::start S>
inline
cpp11::writable::list
yqnqd<S>::to_list() const
{
  cpp11::writable::list out({yqn<S>::year_.sexp(), yqn<S>::quarter_.sexp(), day_.sexp()});
  out.names() = {"year", "quarter", "day"};
  return out;
}

// yqnqdh

template <quarterly::start S>
inline
yqnqdh<S>::yqnqdh(r_ssize size)
  : yqnqd<S>(size),
    hour_(size)
  {}

template <quarterly::start S>
inline
yqnqdh<S>::yqnqdh(const cpp11::integers& year,
                  const cpp11::integers& quarter,
                  const cpp11::integers& day,
                  const cpp11::integers& hour)
  : yqnqd<S>(year, quarter, day),
    hour_(hour)
  {}

template <quarterly::start S>
inline
std::ostringstream&
yqnqdh<S>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yqnqd<S>::stream(os, i);
  os << 'T';
  rclock::detail::stream_hour(os, hour_[i]);
  return os;
}

template <quarterly::start S>
inline
void
yqnqdh<S>::assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT
{
  hour_.assign(x.count(), i);
}

template <quarterly::start S>
inline
void
yqnqdh<S>::assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<date::days> day_point = date::floor<date::days>(x);
  const std::chrono::hours hours = x - day_point;
  yqnqd<S>::assign_sys_time(day_point, i);
  assign_hour(hours, i);
}

template <quarterly::start S>
inline
void
yqnqdh<S>::assign_na(r_ssize i) NOEXCEPT
{
  yqnqd<S>::assign_na(i);
  hour_.assign_na(i);
}

template <quarterly::start S>
inline
void
yqnqdh<S>::resolve(r_ssize i, const enum invalid type)
{
  const quarterly::year_quarternum_quarterday<S> elt = yqnqd<S>::to_year_quarternum_quarterday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    yqnqd<S>::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    break;
  case invalid::next: {
    yqnqd<S>::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    break;
  }
  case invalid::previous_day:
    yqnqd<S>::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    break;
  case invalid::previous: {
    yqnqd<S>::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    break;
  }
  case invalid::overflow_day:
    yqnqd<S>::assign_year_quarternum_quarterday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    yqnqd<S>::assign_year_quarternum_quarterday(date::sys_days{elt}, i);
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

template <quarterly::start S>
inline
date::sys_time<std::chrono::hours>
yqnqdh<S>::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yqnqd<S>::to_sys_time(i) + std::chrono::hours{hour_[i]};
}

template <quarterly::start S>
inline
cpp11::writable::list
yqnqdh<S>::to_list() const
{
  cpp11::writable::list out({yqnqd<S>::year_.sexp(), yqnqd<S>::quarter_.sexp(), yqnqd<S>::day_.sexp(), hour_.sexp()});
  out.names() = {"year", "quarter", "day", "hour"};
  return out;
}

// yqnqdhm

template <quarterly::start S>
inline
yqnqdhm<S>::yqnqdhm(r_ssize size)
  : yqnqdh<S>(size),
    minute_(size)
  {}

template <quarterly::start S>
inline
yqnqdhm<S>::yqnqdhm(const cpp11::integers& year,
                    const cpp11::integers& quarter,
                    const cpp11::integers& day,
                    const cpp11::integers& hour,
                    const cpp11::integers& minute)
  : yqnqdh<S>(year, quarter, day, hour),
    minute_(minute)
  {}

template <quarterly::start S>
inline
std::ostringstream&
yqnqdhm<S>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yqnqdh<S>::stream(os, i);
  os << ':';
  rclock::detail::stream_minute(os, minute_[i]);
  return os;
}

template <quarterly::start S>
inline
void
yqnqdhm<S>::assign_minute(const std::chrono::minutes& x, r_ssize i) NOEXCEPT
{
  minute_.assign(x.count(), i);
}

template <quarterly::start S>
inline
void
yqnqdhm<S>::assign_sys_time(const date::sys_time<std::chrono::minutes>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::hours> hour_point = date::floor<std::chrono::hours>(x);
  const std::chrono::minutes minutes = x - hour_point;
  yqnqdh<S>::assign_sys_time(hour_point, i);
  assign_minute(minutes, i);
}

template <quarterly::start S>
inline
void
yqnqdhm<S>::assign_na(r_ssize i) NOEXCEPT
{
  yqnqdh<S>::assign_na(i);
  minute_.assign_na(i);
}

template <quarterly::start S>
inline
void
yqnqdhm<S>::resolve(r_ssize i, const enum invalid type)
{
  const quarterly::year_quarternum_quarterday<S> elt = yqnqdh<S>::to_year_quarternum_quarterday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    yqnqdh<S>::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    break;
  case invalid::next: {
    yqnqdh<S>::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    yqnqdh<S>::assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    break;
  }
  case invalid::previous_day:
    yqnqdh<S>::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    break;
  case invalid::previous: {
    yqnqdh<S>::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    yqnqdh<S>::assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    break;
  }
  case invalid::overflow_day:
    yqnqdh<S>::assign_year_quarternum_quarterday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    yqnqdh<S>::assign_year_quarternum_quarterday(date::sys_days{elt}, i);
    yqnqdh<S>::assign_hour(rclock::detail::resolve_next_hour(), i);
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

template <quarterly::start S>
inline
date::sys_time<std::chrono::minutes>
yqnqdhm<S>::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yqnqdh<S>::to_sys_time(i) + std::chrono::minutes{minute_[i]};
}

template <quarterly::start S>
inline
cpp11::writable::list
yqnqdhm<S>::to_list() const
{
  cpp11::writable::list out({yqnqdh<S>::year_.sexp(), yqnqdh<S>::quarter_.sexp(), yqnqdh<S>::day_.sexp(), yqnqdh<S>::hour_.sexp(), minute_.sexp()});
  out.names() = {"year", "quarter", "day", "hour", "minute"};
  return out;
}

// yqnqdhms

template <quarterly::start S>
inline
yqnqdhms<S>::yqnqdhms(r_ssize size)
  : yqnqdhm<S>(size),
    second_(size)
  {}

template <quarterly::start S>
inline
yqnqdhms<S>::yqnqdhms(const cpp11::integers& year,
                      const cpp11::integers& quarter,
                      const cpp11::integers& day,
                      const cpp11::integers& hour,
                      const cpp11::integers& minute,
                      const cpp11::integers& second)
  : yqnqdhm<S>(year, quarter, day, hour, minute),
    second_(second)
  {}

template <quarterly::start S>
inline
std::ostringstream&
yqnqdhms<S>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yqnqdhm<S>::stream(os, i);
  os << ':';
  rclock::detail::stream_second(os, second_[i]);
  return os;
}

template <quarterly::start S>
inline
void
yqnqdhms<S>::assign_second(const std::chrono::seconds& x, r_ssize i) NOEXCEPT
{
  second_.assign(x.count(), i);
}

template <quarterly::start S>
inline
void
yqnqdhms<S>::assign_sys_time(const date::sys_time<std::chrono::seconds>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::minutes> minute_point = date::floor<std::chrono::minutes>(x);
  const std::chrono::seconds seconds = x - minute_point;
  yqnqdhm<S>::assign_sys_time(minute_point, i);
  assign_second(seconds, i);
}

template <quarterly::start S>
inline
void
yqnqdhms<S>::assign_na(r_ssize i) NOEXCEPT
{
  yqnqdhm<S>::assign_na(i);
  second_.assign_na(i);
}

template <quarterly::start S>
inline
void
yqnqdhms<S>::resolve(r_ssize i, const enum invalid type)
{
  const quarterly::year_quarternum_quarterday<S> elt = yqnqdhms<S>::to_year_quarternum_quarterday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    yqnqdhm<S>::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    break;
  case invalid::next: {
    yqnqdhm<S>::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    yqnqdhm<S>::assign_hour(rclock::detail::resolve_next_hour(), i);
    yqnqdhm<S>::assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    break;
  }
  case invalid::previous_day:
    yqnqdhm<S>::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    break;
  case invalid::previous: {
    yqnqdhm<S>::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    yqnqdhm<S>::assign_hour(rclock::detail::resolve_previous_hour(), i);
    yqnqdhm<S>::assign_minute(rclock::detail::resolve_previous_minute(), i);
    assign_second(rclock::detail::resolve_previous_second(), i);
    break;
  }
  case invalid::overflow_day:
    yqnqdhm<S>::assign_year_quarternum_quarterday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    yqnqdhm<S>::assign_year_quarternum_quarterday(date::sys_days{elt}, i);
    yqnqdhm<S>::assign_hour(rclock::detail::resolve_next_hour(), i);
    yqnqdhm<S>::assign_minute(rclock::detail::resolve_next_minute(), i);
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

template <quarterly::start S>
inline
date::sys_time<std::chrono::seconds>
yqnqdhms<S>::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yqnqdhm<S>::to_sys_time(i) + std::chrono::seconds{second_[i]};
}

template <quarterly::start S>
inline
cpp11::writable::list
yqnqdhms<S>::to_list() const
{
  cpp11::writable::list out({yqnqdhms<S>::year_.sexp(), yqnqdhms<S>::quarter_.sexp(), yqnqdhms<S>::day_.sexp(), yqnqdhms<S>::hour_.sexp(), yqnqdhms<S>::minute_.sexp(), second_.sexp()});
  out.names() = {"year", "quarter", "day", "hour", "minute", "second"};
  return out;
}

// yqnqdhmss

template <typename Duration, quarterly::start S>
inline
yqnqdhmss<Duration, S>::yqnqdhmss(r_ssize size)
  : yqnqdhms<S>(size),
    subsecond_(size)
  {}

template <typename Duration, quarterly::start S>
inline
yqnqdhmss<Duration, S>::yqnqdhmss(const cpp11::integers& year,
                                  const cpp11::integers& quarter,
                                  const cpp11::integers& day,
                                  const cpp11::integers& hour,
                                  const cpp11::integers& minute,
                                  const cpp11::integers& second,
                                  const cpp11::integers& subsecond)
  : yqnqdhms<S>(year, quarter, day, hour, minute, second),
    subsecond_(subsecond)
  {}

template <typename Duration, quarterly::start S>
inline
std::ostringstream&
yqnqdhmss<Duration, S>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yqnqdhm<S>::stream(os, i);
  os << ':';
  rclock::detail::stream_second_and_subsecond<Duration>(os, yqnqdhms<S>::second_[i], subsecond_[i]);
  return os;
}

template <typename Duration, quarterly::start S>
inline
void
yqnqdhmss<Duration, S>::assign_subsecond(const Duration& x, r_ssize i) NOEXCEPT
{
  subsecond_.assign(x.count(), i);
}

template <typename Duration, quarterly::start S>
inline
void
yqnqdhmss<Duration, S>::assign_sys_time(const date::sys_time<Duration>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::seconds> second_point = date::floor<std::chrono::seconds>(x);
  const Duration subseconds = x - second_point;
  yqnqdhms<S>::assign_sys_time(second_point, i);
  assign_subsecond(subseconds, i);
}

template <typename Duration, quarterly::start S>
inline
void
yqnqdhmss<Duration, S>::assign_na(r_ssize i) NOEXCEPT
{
  yqnqdhms<S>::assign_na(i);
  subsecond_.assign_na(i);
}

template <typename Duration, quarterly::start S>
inline
void
yqnqdhmss<Duration, S>::resolve(r_ssize i, const enum invalid type)
{
  const quarterly::year_quarternum_quarterday<S> elt = yqnqdhms<S>::to_year_quarternum_quarterday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    yqnqdhms<S>::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    break;
  case invalid::next: {
    yqnqdhms<S>::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    yqnqdhms<S>::assign_hour(rclock::detail::resolve_next_hour(), i);
    yqnqdhms<S>::assign_minute(rclock::detail::resolve_next_minute(), i);
    yqnqdhms<S>::assign_second(rclock::detail::resolve_next_second(), i);
    assign_subsecond(rclock::detail::resolve_next_subsecond<Duration>(), i);
    break;
  }
  case invalid::previous_day:
    yqnqdhms<S>::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    break;
  case invalid::previous: {
    yqnqdhms<S>::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    yqnqdhms<S>::assign_hour(rclock::detail::resolve_previous_hour(), i);
    yqnqdhms<S>::assign_minute(rclock::detail::resolve_previous_minute(), i);
    yqnqdhms<S>::assign_second(rclock::detail::resolve_previous_second(), i);
    assign_subsecond(rclock::detail::resolve_previous_subsecond<Duration>(), i);
    break;
  }
  case invalid::overflow_day:
    yqnqdhms<S>::assign_year_quarternum_quarterday(date::sys_days{elt}, i);
    break;
  case invalid::overflow: {
    yqnqdhms<S>::assign_year_quarternum_quarterday(date::sys_days{elt}, i);
    yqnqdhms<S>::assign_hour(rclock::detail::resolve_next_hour(), i);
    yqnqdhms<S>::assign_minute(rclock::detail::resolve_next_minute(), i);
    yqnqdhms<S>::assign_second(rclock::detail::resolve_next_second(), i);
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

template <typename Duration, quarterly::start S>
inline
date::sys_time<Duration>
yqnqdhmss<Duration, S>::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yqnqdhms<S>::to_sys_time(i) + Duration{subsecond_[i]};
}

template <typename Duration, quarterly::start S>
inline
cpp11::writable::list
yqnqdhmss<Duration, S>::to_list() const
{
  cpp11::writable::list out({yqnqdhms<S>::year_.sexp(), yqnqdhms<S>::quarter_.sexp(), yqnqdhms<S>::day_.sexp(), yqnqdhms<S>::hour_.sexp(), yqnqdhms<S>::minute_.sexp(), yqnqdhms<S>::second_.sexp(), subsecond_.sexp()});
  out.names() = {"year", "quarter", "day", "hour", "minute", "second", "subsecond"};
  return out;
}

} // namespace quarterly

} // namespace rclock

#endif
