# Is `x` a duration?

This function determines if the input is a duration object.

## Usage

``` r
is_duration(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

`TRUE` if `x` inherits from `"clock_duration"`, otherwise `FALSE`.

## Examples

``` r
is_duration(1)
#> [1] FALSE
is_duration(duration_days(1))
#> [1] TRUE
```
