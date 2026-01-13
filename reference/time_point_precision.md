# Precision: time point

`time_point_precision()` extracts the precision from a time point, such
as a sys-time or naive-time. It returns the precision as a single
string.

## Usage

``` r
time_point_precision(x)
```

## Arguments

- x:

  `[clock_time_point]`

  A time point.

## Value

A single string holding the precision of the time point.

## Examples

``` r
time_point_precision(sys_time_now())
#> [1] "nanosecond"
time_point_precision(as_naive_time(duration_days(1)))
#> [1] "day"
```
