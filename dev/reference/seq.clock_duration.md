# Sequences: duration

This is a duration method for the
[`seq()`](https://rdrr.io/r/base/seq.html) generic.

Using [`seq()`](https://rdrr.io/r/base/seq.html) on duration objects
always retains the type of `from`.

When calling [`seq()`](https://rdrr.io/r/base/seq.html), exactly two of
the following must be specified:

- `to`

- `by`

- Either `length.out` or `along.with`

## Usage

``` r
# S3 method for class 'clock_duration'
seq(from, to = NULL, by = NULL, length.out = NULL, along.with = NULL, ...)
```

## Arguments

- from:

  `[clock_duration(1)]`

  A duration to start the sequence from.

- to:

  `[clock_duration(1) / NULL]`

  A duration to stop the sequence at.

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

## Details

If `from > to` and `by > 0`, then the result will be length 0. This
matches the behavior of
[`rlang::seq2()`](https://rlang.r-lib.org/reference/seq2.html), and
results in nicer theoretical properties when compared with throwing an
error. Similarly, if `from < to` and `by < 0`, then the result will also
be length 0.

## Examples

``` r
seq(duration_days(0), duration_days(100), by = 5)
#> <duration<day>[21]>
#>  [1] 0   5   10  15  20  25  30  35  40  45  50  55  60  65  70  75 
#> [17] 80  85  90  95  100

# Using a duration `by`. Note that `by` is cast to the type of `from`.
seq(duration_days(0), duration_days(100), by = duration_weeks(1))
#> <duration<day>[15]>
#>  [1] 0  7  14 21 28 35 42 49 56 63 70 77 84 91 98

# `to` is cast from 5 years to 60 months
# `by` is cast from 1 quarter to 4 months
seq(duration_months(0), duration_years(5), by = duration_quarters(1))
#> <duration<month>[21]>
#>  [1] 0  3  6  9  12 15 18 21 24 27 30 33 36 39 42 45 48 51 54 57 60

seq(duration_days(20), by = 2, length.out = 5)
#> <duration<day>[5]>
#> [1] 20 22 24 26 28
```
