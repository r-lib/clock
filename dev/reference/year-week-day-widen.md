# Widen: year-week-day

This is a year-week-day method for the
[`calendar_widen()`](https://clock.r-lib.org/dev/reference/calendar_widen.md)
generic. It widens a year-week-day vector to the specified `precision`.

## Usage

``` r
# S3 method for class 'clock_year_week_day'
calendar_widen(x, precision)
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

`x` widened to the supplied `precision`.

## Examples

``` r
# Week precision
x <- year_week_day(2019, 1, start = clock_weekdays$monday)
x
#> <year_week_day<Monday><week>[1]>
#> [1] "2019-W01"

# Widen to day precision
# In this calendar, the first day of the week is a Monday
calendar_widen(x, "day")
#> <year_week_day<Monday><day>[1]>
#> [1] "2019-W01-1"

# Or second precision
sec <- calendar_widen(x, "second")
sec
#> <year_week_day<Monday><second>[1]>
#> [1] "2019-W01-1T00:00:00"
```
