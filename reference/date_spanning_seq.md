# Spanning sequence: date and date-time

`date_spanning_seq()` generates a regular sequence along the span of
`x`, i.e. along `[min(x), max(x)]`. For dates, this generates a day
precision sequence, and for date-times it generates a second precision
sequence.

## Usage

``` r
date_spanning_seq(x)
```

## Arguments

- x:

  `[Date / POSIXct / POSIXlt]`

  A date or date-time vector.

## Value

A sequence along `[min(x), max(x)]`.

## Details

Missing and infinite values are automatically removed before the
sequence is generated.

For date-times, sys-time based sequences are generated, consistent with
[`date_seq()`](https://clock.r-lib.org/reference/posixt-sequence.md)
when using a second precision `by` value.

If you need more precise sequence generation, call
[`range()`](https://rdrr.io/r/base/range.html) and
[`date_seq()`](https://clock.r-lib.org/reference/date_seq.md) directly.

## Examples

``` r
x <- date_build(2020, c(1, 2, 1), c(10, 5, 12))
date_spanning_seq(x)
#>  [1] "2020-01-10" "2020-01-11" "2020-01-12" "2020-01-13" "2020-01-14"
#>  [6] "2020-01-15" "2020-01-16" "2020-01-17" "2020-01-18" "2020-01-19"
#> [11] "2020-01-20" "2020-01-21" "2020-01-22" "2020-01-23" "2020-01-24"
#> [16] "2020-01-25" "2020-01-26" "2020-01-27" "2020-01-28" "2020-01-29"
#> [21] "2020-01-30" "2020-01-31" "2020-02-01" "2020-02-02" "2020-02-03"
#> [26] "2020-02-04" "2020-02-05"

# Missing and infinite dates are removed before the sequence is generated
x <- c(x, NA, Inf, -Inf)
x
#> [1] "2020-01-10" "2020-02-05" "2020-01-12" NA           "Inf"       
#> [6] "-Inf"      

date_spanning_seq(x)
#>  [1] "2020-01-10" "2020-01-11" "2020-01-12" "2020-01-13" "2020-01-14"
#>  [6] "2020-01-15" "2020-01-16" "2020-01-17" "2020-01-18" "2020-01-19"
#> [11] "2020-01-20" "2020-01-21" "2020-01-22" "2020-01-23" "2020-01-24"
#> [16] "2020-01-25" "2020-01-26" "2020-01-27" "2020-01-28" "2020-01-29"
#> [21] "2020-01-30" "2020-01-31" "2020-02-01" "2020-02-02" "2020-02-03"
#> [26] "2020-02-04" "2020-02-05"

# For date-times, sequences are generated at second precision
x <- date_time_build(
  2020, 1, 2, 3, c(5, 4, 5), c(10, 48, 12),
  zone = "America/New_York"
)
x
#> [1] "2020-01-02 03:05:10 EST" "2020-01-02 03:04:48 EST"
#> [3] "2020-01-02 03:05:12 EST"

date_spanning_seq(x)
#>  [1] "2020-01-02 03:04:48 EST" "2020-01-02 03:04:49 EST"
#>  [3] "2020-01-02 03:04:50 EST" "2020-01-02 03:04:51 EST"
#>  [5] "2020-01-02 03:04:52 EST" "2020-01-02 03:04:53 EST"
#>  [7] "2020-01-02 03:04:54 EST" "2020-01-02 03:04:55 EST"
#>  [9] "2020-01-02 03:04:56 EST" "2020-01-02 03:04:57 EST"
#> [11] "2020-01-02 03:04:58 EST" "2020-01-02 03:04:59 EST"
#> [13] "2020-01-02 03:05:00 EST" "2020-01-02 03:05:01 EST"
#> [15] "2020-01-02 03:05:02 EST" "2020-01-02 03:05:03 EST"
#> [17] "2020-01-02 03:05:04 EST" "2020-01-02 03:05:05 EST"
#> [19] "2020-01-02 03:05:06 EST" "2020-01-02 03:05:07 EST"
#> [21] "2020-01-02 03:05:08 EST" "2020-01-02 03:05:09 EST"
#> [23] "2020-01-02 03:05:10 EST" "2020-01-02 03:05:11 EST"
#> [25] "2020-01-02 03:05:12 EST"
```
