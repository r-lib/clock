#ifndef CLOCK_WEEK_SHIM_H
#define CLOCK_WEEK_SHIM_H

/*
 * This file contains a minimal shim around the classes in `week.h` that
 * exposes those classes in a template free way. The templating is necessary
 * in `week.h` to correctly define the type, since the week start day
 * is a part of the type itself, but we can gloss over some of those details
 * for use in `week-year-week-day.h`.
 */

#include <tzdb/date.h>
#include "week.h"

namespace rclock {
namespace rweek {
namespace week_shim {

class year;
class weekday;

class year_weeknum;
class year_lastweek;

class year_weeknum_weekday;
class year_lastweek_weekday;

// date composition operators

CONSTCD11 year_weeknum operator/(const year& y, const week::weeknum& wn) NOEXCEPT;
CONSTCD11 year_weeknum operator/(const year& y, int                  wn) NOEXCEPT;

CONSTCD11 year_lastweek operator/(const year& y, week::last_week wn) NOEXCEPT;

CONSTCD11 year_weeknum_weekday operator/(const year_weeknum& ywn, const weekday& wd) NOEXCEPT;
CONSTCD11 year_weeknum_weekday operator/(const year_weeknum& ywn, int            wd) NOEXCEPT;

CONSTCD11 year_lastweek_weekday operator/(const year_lastweek& ylw, const weekday& wd) NOEXCEPT;

// year

class year
{
  short y_;
  week::start s_;

public:
  year() = default;
  explicit CONSTCD11 year(int y, week::start s) NOEXCEPT;

  CONSTCD11 week::start start() const NOEXCEPT;

  CONSTCD11 explicit operator int() const NOEXCEPT;

  CONSTCD14 bool is_leap() const NOEXCEPT;
};

CONSTCD14 year operator+(const year& x, const week::years& y) NOEXCEPT;

CONSTCD14 week::years operator-(const year& x, const year& y) NOEXCEPT;

// weekday

// `start` is implied by the `rweek::year`, since for clock you can't ever have
// a free floating `weekday` (so it doesn't need to know about `start`)
class weekday
{
  unsigned char wd_;

public:
  weekday() = default;
  explicit CONSTCD11 weekday(unsigned wd) NOEXCEPT;

  CONSTCD11 explicit operator unsigned() const NOEXCEPT;
};

// year_weeknum

class year_weeknum
{
  week_shim::year y_;
  week::weeknum wn_;

public:
  year_weeknum() = default;
  CONSTCD11 year_weeknum(const week_shim::year& y,
                         const week::weeknum& wn) NOEXCEPT;
  CONSTCD14 year_weeknum(const week_shim::year_lastweek& ylw) NOEXCEPT;

  CONSTCD11 week_shim::year year() const NOEXCEPT;
  CONSTCD11 week::weeknum weeknum() const NOEXCEPT;

  CONSTCD14 bool ok() const NOEXCEPT;
};

// year_lastweek

class year_lastweek
{
    week_shim::year y_;

public:
    year_lastweek() = default;
    CONSTCD11 explicit year_lastweek(const week_shim::year& y) NOEXCEPT;

    CONSTCD11 week_shim::year year() const NOEXCEPT;
    CONSTCD14 week::weeknum weeknum() const NOEXCEPT;
};

// year_weeknum_weekday

class year_weeknum_weekday
{
  week_shim::year y_;
  week::weeknum wn_;
  week_shim::weekday wd_;

public:
  year_weeknum_weekday() = default;
  CONSTCD11 year_weeknum_weekday(const week_shim::year& y,
                                 const week::weeknum& wn,
                                 const week_shim::weekday& wd) NOEXCEPT;
  CONSTCD11 year_weeknum_weekday(const week_shim::year_weeknum& ywn,
                                 const week_shim::weekday& wd) NOEXCEPT;
  CONSTCD14 year_weeknum_weekday(const year_lastweek_weekday& ylwwd) NOEXCEPT;
  CONSTCD14 year_weeknum_weekday(const date::sys_days& dp, week::start s) NOEXCEPT;
  CONSTCD14 year_weeknum_weekday(const date::local_days& dp, week::start s) NOEXCEPT;

  CONSTCD11 week_shim::year year() const NOEXCEPT;
  CONSTCD11 week::weeknum weeknum() const NOEXCEPT;
  CONSTCD11 week_shim::weekday weekday() const NOEXCEPT;

  CONSTCD14 operator date::sys_days() const NOEXCEPT;
  CONSTCD14 explicit operator date::local_days() const NOEXCEPT;
  CONSTCD14 bool ok() const NOEXCEPT;

private:
  CONSTCD14 year_weeknum_weekday from_sys_days(const date::sys_days& dp, week::start s) NOEXCEPT;
  CONSTCD14 year_weeknum_weekday from_local_days(const date::local_days& dp, week::start s) NOEXCEPT;
};

// year_lastweek_weekday

class year_lastweek_weekday
{
  week_shim::year y_;
  week_shim::weekday wd_;

public:
  year_lastweek_weekday() = default;
  CONSTCD11 year_lastweek_weekday(const week_shim::year& y,
                                  const week_shim::weekday& wd) NOEXCEPT;

  CONSTCD11 week_shim::year year() const NOEXCEPT;
  CONSTCD14 week::weeknum weeknum() const NOEXCEPT;
  CONSTCD11 week_shim::weekday weekday() const NOEXCEPT;
};

//----------------+
// Implementation |
//----------------+

namespace detail {

static
inline
void
never_reached [[noreturn]] () {
  // Compiler hint to allow [[noreturn]] attribute. This is never executed since
  // `never_reached()` is never actually called.
  throw std::runtime_error("[[noreturn]]");
}

} // namespace detail

namespace detail {

template <week::start S>
CONSTCD14
inline
week::year<S>
to_week(const week_shim::year& y) NOEXCEPT {
  return week::year<S>(static_cast<int>(y));
}

template <week::start S>
CONSTCD14
inline
week::weekday<S>
to_week(const week_shim::weekday& wd) NOEXCEPT {
  return week::weekday<S>(static_cast<unsigned>(wd));
}

template <week::start S>
CONSTCD14
inline
week::year_weeknum<S>
to_week(const week_shim::year_weeknum& ywn) NOEXCEPT {
  return week::year_weeknum<S>(
    to_week<S>(ywn.year()),
    ywn.weeknum()
  );
}

template <week::start S>
CONSTCD14
inline
week::year_lastweek<S>
to_week(const week_shim::year_lastweek& ylw) NOEXCEPT {
  return week::year_lastweek<S>(
    to_week<S>(ylw.year())
  );
}

template <week::start S>
CONSTCD14
inline
week::year_weeknum_weekday<S>
to_week(const week_shim::year_weeknum_weekday& ywnwd) NOEXCEPT {
  return week::year_weeknum_weekday<S>(
    to_week<S>(ywnwd.year()),
    ywnwd.weeknum(),
    to_week<S>(ywnwd.weekday())
  );
}

template <week::start S>
CONSTCD14
inline
week::year_lastweek_weekday<S>
to_week(const week_shim::year_lastweek_weekday& ylwwd) NOEXCEPT {
  return week::year_lastweek_weekday<S>(
    to_week<S>(ylwwd.year()),
    to_week<S>(ylwwd.weekday())
  );
}

template <week::start S>
CONSTCD14
inline
week_shim::year
from_week(const week::year<S>& y) NOEXCEPT {
  return week_shim::year(static_cast<int>(y), S);
}

template <week::start S>
CONSTCD14
inline
week_shim::weekday
from_week(const week::weekday<S>& wd) NOEXCEPT {
  return week_shim::weekday(static_cast<unsigned>(wd));
}

template <week::start S>
CONSTCD14
inline
week_shim::year_weeknum
from_week(const week::year_weeknum<S>& ywn) NOEXCEPT {
  return week_shim::year_weeknum(
    from_week(ywn.year()),
    ywn.weeknum()
  );
}

template <week::start S>
CONSTCD14
inline
week_shim::year_weeknum_weekday
from_week(const week::year_weeknum_weekday<S>& ywnwd) NOEXCEPT {
  return week_shim::year_weeknum_weekday(
    from_week(ywnwd.year()),
    ywnwd.weeknum(),
    from_week(ywnwd.weekday())
  );
}

} // namespace detail

// year

CONSTCD11
inline
year::year(int y, week::start s) NOEXCEPT
  : y_(static_cast<decltype(y_)>(y)),
    s_(s)
  {}

CONSTCD11
inline
week::start
year::start() const NOEXCEPT
{
  return s_;
}

CONSTCD11
inline
year::operator int() const NOEXCEPT
{
  return y_;
}

CONSTCD14
bool
year::is_leap() const NOEXCEPT {
  using start = week::start;
  using detail::to_week;

  switch (s_) {
  case start::sunday: return to_week<start::sunday>(*this).is_leap();
  case start::monday: return to_week<start::monday>(*this).is_leap();
  case start::tuesday: return to_week<start::tuesday>(*this).is_leap();
  case start::wednesday: return to_week<start::wednesday>(*this).is_leap();
  case start::thursday: return to_week<start::thursday>(*this).is_leap();
  case start::friday: return to_week<start::friday>(*this).is_leap();
  case start::saturday: return to_week<start::saturday>(*this).is_leap();
  default: detail::never_reached();
  }
}

CONSTCD14
inline
year
operator+(const year& x, const week::years& y) NOEXCEPT
{
  using start = week::start;
  using detail::from_week;
  using detail::to_week;

  switch (x.start()) {
  case start::sunday: return from_week(to_week<start::sunday>(x) + y);
  case start::monday: return from_week(to_week<start::monday>(x) + y);
  case start::tuesday: return from_week(to_week<start::tuesday>(x) + y);
  case start::wednesday: return from_week(to_week<start::wednesday>(x) + y);
  case start::thursday: return from_week(to_week<start::thursday>(x) + y);
  case start::friday: return from_week(to_week<start::friday>(x) + y);
  case start::saturday: return from_week(to_week<start::saturday>(x) + y);
  default: detail::never_reached();
  }
}

CONSTCD14
inline
week::years
operator-(const year& x, const year& y) NOEXCEPT
{
  using start = week::start;
  using detail::to_week;

  // Assumes `x.start()` and `y.start()` are the same, verified by caller
  switch (x.start()) {
  case start::sunday: return to_week<start::sunday>(x) - to_week<start::sunday>(y);
  case start::monday: return to_week<start::monday>(x) - to_week<start::monday>(y);
  case start::tuesday: return to_week<start::tuesday>(x) - to_week<start::tuesday>(y);
  case start::wednesday: return to_week<start::wednesday>(x) - to_week<start::wednesday>(y);
  case start::thursday: return to_week<start::thursday>(x) - to_week<start::thursday>(y);
  case start::friday: return to_week<start::friday>(x) - to_week<start::friday>(y);
  case start::saturday: return to_week<start::saturday>(x) - to_week<start::saturday>(y);
  default: detail::never_reached();
  }
}

// weekday

CONSTCD11
inline
weekday::weekday(unsigned wd) NOEXCEPT
  : wd_(static_cast<decltype(wd_)>(wd))
  {}

CONSTCD11
inline
weekday::operator unsigned() const NOEXCEPT
{
  return wd_;
}

// year_weeknum

CONSTCD11
inline
year_weeknum::year_weeknum(const week_shim::year& y,
                           const week::weeknum& wn) NOEXCEPT
  : y_(y)
  , wn_(wn)
  {}

CONSTCD14
inline
year_weeknum::year_weeknum(const week_shim::year_lastweek& ylw) NOEXCEPT
  : y_(ylw.year())
  , wn_(ylw.weeknum())
  {}

CONSTCD11
inline
week_shim::year
year_weeknum::year() const NOEXCEPT
{
  return y_;
}

CONSTCD11
inline
week::weeknum
year_weeknum::weeknum() const NOEXCEPT
{
  return wn_;
}

CONSTCD14
inline
bool
year_weeknum::ok() const NOEXCEPT
{
  using start = week::start;
  using detail::to_week;

  switch (this->year().start()) {
  case start::sunday: return to_week<start::sunday>(*this).ok();
  case start::monday: return to_week<start::monday>(*this).ok();
  case start::tuesday: return to_week<start::tuesday>(*this).ok();
  case start::wednesday: return to_week<start::wednesday>(*this).ok();
  case start::thursday: return to_week<start::thursday>(*this).ok();
  case start::friday: return to_week<start::friday>(*this).ok();
  case start::saturday: return to_week<start::saturday>(*this).ok();
  default: detail::never_reached();
  }
}

// year_lastweek

CONSTCD11
inline
year_lastweek::year_lastweek(const week_shim::year& y) NOEXCEPT
    : y_(y)
    {}

CONSTCD11 inline year year_lastweek::year() const NOEXCEPT {return y_;}

CONSTCD14
inline
week::weeknum
year_lastweek::weeknum() const NOEXCEPT
{
  using start = week::start;
  using detail::to_week;

  switch (this->year().start()) {
  case start::sunday: return to_week<start::sunday>(*this).weeknum();
  case start::monday: return to_week<start::monday>(*this).weeknum();
  case start::tuesday: return to_week<start::tuesday>(*this).weeknum();
  case start::wednesday: return to_week<start::wednesday>(*this).weeknum();
  case start::thursday: return to_week<start::thursday>(*this).weeknum();
  case start::friday: return to_week<start::friday>(*this).weeknum();
  case start::saturday: return to_week<start::saturday>(*this).weeknum();
  default: detail::never_reached();
  }
}

// year_weeknum_weekday

CONSTCD11
inline
year_weeknum_weekday::year_weeknum_weekday(const week_shim::year& y,
                                           const week::weeknum& wn,
                                           const week_shim::weekday& wd) NOEXCEPT
  : y_(y)
  , wn_(wn)
  , wd_(wd)
  {}

CONSTCD11
inline
year_weeknum_weekday::year_weeknum_weekday(const week_shim::year_weeknum& ywn,
                                           const week_shim::weekday& wd) NOEXCEPT
  : y_(ywn.year())
  , wn_(ywn.weeknum())
  , wd_(wd)
  {}

CONSTCD14
inline
year_weeknum_weekday::year_weeknum_weekday(const year_lastweek_weekday& ylwwd) NOEXCEPT
  : y_(ylwwd.year())
  , wn_(ylwwd.weeknum())
  , wd_(ylwwd.weekday())
  {}

CONSTCD14
inline
year_weeknum_weekday::year_weeknum_weekday(const date::sys_days& dp, week::start s) NOEXCEPT
  : year_weeknum_weekday(from_sys_days(dp, s))
  {}

CONSTCD14
inline
year_weeknum_weekday::year_weeknum_weekday(const date::local_days& dp, week::start s) NOEXCEPT
  : year_weeknum_weekday(from_local_days(dp, s))
  {}

CONSTCD14
inline
year_weeknum_weekday
year_weeknum_weekday::from_sys_days(const date::sys_days& dp, week::start s) NOEXCEPT {
  using start = week::start;
  using detail::from_week;

  switch (s) {
  case start::sunday: return from_week(week::year_weeknum_weekday<start::sunday>(dp));
  case start::monday: return from_week(week::year_weeknum_weekday<start::monday>(dp));
  case start::tuesday: return from_week(week::year_weeknum_weekday<start::tuesday>(dp));
  case start::wednesday: return from_week(week::year_weeknum_weekday<start::wednesday>(dp));
  case start::thursday: return from_week(week::year_weeknum_weekday<start::thursday>(dp));
  case start::friday: return from_week(week::year_weeknum_weekday<start::friday>(dp));
  case start::saturday: return from_week(week::year_weeknum_weekday<start::saturday>(dp));
  default: detail::never_reached();
  }
}

CONSTCD14
inline
year_weeknum_weekday
year_weeknum_weekday::from_local_days(const date::local_days& dp, week::start s) NOEXCEPT {
  return from_sys_days(date::sys_days(dp.time_since_epoch()), s);
}

CONSTCD11
inline
week_shim::year
year_weeknum_weekday::year() const NOEXCEPT
{
  return y_;
}

CONSTCD11
inline
week::weeknum
year_weeknum_weekday::weeknum() const NOEXCEPT
{
  return wn_;
}

CONSTCD11
inline
week_shim::weekday
year_weeknum_weekday::weekday() const NOEXCEPT
{
  return wd_;
}

CONSTCD14
inline
year_weeknum_weekday::operator date::sys_days() const NOEXCEPT
{
  using start = week::start;
  using detail::to_week;

  switch (y_.start()) {
  case start::sunday: return to_week<start::sunday>(*this);
  case start::monday: return to_week<start::monday>(*this);
  case start::tuesday: return to_week<start::tuesday>(*this);
  case start::wednesday: return to_week<start::wednesday>(*this);
  case start::thursday: return to_week<start::thursday>(*this);
  case start::friday: return to_week<start::friday>(*this);
  case start::saturday: return to_week<start::saturday>(*this);
  default: detail::never_reached();
  }
}

CONSTCD14
inline
year_weeknum_weekday::operator date::local_days() const NOEXCEPT
{
  using start = week::start;
  using detail::to_week;

  switch (y_.start()) {
  case start::sunday: return date::local_days(to_week<start::sunday>(*this));
  case start::monday: return date::local_days(to_week<start::monday>(*this));
  case start::tuesday: return date::local_days(to_week<start::tuesday>(*this));
  case start::wednesday: return date::local_days(to_week<start::wednesday>(*this));
  case start::thursday: return date::local_days(to_week<start::thursday>(*this));
  case start::friday: return date::local_days(to_week<start::friday>(*this));
  case start::saturday: return date::local_days(to_week<start::saturday>(*this));
  default: detail::never_reached();
  }
}

CONSTCD14
inline
bool
year_weeknum_weekday::ok() const NOEXCEPT
{
  using start = week::start;
  using detail::to_week;

  switch (y_.start()) {
  case start::sunday: return to_week<start::sunday>(*this).ok();
  case start::monday: return to_week<start::monday>(*this).ok();
  case start::tuesday: return to_week<start::tuesday>(*this).ok();
  case start::wednesday: return to_week<start::wednesday>(*this).ok();
  case start::thursday: return to_week<start::thursday>(*this).ok();
  case start::friday: return to_week<start::friday>(*this).ok();
  case start::saturday: return to_week<start::saturday>(*this).ok();
  default: detail::never_reached();
  }
}

// year_lastweek_weekday

CONSTCD11
inline
year_lastweek_weekday::year_lastweek_weekday(const week_shim::year& y,
                                             const week_shim::weekday& wd) NOEXCEPT
  : y_(y)
  , wd_(wd)
  {}

CONSTCD11
inline
week_shim::year
year_lastweek_weekday::year() const NOEXCEPT
{
  return y_;
}

CONSTCD14
inline
week::weeknum
year_lastweek_weekday::weeknum() const NOEXCEPT
{
  using start = week::start;
  using detail::to_week;

  switch (y_.start()) {
  case start::sunday: return to_week<start::sunday>(*this).weeknum();
  case start::monday: return to_week<start::monday>(*this).weeknum();
  case start::tuesday: return to_week<start::tuesday>(*this).weeknum();
  case start::wednesday: return to_week<start::wednesday>(*this).weeknum();
  case start::thursday: return to_week<start::thursday>(*this).weeknum();
  case start::friday: return to_week<start::friday>(*this).weeknum();
  case start::saturday: return to_week<start::saturday>(*this).weeknum();
  default: detail::never_reached();
  }
}

CONSTCD11
inline
week_shim::weekday
year_lastweek_weekday::weekday() const NOEXCEPT
{
  return wd_;
}

// year_weeknum from operator/()

CONSTCD11
inline
year_weeknum
operator/(const year& y, const week::weeknum& wn) NOEXCEPT {
  return {y, wn};
}

CONSTCD11
inline
year_weeknum
operator/(const year& y, int wn) NOEXCEPT {
  return y / week::weeknum(static_cast<unsigned>(wn));
}

// year_lastweek from operator/()

CONSTCD11
inline
year_lastweek
operator/(const year& y, week::last_week) NOEXCEPT
{
    return year_lastweek{y};
}

// year_weeknum_weekday from operator/()

CONSTCD11
inline
year_weeknum_weekday
operator/(const year_weeknum& ywn, const weekday& wd) NOEXCEPT {
  return {ywn, wd};
}

CONSTCD11
inline
year_weeknum_weekday
operator/(const year_weeknum& ywn, int wd) NOEXCEPT {
  return ywn / weekday(static_cast<unsigned>(wd));
}

// year_lastweek_weekday from operator/()

CONSTCD11
inline
year_lastweek_weekday
operator/(const year_lastweek& ylw, const weekday& wd) NOEXCEPT
{
    return {ylw.year(), wd};
}

} // namespace week_shim
} // namespace rweek
} // namespace rclock

#endif
