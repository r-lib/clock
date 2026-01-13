# Setters: date

These are Date methods for the [setter
generics](https://clock.r-lib.org/reference/clock-setters.md).

- [`set_year()`](https://clock.r-lib.org/reference/clock-setters.md)
  sets the year.

- [`set_month()`](https://clock.r-lib.org/reference/clock-setters.md)
  sets the month of the year. Valid values are in the range of
  `[1, 12]`.

- [`set_day()`](https://clock.r-lib.org/reference/clock-setters.md) sets
  the day of the month. Valid values are in the range of `[1, 31]`.

## Usage

``` r
# S3 method for class 'Date'
set_year(x, value, ..., invalid = NULL)

# S3 method for class 'Date'
set_month(x, value, ..., invalid = NULL)

# S3 method for class 'Date'
set_day(x, value, ..., invalid = NULL)
```

## Arguments

- x:

  `[Date]`

  A Date vector.

- value:

  `[integer / "last"]`

  The value to set the component to.

  For [`set_day()`](https://clock.r-lib.org/reference/clock-setters.md),
  this can also be `"last"` to set the day to the last day of the month.

- ...:

  These dots are for future extensions and must be empty.

- invalid:

  `[character(1) / NULL]`

  One of the following invalid date resolution strategies:

  - `"previous"`: The previous valid instant in time.

  - `"previous-day"`: The previous valid day in time, keeping the time
    of day.

  - `"next"`: The next valid instant in time.

  - `"next-day"`: The next valid day in time, keeping the time of day.

  - `"overflow"`: Overflow by the number of days that the input is
    invalid by. Time of day is dropped.

  - `"overflow-day"`: Overflow by the number of days that the input is
    invalid by. Time of day is kept.

  - `"NA"`: Replace invalid dates with `NA`.

  - `"error"`: Error on invalid dates.

  Using either `"previous"` or `"next"` is generally recommended, as
  these two strategies maintain the *relative ordering* between elements
  of the input.

  If `NULL`, defaults to `"error"`.

  If `getOption("clock.strict")` is `TRUE`, `invalid` must be supplied
  and cannot be `NULL`. This is a convenient way to make production code
  robust to invalid dates.

## Value

`x` with the component set.

## Examples

``` r
x <- as.Date("2019-02-01")

# Set the day
set_day(x, 12:14)
#> [1] "2019-02-12" "2019-02-13" "2019-02-14"

# Set to the "last" day of the month
set_day(x, "last")
#> [1] "2019-02-28"

# You cannot set a Date to an invalid day like you can with
# a year-month-day. Instead, the default strategy is to error.
try(set_day(x, 31))
#> Error in invalid_resolve(x, invalid = invalid) : 
#>   Invalid date found at location 1.
#> ℹ Resolve invalid date issues by specifying the `invalid` argument.
set_day(as_year_month_day(x), 31)
#> <year_month_day<day>[1]>
#> [1] "2019-02-31"

# You can resolve these issues while setting the day by specifying
# an invalid date resolution strategy with `invalid`
set_day(x, 31, invalid = "previous")
#> [1] "2019-02-28"
```
