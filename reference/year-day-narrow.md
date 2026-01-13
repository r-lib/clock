# Narrow: year-day

This is a year-day method for the
[`calendar_narrow()`](https://clock.r-lib.org/reference/calendar_narrow.md)
generic. It narrows a year-day vector to the specified `precision`.

## Usage

``` r
# S3 method for class 'clock_year_day'
calendar_narrow(x, precision)
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

`x` narrowed to the supplied `precision`.

## Examples

``` r
# Hour precision
x <- year_day(2019, 3, 4)
x
#> <year_day<hour>[1]>
#> [1] "2019-003T04"

# Narrowed to day precision
calendar_narrow(x, "day")
#> <year_day<day>[1]>
#> [1] "2019-003"

# Or year precision
calendar_narrow(x, "year")
#> <year_day<year>[1]>
#> [1] "2019"

# Subsecond precision can be narrowed to second precision
milli <- calendar_widen(x, "millisecond")
micro <- calendar_widen(x, "microsecond")
milli
#> <year_day<millisecond>[1]>
#> [1] "2019-003T04:00:00.000"
micro
#> <year_day<microsecond>[1]>
#> [1] "2019-003T04:00:00.000000"

calendar_narrow(milli, "second")
#> <year_day<second>[1]>
#> [1] "2019-003T04:00:00"
calendar_narrow(micro, "second")
#> <year_day<second>[1]>
#> [1] "2019-003T04:00:00"

# But once you have "locked in" a subsecond precision, it can't be
# narrowed to another subsecond precision
try(calendar_narrow(micro, "millisecond"))
#> Error in calendar_narrow(micro, "millisecond") : 
#>   Can't narrow a subsecond precision `x` ("microsecond") to
#> another subsecond precision ("millisecond").
```
