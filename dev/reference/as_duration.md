# Convert to a duration

You generally convert to a duration from either a sys-time or a
naive-time. The precision of the input is retained in the returned
duration.

To round an existing duration to another precision, see
[`duration_floor()`](https://clock.r-lib.org/dev/reference/duration-rounding.md).

## Usage

``` r
as_duration(x, ...)
```

## Arguments

- x:

  `[object]`

  An object to convert to a duration.

- ...:

  These dots are for future extensions and must be empty.

## Value

A duration with the same precision as `x`.

## Examples

``` r
x <- as_sys_time(year_month_day(2019, 01, 01))

# The number of days since 1970-01-01 UTC
as_duration(x)
#> <duration<day>[1]>
#> [1] 17897

x <- x + duration_seconds(1)
x
#> <sys_time<second>[1]>
#> [1] "2019-01-01T00:00:01"

# The number of seconds since 1970-01-01 00:00:00 UTC
as_duration(x)
#> <duration<second>[1]>
#> [1] 1546300801
```
