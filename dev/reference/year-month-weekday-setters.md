# Setters: year-month-weekday

These are year-month-weekday methods for the [setter
generics](https://clock.r-lib.org/dev/reference/clock-setters.md).

- [`set_year()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the Gregorian year.

- [`set_month()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the month of the year. Valid values are in the range of
  `[1, 12]`.

- [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the day of the week. Valid values are in the range of `[1, 7]`,
  with 1 = Sunday, and 7 = Saturday.

- [`set_index()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the index indicating that the corresponding weekday is the n-th
  instance of that weekday in the current month. Valid values are in the
  range of `[1, 5]`.

- There are sub-daily setters for setting more precise components.

## Usage

``` r
# S3 method for class 'clock_year_month_weekday'
set_year(x, value, ...)

# S3 method for class 'clock_year_month_weekday'
set_month(x, value, ...)

# S3 method for class 'clock_year_month_weekday'
set_day(x, value, ..., index = NULL)

# S3 method for class 'clock_year_month_weekday'
set_index(x, value, ...)

# S3 method for class 'clock_year_month_weekday'
set_hour(x, value, ...)

# S3 method for class 'clock_year_month_weekday'
set_minute(x, value, ...)

# S3 method for class 'clock_year_month_weekday'
set_second(x, value, ...)

# S3 method for class 'clock_year_month_weekday'
set_millisecond(x, value, ...)

# S3 method for class 'clock_year_month_weekday'
set_microsecond(x, value, ...)

# S3 method for class 'clock_year_month_weekday'
set_nanosecond(x, value, ...)
```

## Arguments

- x:

  `[clock_year_month_weekday]`

  A year-month-weekday vector.

- value:

  `[integer / "last"]`

  The value to set the component to.

  For
  [`set_index()`](https://clock.r-lib.org/dev/reference/clock-setters.md),
  this can also be `"last"` to adjust to the last instance of the
  corresponding weekday in that month.

- ...:

  These dots are for future extensions and must be empty.

- index:

  `[NULL / integer / "last"]`

  This argument is only used with
  [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md),
  and allows you to set the index while also setting the weekday.

  If `x` is a month precision year-month-weekday, `index` is required to
  be set, as you must specify the weekday and the index simultaneously
  to promote from month to day precision.

## Value

`x` with the component set.

## Examples

``` r
x <- year_month_weekday(2019, 1:3)

set_year(x, 2020:2022)
#> <year_month_weekday<month>[3]>
#> [1] "2020-01" "2021-02" "2022-03"

# Setting the weekday on a month precision year-month-weekday requires
# also setting the `index` to fully specify the day information
x <- set_day(x, clock_weekdays$sunday, index = 1)
x
#> <year_month_weekday<day>[3]>
#> [1] "2019-01-Sun[1]" "2019-02-Sun[1]" "2019-03-Sun[1]"

# Once you have at least day precision, you can set the weekday and
# the index separately
set_day(x, clock_weekdays$monday)
#> <year_month_weekday<day>[3]>
#> [1] "2019-01-Mon[1]" "2019-02-Mon[1]" "2019-03-Mon[1]"
set_index(x, 3)
#> <year_month_weekday<day>[3]>
#> [1] "2019-01-Sun[3]" "2019-02-Sun[3]" "2019-03-Sun[3]"

# Set to the "last" instance of the corresponding weekday in this month
# (Note that some months have 4 Sundays, and others have 5)
set_index(x, "last")
#> <year_month_weekday<day>[3]>
#> [1] "2019-01-Sun[4]" "2019-02-Sun[4]" "2019-03-Sun[5]"

# Set to an invalid index
# January and February of 2019 don't have 5 Sundays!
invalid <- set_index(x, 5)
invalid
#> <year_month_weekday<day>[3]>
#> [1] "2019-01-Sun[5]" "2019-02-Sun[5]" "2019-03-Sun[5]"

# Resolve the invalid dates by choosing the previous/next valid moment
invalid_resolve(invalid, invalid = "previous")
#> <year_month_weekday<day>[3]>
#> [1] "2019-01-Thu[5]" "2019-02-Thu[4]" "2019-03-Sun[5]"
invalid_resolve(invalid, invalid = "next")
#> <year_month_weekday<day>[3]>
#> [1] "2019-02-Fri[1]" "2019-03-Fri[1]" "2019-03-Sun[5]"

# You can also "overflow" the index. This keeps the weekday, but resets
# the index to 1 and increments the month value by 1.
invalid_resolve(invalid, invalid = "overflow")
#> <year_month_weekday<day>[3]>
#> [1] "2019-02-Sun[1]" "2019-03-Sun[1]" "2019-03-Sun[5]"
```
