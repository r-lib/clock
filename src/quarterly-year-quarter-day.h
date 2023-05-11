#ifndef CLOCK_QUARTERLY_YEAR_QUARTER_DAY_H
#define CLOCK_QUARTERLY_YEAR_QUARTER_DAY_H

#include "clock.h"
#include "quarterly-shim.h"
#include "integers.h"
#include "enums.h"
#include "utils.h"
#include "stream.h"
#include "resolve.h"

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

inline
quarterly_shim::year_quarternum_quarterday
resolve_next_day_yqd(const quarterly_shim::year_quarternum_quarterday& x) {
  return ((x.year() / x.quarternum()) + quarterly::quarters(1)) / quarterly::quarterday{1u};
}
inline
quarterly_shim::year_quarternum_quarterday
resolve_previous_day_yqd(const quarterly_shim::year_quarternum_quarterday& x) {
  return x.year() / x.quarternum() / quarterly::last;
}
inline
quarterly_shim::year_quarternum_quarterday
resolve_overflow_day_yqd(const quarterly_shim::year_quarternum_quarterday& x) {
  return quarterly_shim::year_quarternum_quarterday{date::sys_days{x}, x.year().start()};
}

} // namespace detail

class y
{
protected:
  rclock::integers year_;
  quarterly::start start_;

public:
  y(r_ssize size, quarterly::start start);
  y(const cpp11::integers& year, quarterly::start start);

  bool is_na(r_ssize i) const NOEXCEPT;
  r_ssize size() const NOEXCEPT;

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void add(const date::years& x, r_ssize i) NOEXCEPT;

  void assign_year(const quarterly_shim::year& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  quarterly_shim::year to_year(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class yqn : public y
{
protected:
  rclock::integers quarter_;

public:
  yqn(r_ssize size, quarterly::start start);
  yqn(const cpp11::integers& year,
      const cpp11::integers& quarter,
      quarterly::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void add(const quarterly::quarters& x, r_ssize i) NOEXCEPT;

  void assign_quarternum(const quarterly::quarternum& x, r_ssize i) NOEXCEPT;
  void assign_year_quarternum(const quarterly_shim::year_quarternum& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  quarterly_shim::year_quarternum to_year_quarternum(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class yqnqd : public yqn
{
protected:
  rclock::integers day_;

public:
  yqnqd(r_ssize size, quarterly::start start);
  yqnqd(const cpp11::integers& year,
        const cpp11::integers& quarter,
        const cpp11::integers& day,
        quarterly::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_quarterday(const quarterly::quarterday& x, r_ssize i) NOEXCEPT;
  void assign_year_quarternum_quarterday(const quarterly_shim::year_quarternum_quarterday& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call);

  date::sys_time<date::days> to_sys_time(r_ssize i) const NOEXCEPT;
  quarterly_shim::year_quarternum_quarterday to_year_quarternum_quarterday(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class yqnqdh : public yqnqd
{
protected:
  rclock::integers hour_;

public:
  yqnqdh(r_ssize size, quarterly::start start);
  yqnqdh(const cpp11::integers& year,
         const cpp11::integers& quarter,
         const cpp11::integers& day,
         const cpp11::integers& hour,
         quarterly::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call);

  date::sys_time<std::chrono::hours> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class yqnqdhm : public yqnqdh
{
protected:
  rclock::integers minute_;

public:
  yqnqdhm(r_ssize size, quarterly::start start);
  yqnqdhm(const cpp11::integers& year,
          const cpp11::integers& quarter,
          const cpp11::integers& day,
          const cpp11::integers& hour,
          const cpp11::integers& minute,
          quarterly::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_minute(const std::chrono::minutes& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::minutes>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call);

  date::sys_time<std::chrono::minutes> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

class yqnqdhms : public yqnqdhm
{
protected:
  rclock::integers second_;

public:
  yqnqdhms(r_ssize size, quarterly::start start);
  yqnqdhms(const cpp11::integers& year,
           const cpp11::integers& quarter,
           const cpp11::integers& day,
           const cpp11::integers& hour,
           const cpp11::integers& minute,
           const cpp11::integers& second,
           quarterly::start start);

  std::ostringstream& stream(std::ostringstream&, r_ssize i) const NOEXCEPT;

  void assign_second(const std::chrono::seconds& x, r_ssize i) NOEXCEPT;
  void assign_sys_time(const date::sys_time<std::chrono::seconds>& x, r_ssize i) NOEXCEPT;
  void assign_na(r_ssize i) NOEXCEPT;

  void resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call);

  date::sys_time<std::chrono::seconds> to_sys_time(r_ssize i) const NOEXCEPT;
  cpp11::writable::list to_list() const;
};

template <typename Duration>
class yqnqdhmss : public yqnqdhms
{
protected:
  rclock::integers subsecond_;

public:
  yqnqdhmss(r_ssize size, quarterly::start start);
  yqnqdhmss(const cpp11::integers& year,
            const cpp11::integers& quarter,
            const cpp11::integers& day,
            const cpp11::integers& hour,
            const cpp11::integers& minute,
            const cpp11::integers& second,
            const cpp11::integers& subsecond,
            quarterly::start start);

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
y::y(r_ssize size, quarterly::start start)
  : year_(size),
    start_(start)
  {}

inline
y::y(const cpp11::integers& year, quarterly::start start)
  : year_(rclock::integers(year)),
    start_(start)
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
y::assign_year(const quarterly_shim::year& x, r_ssize i) NOEXCEPT
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
quarterly_shim::year
y::to_year(r_ssize i) const NOEXCEPT
{
  return quarterly_shim::year{year_[i], start_};
}

inline
cpp11::writable::list
y::to_list() const
{
  cpp11::writable::list out({year_.sexp()});
  out.names() = {"year"};
  return out;
}

// yqn

inline
yqn::yqn(r_ssize size, quarterly::start start)
  : y(size, start),
    quarter_(size)
  {}

inline
yqn::yqn(const cpp11::integers& year,
         const cpp11::integers& quarter,
         quarterly::start start)
  : y(year, start),
    quarter_(quarter)
  {}

inline
std::ostringstream&
yqn::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  y::stream(os, i);
  os << '-';
  detail::stream_quarter(os, quarter_[i]);
  return os;
}

inline
void
yqn::add(const quarterly::quarters& x, r_ssize i) NOEXCEPT
{
  assign_year_quarternum(to_year_quarternum(i) + x, i);
}

inline
void
yqn::assign_quarternum(const quarterly::quarternum& x, r_ssize i) NOEXCEPT
{
  quarter_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

inline
void
yqn::assign_year_quarternum(const quarterly_shim::year_quarternum& x, r_ssize i) NOEXCEPT
{
  y::assign_year(x.year(), i);
  assign_quarternum(x.quarternum(), i);
}

inline
void
yqn::assign_na(r_ssize i) NOEXCEPT
{
  y::assign_na(i);
  quarter_.assign_na(i);
}

inline
quarterly_shim::year_quarternum
yqn::to_year_quarternum(r_ssize i) const NOEXCEPT
{
  return y::to_year(i) / static_cast<unsigned>(quarter_[i]);
}

inline
cpp11::writable::list
yqn::to_list() const
{
  cpp11::writable::list out({y::year_.sexp(), quarter_.sexp()});
  out.names() = {"year", "quarter"};
  return out;
}

// yqnqd

inline
yqnqd::yqnqd(r_ssize size, quarterly::start start)
  : yqn(size, start),
    day_(size)
  {}

inline
yqnqd::yqnqd(const cpp11::integers& year,
             const cpp11::integers& quarter,
             const cpp11::integers& day,
             quarterly::start start)
  : yqn(year, quarter, start),
    day_(day)
  {}

inline
std::ostringstream&
yqnqd::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yqn::stream(os, i);
  os << '-';
  detail::stream_day(os, day_[i]);
  return os;
}

inline
void
yqnqd::assign_quarterday(const quarterly::quarterday& x, r_ssize i) NOEXCEPT
{
  day_.assign(static_cast<int>(static_cast<unsigned>(x)), i);
}

inline
void
yqnqd::assign_year_quarternum_quarterday(const quarterly_shim::year_quarternum_quarterday& x, r_ssize i) NOEXCEPT
{
  yqn::assign_year(x.year(), i);
  yqn::assign_quarternum(x.quarternum(), i);
  assign_quarterday(x.quarterday(), i);
}

inline
void
yqnqd::assign_sys_time(const date::sys_time<date::days>& x, r_ssize i) NOEXCEPT
{
  quarterly_shim::year_quarternum_quarterday yqnqd{x, y::start_};
  assign_year_quarternum_quarterday(yqnqd, i);
}

inline
void
yqnqd::assign_na(r_ssize i) NOEXCEPT
{
  yqn::assign_na(i);
  day_.assign_na(i);
}

inline
void
yqnqd::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const quarterly_shim::year_quarternum_quarterday elt = to_year_quarternum_quarterday(i);

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
    assign_year_quarternum_quarterday(detail::resolve_overflow_day_yqd(elt), i);
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
yqnqd::to_sys_time(r_ssize i) const NOEXCEPT
{
  return date::sys_time<date::days>{to_year_quarternum_quarterday(i)};
}

inline
quarterly_shim::year_quarternum_quarterday
yqnqd::to_year_quarternum_quarterday(r_ssize i) const NOEXCEPT
{
  return yqn::to_year_quarternum(i) / static_cast<unsigned>(day_[i]);
}

inline
cpp11::writable::list
yqnqd::to_list() const
{
  cpp11::writable::list out({yqn::year_.sexp(), yqn::quarter_.sexp(), day_.sexp()});
  out.names() = {"year", "quarter", "day"};
  return out;
}

// yqnqdh

inline
yqnqdh::yqnqdh(r_ssize size, quarterly::start start)
  : yqnqd(size, start),
    hour_(size)
  {}

inline
yqnqdh::yqnqdh(const cpp11::integers& year,
               const cpp11::integers& quarter,
               const cpp11::integers& day,
               const cpp11::integers& hour,
               quarterly::start start)
  : yqnqd(year, quarter, day, start),
    hour_(hour)
  {}

inline
std::ostringstream&
yqnqdh::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yqnqd::stream(os, i);
  os << 'T';
  rclock::detail::stream_hour(os, hour_[i]);
  return os;
}

inline
void
yqnqdh::assign_hour(const std::chrono::hours& x, r_ssize i) NOEXCEPT
{
  hour_.assign(x.count(), i);
}

inline
void
yqnqdh::assign_sys_time(const date::sys_time<std::chrono::hours>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<date::days> day_point = date::floor<date::days>(x);
  const std::chrono::hours hours = x - day_point;
  yqnqd::assign_sys_time(day_point, i);
  assign_hour(hours, i);
}

inline
void
yqnqdh::assign_na(r_ssize i) NOEXCEPT
{
  yqnqd::assign_na(i);
  hour_.assign_na(i);
}

inline
void
yqnqdh::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const quarterly_shim::year_quarternum_quarterday elt = yqnqd::to_year_quarternum_quarterday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    yqnqd::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    break;
  case invalid::next: {
    yqnqd::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    assign_hour(rclock::detail::resolve_next_hour(), i);
    break;
  }
  case invalid::previous_day:
    yqnqd::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    break;
  case invalid::previous: {
    yqnqd::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    assign_hour(rclock::detail::resolve_previous_hour(), i);
    break;
  }
  case invalid::overflow_day: {
    yqnqd::assign_year_quarternum_quarterday(detail::resolve_overflow_day_yqd(elt), i);
    break;
  }
  case invalid::overflow: {
    yqnqd::assign_year_quarternum_quarterday(detail::resolve_overflow_day_yqd(elt), i);
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
yqnqdh::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yqnqd::to_sys_time(i) + std::chrono::hours{hour_[i]};
}

inline
cpp11::writable::list
yqnqdh::to_list() const
{
  cpp11::writable::list out({yqnqd::year_.sexp(), yqnqd::quarter_.sexp(), yqnqd::day_.sexp(), hour_.sexp()});
  out.names() = {"year", "quarter", "day", "hour"};
  return out;
}

// yqnqdhm

inline
yqnqdhm::yqnqdhm(r_ssize size, quarterly::start start)
  : yqnqdh(size, start),
    minute_(size)
  {}

inline
yqnqdhm::yqnqdhm(const cpp11::integers& year,
                 const cpp11::integers& quarter,
                 const cpp11::integers& day,
                 const cpp11::integers& hour,
                 const cpp11::integers& minute,
                 quarterly::start start)
  : yqnqdh(year, quarter, day, hour, start),
    minute_(minute)
  {}

inline
std::ostringstream&
yqnqdhm::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yqnqdh::stream(os, i);
  os << ':';
  rclock::detail::stream_minute(os, minute_[i]);
  return os;
}

inline
void
yqnqdhm::assign_minute(const std::chrono::minutes& x, r_ssize i) NOEXCEPT
{
  minute_.assign(x.count(), i);
}

inline
void
yqnqdhm::assign_sys_time(const date::sys_time<std::chrono::minutes>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::hours> hour_point = date::floor<std::chrono::hours>(x);
  const std::chrono::minutes minutes = x - hour_point;
  yqnqdh::assign_sys_time(hour_point, i);
  assign_minute(minutes, i);
}

inline
void
yqnqdhm::assign_na(r_ssize i) NOEXCEPT
{
  yqnqdh::assign_na(i);
  minute_.assign_na(i);
}

inline
void
yqnqdhm::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const quarterly_shim::year_quarternum_quarterday elt = yqnqdh::to_year_quarternum_quarterday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    yqnqdh::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    break;
  case invalid::next: {
    yqnqdh::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    yqnqdh::assign_hour(rclock::detail::resolve_next_hour(), i);
    assign_minute(rclock::detail::resolve_next_minute(), i);
    break;
  }
  case invalid::previous_day:
    yqnqdh::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    break;
  case invalid::previous: {
    yqnqdh::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    yqnqdh::assign_hour(rclock::detail::resolve_previous_hour(), i);
    assign_minute(rclock::detail::resolve_previous_minute(), i);
    break;
  }
  case invalid::overflow_day: {
    yqnqdh::assign_year_quarternum_quarterday(detail::resolve_overflow_day_yqd(elt), i);
    break;
  }
  case invalid::overflow: {
    yqnqdh::assign_year_quarternum_quarterday(detail::resolve_overflow_day_yqd(elt), i);
    yqnqdh::assign_hour(rclock::detail::resolve_next_hour(), i);
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
yqnqdhm::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yqnqdh::to_sys_time(i) + std::chrono::minutes{minute_[i]};
}

inline
cpp11::writable::list
yqnqdhm::to_list() const
{
  cpp11::writable::list out({yqnqdh::year_.sexp(), yqnqdh::quarter_.sexp(), yqnqdh::day_.sexp(), yqnqdh::hour_.sexp(), minute_.sexp()});
  out.names() = {"year", "quarter", "day", "hour", "minute"};
  return out;
}

// yqnqdhms

inline
yqnqdhms::yqnqdhms(r_ssize size, quarterly::start start)
  : yqnqdhm(size, start),
    second_(size)
  {}

inline
yqnqdhms::yqnqdhms(const cpp11::integers& year,
                   const cpp11::integers& quarter,
                   const cpp11::integers& day,
                   const cpp11::integers& hour,
                   const cpp11::integers& minute,
                   const cpp11::integers& second,
                   quarterly::start start)
  : yqnqdhm(year, quarter, day, hour, minute, start),
    second_(second)
  {}

inline
std::ostringstream&
yqnqdhms::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yqnqdhm::stream(os, i);
  os << ':';
  rclock::detail::stream_second(os, second_[i]);
  return os;
}

inline
void
yqnqdhms::assign_second(const std::chrono::seconds& x, r_ssize i) NOEXCEPT
{
  second_.assign(x.count(), i);
}

inline
void
yqnqdhms::assign_sys_time(const date::sys_time<std::chrono::seconds>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::minutes> minute_point = date::floor<std::chrono::minutes>(x);
  const std::chrono::seconds seconds = x - minute_point;
  yqnqdhm::assign_sys_time(minute_point, i);
  assign_second(seconds, i);
}

inline
void
yqnqdhms::assign_na(r_ssize i) NOEXCEPT
{
  yqnqdhm::assign_na(i);
  second_.assign_na(i);
}

inline
void
yqnqdhms::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const quarterly_shim::year_quarternum_quarterday elt = yqnqdhms::to_year_quarternum_quarterday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    yqnqdhm::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    break;
  case invalid::next: {
    yqnqdhm::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    yqnqdhm::assign_hour(rclock::detail::resolve_next_hour(), i);
    yqnqdhm::assign_minute(rclock::detail::resolve_next_minute(), i);
    assign_second(rclock::detail::resolve_next_second(), i);
    break;
  }
  case invalid::previous_day:
    yqnqdhm::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    break;
  case invalid::previous: {
    yqnqdhm::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    yqnqdhm::assign_hour(rclock::detail::resolve_previous_hour(), i);
    yqnqdhm::assign_minute(rclock::detail::resolve_previous_minute(), i);
    assign_second(rclock::detail::resolve_previous_second(), i);
    break;
  }
  case invalid::overflow_day: {
    yqnqdhm::assign_year_quarternum_quarterday(detail::resolve_overflow_day_yqd(elt), i);
    break;
  }
  case invalid::overflow: {
    yqnqdhm::assign_year_quarternum_quarterday(detail::resolve_overflow_day_yqd(elt), i);
    yqnqdhm::assign_hour(rclock::detail::resolve_next_hour(), i);
    yqnqdhm::assign_minute(rclock::detail::resolve_next_minute(), i);
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
yqnqdhms::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yqnqdhm::to_sys_time(i) + std::chrono::seconds{second_[i]};
}

inline
cpp11::writable::list
yqnqdhms::to_list() const
{
  cpp11::writable::list out({yqnqdhms::year_.sexp(), yqnqdhms::quarter_.sexp(), yqnqdhms::day_.sexp(), yqnqdhms::hour_.sexp(), yqnqdhms::minute_.sexp(), second_.sexp()});
  out.names() = {"year", "quarter", "day", "hour", "minute", "second"};
  return out;
}

// yqnqdhmss

template <typename Duration>
inline
yqnqdhmss<Duration>::yqnqdhmss(r_ssize size, quarterly::start start)
  : yqnqdhms(size, start),
    subsecond_(size)
  {}

template <typename Duration>
inline
yqnqdhmss<Duration>::yqnqdhmss(const cpp11::integers& year,
                               const cpp11::integers& quarter,
                               const cpp11::integers& day,
                               const cpp11::integers& hour,
                               const cpp11::integers& minute,
                               const cpp11::integers& second,
                               const cpp11::integers& subsecond,
                               quarterly::start start)
  : yqnqdhms(year, quarter, day, hour, minute, second, start),
    subsecond_(subsecond)
  {}

template <typename Duration>
inline
std::ostringstream&
yqnqdhmss<Duration>::stream(std::ostringstream& os, r_ssize i) const NOEXCEPT
{
  yqnqdhm::stream(os, i);
  os << ':';
  rclock::detail::stream_second_and_subsecond<Duration>(os, yqnqdhms::second_[i], subsecond_[i]);
  return os;
}

template <typename Duration>
inline
void
yqnqdhmss<Duration>::assign_subsecond(const Duration& x, r_ssize i) NOEXCEPT
{
  subsecond_.assign(x.count(), i);
}

template <typename Duration>
inline
void
yqnqdhmss<Duration>::assign_sys_time(const date::sys_time<Duration>& x, r_ssize i) NOEXCEPT
{
  const date::sys_time<std::chrono::seconds> second_point = date::floor<std::chrono::seconds>(x);
  const Duration subseconds = x - second_point;
  yqnqdhms::assign_sys_time(second_point, i);
  assign_subsecond(subseconds, i);
}

template <typename Duration>
inline
void
yqnqdhmss<Duration>::assign_na(r_ssize i) NOEXCEPT
{
  yqnqdhms::assign_na(i);
  subsecond_.assign_na(i);
}

template <typename Duration>
inline
void
yqnqdhmss<Duration>::resolve(r_ssize i, const enum invalid type, const cpp11::sexp& call)
{
  const quarterly_shim::year_quarternum_quarterday elt = yqnqdhms::to_year_quarternum_quarterday(i);

  if (elt.ok()) {
    return;
  }

  switch (type) {
  case invalid::next_day:
    yqnqdhms::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    break;
  case invalid::next: {
    yqnqdhms::assign_year_quarternum_quarterday(detail::resolve_next_day_yqd(elt), i);
    yqnqdhms::assign_hour(rclock::detail::resolve_next_hour(), i);
    yqnqdhms::assign_minute(rclock::detail::resolve_next_minute(), i);
    yqnqdhms::assign_second(rclock::detail::resolve_next_second(), i);
    assign_subsecond(rclock::detail::resolve_next_subsecond<Duration>(), i);
    break;
  }
  case invalid::previous_day:
    yqnqdhms::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    break;
  case invalid::previous: {
    yqnqdhms::assign_quarterday(detail::resolve_previous_day_yqd(elt).quarterday(), i);
    yqnqdhms::assign_hour(rclock::detail::resolve_previous_hour(), i);
    yqnqdhms::assign_minute(rclock::detail::resolve_previous_minute(), i);
    yqnqdhms::assign_second(rclock::detail::resolve_previous_second(), i);
    assign_subsecond(rclock::detail::resolve_previous_subsecond<Duration>(), i);
    break;
  }
  case invalid::overflow_day: {
    yqnqdhms::assign_year_quarternum_quarterday(detail::resolve_overflow_day_yqd(elt), i);
    break;
  }
  case invalid::overflow: {
    yqnqdhms::assign_year_quarternum_quarterday(detail::resolve_overflow_day_yqd(elt), i);
    yqnqdhms::assign_hour(rclock::detail::resolve_next_hour(), i);
    yqnqdhms::assign_minute(rclock::detail::resolve_next_minute(), i);
    yqnqdhms::assign_second(rclock::detail::resolve_next_second(), i);
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
yqnqdhmss<Duration>::to_sys_time(r_ssize i) const NOEXCEPT
{
  return yqnqdhms::to_sys_time(i) + Duration{subsecond_[i]};
}

template <typename Duration>
inline
cpp11::writable::list
yqnqdhmss<Duration>::to_list() const
{
  cpp11::writable::list out({yqnqdhms::year_.sexp(), yqnqdhms::quarter_.sexp(), yqnqdhms::day_.sexp(), yqnqdhms::hour_.sexp(), yqnqdhms::minute_.sexp(), yqnqdhms::second_.sexp(), subsecond_.sexp()});
  out.names() = {"year", "quarter", "day", "hour", "minute", "second", "subsecond"};
  return out;
}

} // namespace quarterly

} // namespace rclock

#endif
