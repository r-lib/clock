# Boundaries: year-month-day

This is a year-month-day method for the
[`calendar_start()`](https://clock.r-lib.org/dev/reference/calendar-boundary.md)
and
[`calendar_end()`](https://clock.r-lib.org/dev/reference/calendar-boundary.md)
generics. They adjust components of a calendar to the start or end of a
specified `precision`.

## Usage

``` r
# S3 method for class 'clock_year_month_day'
calendar_start(x, precision)

# S3 method for class 'clock_year_month_day'
calendar_end(x, precision)
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

`x` at the same precision, but with some components altered to be at the
boundary value.

## Examples

``` r
# Hour precision
x <- year_month_day(2019, 2:4, 5, 6)
x
#> <year_month_day<hour>[3]>
#> [1] "2019-02-05T06" "2019-03-05T06" "2019-04-05T06"

# Compute the start of the month
calendar_start(x, "month")
#> <year_month_day<hour>[3]>
#> [1] "2019-02-01T00" "2019-03-01T00" "2019-04-01T00"

# Or the end of the month, notice that the hour value is adjusted as well
calendar_end(x, "month")
#> <year_month_day<hour>[3]>
#> [1] "2019-02-28T23" "2019-03-31T23" "2019-04-30T23"

# Compare that with just setting the day of the month to `"last"`, which
# doesn't adjust any other components
set_day(x, "last")
#> <year_month_day<hour>[3]>
#> [1] "2019-02-28T06" "2019-03-31T06" "2019-04-30T06"

# You can't compute the start / end at a more precise precision than
# the input is at
try(calendar_start(x, "second"))
#> Error in calendar_start_end_checks(x, x_precision, precision, "start") : 
#>   Can't compute the start of `x` ("hour") at a more precise
#> precision ("second").
```
