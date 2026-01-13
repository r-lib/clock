# Widen: year-quarter-day

This is a year-quarter-day method for the
[`calendar_widen()`](https://clock.r-lib.org/reference/calendar_widen.md)
generic. It widens a year-quarter-day vector to the specified
`precision`.

## Usage

``` r
# S3 method for class 'clock_year_quarter_day'
calendar_widen(x, precision)
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

`x` widened to the supplied `precision`.

## Examples

``` r
# Quarter precision
x <- year_quarter_day(2019, 1)
x
#> <year_quarter_day<January><quarter>[1]>
#> [1] "2019-Q1"

# Widen to day precision
calendar_widen(x, "day")
#> <year_quarter_day<January><day>[1]>
#> [1] "2019-Q1-01"

# Or second precision
sec <- calendar_widen(x, "second")
sec
#> <year_quarter_day<January><second>[1]>
#> [1] "2019-Q1-01T00:00:00"
```
