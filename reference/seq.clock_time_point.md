# Sequences: time points

This is a time point method for the
[`seq()`](https://rdrr.io/r/base/seq.html) generic. It works for
sys-time and naive-time vectors.

Sequences can be generated for all valid time point precisions (daily
through nanosecond).

When calling [`seq()`](https://rdrr.io/r/base/seq.html), exactly two of
the following must be specified:

- `to`

- `by`

- Either `length.out` or `along.with`

## Usage

``` r
# S3 method for class 'clock_time_point'
seq(from, to = NULL, by = NULL, length.out = NULL, along.with = NULL, ...)
```

## Arguments

- from:

  `[clock_sys_time(1) / clock_naive_time(1)]`

  A time point to start the sequence from.

  `from` is always included in the result.

- to:

  `[clock_sys_time(1) / clock_naive_time(1) / NULL]`

  A time point to stop the sequence at.

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
# Daily sequence
seq(
  as_naive_time(year_month_day(2019, 1, 1)),
  as_naive_time(year_month_day(2019, 2, 4)),
  by = 5
)
#> <naive_time<day>[7]>
#> [1] "2019-01-01" "2019-01-06" "2019-01-11" "2019-01-16" "2019-01-21"
#> [6] "2019-01-26" "2019-01-31"

# Minutely sequence using minute precision naive-time
x <- as_naive_time(year_month_day(2019, 1, 2, 3, 3))
x
#> <naive_time<minute>[1]>
#> [1] "2019-01-02T03:03"

seq(x, by = 4, length.out = 10)
#> <naive_time<minute>[10]>
#>  [1] "2019-01-02T03:03" "2019-01-02T03:07" "2019-01-02T03:11"
#>  [4] "2019-01-02T03:15" "2019-01-02T03:19" "2019-01-02T03:23"
#>  [7] "2019-01-02T03:27" "2019-01-02T03:31" "2019-01-02T03:35"
#> [10] "2019-01-02T03:39"

# You can use larger step sizes by using a duration-based `by`
seq(x, by = duration_days(1), length.out = 5)
#> <naive_time<minute>[5]>
#> [1] "2019-01-02T03:03" "2019-01-03T03:03" "2019-01-04T03:03"
#> [4] "2019-01-05T03:03" "2019-01-06T03:03"

# Nanosecond sequence
from <- as_naive_time(year_month_day(2019, 1, 1))
from <- time_point_cast(from, "nanosecond")
to <- from + 100
seq(from, to, by = 10)
#> <naive_time<nanosecond>[11]>
#>  [1] "2019-01-01T00:00:00.000000000" "2019-01-01T00:00:00.000000010"
#>  [3] "2019-01-01T00:00:00.000000020" "2019-01-01T00:00:00.000000030"
#>  [5] "2019-01-01T00:00:00.000000040" "2019-01-01T00:00:00.000000050"
#>  [7] "2019-01-01T00:00:00.000000060" "2019-01-01T00:00:00.000000070"
#>  [9] "2019-01-01T00:00:00.000000080" "2019-01-01T00:00:00.000000090"
#> [11] "2019-01-01T00:00:00.000000100"
```
