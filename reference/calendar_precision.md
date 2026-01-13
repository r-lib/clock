# Precision: calendar

`calendar_precision()` extracts the precision from a calendar object. It
returns the precision as a single string.

## Usage

``` r
calendar_precision(x)
```

## Arguments

- x:

  `[clock_calendar]`

  A calendar.

## Value

A single string holding the precision of the calendar.

## Examples

``` r
calendar_precision(year_month_day(2019))
#> [1] "year"
calendar_precision(year_month_day(2019, 1, 1))
#> [1] "day"
calendar_precision(year_quarter_day(2019, 3))
#> [1] "quarter"
```
