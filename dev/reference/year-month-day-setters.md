# Setters: year-month-day

These are year-month-day methods for the [setter
generics](https://clock.r-lib.org/dev/reference/clock-setters.md).

- [`set_year()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the Gregorian year.

- [`set_month()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the month of the year. Valid values are in the range of
  `[1, 12]`.

- [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the day of the month. Valid values are in the range of `[1, 31]`.

- There are sub-daily setters for setting more precise components.

## Usage

``` r
# S3 method for class 'clock_year_month_day'
set_year(x, value, ...)

# S3 method for class 'clock_year_month_day'
set_month(x, value, ...)

# S3 method for class 'clock_year_month_day'
set_day(x, value, ...)

# S3 method for class 'clock_year_month_day'
set_hour(x, value, ...)

# S3 method for class 'clock_year_month_day'
set_minute(x, value, ...)

# S3 method for class 'clock_year_month_day'
set_second(x, value, ...)

# S3 method for class 'clock_year_month_day'
set_millisecond(x, value, ...)

# S3 method for class 'clock_year_month_day'
set_microsecond(x, value, ...)

# S3 method for class 'clock_year_month_day'
set_nanosecond(x, value, ...)
```

## Arguments

- x:

  `[clock_year_month_day]`

  A year-month-day vector.

- value:

  `[integer / "last"]`

  The value to set the component to.

  For
  [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md),
  this can also be `"last"` to set the day to the last day of the month.

- ...:

  These dots are for future extensions and must be empty.

## Value

`x` with the component set.

## Examples

``` r
x <- year_month_day(2019, 1:3)

# Set the day
set_day(x, 12:14)
#> <year_month_day<day>[3]>
#> [1] "2019-01-12" "2019-02-13" "2019-03-14"

# Set to the "last" day of the month
set_day(x, "last")
#> <year_month_day<day>[3]>
#> [1] "2019-01-31" "2019-02-28" "2019-03-31"

# Set to an invalid day of the month
invalid <- set_day(x, 31)
invalid
#> <year_month_day<day>[3]>
#> [1] "2019-01-31" "2019-02-31" "2019-03-31"

# Then resolve the invalid day by choosing the next valid day
invalid_resolve(invalid, invalid = "next")
#> <year_month_day<day>[3]>
#> [1] "2019-01-31" "2019-03-01" "2019-03-31"

# Cannot set a component two levels more precise than where you currently are
try(set_hour(x, 5))
#> Error in set_hour(x, 5) : 
#>   Can't perform this operation because of the precision of `x`.
#> ℹ The precision of `x` must be at least "day".
#> ℹ `x` has a precision of "month".
```
