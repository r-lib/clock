# Precision: zoned-time

`zoned_time_precision()` extracts the precision from a zoned-time. It
returns the precision as a single string.

## Usage

``` r
zoned_time_precision(x)
```

## Arguments

- x:

  `[clock_zoned_time]`

  A zoned-time.

## Value

A single string holding the precision of the zoned-time.

## Examples

``` r
zoned_time_precision(zoned_time_now("America/New_York"))
#> [1] "nanosecond"
```
