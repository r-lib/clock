# Narrow: year-month-day

This is a year-month-day method for the
[`calendar_narrow()`](https://clock.r-lib.org/dev/reference/calendar_narrow.md)
generic. It narrows a year-month-day vector to the specified
`precision`.

## Usage

``` r
# S3 method for class 'clock_year_month_day'
calendar_narrow(x, precision)
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

`x` narrowed to the supplied `precision`.

## Examples

``` r
# Hour precision
x <- year_month_day(2019, 1, 3, 4)
x
#> <year_month_day<hour>[1]>
#> [1] "2019-01-03T04"

# Narrowed to day precision
calendar_narrow(x, "day")
#> <year_month_day<day>[1]>
#> [1] "2019-01-03"

# Or month precision
calendar_narrow(x, "month")
#> <year_month_day<month>[1]>
#> [1] "2019-01"

# Subsecond precision can be narrowed to second precision
milli <- calendar_widen(x, "millisecond")
micro <- calendar_widen(x, "microsecond")
milli
#> <year_month_day<millisecond>[1]>
#> [1] "2019-01-03T04:00:00.000"
micro
#> <year_month_day<microsecond>[1]>
#> [1] "2019-01-03T04:00:00.000000"

calendar_narrow(milli, "second")
#> <year_month_day<second>[1]>
#> [1] "2019-01-03T04:00:00"
calendar_narrow(micro, "second")
#> <year_month_day<second>[1]>
#> [1] "2019-01-03T04:00:00"

# But once you have "locked in" a subsecond precision, it can't be
# narrowed to another subsecond precision
try(calendar_narrow(micro, "millisecond"))
#> Error in calendar_narrow(micro, "millisecond") : 
#>   Can't narrow a subsecond precision `x` ("microsecond") to
#> another subsecond precision ("millisecond").
```
