# Widen: year-day

This is a year-day method for the
[`calendar_widen()`](https://clock.r-lib.org/reference/calendar_widen.md)
generic. It widens a year-day vector to the specified `precision`.

## Usage

``` r
# S3 method for class 'clock_year_day'
calendar_widen(x, precision)
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

`x` widened to the supplied `precision`.

## Examples

``` r
# Year precision
x <- year_day(2019)
x
#> <year_day<year>[1]>
#> [1] "2019"

# Widen to day precision
calendar_widen(x, "day")
#> <year_day<day>[1]>
#> [1] "2019-001"

# Or second precision
sec <- calendar_widen(x, "second")
sec
#> <year_day<second>[1]>
#> [1] "2019-001T00:00:00"

# Second precision can be widened to subsecond precision
milli <- calendar_widen(sec, "millisecond")
micro <- calendar_widen(sec, "microsecond")
milli
#> <year_day<millisecond>[1]>
#> [1] "2019-001T00:00:00.000"
micro
#> <year_day<microsecond>[1]>
#> [1] "2019-001T00:00:00.000000"

# But once you have "locked in" a subsecond precision, it can't
# be widened again
try(calendar_widen(milli, "microsecond"))
#> Error in calendar_widen(milli, "microsecond") : 
#>   Can't widen a subsecond precision `x` ("millisecond") to
#> another subsecond precision ("microsecond").
```
