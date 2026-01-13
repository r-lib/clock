# Widen: year-month-weekday

This is a year-month-weekday method for the
[`calendar_widen()`](https://clock.r-lib.org/dev/reference/calendar_widen.md)
generic. It widens a year-month-weekday vector to the specified
`precision`.

## Usage

``` r
# S3 method for class 'clock_year_month_weekday'
calendar_widen(x, precision)
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

  - `"day"`

  - `"hour"`

  - `"minute"`

  - `"second"`

  - `"millisecond"`

  - `"microsecond"`

  - `"nanosecond"`

## Value

`x` widened to the supplied `precision`.

## Details

Widening a month precision year-month-weekday to day precision will set
the day and the index to `1`. This sets the weekday components to the
first Sunday of the month.

## Examples

``` r
# Month precision
x <- year_month_weekday(2019, 1)
x
#> <year_month_weekday<month>[1]>
#> [1] "2019-01"

# Widen to day precision
# Note that this sets both the day and index to 1,
# i.e. the first Sunday of the month.
calendar_widen(x, "day")
#> <year_month_weekday<day>[1]>
#> [1] "2019-01-Sun[1]"

# Or second precision
sec <- calendar_widen(x, "second")
sec
#> <year_month_weekday<second>[1]>
#> [1] "2019-01-Sun[1]T00:00:00"
```
