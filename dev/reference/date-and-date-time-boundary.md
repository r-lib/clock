# Boundaries: date and date-time

- `date_start()` computes the date at the start of a particular
  `precision`, such as the "start of the year".

- `date_end()` computes the date at the end of a particular `precision`,
  such as the "end of the month".

There are separate help pages for computing boundaries for dates and
date-times:

- [dates (Date)](https://clock.r-lib.org/dev/reference/date-boundary.md)

- [date-times
  (POSIXct/POSIXlt)](https://clock.r-lib.org/dev/reference/posixt-boundary.md)

## Usage

``` r
date_start(x, precision, ...)

date_end(x, precision, ...)
```

## Arguments

- x:

  `[Date / POSIXct / POSIXlt]`

  A date or date-time vector.

- precision:

  `[character(1)]`

  A precision. Allowed precisions are dependent on the input used.

- ...:

  These dots are for future extensions and must be empty.

## Value

`x` but with some components altered to be at the boundary value.

## Examples

``` r
# See type specific documentation for more examples

x <- date_build(2019, 2:4)

date_end(x, "month")
#> [1] "2019-02-28" "2019-03-31" "2019-04-30"

x <- date_time_build(2019, 2:4, 3:5, 4, 5, zone = "America/New_York")

# Note that the hour, minute, and second components are also adjusted
date_end(x, "month")
#> [1] "2019-02-28 23:59:59 EST" "2019-03-31 23:59:59 EDT"
#> [3] "2019-04-30 23:59:59 EDT"
```
