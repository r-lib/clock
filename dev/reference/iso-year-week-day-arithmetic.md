# Arithmetic: iso-year-week-day

These are iso-year-week-day methods for the [arithmetic
generics](https://clock.r-lib.org/dev/reference/clock-arithmetic.md).

- [`add_years()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)

You cannot add weeks or days to an iso-year-week-day calendar. Adding
days is much more efficiently done by converting to a time point first
by using
[`as_naive_time()`](https://clock.r-lib.org/dev/reference/as_naive_time.md)
or
[`as_sys_time()`](https://clock.r-lib.org/dev/reference/as_sys_time.md).
Adding weeks is equally as efficient as adding 7 days. Additionally,
adding weeks to an invalid iso-year-week object containing
`iso_year_week_day(2019, 53)` would be undefined, as the 53rd ISO week
of 2019 doesn't exist to begin with.

## Usage

``` r
# S3 method for class 'clock_iso_year_week_day'
add_years(x, n, ...)
```

## Arguments

- x:

  `[clock_iso_year_week_day]`

  A iso-year-week-day vector.

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
x <- iso_year_week_day(2019, 1, 1)
add_years(x, 1:2)
#> <iso_year_week_day<day>[2]>
#> [1] "2020-W01-1" "2021-W01-1"
```
