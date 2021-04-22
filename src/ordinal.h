#ifndef ORDINAL_H
#define ORDINAL_H

// The MIT License (MIT)
//
// For the original `date.h` implementation:
// Copyright (c) 2015, 2016, 2017 Howard Hinnant
// For the `ordinal.h` extension:
// Copyright (c) 2021 Davis Vaughan
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
//
// Our apologies.  When the previous paragraph was written, lowercase had not yet
// been invented (that would involve another several millennia of evolution).
// We did not mean to shout.

#include <tzdb/date.h>

namespace ordinal
{

//-----------+
// Interface |
//-----------+

// durations

using days = date::days;
using years = date::years;

// time_point

using sys_days = date::sys_days;
using local_days = date::local_days;

// types

struct last_day
{
    explicit last_day() = default;
};

class yearday;
class year;

class year_yearday;
class year_yearday_last;

// date composition operators

CONSTCD11 year_yearday operator/(const year& y, const yearday& yd) NOEXCEPT;
CONSTCD11 year_yearday operator/(const year& y, int            yd) NOEXCEPT;

CONSTCD11 year_yearday_last operator/(const year& y, last_day yd) NOEXCEPT;

// yearday

class yearday
{
    unsigned short yd_;

public:
    yearday() = default;
    explicit CONSTCD11 yearday(unsigned d) NOEXCEPT;

    CONSTCD14 yearday& operator++()    NOEXCEPT;
    CONSTCD14 yearday  operator++(int) NOEXCEPT;
    CONSTCD14 yearday& operator--()    NOEXCEPT;
    CONSTCD14 yearday  operator--(int) NOEXCEPT;

    CONSTCD14 yearday& operator+=(const days& d) NOEXCEPT;
    CONSTCD14 yearday& operator-=(const days& d) NOEXCEPT;

    CONSTCD11 explicit operator unsigned() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const yearday& x, const yearday& y) NOEXCEPT;
CONSTCD11 bool operator!=(const yearday& x, const yearday& y) NOEXCEPT;
CONSTCD11 bool operator< (const yearday& x, const yearday& y) NOEXCEPT;
CONSTCD11 bool operator> (const yearday& x, const yearday& y) NOEXCEPT;
CONSTCD11 bool operator<=(const yearday& x, const yearday& y) NOEXCEPT;
CONSTCD11 bool operator>=(const yearday& x, const yearday& y) NOEXCEPT;

CONSTCD11 yearday operator+(const yearday&  x, const days&     y) NOEXCEPT;
CONSTCD11 yearday operator+(const days&     x, const yearday&  y) NOEXCEPT;
CONSTCD11 yearday operator-(const yearday&  x, const days&     y) NOEXCEPT;
CONSTCD11 days    operator-(const yearday&  x, const yearday&  y) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const yearday& yd);

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

    CONSTCD14 year& operator+=(const years& y) NOEXCEPT;
    CONSTCD14 year& operator-=(const years& y) NOEXCEPT;

    CONSTCD11 year operator-() const NOEXCEPT;
    CONSTCD11 year operator+() const NOEXCEPT;

    CONSTCD11 bool is_leap() const NOEXCEPT;

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

// class year_yearday

class year_yearday
{
    ordinal::year y_;
    ordinal::yearday yd_;

public:
    year_yearday() = default;
    CONSTCD11 year_yearday(const ordinal::year& y, const ordinal::yearday& yd) NOEXCEPT;
    CONSTCD14 year_yearday(const year_yearday_last& yydl) NOEXCEPT;

    CONSTCD14 year_yearday(sys_days dp) NOEXCEPT;
    CONSTCD14 explicit year_yearday(local_days dp) NOEXCEPT;

    CONSTCD14 year_yearday& operator+=(const years& y)  NOEXCEPT;
    CONSTCD14 year_yearday& operator-=(const years& y)  NOEXCEPT;

    CONSTCD11 ordinal::year    year()    const NOEXCEPT;
    CONSTCD11 ordinal::yearday yearday() const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD14 bool ok() const NOEXCEPT;

private:
    static CONSTCD14 year_yearday from_days(const days& dp) NOEXCEPT;
    CONSTCD14 days to_days() const NOEXCEPT;
};

CONSTCD11 bool operator==(const year_yearday& x, const year_yearday& y) NOEXCEPT;
CONSTCD11 bool operator!=(const year_yearday& x, const year_yearday& y) NOEXCEPT;
CONSTCD11 bool operator< (const year_yearday& x, const year_yearday& y) NOEXCEPT;
CONSTCD11 bool operator> (const year_yearday& x, const year_yearday& y) NOEXCEPT;
CONSTCD11 bool operator<=(const year_yearday& x, const year_yearday& y) NOEXCEPT;
CONSTCD11 bool operator>=(const year_yearday& x, const year_yearday& y) NOEXCEPT;

CONSTCD11 year_yearday operator+(const year_yearday& yyd, const years& dy)  NOEXCEPT;
CONSTCD11 year_yearday operator+(const years& dy, const year_yearday& yyd)  NOEXCEPT;
CONSTCD11 year_yearday operator-(const year_yearday& yyd, const years& dy)  NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_yearday& yyd);

// year_yearday_last

class year_yearday_last
{
    ordinal::year y_;

public:
    CONSTCD11 year_yearday_last(const ordinal::year& y) NOEXCEPT;

    CONSTCD14 year_yearday_last& operator+=(const years& y)  NOEXCEPT;
    CONSTCD14 year_yearday_last& operator-=(const years& y)  NOEXCEPT;

    CONSTCD11 ordinal::year    year()    const NOEXCEPT;
    CONSTCD14 ordinal::yearday yearday() const NOEXCEPT;

    CONSTCD14 operator sys_days() const NOEXCEPT;
    CONSTCD14 explicit operator local_days() const NOEXCEPT;
    CONSTCD11 bool ok() const NOEXCEPT;
};

CONSTCD11 bool operator==(const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT;
CONSTCD11 bool operator!=(const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT;
CONSTCD11 bool operator< (const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT;
CONSTCD11 bool operator> (const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT;
CONSTCD11 bool operator<=(const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT;
CONSTCD11 bool operator>=(const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT;

CONSTCD11 year_yearday_last operator+(const year_yearday_last& yydl, const years& dy) NOEXCEPT;
CONSTCD11 year_yearday_last operator+(const years& dy, const year_yearday_last& yydl) NOEXCEPT;
CONSTCD11 year_yearday_last operator-(const year_yearday_last& yydl, const years& dy) NOEXCEPT;

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_yearday_last& yydl);

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
inline namespace literals
{

CONSTCD11 ordinal::yearday operator "" _yd(unsigned long long yd) NOEXCEPT;
CONSTCD11 ordinal::year    operator "" _y(unsigned long long y) NOEXCEPT;

}  // inline namespace literals
#endif // !defined(_MSC_VER) || (_MSC_VER >= 1900)

//----------------+
// Implementation |
//----------------+

// yearday

CONSTCD11
inline
yearday::yearday(unsigned yd) NOEXCEPT
    : yd_(static_cast<decltype(yd_)>(yd))
    {}

CONSTCD14
inline
yearday&
yearday::operator++() NOEXCEPT
{
    ++yd_;
    return *this;
}

CONSTCD14
inline
yearday
yearday::operator++(int) NOEXCEPT
{
    auto tmp(*this);
    ++(*this);
    return tmp;
}

CONSTCD14
inline
yearday&
yearday::operator--() NOEXCEPT
{
    --yd_;
    return *this;
}

CONSTCD14
inline
yearday
yearday::operator--(int) NOEXCEPT
{
    auto tmp(*this);
    --(*this);
    return tmp;
}

CONSTCD14
inline
yearday&
yearday::operator+=(const days& d) NOEXCEPT
{
    *this = *this + d;
    return *this;
}

CONSTCD14
inline
yearday&
yearday::operator-=(const days& d) NOEXCEPT
{
    *this = *this - d;
    return *this;
}

CONSTCD11
inline
yearday::operator unsigned() const NOEXCEPT
{
    return yd_;
}

CONSTCD11
inline
bool
yearday::ok() const NOEXCEPT
{
    return 1 <= yd_ && yd_ <= 366;
}

CONSTCD11
inline
bool
operator==(const yearday& x, const yearday& y) NOEXCEPT
{
    return static_cast<unsigned>(x) == static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator!=(const yearday& x, const yearday& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const yearday& x, const yearday& y) NOEXCEPT
{
    return static_cast<unsigned>(x) < static_cast<unsigned>(y);
}

CONSTCD11
inline
bool
operator>(const yearday& x, const yearday& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const yearday& x, const yearday& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const yearday& x, const yearday& y) NOEXCEPT
{
    return !(x < y);
}

CONSTCD11
inline
days
operator-(const yearday& x, const yearday& y) NOEXCEPT
{
    return days{static_cast<days::rep>(static_cast<unsigned>(x)
                                     - static_cast<unsigned>(y))};
}

CONSTCD11
inline
yearday
operator+(const yearday& x, const days& y) NOEXCEPT
{
    return yearday{static_cast<unsigned>(x) + static_cast<unsigned>(y.count())};
}

CONSTCD11
inline
yearday
operator+(const days& x, const yearday& y) NOEXCEPT
{
    return y + x;
}

CONSTCD11
inline
yearday
operator-(const yearday& x, const days& y) NOEXCEPT
{
    return x + -y;
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const yearday& yd)
{
    date::detail::save_ostream<CharT, Traits> _(os);
    os.fill('0');
    os.flags(std::ios::dec | std::ios::right);
    os.width(3);
    os << static_cast<unsigned>(yd);
    if (!yd.ok())
        os << " is not a valid day of the year";
    return os;
}

// year

CONSTCD11
inline
year::year(int y) NOEXCEPT
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
year::operator+=(const years& y) NOEXCEPT
{
    *this = *this + y;
    return *this;
}

CONSTCD14
inline
year&
year::operator-=(const years& y) NOEXCEPT
{
    *this = *this - y;
    return *this;
}

CONSTCD11
inline
year
year::operator-() const NOEXCEPT
{
    return year{-y_};
}

CONSTCD11
inline
year
year::operator+() const NOEXCEPT
{
    return *this;
}

CONSTCD11
inline
bool
year::is_leap() const NOEXCEPT
{
    return date::year{y_}.is_leap();
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
    os.imbue(std::locale::classic());
    os << static_cast<int>(y);
    if (!y.ok())
        os << " is not a valid year";
    return os;
}

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
inline namespace literals
{

CONSTCD11
inline
ordinal::yearday
operator "" _yd(unsigned long long yd) NOEXCEPT
{
    return ordinal::yearday{static_cast<unsigned>(yd)};
}

CONSTCD11
inline
ordinal::year
operator "" _y(unsigned long long y) NOEXCEPT
{
    return ordinal::year(static_cast<int>(y));
}
#endif  // !defined(_MSC_VER) || (_MSC_VER >= 1900)

CONSTDATA ordinal::last_day last{};

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
}  // inline namespace literals
#endif

// year_yearday_last

CONSTCD11
inline
year_yearday_last::year_yearday_last(const ordinal::year& y) NOEXCEPT
    : y_(y)
    {}

CONSTCD14
inline
year_yearday_last&
year_yearday_last::operator+=(const years& y) NOEXCEPT
{
    *this = *this + y;
    return *this;
}

CONSTCD14
inline
year_yearday_last&
year_yearday_last::operator-=(const years& y) NOEXCEPT
{
    *this = *this - y;
    return *this;
}

CONSTCD11
inline
year
year_yearday_last::year() const NOEXCEPT
{
    return y_;
}

CONSTCD14
inline
yearday
year_yearday_last::yearday() const NOEXCEPT
{
    return y_.is_leap() ? ordinal::yearday{366u} : ordinal::yearday{365u};
}

CONSTCD14
inline
year_yearday_last::operator sys_days() const NOEXCEPT
{
    return sys_days(year()/yearday());
}

CONSTCD14
inline
year_yearday_last::operator local_days() const NOEXCEPT
{
    return local_days(year()/yearday());
}

CONSTCD11
inline
bool
year_yearday_last::ok() const NOEXCEPT
{
    return y_.ok();
}

CONSTCD11
inline
bool
operator==(const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT
{
    return x.year() == y.year();
}

CONSTCD11
inline
bool
operator!=(const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT
{
    return x.year() < y.year();
}

CONSTCD11
inline
bool
operator>(const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const year_yearday_last& x, const year_yearday_last& y) NOEXCEPT
{
    return !(x < y);
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_yearday_last& yydl)
{
    return os << yydl.year() << '-' << "last";
}

CONSTCD11
inline
year_yearday_last
operator+(const year_yearday_last& yydl, const years& dy) NOEXCEPT
{
    return year_yearday_last{yydl.year() + dy};
}

CONSTCD11
inline
year_yearday_last
operator+(const years& dy, const year_yearday_last& yydl) NOEXCEPT
{
    return yydl + dy;
}

CONSTCD11
inline
year_yearday_last
operator-(const year_yearday_last& yydl, const years& dy) NOEXCEPT
{
    return yydl + (-dy);
}

// year_yearday

CONSTCD11
inline
year_yearday::year_yearday(const ordinal::year& y, const ordinal::yearday& yd) NOEXCEPT
    : y_(y)
    , yd_(yd)
    {}

CONSTCD14
inline
year_yearday::year_yearday(const year_yearday_last& yydl) NOEXCEPT
    : y_(yydl.year())
    , yd_(yydl.yearday())
    {}

CONSTCD14
inline
year_yearday::year_yearday(sys_days dp) NOEXCEPT
    : year_yearday(from_days(dp.time_since_epoch()))
    {}

CONSTCD14
inline
year_yearday::year_yearday(local_days dp) NOEXCEPT
    : year_yearday(from_days(dp.time_since_epoch()))
    {}

CONSTCD11
inline
year
year_yearday::year() const NOEXCEPT
{
    return y_;
}

CONSTCD11
inline
yearday
year_yearday::yearday() const NOEXCEPT
{
    return yd_;
}

CONSTCD14
inline
year_yearday&
year_yearday::operator+=(const years& y) NOEXCEPT
{
    *this = *this + y;
    return *this;
}

CONSTCD14
inline
year_yearday&
year_yearday::operator-=(const years& y) NOEXCEPT
{
    *this = *this - y;
    return *this;
}

CONSTCD14
inline
days
year_yearday::to_days() const NOEXCEPT
{
    const int y = static_cast<int>(y_);
    const unsigned yd = static_cast<unsigned>(yd_);
    return sys_days{date::year{y} / 1 / 1}.time_since_epoch() + days{yd - 1};
}

CONSTCD14
inline
year_yearday::operator sys_days() const NOEXCEPT
{
    return sys_days{to_days()};
}

CONSTCD14
inline
year_yearday::operator local_days() const NOEXCEPT
{
    return local_days{to_days()};
}

CONSTCD14
inline
bool
year_yearday::ok() const NOEXCEPT
{
    if (!y_.ok()) {
        return false;
    }
    return ordinal::yearday{1} <= yd_ && yd_ <= (y_ / last).yearday();
}

CONSTCD11
inline
bool
operator==(const year_yearday& x, const year_yearday& y) NOEXCEPT
{
    return x.year() == y.year() && x.yearday() == y.yearday();
}

CONSTCD11
inline
bool
operator!=(const year_yearday& x, const year_yearday& y) NOEXCEPT
{
    return !(x == y);
}

CONSTCD11
inline
bool
operator<(const year_yearday& x, const year_yearday& y) NOEXCEPT
{
    return x.year() < y.year() ? true
        : (x.year() > y.year() ? false
        : (x.yearday() < y.yearday()));
}

CONSTCD11
inline
bool
operator>(const year_yearday& x, const year_yearday& y) NOEXCEPT
{
    return y < x;
}

CONSTCD11
inline
bool
operator<=(const year_yearday& x, const year_yearday& y) NOEXCEPT
{
    return !(y < x);
}

CONSTCD11
inline
bool
operator>=(const year_yearday& x, const year_yearday& y) NOEXCEPT
{
    return !(x < y);
}

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const year_yearday& yyd)
{
    date::detail::save_ostream<CharT, Traits> _(os);
    os.fill('0');
    os.flags(std::ios::dec | std::ios::right);
    os.imbue(std::locale::classic());
    os << yyd.year() << '-';
    os << yyd.yearday();
    if (!yyd.ok())
        os << " is not a valid date";
    return os;
}

CONSTCD14
inline
year_yearday
year_yearday::from_days(const days& dp) NOEXCEPT
{
    CONSTDATA unsigned days_before_month[] = {
        0u, 31u, 59u, 90u, 120u, 151u, 181u, 212u, 243u, 273u, 304u, 334u
    };
    const date::year_month_day ymd{sys_days{dp}};
    const ordinal::year y{static_cast<int>(ymd.year())};
    const unsigned m = static_cast<unsigned>(ymd.month());
    const unsigned d = static_cast<unsigned>(ymd.day());
    const ordinal::yearday yd{days_before_month[m - 1] + (m > 2 && y.is_leap()) + d};
    return {y, yd};
}

CONSTCD11
inline
year_yearday
operator+(const year_yearday& yyd, const years& dy) NOEXCEPT
{
    return (yyd.year() + dy) / yyd.yearday();
}

CONSTCD11
inline
year_yearday
operator+(const years& dy, const year_yearday& yyd) NOEXCEPT
{
    return yyd + dy;
}

CONSTCD11
inline
year_yearday
operator-(const year_yearday& yyd, const years& dy) NOEXCEPT
{
    return yyd + (-dy);
}

// year_yearday from operator/()

CONSTCD11
year_yearday
operator/(const year& y, const yearday& yd) NOEXCEPT
{
    return {y, yd};
}

CONSTCD11
year_yearday
operator/(const year& y, int yd) NOEXCEPT
{
    return y / yearday(static_cast<unsigned>(yd));
}

// year_yearday_last from operator/()

CONSTCD11
inline
year_yearday_last
operator/(const year& y, last_day) NOEXCEPT
{
    return year_yearday_last{y};
}

}  // namespace ordinal

#endif  // ORDINAL_H
