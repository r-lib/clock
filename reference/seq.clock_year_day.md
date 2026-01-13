# Sequences: year-day

This is a year-day method for the
[`seq()`](https://rdrr.io/r/base/seq.html) generic.

Sequences can only be generated for `"year"` precision year-day vectors.

When calling [`seq()`](https://rdrr.io/r/base/seq.html), exactly two of
the following must be specified:

- `to`

- `by`

- Either `length.out` or `along.with`

## Usage

``` r
# S3 method for class 'clock_year_day'
seq(from, to = NULL, by = NULL, length.out = NULL, along.with = NULL, ...)
```

## Arguments

- from:

  `[clock_year_day(1)]`

  A `"year"` precision year-day to start the sequence from.

  `from` is always included in the result.

- to:

  `[clock_year_day(1) / NULL]`

  A `"year"` precision year-day to stop the sequence at.

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
# Yearly sequence
x <- seq(year_day(2020), year_day(2040), by = 2)
x
#> <year_day<year>[11]>
#>  [1] "2020" "2022" "2024" "2026" "2028" "2030" "2032" "2034" "2036"
#> [10] "2038" "2040"

# Which we can then set the day of to get a sequence of end-of-year values
set_day(x, "last")
#> <year_day<day>[11]>
#>  [1] "2020-366" "2022-365" "2024-366" "2026-365" "2028-366" "2030-365"
#>  [7] "2032-366" "2034-365" "2036-366" "2038-365" "2040-366"

# Daily sequences are not allowed. Use a naive-time for this instead.
try(seq(year_day(2019, 1), by = 2, length.out = 2))
#> Error in seq(year_day(2019, 1), by = 2, length.out = 2) : 
#>   `from` must be 'year' precision.
as_year_day(seq(as_naive_time(year_day(2019, 1)), by = 2, length.out = 2))
#> <year_day<day>[2]>
#> [1] "2019-001" "2019-003"
```
