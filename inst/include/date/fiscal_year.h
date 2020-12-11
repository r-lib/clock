#ifndef FISCAL_YEAR_H
#define FISCAL_YEAR_H

// The MIT License (MIT)
//
// For the original `date.h` and `iso_week.h` implementations:
// Copyright (c) 2015, 2016, 2017 Howard Hinnant
// For the `fiscal_year.h` extension:
// Copyright (c) 2020 Davis Vaughan
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "date.h"

#include <climits>

namespace fiscal_year
{

using days = date::days;
using years = date::years;

using quarters = std::chrono::duration
    <int, date::detail::ratio_divide<years::period, std::ratio<4>>>;

// time_point

using sys_days = date::sys_days;
using local_days = date::local_days;

// types

struct last_spec
{
    explicit last_spec() = default;
};

enum class start: unsigned char {
    january = 1u,
    february = 2u,
    march = 3u,
    april = 4u,
    may = 5u,
    june = 6u,
    july = 7u,
    august = 8u,
    september = 9u,
    october = 10u,
    november = 11u,
    december = 12u
};

class day;
class quarternum;
template <start S>
class year;

template <start S>
class year_quarternum;
class quarternum_day;
class quarternum_day_last;

template <start S>
class year_quarternum_day;
template <start S>
class year_quarternum_day_last;

// date composition operators

template <start S>
CONSTCD11 year_quarternum<S> operator/(const year<S>& y, const quarternum& qn) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum<S> operator/(const year<S>& y, int               qn) NOEXCEPT;

CONSTCD11 quarternum_day operator/(const quarternum& qn, const day& d) NOEXCEPT;
CONSTCD11 quarternum_day operator/(const quarternum& qn, int        d) NOEXCEPT;
CONSTCD11 quarternum_day operator/(int               qn, const day& d) NOEXCEPT;
CONSTCD11 quarternum_day operator/(const day& d, const quarternum& qn) NOEXCEPT;
CONSTCD11 quarternum_day operator/(const day& d, int               qn) NOEXCEPT;

CONSTCD11 quarternum_day_last operator/(const quarternum& qn, last_spec) NOEXCEPT;
CONSTCD11 quarternum_day_last operator/(int               qn, last_spec) NOEXCEPT;
CONSTCD11 quarternum_day_last operator/(last_spec, const quarternum& qn) NOEXCEPT;
CONSTCD11 quarternum_day_last operator/(last_spec, int               qn) NOEXCEPT;

template <start S>
CONSTCD11 year_quarternum_day<S> operator/(const year_quarternum<S>& yqn, const day& d) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_day<S> operator/(const year_quarternum<S>& yqn, int        d) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_day<S> operator/(const year<S>& y, const quarternum_day& qnd) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_day<S> operator/(const quarternum_day& qnd, const year<S>& y) NOEXCEPT;

template <start S>
CONSTCD11 year_quarternum_day_last<S> operator/(const year_quarternum<S>& yqn, last_spec) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_day_last<S> operator/(const year<S>& y, const quarternum_day_last& qndl) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_day_last<S> operator/(const quarternum_day_last& qndl, const year<S>& y) NOEXCEPT;

// day

class day
{
    unsigned char d_;
public:
    day() = default;
    explicit CONSTCD11 day(unsigned d) NOEXCEPT;
    explicit day(int) = delete;

    CONSTCD14 day& operator++()    NOEXCEPT;
    CONSTCD14 day  operator++(int) NOEXCEPT;
    CONSTCD14 day& operator--()    NOEXCEPT;
    CONSTCD14 day  operator--(int) NOEXCEPT;

    CONSTCD14 day& operator+=(const days& dd) NOEXCEPT;
    CONSTCD14 day& operator-=(const days& dd) NOEXCEPT;

    CONSTCD11 explicit operator unsigned() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const day& x, const day& y) NOEXCEPT;
CONSTCD11 bool operator!=(const day& x, const day& y) NOEXCEPT;
CONSTCD11 bool operator< (const day& x, const day& y) NOEXCEPT;
CONSTCD11 bool operator> (const day& x, const day& y) NOEXCEPT;
CONSTCD11 bool operator<=(const day& x, const day& y) NOEXCEPT;
CONSTCD11 bool operator>=(const day& x, const day& y) NOEXCEPT;

CONSTCD11 day  operator+(const day&  x, const days& y) NOEXCEPT;
CONSTCD11 day  operator+(const days& x, const day&  y) NOEXCEPT;
CONSTCD11 day  operator-(const day&  x, const days& y) NOEXCEPT;
CONSTCD11 days operator-(const day&  x, const day&  y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const day& d);

// quarternum

class quarternum
{
    unsigned char qn_;

public:
    quarternum() = default;
    explicit CONSTCD11 quarternum(unsigned qn) NOEXCEPT;
    explicit quarternum(int) = delete;

    CONSTCD14 quarternum& operator++()    NOEXCEPT;
    CONSTCD14 quarternum  operator++(int) NOEXCEPT;
    CONSTCD14 quarternum& operator--()    NOEXCEPT;
    CONSTCD14 quarternum  operator--(int) NOEXCEPT;

    CONSTCD14 quarternum& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 quarternum& operator-=(const quarters& dq) NOEXCEPT;

    CONSTCD11 explicit operator unsigned() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const quarternum& x, const quarternum& y) NOEXCEPT;
CONSTCD11 bool operator!=(const quarternum& x, const quarternum& y) NOEXCEPT;
CONSTCD11 bool operator< (const quarternum& x, const quarternum& y) NOEXCEPT;
CONSTCD11 bool operator> (const quarternum& x, const quarternum& y) NOEXCEPT;
CONSTCD11 bool operator<=(const quarternum& x, const quarternum& y) NOEXCEPT;
CONSTCD11 bool operator>=(const quarternum& x, const quarternum& y) NOEXCEPT;

CONSTCD11 quarternum  operator+(const quarternum&  x, const quarters&   y) NOEXCEPT;
CONSTCD11 quarternum  operator+(const quarters&    x, const quarternum& y) NOEXCEPT;
CONSTCD11 quarternum  operator-(const quarternum&  x, const quarters&   y) NOEXCEPT;
CONSTCD11 quarters    operator-(const quarternum&  x, const quarternum& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarternum& qn);

// year

template <start S>
class year
{
    short y_;

public:
    year<S>() = default;
    explicit CONSTCD11 year<S>(int y) NOEXCEPT;

    CONSTCD14 year<S>& operator++()    NOEXCEPT;
    CONSTCD14 year<S>  operator++(int) NOEXCEPT;
    CONSTCD14 year<S>& operator--()    NOEXCEPT;
    CONSTCD14 year<S>  operator--(int) NOEXCEPT;

    CONSTCD14 year<S>& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year<S>& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 explicit operator int() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;

    CONSTCD11 fiscal_year::start start() const NOEXCEPT;

    CONSTCD11 bool is_leap() const NOEXCEPT;

    CONSTCD11 year<S> min() const NOEXCEPT;
    CONSTCD11 year<S> max() const NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year<S>& x, const year<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year<S>& x, const year<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year<S>& x, const year<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year<S>& x, const year<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year<S>& x, const year<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year<S>& x, const year<S>& y) NOEXCEPT;

template <start S>
CONSTCD11 year<S> operator+(const year<S>& x, const years& y) NOEXCEPT;
template <start S>
CONSTCD11 year<S> operator+(const years& x, const year<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 year<S> operator-(const year<S>& x, const years& y) NOEXCEPT;
template <start S>
CONSTCD11 years operator-(const year<S>& x, const year<S>& y) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year<S>& y);

// year_quarternum

template <start S>
class year_quarternum
{
    fiscal_year::year<S> y_;
    fiscal_year::quarternum qn_;

public:
    year_quarternum<S>() = default;
    CONSTCD11 year_quarternum<S>(const fiscal_year::year<S>& y,
                                 const fiscal_year::quarternum& qn) NOEXCEPT;

    CONSTCD11 fiscal_year::year<S> year() const NOEXCEPT;
    CONSTCD11 fiscal_year::quarternum quarternum() const NOEXCEPT;

    CONSTCD14 year_quarternum<S>& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarternum<S>& operator-=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarternum<S>& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarternum<S>& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 bool ok() const NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT;

template <start S>
CONSTCD14 year_quarternum<S> operator+(const year_quarternum<S>& yqn, const quarters& dq) NOEXCEPT;
template <start S>
CONSTCD14 year_quarternum<S> operator+(const quarters& dq, const year_quarternum<S>& yqn) NOEXCEPT;
template <start S>
CONSTCD14 year_quarternum<S> operator-(const year_quarternum<S>& yqn, const quarters& dq) NOEXCEPT;

template <start S>
CONSTCD11 year_quarternum<S> operator+(const year_quarternum<S>& yqn, const years& dy) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum<S> operator+(const years& dy, const year_quarternum<S>& yqn) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum<S> operator-(const year_quarternum<S>& yqn, const years& dy) NOEXCEPT;

template <start S>
CONSTCD11 quarters operator-(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarternum<S>& yqn);

// quarternum_day

class quarternum_day
{
    fiscal_year::quarternum qn_;
    fiscal_year::day d_;

public:
    quarternum_day() = default;
    CONSTCD11 quarternum_day(const fiscal_year::quarternum& qn,
                             const fiscal_year::day& d) NOEXCEPT;

    CONSTCD11 fiscal_year::quarternum quarternum() const NOEXCEPT;
    CONSTCD11 fiscal_year::day day() const NOEXCEPT;

    CONSTCD14 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const quarternum_day& x, const quarternum_day& y) NOEXCEPT;
CONSTCD11 bool operator!=(const quarternum_day& x, const quarternum_day& y) NOEXCEPT;
CONSTCD11 bool operator< (const quarternum_day& x, const quarternum_day& y) NOEXCEPT;
CONSTCD11 bool operator> (const quarternum_day& x, const quarternum_day& y) NOEXCEPT;
CONSTCD11 bool operator<=(const quarternum_day& x, const quarternum_day& y) NOEXCEPT;
CONSTCD11 bool operator>=(const quarternum_day& x, const quarternum_day& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarternum_day& qnd);

// quarternum_day_last

class quarternum_day_last
{
    fiscal_year::quarternum qn_;

public:
    quarternum_day_last() = default;
    CONSTCD11 explicit quarternum_day_last(const fiscal_year::quarternum& qn) NOEXCEPT;

    CONSTCD11 fiscal_year::quarternum quarternum() const NOEXCEPT;

    CONSTCD14 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT;
CONSTCD11 bool operator!=(const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT;
CONSTCD11 bool operator< (const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT;
CONSTCD11 bool operator> (const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT;
CONSTCD11 bool operator<=(const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT;
CONSTCD11 bool operator>=(const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarternum_day_last& qndl);

// year_quarternum_day_last

template <start S>
class year_quarternum_day_last
{
    fiscal_year::year<S> y_;
    fiscal_year::quarternum qn_;

public:
    year_quarternum_day_last<S>() = default;
    CONSTCD11 year_quarternum_day_last<S>(const fiscal_year::year<S>& y,
                                          const fiscal_year::quarternum& qn) NOEXCEPT;
    CONSTCD11 year_quarternum_day_last<S>(const fiscal_year::year_quarternum<S>& yqn) NOEXCEPT;

    CONSTCD14 year_quarternum_day_last<S>& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarternum_day_last<S>& operator-=(const quarters& dq) NOEXCEPT;

    CONSTCD14 year_quarternum_day_last<S>& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarternum_day_last<S>& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 fiscal_year::year<S> year() const NOEXCEPT;
    CONSTCD11 fiscal_year::quarternum quarternum() const NOEXCEPT;
    CONSTCD14 fiscal_year::day day() const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT;

template <start S>
CONSTCD14 year_quarternum_day_last<S> operator+(const year_quarternum_day_last<S>& yqndl, const quarters& dq) NOEXCEPT;
template <start S>
CONSTCD14 year_quarternum_day_last<S> operator+(const quarters& dq, const year_quarternum_day_last<S>& yqndl) NOEXCEPT;
template <start S>
CONSTCD14 year_quarternum_day_last<S> operator-(const year_quarternum_day_last<S>& yqndl, const quarters& dq) NOEXCEPT;

template <start S>
CONSTCD11 year_quarternum_day_last<S> operator+(const year_quarternum_day_last<S>& yqndl, const years& dy) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_day_last<S> operator+(const years& dy, const year_quarternum_day_last<S>& yqndl) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_day_last<S> operator-(const year_quarternum_day_last<S>& yqndl, const years& dy) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarternum_day_last<S>& yqndl);

// year_quarternum_day

template <start S>
class year_quarternum_day
{
    fiscal_year::year<S> y_;
    fiscal_year::quarternum qn_;
    fiscal_year::day d_;

public:
    year_quarternum_day<S>() = default;
    CONSTCD11 year_quarternum_day<S>(const fiscal_year::year<S>& y,
                                     const fiscal_year::quarternum& qn,
                                     const fiscal_year::day& d) NOEXCEPT;
    CONSTCD11 year_quarternum_day<S>(const fiscal_year::year_quarternum<S>& yqn,
                                     const fiscal_year::day& d) NOEXCEPT;
    CONSTCD14 year_quarternum_day<S>(const year_quarternum_day_last<S>& yqndl) NOEXCEPT;
    CONSTCD14 year_quarternum_day<S>(const sys_days& dp) NOEXCEPT;
    CONSTCD14 year_quarternum_day<S>(const local_days& dp) NOEXCEPT;

    CONSTCD14 year_quarternum_day<S>& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarternum_day<S>& operator-=(const quarters& dq) NOEXCEPT;

    CONSTCD14 year_quarternum_day<S>& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarternum_day<S>& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 fiscal_year::year<S> year() const NOEXCEPT;
    CONSTCD11 fiscal_year::quarternum quarternum() const NOEXCEPT;
    CONSTCD11 fiscal_year::day day() const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD14 bool ok() const NOEXCEPT;

private:
    CONSTCD14 days to_days() const NOEXCEPT;
    static CONSTCD14 year_quarternum_day<S> from_days(const days& dd) NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT;

template <start S>
CONSTCD14 year_quarternum_day<S> operator+(const year_quarternum_day<S>& yqnd, const quarters& dq) NOEXCEPT;
template <start S>
CONSTCD14 year_quarternum_day<S> operator+(const quarters& dq, const year_quarternum_day<S>& yqnd) NOEXCEPT;
template <start S>
CONSTCD14 year_quarternum_day<S> operator-(const year_quarternum_day<S>& yqnd, const quarters& dq) NOEXCEPT;

template <start S>
CONSTCD11 year_quarternum_day<S> operator+(const year_quarternum_day<S>& yqnd, const years& dy) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_day<S> operator+(const years& dy, const year_quarternum_day<S>& yqnd) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_day<S> operator-(const year_quarternum_day<S>& yqnd, const years& dy) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarternum_day<S>& yqnd);

//----------------+
// Implementation |
//----------------+

// day

CONSTCD11
inline
day::day(unsigned d) NOEXCEPT
    : d_(static_cast<decltype(d_)>(d))
    {}

CONSTCD14
inline
day&
day::operator++() NOEXCEPT
{
    ++d_;
    return *this;
}

CONSTCD14
inline
day
day::operator++(int) NOEXCEPT
{
    auto tmp(*this);
    ++(*this);
    return tmp;
}

CONSTCD14
inline
day&
day::operator--() NOEXCEPT
{
    --d_;
    return *this;
}

CONSTCD14
inline
day
day::operator--(int) NOEXCEPT
{
    auto tmp(*this);
    --(*this);
    return tmp;
}

CONSTCD14
inline
day&
day::operator+=(const days& dd) NOEXCEPT
{
    *this = *this + dd;
    return *this;
}

CONSTCD14
inline
day&
day::operator-=(const days& dd) NOEXCEPT
{
    *this = *this - dd;
    return *this;
}

CONSTCD11
inline
day::operator unsigned() const NOEXCEPT
{
    return d_;
}

CONSTCD11
inline
bool
day::ok() const NOEXCEPT
{
    // 92 from a quarter triplet with [31, 31, 30] days
    return 1 <= d_ && d_ <= 92;
}

CONSTCD11
inline
bool
operator==(const day& x, const day& y) NOEXCEPT
{
    return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator!=(const day& x, const day& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const day& x, const day& y) NOEXCEPT
{
    return static_cast<unsigned>(x) < static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator>(const day& x, const day& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const day& x, const day& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const day& x, const day& y) NOEXCEPT
{
    return !(x < y);
}

CONSTCD11
inline
days
operator-(const day& x, const day& y) NOEXCEPT
{
    return days{static_cast<days::rep>(static_cast<unsigned>(x) - static_cast<unsigned>(y))};
}

CONSTCD11
inline
day
operator+(const day& x, const days& y) NOEXCEPT
{
    return day{static_cast<unsigned>(x) + static_cast<unsigned>(y.count())};
}

CONSTCD11
inline
day
operator+(const days& x, const day& y) NOEXCEPT
{
    return y + x;
}

CONSTCD11
inline
day
operator-(const day& x, const days& y) NOEXCEPT
{
    return x + -y;
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const day& d)
{
    date::detail::save_ostream<CharT, Traits> _(os);
    os.fill('0');
    os.flags(std::ios::dec | std::ios::right);
    os.width(2);
    os << static_cast<unsigned>(d);
    if (!d.ok())
        os << " is not a valid day of the quarter";
    return os;
}

// quarternum

CONSTCD11
inline
quarternum::quarternum(unsigned qn) NOEXCEPT
    : qn_(static_cast<decltype(qn_)>(qn))
    {}

CONSTCD14
inline
quarternum&
quarternum::operator++() NOEXCEPT
{
    ++qn_;
    return *this;
}

CONSTCD14
inline
quarternum
quarternum::operator++(int) NOEXCEPT
{
    auto tmp(*this);
    ++(*this);
    return tmp;
}

CONSTCD14
inline
quarternum&
quarternum::operator--() NOEXCEPT
{
    --qn_;
    return *this;
}

CONSTCD14
inline
quarternum
quarternum::operator--(int) NOEXCEPT
{
    auto tmp(*this);
    --(*this);
    return tmp;
}

CONSTCD14
inline
quarternum&
quarternum::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

CONSTCD14
inline
quarternum&
quarternum::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

CONSTCD11
inline
quarternum::operator unsigned() const NOEXCEPT
{
    return qn_;
}

CONSTCD11
inline
bool
quarternum::ok() const NOEXCEPT
{
    return 1 <= qn_ && qn_ <= 4;
}

CONSTCD11
inline
bool
operator==(const quarternum& x, const quarternum& y) NOEXCEPT
{
    return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator!=(const quarternum& x, const quarternum& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const quarternum& x, const quarternum& y) NOEXCEPT
{
    return static_cast<unsigned>(x) < static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator>(const quarternum& x, const quarternum& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const quarternum& x, const quarternum& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const quarternum& x, const quarternum& y) NOEXCEPT
{
    return !(x < y);
}

CONSTCD11
inline
quarters
operator-(const quarternum& x, const quarternum& y) NOEXCEPT
{
    return quarters{static_cast<quarters::rep>(static_cast<unsigned>(x) - static_cast<unsigned>(y))};
}

CONSTCD11
inline
quarternum
operator+(const quarternum& x, const quarters& y) NOEXCEPT
{
    return quarternum{static_cast<unsigned>(x) + static_cast<unsigned>(y.count())};
}

CONSTCD11
inline
quarternum
operator+(const quarters& x, const quarternum& y) NOEXCEPT
{
    return y + x;
}

CONSTCD11
inline
quarternum
operator-(const quarternum& x, const quarters& y) NOEXCEPT
{
    return x + -y;
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarternum& qn)
{
    date::detail::save_ostream<CharT, Traits> _(os);
    os.flags(std::ios::dec | std::ios::right);
    os.width(1);
    os << 'Q' << static_cast<unsigned>(qn);
    if (!qn.ok())
        os << " is not a valid quarter number";
    return os;
}

// year

template <start S>
CONSTCD11
inline
year<S>::year(int y) NOEXCEPT
    : y_(static_cast<decltype(y_)>(y))
    {}

template <start S>
CONSTCD14
inline
year<S>&
year<S>::operator++() NOEXCEPT
{
    ++y_;
    return *this;
}

template <start S>
CONSTCD14
inline
year<S>
year<S>::operator++(int) NOEXCEPT
{
    auto tmp(*this);
    ++(*this);
    return tmp;
}

template <start S>
CONSTCD14
inline
year<S>&
year<S>::operator--() NOEXCEPT
{
    --y_;
    return *this;
}

template <start S>
CONSTCD14
inline
year<S>
year<S>::operator--(int) NOEXCEPT
{
    auto tmp(*this);
    --(*this);
    return tmp;
}

template <start S>
CONSTCD14
inline
year<S>&
year<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
CONSTCD14
inline
year<S>&
year<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD11
inline
year<S>::operator int() const NOEXCEPT
{
    return y_;
}

template <start S>
CONSTCD11
inline
bool
year<S>::ok() const NOEXCEPT
{
    return y_ != std::numeric_limits<short>::min();
}

template <start S>
CONSTCD11
inline
fiscal_year::start
year<S>::start() const NOEXCEPT
{
    return S;
}

template <start S>
CONSTCD11
inline
bool
year<S>::is_leap() const NOEXCEPT
{
    return date::year{y_}.is_leap();
}

template <>
CONSTCD11
inline
bool
year<start::february>::is_leap() const NOEXCEPT
{
    return date::year{y_ - 1}.is_leap();
}

template <start S>
CONSTCD11
inline
bool
operator==(const year<S>& x, const year<S>& y) NOEXCEPT
{
    return static_cast<int>(x) == static_cast<int>(y);
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year<S>& x, const year<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year<S>& x, const year<S>& y) NOEXCEPT
{
    return static_cast<int>(x) < static_cast<int>(y);
}

template <start S>
CONSTCD11
inline
bool
operator>(const year<S>& x, const year<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year<S>& x, const year<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year<S>& x, const year<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD11
inline
years
operator-(const year<S>& x, const year<S>& y) NOEXCEPT
{
    return years{static_cast<int>(x) - static_cast<int>(y)};
}

template <start S>
CONSTCD11
inline
year<S>
operator+(const year<S>& x, const years& y) NOEXCEPT
{
    return year<S>{static_cast<int>(x) + y.count()};
}

template <start S>
CONSTCD11
inline
year<S>
operator+(const years& x, const year<S>& y) NOEXCEPT
{
    return y + x;
}

template <start S>
CONSTCD11
inline
year<S>
operator-(const year<S>& x, const years& y) NOEXCEPT
{
    return year<S>{static_cast<int>(x) - y.count()};
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year<S>& y)
{
    date::detail::save_ostream<CharT, Traits> _(os);
    os.fill('0');
    os.flags(std::ios::dec | std::ios::internal);
    os.width(4 + (y < year<S>{0}));
    os << static_cast<int>(y);
    return os;
}

// year_quarternum

template <start S>
CONSTCD11
inline
year_quarternum<S>::year_quarternum(const fiscal_year::year<S>& y,
                                    const fiscal_year::quarternum& qn) NOEXCEPT
    : y_(y)
    , qn_(qn)
    {}

template <start S>
CONSTCD11
inline
bool
year_quarternum<S>::ok() const NOEXCEPT
{
    return y_.ok() && qn_.ok();
}

template <start S>
CONSTCD11
inline
year<S>
year_quarternum<S>::year() const NOEXCEPT
{
    return y_;
}

template <start S>
CONSTCD11
inline
quarternum
year_quarternum<S>::quarternum() const NOEXCEPT
{
    return qn_;
}

template <start S>
CONSTCD14
inline
year_quarternum<S>&
year_quarternum<S>::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum<S>&
year_quarternum<S>::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum<S>&
year_quarternum<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum<S>&
year_quarternum<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT
{
    return x.year() == y.year() && x.quarternum() == y.quarternum();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.quarternum() < y.quarternum()));
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD14
inline
year_quarternum<S>
operator+(const year_quarternum<S>& yqn, const quarters& dq) NOEXCEPT
{
    auto dqi = static_cast<int>(static_cast<unsigned>(yqn.quarternum())) - 1 + dq.count();
    auto dy = (dqi >= 0 ? dqi : dqi - 3) / 4;
    dqi = dqi - dy * 4 + 1;
    return {(yqn.year() + years(dy)), quarternum(static_cast<unsigned>(dqi))};
}

template <start S>
CONSTCD14
inline
year_quarternum<S>
operator+(const quarters& dq, const year_quarternum<S>& yqn) NOEXCEPT
{
    return yqn + dq;
}

template <start S>
CONSTCD14
inline
year_quarternum<S>
operator-(const year_quarternum<S>& yqn, const quarters& dq) NOEXCEPT
{
    return yqn + -dq;
}

template <start S>
CONSTCD11
inline
year_quarternum<S>
operator+(const year_quarternum<S>& yqn, const years& dy) NOEXCEPT
{
    return {(yqn.year() + dy), yqn.quarternum()};
}

template <start S>
CONSTCD11
inline
year_quarternum<S>
operator+(const years& dy, const year_quarternum<S>& yqn) NOEXCEPT
{
    return yqn + dy;
}

template <start S>
CONSTCD11
inline
year_quarternum<S>
operator-(const year_quarternum<S>& yqn, const years& dy) NOEXCEPT
{
    return yqn + -dy;
}

template <start S>
CONSTCD11
inline
quarters
operator-(const year_quarternum<S>& x, const year_quarternum<S>& y) NOEXCEPT
{
    return (x.year() - y.year()) + (x.quarternum() - y.quarternum());
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarternum<S>& yqn)
{
    return os << yqn.year() << '-' << yqn.quarternum();
}

// quarternum_day

CONSTCD11
inline
quarternum_day::quarternum_day(const fiscal_year::quarternum& qn,
                               const fiscal_year::day& d) NOEXCEPT
    : qn_(qn)
    , d_(d)
    {}

CONSTCD11
inline
quarternum
quarternum_day::quarternum() const NOEXCEPT
{
    return qn_;
}

CONSTCD11
inline
day
quarternum_day::day() const NOEXCEPT
{
    return d_;
}

CONSTCD14
inline
bool
quarternum_day::ok() const NOEXCEPT
{
    return qn_.ok() && d_.ok();
}

CONSTCD11
inline
bool
operator==(const quarternum_day& x, const quarternum_day& y) NOEXCEPT
{
    return x.quarternum() == y.quarternum() && x.day() == y.day();
}

CONSTCD11
inline
bool
operator!=(const quarternum_day& x, const quarternum_day& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const quarternum_day& x, const quarternum_day& y) NOEXCEPT
{
    return x.quarternum() < y.quarternum() ? true
        : (x.quarternum() > y.quarternum() ? false
        : (static_cast<unsigned>(x.day()) < static_cast<unsigned>(y.day())));
}

CONSTCD11
inline
bool
operator>(const quarternum_day& x, const quarternum_day& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const quarternum_day& x, const quarternum_day& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const quarternum_day& x, const quarternum_day& y) NOEXCEPT
{
    return !(x < y);
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarternum_day& qnd)
{
    return os << qnd.quarternum() << '-' << qnd.day();
}

// quarternum_day_last

CONSTCD11
inline
quarternum_day_last::quarternum_day_last(const fiscal_year::quarternum& qn) NOEXCEPT
    : qn_(qn)
    {}

CONSTCD11
inline
quarternum
quarternum_day_last::quarternum() const NOEXCEPT
{
    return qn_;
}

CONSTCD14
inline
bool
quarternum_day_last::ok() const NOEXCEPT
{
    return qn_.ok();
}

CONSTCD11
inline
bool
operator==(const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT
{
    return x.quarternum() == y.quarternum();
}

CONSTCD11
inline
bool
operator!=(const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT
{
    return static_cast<unsigned>(x.quarternum()) < static_cast<unsigned>(y.quarternum());
}

CONSTCD11
inline
bool
operator>(const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const quarternum_day_last& x, const quarternum_day_last& y) NOEXCEPT
{
    return !(x < y);
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarternum_day_last& qnd)
{
    return os << qnd.quarternum() << "-last";
}

// year_quarternum_day_last

template <start S>
CONSTCD11
inline
year_quarternum_day_last<S>::year_quarternum_day_last(const fiscal_year::year<S>& y,
                                                      const fiscal_year::quarternum& qn) NOEXCEPT
    : y_(y)
    , qn_(qn)
    {}

template <start S>
CONSTCD11
inline
year_quarternum_day_last<S>::year_quarternum_day_last(const fiscal_year::year_quarternum<S>& yqn) NOEXCEPT
    : y_(yqn.year())
    , qn_(yqn.quarternum())
    {}

template <start S>
CONSTCD14
inline
year_quarternum_day_last<S>&
year_quarternum_day_last<S>::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_day_last<S>&
year_quarternum_day_last<S>::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_day_last<S>&
year_quarternum_day_last<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_day_last<S>&
year_quarternum_day_last<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD11
inline
year<S>
year_quarternum_day_last<S>::year() const NOEXCEPT
{
    return y_;
}

template <start S>
CONSTCD11
inline
quarternum
year_quarternum_day_last<S>::quarternum() const NOEXCEPT
{
    return qn_;
}

template <start S>
CONSTCD14
inline
day
year_quarternum_day_last<S>::day() const NOEXCEPT
{
    CONSTDATA unsigned s = static_cast<unsigned>(S) - 1;

    CONSTDATA fiscal_year::day days[] = {
        // [12, 1, 2]          [1, 2, 3]              [2, 3, 4]
        fiscal_year::day(90u), fiscal_year::day(90u), fiscal_year::day(89u),
        // [3, 4, 5]           [4, 5, 6]              [5, 6, 7]
        fiscal_year::day(92u), fiscal_year::day(91u), fiscal_year::day(92u),
        // [6, 7, 8]           [7, 8, 9]              [8, 9, 10]
        fiscal_year::day(92u), fiscal_year::day(92u), fiscal_year::day(92u),
        // [9, 10, 11]         [10, 11, 12]           [11, 12, 1]
        fiscal_year::day(91u), fiscal_year::day(92u), fiscal_year::day(92u)
    };

    const unsigned quarternum = static_cast<unsigned>(qn_) - 1;

    // Remap [Jan -> Dec] to [Dec -> Jan] to group quarters with February
    unsigned key = (s == 12) ? 0 : s + 1;

    key = key + 3 * quarternum;
    if (key > 11) {
        key -= 12;
    }

    if (!qn_.ok()) {
        // If `!qn_.ok()`, don't index into `days[]` to avoid OOB error.
        // Instead, return the minimum of the possible "last day of quarter"
        // days, as `year_month_day_last::day()` does.
        return days[2];
    } else if (key <= 2 && y_.is_leap()) {
        return days[key] + fiscal_year::days{1u};
    } else {
        return days[key];
    }
}

template <start S>
CONSTCD14
inline
year_quarternum_day_last<S>::operator sys_days() const NOEXCEPT
{
    return sys_days(year_quarternum_day<S>{year(), quarternum(), day()});
}

template <start S>
CONSTCD14
inline
year_quarternum_day_last<S>::operator local_days() const NOEXCEPT
{
    return local_days(year_quarternum_day<S>{year(), quarternum(), day()});
}

template <start S>
CONSTCD11
inline
bool
year_quarternum_day_last<S>::ok() const NOEXCEPT
{
    return y_.ok() && qn_.ok();
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT
{
    return x.year() == y.year() && x.quarternum() == y.quarternum();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : x.quarternum() < y.quarternum());
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_quarternum_day_last<S>& x, const year_quarternum_day_last<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD14
inline
year_quarternum_day_last<S>
operator+(const year_quarternum_day_last<S>& yqndl, const quarters& dq) NOEXCEPT
{
    return {year_quarternum<S>{yqndl.year(), yqndl.quarternum()} + dq};
}

template <start S>
CONSTCD14
inline
year_quarternum_day_last<S>
operator+(const quarters& dq, const year_quarternum_day_last<S>& yqndl)  NOEXCEPT
{
    return yqndl + dq;
}

template <start S>
CONSTCD14
inline
year_quarternum_day_last<S>
operator-(const year_quarternum_day_last<S>& yqndl, const quarters& dq)  NOEXCEPT
{
    return yqndl + -dq;
}

template <start S>
CONSTCD11
inline
year_quarternum_day_last<S>
operator+(const year_quarternum_day_last<S>& yqndl, const years& dy)  NOEXCEPT
{
    return {yqndl.year() + dy, yqndl.quarternum()};
}

template <start S>
CONSTCD11
inline
year_quarternum_day_last<S>
operator+(const years& dy, const year_quarternum_day_last<S>& yqndl)  NOEXCEPT
{
    return yqndl + dy;
}

template <start S>
CONSTCD11
inline
year_quarternum_day_last<S>
operator-(const year_quarternum_day_last<S>& yqndl, const years& dy)  NOEXCEPT
{
    return yqndl + -dy;
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarternum_day_last<S>& yqndl)
{
    return os << yqndl.year() << "-" << yqndl.quarternum() << "-last";
}

// year_quarternum_day

template <start S>
CONSTCD11
inline
year_quarternum_day<S>::year_quarternum_day(const fiscal_year::year<S>& y,
                                            const fiscal_year::quarternum& qn,
                                            const fiscal_year::day& d) NOEXCEPT
    : y_(y)
    , qn_(qn)
    , d_(d)
    {}

template <start S>
CONSTCD11
inline
year_quarternum_day<S>::year_quarternum_day(const fiscal_year::year_quarternum<S>& yqn,
                                            const fiscal_year::day& d) NOEXCEPT
    : y_(yqn.year())
    , qn_(yqn.quarternum())
    , d_(d)
    {}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>::year_quarternum_day(const year_quarternum_day_last<S>& yqndl) NOEXCEPT
    : y_(yqndl.year())
    , qn_(yqndl.quarternum())
    , d_(yqndl.day())
    {}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>::year_quarternum_day(const sys_days& dp) NOEXCEPT
    : year_quarternum_day<S>(from_days(dp.time_since_epoch()))
    {}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>::year_quarternum_day(const local_days& dp) NOEXCEPT
    : year_quarternum_day<S>(from_days(dp.time_since_epoch()))
    {}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>&
year_quarternum_day<S>::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>&
year_quarternum_day<S>::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>&
year_quarternum_day<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>&
year_quarternum_day<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD11
inline
year<S>
year_quarternum_day<S>::year() const NOEXCEPT
{
    return y_;
}

template <start S>
CONSTCD11
inline
quarternum
year_quarternum_day<S>::quarternum() const NOEXCEPT
{
    return qn_;
}

template <start S>
CONSTCD11
inline
day
year_quarternum_day<S>::day() const NOEXCEPT
{
    return d_;
}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>::operator sys_days() const NOEXCEPT
{
    return date::sys_days{to_days()};
}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>::operator local_days() const NOEXCEPT
{
    return date::local_days{to_days()};
}

template <start S>
CONSTCD14
inline
bool
year_quarternum_day<S>::ok() const NOEXCEPT
{
    return y_.ok() && d_.ok() && fiscal_year::day{1u} <= d_ && d_ <= year_quarternum_day_last<S>{y_, qn_}.day();
}

template <start S>
CONSTCD14
inline
days
year_quarternum_day<S>::to_days() const NOEXCEPT
{
    CONSTDATA unsigned char s = static_cast<unsigned char>(S) - 1;
    const unsigned quarternum = static_cast<unsigned>(qn_) - 1;
    const unsigned fiscal_month = 3 * quarternum;

    int year = static_cast<int>(y_);
    unsigned civil_month = s + fiscal_month;

    if (civil_month > 11) {
        civil_month -= 12;
    } else if (s != 0) {
        --year;
    }

    const unsigned day = static_cast<unsigned>(d_) - 1;

    const date::year_month_day quarter_start{
        date::year{year} / date::month{civil_month + 1} / date::day{1}
    };

    const date::sys_days quarter_days{
        date::sys_days{quarter_start} + date::days{day}
    };

    return quarter_days.time_since_epoch();
}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>
year_quarternum_day<S>::from_days(const days& dd) NOEXCEPT
{
    CONSTDATA unsigned char s = static_cast<unsigned char>(S) - 1;

    const date::sys_days dp{dd};
    const date::year_month_day ymd{dp};

    const unsigned civil_month = static_cast<unsigned>(ymd.month()) - 1;

    int year = static_cast<int>(ymd.year());
    int fiscal_month = static_cast<int>(civil_month) - static_cast<int>(s);

    if (fiscal_month < 0) {
        fiscal_month += 12;
    } else if (s != 0) {
        ++year;
    }

    const unsigned quarternum = static_cast<unsigned>(fiscal_month) / 3;

    const fiscal_year::year_quarternum_day<S> quarter_start{
        fiscal_year::year<S>{year} / fiscal_year::quarternum{quarternum + 1} / fiscal_year::day{1u}
    };

    // Find day of quarter as number of days from start of quarter
    const days days = dp - sys_days{quarter_start};
    const fiscal_year::day day{static_cast<unsigned>(days.count()) + 1};

    return quarter_start.year() / quarter_start.quarternum() / day;
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT
{
    return x.year() == y.year() &&
        x.quarternum() == y.quarternum() &&
        x.day() == y.day();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.quarternum() < y.quarternum() ? true
        : (x.quarternum() > y.quarternum() ? false
        : (x.day() < y.day()))));
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_quarternum_day<S>& x, const year_quarternum_day<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>
operator+(const year_quarternum_day<S>& yqnd, const quarters& dq)  NOEXCEPT
{
    return {year_quarternum<S>{yqnd.year(), yqnd.quarternum()} + dq, yqnd.day()};
}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>
operator+(const quarters& dq, const year_quarternum_day<S>& yqnd)  NOEXCEPT
{
    return yqnd + dq;
}

template <start S>
CONSTCD14
inline
year_quarternum_day<S>
operator-(const year_quarternum_day<S>& yqnd, const quarters& dq)  NOEXCEPT
{
    return yqnd + -dq;
}

template <start S>
CONSTCD11
inline
year_quarternum_day<S>
operator+(const year_quarternum_day<S>& yqnd, const years& dy)  NOEXCEPT
{
    return {yqnd.year() + dy, yqnd.quarternum(), yqnd.day()};
}

template <start S>
CONSTCD11
inline
year_quarternum_day<S>
operator+(const years& dy, const year_quarternum_day<S>& yqnd)  NOEXCEPT
{
    return yqnd + dy;
}

template <start S>
CONSTCD11
inline
year_quarternum_day<S>
operator-(const year_quarternum_day<S>& yqnd, const years& dy)  NOEXCEPT
{
    return yqnd + -dy;
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarternum_day<S>& yqnd)
{
    return os << yqnd.year() << '-' << yqnd.quarternum() << '-' << yqnd.day();
}

// literals

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
inline namespace literals
{
#endif  // !defined(_MSC_VER) || (_MSC_VER >= 1900)

CONSTDATA fiscal_year::last_spec last{};

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
}  // inline namespace literals
#endif

// year_quarternum from operator/()

template <start S>
CONSTCD11
inline
year_quarternum<S> operator/(const year<S>& y, const quarternum& qn) NOEXCEPT
{
    return {y, qn};
}

template <start S>
CONSTCD11
inline
year_quarternum<S> operator/(const year<S>& y, int qn) NOEXCEPT
{
    return y / quarternum(static_cast<unsigned>(qn));
}

// quarternum_day from operator/()

CONSTCD11
inline
quarternum_day
operator/(const quarternum& qn, const day& d) NOEXCEPT
{
    return {qn, d};
}

CONSTCD11
inline
quarternum_day
operator/(const quarternum& qn, int d) NOEXCEPT
{
    return qn / day(static_cast<unsigned>(d));
}

CONSTCD11
inline
quarternum_day
operator/(int qn, const day& d) NOEXCEPT
{
    return quarternum(static_cast<unsigned>(qn)) / d;
}

CONSTCD11
inline
quarternum_day
operator/(const day& d, const quarternum& qn) NOEXCEPT
{
    return qn / d;
}

CONSTCD11
inline
quarternum_day
operator/(const day& d, int qn) NOEXCEPT
{
    return qn / d;
}

// quarternum_day_last from operator/()

CONSTCD11
inline
quarternum_day_last
operator/(const quarternum& qn, last_spec) NOEXCEPT
{
    return quarternum_day_last{qn};
}

CONSTCD11
inline
quarternum_day_last
operator/(int qn, last_spec) NOEXCEPT
{
    return quarternum(static_cast<unsigned>(qn))/last;
}

CONSTCD11
inline
quarternum_day_last
operator/(last_spec, const quarternum& qn) NOEXCEPT
{
    return qn / last;
}

CONSTCD11
inline
quarternum_day_last
operator/(last_spec, int qn) NOEXCEPT
{
    return qn / last;
}

// year_quarternum_day from operator/()

template <start S>
CONSTCD11
inline
year_quarternum_day<S>
operator/(const year_quarternum<S>& yqn, const day& d) NOEXCEPT
{
    return {yqn, d};
}

template <start S>
CONSTCD11
inline
year_quarternum_day<S>
operator/(const year_quarternum<S>& yqn, int d) NOEXCEPT
{
    return yqn / day(static_cast<unsigned>(d));
}

template <start S>
CONSTCD11
inline
year_quarternum_day<S>
operator/(const year<S>& y, const quarternum_day& qnd) NOEXCEPT
{
    return y / qnd.quarternum() / qnd.day();
}

template <start S>
CONSTCD11
inline
year_quarternum_day<S>
operator/(const quarternum_day& qnd, const year<S>& y) NOEXCEPT
{
    return y / qnd;
}

// year_quarternum_day_last from operator/()

template <start S>
CONSTCD11
inline
year_quarternum_day_last<S>
operator/(const year_quarternum<S>& ynq, last_spec) NOEXCEPT
{
    return year_quarternum_day_last<S>{ynq};
}

template <start S>
CONSTCD11
inline
year_quarternum_day_last<S>
operator/(const year<S>& y, const quarternum_day_last& qndl) NOEXCEPT
{
    return {y, qndl};
}

template <start S>
CONSTCD11
inline
year_quarternum_day_last<S>
operator/(const quarternum_day_last& qndl, const year<S>& y) NOEXCEPT
{
    return y / qndl;
}

}  // namespace fiscal_year

#endif  // FISCAL_YEAR_H
