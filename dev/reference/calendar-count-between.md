# Counting: calendars

`calendar_count_between()` counts the number of `precision` units
between `start` and `end` (i.e., the number of years or months). This
count corresponds to the *whole number* of units, and will never return
a fractional value.

This is suitable for, say, computing the whole number of years or months
between two calendar dates, accounting for the day and time of day.

Each calendar has its own help page describing the precisions that you
can count at:

- [year-month-day](https://clock.r-lib.org/dev/reference/year-month-day-count-between.md)

- [year-month-weekday](https://clock.r-lib.org/dev/reference/year-month-weekday-count-between.md)

- [year-week-day](https://clock.r-lib.org/dev/reference/year-week-day-count-between.md)

- [iso-year-week-day](https://clock.r-lib.org/dev/reference/iso-year-week-day-count-between.md)

- [year-quarter-day](https://clock.r-lib.org/dev/reference/year-quarter-day-count-between.md)

- [year-day](https://clock.r-lib.org/dev/reference/year-day-count-between.md)

## Usage

``` r
calendar_count_between(start, end, precision, ..., n = 1L)
```

## Arguments

- start, end:

  `[clock_calendar]`

  A pair of calendar vectors. These will be recycled to their common
  size.

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
# Number of whole years between these dates
x <- year_month_day(2000, 01, 05)
y <- year_month_day(2005, 01, 04:06)

# Note that `2000-01-05 -> 2005-01-04` is only 4 full years
calendar_count_between(x, y, "year")
#> [1] 4 5 5
```
