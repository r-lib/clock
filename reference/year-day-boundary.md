# Boundaries: year-day

This is a year-day method for the
[`calendar_start()`](https://clock.r-lib.org/reference/calendar-boundary.md)
and
[`calendar_end()`](https://clock.r-lib.org/reference/calendar-boundary.md)
generics. They adjust components of a calendar to the start or end of a
specified `precision`.

## Usage

``` r
# S3 method for class 'clock_year_day'
calendar_start(x, precision)

# S3 method for class 'clock_year_day'
calendar_end(x, precision)
```

## Arguments

- x:

  `[clock_year_day]`

  A year-day vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

  - `"millisecond"`

  - `"microsecond"`

  - `"nanosecond"`

## Value

`x` at the same precision, but with some components altered to be at the
boundary value.

## Examples

``` r
# Day precision
x <- year_day(2019:2020, 5)
x
#> <year_day<day>[2]>
#> [1] "2019-005" "2020-005"

# Compute the last day of the year
calendar_end(x, "year")
#> <year_day<day>[2]>
#> [1] "2019-365" "2020-366"
```
