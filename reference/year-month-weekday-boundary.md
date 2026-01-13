# Boundaries: year-month-weekday

This is a year-month-weekday method for the
[`calendar_start()`](https://clock.r-lib.org/reference/calendar-boundary.md)
and
[`calendar_end()`](https://clock.r-lib.org/reference/calendar-boundary.md)
generics. They adjust components of a calendar to the start or end of a
specified `precision`.

This method is restricted to only `"year"` and `"month"` `precision`s,
and `x` can't be more precise than month precision. Computing the
"start" of a day precision year-month-weekday object isn't defined
because a year-month-weekday with `day = 1, index = 1` doesn't
necessarily occur earlier (chronologically) than `day = 2, index = 1`.
Because of these restrictions, this method isn't particularly useful,
but is included for completeness.

## Usage

``` r
# S3 method for class 'clock_year_month_weekday'
calendar_start(x, precision)

# S3 method for class 'clock_year_month_weekday'
calendar_end(x, precision)
```

## Arguments

- x:

  `[clock_year_month_weekday]`

  A year-month-weekday vector.

- precision:

  `[character(1)]`

  One of:

  - `"year"`

  - `"month"`

## Value

`x` at the same precision, but with some components altered to be at the
boundary value.

## Examples

``` r
# Month precision
x <- year_month_weekday(2019, 1:5)
x
#> <year_month_weekday<month>[5]>
#> [1] "2019-01" "2019-02" "2019-03" "2019-04" "2019-05"

# Compute the last month of the year
calendar_end(x, "year")
#> <year_month_weekday<month>[5]>
#> [1] "2019-12" "2019-12" "2019-12" "2019-12" "2019-12"
```
