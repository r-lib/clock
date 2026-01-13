# Counting: year-quarter-day

This is a year-quarter-day method for the
[`calendar_count_between()`](https://clock.r-lib.org/dev/reference/calendar-count-between.md)
generic. It counts the number of `precision` units between `start` and
`end` (i.e., the number of years or quarters).

## Usage

``` r
# S3 method for class 'clock_year_quarter_day'
calendar_count_between(start, end, precision, ..., n = 1L)
```

## Arguments

- start, end:

  `[clock_year_quarter_day]`

  A pair of year-quarter-day vectors. These will be recycled to their
  common size.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"quarter"`

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
# Compute the number of whole quarters between two dates
x <- year_quarter_day(2020, 3, 91)
y <- year_quarter_day(2025, 4, c(90, 92))
calendar_count_between(x, y, "quarter")
#> [1] 20 21

# Note that this is not always the same as the number of whole 3 month
# periods between two dates
x <- as_year_month_day(x)
y <- as_year_month_day(y)
calendar_count_between(x, y, "month", n = 3)
#> [1] 21 21
```
