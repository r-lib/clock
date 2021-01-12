#ifndef CLOCK_YEAR_QUARTERNUM_QUARTERDAY_H
#define CLOCK_YEAR_QUARTERNUM_QUARTERDAY_H

#include "clock.h"

namespace rclock {

template <quarterly::start S>
class year_quarternum_quarterday
{
  const cpp11::integers& year_;
  const cpp11::integers& quarternum_;
  const cpp11::integers& quarterday_;

public:
  CONSTCD11 year_quarternum_quarterday(const cpp11::integers& year,
                                       const cpp11::integers& quarternum,
                                       const cpp11::integers& quarterday) NOEXCEPT;

  bool is_na(r_ssize i) const NOEXCEPT;
  CONSTCD11 r_ssize size() const NOEXCEPT;

  CONSTCD11 quarterly::year_quarternum_quarterday<S> operator[](r_ssize i) const NOEXCEPT;
};

template <quarterly::start S>
CONSTCD11
inline
year_quarternum_quarterday<S>::year_quarternum_quarterday(const cpp11::integers& year,
                                                          const cpp11::integers& quarternum,
                                                          const cpp11::integers& quarterday) NOEXCEPT
  : year_(year),
    quarternum_(quarternum),
    quarterday_(quarterday)
  {}

template <quarterly::start S>
inline
bool
year_quarternum_quarterday<S>::is_na(r_ssize i) const NOEXCEPT
{
  return year_[i] == r_int_na;
}

template <quarterly::start S>
CONSTCD11
inline
r_ssize
year_quarternum_quarterday<S>::size() const NOEXCEPT
{
  return year_.size();
}

template <quarterly::start S>
CONSTCD11
inline
quarterly::year_quarternum_quarterday<S>
year_quarternum_quarterday<S>::operator[](r_ssize i) const NOEXCEPT
{
  return quarterly::year<S>{year_[i]} /
    quarterly::quarternum{static_cast<unsigned>(quarternum_[i])} /
    quarterly::quarterday{static_cast<unsigned>(quarterday_[i])};
}

namespace writable {

template <quarterly::start S>
class year_quarternum_quarterday
{
  cpp11::writable::integers year_;
  cpp11::writable::integers quarternum_;
  cpp11::writable::integers quarterday_;

public:
  year_quarternum_quarterday(r_ssize size);

  cpp11::writable::list_of<cpp11::writable::integers> to_list() const;

  void assign_na(r_ssize i) NOEXCEPT;
  void assign(const quarterly::year_quarternum_quarterday<S>& x, r_ssize i) NOEXCEPT;
};

template <quarterly::start S>
inline
year_quarternum_quarterday<S>::year_quarternum_quarterday(r_ssize size)
{
  year_ = cpp11::writable::integers(size);
  quarternum_ = cpp11::writable::integers(size);
  quarterday_ = cpp11::writable::integers(size);
}

template <quarterly::start S>
inline
cpp11::writable::list_of<cpp11::writable::integers>
year_quarternum_quarterday<S>::to_list() const
{
  cpp11::writable::list_of<cpp11::writable::integers> out(
      {year_, quarternum_, quarterday_}
  );

  out.names() = {"year", "quarternum", "quarterday"};

  return out;
}

template <quarterly::start S>
inline
void
year_quarternum_quarterday<S>::assign_na(r_ssize i) NOEXCEPT
{
  year_[i] = r_int_na;
  quarternum_[i] = r_int_na;
  quarterday_[i] = r_int_na;
}

template <quarterly::start S>
inline
void
year_quarternum_quarterday<S>::assign(const quarterly::year_quarternum_quarterday<S>& x, r_ssize i) NOEXCEPT
{
  year_[i] = static_cast<int>(x.year());
  quarternum_[i] = static_cast<int>(static_cast<unsigned>(x.quarternum()));
  quarterday_[i] = static_cast<int>(static_cast<unsigned>(x.quarterday()));
}

} // namespace writable

} // namespace rclock

#endif
