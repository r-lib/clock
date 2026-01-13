# Boundaries: year-week-day

This is an year-week-day method for the
[`calendar_start()`](https://clock.r-lib.org/reference/calendar-boundary.md)
and
[`calendar_end()`](https://clock.r-lib.org/reference/calendar-boundary.md)
generics. They adjust components of a calendar to the start or end of a
specified `precision`.

## Usage

``` r
# S3 method for class 'clock_year_week_day'
calendar_start(x, precision)

# S3 method for class 'clock_year_week_day'
calendar_end(x, precision)
```

## Arguments

- x:

  `[clock_year_week_day]`

  A year-week-day vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"week"`

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
x <- year_week_day(2019:2020, 5, 6, 10)
x
#> <year_week_day<Sunday><hour>[2]>
#> [1] "2019-W05-6T10" "2020-W05-6T10"

# Compute the last moment of the last week of the year
calendar_end(x, "year")
#> <year_week_day<Sunday><hour>[2]>
#> [1] "2019-W52-7T23" "2020-W53-7T23"

# Compare that to just setting the week to `"last"`,
# which doesn't affect the other components
set_week(x, "last")
#> <year_week_day<Sunday><hour>[2]>
#> [1] "2019-W52-6T10" "2020-W53-6T10"
```
