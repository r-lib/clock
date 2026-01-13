# Convert to a zoned-time from a sys-time

This is a sys-time method for the
[`as_zoned_time()`](https://clock.r-lib.org/dev/reference/as_zoned_time.md)
generic.

Converting to a zoned-time from a sys-time retains the underlying
duration, but changes the printed time, depending on the `zone` that you
choose. Remember that sys-times are interpreted as UTC.

If you want to retain the printed time, try converting to a zoned-time
[from a
naive-time](https://clock.r-lib.org/dev/reference/as-zoned-time-naive-time.md),
which is a time point with a yet-to-be-determined time zone.

## Usage

``` r
# S3 method for class 'clock_sys_time'
as_zoned_time(x, zone, ...)
```

## Arguments

- x:

  `[clock_sys_time]`

  A sys-time to convert to a zoned-time.

- zone:

  `[character(1)]`

  The zone to convert to.

- ...:

  These dots are for future extensions and must be empty.

## Value

A zoned-time vector.

## Examples

``` r
x <- as_sys_time(year_month_day(2019, 02, 01, 02, 30, 00))
x
#> <sys_time<second>[1]>
#> [1] "2019-02-01T02:30:00"

# Since sys-time is interpreted as UTC, converting to a zoned-time with
# a zone of UTC retains the printed time
x_utc <- as_zoned_time(x, "UTC")
x_utc
#> <zoned_time<second><UTC>[1]>
#> [1] "2019-02-01T02:30:00+00:00"

# Converting to a different zone results in a different printed time,
# which corresponds to the exact same point in time, just in a different
# part of the work
x_ny <- as_zoned_time(x, "America/New_York")
x_ny
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-01-31T21:30:00-05:00"
```
