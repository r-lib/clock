# Getters: date

These are Date methods for the [getter
generics](https://clock.r-lib.org/reference/clock-getters.md).

- [`get_year()`](https://clock.r-lib.org/reference/clock-getters.md)
  returns the Gregorian year.

- [`get_month()`](https://clock.r-lib.org/reference/clock-getters.md)
  returns the month of the year.

- [`get_day()`](https://clock.r-lib.org/reference/clock-getters.md)
  returns the day of the month.

For more advanced component extraction, convert to the calendar type
that you are interested in.

## Usage

``` r
# S3 method for class 'Date'
get_year(x)

# S3 method for class 'Date'
get_month(x)

# S3 method for class 'Date'
get_day(x)
```

## Arguments

- x:

  `[Date]`

  A Date to get the component from.

## Value

The component.

## Examples

``` r
x <- as.Date("2019-01-01") + 0:5
get_day(x)
#> [1] 1 2 3 4 5 6
```
