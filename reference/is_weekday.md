# Is `x` a weekday?

This function determines if the input is a weekday object.

## Usage

``` r
is_weekday(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

`TRUE` if `x` inherits from `"clock_weekday"`, otherwise `FALSE`.

## Examples

``` r
is_weekday(1)
#> [1] FALSE
is_weekday(weekday(1))
#> [1] TRUE
```
