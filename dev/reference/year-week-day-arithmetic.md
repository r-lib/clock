# Arithmetic: year-week-day

These are year-week-day methods for the [arithmetic
generics](https://clock.r-lib.org/dev/reference/clock-arithmetic.md).

- [`add_years()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)

You cannot add weeks or days to a year-week-day calendar. Adding days is
much more efficiently done by converting to a time point first by using
[`as_naive_time()`](https://clock.r-lib.org/dev/reference/as_naive_time.md)
or
[`as_sys_time()`](https://clock.r-lib.org/dev/reference/as_sys_time.md).
Adding weeks is equally as efficient as adding 7 days. Additionally,
adding weeks to an invalid year-week object (i.e. one set to the 53rd
week, when that doesn't exist) would be undefined.

## Usage

``` r
# S3 method for class 'clock_year_week_day'
add_years(x, n, ...)
```

## Arguments

- x:

  `[clock_year_week_day]`

  A year-week-day vector.

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
x <- year_week_day(2019, 1, 1)
add_years(x, 1:2)
#> <year_week_day<Sunday><day>[2]>
#> [1] "2020-W01-1" "2021-W01-1"
```
