# Examples and Recipes

``` r
library(clock)
library(magrittr)
```

This vignette shows common examples and recipes that might be useful
when learning about clock. Where possible, both the high and low level
API are shown.

Many of these examples are adapted from the date C++ library’s [Examples
and
Recipes](https://github.com/HowardHinnant/date/wiki/Examples-and-Recipes)
page.

## The current local time

[`zoned_time_now()`](https://clock.r-lib.org/reference/zoned_time_now.md)
returns the current time in a particular time zone. It will display up
to nanosecond precision, but the exact amount is OS dependent (on a Mac
this displays microsecond level information at nanosecond resolution).

Using `""` as the time zone string will try and use whatever R thinks
your local time zone is (i.e. from
[`Sys.timezone()`](https://rdrr.io/r/base/timezones.html)).

``` r
zoned_time_now("")
#> <zoned_time<nanosecond><America/New_York (current)>[1]>
#> [1] "2021-02-10T15:54:29.875011000-05:00"
```

## The current time somewhere else

Pass a time zone name to
[`zoned_time_now()`](https://clock.r-lib.org/reference/zoned_time_now.md)
to get the current time somewhere else.

``` r
zoned_time_now("Asia/Shanghai")
#> <zoned_time<nanosecond><Asia/Shanghai>[1]>
#> [1] "2021-02-11T04:54:29.875011000+08:00"
```

## Set a meeting across time zones

Say you need to set a meeting with someone in Shanghai, but you live in
New York. If you set a meeting for 9am, what time is that for them?

``` r
my_time <- year_month_day(2019, 1, 30, 9) %>%
  as_naive_time() %>%
  as_zoned_time("America/New_York")

my_time
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-30T09:00:00-05:00"

their_time <- zoned_time_set_zone(my_time, "Asia/Shanghai")

their_time
#> <zoned_time<second><Asia/Shanghai>[1]>
#> [1] "2019-01-30T22:00:00+08:00"
```

### High level API

``` r
my_time <- as.POSIXct("2019-01-30 09:00:00", "America/New_York")

date_time_set_zone(my_time, "Asia/Shanghai")
#> [1] "2019-01-30 22:00:00 CST"
```

## Force a specific time zone

Say your co-worker in Shanghai (from the last example) accidentally
logged on at 9am *their time*. What time would this be for you?

The first step to solve this is to force `my_time` to have the same
printed time, but use the Asia/Shanghai time zone. You can do this by
going through naive-time:

``` r
my_time <- year_month_day(2019, 1, 30, 9) %>%
  as_naive_time() %>%
  as_zoned_time("America/New_York")

my_time
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-30T09:00:00-05:00"

# Drop the time zone information, retaining the printed time
my_time %>%
  as_naive_time()
#> <naive_time<second>[1]>
#> [1] "2019-01-30T09:00:00"

# Add the correct time zone name back on,
# again retaining the printed time
their_9am <- my_time %>%
  as_naive_time() %>%
  as_zoned_time("Asia/Shanghai")

their_9am
#> <zoned_time<second><Asia/Shanghai>[1]>
#> [1] "2019-01-30T09:00:00+08:00"
```

Note that a conversion like this isn’t always possible due to daylight
saving time issues, in which case you might need to set the
`nonexistent` and `ambiguous` arguments of
[`as_zoned_time()`](https://clock.r-lib.org/reference/as_zoned_time.md).

What time would this have been for you in New York?

``` r
zoned_time_set_zone(their_9am, "America/New_York")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-29T20:00:00-05:00"
```

### High level API

``` r
my_time <- as.POSIXct("2019-01-30 09:00:00", "America/New_York")

my_time %>%
  as_naive_time() %>%
  as.POSIXct("Asia/Shanghai") %>%
  date_time_set_zone("America/New_York")
#> [1] "2019-01-29 20:00:00 EST"
```

## Finding the next Monday (or Thursday)

Given a particular day precision naive-time, how can you compute the
next Monday? This is very easily accomplished with
[`time_point_shift()`](https://clock.r-lib.org/reference/time_point_shift.md).
It takes a time point vector and a “target” weekday, and shifts the time
points to that target weekday.

``` r
days <- as_naive_time(year_month_day(2019, c(1, 2), 1))

# A Tuesday and a Friday
as_weekday(days)
#> <weekday[2]>
#> [1] Tue Fri

monday <- weekday(clock_weekdays$monday)

time_point_shift(days, monday)
#> <naive_time<day>[2]>
#> [1] "2019-01-07" "2019-02-04"

as_weekday(time_point_shift(days, monday))
#> <weekday[2]>
#> [1] Mon Mon
```

You can also shift to the previous instance of the target weekday:

``` r
time_point_shift(days, monday, which = "previous")
#> <naive_time<day>[2]>
#> [1] "2018-12-31" "2019-01-28"
```

If you happen to already be on the target weekday, the default behavior
returns the input unchanged. However, you can also chose to advance to
the next instance of the target.

``` r
tuesday <- weekday(clock_weekdays$tuesday)

time_point_shift(days, tuesday)
#> <naive_time<day>[2]>
#> [1] "2019-01-01" "2019-02-05"
time_point_shift(days, tuesday, boundary = "advance")
#> <naive_time<day>[2]>
#> [1] "2019-01-08" "2019-02-05"
```

While
[`time_point_shift()`](https://clock.r-lib.org/reference/time_point_shift.md)
is built in to clock, it can be useful to discuss the arithmetic going
on in the underlying weekday type which powers this function. To do so,
we will build some parts of
[`time_point_shift()`](https://clock.r-lib.org/reference/time_point_shift.md)
from scratch.

The weekday type represents a single day of the week and implements
*circular arithmetic*. Let’s see the code for a simple version of
[`time_point_shift()`](https://clock.r-lib.org/reference/time_point_shift.md)
that just shifts to the next target weekday:

``` r
next_weekday <- function(x, target) {
  x + (target - as_weekday(x))
}

next_weekday(days, monday)
#> <naive_time<day>[2]>
#> [1] "2019-01-07" "2019-02-04"

as_weekday(next_weekday(days, monday))
#> <weekday[2]>
#> [1] Mon Mon
```

Let’s break down how `next_weekday()` works. The first step takes the
difference between two weekday vectors. It does this using circular
arithmetic. Once we get passed the 7th day of the week (whatever that
may be), it wraps back around to the 1st day of the week. Implementing
weekday arithmetic in this way means that the following nicely returns
the number of days until the next Monday as a day based duration:

``` r
monday - as_weekday(days)
#> <duration<day>[2]>
#> [1] 6 3
```

Which can be added to our day precision `days` vector to get the date of
the next Monday:

``` r
days + (monday - as_weekday(days))
#> <naive_time<day>[2]>
#> [1] "2019-01-07" "2019-02-04"
```

The current implementation will return the input if it is already on the
target weekday. To use the `boundary = "advance"` behavior, you could
implement `next_weekday()` as:

``` r
next_weekday2 <- function(x, target) {
  x <- x + duration_days(1L)
  x + (target - as_weekday(x))
}

a_monday <- as_naive_time(year_month_day(2018, 12, 31))
as_weekday(a_monday)
#> <weekday[1]>
#> [1] Mon

next_weekday2(a_monday, monday)
#> <naive_time<day>[1]>
#> [1] "2019-01-07"
```

### High level API

In the high level API, you can use
[`date_shift()`](https://clock.r-lib.org/reference/date-and-date-time-shifting.md):

``` r
monday <- weekday(clock_weekdays$monday)

x <- as.Date(c("2019-01-01", "2019-02-01"))

date_shift(x, monday)
#> [1] "2019-01-07" "2019-02-04"

# With a date-time
y <- as.POSIXct(
  c("2019-01-01 02:30:30", "2019-02-01 05:20:22"), 
  "America/New_York"
)

date_shift(y, monday)
#> [1] "2019-01-07 02:30:30 EST" "2019-02-04 05:20:22 EST"
```

Note that adding weekdays to a POSIXct could generate nonexistent or
ambiguous times due to daylight saving time, which would have to be
handled by supplying `nonexistent` and `ambiguous` arguments to
[`date_shift()`](https://clock.r-lib.org/reference/date-and-date-time-shifting.md).

## Generate sequences of dates and date-times

clock implements S3 methods for the
[`seq()`](https://rdrr.io/r/base/seq.html) generic function for the
calendar and time point types it provides. The precision that you can
generate sequences for depends on the type.

- year-month-day: Yearly or monthly sequences
- year-quarter-day: Yearly or quarterly sequences
- sys-time / naive-time: Weekly, Daily, Hourly, …, Subsecond sequences

When generating sequences, the type and precision of `from` determine
the result. For example:

``` r
ym <- seq(year_month_day(2019, 1), by = 2, length.out = 10)
ym
#> <year_month_day<month>[10]>
#>  [1] "2019-01" "2019-03" "2019-05" "2019-07" "2019-09" "2019-11"
#>  [7] "2020-01" "2020-03" "2020-05" "2020-07"
```

``` r
yq <- seq(year_quarter_day(2019, 1), by = 2, length.out = 10)
```

This allows you to generate sequences of year-months or year-quarters
without having to worry about the day of the month/quarter becoming
invalid. You can set the day of the results to get to a day precision
calendar. For example, to get the last days of the month/quarter for
this sequence:

``` r
set_day(ym, "last")
#> <year_month_day<day>[10]>
#>  [1] "2019-01-31" "2019-03-31" "2019-05-31" "2019-07-31" "2019-09-30"
#>  [6] "2019-11-30" "2020-01-31" "2020-03-31" "2020-05-31" "2020-07-31"

set_day(yq, "last")
#> <year_quarter_day<January><day>[10]>
#>  [1] "2019-Q1-90" "2019-Q3-92" "2020-Q1-91" "2020-Q3-92" "2021-Q1-90"
#>  [6] "2021-Q3-92" "2022-Q1-90" "2022-Q3-92" "2023-Q1-90" "2023-Q3-92"
```

You won’t be able to generate day precision sequences with calendars.
Instead, you should use a time point.

``` r
from <- as_naive_time(year_month_day(2019, 1, 1))
to <- as_naive_time(year_month_day(2019, 5, 15))

seq(from, to, by = 20)
#> <naive_time<day>[7]>
#> [1] "2019-01-01" "2019-01-21" "2019-02-10" "2019-03-02" "2019-03-22"
#> [6] "2019-04-11" "2019-05-01"
```

If you use an integer `by` value, it is interpreted as a duration at the
same precision as `from`. You can also use a duration object that can be
cast to the same precision as `from`. For example, to generate a
sequence spaced out by 90 minutes for these second precision end points:

``` r
from <- as_naive_time(year_month_day(2019, 1, 1, 2, 30, 00))
to <- as_naive_time(year_month_day(2019, 1, 1, 12, 30, 00))

seq(from, to, by = duration_minutes(90))
#> <naive_time<second>[7]>
#> [1] "2019-01-01T02:30:00" "2019-01-01T04:00:00" "2019-01-01T05:30:00"
#> [4] "2019-01-01T07:00:00" "2019-01-01T08:30:00" "2019-01-01T10:00:00"
#> [7] "2019-01-01T11:30:00"
```

### High level API

In the high level API, you can use
[`date_seq()`](https://clock.r-lib.org/reference/date_seq.md) to
generate sequences. This doesn’t have all of the flexibility of the
[`seq()`](https://rdrr.io/r/base/seq.html) methods above, but is still
extremely useful and has the added benefit of switching between
calendars, sys-times, and naive-times automatically for you.

If an integer `by` is supplied with a date `from`, it defaults to a
daily sequence:

``` r
date_seq(date_build(2019, 1), by = 2, total_size = 10)
#>  [1] "2019-01-01" "2019-01-03" "2019-01-05" "2019-01-07" "2019-01-09"
#>  [6] "2019-01-11" "2019-01-13" "2019-01-15" "2019-01-17" "2019-01-19"
```

You can generate a monthly sequence by supplying a month precision
duration for `by`.

``` r
date_seq(date_build(2019, 1), by = duration_months(2), total_size = 10)
#>  [1] "2019-01-01" "2019-03-01" "2019-05-01" "2019-07-01" "2019-09-01"
#>  [6] "2019-11-01" "2020-01-01" "2020-03-01" "2020-05-01" "2020-07-01"
```

If you supply `to`, be aware that all components of `to` that are more
precise than the precision of `by` must match `from` exactly. For
example, the day component of `from` and `to` doesn’t match here, so the
sequence isn’t defined.

``` r
date_seq(
  date_build(2019, 1, 1),
  to = date_build(2019, 10, 2),
  by = duration_months(2)
)
#> Error in `date_seq()`:
#> ! All components of `from` and `to` more precise than "month"
#>   must match.
#> ℹ `from` is "2019-01-01".
#> ℹ `to` is "2019-10-02".
```

[`date_seq()`](https://clock.r-lib.org/reference/date_seq.md) also
catches invalid dates for you, forcing you to specify the `invalid`
argument to specify how to handle them.

``` r
jan31 <- date_build(2019, 1, 31)
dec31 <- date_build(2019, 12, 31)

date_seq(jan31, to = dec31, by = duration_months(1))
#> Error in `invalid_resolve()`:
#> ! Invalid date found at location 2.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
```

By specifying `invalid = "previous"` here, we can generate month end
values.

``` r
date_seq(jan31, to = dec31, by = duration_months(1), invalid = "previous")
#>  [1] "2019-01-31" "2019-02-28" "2019-03-31" "2019-04-30" "2019-05-31"
#>  [6] "2019-06-30" "2019-07-31" "2019-08-31" "2019-09-30" "2019-10-31"
#> [11] "2019-11-30" "2019-12-31"
```

Compare this with the automatic “overflow” behavior of
[`seq()`](https://rdrr.io/r/base/seq.html), which is often a source of
confusion.

``` r
seq(jan31, to = dec31, by = "1 month")
#>  [1] "2019-01-31" "2019-03-03" "2019-03-31" "2019-05-01" "2019-05-31"
#>  [6] "2019-07-01" "2019-07-31" "2019-08-31" "2019-10-01" "2019-10-31"
#> [11] "2019-12-01" "2019-12-31"
```

## Grouping by months or quarters

When working on a data analysis, you might be required to summarize
certain metrics at a monthly or quarterly level. With
[`calendar_group()`](https://clock.r-lib.org/reference/calendar_group.md),
you can easily summarize at the granular precision that you care about.
Take this vector of day precision naive-times in 2019:

``` r
from <- as_naive_time(year_month_day(2019, 1, 1))
to <- as_naive_time(year_month_day(2019, 12, 31))

x <- seq(from, to, by = duration_days(20))

x
#> <naive_time<day>[19]>
#>  [1] "2019-01-01" "2019-01-21" "2019-02-10" "2019-03-02" "2019-03-22"
#>  [6] "2019-04-11" "2019-05-01" "2019-05-21" "2019-06-10" "2019-06-30"
#> [11] "2019-07-20" "2019-08-09" "2019-08-29" "2019-09-18" "2019-10-08"
#> [16] "2019-10-28" "2019-11-17" "2019-12-07" "2019-12-27"
```

To group by month, first convert to a year-month-day:

``` r
ymd <- as_year_month_day(x)

head(ymd)
#> <year_month_day<day>[6]>
#> [1] "2019-01-01" "2019-01-21" "2019-02-10" "2019-03-02" "2019-03-22"
#> [6] "2019-04-11"

calendar_group(ymd, "month")
#> <year_month_day<month>[19]>
#>  [1] "2019-01" "2019-01" "2019-02" "2019-03" "2019-03" "2019-04"
#>  [7] "2019-05" "2019-05" "2019-06" "2019-06" "2019-07" "2019-08"
#> [13] "2019-08" "2019-09" "2019-10" "2019-10" "2019-11" "2019-12"
#> [19] "2019-12"
```

To group by quarter, convert to a year-quarter-day:

``` r
yqd <- as_year_quarter_day(x)

head(yqd)
#> <year_quarter_day<January><day>[6]>
#> [1] "2019-Q1-01" "2019-Q1-21" "2019-Q1-41" "2019-Q1-61" "2019-Q1-81"
#> [6] "2019-Q2-11"

calendar_group(yqd, "quarter")
#> <year_quarter_day<January><quarter>[19]>
#>  [1] "2019-Q1" "2019-Q1" "2019-Q1" "2019-Q1" "2019-Q1" "2019-Q2"
#>  [7] "2019-Q2" "2019-Q2" "2019-Q2" "2019-Q2" "2019-Q3" "2019-Q3"
#> [13] "2019-Q3" "2019-Q3" "2019-Q4" "2019-Q4" "2019-Q4" "2019-Q4"
#> [19] "2019-Q4"
```

If you need to group by a multiple of months / quarters, you can do that
too:

``` r
calendar_group(ymd, "month", n = 2)
#> <year_month_day<month>[19]>
#>  [1] "2019-01" "2019-01" "2019-01" "2019-03" "2019-03" "2019-03"
#>  [7] "2019-05" "2019-05" "2019-05" "2019-05" "2019-07" "2019-07"
#> [13] "2019-07" "2019-09" "2019-09" "2019-09" "2019-11" "2019-11"
#> [19] "2019-11"

calendar_group(yqd, "quarter", n = 2)
#> <year_quarter_day<January><quarter>[19]>
#>  [1] "2019-Q1" "2019-Q1" "2019-Q1" "2019-Q1" "2019-Q1" "2019-Q1"
#>  [7] "2019-Q1" "2019-Q1" "2019-Q1" "2019-Q1" "2019-Q3" "2019-Q3"
#> [13] "2019-Q3" "2019-Q3" "2019-Q3" "2019-Q3" "2019-Q3" "2019-Q3"
#> [19] "2019-Q3"
```

Note that the returned calendar vector is at the precision we grouped
by, not at the original precision with, say, the day of the month /
quarter set to `1`.

Additionally, be aware that
[`calendar_group()`](https://clock.r-lib.org/reference/calendar_group.md)
groups “within” the component that is one unit of precision larger than
the `precision` you specify. So, when grouping by `"day"`, this groups
by “day of the month”, which can’t cross the month or year boundary. If
you need to bundle dates together by something like 60 days
(i.e. crossing the month boundary), then you should use
[`time_point_floor()`](https://clock.r-lib.org/reference/time-point-rounding.md).

### High level API

In the high level API, you can use
[`date_group()`](https://clock.r-lib.org/reference/date_group.md) to
group Date vectors by one of their 3 components: year, month, or day.
Since month precision dates can’t be represented with Date vectors,
[`date_group()`](https://clock.r-lib.org/reference/date_group.md) sets
the day of the month to 1.

``` r
x <- seq(as.Date("2019-01-01"), as.Date("2019-12-31"), by = 20)

date_group(x, "month")
#>  [1] "2019-01-01" "2019-01-01" "2019-02-01" "2019-03-01" "2019-03-01"
#>  [6] "2019-04-01" "2019-05-01" "2019-05-01" "2019-06-01" "2019-06-01"
#> [11] "2019-07-01" "2019-08-01" "2019-08-01" "2019-09-01" "2019-10-01"
#> [16] "2019-10-01" "2019-11-01" "2019-12-01" "2019-12-01"
```

You won’t be able to group by `"quarter"`, since this isn’t one of the 3
components that the high level API lets you work with. Instead, this is
a case where you should convert to a year-quarter-day, group on that
type, then convert back to Date.

``` r
x %>%
  as_year_quarter_day() %>%
  calendar_group("quarter") %>%
  set_day(1) %>%
  as.Date()
#>  [1] "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01" "2019-01-01"
#>  [6] "2019-04-01" "2019-04-01" "2019-04-01" "2019-04-01" "2019-04-01"
#> [11] "2019-07-01" "2019-07-01" "2019-07-01" "2019-07-01" "2019-10-01"
#> [16] "2019-10-01" "2019-10-01" "2019-10-01" "2019-10-01"
```

This is actually equivalent to `date_group(x, "month", n = 3)`. If your
fiscal year starts in January, you can use that instead. However, if
your fiscal year starts in a different month, say, June, you’ll need to
use the approach from above like so:

``` r
x %>%
  as_year_quarter_day(start = clock_months$june) %>%
  calendar_group("quarter") %>%
  set_day(1) %>%
  as.Date()
#>  [1] "2018-12-01" "2018-12-01" "2018-12-01" "2019-03-01" "2019-03-01"
#>  [6] "2019-03-01" "2019-03-01" "2019-03-01" "2019-06-01" "2019-06-01"
#> [11] "2019-06-01" "2019-06-01" "2019-06-01" "2019-09-01" "2019-09-01"
#> [16] "2019-09-01" "2019-09-01" "2019-12-01" "2019-12-01"
```

## Flooring by days

While
[`calendar_group()`](https://clock.r-lib.org/reference/calendar_group.md)
can group by “component”, it isn’t useful for bundling together sets of
time points that can cross month/year boundaries, like “60 days” of
data. For that, you are better off *flooring* by rolling sets of 60
days.

``` r
from <- as_naive_time(year_month_day(2019, 1, 1))
to <- as_naive_time(year_month_day(2019, 12, 31))

x <- seq(from, to, by = duration_days(20))
```

``` r
time_point_floor(x, "day", n = 60)
#> <naive_time<day>[19]>
#>  [1] "2018-12-15" "2018-12-15" "2018-12-15" "2019-02-13" "2019-02-13"
#>  [6] "2019-02-13" "2019-04-14" "2019-04-14" "2019-04-14" "2019-06-13"
#> [11] "2019-06-13" "2019-06-13" "2019-08-12" "2019-08-12" "2019-08-12"
#> [16] "2019-10-11" "2019-10-11" "2019-10-11" "2019-12-10"
```

Flooring operates on the underlying duration, which for day precision
time points is a count of days since the *origin*, 1970-01-01.

``` r
unclass(x[1])
#> $lower
#> [1] 2147483648
#> 
#> $upper
#> [1] 17897
#> 
#> attr(,"clock")
#> [1] 1
#> attr(,"precision")
#> [1] 4
```

The 60 day counter starts here, which means that any times between
`[1970-01-01, 1970-03-02)` are all floored to 1970-01-01. At
`1970-03-02`, the counter starts again.

If you would like to change this origin, you can provide a time point to
start counting from with the `origin` argument. This is mostly useful if
you are flooring by weeks and you want to change the day of the week
that the count starts on. Since 1970-01-01 is a Thursday, flooring by 14
days defaults to returning all Thursdays.

``` r
x <- seq(as_naive_time(year_month_day(2019, 1, 1)), by = 3, length.out = 10)
x
#> <naive_time<day>[10]>
#>  [1] "2019-01-01" "2019-01-04" "2019-01-07" "2019-01-10" "2019-01-13"
#>  [6] "2019-01-16" "2019-01-19" "2019-01-22" "2019-01-25" "2019-01-28"

thursdays <- time_point_floor(x, "day", n = 14)
thursdays
#> <naive_time<day>[10]>
#>  [1] "2018-12-27" "2018-12-27" "2018-12-27" "2019-01-10" "2019-01-10"
#>  [6] "2019-01-10" "2019-01-10" "2019-01-10" "2019-01-24" "2019-01-24"

as_weekday(thursdays)
#> <weekday[10]>
#>  [1] Thu Thu Thu Thu Thu Thu Thu Thu Thu Thu
```

You can use `origin` to change this to floor to Mondays.

``` r
origin <- as_naive_time(year_month_day(2018, 12, 31))
as_weekday(origin)
#> <weekday[1]>
#> [1] Mon

mondays <- time_point_floor(x, "day", n = 14, origin = origin)
mondays
#> <naive_time<day>[10]>
#>  [1] "2018-12-31" "2018-12-31" "2018-12-31" "2018-12-31" "2018-12-31"
#>  [6] "2019-01-14" "2019-01-14" "2019-01-14" "2019-01-14" "2019-01-28"

as_weekday(mondays)
#> <weekday[10]>
#>  [1] Mon Mon Mon Mon Mon Mon Mon Mon Mon Mon
```

### High level API

You can use
[`date_floor()`](https://clock.r-lib.org/reference/date-and-date-time-rounding.md)
with Date and POSIXct types.

``` r
x <- seq(as.Date("2019-01-01"), as.Date("2019-12-31"), by = 20)

date_floor(x, "day", n = 60)
#>  [1] "2018-12-15" "2018-12-15" "2018-12-15" "2019-02-13" "2019-02-13"
#>  [6] "2019-02-13" "2019-04-14" "2019-04-14" "2019-04-14" "2019-06-13"
#> [11] "2019-06-13" "2019-06-13" "2019-08-12" "2019-08-12" "2019-08-12"
#> [16] "2019-10-11" "2019-10-11" "2019-10-11" "2019-12-10"
```

The `origin` you provide should be another Date. For week precision
flooring with Dates, you can specify `"week"` as the precision.

``` r
x <- seq(as.Date("2019-01-01"), by = 3, length.out = 10)

origin <- as.Date("2018-12-31")

date_floor(x, "week", n = 2, origin = origin)
#>  [1] "2018-12-31" "2018-12-31" "2018-12-31" "2018-12-31" "2018-12-31"
#>  [6] "2019-01-14" "2019-01-14" "2019-01-14" "2019-01-14" "2019-01-28"
```

## Day of the year

To get the day of the year, convert to the year-day calendar type and
extract the day with
[`get_day()`](https://clock.r-lib.org/reference/clock-getters.md).

``` r
x <- year_month_day(2019, clock_months$july, 4)

yd <- as_year_day(x)
yd
#> <year_day<day>[1]>
#> [1] "2019-185"

get_day(yd)
#> [1] 185
```

### High level API

``` r
x <- as.Date("2019-07-04")

x %>%
  as_year_day() %>%
  get_day()
#> [1] 185
```

## Computing an age in years

To get the age of an individual in years, use
[`calendar_count_between()`](https://clock.r-lib.org/reference/calendar-count-between.md).

``` r
x <- year_month_day(1980, 12, 14:16)
today <- year_month_day(2005, 12, 15)

# Note that the month and day of the month are taken into account!
# (Time of day would also be taken into account if there was any.)
calendar_count_between(x, today, "year")
#> [1] 25 25 24
```

### High level API

You can use
[`date_count_between()`](https://clock.r-lib.org/reference/date_count_between.md)
with Date and POSIXct types.

``` r
x <- date_build(1980, 12, 14:16)
today <- date_build(2005, 12, 15)

date_count_between(x, today, "year")
#> [1] 25 25 24
```

## Computing number of weeks since the start of the year

[`lubridate::week()`](https://lubridate.tidyverse.org/reference/week.html)
is a useful function that returns “the number of complete seven day
periods that have occurred between the date and January 1st, plus one.”

There is no direct equivalent to this, but it is possible to replicate
with
[`calendar_start()`](https://clock.r-lib.org/reference/calendar-boundary.md)
and
[`time_point_count_between()`](https://clock.r-lib.org/reference/time_point_count_between.md).

``` r
x <- year_month_day(2019, 11, 28)

# lubridate::week(as.Date(x))
# [1] 48

x_start <- calendar_start(x, "year")
x_start
#> <year_month_day<day>[1]>
#> [1] "2019-01-01"

time_point_count_between(
  as_naive_time(x_start),
  as_naive_time(x),
  "week"
) + 1L
#> [1] 48
```

You could also peek at the
[`lubridate::week()`](https://lubridate.tidyverse.org/reference/week.html)
implementation to see that this is just:

``` r
doy <- get_day(as_year_day(x))
doy
#> [1] 332

(doy - 1L) %/% 7L + 1L
#> [1] 48
```

### High level API

This is actually a little easier in the high level API because you don’t
have to think about switching between types.

``` r
x <- date_build(2019, 11, 28)

date_count_between(date_start(x, "year"), x, "week") + 1L
#> [1] 48
```

## Compute the number of months between two dates

How can we compute the number of months between these two dates?

``` r
x <- year_month_day(2013, 10, 15)
y <- year_month_day(2016, 10, 13)
```

This is a bit of an ambiguous question because “month” isn’t very
well-defined, and there are various different interpretations we could
take.

We might want to ignore the day component entirely, and just compute the
number of months between `2013-10` and `2016-10`.

``` r
calendar_narrow(y, "month") - calendar_narrow(x, "month")
#> <duration<month>[1]>
#> [1] 36
```

Or we could include the day of the month, and say that `2013-10-15` to
`2014-10-15` defines 1 month (i.e. you have to hit the same day of the
month in the next month).

``` r
calendar_count_between(x, y, "month")
#> [1] 35
```

With this you could also compute the number of days remaining between
these two dates.

``` r
x_close <- add_months(x, calendar_count_between(x, y, "month"))
x_close
#> <year_month_day<day>[1]>
#> [1] "2016-09-15"

x_close_st <- as_sys_time(x_close)
y_st <- as_sys_time(y)

time_point_count_between(x_close_st, y_st, "day")
#> [1] 28
```

Or we could compute the number of days between these two dates in units
of seconds, and divide that by the average number of seconds in 1
proleptic Gregorian month.

``` r
# Days between x and y
days <- as_sys_time(y) - as_sys_time(x)
days
#> <duration<day>[1]>
#> [1] 1094

# In units of seconds
days <- duration_cast(days, "second")
days <- as.numeric(days)
days
#> [1] 94521600

# Average number of seconds in 1 proleptic Gregorian month
avg_sec_in_month <- duration_cast(duration_months(1), "second")
avg_sec_in_month <- as.numeric(avg_sec_in_month)

days / avg_sec_in_month
#> [1] 35.94324
```

### High level API

``` r
x <- date_build(2013, 10, 15)
y <- date_build(2016, 10, 13)
```

To ignore the day of the month, first shift to the start of the month,
then you can use
[`date_count_between()`](https://clock.r-lib.org/reference/date_count_between.md).

``` r
date_count_between(date_start(x, "month"), date_start(y, "month"), "month")
#> [1] 36
```

To utilize the day field, do the same as above but without calling
[`date_start()`](https://clock.r-lib.org/reference/date-and-date-time-boundary.md).

``` r
date_count_between(x, y, "month")
#> [1] 35
```

There is no high level equivalent to the average length of one proleptic
Gregorian month example.

## Computing the ISO year or week

The ISO 8601 standard outlines an alternative calendar that is specified
by the year, the week of the year, and the day of the week. It also
specifies that the *start* of the week is considered to be a Monday.
This ends up meaning that the actual ISO year may be different from the
Gregorian year, and is somewhat difficult to compute “by hand”. Instead,
you can use the
[`year_week_day()`](https://clock.r-lib.org/reference/year_week_day.md)
calendar if you need to work with ISO week dates.

``` r
x <- date_build(2019:2026)
y <- as_year_week_day(x, start = clock_weekdays$monday)

data.frame(x = x, y = y)
#>            x          y
#> 1 2019-01-01 2019-W01-2
#> 2 2020-01-01 2020-W01-3
#> 3 2021-01-01 2020-W53-5
#> 4 2022-01-01 2021-W52-6
#> 5 2023-01-01 2022-W52-7
#> 6 2024-01-01 2024-W01-1
#> 7 2025-01-01 2025-W01-3
#> 8 2026-01-01 2026-W01-4
```

``` r
get_year(y)
#> [1] 2019 2020 2020 2021 2022 2024 2025 2026
get_week(y)
#> [1]  1  1 53 52 52  1  1  1

# Last week in the ISO year
set_week(y, "last")
#> <year_week_day<Monday><day>[8]>
#> [1] "2019-W52-2" "2020-W53-3" "2020-W53-5" "2021-W52-6" "2022-W52-7"
#> [6] "2024-W52-1" "2025-W52-3" "2026-W53-4"
```

The year-week-day calendar is a fully supported calendar, meaning that
all of the `calendar_*()` functions work on it:

``` r
calendar_narrow(y, "week")
#> <year_week_day<Monday><week>[8]>
#> [1] "2019-W01" "2020-W01" "2020-W53" "2021-W52" "2022-W52" "2024-W01"
#> [7] "2025-W01" "2026-W01"
```

There is also an
[`iso_year_week_day()`](https://clock.r-lib.org/reference/iso_year_week_day.md)
calendar available, which is identical to
`year_week_day(start = clock_weekdays$monday)`. That ISO calendar
actually existed first, before we generalized it to any `start` weekday.

## Computing the Epidemiological year or week

Epidemiologists following the US CDC guidelines use a calendar that is
similar to the ISO calendar, but defines the start of the week to be
Sunday instead of Monday.
[`year_week_day()`](https://clock.r-lib.org/reference/year_week_day.md)
supports this as well:

``` r
x <- date_build(2019:2026)
iso <- as_year_week_day(x, start = clock_weekdays$monday)
epi <- as_year_week_day(x, start = clock_weekdays$sunday)

data.frame(x = x, iso = iso, epi = epi)
#>            x        iso        epi
#> 1 2019-01-01 2019-W01-2 2019-W01-3
#> 2 2020-01-01 2020-W01-3 2020-W01-4
#> 3 2021-01-01 2020-W53-5 2020-W53-6
#> 4 2022-01-01 2021-W52-6 2021-W52-7
#> 5 2023-01-01 2022-W52-7 2023-W01-1
#> 6 2024-01-01 2024-W01-1 2024-W01-2
#> 7 2025-01-01 2025-W01-3 2025-W01-4
#> 8 2026-01-01 2026-W01-4 2025-W53-5
```

``` r
get_year(epi)
#> [1] 2019 2020 2020 2021 2023 2024 2025 2025
get_week(epi)
#> [1]  1  1 53 52  1  1  1 53
```

## Converting a time zone abbreviation into a time zone name

It is possible that you might run into date-time strings of the form
`"2020-10-25 01:30:00 IST"`, which contain a time zone *abbreviation*
rather than a full time zone name. Because time zone maintainers change
the abbreviation they use throughout time, and because multiple time
zones sometimes use the same abbreviation, it is generally impossible to
parse strings of this form without more information. That said, if you
know what time zone this abbreviation goes with, you can parse this time
with
[`zoned_time_parse_abbrev()`](https://clock.r-lib.org/reference/zoned-parsing.md),
supplying the `zone`.

``` r
x <- "2020-10-25 01:30:00 IST"

zoned_time_parse_abbrev(x, "Asia/Kolkata")
#> <zoned_time<second><Asia/Kolkata>[1]>
#> [1] "2020-10-25T01:30:00+05:30"
zoned_time_parse_abbrev(x, "Asia/Jerusalem")
#> <zoned_time<second><Asia/Jerusalem>[1]>
#> [1] "2020-10-25T01:30:00+02:00"
```

If you *don’t* know what time zone this abbreviation goes with, then
generally you are out of luck. However, there are low-level tools in
this library that can help you generate a list of *possible* zoned-times
this could map to.

Assuming that `x` is a naive-time with its corresponding time zone
abbreviation attached, the first thing to do is to parse this string as
a naive-time.

``` r
x <- naive_time_parse(x, format = "%Y-%m-%d %H:%M:%S IST")
x
#> <naive_time<second>[1]>
#> [1] "2020-10-25T01:30:00"
```

Next, we’ll develop a function that attempts to turn this naive-time
into a zoned-time, iterating through all of the time zone names
available in the time zone database. These time zone names are
accessible through
[`tzdb_names()`](https://tzdb.r-lib.org/reference/tzdb_names.html). By
using the low-level
[`naive_time_info()`](https://clock.r-lib.org/reference/naive_time_info.md),
rather than
[`as_zoned_time()`](https://clock.r-lib.org/reference/as_zoned_time.md),
to lookup zone specific information, we’ll also get back information
about the UTC offset and time zone abbreviation that is currently in
use. By matching this abbreviation against our input abbreviation, we
can generate a list of zoned-times that use the abbreviation we care
about at that particular instance in time.

``` r
naive_find_by_abbrev <- function(x, abbrev) {
  if (!is_naive_time(x)) {
    abort("`x` must be a naive-time.")
  }
  if (length(x) != 1L) {
    abort("`x` must be length 1.")
  }
  if (!rlang::is_string(abbrev)) {
    abort("`abbrev` must be a single string.")
  }
  
  zones <- tzdb_names()
  info <- naive_time_info(x, zones)
  info$zones <- zones
  
  c(
    compute_uniques(x, info, abbrev),
    compute_ambiguous(x, info, abbrev)
  )
}

compute_uniques <- function(x, info, abbrev) {
  info <- info[info$type == "unique",]
  
  # If the abbreviation of the unique time matches the input `abbrev`,
  # then that candidate zone should be in the output
  matches <- info$first$abbreviation == abbrev
  zones <- info$zones[matches]
  
  lapply(zones, as_zoned_time, x = x)
}

compute_ambiguous <- function(x, info, abbrev) {
  info <- info[info$type == "ambiguous",]

  # Of the two possible times,
  # does the abbreviation of the earliest match the input `abbrev`?
  matches <- info$first$abbreviation == abbrev
  zones <- info$zones[matches]
  
  earliest <- lapply(zones, as_zoned_time, x = x, ambiguous = "earliest")
  
  # Of the two possible times,
  # does the abbreviation of the latest match the input `abbrev`?
  matches <- info$second$abbreviation == abbrev
  zones <- info$zones[matches]
  
  latest <- lapply(zones, as_zoned_time, x = x, ambiguous = "latest")
  
  c(earliest, latest)
}
```

``` r
candidates <- naive_find_by_abbrev(x, "IST")
candidates
#> [[1]]
#> <zoned_time<second><Asia/Calcutta>[1]>
#> [1] "2020-10-25T01:30:00+05:30"
#> 
#> [[2]]
#> <zoned_time<second><Asia/Kolkata>[1]>
#> [1] "2020-10-25T01:30:00+05:30"
#> 
#> [[3]]
#> <zoned_time<second><Eire>[1]>
#> [1] "2020-10-25T01:30:00+01:00"
#> 
#> [[4]]
#> <zoned_time<second><Europe/Dublin>[1]>
#> [1] "2020-10-25T01:30:00+01:00"
#> 
#> [[5]]
#> <zoned_time<second><Asia/Jerusalem>[1]>
#> [1] "2020-10-25T01:30:00+02:00"
#> 
#> [[6]]
#> <zoned_time<second><Asia/Tel_Aviv>[1]>
#> [1] "2020-10-25T01:30:00+02:00"
#> 
#> [[7]]
#> <zoned_time<second><Israel>[1]>
#> [1] "2020-10-25T01:30:00+02:00"
```

While it looks like we got 7 candidates, in reality we only have 3.
Asia/Kolkata, Europe/Dublin, and Asia/Jerusalem are our 3 candidates.
The others are aliases of those 3 that have been retired but are kept
for backwards compatibility.

Looking at the code, there are two ways to add a candidate time zone
name to the list.

If there is a unique mapping from `{naive-time, zone}` to `sys-time`,
then we check if the abbreviation that goes with that unique mapping
matches our input abbreviation. If so, then we convert `x` to a
zoned-time with that time zone.

If there is an ambiguous mapping from `{naive-time, zone}` to
`sys-time`, which is due to a daylight saving fallback, then we check
the abbreviation of both the *earliest* and *latest* possible times. If
either matches, then we convert `x` to a zoned-time using that time zone
and the information about which of the two ambiguous times were used.

This example is particularly interesting, since each of the 3 candidates
came from a different path. The Asia/Kolkata one is unique, the
Europe/Dublin one is ambiguous but the earliest was chosen, and the
Asia/Jerusalem one is ambiguous but the latest was chosen:

``` r
as_zoned_time(x, "Asia/Kolkata")
#> <zoned_time<second><Asia/Kolkata>[1]>
#> [1] "2020-10-25T01:30:00+05:30"
as_zoned_time(x, "Europe/Dublin", ambiguous = "earliest")
#> <zoned_time<second><Europe/Dublin>[1]>
#> [1] "2020-10-25T01:30:00+01:00"
as_zoned_time(x, "Asia/Jerusalem", ambiguous = "latest")
#> <zoned_time<second><Asia/Jerusalem>[1]>
#> [1] "2020-10-25T01:30:00+02:00"
```

## When is the next daylight saving time event?

Given a particular zoned-time, when will it next be affected by daylight
saving time? For this, we can use a relatively low level helper,
[`zoned_time_info()`](https://clock.r-lib.org/reference/zoned_time_info.md).
It returns a data frame of information about the current daylight saving
time transition points, along with information about the offset, the
current time zone abbreviation, and whether or not daylight saving time
is currently active or not.

``` r
x <- zoned_time_parse_complete("2019-01-01T00:00:00-05:00[America/New_York]")

info <- zoned_time_info(x)

# Beginning of the current DST range
info$begin
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2018-11-04T01:00:00-05:00"

# Beginning of the next DST range
info$end
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-03-10T03:00:00-04:00"
```

So on 2018-11-04 at (the second) 1 o’clock hour, daylight saving time
was turned off. On 2019-03-10 at 3 o’clock, daylight saving time will be
considered on again. This is the next moment in time right after a
daylight saving time gap of 1 hour, which you can see by subtracting 1
second (in sys-time):

``` r
# Last moment in time in the current DST range
info$end %>%
  as_sys_time() %>%
  add_seconds(-1) %>%
  as_zoned_time(zoned_time_zone(x))
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-03-10T01:59:59-05:00"
```

### High level API

[`date_time_info()`](https://clock.r-lib.org/reference/date_time_info.md)
exists in the high level API to do a similar thing. It is basically the
same as
[`zoned_time_info()`](https://clock.r-lib.org/reference/zoned_time_info.md),
except the `begin` and `end` columns are returned as R POSIXct
date-times rather than zoned-times, and the `offset` column is returned
as an integer rather than as a clock duration (since we try not to
expose high level API users to low level types).

``` r
x <- date_time_parse("2019-01-01 00:00:00", zone = "America/New_York")

date_time_info(x)
#>                 begin                 end offset   dst abbreviation
#> 1 2018-11-04 01:00:00 2019-03-10 03:00:00 -18000 FALSE          EST
```
