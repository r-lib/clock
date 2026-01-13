# Getters: year-day

These are year-day methods for the [getter
generics](https://clock.r-lib.org/dev/reference/clock-getters.md).

- [`get_year()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the Gregorian year.

- [`get_day()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the day of the year.

- There are sub-daily getters for extracting more precise components.

## Usage

``` r
# S3 method for class 'clock_year_day'
get_year(x)

# S3 method for class 'clock_year_day'
get_day(x)

# S3 method for class 'clock_year_day'
get_hour(x)

# S3 method for class 'clock_year_day'
get_minute(x)

# S3 method for class 'clock_year_day'
get_second(x)

# S3 method for class 'clock_year_day'
get_millisecond(x)

# S3 method for class 'clock_year_day'
get_microsecond(x)

# S3 method for class 'clock_year_day'
get_nanosecond(x)
```

## Arguments

- x:

  `[clock_year_day]`

  A year-day to get the component from.

## Value

The component.

## Examples

``` r
x <- year_day(2019, 101:105, 1, 20, 30)

get_day(x)
#> [1] 101 102 103 104 105
get_second(x)
#> [1] 30 30 30 30 30

# Cannot extract more precise components
y <- year_day(2019, 1)
try(get_hour(y))
#> Error in get_hour(y) : 
#>   Can't perform this operation because of the precision of `x`.
#> ℹ The precision of `x` must be at least "hour".
#> ℹ `x` has a precision of "day".

# Cannot extract components that don't exist for this calendar
try(get_quarter(x))
#> Error in get_quarter(x) : 
#>   Can't perform this operation on a <clock_year_day>.
```
