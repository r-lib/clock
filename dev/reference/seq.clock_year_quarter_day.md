# Sequences: year-quarter-day

This is a year-quarter-day method for the
[`seq()`](https://rdrr.io/r/base/seq.html) generic.

Sequences can only be generated for `"year"` and `"quarter"` precision
year-quarter-day vectors.

When calling [`seq()`](https://rdrr.io/r/base/seq.html), exactly two of
the following must be specified:

- `to`

- `by`

- Either `length.out` or `along.with`

## Usage

``` r
# S3 method for class 'clock_year_quarter_day'
seq(from, to = NULL, by = NULL, length.out = NULL, along.with = NULL, ...)
```

## Arguments

- from:

  `[clock_year_quarter_day(1)]`

  A `"year"` or `"quarter"` precision year-quarter-day to start the
  sequence from.

  `from` is always included in the result.

- to:

  `[clock_year_quarter_day(1) / NULL]`

  A `"year"` or `"quarter"` precision year-quarter-day to stop the
  sequence at.

  `to` is cast to the type of `from`.

  `to` is only included in the result if the resulting sequence divides
  the distance between `from` and `to` exactly.

- by:

  `[integer(1) / clock_duration(1) / NULL]`

  The unit to increment the sequence by.

  If `by` is an integer, it is transformed into a duration with the
  precision of `from`.

  If `by` is a duration, it is cast to the type of `from`.

- length.out:

  `[positive integer(1) / NULL]`

  The length of the resulting sequence.

  If specified, `along.with` must be `NULL`.

- along.with:

  `[vector / NULL]`

  A vector who's length determines the length of the resulting sequence.

  Equivalent to `length.out = vec_size(along.with)`.

  If specified, `length.out` must be `NULL`.

- ...:

  These dots are for future extensions and must be empty.

## Value

A sequence with the type of `from`.

## Examples

``` r
# Quarterly sequence
x <- seq(year_quarter_day(2020, 1), year_quarter_day(2026, 3), by = 2)
x
#> <year_quarter_day<January><quarter>[14]>
#>  [1] "2020-Q1" "2020-Q3" "2021-Q1" "2021-Q3" "2022-Q1" "2022-Q3"
#>  [7] "2023-Q1" "2023-Q3" "2024-Q1" "2024-Q3" "2025-Q1" "2025-Q3"
#> [13] "2026-Q1" "2026-Q3"

# Which we can then set the day of the quarter of
set_day(x, "last")
#> <year_quarter_day<January><day>[14]>
#>  [1] "2020-Q1-91" "2020-Q3-92" "2021-Q1-90" "2021-Q3-92" "2022-Q1-90"
#>  [6] "2022-Q3-92" "2023-Q1-90" "2023-Q3-92" "2024-Q1-91" "2024-Q3-92"
#> [11] "2025-Q1-90" "2025-Q3-92" "2026-Q1-90" "2026-Q3-92"
```
