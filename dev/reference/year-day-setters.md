# Setters: year-day

These are year-day methods for the [setter
generics](https://clock.r-lib.org/dev/reference/clock-setters.md).

- [`set_year()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the Gregorian year.

- [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the day of the year. Valid values are in the range of `[1, 366]`.

- There are sub-daily setters for setting more precise components.

## Usage

``` r
# S3 method for class 'clock_year_day'
set_year(x, value, ...)

# S3 method for class 'clock_year_day'
set_day(x, value, ...)

# S3 method for class 'clock_year_day'
set_hour(x, value, ...)

# S3 method for class 'clock_year_day'
set_minute(x, value, ...)

# S3 method for class 'clock_year_day'
set_second(x, value, ...)

# S3 method for class 'clock_year_day'
set_millisecond(x, value, ...)

# S3 method for class 'clock_year_day'
set_microsecond(x, value, ...)

# S3 method for class 'clock_year_day'
set_nanosecond(x, value, ...)
```

## Arguments

- x:

  `[clock_year_day]`

  A year-day vector.

- value:

  `[integer / "last"]`

  The value to set the component to.

  For
  [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md),
  this can also be `"last"` to set the day to the last day of the year.

- ...:

  These dots are for future extensions and must be empty.

## Value

`x` with the component set.

## Examples

``` r
x <- year_day(2019)

# Set the day
set_day(x, 12:14)
#> <year_day<day>[3]>
#> [1] "2019-012" "2019-013" "2019-014"

# Set to the "last" day of the year
set_day(x, "last")
#> <year_day<day>[1]>
#> [1] "2019-365"

# Set to an invalid day of the year
invalid <- set_day(x, 366)
invalid
#> <year_day<day>[1]>
#> [1] "2019-366"

# Then resolve the invalid day by choosing the next valid day
invalid_resolve(invalid, invalid = "next")
#> <year_day<day>[1]>
#> [1] "2020-001"

# Cannot set a component two levels more precise than where you currently are
try(set_hour(x, 5))
#> Error in set_hour(x, 5) : 
#>   Can't perform this operation because of the precision of `x`.
#> ℹ The precision of `x` must be at least "day".
#> ℹ `x` has a precision of "year".
```
