#ifndef CLOCK_YEAR_MONTH_WEEKDAY_H
#define CLOCK_YEAR_MONTH_WEEKDAY_H

#include "clock.h"

namespace rclock {

class year_month_weekday
{
  const cpp11::integers& year_;
  const cpp11::integers& month_;
  const cpp11::integers& weekday_;
  const cpp11::integers& index_;

public:
  CONSTCD11 year_month_weekday(const cpp11::integers& year,
                               const cpp11::integers& month,
                               const cpp11::integers& weekday,
                               const cpp11::integers& index) NOEXCEPT;

  bool is_na(r_ssize i) const NOEXCEPT;
  CONSTCD11 r_ssize size() const NOEXCEPT;

  CONSTCD11 date::year_month_weekday operator[](r_ssize i) const NOEXCEPT;
};

CONSTCD11
inline
year_month_weekday::year_month_weekday(const cpp11::integers& year,
                                       const cpp11::integers& month,
                                       const cpp11::integers& weekday,
                                       const cpp11::integers& index) NOEXCEPT
  : year_(year),
    month_(month),
    weekday_(weekday),
    index_(index)
  {}

inline
bool
year_month_weekday::is_na(r_ssize i) const NOEXCEPT
{
  return year_[i] == r_int_na;
}

CONSTCD11
inline
r_ssize
year_month_weekday::size() const NOEXCEPT
{
  return year_.size();
}

CONSTCD11
inline
date::year_month_weekday
year_month_weekday::operator[](r_ssize i) const NOEXCEPT
{
  return date::year{year_[i]} /
    date::month{static_cast<unsigned>(month_[i])} /
    date::weekday{static_cast<unsigned>(weekday_[i] - 1)}[static_cast<unsigned>(index_[i])];
}

namespace writable {

class year_month_weekday
{
  cpp11::writable::integers year_;
  cpp11::writable::integers month_;
  cpp11::writable::integers weekday_;
  cpp11::writable::integers index_;

public:
  year_month_weekday(r_ssize size);

  cpp11::writable::list_of<cpp11::writable::integers> to_list() const;

  void assign_na(r_ssize i) NOEXCEPT;
  void assign(const date::year_month_weekday& x, r_ssize i) NOEXCEPT;
};

inline
year_month_weekday::year_month_weekday(r_ssize size)
 {
  year_ = cpp11::writable::integers(size);
  month_ = cpp11::writable::integers(size);
  weekday_ = cpp11::writable::integers(size);
  index_ = cpp11::writable::integers(size);
}

inline
cpp11::writable::list_of<cpp11::writable::integers>
year_month_weekday::to_list() const
{
  cpp11::writable::list_of<cpp11::writable::integers> out(
      {year_, month_, weekday_, index_}
  );

  out.names() = {"year", "month", "weekday", "index"};

  return out;
}

inline
void
year_month_weekday::assign_na(r_ssize i) NOEXCEPT
{
  year_[i] = r_int_na;
  month_[i] = r_int_na;
  weekday_[i] = r_int_na;
  index_[i] = r_int_na;
}

inline
void
year_month_weekday::assign(const date::year_month_weekday& x, r_ssize i) NOEXCEPT
{
  year_[i] = static_cast<int>(x.year());
  month_[i] = static_cast<int>(static_cast<unsigned>(x.month()));
  weekday_[i] = x.weekday().c_encoding() + 1;
  index_[i] = static_cast<int>(x.index());
}

} // namespace writable

} // namespace rclock

#endif
