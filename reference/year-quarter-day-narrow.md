# Narrow: year-quarter-day

This is a year-quarter-day method for the
[`calendar_narrow()`](https://clock.r-lib.org/reference/calendar_narrow.md)
generic. It narrows a year-quarter-day vector to the specified
`precision`.

## Usage

``` r
# S3 method for class 'clock_year_quarter_day'
calendar_narrow(x, precision)
```

## Arguments

- x:

  `[clock_year_quarter_day]`

  A year-quarter-day vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"quarter"`

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
x <- year_quarter_day(2019, 1, 5)
x
#> <year_quarter_day<January><day>[1]>
#> [1] "2019-Q1-05"

# Narrow to quarter precision
calendar_narrow(x, "quarter")
#> <year_quarter_day<January><quarter>[1]>
#> [1] "2019-Q1"
```
