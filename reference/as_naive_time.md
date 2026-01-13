# Convert to a naive-time

`as_naive_time()` converts `x` to a naive-time.

You can convert to a naive-time from any calendar type, as long as it
has at least day precision. There also must not be any invalid dates. If
invalid dates exist, they must first be resolved with
[`invalid_resolve()`](https://clock.r-lib.org/reference/clock-invalid.md).

Converting to a naive-time from a sys-time or zoned-time retains the
printed time, but drops the assumption that the time should be
interpreted with any specific time zone.

Converting to a naive-time from a duration just wraps the duration in a
naive-time object, there is no assumption about the time zone. The
duration must have at least day precision.

There are convenience methods for converting to a naive-time from R's
native date and date-time types. Like converting from a zoned-time,
these retain the printed time.

## Usage

``` r
as_naive_time(x, ...)
```

## Arguments

- x:

  `[object]`

  An object to convert to a naive-time.

- ...:

  These dots are for future extensions and must be empty.

## Value

A naive-time vector.

## Examples

``` r
x <- as.Date("2019-01-01")
as_naive_time(x)
#> <naive_time<day>[1]>
#> [1] "2019-01-01"

ym <- year_month_day(2019, 02)

# A minimum of day precision is required
try(as_naive_time(ym))
#> Error : Can't convert to a time point from a calendar with 'month' precision. A minimum of 'day' precision is required.

ymd <- set_day(ym, 10)
as_naive_time(ymd)
#> <naive_time<day>[1]>
#> [1] "2019-02-10"
```
