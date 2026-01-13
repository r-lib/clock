# Getters: year-quarter-day

These are year-quarter-day methods for the [getter
generics](https://clock.r-lib.org/dev/reference/clock-getters.md).

- [`get_year()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the fiscal year. Note that this can differ from the Gregorian
  year if `start != 1L`.

- [`get_quarter()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the fiscal quarter as a value between 1-4.

- [`get_day()`](https://clock.r-lib.org/dev/reference/clock-getters.md)
  returns the day of the fiscal quarter as a value between 1-92.

- There are sub-daily getters for extracting more precise components.

## Usage

``` r
# S3 method for class 'clock_year_quarter_day'
get_year(x)

# S3 method for class 'clock_year_quarter_day'
get_quarter(x)

# S3 method for class 'clock_year_quarter_day'
get_day(x)

# S3 method for class 'clock_year_quarter_day'
get_hour(x)

# S3 method for class 'clock_year_quarter_day'
get_minute(x)

# S3 method for class 'clock_year_quarter_day'
get_second(x)

# S3 method for class 'clock_year_quarter_day'
get_millisecond(x)

# S3 method for class 'clock_year_quarter_day'
get_microsecond(x)

# S3 method for class 'clock_year_quarter_day'
get_nanosecond(x)
```

## Arguments

- x:

  `[clock_year_quarter_day]`

  A year-quarter-day to get the component from.

## Value

The component.

## Examples

``` r
x <- year_quarter_day(2020, 1:4)

get_quarter(x)
#> [1] 1 2 3 4

# Set and then get the last day of the quarter
x <- set_day(x, "last")
get_day(x)
#> [1] 91 91 92 92

# Start the fiscal year in November and choose the 50th day in
# each quarter of 2020
november <- 11
y <- year_quarter_day(2020, 1:4, 50, start = 11)
y
#> <year_quarter_day<November><day>[4]>
#> [1] "2020-Q1-50" "2020-Q2-50" "2020-Q3-50" "2020-Q4-50"

get_day(y)
#> [1] 50 50 50 50

# What does that map to in year-month-day?
as_year_month_day(y)
#> <year_month_day<day>[4]>
#> [1] "2019-12-20" "2020-03-21" "2020-06-19" "2020-09-19"
```
