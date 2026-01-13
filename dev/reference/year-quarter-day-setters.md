# Setters: year-quarter-day

These are year-quarter-day methods for the [setter
generics](https://clock.r-lib.org/dev/reference/clock-setters.md).

- [`set_year()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the fiscal year.

- [`set_quarter()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the fiscal quarter of the year. Valid values are in the range of
  `[1, 4]`.

- [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md)
  sets the day of the fiscal quarter. Valid values are in the range of
  `[1, 92]`.

- There are sub-daily setters for setting more precise components.

## Usage

``` r
# S3 method for class 'clock_year_quarter_day'
set_year(x, value, ...)

# S3 method for class 'clock_year_quarter_day'
set_quarter(x, value, ...)

# S3 method for class 'clock_year_quarter_day'
set_day(x, value, ...)

# S3 method for class 'clock_year_quarter_day'
set_hour(x, value, ...)

# S3 method for class 'clock_year_quarter_day'
set_minute(x, value, ...)

# S3 method for class 'clock_year_quarter_day'
set_second(x, value, ...)

# S3 method for class 'clock_year_quarter_day'
set_millisecond(x, value, ...)

# S3 method for class 'clock_year_quarter_day'
set_microsecond(x, value, ...)

# S3 method for class 'clock_year_quarter_day'
set_nanosecond(x, value, ...)
```

## Arguments

- x:

  `[clock_year_quarter_day]`

  A year-quarter-day vector.

- value:

  `[integer / "last"]`

  The value to set the component to.

  For
  [`set_day()`](https://clock.r-lib.org/dev/reference/clock-setters.md),
  this can also be `"last"` to adjust to the last day of the current
  fiscal quarter.

- ...:

  These dots are for future extensions and must be empty.

## Value

`x` with the component set.

## Examples

``` r
library(magrittr)

# Quarter precision vector
x <- year_quarter_day(2019, 1:4)
x
#> <year_quarter_day<January><quarter>[4]>
#> [1] "2019-Q1" "2019-Q2" "2019-Q3" "2019-Q4"

# Promote to day precision by setting the day
x <- set_day(x, 1)
x
#> <year_quarter_day<January><day>[4]>
#> [1] "2019-Q1-01" "2019-Q2-01" "2019-Q3-01" "2019-Q4-01"

# Or set to the last day of the quarter
x <- set_day(x, "last")
x
#> <year_quarter_day<January><day>[4]>
#> [1] "2019-Q1-90" "2019-Q2-91" "2019-Q3-92" "2019-Q4-92"

# What year-month-day is this?
as_year_month_day(x)
#> <year_month_day<day>[4]>
#> [1] "2019-03-31" "2019-06-30" "2019-09-30" "2019-12-31"

# Set to an invalid day of the quarter
# (not all quarters have 92 days)
invalid <- set_day(x, 92)
invalid
#> <year_quarter_day<January><day>[4]>
#> [1] "2019-Q1-92" "2019-Q2-92" "2019-Q3-92" "2019-Q4-92"

# Here are the invalid ones
invalid[invalid_detect(invalid)]
#> <year_quarter_day<January><day>[2]>
#> [1] "2019-Q1-92" "2019-Q2-92"

# Resolve the invalid dates by choosing the previous/next valid moment
invalid_resolve(invalid, invalid = "previous")
#> <year_quarter_day<January><day>[4]>
#> [1] "2019-Q1-90" "2019-Q2-91" "2019-Q3-92" "2019-Q4-92"
invalid_resolve(invalid, invalid = "next")
#> <year_quarter_day<January><day>[4]>
#> [1] "2019-Q2-01" "2019-Q3-01" "2019-Q3-92" "2019-Q4-92"

# Or resolve by "overflowing" by the number of days that you have
# gone past the last valid day
invalid_resolve(invalid, invalid = "overflow")
#> <year_quarter_day<January><day>[4]>
#> [1] "2019-Q2-02" "2019-Q3-01" "2019-Q3-92" "2019-Q4-92"

# This is similar to
days <- get_day(invalid) - 1L
invalid %>%
  set_day(1) %>%
  as_naive_time() %>%
  add_days(days) %>%
  as_year_quarter_day()
#> <year_quarter_day<January><day>[4]>
#> [1] "2019-Q2-02" "2019-Q3-01" "2019-Q3-92" "2019-Q4-92"
```
