# Is `x` a year-week-day?

Check if `x` is a year-week-day.

## Usage

``` r
is_year_week_day(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

Returns `TRUE` if `x` inherits from `"clock_year_week_day"`, otherwise
returns `FALSE`.

## Examples

``` r
is_year_week_day(year_week_day(2019))
#> [1] TRUE
```
