# Arithmetic: year-quarter-day

These are year-quarter-day methods for the [arithmetic
generics](https://clock.r-lib.org/reference/clock-arithmetic.md).

- [`add_years()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

- [`add_quarters()`](https://clock.r-lib.org/reference/clock-arithmetic.md)

Notably, *you cannot add days to a year-quarter-day*. For day-based
arithmetic, first convert to a time point with
[`as_naive_time()`](https://clock.r-lib.org/reference/as_naive_time.md)
or [`as_sys_time()`](https://clock.r-lib.org/reference/as_sys_time.md).

## Usage

``` r
# S3 method for class 'clock_year_quarter_day'
add_years(x, n, ...)

# S3 method for class 'clock_year_quarter_day'
add_quarters(x, n, ...)
```

## Arguments

- x:

  `[clock_year_quarter_day]`

  A year-quarter-day vector.

- n:

  `[integer / clock_duration]`

  An integer vector to be converted to a duration, or a duration
  corresponding to the arithmetic function being used. This corresponds
  to the number of duration units to add. `n` may be negative to
  subtract units of duration.

- ...:

  These dots are for future extensions and must be empty.

## Value

`x` after performing the arithmetic.

## Details

`x` and `n` are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

## Examples

``` r
x <- year_quarter_day(2019, 1:3)
x
#> <year_quarter_day<January><quarter>[3]>
#> [1] "2019-Q1" "2019-Q2" "2019-Q3"

add_quarters(x, 2)
#> <year_quarter_day<January><quarter>[3]>
#> [1] "2019-Q3" "2019-Q4" "2020-Q1"

# Make the fiscal year start in March
y <- year_quarter_day(2019, 1:2, 1, start = 3)
y
#> <year_quarter_day<March><day>[2]>
#> [1] "2019-Q1-01" "2019-Q2-01"

add_quarters(y, 1)
#> <year_quarter_day<March><day>[2]>
#> [1] "2019-Q2-01" "2019-Q3-01"

# What year-month-day does this correspond to?
# Note that the fiscal year doesn't necessarily align with the Gregorian
# year!
as_year_month_day(add_quarters(y, 1))
#> <year_month_day<day>[2]>
#> [1] "2018-06-01" "2018-09-01"
```
