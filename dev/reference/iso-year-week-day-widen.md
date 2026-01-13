# Widen: iso-year-week-day

This is a iso-year-week-day method for the
[`calendar_widen()`](https://clock.r-lib.org/dev/reference/calendar_widen.md)
generic. It widens a iso-year-week-day vector to the specified
`precision`.

## Usage

``` r
# S3 method for class 'clock_iso_year_week_day'
calendar_widen(x, precision)
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

`x` widened to the supplied `precision`.

## Examples

``` r
# Week precision
x <- iso_year_week_day(2019, 1)
x
#> <iso_year_week_day<week>[1]>
#> [1] "2019-W01"

# Widen to day precision
# In the ISO calendar, the first day of the week is a Monday
calendar_widen(x, "day")
#> <iso_year_week_day<day>[1]>
#> [1] "2019-W01-1"

# Or second precision
sec <- calendar_widen(x, "second")
sec
#> <iso_year_week_day<second>[1]>
#> [1] "2019-W01-1T00:00:00"
```
