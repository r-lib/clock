#ifndef QUARTERLY_H
#define QUARTERLY_H

// The MIT License (MIT)
//
// For the original `date.h` and `iso_week.h` implementations:
// Copyright (c) 2015, 2016, 2017 Howard Hinnant
// For the `quarterly.h` extension:
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

#include <tzdb/date.h>

#include <climits>

namespace quarterly
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

class quarterday;
class quarternum;
template <start S>
class year;

template <start S>
class year_quarternum;
class quarternum_quarterday;
class quarternum_quarterday_last;

template <start S>
class year_quarternum_quarterday;
template <start S>
class year_quarternum_quarterday_last;

// date composition operators

template <start S>
CONSTCD11 year_quarternum<S> operator/(const year<S>& y, const quarternum& qn) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum<S> operator/(const year<S>& y, int               qn) NOEXCEPT;

CONSTCD11 quarternum_quarterday operator/(const quarternum& qn, const quarterday& qd) NOEXCEPT;
CONSTCD11 quarternum_quarterday operator/(const quarternum& qn, int               qd) NOEXCEPT;
CONSTCD11 quarternum_quarterday operator/(int               qn, const quarterday& qd) NOEXCEPT;
CONSTCD11 quarternum_quarterday operator/(const quarterday& qd, const quarternum& qn) NOEXCEPT;
CONSTCD11 quarternum_quarterday operator/(const quarterday& qd, int               qn) NOEXCEPT;

CONSTCD11 quarternum_quarterday_last operator/(const quarternum& qn, last_spec) NOEXCEPT;
CONSTCD11 quarternum_quarterday_last operator/(int               qn, last_spec) NOEXCEPT;
CONSTCD11 quarternum_quarterday_last operator/(last_spec, const quarternum& qn) NOEXCEPT;
CONSTCD11 quarternum_quarterday_last operator/(last_spec, int               qn) NOEXCEPT;

template <start S>
CONSTCD11 year_quarternum_quarterday<S> operator/(const year_quarternum<S>& yqn, const quarterday& qd) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_quarterday<S> operator/(const year_quarternum<S>& yqn, int               qd) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_quarterday<S> operator/(const year<S>& y, const quarternum_quarterday& qnqd) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_quarterday<S> operator/(const quarternum_quarterday& qnqd, const year<S>& y) NOEXCEPT;

template <start S>
CONSTCD11 year_quarternum_quarterday_last<S> operator/(const year_quarternum<S>& yqn, last_spec) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_quarterday_last<S> operator/(const year<S>& y, const quarternum_quarterday_last& qnqdl) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_quarterday_last<S> operator/(const quarternum_quarterday_last& qnqdl, const year<S>& y) NOEXCEPT;

// quarterday

class quarterday
{
    unsigned char qd_;
public:
    quarterday() = default;
    explicit CONSTCD11 quarterday(unsigned qd) NOEXCEPT;
    explicit quarterday(int) = delete;

    CONSTCD14 quarterday& operator++()    NOEXCEPT;
    CONSTCD14 quarterday  operator++(int) NOEXCEPT;
    CONSTCD14 quarterday& operator--()    NOEXCEPT;
    CONSTCD14 quarterday  operator--(int) NOEXCEPT;

    CONSTCD14 quarterday& operator+=(const days& dd) NOEXCEPT;
    CONSTCD14 quarterday& operator-=(const days& dd) NOEXCEPT;

    CONSTCD11 explicit operator unsigned() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const quarterday& x, const quarterday& y) NOEXCEPT;
CONSTCD11 bool operator!=(const quarterday& x, const quarterday& y) NOEXCEPT;
CONSTCD11 bool operator< (const quarterday& x, const quarterday& y) NOEXCEPT;
CONSTCD11 bool operator> (const quarterday& x, const quarterday& y) NOEXCEPT;
CONSTCD11 bool operator<=(const quarterday& x, const quarterday& y) NOEXCEPT;
CONSTCD11 bool operator>=(const quarterday& x, const quarterday& y) NOEXCEPT;

CONSTCD11 quarterday  operator+(const quarterday& x, const days& y)       NOEXCEPT;
CONSTCD11 quarterday  operator+(const days& x,       const quarterday& y) NOEXCEPT;
CONSTCD11 quarterday  operator-(const quarterday& x, const days& y)       NOEXCEPT;
CONSTCD11 days        operator-(const quarterday& x, const quarterday& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarterday& qd);

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

    CONSTCD11 quarterly::start start() const NOEXCEPT;

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
    quarterly::year<S> y_;
    quarterly::quarternum qn_;

public:
    year_quarternum<S>() = default;
    CONSTCD11 year_quarternum<S>(const quarterly::year<S>& y,
                                 const quarterly::quarternum& qn) NOEXCEPT;

    CONSTCD11 quarterly::year<S> year() const NOEXCEPT;
    CONSTCD11 quarterly::quarternum quarternum() const NOEXCEPT;

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

// quarternum_quarterday

class quarternum_quarterday
{
    quarterly::quarternum qn_;
    quarterly::quarterday qd_;

public:
    quarternum_quarterday() = default;
    CONSTCD11 quarternum_quarterday(const quarterly::quarternum& qn,
                                    const quarterly::quarterday& qd) NOEXCEPT;

    CONSTCD11 quarterly::quarternum quarternum() const NOEXCEPT;
    CONSTCD11 quarterly::quarterday quarterday() const NOEXCEPT;

    CONSTCD14 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT;
CONSTCD11 bool operator!=(const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT;
CONSTCD11 bool operator< (const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT;
CONSTCD11 bool operator> (const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT;
CONSTCD11 bool operator<=(const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT;
CONSTCD11 bool operator>=(const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarternum_quarterday& qnqd);

// quarternum_quarterday_last

class quarternum_quarterday_last
{
    quarterly::quarternum qn_;

public:
    quarternum_quarterday_last() = default;
    CONSTCD11 explicit quarternum_quarterday_last(const quarterly::quarternum& qn) NOEXCEPT;

    CONSTCD11 quarterly::quarternum quarternum() const NOEXCEPT;

    CONSTCD14 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT;
CONSTCD11 bool operator!=(const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT;
CONSTCD11 bool operator< (const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT;
CONSTCD11 bool operator> (const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT;
CONSTCD11 bool operator<=(const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT;
CONSTCD11 bool operator>=(const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarternum_quarterday_last& qnqdl);

// year_quarternum_quarterday_last

template <start S>
class year_quarternum_quarterday_last
{
    quarterly::year<S> y_;
    quarterly::quarternum qn_;

public:
    year_quarternum_quarterday_last<S>() = default;
    CONSTCD11 year_quarternum_quarterday_last<S>(const quarterly::year<S>& y,
                                                 const quarterly::quarternum& qn) NOEXCEPT;
    CONSTCD11 year_quarternum_quarterday_last<S>(const quarterly::year_quarternum<S>& yqn) NOEXCEPT;

    CONSTCD14 year_quarternum_quarterday_last<S>& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarternum_quarterday_last<S>& operator-=(const quarters& dq) NOEXCEPT;

    CONSTCD14 year_quarternum_quarterday_last<S>& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarternum_quarterday_last<S>& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 quarterly::year<S> year() const NOEXCEPT;
    CONSTCD11 quarterly::quarternum quarternum() const NOEXCEPT;
    CONSTCD14 quarterly::quarterday quarterday() const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT;

template <start S>
CONSTCD14 year_quarternum_quarterday_last<S> operator+(const year_quarternum_quarterday_last<S>& yqnqdl, const quarters& dq) NOEXCEPT;
template <start S>
CONSTCD14 year_quarternum_quarterday_last<S> operator+(const quarters& dq, const year_quarternum_quarterday_last<S>& yqnqdl) NOEXCEPT;
template <start S>
CONSTCD14 year_quarternum_quarterday_last<S> operator-(const year_quarternum_quarterday_last<S>& yqnqdl, const quarters& dq) NOEXCEPT;

template <start S>
CONSTCD11 year_quarternum_quarterday_last<S> operator+(const year_quarternum_quarterday_last<S>& yqnqdl, const years& dy) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_quarterday_last<S> operator+(const years& dy, const year_quarternum_quarterday_last<S>& yqnqdl) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_quarterday_last<S> operator-(const year_quarternum_quarterday_last<S>& yqnqdl, const years& dy) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarternum_quarterday_last<S>& yqnqdl);

// year_quarternum_quarterday

template <start S>
class year_quarternum_quarterday
{
    quarterly::year<S> y_;
    quarterly::quarternum qn_;
    quarterly::quarterday qd_;

public:
    year_quarternum_quarterday<S>() = default;
    CONSTCD11 year_quarternum_quarterday<S>(const quarterly::year<S>& y,
                                            const quarterly::quarternum& qn,
                                            const quarterly::quarterday& qd) NOEXCEPT;
    CONSTCD11 year_quarternum_quarterday<S>(const quarterly::year_quarternum<S>& yqn,
                                            const quarterly::quarterday& qd) NOEXCEPT;
    CONSTCD14 year_quarternum_quarterday<S>(const year_quarternum_quarterday_last<S>& yqnqdl) NOEXCEPT;
    CONSTCD14 year_quarternum_quarterday<S>(const sys_days& dp) NOEXCEPT;
    CONSTCD14 year_quarternum_quarterday<S>(const local_days& dp) NOEXCEPT;

    CONSTCD14 year_quarternum_quarterday<S>& operator+=(const quarters& dq) NOEXCEPT;
    CONSTCD14 year_quarternum_quarterday<S>& operator-=(const quarters& dq) NOEXCEPT;

    CONSTCD14 year_quarternum_quarterday<S>& operator+=(const years& dy) NOEXCEPT;
    CONSTCD14 year_quarternum_quarterday<S>& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 quarterly::year<S> year() const NOEXCEPT;
    CONSTCD11 quarterly::quarternum quarternum() const NOEXCEPT;
    CONSTCD11 quarterly::quarterday quarterday() const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD14 bool ok() const NOEXCEPT;

private:
    CONSTCD14 days to_days() const NOEXCEPT;
    static CONSTCD14 year_quarternum_quarterday<S> from_days(const days& dd) NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT;

template <start S>
CONSTCD14 year_quarternum_quarterday<S> operator+(const year_quarternum_quarterday<S>& yqnqd, const quarters& dq) NOEXCEPT;
template <start S>
CONSTCD14 year_quarternum_quarterday<S> operator+(const quarters& dq, const year_quarternum_quarterday<S>& yqnqd) NOEXCEPT;
template <start S>
CONSTCD14 year_quarternum_quarterday<S> operator-(const year_quarternum_quarterday<S>& yqnqd, const quarters& dq) NOEXCEPT;

template <start S>
CONSTCD11 year_quarternum_quarterday<S> operator+(const year_quarternum_quarterday<S>& yqnqd, const years& dy) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_quarterday<S> operator+(const years& dy, const year_quarternum_quarterday<S>& yqnqd) NOEXCEPT;
template <start S>
CONSTCD11 year_quarternum_quarterday<S> operator-(const year_quarternum_quarterday<S>& yqnqd, const years& dy) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarternum_quarterday<S>& yqnqd);

//----------------+
// Implementation |
//----------------+

// quarterday

CONSTCD11
inline
quarterday::quarterday(unsigned qd) NOEXCEPT
    : qd_(static_cast<decltype(qd_)>(qd))
    {}

CONSTCD14
inline
quarterday&
quarterday::operator++() NOEXCEPT
{
    ++qd_;
    return *this;
}

CONSTCD14
inline
quarterday
quarterday::operator++(int) NOEXCEPT
{
    auto tmp(*this);
    ++(*this);
    return tmp;
}

CONSTCD14
inline
quarterday&
quarterday::operator--() NOEXCEPT
{
    --qd_;
    return *this;
}

CONSTCD14
inline
quarterday
quarterday::operator--(int) NOEXCEPT
{
    auto tmp(*this);
    --(*this);
    return tmp;
}

CONSTCD14
inline
quarterday&
quarterday::operator+=(const days& dd) NOEXCEPT
{
    *this = *this + dd;
    return *this;
}

CONSTCD14
inline
quarterday&
quarterday::operator-=(const days& dd) NOEXCEPT
{
    *this = *this - dd;
    return *this;
}

CONSTCD11
inline
quarterday::operator unsigned() const NOEXCEPT
{
    return qd_;
}

CONSTCD11
inline
bool
quarterday::ok() const NOEXCEPT
{
    // 92 from a quarter triplet with [31, 31, 30] days
    return 1 <= qd_ && qd_ <= 92;
}

CONSTCD11
inline
bool
operator==(const quarterday& x, const quarterday& y) NOEXCEPT
{
    return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator!=(const quarterday& x, const quarterday& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const quarterday& x, const quarterday& y) NOEXCEPT
{
    return static_cast<unsigned>(x) < static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator>(const quarterday& x, const quarterday& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const quarterday& x, const quarterday& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const quarterday& x, const quarterday& y) NOEXCEPT
{
    return !(x < y);
}

CONSTCD11
inline
days
operator-(const quarterday& x, const quarterday& y) NOEXCEPT
{
    return days{static_cast<days::rep>(static_cast<unsigned>(x) - static_cast<unsigned>(y))};
}

CONSTCD11
inline
quarterday
operator+(const quarterday& x, const days& y) NOEXCEPT
{
    return quarterday{static_cast<unsigned>(x) + static_cast<unsigned>(y.count())};
}

CONSTCD11
inline
quarterday
operator+(const days& x, const quarterday& y) NOEXCEPT
{
    return y + x;
}

CONSTCD11
inline
quarterday
operator-(const quarterday& x, const days& y) NOEXCEPT
{
    return x + -y;
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarterday& qd)
{
    date::detail::save_ostream<CharT, Traits> _(os);
    os.fill('0');
    os.flags(std::ios::dec | std::ios::right);
    os.width(2);
    os << static_cast<unsigned>(qd);
    if (!qd.ok())
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
quarterly::start
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
year_quarternum<S>::year_quarternum(const quarterly::year<S>& y,
                                    const quarterly::quarternum& qn) NOEXCEPT
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

// quarternum_quarterday

CONSTCD11
inline
quarternum_quarterday::quarternum_quarterday(const quarterly::quarternum& qn,
                                             const quarterly::quarterday& qd) NOEXCEPT
    : qn_(qn)
    , qd_(qd)
    {}

CONSTCD11
inline
quarternum
quarternum_quarterday::quarternum() const NOEXCEPT
{
    return qn_;
}

CONSTCD11
inline
quarterday
quarternum_quarterday::quarterday() const NOEXCEPT
{
    return qd_;
}

CONSTCD14
inline
bool
quarternum_quarterday::ok() const NOEXCEPT
{
    return qn_.ok() && qd_.ok();
}

CONSTCD11
inline
bool
operator==(const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT
{
    return x.quarternum() == y.quarternum() && x.quarterday() == y.quarterday();
}

CONSTCD11
inline
bool
operator!=(const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT
{
    return x.quarternum() < y.quarternum() ? true
        : (x.quarternum() > y.quarternum() ? false
        : (static_cast<unsigned>(x.quarterday()) < static_cast<unsigned>(y.quarterday())));
}

CONSTCD11
inline
bool
operator>(const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const quarternum_quarterday& x, const quarternum_quarterday& y) NOEXCEPT
{
    return !(x < y);
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarternum_quarterday& qnqd)
{
    return os << qnqd.quarternum() << '-' << qnqd.quarterday();
}

// quarternum_quarterday_last

CONSTCD11
inline
quarternum_quarterday_last::quarternum_quarterday_last(const quarterly::quarternum& qn) NOEXCEPT
    : qn_(qn)
    {}

CONSTCD11
inline
quarternum
quarternum_quarterday_last::quarternum() const NOEXCEPT
{
    return qn_;
}

CONSTCD14
inline
bool
quarternum_quarterday_last::ok() const NOEXCEPT
{
    return qn_.ok();
}

CONSTCD11
inline
bool
operator==(const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT
{
    return x.quarternum() == y.quarternum();
}

CONSTCD11
inline
bool
operator!=(const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT
{
    return static_cast<unsigned>(x.quarternum()) < static_cast<unsigned>(y.quarternum());
}

CONSTCD11
inline
bool
operator>(const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const quarternum_quarterday_last& x, const quarternum_quarterday_last& y) NOEXCEPT
{
    return !(x < y);
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const quarternum_quarterday_last& qnqd)
{
    return os << qnqd.quarternum() << "-last";
}

// year_quarternum_quarterday_last

template <start S>
CONSTCD11
inline
year_quarternum_quarterday_last<S>::year_quarternum_quarterday_last(const quarterly::year<S>& y,
                                                                    const quarterly::quarternum& qn) NOEXCEPT
    : y_(y)
    , qn_(qn)
    {}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday_last<S>::year_quarternum_quarterday_last(const quarterly::year_quarternum<S>& yqn) NOEXCEPT
    : y_(yqn.year())
    , qn_(yqn.quarternum())
    {}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday_last<S>&
year_quarternum_quarterday_last<S>::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday_last<S>&
year_quarternum_quarterday_last<S>::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday_last<S>&
year_quarternum_quarterday_last<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday_last<S>&
year_quarternum_quarterday_last<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD11
inline
year<S>
year_quarternum_quarterday_last<S>::year() const NOEXCEPT
{
    return y_;
}

template <start S>
CONSTCD11
inline
quarternum
year_quarternum_quarterday_last<S>::quarternum() const NOEXCEPT
{
    return qn_;
}

template <start S>
CONSTCD14
inline
quarterday
year_quarternum_quarterday_last<S>::quarterday() const NOEXCEPT
{
    CONSTDATA unsigned s = static_cast<unsigned>(S) - 1;

    // Use an unsigned array rather than a quarterday array to avoid a
    // gcc warning with Rtools36's old gcc. Issue #43.
    CONSTDATA unsigned quarterdays[] = {
    //  [12, 1, 2]     [1, 2, 3]     [2, 3, 4]
        90u,           90u,          89u,
    //  [3, 4, 5]      [4, 5, 6]     [5, 6, 7]
        92u,           91u,          92u,
    //  [6, 7, 8]      [7, 8, 9]     [8, 9, 10]
        92u,           92u,          92u,
    //  [9, 10, 11]    [10, 11, 12]  [11, 12, 1]
        91u,           92u,          92u
    };

    const unsigned quarternum = static_cast<unsigned>(qn_) - 1;

    // Remap [Jan -> Dec] to [Dec -> Jan] to group quarters with February
    unsigned key = (s == 12) ? 0 : s + 1;

    key = key + 3 * quarternum;
    if (key > 11) {
        key -= 12;
    }

    if (!qn_.ok()) {
        // If `!qn_.ok()`, don't index into `quarterdays[]` to avoid OOB error.
        // Instead, return the minimum of the possible "last day of quarter"
        // days, as `year_month_day_last::day()` does.
        return quarterly::quarterday{quarterdays[2]};
    } else if (key <= 2 && y_.is_leap()) {
        return quarterly::quarterday{quarterdays[key]} + quarterly::days{1u};
    } else {
        return quarterly::quarterday{quarterdays[key]};
    }
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday_last<S>::operator sys_days() const NOEXCEPT
{
    return sys_days(year_quarternum_quarterday<S>{year(), quarternum(), quarterday()});
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday_last<S>::operator local_days() const NOEXCEPT
{
    return local_days(year_quarternum_quarterday<S>{year(), quarternum(), quarterday()});
}

template <start S>
CONSTCD11
inline
bool
year_quarternum_quarterday_last<S>::ok() const NOEXCEPT
{
    return y_.ok() && qn_.ok();
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT
{
    return x.year() == y.year() && x.quarternum() == y.quarternum();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : x.quarternum() < y.quarternum());
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_quarternum_quarterday_last<S>& x, const year_quarternum_quarterday_last<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday_last<S>
operator+(const year_quarternum_quarterday_last<S>& yqnqdl, const quarters& dq) NOEXCEPT
{
    return {year_quarternum<S>{yqnqdl.year(), yqnqdl.quarternum()} + dq};
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday_last<S>
operator+(const quarters& dq, const year_quarternum_quarterday_last<S>& yqnqdl)  NOEXCEPT
{
    return yqnqdl + dq;
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday_last<S>
operator-(const year_quarternum_quarterday_last<S>& yqnqdl, const quarters& dq)  NOEXCEPT
{
    return yqnqdl + -dq;
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday_last<S>
operator+(const year_quarternum_quarterday_last<S>& yqnqdl, const years& dy)  NOEXCEPT
{
    return {yqnqdl.year() + dy, yqnqdl.quarternum()};
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday_last<S>
operator+(const years& dy, const year_quarternum_quarterday_last<S>& yqnqdl)  NOEXCEPT
{
    return yqnqdl + dy;
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday_last<S>
operator-(const year_quarternum_quarterday_last<S>& yqnqdl, const years& dy)  NOEXCEPT
{
    return yqnqdl + -dy;
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarternum_quarterday_last<S>& yqnqdl)
{
    return os << yqnqdl.year() << "-" << yqnqdl.quarternum() << "-last";
}

// year_quarternum_quarterday

template <start S>
CONSTCD11
inline
year_quarternum_quarterday<S>::year_quarternum_quarterday(const quarterly::year<S>& y,
                                                          const quarterly::quarternum& qn,
                                                          const quarterly::quarterday& qd) NOEXCEPT
    : y_(y)
    , qn_(qn)
    , qd_(qd)
    {}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday<S>::year_quarternum_quarterday(const quarterly::year_quarternum<S>& yqn,
                                                          const quarterly::quarterday& qd) NOEXCEPT
    : y_(yqn.year())
    , qn_(yqn.quarternum())
    , qd_(qd)
    {}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>::year_quarternum_quarterday(const year_quarternum_quarterday_last<S>& yqnqdl) NOEXCEPT
    : y_(yqnqdl.year())
    , qn_(yqnqdl.quarternum())
    , qd_(yqnqdl.quarterday())
    {}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>::year_quarternum_quarterday(const sys_days& dp) NOEXCEPT
    : year_quarternum_quarterday<S>(from_days(dp.time_since_epoch()))
    {}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>::year_quarternum_quarterday(const local_days& dp) NOEXCEPT
    : year_quarternum_quarterday<S>(from_days(dp.time_since_epoch()))
    {}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>&
year_quarternum_quarterday<S>::operator+=(const quarters& dq) NOEXCEPT
{
    *this = *this + dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>&
year_quarternum_quarterday<S>::operator-=(const quarters& dq) NOEXCEPT
{
    *this = *this - dq;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>&
year_quarternum_quarterday<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>&
year_quarternum_quarterday<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD11
inline
year<S>
year_quarternum_quarterday<S>::year() const NOEXCEPT
{
    return y_;
}

template <start S>
CONSTCD11
inline
quarternum
year_quarternum_quarterday<S>::quarternum() const NOEXCEPT
{
    return qn_;
}

template <start S>
CONSTCD11
inline
quarterday
year_quarternum_quarterday<S>::quarterday() const NOEXCEPT
{
    return qd_;
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>::operator sys_days() const NOEXCEPT
{
    return date::sys_days{to_days()};
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>::operator local_days() const NOEXCEPT
{
    return date::local_days{to_days()};
}

template <start S>
CONSTCD14
inline
bool
year_quarternum_quarterday<S>::ok() const NOEXCEPT
{
    return y_.ok() &&
        qd_.ok() &&
        quarterly::quarterday{1u} <= qd_ &&
        qd_ <= year_quarternum_quarterday_last<S>{y_, qn_}.quarterday();
}

template <start S>
CONSTCD14
inline
days
year_quarternum_quarterday<S>::to_days() const NOEXCEPT
{
    CONSTDATA unsigned char s = static_cast<unsigned char>(S) - 1;
    const unsigned quarternum = static_cast<unsigned>(qn_) - 1;
    const unsigned quarterly_month = 3 * quarternum;

    int year = static_cast<int>(y_);
    unsigned civil_month = s + quarterly_month;

    if (civil_month > 11) {
        civil_month -= 12;
    } else if (s != 0) {
        --year;
    }

    const unsigned quarterday = static_cast<unsigned>(qd_) - 1;

    const date::year_month_day quarter_start{
        date::year{year} / date::month{civil_month + 1} / date::day{1}
    };

    const date::sys_days quarter_days{
        date::sys_days{quarter_start} + date::days{quarterday}
    };

    return quarter_days.time_since_epoch();
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>
year_quarternum_quarterday<S>::from_days(const days& dd) NOEXCEPT
{
    CONSTDATA unsigned char s = static_cast<unsigned char>(S) - 1;

    const date::sys_days dp{dd};
    const date::year_month_day ymd{dp};

    const unsigned civil_month = static_cast<unsigned>(ymd.month()) - 1;

    int year = static_cast<int>(ymd.year());
    int quarterly_month = static_cast<int>(civil_month) - static_cast<int>(s);

    if (quarterly_month < 0) {
        quarterly_month += 12;
    } else if (s != 0) {
        ++year;
    }

    const unsigned quarternum = static_cast<unsigned>(quarterly_month) / 3;

    const quarterly::year_quarternum_quarterday<S> quarter_start{
        quarterly::year<S>{year} / quarterly::quarternum{quarternum + 1} / quarterly::quarterday{1u}
    };

    // Find day of quarter as number of days from start of quarter
    const days days = dp - sys_days{quarter_start};
    const quarterly::quarterday quarterday{static_cast<unsigned>(days.count()) + 1};

    return quarter_start.year() / quarter_start.quarternum() / quarterday;
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT
{
    return x.year() == y.year() &&
        x.quarternum() == y.quarternum() &&
        x.quarterday() == y.quarterday();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.quarternum() < y.quarternum() ? true
        : (x.quarternum() > y.quarternum() ? false
        : (x.quarterday() < y.quarterday()))));
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_quarternum_quarterday<S>& x, const year_quarternum_quarterday<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>
operator+(const year_quarternum_quarterday<S>& yqnqd, const quarters& dq)  NOEXCEPT
{
    return {year_quarternum<S>{yqnqd.year(), yqnqd.quarternum()} + dq, yqnqd.quarterday()};
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>
operator+(const quarters& dq, const year_quarternum_quarterday<S>& yqnqd)  NOEXCEPT
{
    return yqnqd + dq;
}

template <start S>
CONSTCD14
inline
year_quarternum_quarterday<S>
operator-(const year_quarternum_quarterday<S>& yqnqd, const quarters& dq)  NOEXCEPT
{
    return yqnqd + -dq;
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday<S>
operator+(const year_quarternum_quarterday<S>& yqnqd, const years& dy)  NOEXCEPT
{
    return {yqnqd.year() + dy, yqnqd.quarternum(), yqnqd.quarterday()};
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday<S>
operator+(const years& dy, const year_quarternum_quarterday<S>& yqnqd)  NOEXCEPT
{
    return yqnqd + dy;
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday<S>
operator-(const year_quarternum_quarterday<S>& yqnqd, const years& dy)  NOEXCEPT
{
    return yqnqd + -dy;
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_quarternum_quarterday<S>& yqnqd)
{
    return os << yqnqd.year() << '-' << yqnqd.quarternum() << '-' << yqnqd.quarterday();
}

// literals

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
inline namespace literals
{
#endif  // !defined(_MSC_VER) || (_MSC_VER >= 1900)

CONSTDATA quarterly::last_spec last{};

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

// quarternum_quarterday from operator/()

CONSTCD11
inline
quarternum_quarterday
operator/(const quarternum& qn, const quarterday& qd) NOEXCEPT
{
    return {qn, qd};
}

CONSTCD11
inline
quarternum_quarterday
operator/(const quarternum& qn, int qd) NOEXCEPT
{
    return qn / quarterday(static_cast<unsigned>(qd));
}

CONSTCD11
inline
quarternum_quarterday
operator/(int qn, const quarterday& qd) NOEXCEPT
{
    return quarternum(static_cast<unsigned>(qn)) / qd;
}

CONSTCD11
inline
quarternum_quarterday
operator/(const quarterday& qd, const quarternum& qn) NOEXCEPT
{
    return qn / qd;
}

CONSTCD11
inline
quarternum_quarterday
operator/(const quarterday& qd, int qn) NOEXCEPT
{
    return qn / qd;
}

// quarternum_quarterday_last from operator/()

CONSTCD11
inline
quarternum_quarterday_last
operator/(const quarternum& qn, last_spec) NOEXCEPT
{
    return quarternum_quarterday_last{qn};
}

CONSTCD11
inline
quarternum_quarterday_last
operator/(int qn, last_spec) NOEXCEPT
{
    return quarternum(static_cast<unsigned>(qn))/last;
}

CONSTCD11
inline
quarternum_quarterday_last
operator/(last_spec, const quarternum& qn) NOEXCEPT
{
    return qn / last;
}

CONSTCD11
inline
quarternum_quarterday_last
operator/(last_spec, int qn) NOEXCEPT
{
    return qn / last;
}

// year_quarternum_quarterday from operator/()

template <start S>
CONSTCD11
inline
year_quarternum_quarterday<S>
operator/(const year_quarternum<S>& yqn, const quarterday& qd) NOEXCEPT
{
    return {yqn, qd};
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday<S>
operator/(const year_quarternum<S>& yqn, int qd) NOEXCEPT
{
    return yqn / quarterday(static_cast<unsigned>(qd));
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday<S>
operator/(const year<S>& y, const quarternum_quarterday& qnqd) NOEXCEPT
{
    return y / qnqd.quarternum() / qnqd.quarterday();
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday<S>
operator/(const quarternum_quarterday& qnqd, const year<S>& y) NOEXCEPT
{
    return y / qnqd;
}

// year_quarternum_quarterday_last from operator/()

template <start S>
CONSTCD11
inline
year_quarternum_quarterday_last<S>
operator/(const year_quarternum<S>& ynq, last_spec) NOEXCEPT
{
    return year_quarternum_quarterday_last<S>{ynq};
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday_last<S>
operator/(const year<S>& y, const quarternum_quarterday_last& qnqdl) NOEXCEPT
{
    return {y, qnqdl};
}

template <start S>
CONSTCD11
inline
year_quarternum_quarterday_last<S>
operator/(const quarternum_quarterday_last& qnqdl, const year<S>& y) NOEXCEPT
{
    return y / qnqdl;
}

}  // namespace quarterly

#endif  // QUARTERLY_H
