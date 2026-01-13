# Is `x` a year-day?

Check if `x` is a year-day.

## Usage

``` r
is_year_day(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

Returns `TRUE` if `x` inherits from `"clock_year_day"`, otherwise
returns `FALSE`.

## Examples

``` r
is_year_day(year_day(2019))
#> [1] TRUE
```
