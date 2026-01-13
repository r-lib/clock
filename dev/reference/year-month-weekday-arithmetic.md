# Arithmetic: year-month-weekday

These are year-month-weekday methods for the [arithmetic
generics](https://clock.r-lib.org/dev/reference/clock-arithmetic.md).

- [`add_years()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)

- [`add_quarters()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)

- [`add_months()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)

Notably, *you cannot add days to a year-month-weekday*. For day-based
arithmetic, first convert to a time point with
[`as_naive_time()`](https://clock.r-lib.org/dev/reference/as_naive_time.md)
or
[`as_sys_time()`](https://clock.r-lib.org/dev/reference/as_sys_time.md).

## Usage

``` r
# S3 method for class 'clock_year_month_weekday'
add_years(x, n, ...)

# S3 method for class 'clock_year_month_weekday'
add_quarters(x, n, ...)

# S3 method for class 'clock_year_month_weekday'
add_months(x, n, ...)
```

## Arguments

- x:

  `[clock_year_month_weekday]`

  A year-month-weekday vector.

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

Adding a single quarter with
[`add_quarters()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)
is equivalent to adding 3 months.

`x` and `n` are recycled against each other using [tidyverse recycling
rules](https://vctrs.r-lib.org/reference/theory-faq-recycling.html).

## Examples

``` r
# 2nd Friday in January, 2019
x <- year_month_weekday(2019, 1, clock_weekdays$friday, 2)
x
#> <year_month_weekday<day>[1]>
#> [1] "2019-01-Fri[2]"

add_months(x, 1:5)
#> <year_month_weekday<day>[5]>
#> [1] "2019-02-Fri[2]" "2019-03-Fri[2]" "2019-04-Fri[2]" "2019-05-Fri[2]"
#> [5] "2019-06-Fri[2]"

# These don't necessarily correspond to the same day of the month
as_year_month_day(add_months(x, 1:5))
#> <year_month_day<day>[5]>
#> [1] "2019-02-08" "2019-03-08" "2019-04-12" "2019-05-10" "2019-06-14"
```
