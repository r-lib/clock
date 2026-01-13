# Clock arithmetic

This is the landing page for all clock arithmetic functions. There are
specific sub-pages describing how arithmetic works for different
calendars and time points, which is where you should look for more
information.

Calendars are efficient at arithmetic with irregular units of time, such
as month, quarters, or years.

- [year-month-day](https://clock.r-lib.org/reference/year-month-day-arithmetic.md)

- [year-month-weekday](https://clock.r-lib.org/reference/year-month-weekday-arithmetic.md)

- [year-quarter-day](https://clock.r-lib.org/reference/year-quarter-day-arithmetic.md)

- [year-week-day](https://clock.r-lib.org/reference/year-week-day-arithmetic.md)

- [iso-year-week-day](https://clock.r-lib.org/reference/iso-year-week-day-arithmetic.md)

- [year-day](https://clock.r-lib.org/reference/year-day-arithmetic.md)

Time points, such as naive-times and sys-times, are efficient at
arithmetic with regular, well-defined units of time, such as days,
hours, seconds, or nanoseconds.

- [time-point](https://clock.r-lib.org/reference/time-point-arithmetic.md)

Durations can use any of these arithmetic functions, and return a new
duration with a precision corresponding to the common type of the input
and the function used.

- [duration](https://clock.r-lib.org/reference/duration-arithmetic.md)

Weekdays can perform day-based circular arithmetic.

- [weekday](https://clock.r-lib.org/reference/weekday-arithmetic.md)

There are also convenience methods for doing arithmetic directly on a
native R date or date-time type:

- [dates (Date)](https://clock.r-lib.org/reference/Date-arithmetic.md)

- [date-times (POSIXct /
  POSIXlt)](https://clock.r-lib.org/reference/posixt-arithmetic.md)

## Usage

``` r
add_years(x, n, ...)

add_quarters(x, n, ...)

add_months(x, n, ...)

add_weeks(x, n, ...)

add_days(x, n, ...)

add_hours(x, n, ...)

add_minutes(x, n, ...)

add_seconds(x, n, ...)

add_milliseconds(x, n, ...)

add_microseconds(x, n, ...)

add_nanoseconds(x, n, ...)
```

## Arguments

- x:

  `[object]`

  An object.

- n:

  `[integer / clock_duration]`

  An integer vector to be converted to a duration, or a duration
  corresponding to the arithmetic function being used. This corresponds
  to the number of duration units to add. `n` may be negative to
  subtract units of duration.

- ...:

  These dots are for future extensions and must be empty.

## Value

`x` after performing the arithmetic.

## Details

`x` and `n` are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

Months and years are considered "irregular" because some months have
more days then others (28, 29, 30, or 31), and some years have more days
than others (365 or 366).

Days are considered "regular" because they are defined as 86,400
seconds.

## Examples

``` r
# See each sub-page for more specific examples
x <- year_month_day(2019, 2, 1)
add_months(x, 1)
#> <year_month_day<day>[1]>
#> [1] "2019-03-01"
```
