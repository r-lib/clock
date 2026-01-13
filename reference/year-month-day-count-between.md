# Counting: year-month-day

This is a year-month-day method for the
[`calendar_count_between()`](https://clock.r-lib.org/reference/calendar-count-between.md)
generic. It counts the number of `precision` units between `start` and
`end` (i.e., the number of years or months).

## Usage

``` r
# S3 method for class 'clock_year_month_day'
calendar_count_between(start, end, precision, ..., n = 1L)
```

## Arguments

- start, end:

  `[clock_year_month_day]`

  A pair of year-month-day vectors. These will be recycled to their
  common size.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"quarter"`

  - `"month"`

- ...:

  These dots are for future extensions and must be empty.

- n:

  `[positive integer(1)]`

  A single positive integer specifying a multiple of `precision` to use.

## Value

An integer representing the number of `precision` units between `start`
and `end`.

## Details

`"quarter"` is equivalent to `"month"` precision with `n` set to
`n * 3L`.

## Examples

``` r
# Compute an individual's age in years
x <- year_month_day(2001, 2, 4)
today <- year_month_day(2021, 11, 30)
calendar_count_between(x, today, "year")
#> [1] 20

# Compute the number of months between two dates, taking
# into account the day of the month and time of day
x <- year_month_day(2000, 4, 2, 5)
y <- year_month_day(2000, 7, c(1, 2, 2), c(3, 4, 6))
calendar_count_between(x, y, "month")
#> [1] 2 2 3
```
