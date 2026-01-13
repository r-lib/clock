# Is `x` a year-month-weekday?

Check if `x` is a year-month-weekday.

## Usage

``` r
is_year_month_weekday(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

Returns `TRUE` if `x` inherits from `"clock_year_month_weekday"`,
otherwise returns `FALSE`.

## Examples

``` r
is_year_month_weekday(year_month_weekday(2019))
#> [1] TRUE
```
