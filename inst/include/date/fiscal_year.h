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
class quarter;
template <start S>
class year;

template <start S>
class year_quarter;
class quarter_day;
class quarter_day_last;

template <start S>
class year_quarter_day;
template <start S>
class year_quarter_day_last;

// date composition operators

template <start S>
CONSTCD11 year_quarter<S> operator/(const year<S>& y, const quarter& q) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter<S> operator/(const year<S>& y, int            q) NOEXCEPT;

CONSTCD11 quarter_day operator/(const quarter& q, const day& d) NOEXCEPT;
CONSTCD11 quarter_day operator/(const quarter& q, int        d) NOEXCEPT;
CONSTCD11 quarter_day operator/(int            q, const day& d) NOEXCEPT;
CONSTCD11 quarter_day operator/(const day& d, const quarter& q) NOEXCEPT;
CONSTCD11 quarter_day operator/(const day& d, int            q) NOEXCEPT;

CONSTCD11 quarter_day_last operator/(const quarter& q, last_spec) NOEXCEPT;
CONSTCD11 quarter_day_last operator/(int            q, last_spec) NOEXCEPT;
CONSTCD11 quarter_day_last operator/(last_spec, const quarter& q) NOEXCEPT;
CONSTCD11 quarter_day_last operator/(last_spec, int            q) NOEXCEPT;

template <start S>
CONSTCD11 year_quarter_day<S> operator/(const year_quarter<S>& yq, const day& d) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter_day<S> operator/(const year_quarter<S>& yq, int        d) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter_day<S> operator/(const year<S>& y, const quarter_day& qd) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter_day<S> operator/(const quarter_day& qd, const year<S>& y) NOEXCEPT;

template <start S>
CONSTCD11 year_quarter_day_last<S> operator/(const year_quarter<S>& yq,   last_spec) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter_day_last<S> operator/(const year<S>& y, const quarter_day_last& qdl) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter_day_last<S> operator/(const quarter_day_last& qdl, const year<S>& y) NOEXCEPT;

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

// quarter

class quarter
{
    unsigned char q_;

public:
    quarter() = default;
    explicit CONSTCD11 quarter(unsigned q) NOEXCEPT;
    explicit quarter(int) = delete;

    CONSTCD14 quarter& operator++()    NOEXCEPT;
    CONSTCD14 quarter  operator++(int) NOEXCEPT;
    CONSTCD14 quarter& operator--()    NOEXCEPT;
    CONSTCD14 quarter  operator--(int) NOEXCEPT;

    CONSTCD14 quarter& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 quarter& operator-=(const quarters& dq) NOEXCEPT;

    CONSTCD11 explicit operator unsigned() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const quarter& x, const quarter& y) NOEXCEPT;
CONSTCD11 bool operator!=(const quarter& x, const quarter& y) NOEXCEPT;
CONSTCD11 bool operator< (const quarter& x, const quarter& y) NOEXCEPT;
CONSTCD11 bool operator> (const quarter& x, const quarter& y) NOEXCEPT;
CONSTCD11 bool operator<=(const quarter& x, const quarter& y) NOEXCEPT;
CONSTCD11 bool operator>=(const quarter& x, const quarter& y) NOEXCEPT;

CONSTCD11 quarter  operator+(const quarter&  x, const quarters& y) NOEXCEPT;
CONSTCD11 quarter  operator+(const quarters& x,  const quarter& y) NOEXCEPT;
CONSTCD11 quarter  operator-(const quarter&  x, const quarters& y) NOEXCEPT;
CONSTCD11 quarters operator-(const quarter&  x,  const quarter& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarter& q);

// year

template <start S>
class year
{
    short y_;
    static const start s_ = S;

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

// year_quarter

template <start S>
class year_quarter
{
    fiscal_year::year<S> y_;
    fiscal_year::quarter q_;

public:
    year_quarter<S>() = default;
    CONSTCD11 year_quarter<S>(const fiscal_year::year<S>& y,
                              const fiscal_year::quarter& q) NOEXCEPT;
    CONSTCD14 year_quarter<S>(const date::year_month& ym) NOEXCEPT;

    CONSTCD11 fiscal_year::year<S> year()    const NOEXCEPT;
    CONSTCD11 fiscal_year::quarter quarter() const NOEXCEPT;

    CONSTCD14 year_quarter<S>& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarter<S>& operator-=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarter<S>& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarter<S>& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 bool ok() const NOEXCEPT;

    CONSTCD14 operator date::year_month() const NOEXCEPT;

private:
    static CONSTCD14 year_quarter<S> from_year_month(const date::year_month& ym) NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT;

template <start S>
CONSTCD14 year_quarter<S> operator+(const year_quarter<S>& yq, const quarters& dq) NOEXCEPT;
template <start S>
CONSTCD14 year_quarter<S> operator+(const quarters& dq, const year_quarter<S>& yq) NOEXCEPT;
template <start S>
CONSTCD14 year_quarter<S> operator-(const year_quarter<S>& yq, const quarters& dq) NOEXCEPT;

template <start S>
CONSTCD11 year_quarter<S> operator+(const year_quarter<S>& yq, const years& dy) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter<S> operator+(const years& dy, const year_quarter<S>& yq) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter<S> operator-(const year_quarter<S>& yq, const years& dy) NOEXCEPT;

template <start S>
CONSTCD11 quarters operator-(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter<S>& yq);

// quarter_day

class quarter_day
{
    fiscal_year::quarter q_;
    fiscal_year::day d_;

public:
    quarter_day() = default;
    CONSTCD11 quarter_day(const fiscal_year::quarter& q,
                          const fiscal_year::day& d) NOEXCEPT;

    CONSTCD11 fiscal_year::quarter quarter() const NOEXCEPT;
    CONSTCD11 fiscal_year::day day() const NOEXCEPT;

    CONSTCD14 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const quarter_day& x, const quarter_day& y) NOEXCEPT;
CONSTCD11 bool operator!=(const quarter_day& x, const quarter_day& y) NOEXCEPT;
CONSTCD11 bool operator< (const quarter_day& x, const quarter_day& y) NOEXCEPT;
CONSTCD11 bool operator> (const quarter_day& x, const quarter_day& y) NOEXCEPT;
CONSTCD11 bool operator<=(const quarter_day& x, const quarter_day& y) NOEXCEPT;
CONSTCD11 bool operator>=(const quarter_day& x, const quarter_day& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarter_day& qd);

// quarter_day_last

class quarter_day_last
{
    fiscal_year::quarter q_;

public:
    quarter_day_last() = default;
    CONSTCD11 explicit quarter_day_last(const fiscal_year::quarter& q) NOEXCEPT;

    CONSTCD11 fiscal_year::quarter quarter() const NOEXCEPT;

    CONSTCD14 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT;
CONSTCD11 bool operator!=(const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT;
CONSTCD11 bool operator< (const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT;
CONSTCD11 bool operator> (const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT;
CONSTCD11 bool operator<=(const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT;
CONSTCD11 bool operator>=(const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarter_day_last& qdl);

// year_quarter_day_last

template <start S>
class year_quarter_day_last
{
    fiscal_year::year<S> y_;
    fiscal_year::quarter q_;

public:
    year_quarter_day_last<S>() = default;
    CONSTCD11 year_quarter_day_last<S>(const fiscal_year::year<S>& y,
                                       const fiscal_year::quarter& q) NOEXCEPT;
    CONSTCD11 year_quarter_day_last<S>(const fiscal_year::year_quarter<S>& yq) NOEXCEPT;

    CONSTCD14 year_quarter_day_last<S>& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarter_day_last<S>& operator-=(const quarters& dq) NOEXCEPT;

    CONSTCD14 year_quarter_day_last<S>& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarter_day_last<S>& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 fiscal_year::year<S> year()    const NOEXCEPT;
    CONSTCD11 fiscal_year::quarter quarter() const NOEXCEPT;
    CONSTCD14 fiscal_year::day     day()     const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT;

template <start S>
CONSTCD14 year_quarter_day_last<S> operator+(const year_quarter_day_last<S>& yqdl, const quarters& dq) NOEXCEPT;
template <start S>
CONSTCD14 year_quarter_day_last<S> operator+(const quarters& dq, const year_quarter_day_last<S>& yqdl) NOEXCEPT;
template <start S>
CONSTCD14 year_quarter_day_last<S> operator-(const year_quarter_day_last<S>& yqdl, const quarters& dq) NOEXCEPT;

template <start S>
CONSTCD11 year_quarter_day_last<S> operator+(const year_quarter_day_last<S>& yqdl, const years& dy) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter_day_last<S> operator+(const years& dy, const year_quarter_day_last<S>& yqdl) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter_day_last<S> operator-(const year_quarter_day_last<S>& yqdl, const years& dy) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter_day_last<S>& yqdl);

// year_quarter_day

template <start S>
class year_quarter_day
{
    fiscal_year::year<S> y_;
    fiscal_year::quarter q_;
    fiscal_year::day d_;

public:
    year_quarter_day<S>() = default;
    CONSTCD11 year_quarter_day<S>(const fiscal_year::year<S>& y,
                                  const fiscal_year::quarter& q,
                                  const fiscal_year::day& d) NOEXCEPT;
    CONSTCD11 year_quarter_day<S>(const fiscal_year::year_quarter<S>& yq,
                                  const fiscal_year::day& d) NOEXCEPT;
    CONSTCD14 year_quarter_day<S>(const year_quarter_day_last<S>& yqdl) NOEXCEPT;
    CONSTCD14 year_quarter_day<S>(const sys_days& dp) NOEXCEPT;
    CONSTCD14 year_quarter_day<S>(const local_days& dp) NOEXCEPT;

    CONSTCD14 year_quarter_day<S>& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarter_day<S>& operator-=(const quarters& dq) NOEXCEPT;

    CONSTCD14 year_quarter_day<S>& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarter_day<S>& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 fiscal_year::year<S> year()    const NOEXCEPT;
    CONSTCD11 fiscal_year::quarter quarter() const NOEXCEPT;
    CONSTCD11 fiscal_year::day     day()     const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD14 bool ok() const NOEXCEPT;

private:
    static CONSTCD14 year_quarter_day<S> from_days(const days& dd) NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT;

template <start S>
CONSTCD14 year_quarter_day<S> operator+(const year_quarter_day<S>& yqd, const quarters& dq) NOEXCEPT;
template <start S>
CONSTCD14 year_quarter_day<S> operator+(const quarters& dq, const year_quarter_day<S>& yqd) NOEXCEPT;
template <start S>
CONSTCD14 year_quarter_day<S> operator-(const year_quarter_day<S>& yqd, const quarters& dq) NOEXCEPT;

template <start S>
CONSTCD11 year_quarter_day<S> operator+(const year_quarter_day<S>& yqd, const years& dy) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter_day<S> operator+(const years& dy, const year_quarter_day<S>& yqd) NOEXCEPT;
template <start S>
CONSTCD11 year_quarter_day<S> operator-(const year_quarter_day<S>& yqd, const years& dy) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter_day<S>& yqd);

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

// quarter

CONSTCD11
inline
quarter::quarter(unsigned q) NOEXCEPT
    : q_(static_cast<decltype(q_)>(q))
    {}

CONSTCD14
inline
quarter&
quarter::operator++() NOEXCEPT
{
    ++q_;
    return *this;
}

CONSTCD14
inline
quarter
quarter::operator++(int) NOEXCEPT
{
    auto tmp(*this);
    ++(*this);
    return tmp;
}

CONSTCD14
inline
quarter&
quarter::operator--() NOEXCEPT
{
    --q_;
    return *this;
}

CONSTCD14
inline
quarter
quarter::operator--(int) NOEXCEPT
{
    auto tmp(*this);
    --(*this);
    return tmp;
}

CONSTCD14
inline
quarter&
quarter::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

CONSTCD14
inline
quarter&
quarter::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

CONSTCD11
inline
quarter::operator unsigned() const NOEXCEPT
{
    return q_;
}

CONSTCD11
inline
bool
quarter::ok() const NOEXCEPT
{
    return 1 <= q_ && q_ <= 4;
}

CONSTCD11
inline
bool
operator==(const quarter& x, const quarter& y) NOEXCEPT
{
    return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator!=(const quarter& x, const quarter& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const quarter& x, const quarter& y) NOEXCEPT
{
    return static_cast<unsigned>(x) < static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator>(const quarter& x, const quarter& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const quarter& x, const quarter& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const quarter& x, const quarter& y) NOEXCEPT
{
    return !(x < y);
}

CONSTCD11
inline
quarters
operator-(const quarter& x, const quarter& y) NOEXCEPT
{
    return quarters{static_cast<quarters::rep>(static_cast<unsigned>(x) - static_cast<unsigned>(y))};
}

CONSTCD11
inline
quarter
operator+(const quarter& x, const quarters& y) NOEXCEPT
{
    return quarter{static_cast<unsigned>(x) + static_cast<unsigned>(y.count())};
}

CONSTCD11
inline
quarter
operator+(const quarters& x, const quarter& y) NOEXCEPT
{
    return y + x;
}

CONSTCD11
inline
quarter
operator-(const quarter& x, const quarters& y) NOEXCEPT
{
    return x + -y;
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarter& q)
{
    date::detail::save_ostream<CharT, Traits> _(os);
    os.flags(std::ios::dec | std::ios::right);
    os.width(1);
    os << 'Q' << static_cast<unsigned>(q);
    if (!q.ok())
        os << " is not a valid quarter";
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
    return s_;
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

// year_quarter

template <start S>
CONSTCD11
inline
year_quarter<S>::year_quarter(const fiscal_year::year<S>& y,
                              const fiscal_year::quarter& q) NOEXCEPT
    : y_(y)
    , q_(q)
    {}

template <start S>
CONSTCD14
inline
year_quarter<S>::year_quarter(const date::year_month& ym) NOEXCEPT
    : year_quarter<S>(from_year_month(ym))
    {}

template <start S>
CONSTCD11
inline
bool
year_quarter<S>::ok() const NOEXCEPT
{
    return y_.ok() && q_.ok();
}

template <start S>
CONSTCD11
inline
year<S>
year_quarter<S>::year() const NOEXCEPT
{
    return y_;
}

template <start S>
CONSTCD11
inline
quarter
year_quarter<S>::quarter() const NOEXCEPT
{
    return q_;
}

template <start S>
CONSTCD14
inline
year_quarter<S>&
year_quarter<S>::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarter<S>&
year_quarter<S>::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarter<S>&
year_quarter<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarter<S>&
year_quarter<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarter<S>::operator date::year_month() const NOEXCEPT
{
    CONSTDATA unsigned char s = static_cast<unsigned char>(S) - 1;
    const unsigned quarter = static_cast<unsigned>(q_) - 1;
    const unsigned fiscal_month = 3 * quarter;

    int year = static_cast<int>(y_);
    unsigned civil_month = s + fiscal_month;

    if (civil_month > 11) {
        civil_month -= 12;
    } else if (s != 0) {
        --year;
    }

    return date::year{year} / date::month{civil_month + 1};
}

template <start S>
CONSTCD14
inline
year_quarter<S>
year_quarter<S>::from_year_month(const date::year_month& ym) NOEXCEPT
{
    CONSTDATA unsigned char s = static_cast<unsigned char>(S) - 1;
    const unsigned civil_month = static_cast<unsigned>(ym.month()) - 1;

    int year = static_cast<int>(ym.year());
    int fiscal_month = static_cast<int>(civil_month) - static_cast<int>(s);

    if (fiscal_month < 0) {
        fiscal_month += 12;
    } else if (s != 0) {
        ++year;
    }

    const unsigned quarter = static_cast<unsigned>(fiscal_month) / 3;

    return fiscal_year::year<S>{year} / fiscal_year::quarter{quarter + 1};
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT
{
    return x.year() == y.year() && x.quarter() == y.quarter();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.quarter() < y.quarter()));
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD14
inline
year_quarter<S>
operator+(const year_quarter<S>& yq, const quarters& dq) NOEXCEPT
{
    auto dqi = static_cast<int>(static_cast<unsigned>(yq.quarter())) - 1 + dq.count();
    auto dy = (dqi >= 0 ? dqi : dqi - 3) / 4;
    dqi = dqi - dy * 4 + 1;
    return {(yq.year() + years(dy)), quarter(static_cast<unsigned>(dqi))};
}

template <start S>
CONSTCD14
inline
year_quarter<S>
operator+(const quarters& dq, const year_quarter<S>& yq) NOEXCEPT
{
    return yq + dq;
}

template <start S>
CONSTCD14
inline
year_quarter<S>
operator-(const year_quarter<S>& yq, const quarters& dq) NOEXCEPT
{
    return yq + -dq;
}

template <start S>
CONSTCD11
inline
year_quarter<S>
operator+(const year_quarter<S>& yq, const years& dy) NOEXCEPT
{
    return {(yq.year() + dy), yq.quarter()};
}

template <start S>
CONSTCD11
inline
year_quarter<S>
operator+(const years& dy, const year_quarter<S>& yq) NOEXCEPT
{
    return yq + dy;
}

template <start S>
CONSTCD11
inline
year_quarter<S>
operator-(const year_quarter<S>& yq, const years& dy) NOEXCEPT
{
    return yq + -dy;
}

template <start S>
CONSTCD11
inline
quarters
operator-(const year_quarter<S>& x, const year_quarter<S>& y) NOEXCEPT
{
    return (x.year() - y.year()) + (x.quarter() - y.quarter());
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter<S>& yq)
{
    return os << yq.year() << '-' << yq.quarter();
}

// quarter_day

CONSTCD11
inline
quarter_day::quarter_day(const fiscal_year::quarter& q,
                         const fiscal_year::day& d) NOEXCEPT
    : q_(q)
    , d_(d)
    {}

CONSTCD11
inline
quarter
quarter_day::quarter() const NOEXCEPT
{
    return q_;
}

CONSTCD11
inline
day
quarter_day::day() const NOEXCEPT
{
    return d_;
}

CONSTCD14
inline
bool
quarter_day::ok() const NOEXCEPT
{
    return q_.ok() && d_.ok();
}

CONSTCD11
inline
bool
operator==(const quarter_day& x, const quarter_day& y) NOEXCEPT
{
    return x.quarter() == y.quarter() && x.day() == y.day();
}

CONSTCD11
inline
bool
operator!=(const quarter_day& x, const quarter_day& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const quarter_day& x, const quarter_day& y) NOEXCEPT
{
    return x.quarter() < y.quarter() ? true
        : (x.quarter() > y.quarter() ? false
        : (static_cast<unsigned>(x.day()) < static_cast<unsigned>(y.day())));
}

CONSTCD11
inline
bool
operator>(const quarter_day& x, const quarter_day& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const quarter_day& x, const quarter_day& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const quarter_day& x, const quarter_day& y) NOEXCEPT
{
    return !(x < y);
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarter_day& qd)
{
    return os << qd.quarter() << '-' << qd.day();
}

// quarter_day_last

CONSTCD11
inline
quarter_day_last::quarter_day_last(const fiscal_year::quarter& q) NOEXCEPT
    : q_(q)
    {}

CONSTCD11
inline
quarter
quarter_day_last::quarter() const NOEXCEPT
{
    return q_;
}

CONSTCD14
inline
bool
quarter_day_last::ok() const NOEXCEPT
{
    return q_.ok();
}

CONSTCD11
inline
bool
operator==(const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT
{
    return x.quarter() == y.quarter();
}

CONSTCD11
inline
bool
operator!=(const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT
{
    return static_cast<unsigned>(x.quarter()) < static_cast<unsigned>(y.quarter());
}

CONSTCD11
inline
bool
operator>(const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const quarter_day_last& x, const quarter_day_last& y) NOEXCEPT
{
    return !(x < y);
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarter_day_last& qd)
{
    return os << qd.quarter() << "-last";
}

// year_quarter_day_last

template <start S>
CONSTCD11
inline
year_quarter_day_last<S>::year_quarter_day_last(const fiscal_year::year<S>& y,
                                                const fiscal_year::quarter& q) NOEXCEPT
    : y_(y)
    , q_(q)
    {}

template <start S>
CONSTCD11
inline
year_quarter_day_last<S>::year_quarter_day_last(const fiscal_year::year_quarter<S>& yq) NOEXCEPT
    : y_(yq.year())
    , q_(yq.quarter())
    {}

template <start S>
CONSTCD14
inline
year_quarter_day_last<S>&
year_quarter_day_last<S>::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarter_day_last<S>&
year_quarter_day_last<S>::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarter_day_last<S>&
year_quarter_day_last<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarter_day_last<S>&
year_quarter_day_last<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD11
inline
year<S>
year_quarter_day_last<S>::year() const NOEXCEPT
{
    return y_;
}

template <start S>
CONSTCD11
inline
quarter
year_quarter_day_last<S>::quarter() const NOEXCEPT
{
    return q_;
}

template <start S>
CONSTCD14
inline
day
year_quarter_day_last<S>::day() const NOEXCEPT
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

    const unsigned quarter = static_cast<unsigned>(q_) - 1;

    // Remap [Jan -> Dec] to [Dec -> Jan] to group quarters with February
    unsigned key = (s == 12) ? 0 : s + 1;

    key = key + 3 * quarter;
    if (key > 11) {
        key -= 12;
    }

    if (key <= 2 && y_.is_leap()) {
        return days[key] + fiscal_year::days{1u};
    } else {
        return days[key];
    }
}

template <start S>
CONSTCD14
inline
year_quarter_day_last<S>::operator sys_days() const NOEXCEPT
{
    return sys_days(year_quarter_day<S>{year(), quarter(), day()});
}

template <start S>
CONSTCD14
inline
year_quarter_day_last<S>::operator local_days() const NOEXCEPT
{
    return local_days(year_quarter_day<S>{year(), quarter(), day()});
}

template <start S>
CONSTCD11
inline
bool
year_quarter_day_last<S>::ok() const NOEXCEPT
{
    return y_.ok() && q_.ok();
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT
{
    return x.year() == y.year() && x.quarter() == y.quarter();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : x.quarter() < y.quarter());
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_quarter_day_last<S>& x, const year_quarter_day_last<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD14
inline
year_quarter_day_last<S>
operator+(const year_quarter_day_last<S>& yqdl, const quarters& dq) NOEXCEPT
{
    return {year_quarter<S>{yqdl.year(), yqdl.quarter()} + dq};
}

template <start S>
CONSTCD14
inline
year_quarter_day_last<S>
operator+(const quarters& dq, const year_quarter_day_last<S>& yqdl)  NOEXCEPT
{
    return yqdl + dq;
}

template <start S>
CONSTCD14
inline
year_quarter_day_last<S>
operator-(const year_quarter_day_last<S>& yqdl, const quarters& dq)  NOEXCEPT
{
    return yqdl + -dq;
}

template <start S>
CONSTCD11
inline
year_quarter_day_last<S>
operator+(const year_quarter_day_last<S>& yqdl, const years& dy)  NOEXCEPT
{
    return {yqdl.year() + dy, yqdl.quarter()};
}

template <start S>
CONSTCD11
inline
year_quarter_day_last<S>
operator+(const years& dy, const year_quarter_day_last<S>& yqdl)  NOEXCEPT
{
    return yqdl + dy;
}

template <start S>
CONSTCD11
inline
year_quarter_day_last<S>
operator-(const year_quarter_day_last<S>& yqdl, const years& dy)  NOEXCEPT
{
    return yqdl + -dy;
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter_day_last<S>& yqdl)
{
    return os << yqdl.year() << "-" << yqdl.quarter() << "-last";
}

// year_quarter_day

template <start S>
CONSTCD11
inline
year_quarter_day<S>::year_quarter_day(const fiscal_year::year<S>& y,
                                      const fiscal_year::quarter& q,
                                      const fiscal_year::day& d) NOEXCEPT
    : y_(y)
    , q_(q)
    , d_(d)
    {}

template <start S>
CONSTCD11
inline
year_quarter_day<S>::year_quarter_day(const fiscal_year::year_quarter<S>& yq,
                                      const fiscal_year::day& d) NOEXCEPT
    : y_(yq.year())
    , q_(yq.quarter())
    , d_(d)
    {}

template <start S>
CONSTCD14
inline
year_quarter_day<S>::year_quarter_day(const year_quarter_day_last<S>& yqdl) NOEXCEPT
    : y_(yqdl.year())
    , q_(yqdl.quarter())
    , d_(yqdl.day())
    {}

template <start S>
CONSTCD14
inline
year_quarter_day<S>::year_quarter_day(const sys_days& dp) NOEXCEPT
    : year_quarter_day<S>(from_days(dp.time_since_epoch()))
    {}

template <start S>
CONSTCD14
inline
year_quarter_day<S>::year_quarter_day(const local_days& dp) NOEXCEPT
    : year_quarter_day<S>(from_days(dp.time_since_epoch()))
    {}

template <start S>
CONSTCD14
inline
year_quarter_day<S>&
year_quarter_day<S>::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarter_day<S>&
year_quarter_day<S>::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarter_day<S>&
year_quarter_day<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarter_day<S>&
year_quarter_day<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD11
inline
year<S>
year_quarter_day<S>::year() const NOEXCEPT
{
    return y_;
}

template <start S>
CONSTCD11
inline
quarter
year_quarter_day<S>::quarter() const NOEXCEPT
{
    return q_;
}

template <start S>
CONSTCD11
inline
day
year_quarter_day<S>::day() const NOEXCEPT
{
    return d_;
}

template <start S>
CONSTCD14
inline
year_quarter_day<S>::operator sys_days() const NOEXCEPT
{
    const fiscal_year::year_quarter<S> yq{y_, q_};
    const date::year_month ym_start{yq};
    const date::year_month_day ymd_start{ym_start / date::day{1}};
    return date::sys_days{ymd_start} + date::days{static_cast<unsigned>(d_) - 1};
}

template <start S>
CONSTCD14
inline
year_quarter_day<S>::operator local_days() const NOEXCEPT
{
    const fiscal_year::year_quarter<S> yq{y_, q_};
    const date::year_month ym_start{yq};
    const date::year_month_day ymd_start{ym_start / date::day{1}};
    return date::local_days{ymd_start} + date::days{static_cast<unsigned>(d_) - 1};
}

template <start S>
CONSTCD14
inline
bool
year_quarter_day<S>::ok() const NOEXCEPT
{
    return y_.ok() && d_.ok() && fiscal_year::day{1u} <= d_ && d_ <= year_quarter_day_last<S>{y_, q_}.day();
}

template <start S>
CONSTCD14
inline
year_quarter_day<S>
year_quarter_day<S>::from_days(const days& dd) NOEXCEPT
{
    const date::sys_days dp{dd};
    const date::year_month_day ymd{dp};

    const date::year_month ym{ymd.year() / ymd.month()};
    const year_quarter<S> yq{ym};

    // Find day of quarter as number of days from start of quarter
    const fiscal_year::year_quarter_day<S> yqd_start{yq, fiscal_year::day{1u}};
    const days dd_quarter = dp - sys_days{yqd_start};
    const fiscal_year::day d{static_cast<unsigned>(dd_quarter.count()) + 1};

    return {yq, d};
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT
{
    return x.year() == y.year() &&
        x.quarter() == y.quarter() &&
        x.day() == y.day();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.quarter() < y.quarter() ? true
        : (x.quarter() > y.quarter() ? false
        : (x.day() < y.day()))));
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_quarter_day<S>& x, const year_quarter_day<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD14
inline
year_quarter_day<S>
operator+(const year_quarter_day<S>& yqd, const quarters& dq)  NOEXCEPT
{
    return {year_quarter<S>{yqd.year(), yqd.quarter()} + dq, yqd.day()};
}

template <start S>
CONSTCD14
inline
year_quarter_day<S>
operator+(const quarters& dq, const year_quarter_day<S>& yqd)  NOEXCEPT
{
    return yqd + dq;
}

template <start S>
CONSTCD14
inline
year_quarter_day<S>
operator-(const year_quarter_day<S>& yqd, const quarters& dq)  NOEXCEPT
{
    return yqd + -dq;
}

template <start S>
CONSTCD11
inline
year_quarter_day<S>
operator+(const year_quarter_day<S>& yqd, const years& dy)  NOEXCEPT
{
    return {yqd.year() + dy, yqd.quarter(), yqd.day()};
}

template <start S>
CONSTCD11
inline
year_quarter_day<S>
operator+(const years& dy, const year_quarter_day<S>& yqd)  NOEXCEPT
{
    return yqd + dy;
}

template <start S>
CONSTCD11
inline
year_quarter_day<S>
operator-(const year_quarter_day<S>& yqd, const years& dy)  NOEXCEPT
{
    return yqd + -dy;
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter_day<S>& yqd)
{
    return os << yqd.year() << '-' << yqd.quarter() << '-' << yqd.day();
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

// year_quarter from operator/()

template <start S>
CONSTCD11
inline
year_quarter<S> operator/(const year<S>& y, const quarter& q) NOEXCEPT
{
    return {y, q};
}

template <start S>
CONSTCD11
inline
year_quarter<S> operator/(const year<S>& y, int q) NOEXCEPT
{
    return y / quarter(static_cast<unsigned>(q));
}

// quarter_day from operator/()

CONSTCD11
inline
quarter_day
operator/(const quarter& q, const day& d) NOEXCEPT
{
    return {q, d};
}

CONSTCD11
inline
quarter_day
operator/(const quarter& q, int d) NOEXCEPT
{
    return q / day(static_cast<unsigned>(d));
}

CONSTCD11
inline
quarter_day
operator/(int q, const day& d) NOEXCEPT
{
    return quarter(static_cast<unsigned>(q)) / d;
}

CONSTCD11
inline
quarter_day
operator/(const day& d, const quarter& q) NOEXCEPT
{
    return q / d;
}

CONSTCD11
inline
quarter_day
operator/(const day& d, int q) NOEXCEPT
{
    return q / d;
}

// quarter_day_last from operator/()

CONSTCD11
inline
quarter_day_last
operator/(const quarter& q, last_spec) NOEXCEPT
{
    return quarter_day_last{q};
}

CONSTCD11
inline
quarter_day_last
operator/(int q, last_spec) NOEXCEPT
{
    return quarter(static_cast<unsigned>(q))/last;
}

CONSTCD11
inline
quarter_day_last
operator/(last_spec, const quarter& q) NOEXCEPT
{
    return q / last;
}

CONSTCD11
inline
quarter_day_last
operator/(last_spec, int q) NOEXCEPT
{
    return q / last;
}

// year_quarter_day from operator/()

template <start S>
CONSTCD11
inline
year_quarter_day<S>
operator/(const year_quarter<S>& yq, const day& d) NOEXCEPT
{
    return {yq, d};
}

template <start S>
CONSTCD11
inline
year_quarter_day<S>
operator/(const year_quarter<S>& yq, int d) NOEXCEPT
{
    return yq / day(static_cast<unsigned>(d));
}

template <start S>
CONSTCD11
inline
year_quarter_day<S>
operator/(const year<S>& y, const quarter_day& qd) NOEXCEPT
{
    return y / qd.quarter() / qd.day();
}

template <start S>
CONSTCD11
inline
year_quarter_day<S>
operator/(const quarter_day& qd, const year<S>& y) NOEXCEPT
{
    return y / qd;
}

// year_quarter_day_last from operator/()

template <start S>
CONSTCD11
inline
year_quarter_day_last<S>
operator/(const year_quarter<S>& yq, last_spec) NOEXCEPT
{
    return year_quarter_day_last<S>{yq};
}

template <start S>
CONSTCD11
inline
year_quarter_day_last<S>
operator/(const year<S>& y, const quarter_day_last& qdl) NOEXCEPT
{
    return {y, qdl};
}

template <start S>
CONSTCD11
inline
year_quarter_day_last<S>
operator/(const quarter_day_last& qdl, const year<S>& y) NOEXCEPT
{
    return y / qdl;
}

}  // namespace fiscal_year

#endif  // FISCAL_YEAR_H
