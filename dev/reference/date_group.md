# Group date and date-time components

`date_group()` groups by a single component of a date-time, such as
month of the year, or day of the month.

There are separate help pages for grouping dates and date-times:

- [dates (Date)](https://clock.r-lib.org/dev/reference/date-group.md)

- [date-times
  (POSIXct/POSIXlt)](https://clock.r-lib.org/dev/reference/posixt-group.md)

## Usage

``` r
date_group(x, precision, ..., n = 1L)
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

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

## Value

`x`, grouped at `precision`.

## Examples

``` r
# See type specific documentation for more examples
date_group(as.Date("2019-01-01") + 0:5, "day", n = 2)
#> [1] "2019-01-01" "2019-01-01" "2019-01-03" "2019-01-03" "2019-01-05"
#> [6] "2019-01-05"
```
