# Narrow: year-week-day

This is a year-week-day method for the
[`calendar_narrow()`](https://clock.r-lib.org/dev/reference/calendar_narrow.md)
generic. It narrows a year-week-day vector to the specified `precision`.

## Usage

``` r
# S3 method for class 'clock_year_week_day'
calendar_narrow(x, precision)
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

`x` narrowed to the supplied `precision`.

## Examples

``` r
# Day precision
x <- year_week_day(2019, 1, 5)
x
#> <year_week_day<Sunday><day>[1]>
#> [1] "2019-W01-5"

# Narrowed to week precision
calendar_narrow(x, "week")
#> <year_week_day<Sunday><week>[1]>
#> [1] "2019-W01"
```
