# Getters: year-month-weekday

These are year-month-weekday methods for the [getter
generics](https://clock.r-lib.org/dev/reference/clock-getters.md).

- [`get_year()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the Gregorian year.

- [`get_month()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the month of the year.

- [`get_day()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the day of the week encoded from 1-7, where 1 = Sunday and 7 =
  Saturday.

- [`get_index()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns a value from 1-5 indicating that the corresponding weekday is
  the n-th instance of that weekday in the current month.

- There are sub-daily getters for extracting more precise components.

## Usage

``` r
# S3 method for class 'clock_year_month_weekday'
get_year(x)

# S3 method for class 'clock_year_month_weekday'
get_month(x)

# S3 method for class 'clock_year_month_weekday'
get_day(x)

# S3 method for class 'clock_year_month_weekday'
get_index(x)

# S3 method for class 'clock_year_month_weekday'
get_hour(x)

# S3 method for class 'clock_year_month_weekday'
get_minute(x)

# S3 method for class 'clock_year_month_weekday'
get_second(x)

# S3 method for class 'clock_year_month_weekday'
get_millisecond(x)

# S3 method for class 'clock_year_month_weekday'
get_microsecond(x)

# S3 method for class 'clock_year_month_weekday'
get_nanosecond(x)
```

## Arguments

- x:

  `[clock_year_month_weekday]`

  A year-month-weekday to get the component from.

## Value

The component.

## Examples

``` r
monday <- clock_weekdays$monday
thursday <- clock_weekdays$thursday

x <- year_month_weekday(2019, 1, monday:thursday, 1:4)
x
#> <year_month_weekday<day>[4]>
#> [1] "2019-01-Mon[1]" "2019-01-Tue[2]" "2019-01-Wed[3]" "2019-01-Thu[4]"

# Gets the weekday, 1 = Sunday, 7 = Saturday
get_day(x)
#> [1] 2 3 4 5

# Gets the index indicating which instance of that particular weekday
# it is in the current month (i.e. the "1st Monday of January, 2019")
get_index(x)
#> [1] 1 2 3 4
```
