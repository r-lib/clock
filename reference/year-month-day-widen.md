# Widen: year-month-day

This is a year-month-day method for the
[`calendar_widen()`](https://clock.r-lib.org/reference/calendar_widen.md)
generic. It widens a year-month-day vector to the specified `precision`.

## Usage

``` r
# S3 method for class 'clock_year_month_day'
calendar_widen(x, precision)
```

## Arguments

- x:

  `[clock_year_month_day]`

  A year-month-day vector.

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

`x` widened to the supplied `precision`.

## Examples

``` r
# Month precision
x <- year_month_day(2019, 1)
x
#> <year_month_day<month>[1]>
#> [1] "2019-01"

# Widen to day precision
calendar_widen(x, "day")
#> <year_month_day<day>[1]>
#> [1] "2019-01-01"

# Or second precision
sec <- calendar_widen(x, "second")
sec
#> <year_month_day<second>[1]>
#> [1] "2019-01-01T00:00:00"

# Second precision can be widened to subsecond precision
milli <- calendar_widen(sec, "millisecond")
micro <- calendar_widen(sec, "microsecond")
milli
#> <year_month_day<millisecond>[1]>
#> [1] "2019-01-01T00:00:00.000"
micro
#> <year_month_day<microsecond>[1]>
#> [1] "2019-01-01T00:00:00.000000"

# But once you have "locked in" a subsecond precision, it can't
# be widened again
try(calendar_widen(milli, "microsecond"))
#> Error in calendar_widen(milli, "microsecond") : 
#>   Can't widen a subsecond precision `x` ("millisecond") to
#> another subsecond precision ("microsecond").
```
