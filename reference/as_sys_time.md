# Convert to a sys-time

`as_sys_time()` converts `x` to a sys-time.

You can convert to a sys-time from any calendar type, as long as it has
at least day precision. There also must not be any invalid dates. If
invalid dates exist, they must first be resolved with
[`invalid_resolve()`](https://clock.r-lib.org/reference/clock-invalid.md).

Converting to a sys-time from a naive-time retains the printed time, but
adds an assumption that the time should be interpreted in the UTC time
zone.

Converting to a sys-time from a zoned-time retains the underlying
duration, but the printed time is the equivalent UTC time to whatever
the zoned-time's zone happened to be.

Converting to a sys-time from a duration just wraps the duration in a
sys-time object, adding the assumption that the time should be
interpreted in the UTC time zone. The duration must have at least day
precision.

There are convenience methods for converting to a sys-time from R's
native date and date-time types. Like converting from a zoned-time,
these retain the underlying duration, but will change the printed time
if the zone was not already UTC.

## Usage

``` r
as_sys_time(x, ...)
```

## Arguments

- x:

  `[object]`

  An object to convert to a sys-time.

- ...:

  These dots are for future extensions and must be empty.

## Value

A sys-time vector.

## Examples

``` r
x <- as.Date("2019-01-01")

# Dates are assumed to be naive, so the printed time is the same whether
# we convert it to sys-time or naive-time
as_sys_time(x)
#> <sys_time<day>[1]>
#> [1] "2019-01-01"
as_naive_time(x)
#> <naive_time<day>[1]>
#> [1] "2019-01-01"

y <- as.POSIXct("2019-01-01 01:00:00", tz = "America/New_York")

# The sys time displays the equivalent time in UTC (5 hours ahead of
# America/New_York at this point in the year)
as_sys_time(y)
#> <sys_time<second>[1]>
#> [1] "2019-01-01T06:00:00"

ym <- year_month_day(2019, 02)

# A minimum of day precision is required
try(as_sys_time(ym))
#> Error : Can't convert to a time point from a calendar with 'month' precision. A minimum of 'day' precision is required.

ymd <- set_day(ym, 10)
as_sys_time(ymd)
#> <sys_time<day>[1]>
#> [1] "2019-02-10"
```
