# Arithmetic: weekday

These are weekday methods for the [arithmetic
generics](https://clock.r-lib.org/dev/reference/clock-arithmetic.md).

- [`add_days()`](https://clock.r-lib.org/dev/reference/clock-arithmetic.md)

Also check out the examples on the
[`weekday()`](https://clock.r-lib.org/dev/reference/weekday.md) page for
more advanced usage.

## Usage

``` r
# S3 method for class 'clock_weekday'
add_days(x, n, ...)
```

## Arguments

- x:

  `[clock_weekday]`

  A weekday vector.

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
saturday <- weekday(clock_weekdays$saturday)
saturday
#> <weekday[1]>
#> [1] Sat

add_days(saturday, 1)
#> <weekday[1]>
#> [1] Sun
add_days(saturday, 2)
#> <weekday[1]>
#> [1] Mon
```
