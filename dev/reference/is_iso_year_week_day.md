# Is `x` a iso-year-week-day?

Check if `x` is a iso-year-week-day.

## Usage

``` r
is_iso_year_week_day(x)
```

## Arguments

- x:

  `[object]`

  An object.

## Value

Returns `TRUE` if `x` inherits from `"clock_iso_year_week_day"`,
otherwise returns `FALSE`.

## Examples

``` r
is_iso_year_week_day(iso_year_week_day(2019))
#> [1] TRUE
```
