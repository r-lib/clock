#ifndef WEEK_H
#define WEEK_H

// The MIT License (MIT)
//
// For the original `date.h` implementation:
// Copyright (c) 2015, 2016, 2017 Howard Hinnant
// For the `week.h` extension:
// Copyright (c) 2023 Davis Vaughan
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

namespace week
{

// y/wn/wd
// wn/wd/y
// wd/wn/y

using days = date::days;
using weeks = date::weeks;
using years = date::years;

// time_point

using sys_days = date::sys_days;
using local_days = date::local_days;

// types

struct last_week
{
    explicit last_week() = default;
};

enum class start: unsigned char {
    sunday = 0u,
    monday = 1u,
    tuesday = 2u,
    wednesday = 3u,
    thursday = 4u,
    friday = 5u,
    saturday = 6u
};

using weekday = date::weekday;
class weeknum;
template <start S>
class year;

template <start S>
class year_weeknum;
template <start S>
class year_lastweek;
class weeknum_weekday;
class lastweek_weekday;

template <start S>
class year_weeknum_weekday;
template <start S>
class year_lastweek_weekday;

// date composition operators

template <start S>
CONSTCD11 year_weeknum<S> operator/(const year<S>& y, const weeknum& wn) NOEXCEPT;
template <start S>
CONSTCD11 year_weeknum<S> operator/(const year<S>& y, int            wn) NOEXCEPT;

template <start S>
CONSTCD11 year_lastweek<S> operator/(const year<S>& y, last_week      wn) NOEXCEPT;

CONSTCD11 weeknum_weekday operator/(const weeknum& wn, const weekday& wd) NOEXCEPT;
CONSTCD11 weeknum_weekday operator/(const weeknum& wn, int            wd) NOEXCEPT;
CONSTCD11 weeknum_weekday operator/(const weekday& wd, const weeknum& wn) NOEXCEPT;
CONSTCD11 weeknum_weekday operator/(const weekday& wd, int            wn) NOEXCEPT;

CONSTCD11 lastweek_weekday operator/(const last_week& wn, const weekday& wd) NOEXCEPT;
CONSTCD11 lastweek_weekday operator/(const last_week& wn, int            wd) NOEXCEPT;
CONSTCD11 lastweek_weekday operator/(const weekday& wd, const last_week& wn) NOEXCEPT;

template <start S>
CONSTCD11 year_weeknum_weekday<S> operator/(const year_weeknum<S>& ywn, const weekday& wd) NOEXCEPT;
template <start S>
CONSTCD11 year_weeknum_weekday<S> operator/(const year_weeknum<S>& ywn, int            wd) NOEXCEPT;
template <start S>
CONSTCD11 year_weeknum_weekday<S> operator/(const weeknum_weekday& wnwd, const year<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 year_weeknum_weekday<S> operator/(const weeknum_weekday& wnwd, int            y) NOEXCEPT;

template <start S>
CONSTCD11 year_lastweek_weekday<S> operator/(const year_lastweek<S>& ylw, const weekday& wd) NOEXCEPT;
template <start S>
CONSTCD11 year_lastweek_weekday<S> operator/(const year_lastweek<S>& ylw, int            wd) NOEXCEPT;

template <start S>
CONSTCD11 year_lastweek_weekday<S> operator/(const lastweek_weekday& lwwd, const year<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 year_lastweek_weekday<S> operator/(const lastweek_weekday& lwwd, int            y) NOEXCEPT;

// year

template <start S>
class year
{
    short y_;

public:
    explicit CONSTCD11 year(int y) NOEXCEPT;

    year& operator++()    NOEXCEPT;
    year  operator++(int) NOEXCEPT;
    year& operator--()    NOEXCEPT;
    year  operator--(int) NOEXCEPT;

    year& operator+=(const years& y) NOEXCEPT;
    year& operator-=(const years& y) NOEXCEPT;

    CONSTCD14 bool is_leap() const NOEXCEPT;

    CONSTCD11 explicit operator int() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;

    static CONSTCD11 year min() NOEXCEPT;
    static CONSTCD11 year max() NOEXCEPT;
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
CONSTCD11 year<S> operator+(const year<S>&  x, const years&    y) NOEXCEPT;
template <start S>
CONSTCD11 year<S> operator+(const years&    x, const year<S>&  y) NOEXCEPT;
template <start S>
CONSTCD11 year<S> operator-(const year<S>&  x, const years&    y) NOEXCEPT;
template <start S>
CONSTCD11 years   operator-(const year<S>&  x, const year<S>&  y) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year<S>& y);

// weeknum

class weeknum
{
    unsigned char wn_;

public:
    explicit CONSTCD11 weeknum(unsigned wn) NOEXCEPT;

    weeknum& operator++()    NOEXCEPT;
    weeknum  operator++(int) NOEXCEPT;
    weeknum& operator--()    NOEXCEPT;
    weeknum  operator--(int) NOEXCEPT;

    weeknum& operator+=(const weeks& y) NOEXCEPT;
    weeknum& operator-=(const weeks& y) NOEXCEPT;

    CONSTCD11 explicit operator unsigned() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const weeknum& x, const weeknum& y) NOEXCEPT;
CONSTCD11 bool operator!=(const weeknum& x, const weeknum& y) NOEXCEPT;
CONSTCD11 bool operator< (const weeknum& x, const weeknum& y) NOEXCEPT;
CONSTCD11 bool operator> (const weeknum& x, const weeknum& y) NOEXCEPT;
CONSTCD11 bool operator<=(const weeknum& x, const weeknum& y) NOEXCEPT;
CONSTCD11 bool operator>=(const weeknum& x, const weeknum& y) NOEXCEPT;

CONSTCD11 weeknum  operator+(const weeknum& x, const weeks&   y) NOEXCEPT;
CONSTCD11 weeknum  operator+(const weeks&   x, const weeknum& y) NOEXCEPT;
CONSTCD11 weeknum  operator-(const weeknum& x, const weeks&   y) NOEXCEPT;
CONSTCD11 weeks    operator-(const weeknum& x, const weeknum& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const weeknum& wn);

// year_weeknum

template <start S>
class year_weeknum
{
    week::year<S> y_;
    week::weeknum wn_;

public:
    CONSTCD11 year_weeknum(const week::year<S>& y, const week::weeknum& wn) NOEXCEPT;

    CONSTCD11 week::year<S> year()    const NOEXCEPT;
    CONSTCD11 week::weeknum weeknum() const NOEXCEPT;

    year_weeknum& operator+=(const years& dy) NOEXCEPT;
    year_weeknum& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 bool ok() const NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT;

template <start S>
CONSTCD11 year_weeknum<S> operator+(const year_weeknum<S>& ym, const years&           dy) NOEXCEPT;
template <start S>
CONSTCD11 year_weeknum<S> operator+(const years&           dy, const year_weeknum<S>& ym) NOEXCEPT;
template <start S>
CONSTCD11 year_weeknum<S> operator-(const year_weeknum<S>& ym, const years&           dy) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_weeknum<S>& ym);

// year_lastweek

template <start S>
class year_lastweek
{
    week::year<S> y_;

public:
    CONSTCD11 explicit year_lastweek(const week::year<S>& y) NOEXCEPT;

    CONSTCD11 week::year<S> year()    const NOEXCEPT;
    CONSTCD14 week::weeknum weeknum() const NOEXCEPT;

    year_lastweek& operator+=(const years& dy) NOEXCEPT;
    year_lastweek& operator-=(const years& dy) NOEXCEPT;

    CONSTCD11 bool ok() const NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT;

template <start S>
CONSTCD11 year_lastweek<S> operator+(const year_lastweek<S>& ym, const years&            dy) NOEXCEPT;
template <start S>
CONSTCD11 year_lastweek<S> operator+(const years&            dy, const year_lastweek<S>& ym) NOEXCEPT;
template <start S>
CONSTCD11 year_lastweek<S> operator-(const year_lastweek<S>& ym, const years&            dy) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_lastweek<S>& ym);

// weeknum_weekday

class weeknum_weekday
{
    week::weeknum wn_;
    week::weekday wd_;

public:
    CONSTCD11 weeknum_weekday(const week::weeknum& wn,
                              const week::weekday& wd) NOEXCEPT;

    CONSTCD11 week::weeknum weeknum() const NOEXCEPT;
    CONSTCD11 week::weekday weekday() const NOEXCEPT;

    CONSTCD14 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT;
CONSTCD11 bool operator!=(const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT;
//CONSTCD11 bool operator< (const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT;
//CONSTCD11 bool operator> (const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT;
//CONSTCD11 bool operator<=(const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT;
//CONSTCD11 bool operator>=(const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const weeknum_weekday& md);

// lastweek_weekday

class lastweek_weekday
{
    week::weekday wd_;

public:
    CONSTCD11 explicit lastweek_weekday(const week::weekday& wd) NOEXCEPT;

    CONSTCD11 week::weekday weekday() const NOEXCEPT;

    CONSTCD14 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT;
CONSTCD11 bool operator!=(const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT;
//CONSTCD11 bool operator< (const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT;
//CONSTCD11 bool operator> (const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT;
//CONSTCD11 bool operator<=(const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT;
//CONSTCD11 bool operator>=(const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const lastweek_weekday& md);

// year_lastweek_weekday

template <start S>
class year_lastweek_weekday
{
    week::year<S> y_;
    week::weekday wd_;

public:
    CONSTCD11 year_lastweek_weekday(const week::year<S>& y,
                                    const week::weekday& wd) NOEXCEPT;

    year_lastweek_weekday& operator+=(const years& y) NOEXCEPT;
    year_lastweek_weekday& operator-=(const years& y) NOEXCEPT;

    CONSTCD11 week::year<S> year()    const NOEXCEPT;
    CONSTCD14 week::weeknum weeknum() const NOEXCEPT;
    CONSTCD11 week::weekday weekday() const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT;

template <start S>
CONSTCD11 year_lastweek_weekday<S> operator+(const year_lastweek_weekday<S>& ywnwd, const years& y) NOEXCEPT;
template <start S>
CONSTCD11 year_lastweek_weekday<S> operator+(const years& y, const year_lastweek_weekday<S>& ywnwd) NOEXCEPT;
template <start S>
CONSTCD11 year_lastweek_weekday<S> operator-(const year_lastweek_weekday<S>& ywnwd, const years& y) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_lastweek_weekday<S>& ywnwd);

// class year_weeknum_weekday

template <start S>
class year_weeknum_weekday
{
    week::year<S> y_;
    week::weeknum wn_;
    week::weekday wd_;

public:
    CONSTCD11 year_weeknum_weekday(const week::year<S>& y, const week::weeknum& wn,
                                   const week::weekday& wd) NOEXCEPT;
    CONSTCD14 year_weeknum_weekday(const year_lastweek_weekday<S>& ylwwd) NOEXCEPT;
    CONSTCD14 year_weeknum_weekday(const sys_days& dp) NOEXCEPT;
    CONSTCD14 explicit year_weeknum_weekday(const local_days& dp) NOEXCEPT;

    year_weeknum_weekday& operator+=(const years& y) NOEXCEPT;
    year_weeknum_weekday& operator-=(const years& y) NOEXCEPT;

    CONSTCD11 week::year<S> year()    const NOEXCEPT;
    CONSTCD11 week::weeknum weeknum() const NOEXCEPT;
    CONSTCD11 week::weekday weekday() const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD14 bool ok() const NOEXCEPT;

private:
    static CONSTCD14 year_weeknum_weekday from_days(days dp) NOEXCEPT;
};

template <start S>
CONSTCD11 bool operator==(const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator!=(const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator< (const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator> (const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator<=(const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT;
template <start S>
CONSTCD11 bool operator>=(const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT;

template <start S>
CONSTCD11 year_weeknum_weekday<S> operator+(const year_weeknum_weekday<S>& ywnwd, const years& y) NOEXCEPT;
template <start S>
CONSTCD11 year_weeknum_weekday<S> operator+(const years& y, const year_weeknum_weekday<S>& ywnwd) NOEXCEPT;
template <start S>
CONSTCD11 year_weeknum_weekday<S> operator-(const year_weeknum_weekday<S>& ywnwd, const years& y) NOEXCEPT;

template<class CharT, class Traits, start S>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_weeknum_weekday<S>& ywnwd);

//----------------+
// Implementation |
//----------------+

// year

template <start S>
CONSTCD11 inline year<S>::year(int y) NOEXCEPT : y_(static_cast<decltype(y_)>(y)) {}
template <start S>
inline year<S>& year<S>::operator++() NOEXCEPT {++y_; return *this;}
template <start S>
inline year<S> year<S>::operator++(int) NOEXCEPT {auto tmp(*this); ++(*this); return tmp;}
template <start S>
inline year<S>& year<S>::operator--() NOEXCEPT {--y_; return *this;}
template <start S>
inline year<S> year<S>::operator--(int) NOEXCEPT {auto tmp(*this); --(*this); return tmp;}
template <start S>
inline year<S>& year<S>::operator+=(const years& y) NOEXCEPT {*this = *this + y; return *this;}
template <start S>
inline year<S>& year<S>::operator-=(const years& y) NOEXCEPT {*this = *this - y; return *this;}

template <start S>
CONSTCD14
inline
bool
year<S>::is_leap() const NOEXCEPT
{
    const auto y = date::year{static_cast<int>(y_)};
    const auto middle = weekday{static_cast<unsigned char>(S)} + days{3};
    const auto s0 = sys_days((y-years{1})/12/middle[date::last]);
    const auto s1 = sys_days(y/12/middle[date::last]);
    return s1-s0 != days{7*52};
}

template <start S>
CONSTCD11 inline year<S>::operator int() const NOEXCEPT {return y_;}
template <start S>
CONSTCD11 inline bool year<S>::ok() const NOEXCEPT {return min() <= *this && *this <= max();}

template <start S>
CONSTCD11
inline
year<S>
year<S>::min() NOEXCEPT
{
    using std::chrono::seconds;
    using std::chrono::minutes;
    using std::chrono::hours;
    using std::chrono::duration_cast;
    static_assert(sizeof(seconds)*CHAR_BIT >= 41, "seconds may overflow");
    static_assert(sizeof(hours)*CHAR_BIT >= 30, "hours may overflow");
    return sizeof(minutes)*CHAR_BIT < 34 ?
        year<S>{1970} + duration_cast<years>(minutes::min()) :
        year<S>{std::numeric_limits<short>::min()};
}

template <start S>
CONSTCD11
inline
year<S>
year<S>::max() NOEXCEPT
{
    using std::chrono::seconds;
    using std::chrono::minutes;
    using std::chrono::hours;
    using std::chrono::duration_cast;
    static_assert(sizeof(seconds)*CHAR_BIT >= 41, "seconds may overflow");
    static_assert(sizeof(hours)*CHAR_BIT >= 30, "hours may overflow");
    return sizeof(minutes)*CHAR_BIT < 34 ?
        year<S>{1969} + duration_cast<years>(minutes::max()) :
        year<S>{std::numeric_limits<short>::max()};
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

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
inline namespace literals
{

CONSTCD11
inline
week::weeknum
operator "" _w(unsigned long long wn) NOEXCEPT
{
    return week::weeknum(static_cast<unsigned>(wn));
}

#endif  // !defined(_MSC_VER) || (_MSC_VER >= 1900)

CONSTDATA week::last_week last{};

CONSTDATA week::weekday sun{0u};
CONSTDATA week::weekday mon{1u};
CONSTDATA week::weekday tue{2u};
CONSTDATA week::weekday wed{3u};
CONSTDATA week::weekday thu{4u};
CONSTDATA week::weekday fri{5u};
CONSTDATA week::weekday sat{6u};

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
}  // inline namespace literals
#endif

// weeknum

CONSTCD11
inline
weeknum::weeknum(unsigned wn) NOEXCEPT
    : wn_(static_cast<decltype(wn_)>(wn))
    {}

inline weeknum& weeknum::operator++() NOEXCEPT {++wn_; return *this;}
inline weeknum weeknum::operator++(int) NOEXCEPT {auto tmp(*this); ++(*this); return tmp;}
inline weeknum& weeknum::operator--() NOEXCEPT {--wn_; return *this;}
inline weeknum weeknum::operator--(int) NOEXCEPT {auto tmp(*this); --(*this); return tmp;}

inline
weeknum&
weeknum::operator+=(const weeks& y) NOEXCEPT
{
    *this = *this + y;
    return *this;
}

inline
weeknum&
weeknum::operator-=(const weeks& y) NOEXCEPT
{
    *this = *this - y;
    return *this;
}

CONSTCD11 inline weeknum::operator unsigned() const NOEXCEPT {return wn_;}
CONSTCD11 inline bool weeknum::ok() const NOEXCEPT {return 1 <= wn_ && wn_ <= 53;}

CONSTCD11
inline
bool
operator==(const weeknum& x, const weeknum& y) NOEXCEPT
{
    return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator!=(const weeknum& x, const weeknum& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const weeknum& x, const weeknum& y) NOEXCEPT
{
    return static_cast<unsigned>(x) < static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator>(const weeknum& x, const weeknum& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const weeknum& x, const weeknum& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const weeknum& x, const weeknum& y) NOEXCEPT
{
    return !(x < y);
}

CONSTCD11
inline
weeks
operator-(const weeknum& x, const weeknum& y) NOEXCEPT
{
    return weeks{static_cast<weeks::rep>(static_cast<unsigned>(x)) -
                 static_cast<weeks::rep>(static_cast<unsigned>(y))};
}

CONSTCD11
inline
weeknum
operator+(const weeknum& x, const weeks& y) NOEXCEPT
{
    return weeknum{static_cast<unsigned>(x) + static_cast<unsigned>(y.count())};
}

CONSTCD11
inline
weeknum
operator+(const weeks& x, const weeknum& y) NOEXCEPT
{
    return y + x;
}

CONSTCD11
inline
weeknum
operator-(const weeknum& x, const weeks& y) NOEXCEPT
{
    return x + -y;
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const weeknum& wn)
{
    date::detail::save_ostream<CharT, Traits> _(os);
    os << 'W';
    os.fill('0');
    os.flags(std::ios::dec | std::ios::right);
    os.width(2);
    os << static_cast<unsigned>(wn);
    return os;
}

// year_weeknum

template <start S>
CONSTCD11
inline
year_weeknum<S>::year_weeknum(const week::year<S>& y, const week::weeknum& wn) NOEXCEPT
    : y_(y)
    , wn_(wn)
    {}

template <start S>
CONSTCD11 inline year<S> year_weeknum<S>::year() const NOEXCEPT {return y_;}
template <start S>
CONSTCD11 inline weeknum year_weeknum<S>::weeknum() const NOEXCEPT {return wn_;}
template <start S>
CONSTCD11 inline bool year_weeknum<S>::ok() const NOEXCEPT
{
    return y_.ok() && 1u <= static_cast<unsigned>(wn_) && wn_ <= (y_/last).weeknum();
}

template <start S>
inline
year_weeknum<S>&
year_weeknum<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
inline
year_weeknum<S>&
year_weeknum<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT
{
    return x.year() == y.year() && x.weeknum() == y.weeknum();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.weeknum() < y.weeknum()));
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_weeknum<S>& x, const year_weeknum<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD11
inline
year_weeknum<S>
operator+(const year_weeknum<S>& ym, const years& dy) NOEXCEPT
{
    return (ym.year() + dy) / ym.weeknum();
}

template <start S>
CONSTCD11
inline
year_weeknum<S>
operator+(const years& dy, const year_weeknum<S>& ym) NOEXCEPT
{
    return ym + dy;
}

template <start S>
CONSTCD11
inline
year_weeknum<S>
operator-(const year_weeknum<S>& ym, const years& dy) NOEXCEPT
{
    return ym + -dy;
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_weeknum<S>& ywn)
{
    return os << ywn.year() << '-' << ywn.weeknum();
}


// year_lastweek

template <start S>
CONSTCD11
inline
year_lastweek<S>::year_lastweek(const week::year<S>& y) NOEXCEPT
    : y_(y)
    {}

template <start S>
CONSTCD11 inline year<S> year_lastweek<S>::year() const NOEXCEPT {return y_;}

template <start S>
CONSTCD14
inline
weeknum
year_lastweek<S>::weeknum() const NOEXCEPT
{
    return week::weeknum(y_.is_leap() ? 53u : 52u);
}

template <start S>
CONSTCD11 inline bool year_lastweek<S>::ok() const NOEXCEPT {return y_.ok();}

template <start S>
inline
year_lastweek<S>&
year_lastweek<S>::operator+=(const years& dy) NOEXCEPT
{
    *this = *this + dy;
    return *this;
}

template <start S>
inline
year_lastweek<S>&
year_lastweek<S>::operator-=(const years& dy) NOEXCEPT
{
    *this = *this - dy;
    return *this;
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT
{
    return x.year() == y.year();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT
{
    return x.year() < y.year();
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_lastweek<S>& x, const year_lastweek<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD11
inline
year_lastweek<S>
operator+(const year_lastweek<S>& ym, const years& dy) NOEXCEPT
{
    return year_lastweek<S>{ym.year() + dy};
}

template <start S>
CONSTCD11
inline
year_lastweek<S>
operator+(const years& dy, const year_lastweek<S>& ym) NOEXCEPT
{
    return ym + dy;
}

template <start S>
CONSTCD11
inline
year_lastweek<S>
operator-(const year_lastweek<S>& ym, const years& dy) NOEXCEPT
{
    return ym + -dy;
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_lastweek<S>& ywn)
{
    return os << ywn.year() << "-W last";
}

// weeknum_weekday

CONSTCD11
inline
weeknum_weekday::weeknum_weekday(const week::weeknum& wn,
                                 const week::weekday& wd) NOEXCEPT
    : wn_(wn)
    , wd_(wd)
    {}

CONSTCD11 inline weeknum weeknum_weekday::weeknum() const NOEXCEPT {return wn_;}
CONSTCD11 inline weekday weeknum_weekday::weekday() const NOEXCEPT {return wd_;}

CONSTCD14
inline
bool
weeknum_weekday::ok() const NOEXCEPT
{
    return wn_.ok() && wd_.ok();
}

CONSTCD11
inline
bool
operator==(const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT
{
    return x.weeknum() == y.weeknum() && x.weekday() == y.weekday();
}

CONSTCD11
inline
bool
operator!=(const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT
{
    return !(x == y);
}

// TODO We don't know the week start yet, so this shouldn't be defined?

// CONSTCD11
// inline
// bool
// operator<(const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT
// {
//     // TODO: We don't know the week start yet, so this shouldn't be defined?
//     return x.weeknum() < y.weeknum() ? true
//         : (x.weeknum() > y.weeknum() ? false
//         : (static_cast<unsigned>(x.weekday()) < static_cast<unsigned>(y.weekday())));
// }

// CONSTCD11
// inline
// bool
// operator>(const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT
// {
//     return y < x;
// }

// CONSTCD11
// inline
// bool
// operator<=(const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT
// {
//     return !(y < x);
// }

// CONSTCD11
// inline
// bool
// operator>=(const weeknum_weekday& x, const weeknum_weekday& y) NOEXCEPT
// {
//     return !(x < y);
// }

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const weeknum_weekday& md)
{
    return os << md.weeknum() << '-' << md.weekday();
}

// lastweek_weekday

CONSTCD11
inline
lastweek_weekday::lastweek_weekday(const week::weekday& wd) NOEXCEPT
    : wd_(wd)
    {}

CONSTCD11 inline weekday lastweek_weekday::weekday() const NOEXCEPT {return wd_;}

CONSTCD14
inline
bool
lastweek_weekday::ok() const NOEXCEPT
{
    return wd_.ok();
}

CONSTCD11
inline
bool
operator==(const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT
{
    return x.weekday() == y.weekday();
}

CONSTCD11
inline
bool
operator!=(const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT
{
    return !(x == y);
}

// CONSTCD11
// inline
// bool
// operator<(const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT
// {
//     return static_cast<unsigned>(x.weekday()) < static_cast<unsigned>(y.weekday());
// }

// CONSTCD11
// inline
// bool
// operator>(const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT
// {
//     return y < x;
// }

// CONSTCD11
// inline
// bool
// operator<=(const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT
// {
//     return !(y < x);
// }

// CONSTCD11
// inline
// bool
// operator>=(const lastweek_weekday& x, const lastweek_weekday& y) NOEXCEPT
// {
//     return !(x < y);
// }

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const lastweek_weekday& md)
{
    return os << "W last-" << md.weekday();
}

// year_lastweek_weekday

template <start S>
CONSTCD11
inline
year_lastweek_weekday<S>::year_lastweek_weekday(const week::year<S>& y,
                                                const week::weekday& wd) NOEXCEPT
    : y_(y)
    , wd_(wd)
    {}

template <start S>
inline
year_lastweek_weekday<S>&
year_lastweek_weekday<S>::operator+=(const years& y) NOEXCEPT
{
    *this = *this + y;
    return *this;
}

template <start S>
inline
year_lastweek_weekday<S>&
year_lastweek_weekday<S>::operator-=(const years& y) NOEXCEPT
{
    *this = *this - y;
    return *this;
}

template <start S>
CONSTCD11 inline year<S> year_lastweek_weekday<S>::year() const NOEXCEPT {return y_;}

template <start S>
CONSTCD14
inline
weeknum
year_lastweek_weekday<S>::weeknum() const NOEXCEPT
{
    return (y_ / last).weeknum();
}

template <start S>
CONSTCD11 inline weekday year_lastweek_weekday<S>::weekday() const NOEXCEPT {return wd_;}

template <start S>
CONSTCD14
inline
year_lastweek_weekday<S>::operator sys_days() const NOEXCEPT
{
    // TODO
    const auto start = week::weekday(static_cast<unsigned char>(S));
    const auto end = start - days{1};
    const auto middle = start + days{3};
    return sys_days(date::year{static_cast<int>(y_)}/date::dec/middle[date::last])
         + (end - middle) - (end - wd_);
}

template <start S>
CONSTCD14
inline
year_lastweek_weekday<S>::operator local_days() const NOEXCEPT
{
    // TODO
    const auto start = week::weekday(static_cast<unsigned char>(S));
    const auto end = start - days{1};
    const auto middle = start + days{3};
    return local_days(date::year{static_cast<int>(y_)}/date::dec/middle[date::last])
         + (end - middle) - (end - wd_);
}

template <start S>
CONSTCD11
inline
bool
year_lastweek_weekday<S>::ok() const NOEXCEPT
{
    return y_.ok() && wd_.ok();
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT
{
    return x.year() == y.year() && x.weekday() == y.weekday();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (static_cast<unsigned>(x.weekday()) < static_cast<unsigned>(y.weekday())));
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_lastweek_weekday<S>& x, const year_lastweek_weekday<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD11
inline
year_lastweek_weekday<S>
operator+(const year_lastweek_weekday<S>& ywnwd, const years& y)  NOEXCEPT
{
    return (ywnwd.year() + y) / last / ywnwd.weekday();
}

template <start S>
CONSTCD11
inline
year_lastweek_weekday<S>
operator+(const years& y, const year_lastweek_weekday<S>& ywnwd)  NOEXCEPT
{
    return ywnwd + y;
}

template <start S>
CONSTCD11
inline
year_lastweek_weekday<S>
operator-(const year_lastweek_weekday<S>& ywnwd, const years& y)  NOEXCEPT
{
    return ywnwd + -y;
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_lastweek_weekday<S>& ywnwd)
{
    return os << ywnwd.year() << "-W last-" << ywnwd.weekday();
}

// year_weeknum_weekday

template <start S>
CONSTCD11
inline
year_weeknum_weekday<S>::year_weeknum_weekday(const week::year<S>& y,
                                              const week::weeknum& wn,
                                              const week::weekday& wd) NOEXCEPT
    : y_(y)
    , wn_(wn)
    , wd_(wd)
    {}

template <start S>
CONSTCD14
inline
year_weeknum_weekday<S>::year_weeknum_weekday(const year_lastweek_weekday<S>& ylwwd) NOEXCEPT
    : y_(ylwwd.year())
    , wn_(ylwwd.weeknum())
    , wd_(ylwwd.weekday())
    {}

template <start S>
CONSTCD14
inline
year_weeknum_weekday<S>::year_weeknum_weekday(const sys_days& dp) NOEXCEPT
    : year_weeknum_weekday(from_days(dp.time_since_epoch()))
    {}

template <start S>
CONSTCD14
inline
year_weeknum_weekday<S>::year_weeknum_weekday(const local_days& dp) NOEXCEPT
    : year_weeknum_weekday(from_days(dp.time_since_epoch()))
    {}

template <start S>
inline
year_weeknum_weekday<S>&
year_weeknum_weekday<S>::operator+=(const years& y) NOEXCEPT
{
    *this = *this + y;
    return *this;
}

template <start S>
inline
year_weeknum_weekday<S>&
year_weeknum_weekday<S>::operator-=(const years& y) NOEXCEPT
{
    *this = *this - y;
    return *this;
}

template <start S>
CONSTCD11 inline year<S> year_weeknum_weekday<S>::year() const NOEXCEPT {return y_;}
template <start S>
CONSTCD11 inline weeknum year_weeknum_weekday<S>::weeknum() const NOEXCEPT {return wn_;}
template <start S>
CONSTCD11 inline weekday year_weeknum_weekday<S>::weekday() const NOEXCEPT {return wd_;}

template <start S>
CONSTCD14
inline
year_weeknum_weekday<S>::operator sys_days() const NOEXCEPT
{
    // TODO
    const auto start = week::weekday(static_cast<unsigned char>(S));
    const auto middle = start + days{3};
    return sys_days(date::year{static_cast<int>(y_)-1}/date::dec/middle[date::last])
         + (start - middle) + weeks{static_cast<unsigned>(wn_)-1} + (wd_ - start);
}

template <start S>
CONSTCD14
inline
year_weeknum_weekday<S>::operator local_days() const NOEXCEPT
{
    // TODO
    const auto start = week::weekday(static_cast<unsigned char>(S));
    const auto middle = start + days{3};
    return local_days(date::year{static_cast<int>(y_)-1}/date::dec/middle[date::last])
         + (start - middle) + weeks{static_cast<unsigned>(wn_)-1} + (wd_ - start);
}

template <start S>
CONSTCD14
inline
bool
year_weeknum_weekday<S>::ok() const NOEXCEPT
{
    return y_.ok() && wd_.ok() && week::weeknum{1u} <= wn_ && wn_ <= year_lastweek<S>{y_}.weeknum();
}

template <start S>
CONSTCD14
inline
year_weeknum_weekday<S>
year_weeknum_weekday<S>::from_days(days d) NOEXCEPT
{
    // TODO
    const auto dp = sys_days{d};
    const auto wd = week::weekday{dp};
    const auto start = week::weekday{static_cast<unsigned char>(S)};
    const auto middle = start + days{3};
    auto y = date::year_month_day{dp + days{3}}.year();
    auto dp_start = sys_days((y - date::years{1})/date::dec/middle[date::last]) + (start-middle);
    if (dp < dp_start)
    {
        --y;
        dp_start = sys_days((y - date::years{1})/date::dec/middle[date::last]) + (start-middle);
    }
    const auto wn = week::weeknum(
                       static_cast<unsigned>(date::trunc<weeks>(dp - dp_start).count() + 1));
    return {week::year<S>(static_cast<int>(y)), wn, wd};
}

template <start S>
CONSTCD11
inline
bool
operator==(const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT
{
    return x.year() == y.year() && x.weeknum() == y.weeknum() && x.weekday() == y.weekday();
}

template <start S>
CONSTCD11
inline
bool
operator!=(const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT
{
    return !(x == y);
}

template <start S>
CONSTCD11
inline
bool
operator<(const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.weeknum() < y.weeknum() ? true
        : (x.weeknum() > y.weeknum() ? false
        : (static_cast<unsigned>(x.weekday()) < static_cast<unsigned>(y.weekday())))));
}

template <start S>
CONSTCD11
inline
bool
operator>(const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT
{
    return y < x;
}

template <start S>
CONSTCD11
inline
bool
operator<=(const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT
{
    return !(y < x);
}

template <start S>
CONSTCD11
inline
bool
operator>=(const year_weeknum_weekday<S>& x, const year_weeknum_weekday<S>& y) NOEXCEPT
{
    return !(x < y);
}

template <start S>
CONSTCD11
inline
year_weeknum_weekday<S>
operator+(const year_weeknum_weekday<S>& ywnwd, const years& y)  NOEXCEPT
{
    return (ywnwd.year() + y) / ywnwd.weeknum() / ywnwd.weekday();
}

template <start S>
CONSTCD11
inline
year_weeknum_weekday<S>
operator+(const years& y, const year_weeknum_weekday<S>& ywnwd)  NOEXCEPT
{
    return ywnwd + y;
}

template <start S>
CONSTCD11
inline
year_weeknum_weekday<S>
operator-(const year_weeknum_weekday<S>& ywnwd, const years& y)  NOEXCEPT
{
    return ywnwd + -y;
}

template<class CharT, class Traits, start S>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_weeknum_weekday<S>& ywnwd)
{
    return os << ywnwd.year() << '-' << ywnwd.weeknum() << '-' << ywnwd.weekday();
}

// date composition operators

template <start S>
CONSTCD11
inline
year_weeknum<S>
operator/(const year<S>& y, const weeknum& wn) NOEXCEPT
{
    return {y, wn};
}

template <start S>
CONSTCD11
inline
year_weeknum<S>
operator/(const year<S>& y, int wn) NOEXCEPT
{
    return y/weeknum(static_cast<unsigned>(wn));
}

template <start S>
CONSTCD11
inline
year_lastweek<S>
operator/(const year<S>& y, last_week) NOEXCEPT
{
    return year_lastweek<S>{y};
}

CONSTCD11
inline
weeknum_weekday
operator/(const weeknum& wn, const weekday& wd) NOEXCEPT
{
    return {wn, wd};
}

CONSTCD11
inline
weeknum_weekday
operator/(const weeknum& wn, int wd) NOEXCEPT
{
    return wn/weekday{static_cast<unsigned>(wd)};
}

CONSTCD11
inline
weeknum_weekday
operator/(const weekday& wd, const weeknum& wn) NOEXCEPT
{
    return wn/wd;
}

CONSTCD11
inline
weeknum_weekday
operator/(const weekday& wd, int wn) NOEXCEPT
{
    return weeknum{static_cast<unsigned>(wn)}/wd;
}

CONSTCD11
inline
lastweek_weekday
operator/(const last_week&, const weekday& wd) NOEXCEPT
{
    return lastweek_weekday{wd};
}

CONSTCD11
inline
lastweek_weekday
operator/(const last_week& wn, int wd) NOEXCEPT
{
    return wn / weekday{static_cast<unsigned>(wd)};
}

CONSTCD11
inline
lastweek_weekday
operator/(const weekday& wd, const last_week& wn) NOEXCEPT
{
    return wn / wd;
}

template <start S>
CONSTCD11
inline
year_weeknum_weekday<S>
operator/(const year_weeknum<S>& ywn, const weekday& wd) NOEXCEPT
{
    return {ywn.year(), ywn.weeknum(), wd};
}

template <start S>
CONSTCD11
inline
year_weeknum_weekday<S>
operator/(const year_weeknum<S>& ywn, int wd) NOEXCEPT
{
    return ywn / weekday(static_cast<unsigned>(wd));
}

template <start S>
CONSTCD11
inline
year_weeknum_weekday<S>
operator/(const weeknum_weekday& wnwd, const year<S>& y) NOEXCEPT
{
    return {y, wnwd.weeknum(), wnwd.weekday()};
}

template <start S>
CONSTCD11
inline
year_weeknum_weekday<S>
operator/(const weeknum_weekday& wnwd, int y) NOEXCEPT
{
    return wnwd / year<S>{y};
}

template <start S>
CONSTCD11
inline
year_lastweek_weekday<S>
operator/(const year_lastweek<S>& ylw, const weekday& wd) NOEXCEPT
{
    return {ylw.year(), wd};
}

template <start S>
CONSTCD11
inline
year_lastweek_weekday<S>
operator/(const year_lastweek<S>& ylw, int wd) NOEXCEPT
{
    return ylw / weekday(static_cast<unsigned>(wd));
}

template <start S>
CONSTCD11
inline
year_lastweek_weekday<S>
operator/(const lastweek_weekday& lwwd, const year<S>& y) NOEXCEPT
{
    return {y, lwwd.weekday()};
}

template <start S>
CONSTCD11
inline
year_lastweek_weekday<S>
operator/(const lastweek_weekday& lwwd, int y) NOEXCEPT
{
    return lwwd / year<S>{y};
}

}  // namespace week

#endif  // WEEK_H
