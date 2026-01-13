# Getters: date-time

These are POSIXct/POSIXlt methods for the [getter
generics](https://clock.r-lib.org/reference/clock-getters.md).

- [`get_year()`](https://clock.r-lib.org/reference/clock-getters.md)
  returns the Gregorian year.

- [`get_month()`](https://clock.r-lib.org/reference/clock-getters.md)
  returns the month of the year.

- [`get_day()`](https://clock.r-lib.org/reference/clock-getters.md)
  returns the day of the month.

- There are sub-daily getters for extracting more precise components, up
  to a precision of seconds.

For more advanced component extraction, convert to the calendar type
that you are interested in.

## Usage

``` r
# S3 method for class 'POSIXt'
get_year(x)

# S3 method for class 'POSIXt'
get_month(x)

# S3 method for class 'POSIXt'
get_day(x)

# S3 method for class 'POSIXt'
get_hour(x)

# S3 method for class 'POSIXt'
get_minute(x)

# S3 method for class 'POSIXt'
get_second(x)
```

## Arguments

- x:

  `[POSIXct / POSIXlt]`

  A date-time type to get the component from.

## Value

The component.

## Examples

``` r
x <- as.POSIXct("2019-01-01", tz = "America/New_York")

x <- add_days(x, 0:5)
x <- set_second(x, 10:15)

get_day(x)
#> [1] 1 2 3 4 5 6
get_second(x)
#> [1] 10 11 12 13 14 15
```
