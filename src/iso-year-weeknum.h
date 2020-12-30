#ifndef CLOCK_ISO_YEAR_WEEKNUM_H
#define CLOCK_ISO_YEAR_WEEKNUM_H

#include "clock.h"

namespace rclock {

class iso_year_weeknum
{
  const cpp11::integers& year_;
  const cpp11::integers& weeknum_;

public:
  CONSTCD11 iso_year_weeknum(const cpp11::integers& year,
                             const cpp11::integers& weeknum) NOEXCEPT;

  bool is_na(r_ssize i) const NOEXCEPT;
  CONSTCD11 r_ssize size() const NOEXCEPT;

  CONSTCD11 iso_week::year_weeknum operator[](r_ssize i) const NOEXCEPT;
};

CONSTCD11
inline
iso_year_weeknum::iso_year_weeknum(const cpp11::integers& year,
                                   const cpp11::integers& weeknum) NOEXCEPT
  : year_(year),
    weeknum_(weeknum)
  {}

inline
bool
iso_year_weeknum::is_na(r_ssize i) const NOEXCEPT
{
  return year_[i] == r_int_na;
}

CONSTCD11
inline
r_ssize
iso_year_weeknum::size() const NOEXCEPT
{
  return year_.size();
}

CONSTCD11
inline
iso_week::year_weeknum
iso_year_weeknum::operator[](r_ssize i) const NOEXCEPT
{
  return iso_week::year{year_[i]} /
    iso_week::weeknum{static_cast<unsigned>(weeknum_[i])};
}

namespace writable {

class iso_year_weeknum
{
  cpp11::writable::integers year_;
  cpp11::writable::integers weeknum_;

public:
  iso_year_weeknum(r_ssize size);

  cpp11::writable::list_of<cpp11::writable::integers> to_list() const;

  void assign_na(r_ssize i) NOEXCEPT;
  void assign(const iso_week::year_weeknum& x, r_ssize i) NOEXCEPT;
};

inline
iso_year_weeknum::iso_year_weeknum(r_ssize size)
{
  year_ = cpp11::writable::integers(size);
  weeknum_ = cpp11::writable::integers(size);
}

inline
cpp11::writable::list_of<cpp11::writable::integers>
iso_year_weeknum::to_list() const
{
  cpp11::writable::list_of<cpp11::writable::integers> out(
      {year_, weeknum_}
  );

  out.names() = {"year", "weeknum"};

  return out;
}

inline
void
iso_year_weeknum::assign_na(r_ssize i) NOEXCEPT
{
  year_[i] = r_int_na;
  weeknum_[i] = r_int_na;
}

inline
void
iso_year_weeknum::assign(const iso_week::year_weeknum& x, r_ssize i) NOEXCEPT
{
  year_[i] = static_cast<int>(x.year());
  weeknum_[i] = static_cast<int>(static_cast<unsigned>(x.weeknum()));
}

} // namespace writable

} // namespace rclock

#endif
