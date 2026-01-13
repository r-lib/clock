# Boundaries: year-quarter-day

This is a year-quarter-day method for the
[`calendar_start()`](https://clock.r-lib.org/dev/reference/calendar-boundary.md)
and
[`calendar_end()`](https://clock.r-lib.org/dev/reference/calendar-boundary.md)
generics. They adjust components of a calendar to the start or end of a
specified `precision`.

## Usage

``` r
# S3 method for class 'clock_year_quarter_day'
calendar_start(x, precision)

# S3 method for class 'clock_year_quarter_day'
calendar_end(x, precision)
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

`x` at the same precision, but with some components altered to be at the
boundary value.

## Examples

``` r
x <- year_quarter_day(2019:2020, 2:3, 5, 6, 7, 8, start = clock_months$march)
x
#> <year_quarter_day<March><second>[2]>
#> [1] "2019-Q2-05T06:07:08" "2020-Q3-05T06:07:08"

# Compute the last moment of the fiscal quarter
calendar_end(x, "quarter")
#> <year_quarter_day<March><second>[2]>
#> [1] "2019-Q2-92T23:59:59" "2020-Q3-91T23:59:59"

# Compare that to just setting the day to `"last"`,
# which doesn't affect the other components
set_day(x, "last")
#> <year_quarter_day<March><second>[2]>
#> [1] "2019-Q2-92T06:07:08" "2020-Q3-91T06:07:08"

# Compute the start of the fiscal year
calendar_start(x, "year")
#> <year_quarter_day<March><second>[2]>
#> [1] "2019-Q1-01T00:00:00" "2020-Q1-01T00:00:00"

as_date(calendar_start(x, "year"))
#> [1] "2018-03-01" "2019-03-01"
```
