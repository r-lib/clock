# Getters: year-month-day

These are year-month-day methods for the [getter
generics](https://clock.r-lib.org/reference/clock-getters.md).

- [`get_year()`](https://clock.r-lib.org/reference/clock-getters.md)
  returns the Gregorian year.

- [`get_month()`](https://clock.r-lib.org/reference/clock-getters.md)
  returns the month of the year.

- [`get_day()`](https://clock.r-lib.org/reference/clock-getters.md)
  returns the day of the month.

- There are sub-daily getters for extracting more precise components.

## Usage

``` r
# S3 method for class 'clock_year_month_day'
get_year(x)

# S3 method for class 'clock_year_month_day'
get_month(x)

# S3 method for class 'clock_year_month_day'
get_day(x)

# S3 method for class 'clock_year_month_day'
get_hour(x)

# S3 method for class 'clock_year_month_day'
get_minute(x)

# S3 method for class 'clock_year_month_day'
get_second(x)

# S3 method for class 'clock_year_month_day'
get_millisecond(x)

# S3 method for class 'clock_year_month_day'
get_microsecond(x)

# S3 method for class 'clock_year_month_day'
get_nanosecond(x)
```

## Arguments

- x:

  `[clock_year_month_day]`

  A year-month-day to get the component from.

## Value

The component.

## Examples

``` r
x <- year_month_day(2019, 1:3, 5:7, 1, 20, 30)

get_month(x)
#> [1] 1 2 3
get_day(x)
#> [1] 5 6 7
get_second(x)
#> [1] 30 30 30

# Cannot extract more precise components
y <- year_month_day(2019, 1)
try(get_day(y))
#> Error in get_day(y) : 
#>   Can't perform this operation because of the precision of `x`.
#> ℹ The precision of `x` must be at least "day".
#> ℹ `x` has a precision of "month".

# Cannot extract components that don't exist for this calendar
try(get_quarter(x))
#> Error in get_quarter(x) : 
#>   Can't perform this operation on a <clock_year_month_day>.
```
