# Sequences: iso-year-week-day

This is a iso-year-week-day method for the
[`seq()`](https://rdrr.io/r/base/seq.html) generic.

Sequences can only be generated for `"year"` precision iso-year-week-day
vectors. If you need to generate week-based sequences, you'll have to
convert to a time point first.

When calling [`seq()`](https://rdrr.io/r/base/seq.html), exactly two of
the following must be specified:

- `to`

- `by`

- Either `length.out` or `along.with`

## Usage

``` r
# S3 method for class 'clock_iso_year_week_day'
seq(from, to = NULL, by = NULL, length.out = NULL, along.with = NULL, ...)
```

## Arguments

- from:

  `[clock_iso_year_week_day(1)]`

  A `"year"` precision iso-year-week-day to start the sequence from.

  `from` is always included in the result.

- to:

  `[clock_iso_year_week_day(1) / NULL]`

  A `"year"` precision iso-year-week-day to stop the sequence at.

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
x <- seq(iso_year_week_day(2020), iso_year_week_day(2026), by = 2)
x
#> <iso_year_week_day<year>[4]>
#> [1] "2020" "2022" "2024" "2026"

# Which we can then set the week of.
# Some years have 53 ISO weeks, some have 52.
set_week(x, "last")
#> <iso_year_week_day<week>[4]>
#> [1] "2020-W53" "2022-W52" "2024-W52" "2026-W53"
```
