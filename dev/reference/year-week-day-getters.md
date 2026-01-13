# Getters: year-week-day

These are year-week-day methods for the [getter
generics](https://clock.r-lib.org/dev/reference/clock-getters.md).

- [`get_year()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the year. Note that this can differ from the Gregorian year.

- [`get_week()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the week of the current year.

- [`get_day()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns a value between 1-7 indicating the weekday of the current
  week, where `1 = start of week` and `7 = end of week`, in line with
  the chosen `start`.

- There are sub-daily getters for extracting more precise components.

## Usage

``` r
# S3 method for class 'clock_year_week_day'
get_year(x)

# S3 method for class 'clock_year_week_day'
get_week(x)

# S3 method for class 'clock_year_week_day'
get_day(x)

# S3 method for class 'clock_year_week_day'
get_hour(x)

# S3 method for class 'clock_year_week_day'
get_minute(x)

# S3 method for class 'clock_year_week_day'
get_second(x)

# S3 method for class 'clock_year_week_day'
get_millisecond(x)

# S3 method for class 'clock_year_week_day'
get_microsecond(x)

# S3 method for class 'clock_year_week_day'
get_nanosecond(x)
```

## Arguments

- x:

  `[clock_year_week_day]`

  A year-week-day to get the component from.

## Value

The component.

## Examples

``` r
x <- year_week_day(2019, 50:52, 1:3)
x
#> <year_week_day<Sunday><day>[3]>
#> [1] "2019-W50-1" "2019-W51-2" "2019-W52-3"

# Get the week
get_week(x)
#> [1] 50 51 52

# Gets the weekday
get_day(x)
#> [1] 1 2 3

# Note that the year can differ from the Gregorian year
iso <- year_week_day(2019, 1, 1, start = clock_weekdays$monday)
ymd <- as_year_month_day(iso)

get_year(iso)
#> [1] 2019
get_year(ymd)
#> [1] 2018
```
