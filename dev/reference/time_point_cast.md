# Cast a time point between precisions

Casting is one way to change a time point's precision.

Casting to a less precise precision will completely drop information
that is more precise than the precision that you are casting to. It does
so in a way that makes it round towards zero. When converting time
points to a less precise precision, you often want
[`time_point_floor()`](https://clock.r-lib.org/dev/reference/time-point-rounding.md)
instead of `time_point_cast()`, as that handles pre-1970 dates (which
are stored as negative durations) in a more intuitive manner.

Casting to a more precise precision is done through a multiplication by
a conversion factor between the current precision and the new precision.

## Usage

``` r
time_point_cast(x, precision)
```

## Arguments

- x:

  `[clock_sys_time / clock_naive_time]`

  A sys-time or naive-time.

- precision:

  `[character(1)]`

  A time point precision. One of:

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

  - `"millisecond"`

  - `"microsecond"`

  - `"nanosecond"`

## Value

`x` cast to the new `precision`.

## Examples

``` r
# Hour precision time points
# One is pre-1970, one is post-1970
x <- duration_hours(c(25, -25))
x <- as_naive_time(x)
x
#> <naive_time<hour>[2]>
#> [1] "1970-01-02T01" "1969-12-30T23"

# Casting rounds the underlying duration towards 0
cast <- time_point_cast(x, "day")
cast
#> <naive_time<day>[2]>
#> [1] "1970-01-02" "1969-12-31"

# Flooring rounds the underlying duration towards negative infinity,
# which is often more intuitive for time points.
# Note that the cast ends up rounding the pre-1970 date up to the next
# day, while the post-1970 date is rounded down.
floor <- time_point_floor(x, "day")
floor
#> <naive_time<day>[2]>
#> [1] "1970-01-02" "1969-12-30"

# Casting to a more precise precision, hour->millisecond
time_point_cast(x, "millisecond")
#> <naive_time<millisecond>[2]>
#> [1] "1970-01-02T01:00:00.000" "1969-12-30T23:00:00.000"
```
