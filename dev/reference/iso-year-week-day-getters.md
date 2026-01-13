# Getters: iso-year-week-day

These are iso-year-week-day methods for the [getter
generics](https://clock.r-lib.org/dev/reference/clock-getters.md).

- [`get_year()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the ISO year. Note that this can differ from the Gregorian
  year.

- [`get_week()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the ISO week of the current ISO year.

- [`get_day()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns a value between 1-7 indicating the weekday of the current ISO
  week, where 1 = Monday and 7 = Sunday, in line with the ISO standard.

- There are sub-daily getters for extracting more precise components.

## Usage

``` r
# S3 method for class 'clock_iso_year_week_day'
get_year(x)

# S3 method for class 'clock_iso_year_week_day'
get_week(x)

# S3 method for class 'clock_iso_year_week_day'
get_day(x)

# S3 method for class 'clock_iso_year_week_day'
get_hour(x)

# S3 method for class 'clock_iso_year_week_day'
get_minute(x)

# S3 method for class 'clock_iso_year_week_day'
get_second(x)

# S3 method for class 'clock_iso_year_week_day'
get_millisecond(x)

# S3 method for class 'clock_iso_year_week_day'
get_microsecond(x)

# S3 method for class 'clock_iso_year_week_day'
get_nanosecond(x)
```

## Arguments

- x:

  `[clock_iso_year_week_day]`

  A iso-year-week-day to get the component from.

## Value

The component.

## Examples

``` r
x <- iso_year_week_day(2019, 50:52, 1:3)
x
#> <iso_year_week_day<day>[3]>
#> [1] "2019-W50-1" "2019-W51-2" "2019-W52-3"

# Get the ISO week
get_week(x)
#> [1] 50 51 52

# Gets the weekday, 1 = Monday, 7 = Sunday
get_day(x)
#> [1] 1 2 3

# Note that the ISO year can differ from the Gregorian year
iso <- iso_year_week_day(2019, 1, 1)
ymd <- as_year_month_day(iso)

get_year(iso)
#> [1] 2019
get_year(ymd)
#> [1] 2018
```
