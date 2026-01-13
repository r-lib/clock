# Counting: date and date-time

`date_count_between()` counts the number of `precision` units between
`start` and `end` (i.e., the number of years or months or hours). This
count corresponds to the *whole number* of units, and will never return
a fractional value.

This is suitable for, say, computing the whole number of years or months
between two dates, accounting for the day and time of day.

There are separate help pages for counting for dates and date-times:

- [dates
  (Date)](https://clock.r-lib.org/dev/reference/date-count-between.md)

- [date-times
  (POSIXct/POSIXlt)](https://clock.r-lib.org/dev/reference/posixt-count-between.md)

## Usage

``` r
date_count_between(start, end, precision, ..., n = 1L)
```

## Arguments

- start, end:

  `[Date / POSIXct / POSIXlt]`

  A pair of date or date-time vectors. These will be recycled to their
  common size.

- precision:

  `[character(1)]`

  A precision. Allowed precisions are dependent on the calendar used.

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

## Value

An integer representing the number of `precision` units between `start`
and `end`.

## Comparison Direction

The computed count has the property that if `start <= end`, then
`start + <count> <= end`. Similarly, if `start >= end`, then
`start + <count> >= end`. In other words, the comparison direction
between `start` and `end` will never change after adding the count to
`start`. This makes this function useful for repeated count computations
at increasingly fine precisions.

## Examples

``` r
# See method specific documentation for more examples

start <- date_parse("2000-05-05")
end <- date_parse(c("2020-05-04", "2020-05-06"))

# Age in years
date_count_between(start, end, "year")
#> [1] 19 20

# Number of "whole" months between these dates
date_count_between(start, end, "month")
#> [1] 239 240
```
