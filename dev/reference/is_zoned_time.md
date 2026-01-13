# Is `x` a zoned-time?

This function determines if the input is a zoned-time object.

## Usage

``` r
is_zoned_time(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

`TRUE` if `x` inherits from `"clock_zoned_time"`, otherwise `FALSE`.

## Examples

``` r
is_zoned_time(1)
#> [1] FALSE
is_zoned_time(zoned_time_now("America/New_York"))
#> [1] TRUE
```
