# Narrow: year-month-weekday

This is a year-month-weekday method for the
[`calendar_narrow()`](https://clock.r-lib.org/dev/reference/calendar_narrow.md)
generic. It narrows a year-month-weekday vector to the specified
`precision`.

## Usage

``` r
# S3 method for class 'clock_year_month_weekday'
calendar_narrow(x, precision)
```

## Arguments

- x:

  `[clock_year_month_weekday]`

  A year-month-weekday vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"month"`

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
x <- year_month_weekday(2019, 1, 1, 2)
x
#> <year_month_weekday<day>[1]>
#> [1] "2019-01-Sun[2]"

# Narrowed to month precision
calendar_narrow(x, "month")
#> <year_month_weekday<month>[1]>
#> [1] "2019-01"
```
