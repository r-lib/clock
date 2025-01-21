#ifndef CLOCK_QUARTERLY_SHIM_H
#define CLOCK_QUARTERLY_SHIM_H

/*
 * This file contains a minimal shim around the classes in `quarterly.h` that
 * exposes those classes in a template free way. The templating is necessary
 * in `quarterly.h` to correctly define the type, since the quarter start month
 * is a part of the type itself, but we can gloss over some of those details
 * for use in `quarterly-year-quarter-day.h`. Encapsulating the templates here
 * reduces the compilation time of clock significantly (around 3x, on an Intel
 * Mac the compilation time dropped from 70s to 25s).
 */

#include <tzdb/date.h>
#include "quarterly.h"

namespace rclock {
namespace rquarterly {
namespace quarterly_shim {

class year;

class year_quarternum;

class year_quarternum_quarterday;
class year_quarternum_quarterday_last;

// date composition operators

CONSTCD11 year_quarternum operator/(const year& y, const quarterly::quarternum& qn) NOEXCEPT;
CONSTCD11 year_quarternum operator/(const year& y, int                          qn) NOEXCEPT;

CONSTCD11 year_quarternum_quarterday operator/(const year_quarternum& yqn, const quarterly::quarterday& qd) NOEXCEPT;
CONSTCD11 year_quarternum_quarterday operator/(const year_quarternum& yqn, int                          qd) NOEXCEPT;

CONSTCD11 year_quarternum_quarterday_last operator/(const year_quarternum& yqn, quarterly::last_spec) NOEXCEPT;

// year

class year
{
  short y_;
  quarterly::start s_;

public:
  year() = default;
  explicit CONSTCD11 year(int y, quarterly::start s) NOEXCEPT;

  CONSTCD11 quarterly::start start() const NOEXCEPT;

  CONSTCD11 explicit operator int() const NOEXCEPT;

  CONSTCD14 bool is_leap() const NOEXCEPT;
};

CONSTCD14 year operator+(const year& x, const quarterly::years& y) NOEXCEPT;

CONSTCD14 quarterly::years operator-(const year& x, const year& y) NOEXCEPT;

// year_quarternum

class year_quarternum
{
  quarterly_shim::year y_;
  quarterly::quarternum qn_;

public:
  year_quarternum() = default;
  CONSTCD11 year_quarternum(const quarterly_shim::year& y,
                            const quarterly::quarternum& qn) NOEXCEPT;

  CONSTCD11 quarterly_shim::year year() const NOEXCEPT;
  CONSTCD11 quarterly::quarternum quarternum() const NOEXCEPT;
};

CONSTCD14 year_quarternum operator+(const year_quarternum& yqn, const quarterly::quarters& dq) NOEXCEPT;

CONSTCD14 quarterly::quarters operator-(const year_quarternum& x, const year_quarternum& y) NOEXCEPT;

// year_quarternum_quarterday

class year_quarternum_quarterday
{
  quarterly_shim::year y_;
  quarterly::quarternum qn_;
  quarterly::quarterday qd_;

public:
  year_quarternum_quarterday() = default;
  CONSTCD11 year_quarternum_quarterday(const quarterly_shim::year& y,
                                       const quarterly::quarternum& qn,
                                       const quarterly::quarterday& qd) NOEXCEPT;
  CONSTCD11 year_quarternum_quarterday(const quarterly_shim::year_quarternum& yqn,
                                       const quarterly::quarterday& qd) NOEXCEPT;
  CONSTCD14 year_quarternum_quarterday(const year_quarternum_quarterday_last& yqnqdl) NOEXCEPT;
  CONSTCD14 year_quarternum_quarterday(const date::sys_days& dp, quarterly::start s) NOEXCEPT;
  CONSTCD14 year_quarternum_quarterday(const date::local_days& dp, quarterly::start s) NOEXCEPT;

  CONSTCD11 quarterly_shim::year year() const NOEXCEPT;
  CONSTCD11 quarterly::quarternum quarternum() const NOEXCEPT;
  CONSTCD11 quarterly::quarterday quarterday() const NOEXCEPT;

  CONSTCD14 operator date::sys_days() const NOEXCEPT;
  CONSTCD14 explicit operator date::local_days() const NOEXCEPT;
  CONSTCD14 bool ok() const NOEXCEPT;

private:
  CONSTCD14 year_quarternum_quarterday from_sys_days(const date::sys_days& dp, quarterly::start s) NOEXCEPT;
  CONSTCD14 year_quarternum_quarterday from_local_days(const date::local_days& dp, quarterly::start s) NOEXCEPT;
};

// year_quarternum_quarterday_last

class year_quarternum_quarterday_last
{
  quarterly_shim::year y_;
  quarterly::quarternum qn_;

public:
  year_quarternum_quarterday_last() = default;
  CONSTCD11 year_quarternum_quarterday_last(const quarterly_shim::year& y,
                                            const quarterly::quarternum& qn) NOEXCEPT;
  CONSTCD11 year_quarternum_quarterday_last(const quarterly_shim::year_quarternum& yqn) NOEXCEPT;

  CONSTCD11 quarterly_shim::year year() const NOEXCEPT;
  CONSTCD11 quarterly::quarternum quarternum() const NOEXCEPT;
  CONSTCD14 quarterly::quarterday quarterday() const NOEXCEPT;
};

//----------------+
// Implementation |
//----------------+

namespace detail {

template <quarterly::start S>
CONSTCD14
inline
quarterly::year<S>
to_quarterly(const quarterly_shim::year& y) NOEXCEPT {
  return quarterly::year<S>(static_cast<int>(y));
}

template <quarterly::start S>
CONSTCD14
inline
quarterly::year_quarternum<S>
to_quarterly(const quarterly_shim::year_quarternum& yqn) NOEXCEPT {
  return quarterly::year_quarternum<S>(
    to_quarterly<S>(yqn.year()),
    yqn.quarternum()
  );
}

template <quarterly::start S>
CONSTCD14
inline
quarterly::year_quarternum_quarterday<S>
to_quarterly(const quarterly_shim::year_quarternum_quarterday& yqnqd) NOEXCEPT {
  return quarterly::year_quarternum_quarterday<S>(
    to_quarterly<S>(yqnqd.year()),
    yqnqd.quarternum(),
    yqnqd.quarterday()
  );
}

template <quarterly::start S>
CONSTCD14
inline
quarterly::year_quarternum_quarterday_last<S>
to_quarterly(const quarterly_shim::year_quarternum_quarterday_last& yqnqdl) NOEXCEPT {
  return quarterly::year_quarternum_quarterday_last<S>(
    to_quarterly<S>(yqnqdl.year()),
    yqnqdl.quarternum()
  );
}

template <quarterly::start S>
CONSTCD14
inline
quarterly_shim::year
from_quarterly(const quarterly::year<S>& y) NOEXCEPT {
  return quarterly_shim::year(static_cast<int>(y), S);
}

template <quarterly::start S>
CONSTCD14
inline
quarterly_shim::year_quarternum
from_quarterly(const quarterly::year_quarternum<S>& yqn) NOEXCEPT {
  return quarterly_shim::year_quarternum(
    from_quarterly(yqn.year()),
    yqn.quarternum()
  );
}

template <quarterly::start S>
CONSTCD14
inline
quarterly_shim::year_quarternum_quarterday
from_quarterly(const quarterly::year_quarternum_quarterday<S>& yqnqd) NOEXCEPT {
  return quarterly_shim::year_quarternum_quarterday(
    from_quarterly(yqnqd.year()),
    yqnqd.quarternum(),
    yqnqd.quarterday()
  );
}

} // namespace detail

// year

CONSTCD11
inline
year::year(int y, quarterly::start s) NOEXCEPT
  : y_(static_cast<decltype(y_)>(y)),
    s_(s)
  {}

CONSTCD11
inline
quarterly::start
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
  using start = quarterly::start;
  using detail::to_quarterly;

  switch (s_) {
  case start::january: return to_quarterly<start::january>(*this).is_leap();
  case start::february: return to_quarterly<start::february>(*this).is_leap();
  case start::march: return to_quarterly<start::march>(*this).is_leap();
  case start::april: return to_quarterly<start::april>(*this).is_leap();
  case start::may: return to_quarterly<start::may>(*this).is_leap();
  case start::june: return to_quarterly<start::june>(*this).is_leap();
  case start::july: return to_quarterly<start::july>(*this).is_leap();
  case start::august: return to_quarterly<start::august>(*this).is_leap();
  case start::september: return to_quarterly<start::september>(*this).is_leap();
  case start::october: return to_quarterly<start::october>(*this).is_leap();
  case start::november: return to_quarterly<start::november>(*this).is_leap();
  case start::december: return to_quarterly<start::december>(*this).is_leap();
  default: return /* unreachable */ false;
  }
}

CONSTCD14
inline
year
operator+(const year& x, const quarterly::years& y) NOEXCEPT
{
  using start = quarterly::start;
  using detail::from_quarterly;
  using detail::to_quarterly;

  switch (x.start()) {
  case start::january: return from_quarterly(to_quarterly<start::january>(x) + y);
  case start::february: return from_quarterly(to_quarterly<start::february>(x) + y);
  case start::march: return from_quarterly(to_quarterly<start::march>(x) + y);
  case start::april: return from_quarterly(to_quarterly<start::april>(x) + y);
  case start::may: return from_quarterly(to_quarterly<start::may>(x) + y);
  case start::june: return from_quarterly(to_quarterly<start::june>(x) + y);
  case start::july: return from_quarterly(to_quarterly<start::july>(x) + y);
  case start::august: return from_quarterly(to_quarterly<start::august>(x) + y);
  case start::september: return from_quarterly(to_quarterly<start::september>(x) + y);
  case start::october: return from_quarterly(to_quarterly<start::october>(x) + y);
  case start::november: return from_quarterly(to_quarterly<start::november>(x) + y);
  case start::december: return from_quarterly(to_quarterly<start::december>(x) + y);
  default: return /* unreachable */ year();
  }
}

CONSTCD14
inline
quarterly::years
operator-(const year& x, const year& y) NOEXCEPT
{
  using start = quarterly::start;
  using detail::to_quarterly;

  // Assumes `x.start()` and `y.start()` are the same, verified by caller
  switch (x.start()) {
  case start::january: return to_quarterly<start::january>(x) - to_quarterly<start::january>(y);
  case start::february: return to_quarterly<start::february>(x) - to_quarterly<start::february>(y);
  case start::march: return to_quarterly<start::march>(x) - to_quarterly<start::march>(y);
  case start::april: return to_quarterly<start::april>(x) - to_quarterly<start::april>(y);
  case start::may: return to_quarterly<start::may>(x) - to_quarterly<start::may>(y);
  case start::june: return to_quarterly<start::june>(x) - to_quarterly<start::june>(y);
  case start::july: return to_quarterly<start::july>(x) - to_quarterly<start::july>(y);
  case start::august: return to_quarterly<start::august>(x) - to_quarterly<start::august>(y);
  case start::september: return to_quarterly<start::september>(x) - to_quarterly<start::september>(y);
  case start::october: return to_quarterly<start::october>(x) - to_quarterly<start::october>(y);
  case start::november: return to_quarterly<start::november>(x) - to_quarterly<start::november>(y);
  case start::december: return to_quarterly<start::december>(x) - to_quarterly<start::december>(y);
  default: return /* unreachable */ quarterly::years();
  }
}

// year_quarternum

CONSTCD11
inline
year_quarternum::year_quarternum(const quarterly_shim::year& y,
                                 const quarterly::quarternum& qn) NOEXCEPT
  : y_(y)
  , qn_(qn)
  {}

CONSTCD11
inline
quarterly_shim::year
year_quarternum::year() const NOEXCEPT
{
  return y_;
}

CONSTCD11
inline
quarterly::quarternum
year_quarternum::quarternum() const NOEXCEPT
{
  return qn_;
}

CONSTCD14
inline
year_quarternum
operator+(const year_quarternum& yqn, const quarterly::quarters& dq) NOEXCEPT
{
  using start = quarterly::start;
  using detail::from_quarterly;
  using detail::to_quarterly;

  switch (yqn.year().start()) {
  case start::january: return from_quarterly(to_quarterly<start::january>(yqn) + dq);
  case start::february: return from_quarterly(to_quarterly<start::february>(yqn) + dq);
  case start::march: return from_quarterly(to_quarterly<start::march>(yqn) + dq);
  case start::april: return from_quarterly(to_quarterly<start::april>(yqn) + dq);
  case start::may: return from_quarterly(to_quarterly<start::may>(yqn) + dq);
  case start::june: return from_quarterly(to_quarterly<start::june>(yqn) + dq);
  case start::july: return from_quarterly(to_quarterly<start::july>(yqn) + dq);
  case start::august: return from_quarterly(to_quarterly<start::august>(yqn) + dq);
  case start::september: return from_quarterly(to_quarterly<start::september>(yqn) + dq);
  case start::october: return from_quarterly(to_quarterly<start::october>(yqn) + dq);
  case start::november: return from_quarterly(to_quarterly<start::november>(yqn) + dq);
  case start::december: return from_quarterly(to_quarterly<start::december>(yqn) + dq);
  default: return /* unreachable */ year_quarternum();
  }
}

CONSTCD14
inline
quarterly::quarters
operator-(const year_quarternum& x, const year_quarternum& y) NOEXCEPT
{
  using start = quarterly::start;
  using detail::to_quarterly;

  // Assumes `x.year().start()` and `y.year().start()` are the same, verified by caller
  switch (x.year().start()) {
  case start::january: return to_quarterly<start::january>(x) - to_quarterly<start::january>(y);
  case start::february: return to_quarterly<start::february>(x) - to_quarterly<start::february>(y);
  case start::march: return to_quarterly<start::march>(x) - to_quarterly<start::march>(y);
  case start::april: return to_quarterly<start::april>(x) - to_quarterly<start::april>(y);
  case start::may: return to_quarterly<start::may>(x) - to_quarterly<start::may>(y);
  case start::june: return to_quarterly<start::june>(x) - to_quarterly<start::june>(y);
  case start::july: return to_quarterly<start::july>(x) - to_quarterly<start::july>(y);
  case start::august: return to_quarterly<start::august>(x) - to_quarterly<start::august>(y);
  case start::september: return to_quarterly<start::september>(x) - to_quarterly<start::september>(y);
  case start::october: return to_quarterly<start::october>(x) - to_quarterly<start::october>(y);
  case start::november: return to_quarterly<start::november>(x) - to_quarterly<start::november>(y);
  case start::december: return to_quarterly<start::december>(x) - to_quarterly<start::december>(y);
  default: return /* unreachable */ quarterly::quarters();
  }
}

// year_quarternum_quarterday

CONSTCD11
inline
year_quarternum_quarterday::year_quarternum_quarterday(const quarterly_shim::year& y,
                                                       const quarterly::quarternum& qn,
                                                       const quarterly::quarterday& qd) NOEXCEPT
  : y_(y)
  , qn_(qn)
  , qd_(qd)
  {}

CONSTCD11
inline
year_quarternum_quarterday::year_quarternum_quarterday(const quarterly_shim::year_quarternum& yqn,
                                                       const quarterly::quarterday& qd) NOEXCEPT
  : y_(yqn.year())
  , qn_(yqn.quarternum())
  , qd_(qd)
  {}

CONSTCD14
inline
year_quarternum_quarterday::year_quarternum_quarterday(const year_quarternum_quarterday_last& yqnqdl) NOEXCEPT
  : y_(yqnqdl.year())
  , qn_(yqnqdl.quarternum())
  , qd_(yqnqdl.quarterday())
  {}

CONSTCD14
inline
year_quarternum_quarterday::year_quarternum_quarterday(const date::sys_days& dp, quarterly::start s) NOEXCEPT
  : year_quarternum_quarterday(from_sys_days(dp, s))
  {}

CONSTCD14
inline
year_quarternum_quarterday::year_quarternum_quarterday(const date::local_days& dp, quarterly::start s) NOEXCEPT
  : year_quarternum_quarterday(from_local_days(dp, s))
  {}

CONSTCD14
inline
year_quarternum_quarterday
year_quarternum_quarterday::from_sys_days(const date::sys_days& dp, quarterly::start s) NOEXCEPT {
  using start = quarterly::start;
  using detail::from_quarterly;

  switch (s) {
  case start::january: return from_quarterly(quarterly::year_quarternum_quarterday<start::january>(dp));
  case start::february: return from_quarterly(quarterly::year_quarternum_quarterday<start::february>(dp));
  case start::march: return from_quarterly(quarterly::year_quarternum_quarterday<start::march>(dp));
  case start::april: return from_quarterly(quarterly::year_quarternum_quarterday<start::april>(dp));
  case start::may: return from_quarterly(quarterly::year_quarternum_quarterday<start::may>(dp));
  case start::june: return from_quarterly(quarterly::year_quarternum_quarterday<start::june>(dp));
  case start::july: return from_quarterly(quarterly::year_quarternum_quarterday<start::july>(dp));
  case start::august: return from_quarterly(quarterly::year_quarternum_quarterday<start::august>(dp));
  case start::september: return from_quarterly(quarterly::year_quarternum_quarterday<start::september>(dp));
  case start::october: return from_quarterly(quarterly::year_quarternum_quarterday<start::october>(dp));
  case start::november: return from_quarterly(quarterly::year_quarternum_quarterday<start::november>(dp));
  case start::december: return from_quarterly(quarterly::year_quarternum_quarterday<start::december>(dp));
  default: return /* unreachable */ year_quarternum_quarterday();
  }
}

CONSTCD14
inline
year_quarternum_quarterday
year_quarternum_quarterday::from_local_days(const date::local_days& dp, quarterly::start s) NOEXCEPT {
  return from_sys_days(date::sys_days(dp.time_since_epoch()), s);
}

CONSTCD11
inline
quarterly_shim::year
year_quarternum_quarterday::year() const NOEXCEPT
{
  return y_;
}

CONSTCD11
inline
quarterly::quarternum
year_quarternum_quarterday::quarternum() const NOEXCEPT
{
  return qn_;
}

CONSTCD11
inline
quarterly::quarterday
year_quarternum_quarterday::quarterday() const NOEXCEPT
{
  return qd_;
}

CONSTCD14
inline
year_quarternum_quarterday::operator date::sys_days() const NOEXCEPT
{
  using start = quarterly::start;
  using detail::to_quarterly;

  switch (y_.start()) {
  case start::january: return to_quarterly<start::january>(*this);
  case start::february: return to_quarterly<start::february>(*this);
  case start::march: return to_quarterly<start::march>(*this);
  case start::april: return to_quarterly<start::april>(*this);
  case start::may: return to_quarterly<start::may>(*this);
  case start::june: return to_quarterly<start::june>(*this);
  case start::july: return to_quarterly<start::july>(*this);
  case start::august: return to_quarterly<start::august>(*this);
  case start::september: return to_quarterly<start::september>(*this);
  case start::october: return to_quarterly<start::october>(*this);
  case start::november: return to_quarterly<start::november>(*this);
  case start::december: return to_quarterly<start::december>(*this);
  default: return /* unreachable */ date::sys_days();
  }
}

CONSTCD14
inline
year_quarternum_quarterday::operator date::local_days() const NOEXCEPT
{
  using start = quarterly::start;
  using detail::to_quarterly;

  switch (y_.start()) {
  case start::january: return date::local_days(to_quarterly<start::january>(*this));
  case start::february: return date::local_days(to_quarterly<start::february>(*this));
  case start::march: return date::local_days(to_quarterly<start::march>(*this));
  case start::april: return date::local_days(to_quarterly<start::april>(*this));
  case start::may: return date::local_days(to_quarterly<start::may>(*this));
  case start::june: return date::local_days(to_quarterly<start::june>(*this));
  case start::july: return date::local_days(to_quarterly<start::july>(*this));
  case start::august: return date::local_days(to_quarterly<start::august>(*this));
  case start::september: return date::local_days(to_quarterly<start::september>(*this));
  case start::october: return date::local_days(to_quarterly<start::october>(*this));
  case start::november: return date::local_days(to_quarterly<start::november>(*this));
  case start::december: return date::local_days(to_quarterly<start::december>(*this));
  default: return /* unreachable */ date::local_days();
  }
}

CONSTCD14
inline
bool
year_quarternum_quarterday::ok() const NOEXCEPT
{
  using start = quarterly::start;
  using detail::to_quarterly;

  switch (y_.start()) {
  case start::january: return to_quarterly<start::january>(*this).ok();
  case start::february: return to_quarterly<start::february>(*this).ok();
  case start::march: return to_quarterly<start::march>(*this).ok();
  case start::april: return to_quarterly<start::april>(*this).ok();
  case start::may: return to_quarterly<start::may>(*this).ok();
  case start::june: return to_quarterly<start::june>(*this).ok();
  case start::july: return to_quarterly<start::july>(*this).ok();
  case start::august: return to_quarterly<start::august>(*this).ok();
  case start::september: return to_quarterly<start::september>(*this).ok();
  case start::october: return to_quarterly<start::october>(*this).ok();
  case start::november: return to_quarterly<start::november>(*this).ok();
  case start::december: return to_quarterly<start::december>(*this).ok();
  default: return /* unreachable */ false;
  }
}

// year_quarternum_quarterday_last

CONSTCD11
inline
year_quarternum_quarterday_last::year_quarternum_quarterday_last(const quarterly_shim::year& y,
                                                                 const quarterly::quarternum& qn) NOEXCEPT
  : y_(y)
  , qn_(qn)
  {}

CONSTCD11
inline
year_quarternum_quarterday_last::year_quarternum_quarterday_last(const quarterly_shim::year_quarternum& yqn) NOEXCEPT
  : y_(yqn.year())
  , qn_(yqn.quarternum())
  {}

CONSTCD11
inline
quarterly_shim::year
year_quarternum_quarterday_last::year() const NOEXCEPT
{
  return y_;
}

CONSTCD11
inline
quarterly::quarternum
year_quarternum_quarterday_last::quarternum() const NOEXCEPT
{
  return qn_;
}

CONSTCD14
inline
quarterly::quarterday
year_quarternum_quarterday_last::quarterday() const NOEXCEPT
{
  using start = quarterly::start;
  using detail::to_quarterly;

  switch (y_.start()) {
  case start::january: return to_quarterly<start::january>(*this).quarterday();
  case start::february: return to_quarterly<start::february>(*this).quarterday();
  case start::march: return to_quarterly<start::march>(*this).quarterday();
  case start::april: return to_quarterly<start::april>(*this).quarterday();
  case start::may: return to_quarterly<start::may>(*this).quarterday();
  case start::june: return to_quarterly<start::june>(*this).quarterday();
  case start::july: return to_quarterly<start::july>(*this).quarterday();
  case start::august: return to_quarterly<start::august>(*this).quarterday();
  case start::september: return to_quarterly<start::september>(*this).quarterday();
  case start::october: return to_quarterly<start::october>(*this).quarterday();
  case start::november: return to_quarterly<start::november>(*this).quarterday();
  case start::december: return to_quarterly<start::december>(*this).quarterday();
  default: return /* unreachable */ quarterly::quarterday();
  }
}

// year_quarternum from operator/()

CONSTCD11
inline
year_quarternum
operator/(const year& y, const quarterly::quarternum& qn) NOEXCEPT {
  return {y, qn};
}

CONSTCD11
inline
year_quarternum
operator/(const year& y, int qn) NOEXCEPT {
  return y / quarterly::quarternum(static_cast<unsigned>(qn));
}

// year_quarternum_quarterday from operator/()

CONSTCD11
inline
year_quarternum_quarterday
operator/(const year_quarternum& yqn, const quarterly::quarterday& qd) NOEXCEPT {
  return {yqn, qd};
}

CONSTCD11
inline
year_quarternum_quarterday
operator/(const year_quarternum& yqn, int qd) NOEXCEPT {
  return yqn / quarterly::quarterday(static_cast<unsigned>(qd));
}

// year_quarternum_quarterday_last from operator/()

CONSTCD11
inline
year_quarternum_quarterday_last
operator/(const year_quarternum& yqn, quarterly::last_spec) NOEXCEPT {
  return year_quarternum_quarterday_last{yqn};
}

} // namespace quarterly_shim
} // namespace rquarterly
} // namespace rclock

#endif
