# Convert to a zoned-time

`as_zoned_time()` converts `x` to a zoned-time. You generally convert to
a zoned time from either a sys-time or a naive time. Each are documented
on their own page:

- [sys-time](https://clock.r-lib.org/dev/reference/as-zoned-time-sys-time.md)

- [naive-time](https://clock.r-lib.org/dev/reference/as-zoned-time-naive-time.md)

There are also convenience methods for converting to a zoned time from
native R date and date-time types:

- [dates
  (Date)](https://clock.r-lib.org/dev/reference/as-zoned-time-Date.md)

- [date-times (POSIXct /
  POSIXlt)](https://clock.r-lib.org/dev/reference/as-zoned-time-posixt.md)

## Usage

``` r
as_zoned_time(x, ...)
```

## Arguments

- x:

  `[object]`

  An object to convert to a zoned-time.

- ...:

  These dots are for future extensions and must be empty.

## Value

A zoned-time vector.

## Examples

``` r
x <- as.Date("2019-01-01")
as_zoned_time(x, "Europe/London")
#> <zoned_time<second><Europe/London>[1]>
#> [1] "2019-01-01T00:00:00+00:00"

y <- as_naive_time(year_month_day(2019, 2, 1))
as_zoned_time(y, zone = "America/New_York")
#> <zoned_time<second><America/New_York>[1]>
#> [1] "2019-02-01T00:00:00-05:00"
```
