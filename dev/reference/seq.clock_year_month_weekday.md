# Sequences: year-month-weekday

This is a year-month-weekday method for the
[`seq()`](https://rdrr.io/r/base/seq.html) generic.

Sequences can only be generated for `"year"` and `"month"` precision
year-month-weekday vectors.

When calling [`seq()`](https://rdrr.io/r/base/seq.html), exactly two of
the following must be specified:

- `to`

- `by`

- Either `length.out` or `along.with`

## Usage

``` r
# S3 method for class 'clock_year_month_weekday'
seq(from, to = NULL, by = NULL, length.out = NULL, along.with = NULL, ...)
```

## Arguments

- from:

  `[clock_year_month_weekday(1)]`

  A `"year"` or `"month"` precision year-month-weekday to start the
  sequence from.

  `from` is always included in the result.

- to:

  `[clock_year_month_weekday(1) / NULL]`

  A `"year"` or `"month"` precision year-month-weekday to stop the
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
# Monthly sequence
x <- seq(year_month_weekday(2019, 1), year_month_weekday(2020, 12), by = 1)
x
#> <year_month_weekday<month>[24]>
#>  [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05" "2019-06"
#>  [7] "2019-07" "2019-08" "2019-09" "2019-10" "2019-11" "2019-12"
#> [13] "2020-01" "2020-02" "2020-03" "2020-04" "2020-05" "2020-06"
#> [19] "2020-07" "2020-08" "2020-09" "2020-10" "2020-11" "2020-12"

# Which we can then set the indexed weekday of
set_day(x, clock_weekdays$sunday, index = "last")
#> <year_month_weekday<day>[24]>
#>  [1] "2019-01-Sun[4]" "2019-02-Sun[4]" "2019-03-Sun[5]"
#>  [4] "2019-04-Sun[4]" "2019-05-Sun[4]" "2019-06-Sun[5]"
#>  [7] "2019-07-Sun[4]" "2019-08-Sun[4]" "2019-09-Sun[5]"
#> [10] "2019-10-Sun[4]" "2019-11-Sun[4]" "2019-12-Sun[5]"
#> [13] "2020-01-Sun[4]" "2020-02-Sun[4]" "2020-03-Sun[5]"
#> [16] "2020-04-Sun[4]" "2020-05-Sun[5]" "2020-06-Sun[4]"
#> [19] "2020-07-Sun[4]" "2020-08-Sun[5]" "2020-09-Sun[4]"
#> [22] "2020-10-Sun[4]" "2020-11-Sun[5]" "2020-12-Sun[4]"
```
