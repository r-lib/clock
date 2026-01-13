# Counting: iso-year-week-day

This is an iso-year-week-day method for the
[`calendar_count_between()`](https://clock.r-lib.org/dev/reference/calendar-count-between.md)
generic. It counts the number of `precision` units between `start` and
`end` (i.e., the number of ISO years).

## Usage

``` r
# S3 method for class 'clock_iso_year_week_day'
calendar_count_between(start, end, precision, ..., n = 1L)
```

## Arguments

- start, end:

  `[clock_iso_year_week_day]`

  A pair of iso-year-week-day vectors. These will be recycled to their
  common size.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

## Value

An integer representing the number of `precision` units between `start`
and `end`.

## Examples

``` r
# Compute the number of whole ISO years between two dates
x <- iso_year_week_day(2001, 1, 2)
y <- iso_year_week_day(2021, 1, c(1, 3))
calendar_count_between(x, y, "year")
#> [1] 19 20
```
