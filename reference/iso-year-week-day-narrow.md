# Narrow: iso-year-week-day

This is a iso-year-week-day method for the
[`calendar_narrow()`](https://clock.r-lib.org/reference/calendar_narrow.md)
generic. It narrows a iso-year-week-day vector to the specified
`precision`.

## Usage

``` r
# S3 method for class 'clock_iso_year_week_day'
calendar_narrow(x, precision)
```

## Arguments

- x:

  `[clock_iso_year_week_day]`

  A iso-year-week-day vector.

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
x <- iso_year_week_day(2019, 1, 5)
x
#> <iso_year_week_day<day>[1]>
#> [1] "2019-W01-5"

# Narrowed to week precision
calendar_narrow(x, "week")
#> <iso_year_week_day<week>[1]>
#> [1] "2019-W01"
```
