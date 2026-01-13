# Formatting: date and date-time

`date_format()` formats a date (Date) or date-time (POSIXct/POSIXlt)
using a `format` string.

There are separate help pages for formatting dates and date-times:

- [dates (Date)](https://clock.r-lib.org/reference/date-formatting.md)

- [date-times
  (POSIXct/POSIXlt)](https://clock.r-lib.org/reference/posixt-formatting.md)

## Usage

``` r
date_format(x, ...)
```

## Arguments

- x:

  `[Date / POSIXct / POSIXlt]`

  A date or date-time vector.

- ...:

  These dots are for future extensions and must be empty.

## Value

A character vector of the formatted input.

## Examples

``` r
# See method specific documentation for more examples

x <- as.Date("2019-01-01")
date_format(x, format = "year: %Y, month: %m, day: %d")
#> [1] "year: 2019, month: 01, day: 01"
```
