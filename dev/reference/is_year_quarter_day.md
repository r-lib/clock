# Is `x` a year-quarter-day?

Check if `x` is a year-quarter-day.

## Usage

``` r
is_year_quarter_day(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

Returns `TRUE` if `x` inherits from `"clock_year_quarter_day"`,
otherwise returns `FALSE`.

## Examples

``` r
is_year_quarter_day(year_quarter_day(2019))
#> [1] TRUE
```
