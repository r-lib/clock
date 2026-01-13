# Sequences: year-month-day

This is a year-month-day method for the
[`seq()`](https://rdrr.io/r/base/seq.html) generic.

Sequences can only be generated for `"year"` and `"month"` precision
year-month-day vectors.

When calling [`seq()`](https://rdrr.io/r/base/seq.html), exactly two of
the following must be specified:

- `to`

- `by`

- Either `length.out` or `along.with`

## Usage

``` r
# S3 method for class 'clock_year_month_day'
seq(from, to = NULL, by = NULL, length.out = NULL, along.with = NULL, ...)
```

## Arguments

- from:

  `[clock_year_month_day(1)]`

  A `"year"` or `"month"` precision year-month-day to start the sequence
  from.

  `from` is always included in the result.

- to:

  `[clock_year_month_day(1) / NULL]`

  A `"year"` or `"month"` precision year-month-day to stop the sequence
  at.

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
# Monthly sequence
x <- seq(year_month_day(2019, 1), year_month_day(2020, 12), by = 1)
x
#> <year_month_day<month>[24]>
#>  [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05" "2019-06"
#>  [7] "2019-07" "2019-08" "2019-09" "2019-10" "2019-11" "2019-12"
#> [13] "2020-01" "2020-02" "2020-03" "2020-04" "2020-05" "2020-06"
#> [19] "2020-07" "2020-08" "2020-09" "2020-10" "2020-11" "2020-12"

# Which we can then set the day of to get a sequence of end-of-month values
set_day(x, "last")
#> <year_month_day<day>[24]>
#>  [1] "2019-01-31" "2019-02-28" "2019-03-31" "2019-04-30" "2019-05-31"
#>  [6] "2019-06-30" "2019-07-31" "2019-08-31" "2019-09-30" "2019-10-31"
#> [11] "2019-11-30" "2019-12-31" "2020-01-31" "2020-02-29" "2020-03-31"
#> [16] "2020-04-30" "2020-05-31" "2020-06-30" "2020-07-31" "2020-08-31"
#> [21] "2020-09-30" "2020-10-31" "2020-11-30" "2020-12-31"

# Daily sequences are not allowed. Use a naive-time for this instead.
try(seq(year_month_day(2019, 1, 1), by = 2, length.out = 2))
#> Error in seq(year_month_day(2019, 1, 1), by = 2, length.out = 2) : 
#>   `from` must be 'year' or 'month' precision.
seq(as_naive_time(year_month_day(2019, 1, 1)), by = 2, length.out = 2)
#> <naive_time<day>[2]>
#> [1] "2019-01-01" "2019-01-03"
```
