# Spanning sequence: time points

`time_point_spanning_seq()` generates a regular sequence along the span
of `x`, i.e. along `[min(x), max(x)]`. The sequence is generated at the
precision of `x`.

## Usage

``` r
time_point_spanning_seq(x)
```

## Arguments

- x:

  `[clock_sys_time / clock_naive_time]`

  A time point vector.

## Value

A sequence along `[min(x), max(x)]`.

## Details

Missing values are automatically removed before the sequence is
generated.

If you need more precise sequence generation, call
[`range()`](https://rdrr.io/r/base/range.html) and
[`seq()`](https://rdrr.io/r/base/seq.html) directly.

## Examples

``` r
x <- as_naive_time(year_month_day(2019, c(1, 2, 1, 2), c(15, 4, 12, 2)))
x
#> <naive_time<day>[4]>
#> [1] "2019-01-15" "2019-02-04" "2019-01-12" "2019-02-02"

time_point_spanning_seq(x)
#> <naive_time<day>[24]>
#>  [1] "2019-01-12" "2019-01-13" "2019-01-14" "2019-01-15" "2019-01-16"
#>  [6] "2019-01-17" "2019-01-18" "2019-01-19" "2019-01-20" "2019-01-21"
#> [11] "2019-01-22" "2019-01-23" "2019-01-24" "2019-01-25" "2019-01-26"
#> [16] "2019-01-27" "2019-01-28" "2019-01-29" "2019-01-30" "2019-01-31"
#> [21] "2019-02-01" "2019-02-02" "2019-02-03" "2019-02-04"

# The sequence is generated at the precision of `x`
x <- as_naive_time(c(
  year_month_day(2019, 1, 1, 5),
  year_month_day(2019, 1, 2, 10),
  year_month_day(2019, 1, 1, 3)
))
time_point_spanning_seq(x)
#> <naive_time<hour>[32]>
#>  [1] "2019-01-01T03" "2019-01-01T04" "2019-01-01T05" "2019-01-01T06"
#>  [5] "2019-01-01T07" "2019-01-01T08" "2019-01-01T09" "2019-01-01T10"
#>  [9] "2019-01-01T11" "2019-01-01T12" "2019-01-01T13" "2019-01-01T14"
#> [13] "2019-01-01T15" "2019-01-01T16" "2019-01-01T17" "2019-01-01T18"
#> [17] "2019-01-01T19" "2019-01-01T20" "2019-01-01T21" "2019-01-01T22"
#> [21] "2019-01-01T23" "2019-01-02T00" "2019-01-02T01" "2019-01-02T02"
#> [25] "2019-01-02T03" "2019-01-02T04" "2019-01-02T05" "2019-01-02T06"
#> [29] "2019-01-02T07" "2019-01-02T08" "2019-01-02T09" "2019-01-02T10"
```
