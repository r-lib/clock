# Spanning sequence: duration

`duration_spanning_seq()` generates a regular sequence along the span of
`x`, i.e. along `[min(x), max(x)]`.

## Usage

``` r
duration_spanning_seq(x)
```

## Arguments

- x:

  `[clock_duration]`

  A duration vector.

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
x <- duration_days(c(1, 5, 2))
duration_spanning_seq(x)
#> <duration<day>[5]>
#> [1] 1 2 3 4 5

# Missing values are removed before the sequence is created
x <- vctrs::vec_c(NA, x, NA)
duration_spanning_seq(x)
#> <duration<day>[5]>
#> [1] 1 2 3 4 5
```
