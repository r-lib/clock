#ifndef CLOCK_ISO_YEAR_WEEKNUM_WEEKDAY_H
#define CLOCK_ISO_YEAR_WEEKNUM_WEEKDAY_H

#include "clock.h"

namespace rclock {

class iso_year_weeknum_weekday
{
  const cpp11::integers& year_;
  const cpp11::integers& weeknum_;
  const cpp11::integers& weekday_;

public:
  CONSTCD11 iso_year_weeknum_weekday(const cpp11::integers& year,
                                     const cpp11::integers& weeknum,
                                     const cpp11::integers& weekday) NOEXCEPT;

  bool is_na(r_ssize i) const NOEXCEPT;
  CONSTCD11 r_ssize size() const NOEXCEPT;

  CONSTCD11 iso_week::year_weeknum_weekday operator[](r_ssize i) const NOEXCEPT;
};

CONSTCD11
inline
iso_year_weeknum_weekday::iso_year_weeknum_weekday(const cpp11::integers& year,
                                                   const cpp11::integers& weeknum,
                                                   const cpp11::integers& weekday) NOEXCEPT
  : year_(year),
    weeknum_(weeknum),
    weekday_(weekday)
  {}

inline
bool
iso_year_weeknum_weekday::is_na(r_ssize i) const NOEXCEPT
{
  return year_[i] == r_int_na;
}

CONSTCD11
inline
r_ssize
iso_year_weeknum_weekday::size() const NOEXCEPT
{
  return year_.size();
}

CONSTCD11
inline
iso_week::year_weeknum_weekday
iso_year_weeknum_weekday::operator[](r_ssize i) const NOEXCEPT
{
  return iso_week::year{year_[i]} /
    iso_week::weeknum{static_cast<unsigned>(weeknum_[i])} /
    iso_week::weekday{static_cast<unsigned>(weekday_[i])};
}

namespace writable {

class iso_year_weeknum_weekday
{
  cpp11::writable::integers year_;
  cpp11::writable::integers weeknum_;
  cpp11::writable::integers weekday_;

public:
  iso_year_weeknum_weekday(r_ssize size);

  cpp11::writable::list_of<cpp11::writable::integers> to_list() const;

  void assign_na(r_ssize i) NOEXCEPT;
  void assign(const iso_week::year_weeknum_weekday& x, r_ssize i) NOEXCEPT;
};

inline
iso_year_weeknum_weekday::iso_year_weeknum_weekday(r_ssize size)
{
  year_ = cpp11::writable::integers(size);
  weeknum_ = cpp11::writable::integers(size);
  weekday_ = cpp11::writable::integers(size);
}

inline
cpp11::writable::list_of<cpp11::writable::integers>
iso_year_weeknum_weekday::to_list() const
{
  cpp11::writable::list_of<cpp11::writable::integers> out(
      {year_, weeknum_, weekday_}
  );

  out.names() = {"year", "weeknum", "weekday"};

  return out;
}

inline
void
iso_year_weeknum_weekday::assign_na(r_ssize i) NOEXCEPT
{
  year_[i] = r_int_na;
  weeknum_[i] = r_int_na;
  weekday_[i] = r_int_na;
}

inline
void
iso_year_weeknum_weekday::assign(const iso_week::year_weeknum_weekday& x, r_ssize i) NOEXCEPT
{
  year_[i] = static_cast<int>(x.year());
  weeknum_[i] = static_cast<int>(static_cast<unsigned>(x.weeknum()));
  weekday_[i] = static_cast<int>(static_cast<unsigned>(x.weekday()));
}

} // namespace writable

} // namespace rclock

#endif
