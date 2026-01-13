# Is `x` a naive-time?

This function determines if the input is a naive-time object.

## Usage

``` r
is_naive_time(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

`TRUE` if `x` inherits from `"clock_naive_time"`, otherwise `FALSE`.

## Examples

``` r
is_naive_time(1)
#> [1] FALSE
is_naive_time(as_naive_time(duration_days(1)))
#> [1] TRUE
```
