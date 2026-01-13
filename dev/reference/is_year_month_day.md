# Is `x` a year-month-day?

Check if `x` is a year-month-day.

## Usage

``` r
is_year_month_day(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

Returns `TRUE` if `x` inherits from `"clock_year_month_day"`, otherwise
returns `FALSE`.

## Examples

``` r
is_year_month_day(year_month_day(2019))
#> [1] TRUE
```
