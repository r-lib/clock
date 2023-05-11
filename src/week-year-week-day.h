#ifndef CLOCK_WEEK_YEAR_WEEK_DAY_H
#define CLOCK_WEEK_YEAR_WEEK_DAY_H

#include "clock.h"
#include "week.h"
#include "week-shim.h"
#include "integers.h"
#include "enums.h"
#include "utils.h"
#include "stream.h"
#include "resolve.h"

namespace rclock {

namespace rweek {

namespace detail {

inline
std::ostringstream&
stream_week(std::ostringstream& os, int week) NOEXCEPT
{
  os << 'W';
  os.fill('0');
  os.flags(std::ios::dec | std::ios::right);
  os.width(2);
  os << week;
  return os;
}
inline
std::ostringstream&
stream_day(std::ostringstream& os, int day) NOEXCEPT
{
  os << day;
  return os;
}

inline
week_shim::year_weeknum_weekday
resolve_next_day_ywd(const week_shim::year_weeknum_weekday& x) {
  // Only invalid on nonexistent week 53 day, rolls to first day of next year
  return (x.year() + week::years{1}) / week::weeknum{1} / week_shim::weekday(1u);
}
inline
week_shim::year_weeknum_weekday
resolve_previous_day_ywd(const week_shim::year_weeknum_weekday& x) {
  // Only invalid on nonexistent week 53 day, rolls to last day of current year
  return x.year() / week::last / week_shim::weekday(7u);
}
inline
week_shim::year_weeknum_weekday
resolve_overflow_day_ywd(const week_shim::year_weeknum_weekday& x) {
  // Only invalid on nonexistent week 53 day, rolls into next year
  return week_shim::year_weeknum_weekday{date::sys_days{x}, x.year().start()};
}

inline
week_shim::year_weeknum
resolve_next_day_yw(const week_shim::year_weeknum& x) {
  // Only invalid on nonexistent week 53 day, rolls to first week of next year
  return (x.year() + week::years{1}) / week::weeknum{1};
}
inline
week_shim::year_weeknum
resolve_previous_day_yw(const week_shim::year_weeknum& x) {
  // Only invalid on nonexistent week 53 day, rolls to last week of current year
  return x.year() / week::last;
}

} // namespace detail

class y
{
protected:
  rclock::integers year_;
  week::start start_;

public:
  y(r_ssize size, week::start start);
  y(const cpp11::integers& year, week::start start);

  bool is_na(r_ssize i) const NOEXCEPT;
  r_ssize size() const NOEXCEPT;

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void add(const date::years& x, r_ssize i) NOEXCEPT;

  void assign_year(const week_shim::year& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  week_shim::year to_year(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ywn : public y
{
protected:
  rclock::integers week_;

public:
  ywn(r_ssize size, week::start start);
  ywn(const cpp11::integers& year,
      const cpp11::integers& week,
      week::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_weeknum(const week::weeknum& x, r_ssize i) NOEXCEPT;
  void assign_year_weeknum(const week_shim::year_weeknum& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call);

  week_shim::year_weeknum to_year_weeknum(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ywnwd : public ywn
{
protected:
  rclock::integers day_;

public:
  ywnwd(r_ssize size, week::start start);
  ywnwd(const cpp11::integers& year,
        const cpp11::integers& week,
        const cpp11::integers& day,
        week::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_weekday(const week_shim::weekday& x, r_ssize i) NOEXCEPT;
  void assign_year_weeknum_weekday(const week_shim::year_weeknum_weekday& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call);

  date::sys_time<date::days> to_sys_time(r_ssize i) const NOEXCEPT;
  week_shim::year_weeknum_weekday to_year_weeknum_weekday(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ywnwdh : public ywnwd
{
protected:
  rclock::integers hour_;

public:
  ywnwdh(r_ssize size, week::start start);
  ywnwdh(const cpp11::integers& year,
         const cpp11::integers& week,
         const cpp11::integers& day,
         const cpp11::integers& hour,
         week::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call);

  date::sys_time<std::chrono::hours> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ywnwdhm : public ywnwdh
{
protected:
  rclock::integers minute_;

public:
  ywnwdhm(r_ssize size, week::start start);
  ywnwdhm(const cpp11::integers& year,
          const cpp11::integers& week,
          const cpp11::integers& day,
          const cpp11::integers& hour,
          const cpp11::integers& minute,
          week::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_minute(const std::chrono::minutes& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::minutes>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call);

  date::sys_time<std::chrono::minutes> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class ywnwdhms : public ywnwdhm
{
protected:
  rclock::integers second_;

public:
  ywnwdhms(r_ssize size, week::start start);
  ywnwdhms(const cpp11::integers& year,
           const cpp11::integers& week,
           const cpp11::integers& day,
           const cpp11::integers& hour,
           const cpp11::integers& minute,
           const cpp11::integers& second,
           week::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_second(const std::chrono::seconds& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::seconds>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call);

  date::sys_time<std::chrono::seconds> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

template <typename Duration>
class ywnwdhmss : public ywnwdhms
{
protected:
  rclock::integers subsecond_;

public:
  ywnwdhmss(r_ssize size, week::start start);
  ywnwdhmss(const cpp11::integers& year,
            const cpp11::integers& week,
            const cpp11::integers& day,
            const cpp11::integers& hour,
            const cpp11::integers& minute,
            const cpp11::integers& second,
            const cpp11::integers& subsecond,
            week::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_subsecond(const Duration& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<Duration>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call);

  date::sys_time<Duration> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

// Implementation

// y

inline
y::y(r_ssize size, week::start start)
  : year_(size)
  , start_(start)
  {}

inline
y::y(const cpp11::integers& year, week::start start)
  : year_(rclock::integers(year))
  , start_(start)
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
void
y::add(const date::years& x, r_ssize i) NOEXCEPT
{
  assign_year(to_year(i) + x, i);
}

inline
void
y::assign_year(const week_shim::year& x, r_ssize i) NOEXCEPT
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
week_shim::year
y::to_year(r_ssize i) const NOEXCEPT
{
  return week_shim::year{year_[i], start_};
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
ywn::ywn(r_ssize size, week::start start)
  : y(size, start)
  , week_(size)
  {}

inline
ywn::ywn(const cpp11::integers& year,
         const cpp11::integers& week,
         week::start start)
  : y(year, start)
  , week_(week)
  {}

inline
std::ostringstream&
ywn::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  y::stream(os, i);
  os << '-';
  detail::stream_week(os, week_[i]);
  return os;
}

inline
void
ywn::assign_weeknum(const week::weeknum& x, r_ssize i) NOEXCEPT
{
  week_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

inline
void
ywn::assign_year_weeknum(const week_shim::year_weeknum& x, r_ssize i) NOEXCEPT
{
  assign_year(x.year(), i);
  assign_weeknum(x.weeknum(), i);
}

inline
void
ywn::assign_na(r_ssize i) NOEXCEPT
{
  y::assign_na(i);
  week_.assign_na(i);
}

inline
void
ywn::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const week_shim::year_weeknum elt = to_year_weeknum(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
  case invalid::next: {
    assign_year_weeknum(detail::resolve_next_day_yw(elt), i);
    break;
  }
  case invalid::previous_day:
  case invalid::previous: {
    assign_year_weeknum(detail::resolve_previous_day_yw(elt), i);
    break;
  }
  case invalid::overflow_day:
  case invalid::overflow: {
    // Overflowing invalid 2019-53 results in 2020-01
    assign_year_weeknum(detail::resolve_next_day_yw(elt), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    rclock::detail::resolve_error(i, call);
  }
  }
}

inline
week_shim::year_weeknum
ywn::to_year_weeknum(r_ssize i) const NOEXCEPT
{
  return week_shim::year{year_[i], start_} / static_cast<unsigned>(week_[i]);
}

inline
cpp11::writable::list
ywn::to_list() const
{
  cpp11::writable::list out({year_.sexp(), week_.sexp()});
  out.names() = {"year", "week"};
  return out;
}

// ywnwd

inline
ywnwd::ywnwd(r_ssize size, week::start start)
  : ywn(size, start)
  , day_(size)
  {}

inline
ywnwd::ywnwd(const cpp11::integers& year,
             const cpp11::integers& week,
             const cpp11::integers& day,
             week::start start)
  : ywn(year, week, start)
  , day_(day)
  {}

inline
std::ostringstream&
ywnwd::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ywn::stream(os, i);
  os << '-';
  detail::stream_day(os, day_[i]);
  return os;
}

inline
void
ywnwd::assign_weekday(const week_shim::weekday& x, r_ssize i) NOEXCEPT
{
  day_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

inline
void
ywnwd::assign_year_weeknum_weekday(const week_shim::year_weeknum_weekday& x, r_ssize i) NOEXCEPT
{
  assign_year(x.year(), i);
  assign_weeknum(x.weeknum(), i);
  assign_weekday(x.weekday(), i);
}

inline
void
ywnwd::assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT
{
  week_shim::year_weeknum_weekday ywnwd{x, start_};
  assign_year_weeknum_weekday(ywnwd, i);
}

inline
void
ywnwd::assign_na(r_ssize i) NOEXCEPT
{
  ywn::assign_na(i);
  day_.assign_na(i);
}

inline
void
ywnwd::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const week_shim::year_weeknum_weekday elt = to_year_weeknum_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
  case invalid::next: {
    assign_year_weeknum_weekday(detail::resolve_next_day_ywd(elt), i);
    break;
  }
  case invalid::previous_day:
  case invalid::previous: {
    assign_year_weeknum_weekday(detail::resolve_previous_day_ywd(elt), i);
    break;
  }
  case invalid::overflow_day:
  case invalid::overflow: {
    assign_year_weeknum_weekday(detail::resolve_overflow_day_ywd(elt), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    rclock::detail::resolve_error(i, call);
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
week_shim::year_weeknum_weekday
ywnwd::to_year_weeknum_weekday(r_ssize i) const NOEXCEPT
{
  return week_shim::year{year_[i], start_} /
    static_cast<unsigned>(week_[i]) /
    static_cast<unsigned>(day_[i]);
}

inline
cpp11::writable::list
ywnwd::to_list() const
{
  cpp11::writable::list out({year_.sexp(), week_.sexp(), day_.sexp()});
  out.names() = {"year", "week", "day"};
  return out;
}

// ywnwdh

inline
ywnwdh::ywnwdh(r_ssize size, week::start start)
  : ywnwd(size, start)
  , hour_(size)
  {}

inline
ywnwdh::ywnwdh(const cpp11::integers& year,
               const cpp11::integers& week,
               const cpp11::integers& day,
               const cpp11::integers& hour,
               week::start start)
  : ywnwd(year, week, day, start)
  , hour_(hour)
{}

inline
std::ostringstream&
ywnwdh::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  ywnwd::stream(os, i);
  os << 'T';
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
ywnwdh::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const week_shim::year_weeknum_weekday elt = to_year_weeknum_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_weeknum_weekday(detail::resolve_next_day_ywd(elt), i);
    break;
  case invalid::next: {
    assign_year_weeknum_weekday(detail::resolve_next_day_ywd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    break;
  }
  case invalid::previous_day:
    assign_year_weeknum_weekday(detail::resolve_previous_day_ywd(elt), i);
    break;
  case invalid::previous: {
    assign_year_weeknum_weekday(detail::resolve_previous_day_ywd(elt), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_weeknum_weekday(detail::resolve_overflow_day_ywd(elt), i);
    break;
  case invalid::overflow: {
    assign_year_weeknum_weekday(detail::resolve_overflow_day_ywd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    rclock::detail::resolve_error(i, call);
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
  cpp11::writable::list out({year_.sexp(), week_.sexp(), day_.sexp(), hour_.sexp()});
  out.names() = {"year", "week", "day", "hour"};
  return out;
}

// ywnwdhm

inline
ywnwdhm::ywnwdhm(r_ssize size, week::start start)
  : ywnwdh(size, start)
  , minute_(size)
  {}

inline
ywnwdhm::ywnwdhm(const cpp11::integers& year,
                 const cpp11::integers& week,
                 const cpp11::integers& day,
                 const cpp11::integers& hour,
                 const cpp11::integers& minute,
                 week::start start)
  : ywnwdh(year, week, day, hour, start)
  , minute_(minute)
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
ywnwdhm::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const week_shim::year_weeknum_weekday elt = to_year_weeknum_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_weeknum_weekday(detail::resolve_next_day_ywd(elt), i);
    break;
  case invalid::next: {
    assign_year_weeknum_weekday(detail::resolve_next_day_ywd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    break;
  }
  case invalid::previous_day:
    assign_year_weeknum_weekday(detail::resolve_previous_day_ywd(elt), i);
    break;
  case invalid::previous: {
    assign_year_weeknum_weekday(detail::resolve_previous_day_ywd(elt), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_weeknum_weekday(detail::resolve_overflow_day_ywd(elt), i);
    break;
  case invalid::overflow: {
    assign_year_weeknum_weekday(detail::resolve_overflow_day_ywd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    break;
  }
  case invalid::na: {
    assign_na(i);
    break;
  }
  case invalid::error: {
    rclock::detail::resolve_error(i, call);
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
  cpp11::writable::list out({year_.sexp(), week_.sexp(), day_.sexp(), hour_.sexp(), minute_.sexp()});
  out.names() = {"year", "week", "day", "hour", "minute"};
  return out;
}

// ywnwdhms

inline
ywnwdhms::ywnwdhms(r_ssize size, week::start start)
  : ywnwdhm(size, start)
  , second_(size)
  {}

inline
ywnwdhms::ywnwdhms(const cpp11::integers& year,
                   const cpp11::integers& week,
                   const cpp11::integers& day,
                   const cpp11::integers& hour,
                   const cpp11::integers& minute,
                   const cpp11::integers& second,
                   week::start start)
  : ywnwdhm(year, week, day, hour, minute, start)
  , second_(second)
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
ywnwdhms::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const week_shim::year_weeknum_weekday elt = to_year_weeknum_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_weeknum_weekday(detail::resolve_next_day_ywd(elt), i);
    break;
  case invalid::next: {
    assign_year_weeknum_weekday(detail::resolve_next_day_ywd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    break;
  }
  case invalid::previous_day:
    assign_year_weeknum_weekday(detail::resolve_previous_day_ywd(elt), i);
    break;
  case invalid::previous: {
    assign_year_weeknum_weekday(detail::resolve_previous_day_ywd(elt), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    assign_second(rclock::detail::resolve_previous_second(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_weeknum_weekday(detail::resolve_overflow_day_ywd(elt), i);
    break;
  case invalid::overflow: {
    assign_year_weeknum_weekday(detail::resolve_overflow_day_ywd(elt), i);
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
    rclock::detail::resolve_error(i, call);
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
  cpp11::writable::list out({year_.sexp(), week_.sexp(), day_.sexp(), hour_.sexp(), minute_.sexp(), second_.sexp()});
  out.names() = {"year", "week", "day", "hour", "minute", "second"};
  return out;
}

// ywnwdhmss

template <typename Duration>
inline
ywnwdhmss<Duration>::ywnwdhmss(r_ssize size, week::start start)
  : ywnwdhms(size, start)
  , subsecond_(size)
  {}

template <typename Duration>
inline
ywnwdhmss<Duration>::ywnwdhmss(const cpp11::integers& year,
                               const cpp11::integers& week,
                               const cpp11::integers& day,
                               const cpp11::integers& hour,
                               const cpp11::integers& minute,
                               const cpp11::integers& second,
                               const cpp11::integers& subsecond,
                               week::start start)
  : ywnwdhms(year, week, day, hour, minute, second, start)
  , subsecond_(subsecond)
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
ywnwdhmss<Duration>::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const week_shim::year_weeknum_weekday elt = to_year_weeknum_weekday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    assign_year_weeknum_weekday(detail::resolve_next_day_ywd(elt), i);
    break;
  case invalid::next: {
    assign_year_weeknum_weekday(detail::resolve_next_day_ywd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    assign_subsecond(rclock::detail::resolve_next_subsecond<Duration>(), i);
    break;
  }
  case invalid::previous_day:
    assign_year_weeknum_weekday(detail::resolve_previous_day_ywd(elt), i);
    break;
  case invalid::previous: {
    assign_year_weeknum_weekday(detail::resolve_previous_day_ywd(elt), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    assign_second(rclock::detail::resolve_previous_second(), i);
    assign_subsecond(rclock::detail::resolve_previous_subsecond<Duration>(), i);
    break;
  }
  case invalid::overflow_day:
    assign_year_weeknum_weekday(detail::resolve_overflow_day_ywd(elt), i);
    break;
  case invalid::overflow: {
    assign_year_weeknum_weekday(detail::resolve_overflow_day_ywd(elt), i);
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
    rclock::detail::resolve_error(i, call);
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
  cpp11::writable::list out({year_.sexp(), week_.sexp(), day_.sexp(), hour_.sexp(), minute_.sexp(), second_.sexp(), subsecond_.sexp()});
  out.names() = {"year", "week", "day", "hour", "minute", "second", "subsecond"};
  return out;
}

} // namespace rweek

} // namespace rclock

#endif
