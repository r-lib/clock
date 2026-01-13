# Arithmetic: year-day

These are year-day methods for the [arithmetic
generics](https://clock.r-lib.org/dev/reference/clock-arithmetic.md).

- [`add_years()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)

Notably, *you cannot add days to a year-day*. For day-based arithmetic,
first convert to a time point with
[`as_naive_time()`](https://clock.r-lib.org/dev/reference/as_naive_time.md)
or
[`as_sys_time()`](https://clock.r-lib.org/dev/reference/as_sys_time.md).

## Usage

``` r
# S3 method for class 'clock_year_day'
add_years(x, n, ...)
```

## Arguments

- x:

  `[clock_year_day]`

  A year-day vector.

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
x <- year_day(2019, 10)

add_years(x, 1:5)
#> <year_day<day>[5]>
#> [1] "2020-010" "2021-010" "2022-010" "2023-010" "2024-010"

# A valid day in a leap year
y <- year_day(2020, 366)
y
#> <year_day<day>[1]>
#> [1] "2020-366"

# Adding 1 year to `y` generates an invalid date
y_plus <- add_years(y, 1)
y_plus
#> <year_day<day>[1]>
#> [1] "2021-366"

# Invalid dates are fine, as long as they are eventually resolved
# by either manually resolving, or by calling `invalid_resolve()`

# Resolve by returning the previous / next valid moment in time
invalid_resolve(y_plus, invalid = "previous")
#> <year_day<day>[1]>
#> [1] "2021-365"
invalid_resolve(y_plus, invalid = "next")
#> <year_day<day>[1]>
#> [1] "2022-001"

# Manually resolve by setting to the last day of the year
invalid <- invalid_detect(y_plus)
y_plus[invalid] <- set_day(y_plus[invalid], "last")
y_plus
#> <year_day<day>[1]>
#> [1] "2021-365"
```
