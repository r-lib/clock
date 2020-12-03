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

class fiscal_start;

class day;
class quarter;
class year;

class year_quarter;
class quarter_day;
class quarter_day_last;

class year_quarter_day;
class year_quarter_day_last;

// fiscal_start

class fiscal_start
{
    unsigned char fs_;
public:
    fiscal_start() = default;
    explicit CONSTCD11 fiscal_start(unsigned fs) NOEXCEPT;
    explicit fiscal_start(int) = delete;

    CONSTCD11 explicit operator unsigned() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const fiscal_start& x, const fiscal_start& y) NOEXCEPT;
CONSTCD11 bool operator!=(const fiscal_start& x, const fiscal_start& y) NOEXCEPT;

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

class year
{
    short y_;

public:
    year() = default;
    explicit CONSTCD11 year(int y) NOEXCEPT;

    CONSTCD14 year& operator++()    NOEXCEPT;
    CONSTCD14 year  operator++(int) NOEXCEPT;
    CONSTCD14 year& operator--()    NOEXCEPT;
    CONSTCD14 year  operator--(int) NOEXCEPT;

    CONSTCD14 year& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 explicit operator int() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;

    static CONSTCD11 year min() NOEXCEPT { return year{-32767}; }
    static CONSTCD11 year max() NOEXCEPT { return year{32767}; }
};

CONSTCD11 bool operator==(const year& x, const year& y) NOEXCEPT;
CONSTCD11 bool operator!=(const year& x, const year& y) NOEXCEPT;
CONSTCD11 bool operator< (const year& x, const year& y) NOEXCEPT;
CONSTCD11 bool operator> (const year& x, const year& y) NOEXCEPT;
CONSTCD11 bool operator<=(const year& x, const year& y) NOEXCEPT;
CONSTCD11 bool operator>=(const year& x, const year& y) NOEXCEPT;

CONSTCD11 year  operator+(const year&  x, const years& y) NOEXCEPT;
CONSTCD11 year  operator+(const years& x, const year&  y) NOEXCEPT;
CONSTCD11 year  operator-(const year&  x, const years& y) NOEXCEPT;
CONSTCD11 years operator-(const year&  x, const year&  y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year& y);

// year_quarter

class year_quarter
{
    fiscal_year::year    y_;
    fiscal_year::quarter q_;
    fiscal_year::fiscal_start fs_;

public:
    year_quarter() = default;
    CONSTCD11 year_quarter(const fiscal_year::year& y,
                           const fiscal_year::quarter& q,
                           const fiscal_year::fiscal_start& fs) NOEXCEPT;
    CONSTCD14 year_quarter(const date::year_month& ym,
                           const fiscal_year::fiscal_start& fs) NOEXCEPT;

    CONSTCD11 fiscal_year::year    year()    const NOEXCEPT;
    CONSTCD11 fiscal_year::quarter quarter() const NOEXCEPT;

    CONSTCD11 fiscal_year::fiscal_start fiscal_start() const NOEXCEPT;

    CONSTCD14 year_quarter& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarter& operator-=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarter& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarter& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 bool ok() const NOEXCEPT;

    CONSTCD14 operator date::year_month() const NOEXCEPT;

private:
    static CONSTCD14 year_quarter from_year_month(const date::year_month& ym,
                                                  const fiscal_year::fiscal_start& fs) NOEXCEPT;
};

CONSTCD11 bool operator==(const year_quarter& x, const year_quarter& y) NOEXCEPT;
CONSTCD11 bool operator!=(const year_quarter& x, const year_quarter& y) NOEXCEPT;
CONSTCD11 bool operator< (const year_quarter& x, const year_quarter& y) NOEXCEPT;
CONSTCD11 bool operator> (const year_quarter& x, const year_quarter& y) NOEXCEPT;
CONSTCD11 bool operator<=(const year_quarter& x, const year_quarter& y) NOEXCEPT;
CONSTCD11 bool operator>=(const year_quarter& x, const year_quarter& y) NOEXCEPT;

CONSTCD14 year_quarter operator+(const year_quarter& yq, const quarters& dq) NOEXCEPT;
CONSTCD14 year_quarter operator+(const quarters& dq, const year_quarter& yq) NOEXCEPT;
CONSTCD14 year_quarter operator-(const year_quarter& yq, const quarters& dq) NOEXCEPT;

CONSTCD11 year_quarter operator+(const year_quarter& yq, const years& dy) NOEXCEPT;
CONSTCD11 year_quarter operator+(const years& dy, const year_quarter& yq) NOEXCEPT;
CONSTCD11 year_quarter operator-(const year_quarter& yq, const years& dy) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter& yq);

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

class year_quarter_day_last
{
    fiscal_year::year    y_;
    fiscal_year::quarter q_;
    fiscal_year::fiscal_start fs_;

public:
    year_quarter_day_last() = default;
    CONSTCD11 year_quarter_day_last(const fiscal_year::year& y,
                                    const fiscal_year::quarter& q,
                                    const fiscal_year::fiscal_start& fs) NOEXCEPT;
    CONSTCD11 year_quarter_day_last(const fiscal_year::year_quarter& yq) NOEXCEPT;

    CONSTCD14 year_quarter_day_last& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarter_day_last& operator-=(const quarters& dq) NOEXCEPT;

    CONSTCD14 year_quarter_day_last& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarter_day_last& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 fiscal_year::year    year()    const NOEXCEPT;
    CONSTCD11 fiscal_year::quarter quarter() const NOEXCEPT;
    CONSTCD14 fiscal_year::day     day()     const NOEXCEPT;

    CONSTCD11 fiscal_year::fiscal_start fiscal_start() const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT;
CONSTCD11 bool operator!=(const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT;
CONSTCD11 bool operator< (const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT;
CONSTCD11 bool operator> (const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT;
CONSTCD11 bool operator<=(const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT;
CONSTCD11 bool operator>=(const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT;

CONSTCD14 year_quarter_day_last operator+(const year_quarter_day_last& yqdl, const quarters& dq) NOEXCEPT;
CONSTCD14 year_quarter_day_last operator+(const quarters& dq, const year_quarter_day_last& yqdl) NOEXCEPT;
CONSTCD14 year_quarter_day_last operator-(const year_quarter_day_last& yqdl, const quarters& dq) NOEXCEPT;

CONSTCD11 year_quarter_day_last operator+(const year_quarter_day_last& yqdl, const years& dy) NOEXCEPT;
CONSTCD11 year_quarter_day_last operator+(const years& dy, const year_quarter_day_last& yqdl) NOEXCEPT;
CONSTCD11 year_quarter_day_last operator-(const year_quarter_day_last& yqdl, const years& dy) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter_day_last& yqdl);

// year_quarter_day

class year_quarter_day
{
    fiscal_year::year    y_;
    fiscal_year::quarter q_;
    fiscal_year::day d_;
    fiscal_year::fiscal_start fs_;

public:
    year_quarter_day() = default;
    CONSTCD11 year_quarter_day(const fiscal_year::year& y,
                               const fiscal_year::quarter& q,
                               const fiscal_year::day& d,
                               const fiscal_year::fiscal_start& fs) NOEXCEPT;
    CONSTCD11 year_quarter_day(const fiscal_year::year_quarter& yq,
                               const fiscal_year::day& d) NOEXCEPT;
    CONSTCD14 year_quarter_day(const year_quarter_day_last& yqdl) NOEXCEPT;
    CONSTCD14 year_quarter_day(const sys_days& dp,
                               const fiscal_year::fiscal_start& fs) NOEXCEPT;
    CONSTCD14 year_quarter_day(const local_days& dp,
                               const fiscal_year::fiscal_start& fs) NOEXCEPT;

    CONSTCD14 year_quarter_day& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarter_day& operator-=(const quarters& dq) NOEXCEPT;

    CONSTCD14 year_quarter_day& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarter_day& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 fiscal_year::year    year()    const NOEXCEPT;
    CONSTCD11 fiscal_year::quarter quarter() const NOEXCEPT;
    CONSTCD11 fiscal_year::day     day()     const NOEXCEPT;

    CONSTCD11 fiscal_year::fiscal_start fiscal_start() const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD14 bool ok() const NOEXCEPT;

private:
    static CONSTCD14 year_quarter_day from_days(const days& dd,
                                                const fiscal_year::fiscal_start& fs) NOEXCEPT;
};

CONSTCD11 bool operator==(const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT;
CONSTCD11 bool operator!=(const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT;
CONSTCD11 bool operator< (const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT;
CONSTCD11 bool operator> (const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT;
CONSTCD11 bool operator<=(const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT;
CONSTCD11 bool operator>=(const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT;

CONSTCD14 year_quarter_day operator+(const year_quarter_day& yqd, const quarters& dq) NOEXCEPT;
CONSTCD14 year_quarter_day operator+(const quarters& dq, const year_quarter_day& yqd) NOEXCEPT;
CONSTCD14 year_quarter_day operator-(const year_quarter_day& yqd, const quarters& dq) NOEXCEPT;

CONSTCD11 year_quarter_day operator+(const year_quarter_day& yqd, const years& dy) NOEXCEPT;
CONSTCD11 year_quarter_day operator+(const years& dy, const year_quarter_day& yqd) NOEXCEPT;
CONSTCD11 year_quarter_day operator-(const year_quarter_day& yqd, const years& dy) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter_day& yqd);

//----------------+
// Implementation |
//----------------+

// fiscal_start

CONSTCD11
inline
fiscal_start::fiscal_start(unsigned fs) NOEXCEPT
    : fs_(static_cast<decltype(fs_)>(fs))
    {}

CONSTCD11
inline
fiscal_start::operator unsigned() const NOEXCEPT
{
    return fs_;
}

CONSTCD11
inline
bool
fiscal_start::ok() const NOEXCEPT
{
    return 1 <= fs_ && fs_ <= 12;
}

CONSTCD11
inline
bool
operator==(const fiscal_start& x, const fiscal_start& y) NOEXCEPT
{
    return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator!=(const fiscal_start& x, const fiscal_start& y) NOEXCEPT
{
    return !(x == y);
}

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

CONSTCD11 inline year::year(int y) NOEXCEPT
    : y_(static_cast<decltype(y_)>(y))
    {}

CONSTCD14
inline
year&
year::operator++() NOEXCEPT
{
    ++y_;
    return *this;
}

CONSTCD14
inline
year
year::operator++(int) NOEXCEPT
{
    auto tmp(*this);
    ++(*this);
    return tmp;
}

CONSTCD14
inline
year&
year::operator--() NOEXCEPT
{
    --y_;
    return *this;
}

CONSTCD14
inline
year
year::operator--(int) NOEXCEPT
{
    auto tmp(*this);
    --(*this);
    return tmp;
}

CONSTCD14
inline
year&
year::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

CONSTCD14
inline
year&
year::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

CONSTCD11
inline
year::operator int() const NOEXCEPT
{
    return y_;
}

CONSTCD11
inline
bool
year::ok() const NOEXCEPT
{
    return y_ != std::numeric_limits<short>::min();
}

CONSTCD11
inline
bool
operator==(const year& x, const year& y) NOEXCEPT
{
    return static_cast<int>(x) == static_cast<int>(y);
}

CONSTCD11
inline
bool
operator!=(const year& x, const year& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const year& x, const year& y) NOEXCEPT
{
    return static_cast<int>(x) < static_cast<int>(y);
}

CONSTCD11
inline
bool
operator>(const year& x, const year& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const year& x, const year& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const year& x, const year& y) NOEXCEPT
{
    return !(x < y);
}

CONSTCD11
inline
years
operator-(const year& x, const year& y) NOEXCEPT
{
    return years{static_cast<int>(x) - static_cast<int>(y)};
}

CONSTCD11
inline
year
operator+(const year& x, const years& y) NOEXCEPT
{
    return year{static_cast<int>(x) + y.count()};
}

CONSTCD11
inline
year
operator+(const years& x, const year& y) NOEXCEPT
{
    return y + x;
}

CONSTCD11
inline
year
operator-(const year& x, const years& y) NOEXCEPT
{
    return year{static_cast<int>(x) - y.count()};
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year& y)
{
    date::detail::save_ostream<CharT, Traits> _(os);
    os.fill('0');
    os.flags(std::ios::dec | std::ios::internal);
    os.width(4 + (y < year{0}));
    os << static_cast<int>(y);
    return os;
}

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
inline namespace literals
{

CONSTCD11
inline
fiscal_year::year
operator "" _y(unsigned long long y) NOEXCEPT
{
    return fiscal_year::year(static_cast<int>(y));
}

CONSTCD11
inline
fiscal_year::quarter
operator "" _q(unsigned long long q) NOEXCEPT
{
    return fiscal_year::quarter(static_cast<unsigned>(q));
}

#endif  // !defined(_MSC_VER) || (_MSC_VER >= 1900)

CONSTDATA fiscal_year::quarter q1{1u};
CONSTDATA fiscal_year::quarter q2{2u};
CONSTDATA fiscal_year::quarter q3{3u};
CONSTDATA fiscal_year::quarter q4{4u};

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
}  // inline namespace literals
#endif

// year_quarter

CONSTCD11
inline
year_quarter::year_quarter(const fiscal_year::year& y,
                           const fiscal_year::quarter& q,
                           const fiscal_year::fiscal_start& fs) NOEXCEPT
    : y_(y)
    , q_(q)
    , fs_(fs)
    {}

CONSTCD14
inline
year_quarter::year_quarter(const date::year_month& ym,
                           const fiscal_year::fiscal_start& fs) NOEXCEPT
    : year_quarter(from_year_month(ym, fs))
    {}

CONSTCD11
inline
bool
year_quarter::ok() const NOEXCEPT
{
    return y_.ok() && q_.ok() && fs_.ok();
}

CONSTCD11
inline
year
year_quarter::year() const NOEXCEPT
{
    return y_;
}

CONSTCD11
inline
quarter
year_quarter::quarter() const NOEXCEPT
{
    return q_;
}

CONSTCD11
inline
fiscal_start
year_quarter::fiscal_start() const NOEXCEPT
{
    return fs_;
}

CONSTCD14
inline
year_quarter&
year_quarter::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

CONSTCD14
inline
year_quarter&
year_quarter::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

CONSTCD14
inline
year_quarter&
year_quarter::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

CONSTCD14
inline
year_quarter&
year_quarter::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

CONSTCD14
inline
year_quarter::operator date::year_month() const NOEXCEPT
{
    const unsigned start = static_cast<unsigned>(fs_) - 1;
    const unsigned quarter = static_cast<unsigned>(q_) - 1;
    const unsigned fiscal_month = 3 * quarter;

    int year = static_cast<int>(y_);
    unsigned civil_month = start + fiscal_month;

    if (civil_month > 11) {
        civil_month -= 12;
    } else if (start != 0) {
        --year;
    }

    return date::year{year} / date::month{civil_month + 1};
}

CONSTCD14
inline
year_quarter
year_quarter::from_year_month(const date::year_month& ym, const fiscal_year::fiscal_start& fs) NOEXCEPT
{
    const unsigned start = static_cast<unsigned>(fs) - 1;
    const unsigned civil_month = static_cast<unsigned>(ym.month()) - 1;

    int year = static_cast<int>(ym.year());
    int fiscal_month = static_cast<int>(civil_month) - static_cast<int>(start);

    if (fiscal_month < 0) {
        fiscal_month += 12;
    } else if (start != 0) {
        ++year;
    }

    const unsigned quarter = static_cast<unsigned>(fiscal_month) / 3;

    return {fiscal_year::year{year}, fiscal_year::quarter{quarter + 1}, fs};
}

CONSTCD11
inline
bool
operator==(const year_quarter& x, const year_quarter& y) NOEXCEPT
{
    return x.year() == y.year() && x.quarter() == y.quarter() && x.fiscal_start() == y.fiscal_start();
}

CONSTCD11
inline
bool
operator!=(const year_quarter& x, const year_quarter& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const year_quarter& x, const year_quarter& y) NOEXCEPT
{
    return x.fiscal_start() == y.fiscal_start() ?
        x.year() < y.year() ? true
            : (x.year() > y.year() ? false
            : (x.quarter() < y.quarter()))
        : date::year_month(x) < date::year_month(y);
}

CONSTCD11
inline
bool
operator>(const year_quarter& x, const year_quarter& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const year_quarter& x, const year_quarter& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const year_quarter& x, const year_quarter& y) NOEXCEPT
{
    return !(x < y);
}

CONSTCD14
inline
year_quarter
operator+(const year_quarter& yq, const quarters& dq) NOEXCEPT
{
    auto dqi = static_cast<int>(static_cast<unsigned>(yq.quarter())) - 1 + dq.count();
    auto dy = (dqi >= 0 ? dqi : dqi - 3) / 4;
    dqi = dqi - dy * 4 + 1;
    return {(yq.year() + years(dy)), quarter(static_cast<unsigned>(dqi)), yq.fiscal_start()};
}

CONSTCD14
inline
year_quarter
operator+(const quarters& dq, const year_quarter& yq) NOEXCEPT
{
    return yq + dq;
}

CONSTCD14
inline
year_quarter
operator-(const year_quarter& yq, const quarters& dq) NOEXCEPT
{
    return yq + -dq;
}

CONSTCD11
inline
year_quarter
operator+(const year_quarter& yq, const years& dy) NOEXCEPT
{
    return {(yq.year() + dy), yq.quarter(), yq.fiscal_start()};
}

CONSTCD11
inline
year_quarter
operator+(const years& dy, const year_quarter& yq) NOEXCEPT
{
    return yq + dy;
}

CONSTCD11
inline
year_quarter
operator-(const year_quarter& yq, const years& dy) NOEXCEPT
{
    return yq + -dy;
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter& yq)
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

CONSTCD11
inline
year_quarter_day_last::year_quarter_day_last(const fiscal_year::year& y,
                                             const fiscal_year::quarter& q,
                                             const fiscal_year::fiscal_start& fs) NOEXCEPT
    : y_(y)
    , q_(q)
    , fs_(fs)
    {}

CONSTCD11
inline
year_quarter_day_last::year_quarter_day_last(const fiscal_year::year_quarter& yq) NOEXCEPT
    : y_(yq.year())
    , q_(yq.quarter())
    , fs_(yq.fiscal_start())
    {}

CONSTCD14
inline
year_quarter_day_last&
year_quarter_day_last::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

CONSTCD14
inline
year_quarter_day_last&
year_quarter_day_last::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

CONSTCD14
inline
year_quarter_day_last&
year_quarter_day_last::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

CONSTCD14
inline
year_quarter_day_last&
year_quarter_day_last::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

CONSTCD11
inline
year
year_quarter_day_last::year() const NOEXCEPT
{
    return y_;
}

CONSTCD11
inline
quarter
year_quarter_day_last::quarter() const NOEXCEPT
{
    return q_;
}

CONSTCD14
inline
day
year_quarter_day_last::day() const NOEXCEPT
{
    CONSTDATA fiscal_year::day q_day[] = {
        fiscal_year::day(90u), fiscal_year::day(89u), fiscal_year::day(92u),
        fiscal_year::day(91u), fiscal_year::day(92u), fiscal_year::day(92u),
        fiscal_year::day(92u), fiscal_year::day(92u), fiscal_year::day(91u),
        fiscal_year::day(92u), fiscal_year::day(92u), fiscal_year::day(90u)
    };
    CONSTDATA fiscal_year::day q_day_leap[] = {
        fiscal_year::day(91u), fiscal_year::day(90u), fiscal_year::day(92u),
        fiscal_year::day(91u), fiscal_year::day(92u), fiscal_year::day(92u),
        fiscal_year::day(92u), fiscal_year::day(92u), fiscal_year::day(91u),
        fiscal_year::day(92u), fiscal_year::day(92u), fiscal_year::day(91u)
    };

    const fiscal_year::year_quarter yq{y_, q_, fs_};
    const date::year_month ym{yq};
    const unsigned m = static_cast<unsigned>(ym.month()) - 1;

    return ym.year().is_leap() ? q_day_leap[m] : q_day[m];
}

CONSTCD11
inline
fiscal_start
year_quarter_day_last::fiscal_start() const NOEXCEPT
{
    return fs_;
}

CONSTCD14
inline
year_quarter_day_last::operator sys_days() const NOEXCEPT
{
    return sys_days(year_quarter_day{year(), quarter(), day(), fiscal_start()});
}

CONSTCD14
inline
year_quarter_day_last::operator local_days() const NOEXCEPT
{
    return local_days(year_quarter_day{year(), quarter(), day(), fiscal_start()});
}

CONSTCD11
inline
bool
year_quarter_day_last::ok() const NOEXCEPT
{
    return y_.ok() && q_.ok() && fs_.ok();
}

CONSTCD11
inline
bool
operator==(const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT
{
    return x.year() == y.year() && x.quarter() == y.quarter() && x.fiscal_start() == y.fiscal_start();
}

CONSTCD11
inline
bool
operator!=(const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT
{
    return x.fiscal_start() == y.fiscal_start() ?
        x.year() < y.year() ? true
            : (x.year() > y.year() ? false
            : x.quarter() < y.quarter())
            : (year_quarter_day{x.year(), x.quarter(), x.day(), x.fiscal_start()} <
               year_quarter_day{y.year(), y.quarter(), y.day(), y.fiscal_start()});
}

CONSTCD11
inline
bool
operator>(const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const year_quarter_day_last& x, const year_quarter_day_last& y) NOEXCEPT
{
    return !(x < y);
}

CONSTCD14
inline
year_quarter_day_last
operator+(const year_quarter_day_last& yqdl, const quarters& dq) NOEXCEPT
{
    return {year_quarter{yqdl.year(), yqdl.quarter(), yqdl.fiscal_start()} + dq};
}

CONSTCD14
inline
year_quarter_day_last
operator+(const quarters& dq, const year_quarter_day_last& yqdl)  NOEXCEPT
{
    return yqdl + dq;
}

CONSTCD14
inline
year_quarter_day_last
operator-(const year_quarter_day_last& yqdl, const quarters& dq)  NOEXCEPT
{
    return yqdl + -dq;
}

CONSTCD11
inline
year_quarter_day_last
operator+(const year_quarter_day_last& yqdl, const years& dy)  NOEXCEPT
{
    return {yqdl.year() + dy, yqdl.quarter(), yqdl.fiscal_start()};
}

CONSTCD11
inline
year_quarter_day_last
operator+(const years& dy, const year_quarter_day_last& yqdl)  NOEXCEPT
{
    return yqdl + dy;
}

CONSTCD11
inline
year_quarter_day_last
operator-(const year_quarter_day_last& yqdl, const years& dy)  NOEXCEPT
{
    return yqdl + -dy;
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter_day_last& yqdl)
{
    return os << yqdl.year() << "-" << yqdl.quarter() << "-last";
}

// year_quarter_day

CONSTCD11
inline
year_quarter_day::year_quarter_day(const fiscal_year::year& y,
                                   const fiscal_year::quarter& q,
                                   const fiscal_year::day& d,
                                   const fiscal_year::fiscal_start& fs) NOEXCEPT
    : y_(y)
    , q_(q)
    , d_(d)
    , fs_(fs)
    {}

CONSTCD11
inline
year_quarter_day::year_quarter_day(const fiscal_year::year_quarter& yq,
                                   const fiscal_year::day& d) NOEXCEPT
    : y_(yq.year())
    , q_(yq.quarter())
    , d_(d)
    , fs_(yq.fiscal_start())
    {}

CONSTCD14
inline
year_quarter_day::year_quarter_day(const year_quarter_day_last& yqdl) NOEXCEPT
    : y_(yqdl.year())
    , q_(yqdl.quarter())
    , d_(yqdl.day())
    , fs_(yqdl.fiscal_start())
    {}

CONSTCD14
inline
year_quarter_day::year_quarter_day(const sys_days& dp,
                                   const fiscal_year::fiscal_start& fs) NOEXCEPT
    : year_quarter_day(from_days(dp.time_since_epoch(), fs))
    {}

CONSTCD14
inline
year_quarter_day::year_quarter_day(const local_days& dp,
                                   const fiscal_year::fiscal_start& fs) NOEXCEPT
    : year_quarter_day(from_days(dp.time_since_epoch(), fs))
    {}

CONSTCD14
inline
year_quarter_day&
year_quarter_day::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

CONSTCD14
inline
year_quarter_day&
year_quarter_day::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

CONSTCD14
inline
year_quarter_day&
year_quarter_day::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

CONSTCD14
inline
year_quarter_day&
year_quarter_day::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

CONSTCD11
inline
year
year_quarter_day::year() const NOEXCEPT
{
    return y_;
}

CONSTCD11
inline
quarter
year_quarter_day::quarter() const NOEXCEPT
{
    return q_;
}

CONSTCD11
inline
day
year_quarter_day::day() const NOEXCEPT
{
    return d_;
}

CONSTCD11
inline
fiscal_start
year_quarter_day::fiscal_start() const NOEXCEPT
{
    return fs_;
}

CONSTCD14
inline
year_quarter_day::operator sys_days() const NOEXCEPT
{
    const fiscal_year::year_quarter yq{y_, q_, fs_};
    const date::year_month ym_start{yq};
    const date::year_month_day ymd_start{ym_start / date::day{1}};
    return date::sys_days{ymd_start} + date::days{static_cast<unsigned>(d_) - 1};
}

CONSTCD14
inline
year_quarter_day::operator local_days() const NOEXCEPT
{
    const fiscal_year::year_quarter yq{y_, q_, fs_};
    const date::year_month ym_start{yq};
    const date::year_month_day ymd_start{ym_start / date::day{1}};
    return date::local_days{ymd_start} + date::days{static_cast<unsigned>(d_) - 1};
}

CONSTCD14
inline
bool
year_quarter_day::ok() const NOEXCEPT
{
    return y_.ok() && d_.ok() && fs_.ok() && fiscal_year::day{1u} <= d_ && d_ <= year_quarter_day_last{y_, q_, fs_}.day();
}

CONSTCD14
inline
year_quarter_day
year_quarter_day::from_days(const days& dd, const fiscal_year::fiscal_start& fs) NOEXCEPT
{
    const date::sys_days dp{dd};
    const date::year_month_day ymd{dp};

    const date::year_month ym{ymd.year() / ymd.month()};
    const year_quarter yq(ym, fs);

    // Find day of quarter as number of days from start of quarter
    const fiscal_year::year_quarter_day yqd_start{yq, fiscal_year::day{1u}};
    const days dd_quarter = dp - sys_days{yqd_start};
    const fiscal_year::day d{static_cast<unsigned>(dd_quarter.count()) + 1};

    return {yq, d};
}

CONSTCD11
inline
bool
operator==(const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT
{
    return x.year() == y.year() &&
        x.quarter() == y.quarter() &&
        x.day() == y.day() &&
        x.fiscal_start() == y.fiscal_start();
}

CONSTCD11
inline
bool
operator!=(const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT
{
    return x.fiscal_start() == y.fiscal_start() ?
        x.year() < y.year() ? true
            : (x.year() > y.year() ? false
            : (x.quarter() < y.quarter() ? true
            : (x.quarter() > y.quarter() ? false
            : (x.day() < y.day()))))
        : sys_days(x) < sys_days(y);
}

CONSTCD11
inline
bool
operator>(const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const year_quarter_day& x, const year_quarter_day& y) NOEXCEPT
{
    return !(x < y);
}

CONSTCD14
inline
year_quarter_day
operator+(const year_quarter_day& yqd, const quarters& dq)  NOEXCEPT
{
    return {year_quarter{yqd.year(), yqd.quarter(), yqd.fiscal_start()} + dq, yqd.day()};
}

CONSTCD14
inline
year_quarter_day
operator+(const quarters& dq, const year_quarter_day& yqd)  NOEXCEPT
{
    return yqd + dq;
}

CONSTCD14
inline
year_quarter_day
operator-(const year_quarter_day& yqd, const quarters& dq)  NOEXCEPT
{
    return yqd + -dq;
}

CONSTCD11
inline
year_quarter_day
operator+(const year_quarter_day& yqd, const years& dy)  NOEXCEPT
{
    return {yqd.year() + dy, yqd.quarter(), yqd.day(), yqd.fiscal_start()};
}

CONSTCD11
inline
year_quarter_day
operator+(const years& dy, const year_quarter_day& yqd)  NOEXCEPT
{
    return yqd + dy;
}

CONSTCD11
inline
year_quarter_day
operator-(const year_quarter_day& yqd, const years& dy)  NOEXCEPT
{
    return yqd + -dy;
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarter_day& yqd)
{
    return os << yqd.year() << '-' << yqd.quarter() << '-' << yqd.day();
}

}  // namespace fiscal_year

#endif  // FISCAL_YEAR_H
