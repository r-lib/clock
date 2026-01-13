# Precision: duration

`duration_precision()` extracts the precision from a duration object. It
returns the precision as a single string.

## Usage

``` r
duration_precision(x)
```

## Arguments

- x:

  `[clock_duration]`

  A duration.

## Value

A single string holding the precision of the duration.

## Examples

``` r
duration_precision(duration_seconds(1))
#> [1] "second"
duration_precision(duration_nanoseconds(2))
#> [1] "nanosecond"
duration_precision(duration_quarters(1:5))
#> [1] "quarter"
```
