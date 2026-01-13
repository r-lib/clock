# Is `x` a sys-time?

This function determines if the input is a sys-time object.

## Usage

``` r
is_sys_time(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

`TRUE` if `x` inherits from `"clock_sys_time"`, otherwise `FALSE`.

## Examples

``` r
is_sys_time(1)
#> [1] FALSE
is_sys_time(as_sys_time(duration_days(1)))
#> [1] TRUE
```
