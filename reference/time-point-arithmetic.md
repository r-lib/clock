# Arithmetic: Time points

These are naive-time and sys-time methods for the [arithmetic
generics](https://clock.r-lib.org/reference/clock-arithmetic.md).

- [`add_weeks()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_days()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_hours()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_minutes()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_seconds()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_milliseconds()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_microseconds()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_nanoseconds()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

When working with zoned times, generally you convert to either sys-time
or naive-time, add the duration, then convert back to zoned time.
Typically, *weeks and days* are added in *naive-time*, and *hours,
minutes, seconds, and subseconds* are added in *sys-time*.

If you aren't using zoned times, arithmetic on sys-times and naive-time
is equivalent.

If you need to add larger irregular units of time, such as months,
quarters, or years, convert to a calendar type with a converter like
[`as_year_month_day()`](https://clock.r-lib.org/reference/as_year_month_day.md).

## Usage

``` r
# S3 method for class 'clock_time_point'
add_weeks(x, n, ...)

# S3 method for class 'clock_time_point'
add_days(x, n, ...)

# S3 method for class 'clock_time_point'
add_hours(x, n, ...)

# S3 method for class 'clock_time_point'
add_minutes(x, n, ...)

# S3 method for class 'clock_time_point'
add_seconds(x, n, ...)

# S3 method for class 'clock_time_point'
add_milliseconds(x, n, ...)

# S3 method for class 'clock_time_point'
add_microseconds(x, n, ...)

# S3 method for class 'clock_time_point'
add_nanoseconds(x, n, ...)
```

## Arguments

- x:

  `[clock_sys_time / clock_naive_time]`

  A time point vector.

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

## Examples

``` r
library(magrittr)

# Say you started with this zoned time, and you want to add 1 day to it
x <- as_naive_time(year_month_day(1970, 04, 25, 02, 30, 00))
x <- as_zoned_time(x, "America/New_York")
x
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-25T02:30:00-05:00"

# Note that there was a daylight saving time gap on 1970-04-26 where
# we jumped from 01:59:59 -> 03:00:00.

# You can choose to add 1 day in "system time", by first converting to
# sys-time (the equivalent UTC time), adding the day, then converting back to
# zoned time. If you sat still for exactly 86,400 seconds, this is the
# time that you would see after daylight saving time adjusted the clock
# (note that the hour field is shifted forward by the size of the gap)
as_sys_time(x)
#> <sys_time<second>[1]>
#> [1] "1970-04-25T07:30:00"

x %>%
  as_sys_time() %>%
  add_days(1) %>%
  as_zoned_time(zoned_time_zone(x))
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T03:30:00-04:00"

# Alternatively, you can add 1 day in "naive time". Naive time represents
# a clock time with a yet-to-be-specified time zone. It tries to maintain
# smaller units where possible, so adding 1 day would attempt to return
# "1970-04-26T02:30:00" in the America/New_York time zone, but...
as_naive_time(x)
#> <naive_time<second>[1]>
#> [1] "1970-04-25T02:30:00"

try({
x %>%
  as_naive_time() %>%
  add_days(1) %>%
  as_zoned_time(zoned_time_zone(x))
})
#> Error in as_zoned_time(., zoned_time_zone(x)) : 
#>   Nonexistent time due to daylight saving time at location 1.
#> ℹ Resolve nonexistent time issues by specifying the `nonexistent` argument.

# ...this time doesn't exist in that time zone! It is "nonexistent".
# You can resolve nonexistent times by setting the `nonexistent` argument
# when converting to zoned time. Let's roll forward to the next available
# moment in time.
x %>%
  as_naive_time() %>%
  add_days(1) %>%
  as_zoned_time(zoned_time_zone(x), nonexistent = "roll-forward")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "1970-04-26T03:00:00-04:00"
```
