# Construct a duration

These helpers construct durations of the specified precision. Durations
represent units of time.

Durations are separated into two categories:

**Calendrical**

- year

- quarter

- month

**Chronological**

- week

- day

- hour

- minute

- second

- millisecond

- microsecond

- nanosecond

Calendrical durations are generally used when manipulating calendar
types, like year-month-day. Chronological durations are generally used
when working with time points, like sys-time or naive-time.

## Usage

``` r
duration_years(n = integer())

duration_quarters(n = integer())

duration_months(n = integer())

duration_weeks(n = integer())

duration_days(n = integer())

duration_hours(n = integer())

duration_minutes(n = integer())

duration_seconds(n = integer())

duration_milliseconds(n = integer())

duration_microseconds(n = integer())

duration_nanoseconds(n = integer())
```

## Arguments

- n:

  `[integer]`

  The number of units of time to use when creating the duration.

## Value

A duration of the specified precision.

## Internal Representation

Durations are internally represented as an integer number of "ticks"
along with a ratio describing how it converts to a number of seconds.
The following duration ratios are used in clock:

- `1 year == 31556952 seconds`

- `1 quarter == 7889238 seconds`

- `1 month == 2629746 seconds`

- `1 week == 604800 seconds`

- `1 day == 86400 seconds`

- `1 hour == 3600 seconds`

- `1 minute == 60 seconds`

- `1 second == 1 second`

- `1 millisecond == 1 / 1000 seconds`

- `1 microsecond == 1 / 1000000 seconds`

- `1 nanosecond == 1 / 1000000000 seconds`

A duration of 1 year is defined to correspond to the average length of a
proleptic Gregorian year, i.e. 365.2425 days.

A duration of 1 month is defined as exactly 1/12 of a year.

A duration of 1 quarter is defined as exactly 1/4 of a year.

A duration of 1 week is defined as exactly 7 days.

These conversions come into play when doing operations like adding or
flooring durations. Generally, you add two calendrical durations
together to get a new calendrical duration, rather than adding a
calendrical and a chronological duration together. The one exception is
[`duration_cast()`](https://clock.r-lib.org/dev/reference/duration_cast.md),
which can cast durations to any other precision, with a potential loss
of information.

## Examples

``` r
duration_years(1:5)
#> <duration<year>[5]>
#> [1] 1 2 3 4 5
duration_nanoseconds(1:5)
#> <duration<nanosecond>[5]>
#> [1] 1 2 3 4 5
```
