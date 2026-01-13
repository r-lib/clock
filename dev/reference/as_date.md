# Convert to a date

`as_date()` is a generic function that converts its input to a date
(Date).

There are methods for converting date-times (POSIXct), calendars, time
points, and zoned-times to dates.

For converting to a date-time, see
[`as_date_time()`](https://clock.r-lib.org/dev/reference/as_date_time.md).

## Usage

``` r
as_date(x, ...)

# S3 method for class 'Date'
as_date(x, ...)

# S3 method for class 'POSIXt'
as_date(x, ...)

# S3 method for class 'clock_calendar'
as_date(x, ...)

# S3 method for class 'clock_time_point'
as_date(x, ...)

# S3 method for class 'clock_zoned_time'
as_date(x, ...)
```

## Arguments

- x:

  `[vector]`

  A vector.

- ...:

  These dots are for future extensions and must be empty.

## Value

A date with the same length as `x`.

## Details

Note that clock always assumes that R's Date class is naive, so
converting a POSIXct to a Date will always retain the printed year,
month, and day value.

This is not a drop-in replacement for
[`as.Date()`](https://rdrr.io/r/base/as.Date.html), as it only converts
a limited set of types to Date. For parsing characters as dates, see
[`date_parse()`](https://clock.r-lib.org/dev/reference/date_parse.md).
For converting numerics to dates, see
[`vctrs::new_date()`](https://vctrs.r-lib.org/reference/new_date.html)
or continue to use [`as.Date()`](https://rdrr.io/r/base/as.Date.html).

## Examples

``` r
x <- date_time_parse("2019-01-01 23:02:03", "America/New_York")

# R's `as.Date.POSIXct()` method defaults to changing the printed time
# to UTC before converting, which can result in odd conversions like this:
as.Date(x)
#> [1] "2019-01-02"

# `as_date()` will never change the printed time before converting
as_date(x)
#> [1] "2019-01-01"

# Can also convert from other clock types
as_date(year_month_day(2019, 2, 5))
#> [1] "2019-02-05"
```
