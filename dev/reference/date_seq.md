# Sequences: date and date-time

`date_seq()` generates a date (Date) or date-time (POSIXct/POSIXlt)
sequence.

There are separate help pages for generating sequences for dates and
date-times:

- [dates (Date)](https://clock.r-lib.org/dev/reference/date-sequence.md)

- [date-times
  (POSIXct/POSIXlt)](https://clock.r-lib.org/dev/reference/posixt-sequence.md)

## Usage

``` r
date_seq(from, ..., to = NULL, by = NULL, total_size = NULL)
```

## Arguments

- from:

  `[Date(1) / POSIXct(1) / POSIXlt(1)]`

  A date or date-time to start the sequence from.

- ...:

  These dots are for future extensions and must be empty.

- to:

  `[Date(1) / POSIXct(1) / POSIXlt(1) / NULL]`

  A date or date-time to stop the sequence at.

  `to` is only included in the result if the resulting sequence divides
  the distance between `from` and `to` exactly.

- by:

  `[integer(1) / clock_duration(1) / NULL]`

  The unit to increment the sequence by.

- total_size:

  `[positive integer(1) / NULL]`

  The size of the resulting sequence.

  If specified alongside `to`, this must generate a non-fractional
  sequence between `from` and `to`.

## Value

A date or date-time vector.

## Examples

``` r
# See method specific documentation for more examples

x <- as.Date("2019-01-01")
date_seq(x, by = duration_months(2), total_size = 20)
#>  [1] "2019-01-01" "2019-03-01" "2019-05-01" "2019-07-01" "2019-09-01"
#>  [6] "2019-11-01" "2020-01-01" "2020-03-01" "2020-05-01" "2020-07-01"
#> [11] "2020-09-01" "2020-11-01" "2021-01-01" "2021-03-01" "2021-05-01"
#> [16] "2021-07-01" "2021-09-01" "2021-11-01" "2022-01-01" "2022-03-01"
```
