# Cast a duration between precisions

Casting is one way to change a duration's precision.

Casting to a less precise precision will completely drop information
that is more precise than the precision that you are casting to. It does
so in a way that makes it round towards zero.

Casting to a more precise precision is done through a multiplication by
a conversion factor between the current precision and the new precision.

## Usage

``` r
duration_cast(x, precision)
```

## Arguments

- x:

  `[clock_duration]`

  A duration.

- precision:

  `[character(1)]`

  A precision. One of:

  - `"year"`

  - `"quarter"`

  - `"month"`

  - `"week"`

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

  - `"millisecond"`

  - `"microsecond"`

  - `"nanosecond"`

## Value

`x` cast to the new `precision`.

## Details

When you want to change to a less precise precision, you often want
[`duration_floor()`](https://clock.r-lib.org/reference/duration-rounding.md)
instead of `duration_cast()`, as that rounds towards negative infinity,
which is generally the desired behavior when working with time points
(especially ones pre-1970, which are stored as negative durations).

## Examples

``` r
x <- duration_seconds(c(86401, -86401))

# Casting rounds towards 0
cast <- duration_cast(x, "day")
cast
#> <duration<day>[2]>
#> [1] 1  -1

# Flooring rounds towards negative infinity
floor <- duration_floor(x, "day")
floor
#> <duration<day>[2]>
#> [1] 1  -2

# Flooring is generally more useful when working with time points,
# note that the cast ends up rounding the pre-1970 date up to the next
# day, while the post-1970 date is rounded down.
as_sys_time(x)
#> <sys_time<second>[2]>
#> [1] "1970-01-02T00:00:01" "1969-12-30T23:59:59"
as_sys_time(cast)
#> <sys_time<day>[2]>
#> [1] "1970-01-02" "1969-12-31"
as_sys_time(floor)
#> <sys_time<day>[2]>
#> [1] "1970-01-02" "1969-12-30"

# Casting to a more precise precision
duration_cast(x, "millisecond")
#> <duration<millisecond>[2]>
#> [1] 86401000  -86401000
```
