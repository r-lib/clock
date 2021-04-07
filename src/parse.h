#ifndef CLOCK_PARSE_H
#define CLOCK_PARSE_H

#include "clock.h"

// -----------------------------------------------------------------------------

namespace rclock {

/*
 * Custom version of `read_long_double()` that allows for supplying the
 * `decimal_mark` directly.
 */
template <class CharT, class Traits>
long double
read_seconds(std::basic_istream<CharT, Traits>& is,
             const CharT& decimal_mark,
             unsigned m = 1,
             unsigned M = 10)
{
  unsigned count = 0;
  unsigned fcount = 0;
  unsigned long long i = 0;
  unsigned long long f = 0;
  bool parsing_fraction = false;
  auto decimal_point = Traits::to_int_type(decimal_mark);
  while (true)
  {
    auto ic = is.peek();
    if (Traits::eq_int_type(ic, Traits::eof()))
      break;
    if (Traits::eq_int_type(ic, decimal_point))
    {
      decimal_point = Traits::eof();
      parsing_fraction = true;
    }
    else
    {
      auto c = static_cast<char>(Traits::to_char_type(ic));
      if (!('0' <= c && c <= '9'))
        break;
      if (!parsing_fraction)
      {
        i = 10*i + static_cast<unsigned>(c - '0');
      }
      else
      {
        f = 10*f + static_cast<unsigned>(c - '0');
        ++fcount;
      }
    }
    (void)is.get();
    if (++count == M)
      break;
  }
  if (count < m)
  {
    is.setstate(std::ios::failbit);
    return 0;
  }
  return i + f/std::pow(10.L, fcount);
}

// Takes the `read()` variant for rld, removes the Args..., and
// uses `read_seconds()`
template <class CharT, class Traits>
void
read(std::basic_istream<CharT, Traits>& is,
     const CharT& decimal_mark,
     date::detail::rld a0)
{
    auto x = read_seconds(is, decimal_mark, a0.m, a0.M);
    if (is.fail())
        return;
    a0.i = x;
}

} // namespace rclock

// -----------------------------------------------------------------------------

namespace rclock {

template <class CharT, class Traits, class Duration, class Alloc = std::allocator<CharT>>
std::basic_istream<CharT, Traits>&
from_stream(std::basic_istream<CharT, Traits>& is,
            const CharT* fmt,
            const std::pair<const std::string*, const std::string*>& month_names_pair,
            const std::pair<const std::string*, const std::string*>& weekday_names_pair,
            const std::pair<const std::string*, const std::string*>& ampm_names_pair,
            const CharT& decimal_mark,
            date::fields<Duration>& fds,
            std::basic_string<CharT, Traits, Alloc>* abbrev,
            std::chrono::minutes* offset)
{
    using namespace date;
    using std::numeric_limits;
    using std::ios;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::chrono::minutes;
    using std::chrono::hours;
    using date::detail::round_i;
    typename std::basic_istream<CharT, Traits>::sentry ok{is, true};
    if (ok)
    {
        date::detail::save_istream<CharT, Traits> ss(is);
        is.fill(' ');
        is.flags(std::ios::skipws | std::ios::dec);
        is.width(0);
        const CharT* command = nullptr;
        auto modified = CharT{};
        auto width = -1;

        CONSTDATA int not_a_year = numeric_limits<short>::min();
        CONSTDATA int not_a_2digit_year = 100;
        CONSTDATA int not_a_century = not_a_year / 100;
        CONSTDATA int not_a_month = 0;
        CONSTDATA int not_a_day = 0;
        CONSTDATA int not_a_hour = numeric_limits<int>::min();
        CONSTDATA int not_a_hour_12_value = 0;
        CONSTDATA int not_a_minute = not_a_hour;
        CONSTDATA Duration not_a_second = Duration::min();
        CONSTDATA int not_a_doy = -1;
        CONSTDATA int not_a_weekday = 8;
        CONSTDATA int not_a_week_num = 100;
        CONSTDATA int not_a_ampm = -1;
        CONSTDATA minutes not_a_offset = minutes::min();

        int Y = not_a_year;             // c, F, Y                   *
        int y = not_a_2digit_year;      // D, x, y                   *
        int g = not_a_2digit_year;      // g                         *
        int G = not_a_year;             // G                         *
        int C = not_a_century;          // C                         *
        int m = not_a_month;            // b, B, h, m, c, D, F, x    *
        int d = not_a_day;              // c, d, D, e, F, x          *
        int j = not_a_doy;              // j                         *
        int wd = not_a_weekday;         // a, A, u, w                *
        int H = not_a_hour;             // c, H, R, T, X             *
        int I = not_a_hour_12_value;    // I, r                      *
        int p = not_a_ampm;             // p, r                      *
        int M = not_a_minute;           // c, M, r, R, T, X          *
        Duration s = not_a_second;      // c, r, S, T, X             *
        int U = not_a_week_num;         // U                         *
        int V = not_a_week_num;         // V                         *
        int W = not_a_week_num;         // W                         *
        std::basic_string<CharT, Traits, Alloc> temp_abbrev;  // Z   *
        minutes temp_offset = not_a_offset;  // z                    *

        using date::detail::read;
        using date::detail::rs;
        using date::detail::ru;
        using date::detail::rld;
        using date::detail::checked_set;
        for (; *fmt != CharT{} && !is.fail(); ++fmt)
        {
            switch (*fmt)
            {
            case 'a':
            case 'A':
            case 'u':
            case 'w':  // wd:  a, A, u, w
                if (command)
                {
                    int trial_wd = not_a_weekday;
                    if (*fmt == 'a' || *fmt == 'A')
                    {
                        if (modified == CharT{})
                        {
                            auto nm = weekday_names_pair;
                            auto i = date::detail::scan_keyword(is, nm.first, nm.second) - nm.first;
                            if (!is.fail())
                                trial_wd = i % 7;
                        }
                        else
                            read(is, CharT{'%'}, width, modified, *fmt);
                    }
                    else  // *fmt == 'u' || *fmt == 'w'
                    {
                        if (modified != CharT{'E'})
                        {
                            read(is, ru{trial_wd, 1, width == -1 ?
                                                      1u : static_cast<unsigned>(width)});
                            if (!is.fail())
                            {
                                if (*fmt == 'u')
                                {
                                    if (!(1 <= trial_wd && trial_wd <= 7))
                                    {
                                        trial_wd = not_a_weekday;
                                        is.setstate(ios::failbit);
                                    }
                                    else if (trial_wd == 7)
                                        trial_wd = 0;
                                }
                                else  // *fmt == 'w'
                                {
                                    if (!(0 <= trial_wd && trial_wd <= 6))
                                    {
                                        trial_wd = not_a_weekday;
                                        is.setstate(ios::failbit);
                                    }
                                }
                            }
                        }
                        else
                            read(is, CharT{'%'}, width, modified, *fmt);
                    }
                    if (trial_wd != not_a_weekday)
                        checked_set(wd, trial_wd, not_a_weekday, is);
                }
                else  // !command
                    read(is, *fmt);
                command = nullptr;
                width = -1;
                modified = CharT{};
                break;
            case 'b':
            case 'B':
            case 'h':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int ttm = not_a_month;
                        auto nm = month_names_pair;
                        auto i = date::detail::scan_keyword(is, nm.first, nm.second) - nm.first;
                        if (!is.fail())
                            ttm = i % 12 + 1;
                        checked_set(m, ttm, not_a_month, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'c':
                if (command)
                {
                    if (modified != CharT{'O'})
                    {
                        // "%a %b %e %T %Y"
                        auto nm = weekday_names_pair;
                        auto i = date::detail::scan_keyword(is, nm.first, nm.second) - nm.first;
                        checked_set(wd, static_cast<int>(i % 7), not_a_weekday, is);
                        ws(is);
                        nm = month_names_pair;
                        i = date::detail::scan_keyword(is, nm.first, nm.second) - nm.first;
                        checked_set(m, static_cast<int>(i % 12 + 1), not_a_month, is);
                        ws(is);
                        int td = not_a_day;
                        read(is, rs{td, 1, 2});
                        checked_set(d, td, not_a_day, is);
                        ws(is);
                        using dfs = date::detail::decimal_format_seconds<Duration>;
                        CONSTDATA auto w = Duration::period::den == 1 ? 2 : 3 + dfs::width;
                        int tH;
                        int tM;
                        long double S;
                        read(is, ru{tH, 1, 2}, CharT{':'}, ru{tM, 1, 2}, CharT{':'});
                        rclock::read(is, decimal_mark, rld{S, 1, w});
                        checked_set(H, tH, not_a_hour, is);
                        checked_set(M, tM, not_a_minute, is);
                        checked_set(s, round_i<Duration>(duration<long double>{S}),
                                    not_a_second, is);
                        ws(is);
                        int tY = not_a_year;
                        read(is, rs{tY, 1, 4u});
                        checked_set(Y, tY, not_a_year, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'x':
                if (command)
                {
                    if (modified != CharT{'O'})
                    {
                        // "%m/%d/%y"
                        int ty = not_a_2digit_year;
                        int tm = not_a_month;
                        int td = not_a_day;
                        read(is, ru{tm, 1, 2}, CharT{'/'}, ru{td, 1, 2}, CharT{'/'},
                                 rs{ty, 1, 2});
                        checked_set(y, ty, not_a_2digit_year, is);
                        checked_set(m, tm, not_a_month, is);
                        checked_set(d, td, not_a_day, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'X':
                if (command)
                {
                    if (modified != CharT{'O'})
                    {
                        // "%T"
                        using dfs = date::detail::decimal_format_seconds<Duration>;
                        CONSTDATA auto w = Duration::period::den == 1 ? 2 : 3 + dfs::width;
                        int tH = not_a_hour;
                        int tM = not_a_minute;
                        long double S;
                        read(is, ru{tH, 1, 2}, CharT{':'}, ru{tM, 1, 2}, CharT{':'});
                        rclock::read(is, decimal_mark, rld{S, 1, w});
                        checked_set(H, tH, not_a_hour, is);
                        checked_set(M, tM, not_a_minute, is);
                        checked_set(s, round_i<Duration>(duration<long double>{S}),
                                    not_a_second, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'C':
                if (command)
                {
                    int tC = not_a_century;
                    read(is, rs{tC, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                    checked_set(C, tC, not_a_century, is);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'D':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tn = not_a_month;
                        int td = not_a_day;
                        int ty = not_a_2digit_year;
                        read(is, ru{tn, 1, 2}, CharT{'\0'}, CharT{'/'}, CharT{'\0'},
                                 ru{td, 1, 2}, CharT{'\0'}, CharT{'/'}, CharT{'\0'},
                                 rs{ty, 1, 2});
                        checked_set(y, ty, not_a_2digit_year, is);
                        checked_set(m, tn, not_a_month, is);
                        checked_set(d, td, not_a_day, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'F':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tY = not_a_year;
                        int tn = not_a_month;
                        int td = not_a_day;
                        read(is, rs{tY, 1, width == -1 ? 4u : static_cast<unsigned>(width)},
                                 CharT{'-'}, ru{tn, 1, 2}, CharT{'-'}, ru{td, 1, 2});
                        checked_set(Y, tY, not_a_year, is);
                        checked_set(m, tn, not_a_month, is);
                        checked_set(d, td, not_a_day, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'd':
            case 'e':
                if (command)
                {
                    if (modified != CharT{'E'})
                    {
                        int td = not_a_day;
                        read(is, rs{td, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                        checked_set(d, td, not_a_day, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'H':
                if (command)
                {
                    if (modified != CharT{'E'})
                    {
                        int tH = not_a_hour;
                        read(is, ru{tH, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                        checked_set(H, tH, not_a_hour, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'I':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tI = not_a_hour_12_value;
                        // reads in an hour into I, but most be in [1, 12]
                        read(is, rs{tI, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                        if (!(1 <= tI && tI <= 12))
                            is.setstate(ios::failbit);
                        checked_set(I, tI, not_a_hour_12_value, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
               break;
            case 'j':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tj = not_a_doy;
                        read(is, ru{tj, 1, width == -1 ? 3u : static_cast<unsigned>(width)});
                        checked_set(j, tj, not_a_doy, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'M':
                if (command)
                {
                    if (modified != CharT{'E'})
                    {
                        int tM = not_a_minute;
                        read(is, ru{tM, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                        checked_set(M, tM, not_a_minute, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'm':
                if (command)
                {
                    if (modified != CharT{'E'})
                    {
                        int tn = not_a_month;
                        read(is, rs{tn, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                        checked_set(m, tn, not_a_month, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'n':
            case 't':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        // %n matches a single white space character
                        // %t matches 0 or 1 white space characters
                        auto ic = is.peek();
                        if (Traits::eq_int_type(ic, Traits::eof()))
                        {
                            ios::iostate err = ios::eofbit;
                            if (*fmt == 'n')
                                err |= ios::failbit;
                            is.setstate(err);
                            break;
                        }
                        if (isspace(ic))
                        {
                            (void)is.get();
                        }
                        else if (*fmt == 'n')
                            is.setstate(ios::failbit);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'p':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tp = not_a_ampm;
                        auto nm = ampm_names_pair;
                        auto i = date::detail::scan_keyword(is, nm.first, nm.second) - nm.first;
                        tp = static_cast<decltype(tp)>(i);
                        checked_set(p, tp, not_a_ampm, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);

               break;
            case 'r':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        // "%I:%M:%S %p"
                        using dfs = date::detail::decimal_format_seconds<Duration>;
                        CONSTDATA auto w = Duration::period::den == 1 ? 2 : 3 + dfs::width;
                        long double S;
                        int tI = not_a_hour_12_value;
                        int tM = not_a_minute;
                        read(is, ru{tI, 1, 2}, CharT{':'}, ru{tM, 1, 2}, CharT{':'});
                        rclock::read(is, decimal_mark, rld{S, 1, w});
                        checked_set(I, tI, not_a_hour_12_value, is);
                        checked_set(M, tM, not_a_minute, is);
                        checked_set(s, round_i<Duration>(duration<long double>{S}),
                                    not_a_second, is);
                        ws(is);
                        auto nm = ampm_names_pair;
                        auto i = date::detail::scan_keyword(is, nm.first, nm.second) - nm.first;
                        checked_set(p, static_cast<int>(i), not_a_ampm, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'R':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tH = not_a_hour;
                        int tM = not_a_minute;
                        read(is, ru{tH, 1, 2}, CharT{'\0'}, CharT{':'}, CharT{'\0'},
                                 ru{tM, 1, 2}, CharT{'\0'});
                        checked_set(H, tH, not_a_hour, is);
                        checked_set(M, tM, not_a_minute, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'S':
                if (command)
                {
                   if (modified != CharT{'E'})
                    {
                        using dfs = date::detail::decimal_format_seconds<Duration>;
                        CONSTDATA auto w = Duration::period::den == 1 ? 2 : 3 + dfs::width;
                        long double S;
                        rclock::read(is, decimal_mark, rld{S, 1, width == -1 ? w : static_cast<unsigned>(width)});
                        checked_set(s, round_i<Duration>(duration<long double>{S}),
                                    not_a_second, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'T':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        using dfs = date::detail::decimal_format_seconds<Duration>;
                        CONSTDATA auto w = Duration::period::den == 1 ? 2 : 3 + dfs::width;
                        int tH = not_a_hour;
                        int tM = not_a_minute;
                        long double S;
                        read(is, ru{tH, 1, 2}, CharT{':'}, ru{tM, 1, 2}, CharT{':'});
                        rclock::read(is, decimal_mark, rld{S, 1, w});
                        checked_set(H, tH, not_a_hour, is);
                        checked_set(M, tM, not_a_minute, is);
                        checked_set(s, round_i<Duration>(duration<long double>{S}),
                                    not_a_second, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'Y':
                if (command)
                {
                    if (modified != CharT{'O'})
                    {
                        int tY = not_a_year;
                        read(is, rs{tY, 1, width == -1 ? 4u : static_cast<unsigned>(width)});
                        checked_set(Y, tY, not_a_year, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'y':
                if (command)
                {
                    int ty = not_a_2digit_year;
                    read(is, ru{ty, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                    checked_set(y, ty, not_a_2digit_year, is);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'g':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tg = not_a_2digit_year;
                        read(is, ru{tg, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                        checked_set(g, tg, not_a_2digit_year, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'G':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tG = not_a_year;
                        read(is, rs{tG, 1, width == -1 ? 4u : static_cast<unsigned>(width)});
                        checked_set(G, tG, not_a_year, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'U':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tU = not_a_week_num;
                        read(is, ru{tU, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                        checked_set(U, tU, not_a_week_num, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'V':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tV = not_a_week_num;
                        read(is, ru{tV, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                        checked_set(V, tV, not_a_week_num, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'W':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        int tW = not_a_week_num;
                        read(is, ru{tW, 1, width == -1 ? 2u : static_cast<unsigned>(width)});
                        checked_set(W, tW, not_a_week_num, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'E':
            case 'O':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        modified = *fmt;
                    }
                    else
                    {
                        read(is, CharT{'%'}, width, modified, *fmt);
                        command = nullptr;
                        width = -1;
                        modified = CharT{};
                    }
                }
                else
                    read(is, *fmt);
                break;
            case '%':
                if (command)
                {
                    if (modified == CharT{})
                        read(is, *fmt);
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    command = fmt;
                break;
            case 'z':
                if (command)
                {
                    int tH, tM;
                    minutes toff = not_a_offset;
                    bool neg = false;
                    auto ic = is.peek();
                    if (!Traits::eq_int_type(ic, Traits::eof()))
                    {
                        auto c = static_cast<char>(Traits::to_char_type(ic));
                        if (c == '-')
                            neg = true;
                    }
                    if (modified == CharT{})
                    {
                        read(is, rs{tH, 2, 2});
                        if (!is.fail())
                            toff = hours{std::abs(tH)};
                        if (is.good())
                        {
                            ic = is.peek();
                            if (!Traits::eq_int_type(ic, Traits::eof()))
                            {
                                auto c = static_cast<char>(Traits::to_char_type(ic));
                                if ('0' <= c && c <= '9')
                                {
                                    read(is, ru{tM, 2, 2});
                                    if (!is.fail())
                                        toff += minutes{tM};
                                }
                            }
                        }
                    }
                    else
                    {
                        read(is, rs{tH, 1, 2});
                        if (!is.fail())
                            toff = hours{std::abs(tH)};
                        if (is.good())
                        {
                            ic = is.peek();
                            if (!Traits::eq_int_type(ic, Traits::eof()))
                            {
                                auto c = static_cast<char>(Traits::to_char_type(ic));
                                if (c == ':')
                                {
                                    (void)is.get();
                                    read(is, ru{tM, 2, 2});
                                    if (!is.fail())
                                        toff += minutes{tM};
                                }
                            }
                        }
                    }
                    if (neg)
                        toff = -toff;
                    checked_set(temp_offset, toff, not_a_offset, is);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            case 'Z':
                if (command)
                {
                    if (modified == CharT{})
                    {
                        std::basic_string<CharT, Traits, Alloc> buf;
                        while (is.rdstate() == std::ios::goodbit)
                        {
                            auto i = is.rdbuf()->sgetc();
                            if (Traits::eq_int_type(i, Traits::eof()))
                            {
                                is.setstate(ios::eofbit);
                                break;
                            }
                            auto wc = Traits::to_char_type(i);
                            auto c = static_cast<char>(wc);
                            // is c a valid time zone name or abbreviation character?
                            if (!(CharT{1} < wc && wc < CharT{127}) || !(isalnum(c) ||
                                    c == '_' || c == '/' || c == '-' || c == '+'))
                                break;
                            buf.push_back(c);
                            is.rdbuf()->sbumpc();
                        }
                        if (buf.empty())
                            is.setstate(ios::failbit);
                        checked_set(temp_abbrev, buf, {}, is);
                    }
                    else
                        read(is, CharT{'%'}, width, modified, *fmt);
                    command = nullptr;
                    width = -1;
                    modified = CharT{};
                }
                else
                    read(is, *fmt);
                break;
            default:
                if (command)
                {
                    if (width == -1 && modified == CharT{} && '0' <= *fmt && *fmt <= '9')
                    {
                        width = static_cast<char>(*fmt) - '0';
                        while ('0' <= fmt[1] && fmt[1] <= '9')
                            width = 10*width + static_cast<char>(*++fmt) - '0';
                    }
                    else
                    {
                        if (modified == CharT{})
                            read(is, CharT{'%'}, width, *fmt);
                        else
                            read(is, CharT{'%'}, width, modified, *fmt);
                        command = nullptr;
                        width = -1;
                        modified = CharT{};
                    }
                }
                else  // !command
                {
                    if (isspace(static_cast<unsigned char>(*fmt)))
                    {
                        // space matches 0 or more white space characters
                        if (is.good())
                           ws(is);
                    }
                    else
                        read(is, *fmt);
                }
                break;
            }
        }
        // is.fail() || *fmt == CharT{}
        if (is.rdstate() == ios::goodbit && command)
        {
            if (modified == CharT{})
                read(is, CharT{'%'}, width);
            else
                read(is, CharT{'%'}, width, modified);
        }
        if (!is.fail())
        {
            if (y != not_a_2digit_year)
            {
                // Convert y and an optional C to Y
                if (!(0 <= y && y <= 99))
                    goto broken;
                if (C == not_a_century)
                {
                    if (Y == not_a_year)
                    {
                        if (y >= 69)
                            C = 19;
                        else
                            C = 20;
                    }
                    else
                    {
                        C = (Y >= 0 ? Y : Y-100) / 100;
                    }
                }
                int tY;
                if (C >= 0)
                    tY = 100*C + y;
                else
                    tY = 100*(C+1) - (y == 0 ? 100 : y);
                if (Y != not_a_year && Y != tY)
                    goto broken;
                Y = tY;
            }
            if (g != not_a_2digit_year)
            {
                // Convert g and an optional C to G
                if (!(0 <= g && g <= 99))
                    goto broken;
                if (C == not_a_century)
                {
                    if (G == not_a_year)
                    {
                        if (g >= 69)
                            C = 19;
                        else
                            C = 20;
                    }
                    else
                    {
                        C = (G >= 0 ? G : G-100) / 100;
                    }
                }
                int tG;
                if (C >= 0)
                    tG = 100*C + g;
                else
                    tG = 100*(C+1) - (g == 0 ? 100 : g);
                if (G != not_a_year && G != tG)
                    goto broken;
                G = tG;
            }
            if (Y < static_cast<int>(year::min()) || Y > static_cast<int>(year::max()))
                Y = not_a_year;
            bool computed = false;
            if (G != not_a_year && V != not_a_week_num && wd != not_a_weekday)
            {
                year_month_day ymd_trial = sys_days(year{G-1}/December/Thursday[last]) +
                                           (Monday-Thursday) + weeks{V-1} +
                                           (date::weekday{static_cast<unsigned>(wd)}-Monday);
                if (Y == not_a_year)
                    Y = static_cast<int>(ymd_trial.year());
                else if (year{Y} != ymd_trial.year())
                    goto broken;
                if (m == not_a_month)
                    m = static_cast<int>(static_cast<unsigned>(ymd_trial.month()));
                else if (month(static_cast<unsigned>(m)) != ymd_trial.month())
                    goto broken;
                if (d == not_a_day)
                    d = static_cast<int>(static_cast<unsigned>(ymd_trial.day()));
                else if (day(static_cast<unsigned>(d)) != ymd_trial.day())
                    goto broken;
                computed = true;
            }
            if (Y != not_a_year && U != not_a_week_num && wd != not_a_weekday)
            {
                year_month_day ymd_trial = sys_days(year{Y}/January/Sunday[1]) +
                                           weeks{U-1} +
                                           (date::weekday{static_cast<unsigned>(wd)} - Sunday);
                if (Y == not_a_year)
                    Y = static_cast<int>(ymd_trial.year());
                else if (year{Y} != ymd_trial.year())
                    goto broken;
                if (m == not_a_month)
                    m = static_cast<int>(static_cast<unsigned>(ymd_trial.month()));
                else if (month(static_cast<unsigned>(m)) != ymd_trial.month())
                    goto broken;
                if (d == not_a_day)
                    d = static_cast<int>(static_cast<unsigned>(ymd_trial.day()));
                else if (day(static_cast<unsigned>(d)) != ymd_trial.day())
                    goto broken;
                computed = true;
            }
            if (Y != not_a_year && W != not_a_week_num && wd != not_a_weekday)
            {
                year_month_day ymd_trial = sys_days(year{Y}/January/Monday[1]) +
                                           weeks{W-1} +
                                           (date::weekday{static_cast<unsigned>(wd)} - Monday);
                if (Y == not_a_year)
                    Y = static_cast<int>(ymd_trial.year());
                else if (year{Y} != ymd_trial.year())
                    goto broken;
                if (m == not_a_month)
                    m = static_cast<int>(static_cast<unsigned>(ymd_trial.month()));
                else if (month(static_cast<unsigned>(m)) != ymd_trial.month())
                    goto broken;
                if (d == not_a_day)
                    d = static_cast<int>(static_cast<unsigned>(ymd_trial.day()));
                else if (day(static_cast<unsigned>(d)) != ymd_trial.day())
                    goto broken;
                computed = true;
            }
            if (j != not_a_doy && Y != not_a_year)
            {
                auto ymd_trial = year_month_day{local_days(year{Y}/1/1) + days{j-1}};
                if (m == 0)
                    m = static_cast<int>(static_cast<unsigned>(ymd_trial.month()));
                else if (month(static_cast<unsigned>(m)) != ymd_trial.month())
                    goto broken;
                if (d == 0)
                    d = static_cast<int>(static_cast<unsigned>(ymd_trial.day()));
                else if (day(static_cast<unsigned>(d)) != ymd_trial.day())
                    goto broken;
                j = not_a_doy;
            }
            auto ymd = year{Y}/m/d;
            if (ymd.ok())
            {
                if (wd == not_a_weekday)
                    wd = static_cast<int>((date::weekday(sys_days(ymd)) - Sunday).count());
                else if (wd != static_cast<int>((date::weekday(sys_days(ymd)) - Sunday).count()))
                    goto broken;
                if (!computed)
                {
                    if (G != not_a_year || V != not_a_week_num)
                    {
                        sys_days sd = ymd;
                        auto G_trial = year_month_day{sd + days{3}}.year();
                        auto start = sys_days((G_trial - years{1})/December/Thursday[last]) +
                                     (Monday - Thursday);
                        if (sd < start)
                        {
                            --G_trial;
                            if (V != not_a_week_num)
                                start = sys_days((G_trial - years{1})/December/Thursday[last])
                                        + (Monday - Thursday);
                        }
                        if (G != not_a_year && G != static_cast<int>(G_trial))
                            goto broken;
                        if (V != not_a_week_num)
                        {
                            auto V_trial = duration_cast<weeks>(sd - start).count() + 1;
                            if (V != V_trial)
                                goto broken;
                        }
                    }
                    if (U != not_a_week_num)
                    {
                        auto start = sys_days(Sunday[1]/January/ymd.year());
                        auto U_trial = floor<weeks>(sys_days(ymd) - start).count() + 1;
                        if (U != U_trial)
                            goto broken;
                    }
                    if (W != not_a_week_num)
                    {
                        auto start = sys_days(Monday[1]/January/ymd.year());
                        auto W_trial = floor<weeks>(sys_days(ymd) - start).count() + 1;
                        if (W != W_trial)
                            goto broken;
                    }
                }
            }
            fds.ymd = ymd;
            if (I != not_a_hour_12_value)
            {
                if (!(1 <= I && I <= 12))
                    goto broken;
                if (p != not_a_ampm)
                {
                    // p is in [0, 1] == [AM, PM]
                    // Store trial H in I
                    if (I == 12)
                        --p;
                    I += p*12;
                    // Either set H from I or make sure H and I are consistent
                    if (H == not_a_hour)
                        H = I;
                    else if (I != H)
                        goto broken;
                }
                else  // p == not_a_ampm
                {
                    // if H, make sure H and I could be consistent
                    if (H != not_a_hour)
                    {
                        if (I == 12)
                        {
                            if (H != 0 && H != 12)
                                goto broken;
                        }
                        else if (!(I == H || I == H+12))
                        {
                            goto broken;
                        }
                    }
                }
            }
            if (H != not_a_hour)
            {
                fds.has_tod = true;
                fds.tod = hh_mm_ss<Duration>{hours{H}};
            }
            if (M != not_a_minute)
            {
                fds.has_tod = true;
                fds.tod.minutes(date::detail::undocumented{}) = minutes{M};
            }
            if (s != not_a_second)
            {
                fds.has_tod = true;
                const date::detail::decimal_format_seconds<Duration> dfs{s};
                fds.tod.seconds(date::detail::undocumented{}) = dfs.seconds();
                fds.tod.subseconds(date::detail::undocumented{}) = dfs.subseconds();
            }
            if (j != not_a_doy)
            {
                fds.has_tod = true;
                fds.tod.hours(date::detail::undocumented{}) += hours{days{j}};
            }
            if (wd != not_a_weekday)
                fds.wd = date::weekday{static_cast<unsigned>(wd)};
            if (abbrev != nullptr)
                *abbrev = std::move(temp_abbrev);
            if (offset != nullptr && temp_offset != not_a_offset)
              *offset = temp_offset;
        }
       return is;
    }
broken:
    is.setstate(ios::failbit);
    return is;
}

// Parse into sys_time
template <class Duration, class CharT, class Traits, class Alloc = std::allocator<CharT>>
std::basic_istream<CharT, Traits>&
from_stream(std::basic_istream<CharT, Traits>& is,
            const CharT* fmt,
            const std::pair<const std::string*, const std::string*>& month_names_pair,
            const std::pair<const std::string*, const std::string*>& weekday_names_pair,
            const std::pair<const std::string*, const std::string*>& ampm_names_pair,
            const CharT& decimal_mark,
            date::sys_time<Duration>& tp,
            std::basic_string<CharT, Traits, Alloc>* abbrev = nullptr,
            std::chrono::minutes* offset = nullptr)
{
  using CT = typename std::common_type<Duration, std::chrono::seconds>::type;
  using date::detail::round_i;
  std::chrono::minutes offset_local{};
  auto offptr = offset ? offset : &offset_local;
  date::fields<CT> fds{};
  fds.has_tod = true;
  rclock::from_stream(is, fmt, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark, fds, abbrev, offptr);
  if (!fds.ymd.ok() || !fds.tod.in_conventional_range())
    is.setstate(std::ios::failbit);
  if (!is.fail())
    tp = round_i<Duration>(date::sys_days(fds.ymd) - *offptr + fds.tod.to_duration());
  return is;
}

// Parse into local_time
template <class Duration, class CharT, class Traits, class Alloc = std::allocator<CharT>>
std::basic_istream<CharT, Traits>&
from_stream(std::basic_istream<CharT, Traits>& is,
            const CharT* fmt,
            const std::pair<const std::string*, const std::string*>& month_names_pair,
            const std::pair<const std::string*, const std::string*>& weekday_names_pair,
            const std::pair<const std::string*, const std::string*>& ampm_names_pair,
            const CharT& decimal_mark,
            date::local_time<Duration>& tp,
            std::basic_string<CharT, Traits, Alloc>* abbrev = nullptr,
            std::chrono::minutes* offset = nullptr)
{
  using CT = typename std::common_type<Duration, std::chrono::seconds>::type;
  using date::detail::round_i;
  date::fields<CT> fds{};
  fds.has_tod = true;
  rclock::from_stream(is, fmt, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark, fds, abbrev, offset);
  if (!fds.ymd.ok() || !fds.tod.in_conventional_range())
    is.setstate(std::ios::failbit);
  if (!is.fail())
    tp = round_i<Duration>(date::local_days(fds.ymd) + fds.tod.to_duration());
  return is;
}

template <class CharT, class Traits, class Alloc = std::allocator<CharT>>
std::basic_istream<CharT, Traits>&
from_stream(std::basic_istream<CharT, Traits>& is,
            const CharT* fmt,
            const std::pair<const std::string*, const std::string*>& month_names_pair,
            const std::pair<const std::string*, const std::string*>& weekday_names_pair,
            const std::pair<const std::string*, const std::string*>& ampm_names_pair,
            const CharT& decimal_mark,
            date::year& y,
            std::basic_string<CharT, Traits, Alloc>* abbrev = nullptr,
            std::chrono::minutes* offset = nullptr)
{
    using CT = std::chrono::seconds;
    date::fields<CT> fds{};
    rclock::from_stream(is, fmt, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark, fds, abbrev, offset);
    if (!fds.ymd.year().ok())
        is.setstate(std::ios::failbit);
    if (!is.fail())
        y = fds.ymd.year();
    return is;
}

template <class CharT, class Traits, class Alloc = std::allocator<CharT>>
std::basic_istream<CharT, Traits>&
from_stream(std::basic_istream<CharT, Traits>& is,
            const CharT* fmt,
            const std::pair<const std::string*, const std::string*>& month_names_pair,
            const std::pair<const std::string*, const std::string*>& weekday_names_pair,
            const std::pair<const std::string*, const std::string*>& ampm_names_pair,
            const CharT& decimal_mark,
            date::year_month& ym,
            std::basic_string<CharT, Traits, Alloc>* abbrev = nullptr,
            std::chrono::minutes* offset = nullptr)
{
    using CT = std::chrono::seconds;
    date::fields<CT> fds{};
    rclock::from_stream(is, fmt, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark, fds, abbrev, offset);
    if (!fds.ymd.month().ok())
        is.setstate(std::ios::failbit);
    if (!is.fail())
        ym = fds.ymd.year()/fds.ymd.month();
    return is;
}

/*
 * Extra template variations of `from_stream()`
 *
 * Both allow "invalid" dates to be parsed, like 2020-02-30. This ensures we
 * can parse() what we produce from format().
 *
 * The first `from_stream()` variant accepts and ymd and tod directly, which
 * no <date> `from_stream()` variants do.
 */

template <class Duration, class CharT, class Traits, class Alloc = std::allocator<CharT>>
std::basic_istream<CharT, Traits>&
from_stream(std::basic_istream<CharT, Traits>& is,
            const CharT* fmt,
            const std::pair<const std::string*, const std::string*>& month_names_pair,
            const std::pair<const std::string*, const std::string*>& weekday_names_pair,
            const std::pair<const std::string*, const std::string*>& ampm_names_pair,
            const CharT& decimal_mark,
            date::year_month_day& ymd,
            date::hh_mm_ss<Duration>& tod,
            std::basic_string<CharT, Traits, Alloc>* abbrev = nullptr,
            std::chrono::minutes* offset = nullptr)
{
  using CT = typename std::common_type<Duration, std::chrono::seconds>::type;
  std::chrono::minutes offset_local{};
  std::chrono::minutes* offptr = offset ? offset : &offset_local;
  date::fields<CT> fds{};
  fds.has_tod = true;
  rclock::from_stream(is, fmt, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark, fds, abbrev, offptr);
  // Fields must be `ok()` independently, not jointly. i.e. invalid dates are allowed.
  if (!fds.ymd.year().ok() || !fds.ymd.month().ok() || !fds.ymd.day().ok() || !fds.tod.in_conventional_range())
    is.setstate(std::ios::failbit);
  if (!is.fail()) {
    ymd = fds.ymd;
    tod = fds.tod;
  }
  return is;
}

template <class CharT, class Traits, class Alloc = std::allocator<CharT>>
std::basic_istream<CharT, Traits>&
from_stream(std::basic_istream<CharT, Traits>& is,
            const CharT* fmt,
            const std::pair<const std::string*, const std::string*>& month_names_pair,
            const std::pair<const std::string*, const std::string*>& weekday_names_pair,
            const std::pair<const std::string*, const std::string*>& ampm_names_pair,
            const CharT& decimal_mark,
            date::year_month_day& ymd,
            std::basic_string<CharT, Traits, Alloc>* abbrev = nullptr,
            std::chrono::minutes* offset = nullptr)
{
  using CT = std::chrono::seconds;
  date::fields<CT> fds{};
  rclock::from_stream(is, fmt, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark, fds, abbrev, offset);
  // Fields must be `ok()` independently, not jointly. i.e. invalid dates are allowed.
  if (!fds.ymd.year().ok() || !fds.ymd.month().ok() || !fds.ymd.day().ok())
    is.setstate(std::ios::failbit);
  if (!is.fail())
    ymd = fds.ymd;
  return is;
}

static
inline
void
fill_formats(const cpp11::strings& src, std::vector<std::string>& dest) {
    const r_ssize size = src.size();
    for (r_ssize i = 0; i < size; ++i) {
        std::string elt = src[i];
        dest[i] = elt;
    }
}

} // namespace rclock

#endif
