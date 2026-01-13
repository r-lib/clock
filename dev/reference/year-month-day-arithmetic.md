# Arithmetic: year-month-day

These are year-month-day methods for the [arithmetic
generics](https://clock.r-lib.org/dev/reference/clock-arithmetic.md).

- [`add_years()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)

- [`add_quarters()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)

- [`add_months()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)

Notably, *you cannot add days to a year-month-day*. For day-based
arithmetic, first convert to a time point with
[`as_naive_time()`](https://clock.r-lib.org/dev/reference/as_naive_time.md)
or
[`as_sys_time()`](https://clock.r-lib.org/dev/reference/as_sys_time.md).

## Usage

``` r
# S3 method for class 'clock_year_month_day'
add_years(x, n, ...)

# S3 method for class 'clock_year_month_day'
add_quarters(x, n, ...)

# S3 method for class 'clock_year_month_day'
add_months(x, n, ...)
```

## Arguments

- x:

  `[clock_year_month_day]`

  A year-month-day vector.

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
x <- year_month_day(2019, 1, 1)

add_years(x, 1:5)
#> <year_month_day<day>[5]>
#> [1] "2020-01-01" "2021-01-01" "2022-01-01" "2023-01-01" "2024-01-01"

y <- year_month_day(2019, 1, 31)

# Adding 1 month to `y` generates an invalid date
y_plus <- add_months(y, 1:2)
y_plus
#> <year_month_day<day>[2]>
#> [1] "2019-02-31" "2019-03-31"

# Invalid dates are fine, as long as they are eventually resolved
# by either manually resolving, or by calling `invalid_resolve()`

# Resolve by returning the previous / next valid moment in time
invalid_resolve(y_plus, invalid = "previous")
#> <year_month_day<day>[2]>
#> [1] "2019-02-28" "2019-03-31"
invalid_resolve(y_plus, invalid = "next")
#> <year_month_day<day>[2]>
#> [1] "2019-03-01" "2019-03-31"

# Manually resolve by setting to the last day of the month
invalid <- invalid_detect(y_plus)
y_plus[invalid] <- set_day(y_plus[invalid], "last")
y_plus
#> <year_month_day<day>[2]>
#> [1] "2019-02-28" "2019-03-31"
```
