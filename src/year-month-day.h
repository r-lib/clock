#ifndef CLOCK_YEAR_MONTH_DAY_H
#define CLOCK_YEAR_MONTH_DAY_H

#include "clock.h"

namespace rclock {

class year_month_day
{
  const cpp11::integers& year_;
  const cpp11::integers& month_;
  const cpp11::integers& day_;

public:
  CONSTCD11 year_month_day(const cpp11::integers& year,
                           const cpp11::integers& month,
                           const cpp11::integers& day) NOEXCEPT;

  bool is_na(r_ssize i) const NOEXCEPT;
  CONSTCD11 r_ssize size() const NOEXCEPT;

  CONSTCD11 date::year_month_day operator[](r_ssize i) const NOEXCEPT;
};

CONSTCD11
inline
year_month_day::year_month_day(const cpp11::integers& year,
                               const cpp11::integers& month,
                               const cpp11::integers& day) NOEXCEPT
  : year_(year),
    month_(month),
    day_(day)
  {}

inline
bool
year_month_day::is_na(r_ssize i) const NOEXCEPT
{
  return year_[i] == r_int_na;
}

CONSTCD11
inline
r_ssize
year_month_day::size() const NOEXCEPT
{
  return year_.size();
}

CONSTCD11
inline
date::year_month_day
year_month_day::operator[](r_ssize i) const NOEXCEPT
{
  return date::year{year_[i]} /
    date::month{static_cast<unsigned>(month_[i])} /
    date::day{static_cast<unsigned>(day_[i])};
}

namespace writable {

class year_month_day
{
  cpp11::writable::integers year_;
  cpp11::writable::integers month_;
  cpp11::writable::integers day_;

public:
  year_month_day(r_ssize size);

  cpp11::writable::list_of<cpp11::writable::integers> to_list() const;

  void assign_na(r_ssize i) NOEXCEPT;
  void assign(const date::year_month_day& ymd, r_ssize i) NOEXCEPT;
};

inline
year_month_day::year_month_day(r_ssize size)
 {
  year_ = cpp11::writable::integers(size);
  month_ = cpp11::writable::integers(size);
  day_ = cpp11::writable::integers(size);
}

inline
cpp11::writable::list_of<cpp11::writable::integers>
year_month_day::to_list() const
{
  cpp11::writable::list_of<cpp11::writable::integers> out(
      {year_, month_, day_}
  );

  out.names() = {"year", "month", "day"};

  return out;
}

inline
void
year_month_day::assign_na(r_ssize i) NOEXCEPT
{
  year_[i] = r_int_na;
  month_[i] = r_int_na;
  day_[i] = r_int_na;
}

inline
void
year_month_day::assign(const date::year_month_day& ymd, r_ssize i) NOEXCEPT
{
  year_[i] = static_cast<int>(ymd.year());
  month_[i] = static_cast<int>(static_cast<unsigned>(ymd.month()));
  day_[i] = static_cast<int>(static_cast<unsigned>(ymd.day()));
}

} // namespace writable

} // namespace rclock

#endif
